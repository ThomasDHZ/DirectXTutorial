#include "pch.h"
#include "Game.h"
#include <fstream>

Array<byte>^ Game::LoadShaderFile(std::string file)
{
	Array<byte>^ FileData = nullptr;
	std::ifstream ShaderFile(file, std::ios::in | std::ios::binary | std::ios::ate);

	if (ShaderFile.is_open())
	{
		int length = (int)ShaderFile.tellg();

		FileData = ref new Array<byte>(length);
		ShaderFile.seekg(0, std::ios::beg);
		ShaderFile.read(reinterpret_cast<char*>(FileData->Data), length);
		ShaderFile.close();
	}

	return FileData;
}

void Game::Initialize()
{
	ComPtr<ID3D11Device> tempDevice;
	ComPtr<ID3D11DeviceContext> tempDeviceContext;
	ComPtr<IDXGIDevice1> dxgiDevice;
	ComPtr<IDXGIAdapter> dxgiAdapter;
	ComPtr<IDXGIFactory2> dxgiFactory;
	ComPtr<ID3D11Texture2D> backBuffer;

	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &tempDevice, nullptr, &tempDeviceContext);

	tempDevice.As(&device);
	tempDeviceContext.As(&deviceContext);
	device.As(&dxgiDevice);

	dxgiDevice->GetAdapter(&dxgiAdapter);
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

	DXGI_SWAP_CHAIN_DESC1 swapChainDescription = { 0 };
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   
	swapChainDescription.BufferCount = 2;                                  
	swapChainDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;            
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;   
	swapChainDescription.SampleDesc.Count = 1;                           

	CoreWindow^ Window = CoreWindow::GetForCurrentThread();  
	dxgiFactory->CreateSwapChainForCoreWindow(device.Get(), reinterpret_cast<IUnknown*>(Window), &swapChainDescription, nullptr, &swapChain);

	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);

	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);

	D3D11_VIEWPORT viewPort = { 0 };
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = Window->Bounds.Width;
	viewPort.Height = Window->Bounds.Height;

	deviceContext->RSSetViewports(1, &viewPort);

	Graphics();
	PipeLine();
}

void Game::Update()
{

}

void Game::Render()
{
	deviceContext->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);
	
	float color[4] = { 0.0f, 0.5, 0.3, 1.0f };
	deviceContext->ClearRenderTargetView(renderTarget.Get(), color);
	
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->Draw(3, 0);

	swapChain->Present(1, 0);
}

void Game::PipeLine()
{
	Array<byte>^ VertexShader = LoadShaderFile("VertexShader.cso");
	Array<byte>^ PixelShader = LoadShaderFile("PixelShader.cso");

	device->CreateVertexShader(VertexShader->Data, VertexShader->Length, nullptr, &vertexShader);
	device->CreatePixelShader(PixelShader->Data, PixelShader->Length, nullptr, &pixelShader);

	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	D3D11_INPUT_ELEMENT_DESC elementDesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	device->CreateInputLayout(elementDesc, ARRAYSIZE(elementDesc), VertexShader->Data, VertexShader->Length, &inputLayout);
	deviceContext->IASetInputLayout(inputLayout.Get());
}

void Game::Graphics()
{
	Vertex Vertices[] =
	{
			{ 0.0f, 0.5f, 0.0f },
			{ 0.45f, -0.5f, 0.0f },
			{ -0.45f, -0.5f, 0.0f },
	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(Vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subresourceData = { Vertices, 0,0 };
	
	device->CreateBuffer(&vertexBufferDesc, &subresourceData, &vertexBuffer);


}