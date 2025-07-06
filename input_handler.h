#pragma once

#include <windows.h>

class Input_Handler
{
public:
    static bool IsSpacePressed()
    {
        return (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
    }
};
