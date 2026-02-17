


//Not an in-game struct, but something the World-to-Screen function will return.
typedef struct
{
    float x;
    float y;
    float depth;
    float magnitude;
    float width;
    float height;
}
custom_rbx_world_to_screen_info_t;

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(camera, camera_subject, vm_address_t, RBX_CAMERA_CAMERASUBJECT_OFFSET)
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(camera, camera_subject, vm_address_t, RBX_CAMERA_CAMERASUBJECT_OFFSET)

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(camera, cframe, rbx_cframe_t, RBX_CAMERA_CFRAME_OFFSET)
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(camera, cframe, rbx_cframe_t, RBX_CAMERA_CFRAME_OFFSET)

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(camera, focus, vector3_t, RBX_CAMERA_CFRAME_OFFSET + sizeof(rbx_cframe_t))
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(camera, focus, vector3_t, RBX_CAMERA_CFRAME_OFFSET + sizeof(rbx_cframe_t))


float rbx_camera_get_field_of_view(task_t task, vm_address_t camera)
{
    return vm_read_float_value(task, camera + RBX_CAMERA_FIELDOFVIEW_OFFSET) * (180/PI);
}

void rbx_camera_set_field_of_view(task_t task, vm_address_t camera, float new_value)
{
    new_value = new_value / (180/PI);
    vm_write(task, camera + RBX_CAMERA_FIELDOFVIEW_OFFSET, (vm_address_t)&new_value, 4);
}










float rbx_get_camera_relative_depth(const rbx_cframe_t camera_cframe,
                                    const vector3_t position,
                                    float* inout_magnitude)
{
    const vector3_t camera_pos = camera_cframe.pos;
    const vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);

    float side_a = vector3_dist_dif(camera_pos, position);
    *inout_magnitude = side_a;
    float side_b = side_a;
    const vector3_t look_vector_offset =  vector3_mul_num(camera_look_vector, side_b);
    float side_c = vector3_dist_dif(vector3_add(camera_pos, look_vector_offset), position);
    
    float angle_c = acos((side_a * side_a + side_b * side_b - side_c * side_c) / (2 * side_a * side_b)) * (180/PI);
    /*float angle_a = 90;
    float angle_b = 180 - (angle_a + angle_c);*/
    
    const float depth = cos(angle_c / (180/PI)) * side_a; //side_a * sin(angle_b / (180/PI)) / sin(angle_a / (180/PI));
    /*
     See right triangle trigonometry
     */
    return depth;
}


NSSize rbx_get_camera_plane_size(const float fov,
                                 const float depth,
                                 const float window_width,
                                 const float window_height)
{
    const float aspect_ratio = window_width / window_height;
    
    //const angle_a = 90;
    const float angle_c = fov/2;
    const float angle_b = 180 - (angle_c + 90);

    //const float side_b = depth;
    const float side_c = depth * sin(angle_c / (180/PI)) / sin(angle_b / (180/PI));
    
    NSSize size;
    size.width = side_c * 2 * aspect_ratio;
    size.height = side_c * 2;
    
    return size;
}



