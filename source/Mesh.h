#pragma once
#include "Structs.h"
#include "Camera.h"
using namespace dae;
class Effect;
class Texture;
class Mesh
{
public:
	Mesh(ID3D11Device* pDevice,const std::vector<Vertex_In>& vertices, const std::vector<uint32_t>& indices,Texture* pTexture );
	~Mesh();

	void Render(ID3D11DeviceContext* pDeviceContext);
	void UpdateWorldViewProjMat(const Camera& camera);

private:
	Effect* m_pEffect;

	ID3DX11Effect* m_pEffectLocalPointer;
	ID3DX11EffectTechnique* m_pTechniqueLocalPointer;

	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	uint32_t m_NumIndices;
	Matrix m_WorldMatrix{};

};

