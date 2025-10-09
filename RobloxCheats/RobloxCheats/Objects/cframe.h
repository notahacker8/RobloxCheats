

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

const rbx_cframe_t DEFAULT_CFRAME = {
    .r0 = 1, .r10 = 0, .r20 = 0,
    .r1 = 0, .r11 = 1, .r21 = 0,
    .r2 = 0, .r12 = 0, .r22 = 1,
    .pos = {.x = 0, .y = 0, .z = 0}
};

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


// Build rbx_cframe_t from LookVector and position
rbx_cframe_t rbx_cframe_from_look(vector3_t look, vector3_t position) {
    rbx_cframe_t result;
    
    look.x = look.x;
    look.y = look.y;
    look.z = look.z;

    vector3_t forward = vector3_normalized(look);
    vector3_t world_up = {0, 1, 0};

    // If forward is near-parallel to up, change up vector
    if (fabsf(forward.x) < 0.0001f && fabsf(forward.z) < 0.0001f) {
        world_up = (vector3_t){0, 0, 1};
    }

    vector3_t right = vector3_normalized(vector3_cross(world_up, forward));
    vector3_t up = vector3_cross(forward, right); // no need to normalize here

    // Fill in rotation matrix (column-major order)
    result.r0 = right.x;
    result.r10 = right.y;
    result.r20 = right.z;

    result.r1 = up.x;
    result.r11 = up.y;
    result.r21 = up.z;

    result.r2 = forward.x;
    result.r12 = forward.y;
    result.r22 = forward.z;

    // Fill position
    result.pos = position;

    return result;
}
