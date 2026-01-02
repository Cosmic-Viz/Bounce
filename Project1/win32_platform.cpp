#include <Windows.h>
#include "platform_common.cpp"
bool running = true;

struct RENDER_STATE {
    void* memory;
    int width;
    int height;
    BITMAPINFO bitmap_info;
};

RENDER_STATE render_state;
#include "renderer.cpp"
#include "game.cpp"
#define process_button(b, vk) \
case vk: { \
    input.buttons[b].changed = is_down!=input.buttons[b].is_down; \
    input.buttons[b].is_down = is_down; \
} break;\

LRESULT CALLBACK window_callback(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    LRESULT result = 0;
    switch (uMsg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        running = false;
        break;
    }

    case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		render_state.width = rect.right - rect.left;
		render_state.height = rect.bottom - rect.top;

        int size = render_state.width * render_state.height * sizeof(int);
        if(render_state.memory) {
            VirtualFree(render_state.memory, 0, MEM_RELEASE);
		}

		render_state.memory = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
		render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
		render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
		render_state.bitmap_info.bmiHeader.biPlanes = 1;
		render_state.bitmap_info.bmiHeader.biBitCount = 32; // 32 bits per pixel
		render_state.bitmap_info.bmiHeader.biCompression = BI_RGB; // No compression

}

    default: {
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
    return result;
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd
)
{
    //create window class

    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = L"Game Window";
    window_class.lpfnWndProc = window_callback;

    // Register the window class
    RegisterClass(&window_class);

    // create a window
    HWND window = CreateWindow(
        window_class.lpszClassName,
        L"My First Game Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        NULL, NULL, hInstance, NULL
    );
    HDC hdc = GetDC(window);

    //Show and update the window
   /*ShowWindow(window, nShowCmd);
   UpdateWindow(window);*/


    //calculating seconds per frame
	float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);
    float performance_frequency;
    {
        LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
    }

    Input input = {};
   //gameplay loop
    while (running) {
        


        MSG message;

        for (int i = 0; i < BUTTON_COUNT; i++) {
            input.buttons[i].changed = false;
            //input.buttons[i].is_down = false;
        }

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            switch (message.message) {
            case WM_KEYUP:
            case WM_KEYDOWN:
            {
                uint vk_code = (uint)message.wParam;
                bool is_down = ((message.lParam & (1 << 31)) == 0);

                    switch (vk_code) {
                process_button(BUTTON_UP, VK_UP);
                process_button(BUTTON_DOWN, VK_DOWN);
                process_button(BUTTON_W, 'W');
                process_button(BUTTON_S, 'S');
                process_button(BUTTON_LEFT, VK_LEFT);
                process_button(BUTTON_RIGHT, VK_RIGHT);
                process_button(BUTTON_ENTER, VK_RETURN);
                process_button(BUTTON_ESCAPE, VK_ESCAPE);
                    }

            } break;

            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }


        //Simulate

        simulate_game(&input, delta_time);


        //Render
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)(frame_end_time.QuadPart-frame_begin_time.QuadPart)/performance_frequency;
        frame_begin_time = frame_end_time;
        // Limit the frame rate to 60 FPS
        /*if (delta_time < 1.0f / 60.0f) {
            Sleep((DWORD)((1.0f / 60.0f - delta_time) * 1000.0f));
		}*/
    }
    return 0;
}