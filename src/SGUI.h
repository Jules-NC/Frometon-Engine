#include <vector>

#ifndef IMGUI_I
    #define IMGUI_I
    #include "imgui.h"
    #include "imgui_impl_glfw.h"
    #include "imgui_impl_opengl3.h"
#endif


class SGUI{
public:
    static SGUI& getInstance() {
        static SGUI instance;
        return instance;
    }

    void init();
    void beginDrawFrame();
    void endDrawFrame();

    void cleanup();

private:
    SGUI() = default;
    ~SGUI() = default;
    SGUI(const SGUI&) = delete;
    SGUI& operator=(const SGUI&) = delete;

    void drawLeftbar();
    void drawTopMenu();
    void drawDownMenu();
    ImGuiIO io;
};
