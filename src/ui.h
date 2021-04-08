#include <GLFW/glfw3.h>

class UI
{
    private:
        GLFWwindow *window;
    public:
        UI();
        ~UI();
        bool closeCondition();
        void updateDisplay(uint32_t* vid);
        void processInput(uint16_t* keypad);
};
