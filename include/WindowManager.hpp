#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"

namespace ion
{
	class WindowManager final : public SubSystem<WindowManager>
	{
	private:
#ifdef _WIN32
		static LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
		SUBSYSTEM_CTOR(WindowManager)

	public:
		virtual const char* name() override;
		virtual void initialize() override;
		virtual void dispose() override;

		void startEventLoop();

	private:
#ifdef _WIN32
		const wchar_t* CLASS_NAME = L"ION_ENGINE";
		HINSTANCE hInstance_;
		WNDCLASSEX wcx_;
		HWND hwnd_;
#elif defined(__unix__)
		Display* display;
		Window root;
		GLint att[5] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		XVisualInfo* vi;
		Colormap cmap;
		XSetWindowAttributes swa;
		Window win;
		GLXContext glc;
#endif
	};
}