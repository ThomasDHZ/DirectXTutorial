#pragma once
#include <wrl.h>
#include <windows.graphics.directx.h>
#include <string>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;

struct Vertex
{
	float x;
	float y;
	float z;
};

class Game
{
private:
	ComPtr<ID3D11Device1> device;
	ComPtr<ID3D11DeviceContext1> deviceContext;
	ComPtr<ID3D11RenderTargetView> renderTarget;
	ComPtr<IDXGISwapChain1> swapChain;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11InputLayout> inputLayout;

	Array<byte>^ LoadShaderFile(std::string file);

public:
	void Initialize();
	void Graphics();
	void Update();
	void Render();
	void PipeLine();
};

