
#include "frustum.h"

/*
    | Xh |   |     row 0     |     | X | 
    | Yh | = |     row 1     |  *  | Y |
    | Zh |   |     row 2     |     | Z |
    | Wh |   |     row 3     |     | 1 |

    Wh + Xh >= 0 ... row 3 + row 0
    Wh - Xh >= 0 ... row 3 - row 0
    Wh + Yh >= 0 ... row 3 + row 1
    Wh - Yh >= 0 ... row 3 - row 1
    Wh + Zh >= 0 ... row 3 + row 2
    Wh - Zh >= 0 ... row 3 - row 2

    Maybe, it can be made more clean code
    but, I think performance is more important than readability 
*/
void Frustum::ExtractFrustumPlanes(const Mat4x4& proj_view)
{
    frustum_planes[0] = Vec4(
        proj_view.m[3][0] + proj_view.m[0][0],
        proj_view.m[3][1] + proj_view.m[0][1],
        proj_view.m[3][2] + proj_view.m[0][2],
        proj_view.m[3][3] + proj_view.m[0][3]
    );
    frustum_planes[1] = Vec4(
        proj_view.m[3][0] - proj_view.m[0][0],
        proj_view.m[3][1] - proj_view.m[0][1],
        proj_view.m[3][2] - proj_view.m[0][2],
        proj_view.m[3][3] - proj_view.m[0][3]
    );
    frustum_planes[2] = Vec4(
        proj_view.m[3][0] + proj_view.m[1][0],
        proj_view.m[3][1] + proj_view.m[1][1],
        proj_view.m[3][2] + proj_view.m[1][2],
        proj_view.m[3][3] + proj_view.m[1][3]
    );
    frustum_planes[3] = Vec4(
        proj_view.m[3][0] - proj_view.m[1][0],
        proj_view.m[3][1] - proj_view.m[1][1],
        proj_view.m[3][2] - proj_view.m[1][2],
        proj_view.m[3][3] - proj_view.m[1][3]
    );
    frustum_planes[4] = Vec4(
        proj_view.m[3][0] + proj_view.m[2][0],
        proj_view.m[3][1] + proj_view.m[2][1],
        proj_view.m[3][2] + proj_view.m[2][2],
        proj_view.m[3][3] + proj_view.m[2][3]
    );
    frustum_planes[5] = Vec4(
        proj_view.m[3][0] - proj_view.m[2][0],
        proj_view.m[3][1] - proj_view.m[2][1],
        proj_view.m[3][2] - proj_view.m[2][2],
        proj_view.m[3][3] - proj_view.m[2][3]
    );

    for (auto& p : frustum_planes) {
        float len = std::sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
        p /= len;
    }
}

const std::array<Vec4, 6>& Frustum::GetFrustumPlanes() const
{
    return frustum_planes;
}