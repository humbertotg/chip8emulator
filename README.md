# chip8emulator
Chip8 Emulator written in C++ with OpenGL for input output

# What is it?
I made this project with the goal of learning more about emulator development in C++ and an introduction to OpenGL contexts with GLWF. Chip-8 is an interpreted programming language developed in the 70s. Chip-8 programs run on a virtual machine, hence why the project is more of an interpreter than it is an emulator. The standard version has only 35 instruction of the same length, which is why it's a popular starting point for learning emulation. 

# Prerequisites
- A C++ compiler 
- OpenGL library GLWF (Tested with GLWF 3.3.3)

# How to build
    g++ src/main.cpp src/chip.cpp src/ui.cpp -I(path to header files) - L(path to library) -lopengl32 -lglfw3dll -o (outputFileName)
The GLWF dll needs to be where the output file is

# Usage
    main.exe path/to/rom

It only takes one argument which is the path to the rom. Emulation is set to stop when the window is closed

# Controls

The original CHIP-8 had a 16 key keypad for input. The following keys were mapped into a modern keybaord this way

     CHIP8        KEYBOARD
    1 2 3 C	      1 2 3 4  
    4 5 6 D   =>   Q W E R  
    7 8 9 E         A S D F  
    A 0 B F          Z X C V  

# Additional Resources

Some resouces that were helpful for the making of this project

- [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [BUILDING A CHIP-8 EMULATOR C++](https://austinmorlan.com/posts/chip8_emulator/)

# What's Next?

- Add Super Chip-8 Instructions
- Add Audio
- Improve cycle emulation
- Improve error handling




