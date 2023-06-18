#include "pch.hpp"
#include "WindowManager.hpp"
#include "core.hpp"

namespace ion
{
	const char* WindowManager::name() { return "WindowManager"; }

	void WindowManager::initialize()
	{
#ifdef __unix__
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
#ifdef __unix__
		XDestroyWindow(display, win);
		XCloseDisplay(display);
#endif
	}

	void WindowManager::startEventLoop()
	{
#ifdef __unix__
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
}