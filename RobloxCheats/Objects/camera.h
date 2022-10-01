

#define __RBX_TOTAL_Y_GUI_INSET 76
#define __RBX_TOTAL_X_GUI_INSET 0

typedef struct
{
    float x;
    float y;
    float depth;
    float magnitude;
}
custom_rbx_world_to_screen_info_t;

vm_address_t rbx_camera_get_camera_subject(task_t task, vm_address_t camera)
{
    if (camera == 0) { return 0; }
    vm_address_t* subject;
    vm_address_t _subject = 0;
    kern_return_t kr;
    static mach_msg_type_number_t data_cnt;
    kr = vm_read(task, camera + RBX_CAMERA_CAMERA_SUBJECT_OFFSET, 8, (vm_offset_t*)&subject, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        _subject = *subject;
        vm_deallocate(mach_task_self_, (vm_address_t)subject, 8);
    }
    return _subject;
}

rbx_cframe_t rbx_camera_get_cframe(task_t task, vm_address_t camera)
{
    rbx_cframe_t _cf;
    bzero(&_cf, sizeof(_cf));
    rbx_cframe_t* cf;
    if (camera == 0) {return _cf;}
    static mach_msg_type_number_t data_cnt;
    vm_read(task, camera + RBX_CAMERA_CFRAME_0_OFFSET, sizeof(rbx_cframe_t), (vm_offset_t*)&cf, &data_cnt);
    _cf = *cf;
    vm_deallocate(mach_task_self_, (vm_address_t)cf, sizeof(rbx_cframe_t));
    return _cf;
}

void rbx_camera_set_cframe(task_t task, vm_address_t camera, rbx_cframe_t* cf)
{
    if (camera == 0) {return;}
    vm_write(task, camera + RBX_CAMERA_CFRAME_0_OFFSET, (vm_address_t)cf, sizeof(rbx_cframe_t));
    return;
}

float rbx_camera_get_field_of_view(task_t task, vm_address_t camera)
{
    if (camera == 0) { return NAN; }
    float* fov;
    float _fov;
    static mach_msg_type_number_t data_cnt;
    vm_read(task, camera + RBX_CAMERA_FOV_OFFSET, 4, (vm_offset_t*)&fov, &data_cnt);
    _fov = *fov;
    vm_deallocate(mach_task_self_, (vm_address_t)fov, 4);
    return _fov * (180/PI);
}

void rbx_camera_set_field_of_view(task_t task, vm_address_t camera, float new_value)
{
    if (camera == 0) {return;}
    new_value = new_value / (180/PI);
    vm_write(task, camera + RBX_CAMERA_FOV_OFFSET, (vm_address_t)&new_value, 4);
}


float rbx_get_camera_relative_depth(const rbx_cframe_t camera_cframe,
                                    const vector3_t position,
                                    float* inout_magnitude)
{
    const vector3_t camera_pos = camera_cframe.pos;
    const vector3_t camera_look_vector = rbx_get_cframe_look_vector(camera_cframe);

    float side_a = vector3_dist_dif(camera_pos, position);
    *inout_magnitude = side_a;
    float side_b = side_a;
    const vector3_t look_vector_offset =  vector3_mul_num(camera_look_vector, side_b);
    float side_c = vector3_dist_dif(vector3_add(camera_pos, look_vector_offset), position);
    
    float angle_c = acos((side_a * side_a + side_b * side_b - side_c * side_c) / (2 * side_a * side_b)) * (180/PI);
    float angle_a = 90;
    float angle_b = 180 - (angle_a + angle_c);
    
    const float depth = side_a * sin(angle_b / (180/PI)) / sin(angle_a / (180/PI));
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
                                                            float window_height)
{
    vector3_t camera_pos = camera_cframe.pos;
    vector3_t camera_look_vector = rbx_get_cframe_look_vector(camera_cframe);
    vector3_t camera_up_vector = rbx_get_cframe_up_vector(camera_cframe);
    vector3_t camera_right_vector = rbx_get_cframe_right_vector(camera_cframe);
    float magnitude;
    custom_rbx_world_to_screen_info_t wtsi;
    
    float depth = rbx_get_camera_relative_depth(camera_cframe, target_pos, &magnitude);
    
    //The target is behind the camera.
    if (depth < 0)
    {
        wtsi.x = 0.0f/0.0f;
        wtsi.y = 0.0f/0.0f;
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
    
    
    vector3_t r_side_pos = vector3_add(center_pos, right_vector_offset);
    vector3_t l_side_pos = vector3_sub(center_pos, right_vector_offset);
    
    vector3_t tr_corner_pos = vector3_add(r_side_pos, up_vector_offset);
    vector3_t bl_corner_pos = vector3_sub(l_side_pos, up_vector_offset);
    
    
    //Where the magic happens.

    
    float side_a = vector3_dist_dif(bl_corner_pos, target_pos);
    float side_b = side_a;
    float side_c = vector3_dist_dif(vector3_add(bl_corner_pos, vector3_mul_num(camera_right_vector, side_a)), target_pos);
    
    float angle_c = acos((side_a * side_a + side_b * side_b - side_c * side_c) / (2 * side_a * side_b)) * (180/PI);
    float angle_b = 180 - (90 + angle_c);
    
    
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
    
    
    if (dist_from_top > plane_size_y)
    {
        y_dist = -y_dist;
    }
    
       
    
    float x_ratio = x_dist / plane_size_x;
    float x = (window_width * x_ratio);
        
    float y_ratio = y_dist / plane_size_y;
    float y = (window_height * y_ratio);
    
    wtsi.x = x;
    //wtsi.y = (y - (__RBX_TOTAL_Y_GUI_INSET/2));
    wtsi.y = y;
    wtsi.depth = depth;
    wtsi.magnitude = magnitude;
    
    return wtsi;
}


