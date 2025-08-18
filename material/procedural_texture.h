/*
#pragma once

#include "texture.h"
#include <cmath>

class Checker_Texture : public Texture
{
public:
    Checker_Texture(int w = 256, int h = 256)
        : Texture(Texture_Type::PROCEDURAL, w, h) {}

    Color Sample(float u, float v) const override {
        auto wrap01 = [](float t){ return t - std::floor(t); };
        float uu = wrap01(u), vv = wrap01(v);

        // 기본값
        const int tilesX = 8;
        const int tilesY = 8;
        const Color A{0.92f, 0.92f, 0.92f, 1.0f};
        const Color B{0.08f, 0.08f, 0.08f, 1.0f};

        int iu = (int)std::floor(uu * tilesX);
        int iv = (int)std::floor(vv * tilesY);
        bool on = ((iu ^ iv) & 1) == 0;
        return on ? A : B;
    }
};

class Brick_Texture : public Texture
{
public:
    Brick_Texture(int w = 512, int h = 512)
        : Texture(Texture_Type::PROCEDURAL, w, h) {}

    Color Sample(float u, float v) const override {
        // 기본값
        const int   bricksX   = 10;
        const int   bricksY   = 6;
        const float mortar    = 0.07f;                 // 0..0.49 (셀 비율)
        const float variation = 0.12f;                 // 0..1   (밝기 변조)
        const Color brickA {0.63f, 0.21f, 0.16f, 1.0f};
        const Color brickB {0.55f, 0.18f, 0.12f, 1.0f};
        const Color mortarC{0.80f, 0.80f, 0.80f, 1.0f};

        auto wrap01 = [](float t){ return t - std::floor(t); };
        auto fract  = [](float x){ return x - std::floor(x); };
        auto clamp01= [](float x){ return x < 0.f ? 0.f : (x > 1.f ? 1.f : x); };
        auto lerpf  = [](float a, float b, float t){ return a + (b - a) * t; };
        auto lerpC  = [&](const Color& a, const Color& b, float t){
            return Color{ lerpf(a.r,b.r,t), lerpf(a.g,b.g,t), lerpf(a.b,b.b,t), lerpf(a.a,b.a,t) };
        };
        // 셀별 작은 난수(빠른 해시)
        auto hash2i = [](int x, int y){
            uint32_t h = (uint32_t)x * 374761393u + (uint32_t)y * 668265263u;
            h = (h ^ (h >> 13)) * 1274126177u;
            h ^= (h >> 16);
            return (float)h * (1.0f/4294967295.0f); // 0..1
        };

        float uu = u * bricksX;
        float vv = v * bricksY;

        int row = (int)std::floor(vv);
        float fu = fract(uu + ((row & 1) ? 0.5f : 0.0f)); // 홀수 줄 반칸 시프트
        float fv = fract(vv);

        const float m = mortar < 0.f ? 0.f : (mortar > 0.49f ? 0.49f : mortar);
        if (fu < m || fu > 1.0f - m || fv < m || fv > 1.0f - m) {
            return mortarC; // 모르타르
        }

        int col = (int)std::floor(uu + ((row & 1) ? 0.5f : 0.0f));
        bool useA = ((row ^ col) & 1) == 0;
        Color base = useA ? brickA : brickB;

        float rnd = hash2i(col, row) * 2.0f - 1.0f; // -1..1
        float t = clamp01(0.5f + rnd * variation);

        Color dark{ base.r*0.7f, base.g*0.7f, base.b*0.7f, base.a };
        Color lite{ base.r*1.2f, base.g*1.2f, base.b*1.2f, base.a };
        return lerpC(dark, lite, t);
    }
};
*/