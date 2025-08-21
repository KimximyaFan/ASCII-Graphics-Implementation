class Input_Handler
{
public:
    // hwnd==nullptr이면 콘솔 창 핸들을 자동 사용
    explicit Input_Handler(HWND hwnd = nullptr, bool lockMouse = true);
    ~Input_Handler();

    // 수명 관리
    void Initialize();
    void Shutdown();

    // 매 프레임 1회 호출
    void Poll();

    // 키 상태
    bool IsKeyDown(Key k) const;
    bool WasKeyPressed(Key k) const;    // 이번 프레임에 눌림(엣지)
    bool WasKeyReleased(Key k) const;   // 이번 프레임에 떼짐(엣지)

    // 마우스 상대 이동(프레임 누적)
    void GetMouseDelta(float& dx, float& dy) const; // 반환 후 값 유지(초기화X)

    // 기존 API 호환
    bool IsSpacePressed() const { return IsKeyDown(Key::Space); }

    // 옵션
    void SetPointerLock(bool enable);   // 런타임 토글
    void SetMouseSensitivity(float s);  // 기본 1.0
    void SetHWND(HWND hwnd);            // 창 전환 시

private:
    static int  ToVK(Key k);
    bool        HasFocus_() const;
    void        UpdateKeyboard_();
    void        UpdateMouse_();

private:
    HWND  hwnd_ = nullptr;
    bool  initialized_ = false;
    bool  pointerLock_ = true;
    bool  resetMouse_ = true;
    float mouseSensitivity_ = 1.0f;

    std::array<uint8_t, (size_t)Key::COUNT> curr_{};
    std::array<uint8_t, (size_t)Key::COUNT> prev_{};

    float mouseDX_ = 0.0f;
    float mouseDY_ = 0.0f;

    POINT prevPos_{0,0};   // lock=false 경로에서 사용
};





#include "Input_Handler.h"

static inline int clampi(int v, int lo, int hi){ return v<lo?lo:(v>hi?hi:v); }

Input_Handler::Input_Handler(HWND hwnd, bool lockMouse)
: hwnd_(hwnd ? hwnd : GetConsoleWindow())
, pointerLock_(lockMouse)
{
    // 배열/상태는 멤버 초기화자로 0
}

Input_Handler::~Input_Handler() {
    Shutdown();
}

void Input_Handler::Initialize()
{
    if (initialized_) return;
    if (!hwnd_) hwnd_ = GetConsoleWindow();
    resetMouse_ = true;
    if (pointerLock_ && HasFocus_()) ShowCursor(FALSE);
    initialized_ = true;
}

void Input_Handler::Shutdown()
{
    if (!initialized_) return;
    if (pointerLock_) ShowCursor(TRUE);
    initialized_ = false;
}

void Input_Handler::SetHWND(HWND hwnd)
{
    hwnd_ = hwnd ? hwnd : GetConsoleWindow();
    resetMouse_ = true;
}

void Input_Handler::SetPointerLock(bool enable)
{
    if (pointerLock_ == enable) return;
    pointerLock_ = enable;
    resetMouse_ = true;
    if (!initialized_) return;
    if (pointerLock_ && HasFocus_()) ShowCursor(FALSE);
    else                             ShowCursor(TRUE);
}

void Input_Handler::SetMouseSensitivity(float s)
{
    mouseSensitivity_ = (s > 0.f ? s : 1.f);
}

bool Input_Handler::HasFocus_() const
{
    return hwnd_ && (GetForegroundWindow() == hwnd_);
}

int Input_Handler::ToVK(Key k)
{
    switch (k)
    {
        case Key::W:         return 'W';
        case Key::A:         return 'A';
        case Key::S:         return 'S';
        case Key::D:         return 'D';
        case Key::LeftShift: return VK_SHIFT; // 좌/우 구분 없이
        case Key::Space:     return VK_SPACE;
        case Key::Escape:    return VK_ESCAPE;
        case Key::Q:         return 'Q';
        case Key::E:         return 'E';
        default:             return 0;
    }
}

void Input_Handler::Poll()
{
    if (!initialized_) Initialize();
    UpdateKeyboard_();
    UpdateMouse_();
}

bool Input_Handler::IsKeyDown(Key k) const
{
    return curr_[(size_t)k] != 0;
}

bool Input_Handler::WasKeyPressed(Key k) const
{
    size_t i = (size_t)k;
    return (curr_[i] != 0) && (prev_[i] == 0);
}

bool Input_Handler::WasKeyReleased(Key k) const
{
    size_t i = (size_t)k;
    return (curr_[i] == 0) && (prev_[i] != 0);
}

void Input_Handler::GetMouseDelta(float& dx, float& dy) const
{
    dx = mouseDX_;
    dy = mouseDY_;
}

void Input_Handler::UpdateKeyboard_()
{
    prev_ = curr_;
    for (size_t i = 0; i < (size_t)Key::COUNT; ++i)
    {
        int vk = ToVK((Key)i);
        if (vk == 0) { curr_[i] = 0; continue; }
        SHORT st = GetAsyncKeyState(vk);
        curr_[i] = ( (st & 0x8000) != 0 ) ? 1 : 0;
    }
}

void Input_Handler::UpdateMouse_()
{
    mouseDX_ = 0.0f;
    mouseDY_ = 0.0f;

    if (!HasFocus_()) {
        resetMouse_ = true;
        return;
    }

    if (pointerLock_ && hwnd_)
    {
        RECT rc; GetClientRect(hwnd_, &rc);
        // 콘솔 클라이언트 크기가 0일 수 있어 방지
        int cx = clampi((rc.left + rc.right)/2, rc.left, rc.right);
        int cy = clampi((rc.top  + rc.bottom)/2, rc.top,  rc.bottom);
        POINT centerClient{ cx, cy };
        POINT centerScreen = centerClient; ClientToScreen(hwnd_, &centerScreen);

        if (resetMouse_) {
            SetCursorPos(centerScreen.x, centerScreen.y);
            resetMouse_ = false;
            return; // 첫 프레임 델타 0
        }

        POINT p; GetCursorPos(&p);
        mouseDX_ = float(p.x - centerScreen.x) * mouseSensitivity_;
        mouseDY_ = float(p.y - centerScreen.y) * mouseSensitivity_;

        SetCursorPos(centerScreen.x, centerScreen.y);
    }
    else
    {
        POINT p; GetCursorPos(&p);
        if (resetMouse_) {
            prevPos_ = p;
            resetMouse_ = false;
            return;
        }
        mouseDX_ = float(p.x - prevPos_.x) * mouseSensitivity_;
        mouseDY_ = float(p.y - prevPos_.y) * mouseSensitivity_;
        prevPos_ = p;
    }
}
