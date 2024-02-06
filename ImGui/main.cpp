
#include "main.hpp"

// Main code
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX windowClass = { };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = NULL;
    windowClass.cbWndExtra = NULL;
    windowClass.hInstance = GetModuleHandleA(0); // nullptr
    windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hbrBackground = nullptr;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = Window::ClassName;
    windowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);

    RegisterClassEx(&windowClass);
    hwnd = CreateWindowEx(NULL, windowClass.lpszClassName, Window::WindowName, WS_POPUP | CW_USEDEFAULT, (GetSystemMetrics(SM_CXSCREEN) / 2) - (Gui::Size.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (Gui::Size.y / 2), Gui::Size.x, Gui::Size.y, 0, 0, 0, 0);

    SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    POINT mouse;
    rc = { 0 };
    GetWindowRect(hwnd, &rc);

    //RenderBlur(hwnd);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Getting some shutdown stuff
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = NULL; // Remove the imgui.ini

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 5;

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    AddCustomDefaultFont(nullptr);

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_16_FAB, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    ImFont* FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
    ImFont* FontAwesomeBrands = io.Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 14.f, &icons_config, icons_ranges_brands);

    // Main loop
    bool loaderOpen = true;

    bool boxespp = false;
    bool distancee = false;
    bool enable = false;
    bool barhealth = false;
    bool nameesp = false;
    bool weaponn = false;
    bool armorbar = false;
    bool selfignore = false;
    bool skeletonsurallah = false;
    bool lines = false;
    bool markwater = false;
    bool viseur = false;

    if (WinVersion::GetVersion(info) && info.Major <= 6)
    {
        MessageBox(hwnd, "your operating system is not supported", "Error", MB_ICONERROR);
    }
    else while ( loaderOpen && (info.Major > 6) )
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
            }
        }
        if (!loaderOpen)
        {
            break;
        }
        
        style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);         // Text color
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Disabled text color
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.90f);     // Window background color
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.90f);      // Blue child window background color with reduced alpha
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);      // Popup background color
        style.Colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.10f, 0.90f);        // Blue window border color
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.10f, 0.10f, 0.10f, 0.90f);  // Window border shadow color
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);       // Frame background color
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.40f, 0.75f, 0.60f);// Blue frame background color when hovered
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 0.70f); // Frame background color when active
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.90f);      // Title background color
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 0.90f); // Title background color when active
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 0.90f); // Title background color when collapsed
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.70f);     // Menu bar background color
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.70f);   // Scrollbar background color
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.45f, 0.45f, 0.45f, 0.60f); // Scrollbar grab color
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.70f); // Scrollbar grab color when hovered
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.80f, 0.80f, 0.70f);  // Scrollbar grab color when active
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);    // Checkbox checkmark color
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);   // Slider grab color
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.90f, 0.90f, 0.70f);  // Slider grab color when active
        style.Colors[ImGuiCol_Button] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);        // Button color
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.40f, 0.75f, 0.70f); // Blue button color when hovered
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);  // Button color when active
        style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.30f, 0.30f, 0.70f);        // Header color
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.80f); // Header color when hovered
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.50f, 0.50f, 0.80f);  // Header color when active
        style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.70f);     // Separator color
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.70f); // Separator color when hovered
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);// Separator color when active
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);     // Resize grip color
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.70f); // Resize grip color when hovered
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);// Resize grip color when active
        style.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 0.86f);           // Tab color
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.40f, 0.75f, 0.80f);    // Blue tab color when hovered
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.23f, 0.85f);     // Tab color when active
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.08f, 0.97f);   // Tab color when unfocused
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.13f, 0.13f, 0.97f); // Tab color when unfocused and active
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);     // Plot lines color
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f); // Blue plot lines color when hovered
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f); // Plot histogram color
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f); // Blue plot histogram color when hovered
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f); // Selected text background color

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        D3DX11_IMAGE_LOAD_INFO FiveM; ID3DX11ThreadPump* Logo{ nullptr };
        ID3D11ShaderResourceView* Simple = nullptr;

        {
            ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
            ImGui::SetNextWindowPos({0 , 0}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(Gui::Size);
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, fivemm, sizeof(fivemm), &FiveM, Logo, &Simple, 0);
            ImGui::Begin(Window::WindowName, &loaderOpen, main_window_flags);
            UpdateWindowLocation();
            {
                ImGui::SetCursorPosX(290);
                ImGui::BeginChild("#1", ImVec2(300, 300));
                ImGui::Image(Simple, ImVec2(320, 300));
                ImGui::EndChild();
                ImGui::SameLine();
                ImGui::SetCursorPosX(5);
                ImGui::BeginChild("#2", ImVec2(300, 300), ImGuiWindowFlags_NoScrollbar);
                ImGui::Checkbox("Toggle", &enable);
                ImGui::SetCursorPosY(70);
                ImGui::Checkbox("Box ESP", &boxespp);
                ImGui::SetCursorPosY(100);
                ImGui::Checkbox("Name", &nameesp);
                ImGui::SetCursorPosY(130);
                ImGui::Checkbox("Weapon", &weaponn);
                ImGui::SetCursorPosY(160);
                ImGui::Checkbox("Distance", &distancee);
                ImGui::SetCursorPosY(190);
                ImGui::Checkbox("Health Bar", &barhealth);
                ImGui::SetCursorPosY(220);
                ImGui::Checkbox("Armor Bar", &armorbar);
                ImGui::SetCursorPosY(250);
                ImGui::SetCursorPosY(310);
                ImGui::Checkbox("Ignore Self", &selfignore);
                ImGui::SetCursorPosY(340);
                ImGui::Checkbox("Skeleton", &skeletonsurallah);
                ImGui::SetCursorPosY(370);
                ImGui::Checkbox("Lines", &lines);
                ImGui::SetCursorPosY(400);
                ImGui::Checkbox("WaterMark", &markwater);
                ImGui::SetCursorPosY(430);
                ImGui::Checkbox("CrossHair", &viseur);
                ImGui::EndChild();
            }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
       
        const float clear_color_with_alpha[4] = { 0 };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Never set the 1 to 0 or the panel movment will get fucked
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

    return 0;
}

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
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
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
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}