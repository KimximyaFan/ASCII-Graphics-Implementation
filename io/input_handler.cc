
#include <iostream>
#include "input_handler.h"
#include <algorithm>

Input_Handler::Input_Handler(HWND hwnd, bool lock_mouse)
: hwnd(hwnd ? hwnd : GetConsoleWindow()), pointer_lock(lock_mouse)
{
    key_down.fill(false);
}

int Input_Handler::ToVK(Key k)
{
    switch (k)
    {
        case Key::W: return 'W';
        case Key::A: return 'A';
        case Key::S: return 'S';
        case Key::D: return 'D';
        case Key::Q: return 'Q';
        case Key::E: return 'E';
        case Key::LEFTSHIFT: return VK_SHIFT;
        case Key::SPACE: return VK_SPACE;
        case Key::ESC: return VK_ESCAPE;
        default: return 0;
    }
}

void Input_Handler::UpdateKeyboard()
{
    for (size_t i = 0; i < static_cast<size_t>(Key::COUNT); ++i)
        key_down[i] = (GetAsyncKeyState(ToVK(static_cast<Key>(i))) & 0x8000) != 0;

    /*
    for (size_t i = 0; i < static_cast<size_t>(Key::COUNT); ++i)
    {
        int vk = ToVK(static_cast<Key>(i));

        if (vk == 0)
        {
            key_down[i] = false;
            continue;
        }
        
        key_down[i] = (GetAsyncKeyState(vk) & 0x8000) != 0;
    }
    */
}

void Input_Handler::UpdateMouse()
{
    mouse_dx = 0.0f;
    mouse_dy = 0.0f;

    if (pointer_lock == false)
        return;
    
    RECT rc;
    GetClientRect(hwnd, &rc);

    int cx = std::clamp((rc.left + rc.right)/2, rc.left, rc.right);
    int cy = std::clamp((rc.top + rc.bottom)/2, rc.top, rc.bottom);

    POINT center_screen = { cx, cy };
    ClientToScreen(hwnd, &center_screen);

    POINT p;
    GetCursorPos(&p);

    mouse_dx = float(p.x - center_screen.x);
    mouse_dy = float(p.y - center_screen.y);

    SetCursorPos(center_screen.x, center_screen.y);
}

void Input_Handler::Poll()
{
    UpdateKeyboard();
    UpdateMouse();
}