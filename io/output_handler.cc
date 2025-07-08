
#include "output_handler.h"
#include "rendering/shade_mapper.h"
#include <iostream>

Output_Handler::Output_Handler(int w, int h) 
: width(w), height(h), hConsole(GetStdHandle(STD_OUTPUT_HANDLE)), buf(w) {}

void Output_Handler::PrintBuffer(const std::vector<Color>& frame_buffer)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
            buf[x] = Shade_Mapper::ColorToChar(frame_buffer[y * width + x]);
        
        WriteConsoleOutputCharacterA(
            hConsole,
            buf.data(),
            width,
            {0, (SHORT)y},
            &written
        );
    } 
}