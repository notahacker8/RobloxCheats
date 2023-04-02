

#define RBX_MESHPART_ASSET_ID_STRING_LENGTH 280


char* rbx_meshpart_get_meshid(task_t task, vm_address_t meshpart)
{
    static mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    kern_return_t kr = 0;
    vm_address_t str_ptr = 0;
    char* meshid = NULL;
    vm_read(task, meshpart + 0x1f0, 8, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        str_ptr = *(vm_address_t*)read_data;
        vm_read(task, str_ptr, RBX_MESHPART_ASSET_ID_STRING_LENGTH, (vm_address_t*)&meshid, &data_cnt);
        vm_deallocate(mach_task_self_, read_data, 8);
    }
    return meshid;
}

char* rbx_meshpart_get_textureid(task_t task, vm_address_t meshpart)
{
    static mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    kern_return_t kr = 0;
    vm_address_t str_ptr = 0;
    char* meshid = NULL;
    vm_read(task, meshpart + 0x1f0 + 0x18, 8, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        str_ptr = *(vm_address_t*)read_data;
        vm_read(task, str_ptr, RBX_MESHPART_ASSET_ID_STRING_LENGTH, (vm_address_t*)&meshid, &data_cnt);
        vm_deallocate(mach_task_self_, read_data, 8);
    }
    return meshid;
}
