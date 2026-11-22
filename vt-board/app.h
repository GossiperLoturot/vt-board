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

  // Viewport dimensions.
  UINT m_width;
  UINT m_height;
  float m_aspectRatio;

  // Adapter info.
  bool m_useWarpDevice;

  // Windows
  HWND m_hWnd;

  // Pipeline objects.
  ComPtr<IDXGISwapChain3> m_swapChain;
  ComPtr<ID3D12Device> m_device;
  ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
  ComPtr<ID3D12CommandAllocator> m_commandAllocator;
  ComPtr<ID3D12CommandQueue> m_commandQueue;
  ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
  ComPtr<ID3D12PipelineState> m_pipelineState;
  ComPtr<ID3D12GraphicsCommandList> m_commandList;
  UINT m_rtvDescriptorSize;

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
};
