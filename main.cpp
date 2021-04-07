#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "chip8.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320
#define OFFSET 10

int main()  
{   
    GLFWwindow *window;
    
    // Initialize the library
    if ( !glfwInit( ) )
    {
        return -1;
    }
    
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL );
    
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent( window );

    float vertices[] =
    {
        500, 500, 0.0, // top right corner
        500, 300, 0.0, // top left corner
        0, 0, 0.0, // bottom left corner
        300, 500, 0.0 // bottom right corner
    };

    glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT ); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode( GL_PROJECTION ); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity( ); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1 ); // essentially set coordinate system
    glMatrixMode( GL_MODELVIEW ); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity( ); // same as above comment

    Chip8 test;
    char const* filename = "test_opcode.ch8";
    test.loadRom(filename);
    while(!glfwWindowShouldClose( window ))
    {
        
        glClear( GL_COLOR_BUFFER_BIT );
        for(int i = 0; i < 32;i++){
            for(int j = 0; j < 64;j++){
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
                if(test.vid[i*64 + j] != 0){
                     glEnableClientState( GL_VERTEX_ARRAY ); // tell OpenGL that you're using a vertex array for fixed-function attribute
                     glVertexPointer( 3, GL_FLOAT, 0, vertices ); // point to the vertices to be used
                     glDrawArrays( GL_QUADS, 0, 4 ); // draw the vertixes
                     glColor3f(1.0f, 1.0f,1.0f);
                     glDisableClientState( GL_VERTEX_ARRAY ); // tell OpenGL that you're finished using the vertex arrayattribute
                }
            }
        }
        test.cycle();
        // Poll for and process events
        glfwSwapBuffers( window );
        glfwPollEvents( );
    }
    glfwTerminate( );
}

