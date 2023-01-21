#pragma once

class Texture
{
public:
	Texture(ID3D11Device* pDevice, const std::string& path);
	~Texture();

	ID3D11ShaderResourceView* GetRV()const { return m_pResourceView; }

private:
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pResourceView; //Used to read a texture in shader?


	SDL_Surface* m_pSurface{ nullptr };
	uint32_t* m_pSurfacePixels{ nullptr };
};

	

