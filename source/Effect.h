#pragma once
class Texture;

static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

class Effect 
{
public:
	Effect(ID3D11Device* pDevice, const std::wstring& path);
	~Effect();

	void SetWorldViewProjMatrix(const dae::Matrix& worldViewProjection);
	void SetDiffuseMap(Texture* pDiffuseTexture);
	void CycleFilterMode();


	ID3DX11Effect*				GetEffect()		const { return m_pEffect; };
	ID3DX11EffectTechnique*		GetTechnique()	const;


private:
	enum class FilterMode
	{
		Point, Linear, Anisotropic
	};


	ID3DX11Effect*							m_pEffect;
	ID3DX11EffectTechnique*					m_pPointTechnique;
	ID3DX11EffectTechnique*					m_pLinearTechnique;
	ID3DX11EffectTechnique*					m_pAnisotropicTechnique;
	ID3DX11EffectMatrixVariable*			m_pMatWorldViewProj;
	ID3DX11EffectShaderResourceVariable*	m_pDiffuseMapVar;

	FilterMode m_CurrentFilterMode;

};

