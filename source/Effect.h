#pragma once

static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

class Effect 
{
public:
	Effect(ID3D11Device* pDevice, const std::wstring& path);
	~Effect();

	ID3DX11Effect*				GetEffect()		const { return m_pEffect; };
	ID3DX11EffectTechnique*		GetTechnique()	const { return m_pTechnique; };

private:

	ID3DX11Effect*			m_pEffect;
	ID3DX11EffectTechnique*	m_pTechnique;

};

