#include "pch.h"
#include "Mesh.h"
#include "Effect.h"
#include "Texture.h"


Mesh::Mesh(ID3D11Device* pDevice, const std::vector<Vertex_In>& vertices, const std::vector<uint32_t>& indices, Texture* pTexture)
{
	//Create effect
	m_pEffect = new Effect{ pDevice,L"Resources/PosCol3D.fx" };
	if (m_pEffect)
	{
		m_pEffectLocalPointer = m_pEffect->GetEffect();
		m_pTechniqueLocalPointer = m_pEffect->GetTechnique();

	}

	//Create Vertex Layout
	static constexpr uint32_t numElements{ 2};
	D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

	vertexDesc[0].SemanticName = "POSITION";
	vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[0].AlignedByteOffset = 0;
	vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[1].SemanticName = "TEXCOORD";
	vertexDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexDesc[1].AlignedByteOffset = 12;
	vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	//Create input layout
	D3DX11_PASS_DESC passDesc{};
	m_pTechniqueLocalPointer->GetPassByIndex(0)->GetDesc(&passDesc);

	const HRESULT result = pDevice->CreateInputLayout(
		vertexDesc,
		numElements,
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize,
		&m_pInputLayout);
	
	if (FAILED(result))
		assert(false);

	//Create vertex buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex_In) * static_cast<uint32_t>(vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices.data();

	HRESULT result1 = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(result1))
		assert(false);

	//Create index buffer
	m_NumIndices = static_cast<uint32_t>(indices.size());
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();

	result1 = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);

	if (FAILED(result1))
		assert(false);

	m_pEffect->SetDiffuseMap(pTexture);
}

Mesh::~Mesh()
{
	if (m_pIndexBuffer)
		m_pIndexBuffer->Release();

	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();

	if (m_pInputLayout)
		m_pInputLayout->Release();

	if (m_pEffect)
	{
		delete m_pEffect;
		m_pEffect = nullptr;
	}
}

void Mesh::Render(ID3D11DeviceContext* pDeviceContext)
{
	//1. Set Primitive Topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//2. Set Input Layout
	pDeviceContext->IASetInputLayout(m_pInputLayout);

	//3. Set VertexBuffer
	constexpr UINT stride = sizeof(Vertex_In);
	constexpr UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//4. Set IndexBuffer
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//5. Draw
	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pTechniqueLocalPointer->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; p++)
	{
		m_pTechniqueLocalPointer->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}

}

void Mesh::UpdateWorldViewProjMat(const Camera& camera)
{
	const Matrix worldViewProjection{ m_WorldMatrix * camera.invViewMatrix * camera.projectionMatrix };
	m_pEffect->SetWorldViewProjMatrix(worldViewProjection);
}

void Mesh::CycleFilterMode()
{
	m_pEffect->CycleFilterMode();
	m_pTechniqueLocalPointer = m_pEffect->GetTechnique();
}
