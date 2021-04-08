#include <cstdint>

class Chip8
{
    private:
        uint8_t registers[16];
        uint8_t memory[4096];
        uint16_t index;
        uint16_t pc;
        uint16_t stack[16];
        uint16_t sp;
        uint8_t delay_timer;
        uint8_t sound_timer;
        uint16_t instruction;
    public:
        uint32_t vid[64 * 32];
        uint16_t keypad[16];
        Chip8();
        void loadRom(char const* filename);
        void cycle();
        void execute();
};
