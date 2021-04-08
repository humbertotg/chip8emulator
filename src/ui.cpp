#include "ui.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320
#define OFFSET 10

int keyPress;
bool pressedFlag;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

UI::UI()
{
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "CHIP8", NULL, NULL );
    glfwMakeContextCurrent( window );
    glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT ); 
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( ); 
    glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1 );
    glMatrixMode( GL_MODELVIEW ); 
    glLoadIdentity( );
    glfwSetKeyCallback(window, key_callback);
}

bool UI::closeCondition()
{
    return glfwWindowShouldClose( window );
}

void UI::updateDisplay(uint32_t* vid)
{
    float vertices[] =
    {
        0, 0, 0, // top right corner
        0, 0, 0, // top left corner
        0, 0, 0, // bottom left corner
        0, 0, 0 // bottom right corner
    };
    glClear( GL_COLOR_BUFFER_BIT );
        for(int i = 0; i < SCREEN_HEIGHT/OFFSET;++i){
            for(int j = 0; j < SCREEN_WIDTH/OFFSET;++j){
                    vertices[3] = float(j) * OFFSET;
                    vertices[4] = float(i) * OFFSET;
                    vertices[5] = 0;

                    vertices[0] = float(j) * OFFSET + OFFSET;
                    vertices[1] = float(i) * OFFSET;
                    vertices[2] = 0;

                    vertices[6] = float(j) * OFFSET;
                    vertices[7] = float(i) * OFFSET + OFFSET;
                    vertices[8] = 0;

                    vertices[9] = float(j) * OFFSET + OFFSET;
                    vertices[10] = float(i) * OFFSET + OFFSET;
                    vertices[11] = 0;
                if(vid[i*SCREEN_WIDTH/OFFSET + j] != 0){
                     glEnableClientState( GL_VERTEX_ARRAY ); 
                     glVertexPointer( 3, GL_FLOAT, 0, vertices ); 
                     glDrawArrays( GL_QUADS, 0, 4 ); 
                     glColor3f(1.0f, 1.0f,1.0f);
                     glDisableClientState( GL_VERTEX_ARRAY );
                }
            }
        }
        glfwSwapBuffers( window );
        glfwPollEvents( );
}

void UI::processInput(uint16_t* keypad)
{
    if(pressedFlag)
    {
        keypad[keyPress] = 1;
    } else
    {
        keypad[keyPress] = 0;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    int keyP = -1;
    switch(key)
    {
        case GLFW_KEY_1:
            keyP = 0x01;
            break;
        case GLFW_KEY_2:
            keyP = 0x02;
            break;
        case GLFW_KEY_3:
            keyP = 0x03;
            break;
        case GLFW_KEY_4:
            keyP = 0x0C;
            break;
        case GLFW_KEY_Q:
            keyP = 0x04;
            break;
        case GLFW_KEY_W:
            keyP = 0x05;
            break;
        case GLFW_KEY_E:
            keyP = 0x06;
            break;
        case GLFW_KEY_R:
            keyP = 0x0D;
            break;
        case GLFW_KEY_A:
            keyP = 0x07;
            break;
        case GLFW_KEY_S:
            keyP = 0x08;
            break;
        case GLFW_KEY_D:
            keyP = 0x09;
            break;
        case GLFW_KEY_F:
            keyP = 0x0E;
            break;
        case GLFW_KEY_Z:
            keyP = 0x0A;
            break;
        case GLFW_KEY_X:
            keyP = 0x00;
            break;
        case GLFW_KEY_C:
            keyP = 0x0B;
            break;
        case GLFW_KEY_V:
            keyP = 0x0F;
            break;
    } 
    if(keyP != -1)
    {
        keyPress = keyP;
        if(action == GLFW_PRESS)
        {
            pressedFlag = true;
        } 
        else if(action == GLFW_RELEASE)
        {
            pressedFlag = false;
        }
    }
}


UI::~UI()
{
    glfwTerminate();
}



