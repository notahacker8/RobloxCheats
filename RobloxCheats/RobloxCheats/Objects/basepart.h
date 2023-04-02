


vm_address_t rbx_basepart_get_properties_address(task_t task, vm_address_t basepart)
{
    return vm_read_8byte_value(task, basepart + RBX_PART_PROPERTIES_OFFSET);
}


float rbx_basepart_get_gravity(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return ((int_float_u)vm_read_4byte_value(task, p_a + RBX_PART_PROPERTIES_GRAVITY_OFFSET)).f;
}

void rbx_basepart_set_gravity(task_t task, vm_address_t basepart, float new_value)
{
    if (basepart == 0) {return;}
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_PART_PROPERTIES_GRAVITY_OFFSET, (vm_address_t)&new_value, 4);
}


rbx_cframe_t rbx_basepart_get_cframe(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_rbx_cframe_value(task, p_a + RBX_PART_PROPERTIES_CFRAME_OFFSET);
}

void rbx_basepart_set_cframe(task_t task, vm_address_t basepart, rbx_cframe_t cf)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_PART_PROPERTIES_CFRAME_OFFSET, (vm_address_t)&cf, sizeof(rbx_cframe_t));
    return;
}


vector3_t rbx_basepart_get_velocity(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_vector3_value(task, p_a + RBX_PART_PROPERTIES_CFRAME_OFFSET + sizeof(rbx_cframe_t));
}

void rbx_basepart_set_velocity(task_t task, vm_address_t basepart, vector3_t new_velocity)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_PART_PROPERTIES_CFRAME_OFFSET + sizeof(rbx_cframe_t), (vm_address_t)&new_velocity, sizeof(vector3_t));
}


vector3_t rbx_basepart_get_size(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_vector3_value(task, p_a + RBX_PART_PROPERTIES_SIZE_OFFSET);
}

void rbx_basepart_set_size(task_t task, vm_address_t basepart, vector3_t new_size)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_PART_PROPERTIES_SIZE_OFFSET, (vm_address_t)&new_size, sizeof(vector3_t));
}


rbx_rgb_t rbx_basepart_get_color(task_t task, vm_address_t basepart)
{
    rbx_rgb_t _color;
    bzero(&_color, sizeof(_color));
    rbx_rgb_t* color;
    if (basepart == 0) {return _color;}
    static mach_msg_type_number_t data_cnt;
    kern_return_t kr;
    kr = vm_read(task, basepart + RBX_PART_RGB_COLOR_OFFSET, sizeof(rbx_rgb_t), (vm_offset_t*)&color, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        _color = *color;
        vm_deallocate(mach_task_self_, (vm_address_t)color, sizeof(rbx_rgb_t));
    }
    return _color;
}



char rbx_basepart_get_cancollide(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_1byte_value(task, p_a + RBX_PART_PROPERTIES_CANCOLLIDE_OFFSET);
}

void rbx_basepart_set_cancollide(task_t task, vm_address_t basepart, char new_value)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_PART_PROPERTIES_CANCOLLIDE_OFFSET, (vm_address_t)&new_value, 1);
}
