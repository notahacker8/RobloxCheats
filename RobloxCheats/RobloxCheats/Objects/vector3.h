


///Vector 3 structure for most, if not all 3D games. (Size: 12)
typedef struct
{
    float x;
    float y;
    float z;
}
vector3_t;


vector3_t vector3_mul(const vector3_t _1,
                      const vector3_t _2)
{
    vector3_t result = _1;
    result.x *= _2.x;
    result.y *= _2.y;
    result.z *= _2.z;
    return result;
}

vector3_t vector3_mul_num(const vector3_t _1,
                          const float _2)
{
    vector3_t result = _1;
    result.x *= _2;
    result.y *= _2;
    result.z *= _2;
    return result;
}

vector3_t vector3_div(const vector3_t _1,
                      const vector3_t _2)
{
    vector3_t result = _1;
    result.x /= _2.x;
    result.y /= _2.y;
    result.z /= _2.z;
    return result;
}

vector3_t vector3_div_num(const vector3_t _1,
                          const float _2)
{
    vector3_t result = _1;
    result.x /= _2;
    result.y /= _2;
    result.z /= _2;
    return result;
}

vector3_t vector3_add(const vector3_t _1,
                      const vector3_t _2)
{
    vector3_t result = _1;
    result.x += _2.x;
    result.y += _2.y;
    result.z += _2.z;
    return result;
}

vector3_t vector3_add_num(const vector3_t _1,
                          const float _2) {
    vector3_t result = _1;
    result.x += _2;
    result.y += _2;
    result.z += _2;
    return result;
}

vector3_t vector3_sub(const vector3_t _1,
                      const vector3_t _2)
{
    vector3_t result = _1;
    result.x -= _2.x;
    result.y -= _2.y;
    result.z -= _2.z;
    return result;
}

vector3_t vector3_sub_num(const vector3_t _1,
                          const float _2)
{
    vector3_t result = _1;
    result.x -= _2;
    result.y -= _2;
    result.z -= _2;
    return result;
}

float vector3_dist_dif(const vector3_t _1,
                       const vector3_t _2)
{
    const vector3_t dif = vector3_sub(_1, _2);
    return sqrtf((dif.x * dif.x) + (dif.y * dif.y) + (dif.z * dif.z));
}


float vector3_magnitude(const vector3_t v)
{
    return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vector3_t vector3_normalized(const vector3_t v)
{
    float m = vector3_magnitude(v);
    vector3_t r = {.x = 0, .y = 0, .z = 0};
    if (m > 0)
    {
        r.x = v.x / m;
        r.y = v.y / m;
        r.z = v.z / m;
    }
    return r;
}

/*
 Get the direction to a position from a start position.
 */
vector3_t vector3_direction_to_from(const vector3_t to,
                                    const vector3_t from)
{
    const vector3_t dif = vector3_sub(to, from);
    const float mag = vector3_dist_dif(to, from);
    const vector3_t unit = vector3_div_num(dif, mag);
    return unit;
}

/**
 What????
 */
vector3_t vector3_cross_product(const vector3_t _1,
                                const vector3_t _2)
{
    return (vector3_t){.x = _1.y * _2.z - _1.z * _2.y, .y = _1.z * _2.x - _1.x * _2.z, .z = _1.x * _2.y - _1.y * _2.x};
}


