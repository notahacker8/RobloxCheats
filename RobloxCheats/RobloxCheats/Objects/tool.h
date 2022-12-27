



rbx_cframe_t rbx_tool_get_grip(task_t task, vm_address_t tool)
{
    static mach_msg_type_number_t data_cnt;
    rbx_cframe_t cframe;
    bzero(&cframe, sizeof(cframe));
    
    kern_return_t kr;
    vm_address_t read_data;
    
    if (tool == 0) {return cframe;}
    kr = vm_read(task, tool + 0x1b4, sizeof(rbx_cframe_t), (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        cframe = *(rbx_cframe_t*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, sizeof(rbx_cframe_t));
    }
    return cframe;
}

void rbx_tool_set_grip(task_t task, vm_address_t tool, rbx_cframe_t cf)
{
    if (tool == 0) {return;}
    vm_write(task, tool + 0x1b4, (vm_address_t)&cf, sizeof(rbx_cframe_t));
    return;
}
