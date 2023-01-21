#include "pch.h"
#include "Texture.h"
//Resource creation
//https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-textures-create
//https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_subresource_data
//https://wiki.libsdl.org/SDL2/SDL_Surface


Texture::Texture(ID3D11Device* pDevice, const std::string& path)
{
	m_pSurface = IMG_Load(path.c_str());
	m_pSurfacePixels = (uint32_t*)m_pSurface->pixels;

	/**Texture*/
	D3D11_TEXTURE2D_DESC desc{}; //Describes a 2d Texture
	desc.Width = m_pSurface->w;
	desc.Height = m_pSurface->h;
	desc.MipLevels = 1; //How many Downsized version of texture
	desc.ArraySize = 1; //How many textures in texture array
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT; //Read write access by GPU
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subResData{};
	subResData.pSysMem = m_pSurface->pixels; //Pointer to init data
	subResData.SysMemPitch = static_cast<UINT>(m_pSurface->pitch);  //Distance in bytes 
	subResData.SysMemSlicePitch = static_cast<UINT>(m_pSurface->h * m_pSurface->pitch); //Doc says this is only for 3D textures, but whatever

	HRESULT result{ pDevice->CreateTexture2D(&desc, &subResData, &m_pTexture) };

	if (FAILED(result))
	{
		assert(false && "Couldn't create 2D texture");
	}

	/**Resource view*/
	D3D11_SHADER_RESOURCE_VIEW_DESC rvDesc{};
	rvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; //Must be the same as resource type
	rvDesc.Texture2D.MipLevels = 1;

	result = pDevice->CreateShaderResourceView(m_pTexture, &rvDesc, &m_pResourceView);
	if (FAILED(result))
	{
		assert(false && "Couldn't create Resource view");
	}

	if (m_pSurface)
	{
		SDL_FreeSurface(m_pSurface);
		m_pSurface = nullptr;
	}
}

Texture::~Texture()
{
	m_pResourceView->Release();
	m_pTexture->Release();

}


