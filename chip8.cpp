#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "chip8.h"

const unsigned int START_ADDR = 0x200;
const unsigned int FONT_START_ADDR = 0x50;

Chip8::Chip8()
{
    pc = START_ADDR;
    for(unsigned int i = 0; i < FONTSET_SIZE;++i)
    {
        memory[FONT_START_ADDR + i ] = fontset[i];
    }
}

void Chip8::loadRom(std::string &filename)
{
    std::ifstream rom(filename,std::ios::binary | std::ios::ate);
    if(rom.is_open())
    {
        std::streampos len = rom.tellg();
        char* buffer = new char[len];

        rom.seekg(0,std::ios::beg);
        rom.read(buffer,len);
        rom.close();
        for(long i = 0; i < len; ++i){
            memory[START_ADDR + i] = buffer[i];
        }
        delete[] buffer;
    }
}

void Chip8::cycle()
{
    instruction = memory[pc];
    pc += 2;
    execute();
    
    if(delay_timer > 0)
    {
        --delay_timer;
    }

    if(sound_timer > 0)
    {
        --sound_timer;
    }
}

void Chip8::execute()
{
    switch(instruction & 0XF00)
    {
        case 0x0:
            switch(instruction & 0XFF)
            {
                case 0xE0:
                    memset(vid,0,sizeof(vid));
                    break;
                case 0xEE:
                    --sp;
                    pc = stack[sp];
                    break;
                default:
                    std::cout<<"error";
            }
            break;
        case 0x1:
            pc = instruction & 0x0FFF;
            break;
        case 0x2:
            stack[sp] = pc;
            ++sp;
            pc = instruction & 0X0FFF;
            break;
        case 0x3:
            uint8_t Vx = (instruction & 0xF00) >> 8;
            uint8_t imme = (instruction & 0xFF); 

            if(registers[Vx] == imme)
            {
                pc += 2;
            }
            break;
        case 0x4:
            uint8_t Vx = (instruction & 0xF00) >> 8;
            uint8_t imme = (instruction & 0xFF); 

            if(registers[Vx] != imme)
            {
                pc += 2;
            }
            break;
        case 0x5:
            uint8_t Vx = (instruction & 0xF00) >> 8;
            uint8_t Vy = (instruction & 0xF0) >> 4;

            if(registers[Vx] == registers[Vy])
            {
                pc += 2;
            }
            break;
        case 0x6:
            uint8_t Vx = (instruction & 0xF00) >> 8;
            uint8_t Vy = (instruction & 0xF0) >> 4;

            if(registers[Vx] != registers[Vy])
            {
                pc += 2;
            }
            break;
        case 0x7:
            uint8_t Vx = (instruction & 0xF00) >> 8;
            uint8_t imme = (instruction & 0xFF); 

            registers[Vx] += imme;
            break;
        case 0x8:
            uint8_t Vx = (instruction & 0xF00) >> 8;
            uint8_t Vy = (instruction & 0xF0) >> 4;
            switch(instruction & 0xF)
            {
                case 0x00:
                    registers[Vx] = registers[Vy];
                    break;
                case 0x01:
                    registers[Vx] |= registers[Vy];
                    break;
                case 0x02:
                    registers[Vx] &= registers[Vy];
                    break;
                case 0x03:
                    registers[Vx] ^= registers[Vy];
                    break;
                case 0x04:
                    uint16_t sum = registers[Vx] + registers[Vy];
                    if(sum > 255)
                    {
                        registers[0xF] = 1;
                    } 
                    else
                    {
                        registers[0xF] = 0;
                    }

                    registers[Vx] = sum & 0xFF;
                    break;
                case 0x5:
                    if(registers[Vx] > registers[Vy])
                    {
                        registers[0xF] = 1;
                    }
                    else
                    {
                        registers[0xF] = 0;
                    }

                    registers[Vx] -= registers[Vy];
                    break;
                case 0x6:
                    registers[0xF] = (registers[Vx] & 0x1);
                    registers[Vx] >>= 1;
                    break;
                case 0x7:
                    if(registers[Vy] > registers[Vx])
                    {
                        registers[0xF] = 1;
                    }
                    else 
                    {
                        registers[0xF] = 0;
                    }
                        
                    registers[Vx] = registers[Vy] - registers[Vx];
                    break;
                 case 0xE:
                    registers[0xF] = (registers[Vx] & 0x80) >> 7;
                    registers[Vx] <<=1;
                    break;
                default:
                    std::cout<<"error";
            
            }
            break;
        case 0x9:
            uint8_t Vx = (instruction & 0xF00) >> 8;
            uint8_t Vy = (instruction & 0xF0) >> 4;
            if(registers[Vx] != registers[Vy])
            {
                pc += 2;
            }
            break;
        case 0xA:
            uint16_t addr = instruction & 0xFFF;

            index = addr;
            break;
        case 0xB:
            uint16_t addr = instruction & 0xFFF;

            pc = registers[0] + addr;
            break;
        //case 0xC: TODO
        
        case 0xD:
            uint8_t Vx = (instruction & 0xF00) >> 8;
            uint8_t Vy = (instruction & 0xF0) >> 4;
            uint8_t h = instruction & 0xF;

            uint8_t x = registers[Vx] % 64;
            uint8_t y = registers[Vy] % 32;

            registers[0xF] = 0;
            for(unsigned int i = 0;i < h;++i)
            {
                uint8_t sByte = memory[index + i];

                for(unsigned int j = 0; j < 8;++j)
                {
                    uint8_t sPixel = sByte & (0x80 >> j);
                    uint32_t* screenPixel = &vid[(y + i) * 64 +(x + j)]; 

                    if(sPixel)
                    {
                        if(*screenPixel == 0xFFFFFFFF)
                        {
                            registers[0xF] = 1;
                        }

                        *screenPixel ^= 0xFFFFFFFF;
                    }
                }
            }
            break;
        case 0xE:
            switch(instruction & 0xFF)
            {
                case 0x9E:
                    uint8_t Vx = (instruction & 0xF00) >> 8;

                    uint8_t key = registers[Vx];

                    if(keypad[key])
                    {
                        pc += 2;
                    }
                    break;
                case 0xA1:
                    uint8_t Vx = (instruction & 0xF00) >> 8;

                    uint8_t key = registers[Vx];

                    if(!keypad[key])
                    {
                        pc += 2;
                    }
                    break;
                default:
                    std::cout<<"error";
            }
            break;
        case 0xF:
            switch(instruction & 0xFF)
            {
                case 0x07:
                    uint8_t Vx = (instruction & 0xF00) >> 8;

                    registers[Vx] = delay_timer;
                    break;
                case 0x0A:
                    uint8_t Vx = (instruction & 0xF00) >> 8;
                    bool keyPress = false;

                    for(uint8_t i = 0; i < 16; ++i)
                    {
                        if(keypad[i])
                        {
                            registers[Vx] = i;
                            keyPress = true;
                            break;
                        }
                    }
                    if(!keyPress)
                    {
                        pc -= 2;
                    }
                    break;
                case 0x15:
                    uint8_t Vx = (instruction & 0xF00) >> 8;

                    delay_timer = registers[Vx];
                    break;
                case 0x18:
                    uint8_t Vx = (instruction & 0xF00) >> 8;

                    sound_timer = registers[Vx];
                    break;
                case 0x1E:
                    uint8_t Vx = (instruction & 0xF00) >> 8;

                    index += registers[Vx];
                    break;
                case 0x29:
                    uint8_t Vx = (instruction & 0xF00) >> 8;

                    uint8_t digit = registers[Vx];
                    index = FONT_START_ADDR + (5 * digit);
                    break;
                case 0x33:
                    uint8_t Vx = (instruction & 0xF00) >> 8;
                    uint8_t num = registers[Vx];

                    memory[index + 2] = num % 10;
                    num /= 10;

                    memory[index + 1] = num % 10;
                    num /= 10;

                    memory[index] = value % 10;
                    break;
                case 0x55:
                    uint8_t Vx = (instruction & 0xF00) >> 8;

                    for(uint8_t i = 0; i <= Vx;i++)
                    {
                        memory[index + i] = registers[i];
                    }
                    break;
                case 0x65:
                    uint8_t Vx = (instruction & 0xF00) >> 8;

                    for(uint8_t i = 0; i <= Vx;i++)
                    {
                        registers[i] = memory[index + 1];
                    }
                    break;
                    default:
                    std::cout<<"error";
            }
            break;
        default:
            std::cout<<"error";
    }
}

