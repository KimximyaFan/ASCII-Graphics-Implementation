#include "procedural_texture.h"
#include <cmath>
#include <algorithm>

std::vector<Color> Procedural_Texture::BakeChecker(int W, int H, bool antialias)
{
    if (W <= 0 || H <= 0) return {};
    std::vector<Color> pixels(static_cast<size_t>(W) * static_cast<size_t>(H));

    // 기본값
    constexpr int   tilesX = 8;
    constexpr int   tilesY = 8;
    const Color A{0.92f, 0.92f, 0.92f, 1.0f};
    const Color B{0.08f, 0.08f, 0.08f, 1.0f};

    const float du = antialias ? 0.25f / static_cast<float>(W) : 0.0f;
    const float dv = antialias ? 0.25f / static_cast<float>(H) : 0.0f;

    auto sample = [&](float u, float v) -> Color {
        // [0,1)로 래핑
        u -= std::floor(u);
        v -= std::floor(v);
        int iu = static_cast<int>(std::floor(u * tilesX));
        int iv = static_cast<int>(std::floor(v * tilesY));
        return (((iu ^ iv) & 1) == 0) ? A : B;
    };

    for (int y = 0; y < H; ++y) {
        float v0 = (y + 0.5f) / static_cast<float>(H);
        for (int x = 0; x < W; ++x) {
            float u0 = (x + 0.5f) / static_cast<float>(W);
            Color c;
            if (!antialias) {
                c = sample(u0, v0);
            } else {
                // 2x2 슈퍼샘플
                Color c00 = sample(u0 - du, v0 - dv);
                Color c10 = sample(u0 + du, v0 - dv);
                Color c01 = sample(u0 - du, v0 + dv);
                Color c11 = sample(u0 + du, v0 + dv);
                c = {
                    (c00.r + c10.r + c01.r + c11.r) * 0.25f,
                    (c00.g + c10.g + c01.g + c11.g) * 0.25f,
                    (c00.b + c10.b + c01.b + c11.b) * 0.25f,
                    (c00.a + c10.a + c01.a + c11.a) * 0.25f
                };
            }
            pixels[static_cast<size_t>(y) * W + x] = c;
        }
    }
    return pixels;
}

std::vector<Color> Procedural_Texture::BakeBrick(int W, int H)
{
    if (W <= 0 || H <= 0) return {};
    std::vector<Color> pixels(static_cast<size_t>(W) * static_cast<size_t>(H));

    // 기본값
    constexpr int   bricksX   = 10;
    constexpr int   bricksY   = 6;
    constexpr float mortar    = 0.07f;   // 0..0.49 (셀 경계 폭 비율)
    constexpr float variation = 0.12f;   // 0..1   (벽돌 밝기 변조)
    const Color brickA {0.63f, 0.21f, 0.16f, 1.0f};
    const Color brickB {0.55f, 0.18f, 0.12f, 1.0f};
    const Color mortarC{0.80f, 0.80f, 0.80f, 1.0f};

    auto hash2i01 = [](int x, int y) -> float {
        uint32_t h = static_cast<uint32_t>(x) * 374761393u + static_cast<uint32_t>(y) * 668265263u;
        h = (h ^ (h >> 13)) * 1274126177u;
        h ^= (h >> 16);
        return static_cast<float>(h) * (1.0f / 4294967295.0f); // 0..1
    };

    for (int y = 0; y < H; ++y) {
        float v0 = (y + 0.5f) / static_cast<float>(H);
        for (int x = 0; x < W; ++x) {
            float u0 = (x + 0.5f) / static_cast<float>(W);

            // [0,1) 래핑
            float u = u0 - std::floor(u0);
            float v = v0 - std::floor(v0);

            // 셀 좌표
            float su = u * bricksX;
            float sv = v * bricksY;

            int   row   = static_cast<int>(sv);
            float suOff = su + ((row & 1) ? 0.5f : 0.0f); // 홀수 줄 반칸 시프트
            int   col   = static_cast<int>(suOff);

            float fu = suOff - static_cast<float>(col); // 0..1
            float fv = sv    - static_cast<float>(row); // 0..1

            float m = std::clamp(mortar, 0.0f, 0.49f);
            // 경계까지 거리(양쪽 중 더 가까운 쪽)
            float du = (fu < 1.0f - fu) ? fu : (1.0f - fu);
            float dv = (fv < 1.0f - fv) ? fv : (1.0f - fv);

            if (du < m || dv < m) {
                pixels[static_cast<size_t>(y) * W + x] = mortarC;
                continue;
            }

            // 벽돌 색 + 변조
            bool  useA = ((row ^ col) & 1) == 0;
            Color base = useA ? brickA : brickB;

            float rnd = hash2i01(col, row) * 2.0f - 1.0f;             // -1..1
            float t   = std::clamp(0.5f + rnd * variation, 0.0f, 1.0f);

            Color dark{ base.r * 0.7f, base.g * 0.7f, base.b * 0.7f, base.a };
            Color lite{ base.r * 1.2f, base.g * 1.2f, base.b * 1.2f, base.a };

            // lerp
            Color out;
            out.r = dark.r + (lite.r - dark.r) * t;
            out.g = dark.g + (lite.g - dark.g) * t;
            out.b = dark.b + (lite.b - dark.b) * t;
            out.a = dark.a + (lite.a - dark.a) * t;

            pixels[static_cast<size_t>(y) * W + x] = out;
        }
    }
    return pixels;
}
