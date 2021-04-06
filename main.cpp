#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "chip8.h"

int main()  
{   
    Chip8 test;
    char const* filename = "pong.rom";
    test.loadRom(filename);
    while(true)
    {
		test.cycle();
        std::cout<<test.getInstruction()<<std::endl;
    }
}

