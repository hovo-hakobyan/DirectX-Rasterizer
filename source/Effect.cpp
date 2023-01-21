#include "pch.h"
#include "Effect.h"
#include "Texture.h"

Effect::Effect(ID3D11Device* pDevice, const std::wstring& path)
{
	m_pEffect = LoadEffect(pDevice, path);
	if (m_pEffect)
	{
		m_pPointTechnique = m_pEffect->GetTechniqueByName("PointFilterTechnique");
		m_pLinearTechnique = m_pEffect->GetTechniqueByName("LinearFilterTechnique");
		m_pAnisotropicTechnique = m_pEffect->GetTechniqueByName("AnisotropicFilterTechnique");
	}
	
	if (!m_pPointTechnique->IsValid() || !m_pLinearTechnique->IsValid() || !m_pAnisotropicTechnique->IsValid())
	{
		std::wcout << L"Technique not valid\n";
	}

	m_pMatWorldViewProj = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pMatWorldViewProj->IsValid())
	{
		std::wcout << L"m_pMatWorldViewProjVariable not valid!\n";
	}

	m_pDiffuseMapVar = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	if (!m_pDiffuseMapVar->IsValid())
	{
		std::wcout << L"Diffuse map variable not valid\n";
	}

	m_CurrentFilterMode = FilterMode::Point;
}

Effect::~Effect()
{
	if (m_pDiffuseMapVar)
		m_pDiffuseMapVar->Release();

	if (m_pMatWorldViewProj)
		m_pMatWorldViewProj->Release();

	if (m_pAnisotropicTechnique)
		m_pAnisotropicTechnique->Release();

	if (m_pLinearTechnique)
		m_pLinearTechnique->Release();

	if (m_pPointTechnique)
		m_pPointTechnique->Release();

	if (m_pEffect)
		m_pEffect->Release();
}

void Effect::SetWorldViewProjMatrix(const dae::Matrix& worldViewProjection)
{
	m_pMatWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&worldViewProjection));
}

void Effect::SetDiffuseMap(Texture* pDiffuseTexture)
{
	if (m_pDiffuseMapVar)
	{
		m_pDiffuseMapVar->SetResource(pDiffuseTexture->GetRV());
		
	}
}

void Effect::CycleFilterMode()
{
	switch (m_CurrentFilterMode)
	{
	case FilterMode::Point:
		m_CurrentFilterMode = FilterMode::Linear;
		break;
	case FilterMode::Linear:
		m_CurrentFilterMode = FilterMode::Anisotropic;
		break;
	case FilterMode::Anisotropic:
		m_CurrentFilterMode = FilterMode::Point;
		break;
	}
}

ID3DX11EffectTechnique* Effect::GetTechnique() const
{
	switch (m_CurrentFilterMode)
	{
	case FilterMode::Point:
		return m_pPointTechnique;
		break;
	case FilterMode::Linear:
		return m_pLinearTechnique;
		break;
	case FilterMode::Anisotropic:
		return m_pAnisotropicTechnique;
		break;
	}

	return nullptr;
}

static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	HRESULT result;
	ID3D10Blob* pErrorBlob{ nullptr };
	ID3DX11Effect* pEffect{ nullptr };

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	result = D3DX11CompileEffectFromFile(assetFile.c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		pDevice,
		&pEffect,
		&pErrorBlob);

	if (FAILED(result))
	{
		if (pErrorBlob!= nullptr)
		{
			const char* pErrors = static_cast<char*>(pErrorBlob->GetBufferPointer());

			std::wstringstream ss;
			for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
			{
				ss << pErrors[i];
			}

			OutputDebugStringW(ss.str().c_str());
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			std::wcout << ss.str() << '\n';
		}
		else
		{
			std::wstringstream ss;
			ss << L"EffectLoader: Failed to CreateEffectFromFile!\nPath: " << assetFile;
			std::wcout << ss.str() << '\n';
			return nullptr;
		}
	}

	return pEffect;
}
