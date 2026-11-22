#pragma once

#include "framework.h"

using Microsoft::WRL::ComPtr;

class App
{
public:
  App();
  ~App();

  void OnInit(HWND hWnd, UINT m_width, UINT m_height);
  void OnResize(UINT m_width, UINT m_height);
  void OnUpdate();
  void OnRender();
  void OnDestroy();

private:
  static const UINT FrameCount = 2;
  static const UINT TextureWidth = 256;
  static const UINT TextureHeight = 256;
  static const UINT TexturePixelSize = 4;  // The number of bytes used to represent a pixel in the texture.

  struct Vertex
  {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 uv;
  };

  // Root assets path.
  std::wstring m_assetsPath;

  // Viewport dimensions.
  UINT m_width;
  UINT m_height;
  float m_aspectRatio;

  // Adapter info.
  bool m_useWarpDevice;

  // Windows
  HWND m_hWnd;

  // Pipeline objects.
  CD3DX12_VIEWPORT m_viewport;
  CD3DX12_RECT m_scissorRect;
  ComPtr<IDXGISwapChain3> m_swapChain;
  ComPtr<ID3D12Device> m_device;
  ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
  ComPtr<ID3D12CommandAllocator> m_commandAllocator;
  ComPtr<ID3D12CommandQueue> m_commandQueue;
  ComPtr<ID3D12RootSignature> m_rootSignature;
  ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
  ComPtr<ID3D12DescriptorHeap> m_srvHeap;
  ComPtr<ID3D12PipelineState> m_pipelineState;
  ComPtr<ID3D12GraphicsCommandList> m_commandList;
  UINT m_rtvDescriptorSize;

  // App resources.
  ComPtr<ID3D12Resource> m_vertexBuffer;
  D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
  ComPtr<ID3D12Resource> m_texture;

  // Synchronization objects.
  UINT m_frameIndex;
  HANDLE m_fenceEvent;
  ComPtr<ID3D12Fence> m_fence;
  UINT64 m_fenceValue;

  // Helper function.
  void LoadPipeline();
  void LoadAssets();
  void PopulateCommandList();
  void WaitForPreviousFrame();
  void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter);
  std::wstring GetAssetFullPath(LPCWSTR assetName);
};
