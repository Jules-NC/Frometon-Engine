#include "imgui.h"

class SGUI{
public:
    static SGUI& getInstance() {
        static SGUI instance;
        return instance;
    }

    void init();
    void drawFrame();
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
