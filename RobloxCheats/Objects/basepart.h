


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
    if (basepart == 0) { return 0; }
    vm_address_t* properties;
    vm_address_t _properties;
    kern_return_t kr;
    static mach_msg_type_number_t data_cnt;
    kr = vm_read(task, basepart + RBX_PART_PROPERTIES_OFFSET, 8, (vm_offset_t*)&properties, &data_cnt);
    _KR_;
    _properties = *properties;
    vm_deallocate(mach_task_self_, (vm_address_t)properties, 8);
    return _properties;
}





float rbx_basepart_get_gravity(task_t task, vm_address_t basepart)
{
    if (basepart == 0) { return NAN; }
    float _gravity = NAN;
    float* gravity;
    kern_return_t kr;
    static mach_msg_type_number_t data_cnt;
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    kr = vm_read(task, p_a + RBX_PART_PROPERTIES_GRAVITY_OFFSET, 4, (vm_offset_t*)&gravity, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        _gravity = *gravity;
        vm_deallocate(task, (vm_address_t)gravity, 4);
    }
    return _gravity;
}

void rbx_basepart_set_gravity(task_t task, vm_address_t basepart, float new_value)
{
    if (basepart == 0) {return;}
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_PART_PROPERTIES_GRAVITY_OFFSET, (vm_address_t)&new_value, 4);
}





rbx_cframe_t rbx_basepart_get_cframe(task_t task, vm_address_t basepart)
{
    rbx_cframe_t _cf;
    bzero(&_cf, sizeof(_cf));
    rbx_cframe_t* cf;
    if (basepart == 0) {return _cf;}
    static mach_msg_type_number_t data_cnt;
    kern_return_t kr;
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    kr = vm_read(task, p_a + RBX_PART_PROPERTIES_CFRAME_OFFSET, sizeof(rbx_cframe_t), (vm_offset_t*)&cf, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        _cf = *cf;
        vm_deallocate(mach_task_self_, (vm_address_t)cf, sizeof(rbx_cframe_t));
    }
    return _cf;
}

void rbx_basepart_set_cframe(task_t task, vm_address_t basepart, rbx_cframe_t* cf)
{
    if (basepart == 0) {return;}
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_PART_PROPERTIES_CFRAME_OFFSET, (vm_address_t)&cf, sizeof(rbx_cframe_t));
    return;
}
