#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>

// -- Variabili globali DirectX (infrastruttura, non toccare!!!) --
static ID3D11Device* g_device = nullptr;
static ID3D11DeviceContext* g_context = nullptr;
static IDXGISwapChain* g_swapChain = nullptr;
static ID3D11RenderTargetView* g_renderTarget = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

// -- Gestore messaggi Windows (obbligatorio, lascialo così) --
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;
    if (msg == WM_DESTROY) { PostQuitMessage(0); return 0; }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // =====================================================
    // FASE 1 — SETUP (eseguita una volta sola)
    // =====================================================

    // Crea la finestra Windows
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0, 0,
                       GetModuleHandle(nullptr), nullptr, nullptr,
                       nullptr, nullptr, L"Software calcolatore di dati statistici", nullptr };
    RegisterClassExW(&wc);
    HWND hwnd = CreateWindowW(L"Software calcolatore di dati statistici", L"Calcolatore di dati statistici",
        WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800,
        nullptr, nullptr, wc.hInstance, nullptr);

    // Inizializza DirectX 11
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    D3D_FEATURE_LEVEL fl;
    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
        &sd, &g_swapChain, &g_device, &fl, &g_context);
    ID3D11Texture2D* pBack = nullptr;
    g_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBack));
    g_device->CreateRenderTargetView(pBack, nullptr, &g_renderTarget);
    pBack->Release();

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    // Inizializza ImGui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();           // tema scuro (puoi provare StyleColorsLight)
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_device, g_context);

    // =====================================================
    // FASE 2 — LOOP PRINCIPALE (gira ogni frame)
    // =====================================================
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        // Gestisci i messaggi Windows (click, tasti, ecc.)
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // -- Inizio frame ImGui --
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // =====================================================
        // FASE 3 — LA TUA UI (tutto quello che vuoi disegnare)
        // Aggiungi qui le tue finestre e i tuoi widget ImGui
        // =====================================================
        ImGui::Begin("Calcolatore di dati statistici");    // apre una finestra
        ImGui::Text("Testo di prova!!!");        // testo semplice
        ImGui::End();                       // chiude la finestra

        // =====================================================
        // FASE 4 — RENDERING (non toccare, va lasciato così)
        // =====================================================
        float bg[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // colore sfondo
        g_context->ClearRenderTargetView(g_renderTarget, bg);
        g_context->OMSetRenderTargets(1, &g_renderTarget, nullptr);
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_swapChain->Present(1, 0);
    }

    // =====================================================
    // FASE 5 — PULIZIA (eseguita una volta sola alla fine)
    // =====================================================
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    if (g_renderTarget) g_renderTarget->Release();
    if (g_swapChain)    g_swapChain->Release();
    if (g_context)      g_context->Release();
    if (g_device)       g_device->Release();
    DestroyWindow(hwnd);
    return 0;
}