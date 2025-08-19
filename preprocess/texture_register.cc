#include "texture_register.h"
#include "material/procedural_texture.h"

bool TextureRegisterPreprocess(Texture_Manager& texture_manager)
{
    bool ok = true;
    Sampler_Desc s{};

    int W = 256, H = 256;
    auto pix = Procedural_Texture::BakeChecker(W, H, true);
    ok &= texture_manager.RegisterProcedural(Texture_Handle::CHECKER, std::move(pix), W, H, s);

    int W = 256, H = 256;
    auto pix = Procedural_Texture::BakeBrick(W, H);
    ok &= texture_manager.RegisterProcedural(Texture_Handle::BRICK, std::move(pix), W, H, s);

    return ok;
}