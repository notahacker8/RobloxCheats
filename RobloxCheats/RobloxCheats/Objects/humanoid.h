
vector3_t rbx_humanoid_get_move_direction(task_t task, vm_address_t humanoid)
{
    static mach_msg_type_number_t data_cnt;
    vector3_t dir;
    bzero(&dir, sizeof(dir));
    
    kern_return_t kr;
    vm_address_t read_data;
    
    if (humanoid == 0) {return dir;}
    kr = vm_read(task, humanoid + 0x2c8, sizeof(vector3_t), (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        dir = *(vector3_t*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, sizeof(vector3_t));
    }
    return dir;
}

char rbx_humanoid_get_state(task_t task, vm_address_t humanoid)
{
    return vm_read_1byte_value(task, humanoid + 0x808);
}
