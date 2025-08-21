#pragma once

#include <windows.h>
#include <array>

enum class Key : uint8_t
{
    W, 
    A, 
    S, 
    D,
    Q,
    E,
    LEFTSHIFT,
    SPACE,
    ESC,
    COUNT
};

struct Mouse_Delta
{
    float dx, dy;
};

class Input_Handler
{
public:
    Input_Handler(HWND hwnd, bool lock_mouse);
    ~Input_Handler() = default;

    void Poll();

    bool IsKeyDown(Key k) const;

    Mouse_Delta GetMouseDelta() const;

private:
    HWND hwnd = nullptr;
    bool pointer_lock;
    float mouse_dx;
    float mouse_dy;
    std::array<bool, static_cast<size_t>(Key::COUNT)> key_down;

    static int ToVK(Key k);
    void UpdateKeyboard();
    void UpdateMouse();
};