custom_rbx_world_to_screen_info_t rbx_world_to_screen_point(rbx_cframe_t camera_cframe,
                                                            float field_of_view,
                                                            vector3_t target_pos,
                                                            float window_width,
                                                            float window_height,
                                                            float size_width,
                                                            float size_height)
{
    vector3_t camera_pos = camera_cframe.pos;
    vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);
    vector3_t camera_up_vector = rbx_cframe_get_up_vector(camera_cframe);
    vector3_t camera_right_vector = rbx_cframe_get_right_vector(camera_cframe);
    
    float magnitude;
    custom_rbx_world_to_screen_info_t wtsi;
    
    float depth = rbx_get_camera_relative_depth(camera_cframe, target_pos, &magnitude);
    
    //The target is behind the camera.
    if (depth < 0)
    {
        wtsi.x = NAN;
        wtsi.y = NAN;
        return wtsi;
    }
    
    NSSize plane_size = rbx_get_camera_plane_size(field_of_view, depth, window_width, window_height);
    
    float plane_size_x = plane_size.width;
    float plane_size_y = plane_size.height;
    
    float right_angle_sin_deg = sin(90 / (180/PI));
    
    vector3_t look_vector_offset = vector3_mul_num(camera_look_vector, depth);
    vector3_t right_vector_offset = vector3_mul_num(camera_right_vector, plane_size_x / 2);
    vector3_t up_vector_offset = vector3_mul_num(camera_up_vector, plane_size_y / 2);
    
    vector3_t center_pos = vector3_add(camera_pos, look_vector_offset);
    
    
    //Get the 3d world coordinates of where the corners of the screen are at a certain depth
    vector3_t r_side_pos = vector3_add(center_pos, right_vector_offset);
    vector3_t l_side_pos = vector3_sub(center_pos, right_vector_offset);
    
    //Top right
    vector3_t tr_corner_pos = vector3_add(r_side_pos, up_vector_offset);
    //Bottom left
    vector3_t bl_corner_pos = vector3_sub(l_side_pos, up_vector_offset);
    
    
    //Where the magic happens.

    //Triangle between the bottom left corner, the target, and some distance right of the corner.
    float side_a = vector3_dist_dif(bl_corner_pos, target_pos);
    float side_b = side_a;
    float side_c = vector3_dist_dif(vector3_add(bl_corner_pos, vector3_mul_num(camera_right_vector, side_a)), target_pos);
    
    //Right triangle between corner of screen and the target
    ///float angle_a = 90;
    float angle_c = acos((side_a * side_a + side_b * side_b - side_c * side_c) / (2 * side_a * side_b)) * (180/PI);
    float angle_b = 180 - (90 + angle_c);
    
    //The sides of the right triangle
    float dist_from_bottom = side_a * sin(angle_c / (180/PI)) / right_angle_sin_deg;
    float dist_from_left = side_a * sin(angle_b / (180/PI)) / right_angle_sin_deg;
    
    
    side_a = vector3_dist_dif(tr_corner_pos, target_pos);
    side_b = side_a;
    side_c = vector3_dist_dif(vector3_add(tr_corner_pos, vector3_mul_num(camera_right_vector, -side_a)), target_pos);
    
    angle_c = acos((side_a * side_a + side_b * side_b - side_c * side_c) / (2 * side_a * side_b)) * (180/PI);
    angle_b = 180 - (90 + angle_c);
    
    
    float dist_from_top = side_a * sin(angle_c / (180/PI)) / right_angle_sin_deg;

    
    float x_dist = dist_from_left;
    float y_dist = dist_from_bottom;
    
    //The initial triangle can be "upside down"
    if (dist_from_top > plane_size_y)
    {
        y_dist = -y_dist;
    }
    
       
    
    float x_ratio = x_dist / plane_size_x;
    float x = (window_width * x_ratio);
        
    float y_ratio = y_dist / plane_size_y;
    float y = (window_height * y_ratio);
    
    wtsi.x = x;
    wtsi.y = y;
    wtsi.depth = depth;
    wtsi.magnitude = magnitude;
    wtsi.width = (size_width/plane_size.width) * window_width;
    wtsi.height = (size_height/plane_size.height) * window_height;
    
    return wtsi;
}





custom_rbx_world_to_screen_info_t exp_rbx_world_to_screen_point(rbx_cframe_t camera_cframe,
                                                            float field_of_view,
                                                            vector3_t target_pos,
                                                            float window_width,
                                                            float window_height,
                                                            float size_width,
                                                            float size_height)
{
    vector3_t camera_pos = camera_cframe.pos;
    vector3_t camera_right = rbx_cframe_get_right_vector(camera_cframe);
    vector3_t camera_up = rbx_cframe_get_up_vector(camera_cframe);
    vector3_t camera_forward = rbx_cframe_get_look_vector(camera_cframe);

    vector3_t to_target = vector3_sub(target_pos, camera_pos);

    float x = vector3_dot(to_target, camera_right);
    float y = vector3_dot(to_target, camera_up);
    float z = vector3_dot(to_target, camera_forward); // depth

    custom_rbx_world_to_screen_info_t info;
    float magnitude = vector3_magnitude(to_target);
    info.magnitude = magnitude;
    info.depth = z;

    if (z <= 0.01f) { // Behind camera or too close
        info.x = NAN;
        info.y = NAN;
        return info;
    }

    float fov_rad = field_of_view * (PI / 180.0f);
    float aspect = window_width / window_height;

    float screen_x = (x / (z * tanf(fov_rad / 2.0f))) * (window_width / 2.0f) + window_width / 2.0f;
    float screen_y = (y / (z * tanf(fov_rad / 2.0f))) * (window_height / 2.0f) + window_height / 2.0f;

    info.x = screen_x;
    info.y = screen_y;

    // Optional: project size to screen dimensions
    float height_on_screen = (size_height / z) / tanf(fov_rad / 2.0f) * (window_height / 2.0f);
    float width_on_screen = height_on_screen * aspect * (size_width / size_height);

    info.width = width_on_screen;
    info.height = height_on_screen;

    return info;
}
