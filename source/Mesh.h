#pragma once
#include "Structs.h"
using namespace dae;
class Effect;

class Mesh
{
public:
	Mesh(ID3D11Device* pDevice,const std::vector<Vertex_In>& vertices, const std::vector<uint32_t>& indices);
	~Mesh();

	void Render(ID3D11DeviceContext* pDeviceContext);

private:
	Effect* m_pEffect;

	ID3DX11Effect* m_pEffectLocalPointer;
	ID3DX11EffectTechnique* m_pTechniqueLocalPointer;

	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	uint32_t m_NumIndices;

};
