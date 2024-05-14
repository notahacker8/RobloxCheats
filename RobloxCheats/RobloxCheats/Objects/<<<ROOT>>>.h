


vm_address_t rbx_find_root_table(task_t task)
{
    vm_address_t root_table = 0;
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    struct segment_command_64 __DATA_seg = task_get_seg(task, task_base_address, "__DATA");
    
    vm_address_t search_start = task_base_address + __DATA_seg.fileoff;
    vm_size_t search_size = __DATA_seg.vmsize;
    
    vm_address_t* read_data;
    static mach_msg_type_number_t data_cnt = 0;
    kern_return_t kr = vm_read(task, search_start, search_size, (vm_address_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (search_size/8) - 8 ; i++)
        {
            long str1_len = 0;
            char* str1 = vm_read_rbx_string(task, read_data[i], &str1_len);
            if (str1)
            {
                if (strcmp(str1, "<<<ROOT>>>") == 0)
                {
                    root_table = task_base_address + __DATA_seg.fileoff + (i * 8);
                    long str2_len = 0;
                    char* str2 = vm_read_rbx_string(task, read_data[i + 1], &str2_len);
                    if (str2)
                    {
                        if (strcmp(str2, "Instance") == 0)
                        {
                            root_table = task_base_address + __DATA_seg.fileoff + (i * 8);
                            i = (search_size/8) - 8;
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)str2, str2_len);
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)str1, str1_len);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, search_size);
    }
    return root_table;
}

#define RBX_ROOT_TABLE_SIZE 0x10000  //rough estimate....????

vm_address_t rbx_root_table_lookup(task_t task, vm_address_t root_table, char* search_string)
{
    vm_address_t* read_data;
    static mach_msg_type_number_t data_cnt = 0;
    vm_address_t root_table_string = 0;
    kern_return_t kr = vm_read(task, root_table, RBX_ROOT_TABLE_SIZE, (vm_address_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (RBX_ROOT_TABLE_SIZE/8) ; i++)
        {
            long string_length = 0;
            char* string = vm_read_rbx_string(task, read_data[i], &string_length);
            if (string)
            {
                if (strcmp(search_string, string) == 0)
                {
                    root_table_string = read_data[i];
                    //printf("\"%s\" @ %p\n", string, read_data[i]);
                    i = (RBX_ROOT_TABLE_SIZE/8);
                }
                vm_deallocate(mach_task_self_, (vm_address_t)string, string_length);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, RBX_ROOT_TABLE_SIZE);
    }
    return root_table_string;
}
