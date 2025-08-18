#pragma once
#include <unordered_map>
#include <memory>
#include <type_traits>
#include "texture.h"
#include "texture_handle.h"

// enum class 를 unordered_map 키로 쓰기 위한 해시
struct Enum_Class_Hash {
    template <class T>
    size_t operator()(T v) const noexcept {
        static_assert(std::is_enum<T>::value, "Enum_Class_Hash expects enum");
        using U = typename std::underlying_type<T>::type;
        return std::hash<U>{}(static_cast<U>(v));
    }
};

class Texture_Manager {
public:
    Texture_Manager() = default;
    ~Texture_Manager() = default;

    // ---------- 등록 API ----------
    // 1) 이미지 파일로 등록 (핸들/중복 보호)
    // FromFile 구현은 Image_Texture::FromFile에 위임.
    // 성공 시 true. 이미 존재하면 false (덮어쓰기 방지).
    bool Register_Image_From_File(Texture_Handle handle,
                                  const char* path,
                                  const Sampler_Desc& samp = {}) 
    {
        if (exists(handle)) return false;
        auto tex = Image_Texture::FromFile(path, samp);
        if (!tex) return false;
        map_.emplace(handle, std::move(tex));
        return true;
    }

    // 2) 메모리로부터 이미지 등록
    bool Register_Image_From_Memory(Texture_Handle handle,
                                    int w, int h,
                                    std::vector<Color> pixels,
                                    const Sampler_Desc& samp = {})
    {
        if (exists(handle)) return false;
        auto tex = std::make_unique<Image_Texture>(w, h, std::move(pixels), samp);
        map_.emplace(handle, std::move(tex));
        return true;
    }

    // 3) 코드(프로시저럴) 텍스처 등록
    bool Register_Procedural(Texture_Handle handle,
                             Procedural_Texture::SamplerFn fn,
                             int logicalW = 1, int logicalH = 1)
    {
        if (exists(handle)) return false;
        auto tex = std::make_unique<Procedural_Texture>(std::move(fn), logicalW, logicalH);
        map_.emplace(handle, std::move(tex));
        return true;
    }

    // ---------- 조회 ----------
    const Texture* Get(Texture_Handle handle) const {
        auto it = map_.find(handle);
        return (it == map_.end()) ? nullptr : it->second.get();
    }

    Texture* GetMutable(Texture_Handle handle) {
        auto it = map_.find(handle);
        return (it == map_.end()) ? nullptr : it->second.get();
    }

    bool IsRegistered(Texture_Handle handle) const {
        return map_.find(handle) != map_.end();
    }

    // 필요 시: 등록 취소(테스트용)
    bool Unregister(Texture_Handle handle) {
        return map_.erase(handle) > 0;
    }

private:
    bool exists(Texture_Handle h) const {
        return map_.find(h) != map_.end();
    }

    std::unordered_map<Texture_Handle, std::unique_ptr<Texture>, Enum_Class_Hash> map_;
};