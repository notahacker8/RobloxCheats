


typedef struct
{
    char __pad__1__[0x110];
    float gravity; // + 0x110
    char __pad__2__[0x4]; // + 0x114
    rbx_cframe_t cframe0; // + 0x118
    
}
rbx_base_part_properties_t;

typedef struct
{
    char __pad__1__[0x158];
    void* properties;
}
rbx_base_part_t;


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
    static mach_msg_type_number_t data_cnt;
    rbx_cframe_t cframe;
    bzero(&cframe, sizeof(cframe));
    
    kern_return_t kr;
    vm_address_t read_data;
    
    if (basepart == 0) {return cframe;}
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    kr = vm_read(task, p_a + RBX_PART_PROPERTIES_CFRAME_OFFSET, sizeof(rbx_cframe_t), (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        cframe = *(rbx_cframe_t*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, sizeof(rbx_cframe_t));
    }
    return cframe;
}

void rbx_basepart_set_cframe(task_t task, vm_address_t basepart, rbx_cframe_t* cf)
{
    if (basepart == 0) {return;}
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_PART_PROPERTIES_CFRAME_OFFSET, (vm_address_t)cf, sizeof(rbx_cframe_t));
    return;
}


rbx_rgb_t rbx_basepart_get_color(task_t task, vm_address_t basepart)
{
    rbx_rgb_t _color;
    bzero(&_color, sizeof(_color));
    rbx_rgb_t* color;
    if (basepart == 0) {return _color;}
    static mach_msg_type_number_t data_cnt;
    kern_return_t kr;
    kr = vm_read(task, basepart + RBX_PART_PROPERTIES_OFFSET + 0x30, sizeof(rbx_rgb_t), (vm_offset_t*)&color, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        _color = *color;
        vm_deallocate(mach_task_self_, (vm_address_t)color, sizeof(rbx_rgb_t));
    }
    return _color;
}
