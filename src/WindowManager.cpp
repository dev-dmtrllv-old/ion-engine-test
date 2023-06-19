#include "pch.hpp"
#include "WindowManager.hpp"
#include "core.hpp"

namespace ion
{
	const char* WindowManager::name() { return "WindowManager"; }

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void WindowManager::initialize()
	{
#ifdef _WIN32
		hInstance_ = GetModuleHandle(NULL);

		wcx_.cbClsExtra = 0;
		wcx_.cbSize = sizeof(WNDCLASSEX);
		wcx_.cbWndExtra = 0;
		wcx_.hbrBackground = CreateSolidBrush(RGB(200,0,0));
		wcx_.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcx_.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcx_.hIconSm = NULL;
		wcx_.hInstance = hInstance_;
		wcx_.lpfnWndProc = wndProc;
		wcx_.lpszClassName = CLASS_NAME;
		wcx_.lpszMenuName = 0;
		wcx_.style = CS_HREDRAW | CS_VREDRAW;

		RegisterClassEx(&wcx_);

		hwnd_ = CreateWindowEx(0, CLASS_NAME, TEXT("Ion Game Window"), WS_OVERLAPPEDWINDOW, CW_DEFAULT, CW_DEFAULT, CW_DEFAULT, CW_DEFAULT, NULL, NULL, hInstance_, NULL);
			
		if (hwnd_ == NULL)
			throw std::runtime_error("Could not create window!");

		ShowWindow(hwnd_, SW_NORMAL | SW_MAXIMIZE);
		UpdateWindow(hwnd_);

#elif defined(__unix__)

		display = CHECK_NULL(XOpenDisplay(nullptr));

		root = DefaultRootWindow(display);

		vi = CHECK_NULL(glXChooseVisual(display, 0, att));

		cmap = XCreateColormap(display, root, vi->visual, AllocNone);
		swa = {
			.event_mask = ExposureMask | KeyPressMask,
			.colormap = cmap,
		};

		int s = DefaultScreen(display);

		win = XCreateSimpleWindow(display, RootWindow(display, s), 10, 10, 640, 480, 1, BlackPixel(display, s), WhitePixel(display, s));

		Atom delWindow = XInternAtom(display, "WM_DELETE_WINDOW", 0);
		XSetWMProtocols(display, win, &delWindow, 1);

		XSelectInput(display, win, ExposureMask | KeyPressMask);
		XMapWindow(display, win);
		XStoreName(display, win, "Ion Game Window");
#endif
	}

	void WindowManager::dispose()
	{
#ifdef _WIN32

#elif defined(__unix__)
		XDestroyWindow(display, win);
		XCloseDisplay(display);
#endif
	}

	void WindowManager::startEventLoop()
	{
#ifdef _WIN32
		MSG msg = { };
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
#elif defined(__unix__)
		XWindowAttributes gwa = { 0 };
		XEvent e;

		int s = DefaultScreen(display);

		while (1)
		{
			XNextEvent(display, &e);

			if (e.type == Expose)
			{
				XFillRectangle(display, win, DefaultGC(display, s), 20, 20, 10, 10);
			}

			if (e.type == KeyPress)
				break;

			if (e.type == ClientMessage)
				break;
		}

		core.logger().debug("eventloop exit");
#endif
	}

	LRESULT CALLBACK WindowManager::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(32,32,32)));

			EndPaint(hwnd, &ps);
		}
		return 0;

		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}