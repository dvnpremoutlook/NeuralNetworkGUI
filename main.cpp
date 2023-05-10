// Dear ImGui: standalone example application for DirectX 11
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include "ImageInfo.h"
#include <chrono>
#include <queue>
#include <typeinfo>
#define PY_SSIZE_T_CLEAN
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <python3.10/Python.h>
#include <python3.10/numpy/arrayobject.h>
#include <stdio.h>
#include <atomic>
// #include "globals.h"
#include <thread>
std::atomic<bool> should_stop{ false };
using namespace std;

// Data
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);




// Main code
int main(int, char**)
{

    auto start = std::chrono::high_resolution_clock::now();
    Py_Initialize();


    vector<queue<queue<double>>> XOR_Results;
    for(int x = 0; x < 4; x++){
        ImageInfo* xrInstance = new ImageInfo(x);
        queue<queue<double>> layersX = xrInstance->layersX();
        queue<queue<double>> layerInfoX = xrInstance->layerInfoX();
        queue<queue<double>> layerInfoX2 = xrInstance->layerInfoX2();
        queue<queue<double>> layerInfoX3 = xrInstance->layerInfoX3();
        XOR_Results.push_back(layersX);
        XOR_Results.push_back(layerInfoX);
        XOR_Results.push_back(layerInfoX2);
        XOR_Results.push_back(layerInfoX3);        
    }

    Py_Finalize();


    // Get the current time after the code to be measured
    auto end = std::chrono::high_resolution_clock::now();

    // // Calculate the duration in microseconds
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    // // Print the duration in microseconds
    std::cout << "Time taken: " << duration << " microseconds" << std::endl;

    // End Of Testing 
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);



    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.30f, 1.00f);
    
    // Main loop
    bool done = false;
    while (!done)
    {

        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();

        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();

  


        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // if (show_demo_window)
        //     ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        static float circle_line_width = 0.0f;
        static float circle_radius = 0.016;
        // static float y = 1.0f;
        // static float x = 1.0f;

        static int counter = 0;
        queue<queue<double>> layers_queue = XOR_Results[0 + (counter * 4)];
        queue<queue<double>> layerInfoX_queue = XOR_Results[1 + (counter * 4)];
        queue<queue<double>> layerInfoX2_queue = XOR_Results[2 + (counter * 4)];
        queue<queue<double>> layerInfoX3_queue = XOR_Results[3 + (counter * 4)];
        queue<double> layer1 = layers_queue.front();
        layers_queue.pop();
        queue<double> layer2 = layers_queue.front();
        layers_queue.pop();
        queue<double> layer3 = layers_queue.front();

        ImGui::SliderFloat("circle_radius", &circle_radius, 0.002f,  0.02f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderInt("My Integer Slider", &counter, 0, 3);

        // ImGui::SliderFloat("y", &y, 1.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f     
        // ImGui::SliderFloat("x", &x, 1.0f, 50.0f); 
        {
            ImGui::SetNextWindowBgAlpha(0.1f);
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
 
            queue<ImVec2> input_layer_corrdinates;
            queue<ImVec2> layer2_corrdinates;
            queue<ImVec2> layer3_corrdinates;
            queue<ImVec2> layer4_corrdinates;
            ImVec2 output_corrdinates;

            // layerInfoX_queue
            // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            for (int i = 0; i<2;i++){
                ImVec2 window_pos = ImGui::GetWindowPos();
                ImVec2 window_size = ImGui::GetWindowSize();
                ImVec2 window_center = ImVec2((window_pos.x + window_size.x * 0.3f),(window_pos.y + window_size.y * 0.08f)+(i+11)*(30));
                ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * circle_radius , IM_COL32(0, 255, 0, 200), 0, 0);    
                input_layer_corrdinates.push(window_center);
            }

            queue<double> layer1_temp = layer1;
            for (int i = 0; i<10;i++){
                ImVec2 window_pos = ImGui::GetWindowPos();
                ImVec2 window_size = ImGui::GetWindowSize();
                // float offset = i * y;
                ImU32 color = IM_COL32(255, 0, 0, 255);
                if(layer1_temp.front() > 1.0){
                    color = IM_COL32(0, 255, 0, 255);
                }
                layer1_temp.pop();
                ImVec2 window_center = ImVec2((window_pos.x + window_size.x * 0.4f),(window_pos.y + window_size.y * 0.08f)+(i+7)*30);
                ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * circle_radius , color, 0, 0);
                layer2_corrdinates.push(window_center);
            }




            queue<queue<double>> layer_lines = layerInfoX_queue;
            queue<ImVec2> temp1 = layer2_corrdinates ;
            for(int i = 0; i< 10; i++){
                queue<ImVec2> temp2 = input_layer_corrdinates;
                queue<double> temp_lines = layer_lines.front();
                for (int j = 0; j < 2; j++){
                    ImU32 color = IM_COL32(0, 255, 255, 255);
                    if(temp_lines.front() > 0.0){
                        color = IM_COL32(255, 0, 0, 255);
                    }
                    ImGui::GetForegroundDrawList()->AddLine(temp2.front(), temp1.front(), color, 1.0f);
                    temp2.pop();    
                    temp_lines.pop();
                }
                temp1.pop();
                layer_lines.pop();
            }

            // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            queue<double> layer2_temp = layer2;
            for (int i = 0; i<10;i++){
                ImVec2 window_pos = ImGui::GetWindowPos();
                ImVec2 window_size = ImGui::GetWindowSize();
                // float offset = i * y;
                ImU32 color = IM_COL32(255, 0, 0, 255);
                if(layer2_temp.front() > 1.0){
                    color = IM_COL32(0, 255, 0, 255);
                }
                layer2_temp.pop();
                ImVec2 window_center = ImVec2((window_pos.x + window_size.x * 0.5f),(window_pos.y + window_size.y * 0.08f)+(i+7)*30);
                ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * circle_radius , color, 0, 0);
                layer3_corrdinates.push(window_center);
            }

            queue<queue<double>> layer2_lines = layerInfoX2_queue;
            temp1 = layer2_corrdinates;
            for(int i = 0; i< 10; i++){
                queue<ImVec2> temp2 = layer3_corrdinates;
                queue<double> temp_lines = layer2_lines.front();
                for (int j = 0; j < 10; j++){
                    ImU32 color = IM_COL32(0, 255, 255, 255);
                    if(temp_lines.front() > 0.0){
                        color = IM_COL32(255, 0, 0, 255);
                    }
                    ImGui::GetForegroundDrawList()->AddLine(temp1.front(), temp2.front(), color, 1.0f);
                    temp2.pop();    
                    temp_lines.pop();
                }
                temp1.pop();
                layer2_lines.pop();
            }

            // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            queue<double> layer3_temp = layer3;
            for (int i = 0; i<10;i++){
                ImVec2 window_pos = ImGui::GetWindowPos();
                ImVec2 window_size = ImGui::GetWindowSize();
                // float offset = i * y;
                ImU32 color = IM_COL32(255, 0, 0, 255);
                if(layer3_temp.front() > 1.0){
                    color = IM_COL32(0, 255, 0, 255);
                }
                layer3_temp.pop();
                ImVec2 window_center = ImVec2((window_pos.x + window_size.x * 0.6f),(window_pos.y + window_size.y * 0.08f)+(i+7)*30);
                ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * circle_radius , color, 0, 0);
                layer4_corrdinates.push(window_center);
            }


            queue<queue<double>> layer3_lines = layerInfoX3_queue;
            temp1 = layer3_corrdinates;
            for(int i = 0; i< 10; i++){
                queue<ImVec2> temp2 = layer4_corrdinates;
                queue<double> temp_lines = layer3_lines.front();
                for (int j = 0; j < 10; j++){
                    ImU32 color = IM_COL32(0, 255, 255, 255);
                    if(temp_lines.front() > 0.0){
                        color = IM_COL32(255, 0, 0, 255);
                    }
                    ImGui::GetForegroundDrawList()->AddLine(temp1.front(), temp2.front(), color, 1.0f);
                    temp2.pop();    
                    temp_lines.pop();
                }
                temp1.pop();
                layer3_lines.pop();
            }


            // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            for (int i = 0; i<1;i++){
                ImVec2 window_pos = ImGui::GetWindowPos();
                ImVec2 window_size = ImGui::GetWindowSize();
                // float offset = i * y;
                ImVec2 window_center = ImVec2((window_pos.x + window_size.x * 0.7f),(window_pos.y + window_size.y * 0.08f)+(i+11.5)*30);
                ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * circle_radius , IM_COL32(0, 255, 0, 200), 0, 0);
                output_corrdinates = window_center;
            }
            
            temp1 = layer4_corrdinates;
            for(int i = 0; i< 10; i++){
                ImGui::GetForegroundDrawList()->AddLine(temp1.front(), output_corrdinates, IM_COL32(0, 255, 255, 255), 1.0f); 
                temp1.pop();
            }
            // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

