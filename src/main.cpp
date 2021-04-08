#include <iostream>
#include <chrono>
#include <thread>
#include "chip8.h"
#include "ui.h"


int main(int argc, char* argv[]) 
{    
    char* filename;
    if(argc < 2)
    {
        std::cout<<"Use requires path to a ROM";
        return -1;
    } else
    {
        filename = argv[1];
    }

    if ( !glfwInit( ) )
    {
        return -1;
    }
    Chip8 chip8cpu;
    UI display;
    chip8cpu.loadRom(filename);
    bool stop = false;
    while(!stop)
    {
        stop = display.closeCondition();
        chip8cpu.cycle();
        display.updateDisplay(chip8cpu.vid);
        display.processInput(chip8cpu.keypad);
        std::this_thread::sleep_for(std::chrono::microseconds(400));
    }
}