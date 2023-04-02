

typedef struct
{
    float r0;           // + 0x0
    float r10;          // + 0x4
    float r20;          // + 0x8
    float r1;           // + 0xc
    float r11;          // + 0x10
    float r21;          // + 0x14
    float r2;           // + 0x18
    float r12;          // + 0x1c
    float r22;          // + 0x20
    vector3_t pos;      // + 0x24
}
rbx_cframe_t;

vector3_t rbx_cframe_get_look_vector(const rbx_cframe_t cframe)
{
    return (vector3_t){.x = -cframe.r20, .y = -cframe.r21, .z = -cframe.r22};
}

vector3_t rbx_cframe_get_up_vector(const rbx_cframe_t cframe)
{
    return (vector3_t){.x = cframe.r10, .y = cframe.r11, .z = cframe.r12};
}

vector3_t rbx_cframe_get_right_vector(const rbx_cframe_t cframe)
{
    return (vector3_t){.x = cframe.r0, .y = cframe.r1, .z = cframe.r2};
}

rbx_cframe_t rbx_cframe_from_pos_and_look_vector(const vector3_t pos,
                                                 const vector3_t lv)
{
    rbx_cframe_t cf;
    cf.r20 = -lv.x;
    cf.r21 = -lv.y;
    cf.r22 = -lv.z;
    vector3_t rv = (vector3_t){.x = -lv.z, .y = 0.00000000001, .z = lv.x};
    float rv_mag = vector3_magnitude(rv);
    rv = vector3_div_num(rv, rv_mag);
    cf.r0 = rv.x;
    cf.r1 = rv.y;
    cf.r2 = rv.z;
    vector3_t uv = vector3_cross_product(lv, rv);
    cf.r10 = -uv.x;
    cf.r11 = -uv.y;
    cf.r12 = -uv.z;
    cf.pos = pos;
    return cf;
}
