#pragma once
#include "Camera.h"

struct SDL_Window;
struct SDL_Surface;
class Mesh;
class Texture;

namespace dae
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Update(const Timer* pTimer);
		void Render() const;

		void CycleFilterMode();

	private:
		SDL_Window* m_pWindow{};

		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{ false };
		ID3D11Device*			m_pDevice;
		ID3D11DeviceContext*	m_pDeviceContext;
	
		IDXGISwapChain*			m_pSwapChain;

		ID3D11Texture2D*		m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;

		ID3D11Resource*			m_pRenderTargetBuffer;
		ID3D11RenderTargetView* m_pRenderTargetView;

		Mesh*					m_pMesh;
		Texture*				m_pCheckersPattern;

		Camera m_Camera{};

		//DIRECTX
		HRESULT InitializeDirectX();
		
		//...
	};
}
