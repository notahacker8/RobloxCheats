


///Vector 3 structure for most, if not all 3D games. (Size: 12)
typedef struct
{
    float x;
    float y;
    float z;
}
vector3_t;


vector3_t vector3_init(const float x,
                       const float y,
                       const float z)
{
    vector3_t _;
    _.x = x;
    _.y = y;
    _.z = z;
    return _;
}

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


vector3_t vector3_unit(const vector3_t _1,
                       const vector3_t _2)
{
    const vector3_t dif = vector3_sub(_1, _2);
    const float mag = vector3_dist_dif(_1, _2);
    const vector3_t unit = vector3_div_num(dif, mag);
    return unit;
}


vector3_t vector3_cross_product(const vector3_t _1,
                                const vector3_t _2)
{
    return vector3_init(_1.y * _2.z - _1.z * _2.y, _1.z * _2.x - _1.x * _2.z, _1.x * _2.y - _1.y * _2.x);
}


#define VECTOR3_ORIGIN (vector3_init(0, 0, 0))
