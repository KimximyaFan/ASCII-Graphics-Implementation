#include <vector.h>

class Mat4x4
{
public:

    float m[4][4] = {};

    Mat4x4 () {}

    Mat4x4 (float value)
    {
        for (int i=0; i<4; i++)
        {
            for (int j=0; j<4; j++)
                m[i][j] = value;
        }
    }

    Mat4x4 (const float data[16])
    {
        for (int i=0; i<4; i++)
            for (int j=0; j<4; j++)
                m[i][j] = data[ i*4 + j ];
    }

    Mat4x4 operator* (Mat4x4& other) const
    {

    }
};