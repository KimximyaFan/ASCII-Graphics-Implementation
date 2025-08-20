Projected_Vertex Renderer::ProjectVertex (const Vertex& v)
{
    
    //    Clip Position -> Nomalized Device Coordinate
    
    float invW = 1.0f / v.position.w;
    float ndcX = v.position.x * invW;
    float ndcY = v.position.y * invW;
    float ndcZ = v.position.z * invW;

    
    //    overhead
    
    //Mat4x4 viewport_matrix = Mat4x4::ViewportTransformation(0, width, 0, height);
    
    
    //    NDC -> Screen Coordinate
    
    Projected_Vertex out;
    out.invW = invW;
    out.x = (ndcX*0.5f + 0.5f) * width;
    // 배열의 y축은 위에서 아래로 가기때문에, y를 뒤집어줘야함, height - y 
    out.y = (1.0f - (ndcY*0.5f + 0.5f)) * height;
    out.z_ndc = ndcZ;
    out.z = ndcZ*0.5f + 0.5f;
    out.r_over_w = v.color.r * invW;
    out.g_over_w = v.color.g * invW;
    out.b_over_w = v.color.b * invW;
    out.u_over_w = v.uv.x * invW;
    out.v_over_w = v.uv.y * invW;
    out.u = v.uv.x;
    out.v = v.uv.y;

    return out;
}


void Renderer::RasterizeTriangle (const Vertex& v0, const Vertex& v1, const Vertex& v2, const Texture* texture)
{
    Projected_Vertex A = ProjectVertex(v0);
    Projected_Vertex B = ProjectVertex(v1);
    Projected_Vertex C = ProjectVertex(v2);
    Projected_Vertex P;

    int x_min = std::max(0, (int)std::floor(std::min({A.x, B.x, C.x})));
    int x_max = std::min(width-1, (int)std::ceil(std::max({A.x, B.x, C.x})));
    int y_min = std::max(0, (int)std::floor(std::min({A.y, B.y, C.y})));
    int y_max = std::min(height-1, (int)std::ceil(std::max({A.y, B.y, C.y})));

    float area = GetTriangleSpace(A, B, C);

    if (fabs(area) < 1e-6f) return;

    float inv_area = 1.0f/area;

    for (int y=y_min; y<=y_max; ++y)
    {
        for (int x=x_min; x<=x_max; ++x)
        {
            P.x = x+0.5f;
            P.y = y+0.5f;

            float w0 = GetTriangleSpace(A, B, P) * inv_area;
            float w1 = GetTriangleSpace(B, C, P) * inv_area;
            float w2 = GetTriangleSpace(C, A, P) * inv_area;

            if ( w0 < 0 || w1 < 0 || w2 < 0)
                continue;

            //float denom = w0*A.invW + w1*B.invW + w2*C.invW;

            //if (denom <= 0.0f) continue;

            //float z = w0*A.z + w1*B.z + w2*C.z;
            
            float z_ndc = w0*A.z_ndc + w1*B.z_ndc + w2*C.z_ndc;   // [-1,1]
            float z     = z_ndc * 0.5f + 0.5f;

            //float z = 1.0f / (w0/A.z + w1/B.z + w2/C.z);

            int index = y*width + x;
            
            Color col;

            col.r = ((w0/A.z)*A.color.r + (w1/B.z)*B.color.r + (w2/C.z)*C.color.r) * z;
            col.g = ((w0/A.z)*A.color.g + (w1/B.z)*B.color.g + (w2/C.z)*C.color.g) * z;
            col.b = ((w0/A.z)*A.color.b + (w1/B.z)*B.color.b + (w2/C.z)*C.color.b) * z;
            col.a = ((w0/A.z)*A.color.a + (w1/B.z)*B.color.a + (w2/C.z)*C.color.a) * z;

            float buffer_intensity = frame_buffer[index].r + frame_buffer[index].g + frame_buffer[index].b;
            float current_intensity = col.r + col.g + col.b;

            if ( buffer_intensity < current_intensity )
                frame_buffer[index] = col;
            
            
            if ( z < z_buffer[index] )
            {
                float denom = w0*A.invW + w1*B.invW + w2*C.invW;

                //if (!(denom > 0.0f) || !std::isfinite(denom)) continue;
                if (!std::isfinite(denom) || std::fabs(denom) < 1e-12f) continue;

                z_buffer[index] = z;

                Color col;
                
                //col.r = ((w0/A.z)*A.color.r + (w1/B.z)*B.color.r + (w2/C.z)*C.color.r) * z;
                //col.g = ((w0/A.z)*A.color.g + (w1/B.z)*B.color.g + (w2/C.z)*C.color.g) * z;
                //col.b = ((w0/A.z)*A.color.b + (w1/B.z)*B.color.b + (w2/C.z)*C.color.b) * z;
                //col.a = ((w0/A.z)*A.color.a + (w1/B.z)*B.color.a + (w2/C.z)*C.color.a) * z;
                
                float u = (w0*A.u_over_w + w1*B.u_over_w + w2*C.u_over_w) / denom;
                float v = (w0*A.v_over_w + w1*B.v_over_w + w2*C.v_over_w) / denom;

                //float u = w0*A.u + w1*B.u + w2*C.u;
                //float v = w0*A.v + w1*B.v + w2*C.v;

                Color tex = texture ? texture->Sample(u, v) : Color{1,1,1,1};
                
                col.r = (w0*A.r_over_w + w1*B.r_over_w + w2*C.r_over_w) / denom;
                col.g = (w0*A.g_over_w + w1*B.g_over_w + w2*C.g_over_w) / denom;
                col.b = (w0*A.b_over_w + w1*B.b_over_w + w2*C.b_over_w) / denom;
                col.a = 1.0f;

                Color out = col * tex;
                //col.a = (w0*A.color.a*A.invW + w1*B.color.a*B.invW + w2*C.color.a*C.invW) / denom;
                
                frame_buffer[index] = out;
            }
            
        }
    }
}