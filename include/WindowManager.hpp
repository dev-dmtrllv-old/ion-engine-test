#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"

namespace ion
{
    class WindowManager final : public SubSystem<WindowManager>
    {
    private:
        WindowManager(Core& core) : SubSystem(core) {}
        WindowManager(const WindowManager&) = delete;
        WindowManager(WindowManager&&) = delete;
        ~WindowManager() {}
        friend class ion::Core;

    public:
        virtual const char* name() override;
        virtual void initialize() override;
        virtual void dispose() override;

        void startEventLoop();

    private:
#ifdef __unix__
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