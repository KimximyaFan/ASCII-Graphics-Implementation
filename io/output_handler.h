#pragma once

#include "material/color.h"
#include <vector>
#include <windows.h>

class Output_Handler
{
public:
    Output_Handler(int w, int h);
    
    void PrintBuffer(const std::vector<Color>& frame_buffer);

private:
    int width;
    int height;
    HANDLE hConsole;
    std::vector<char> buf;
};