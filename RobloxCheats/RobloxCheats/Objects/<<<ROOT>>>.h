


vm_address_t rbx_find_root_table(task_t task)
{
    vm_address_t root_table = 0;
    //These are including the first size byte.
    static const unsigned char root_string[] = {20, '<', '<', '<', 'R', 'O', 'O', 'T', '>', '>', '>'};
    static const unsigned char instance_string[] = {0x10, 0x49, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65};
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    
    struct segment_command_64 __DATA_seg;
    task_mach_segment_info(task, task_base_address, "__DATA", &__DATA_seg);
    
    vm_address_t search_start = task_base_address + __DATA_seg.fileoff;
    vm_address_t search_size = __DATA_seg.vmsize;
    
    vm_address_t* read_data;
    static mach_msg_type_number_t data_cnt = 0;
    kern_return_t kr = vm_read(task, search_start, search_size, (vm_address_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (search_size/8) ; i++)
        {
            unsigned char* read_bytes;
            kr = vm_read(task, read_data[i], sizeof(root_string), (vm_address_t*)&read_bytes, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                if (memcmp(read_bytes, root_string, sizeof(root_string)) == 0)
                {
                    unsigned char* __read_bytes;
                    kr = vm_read(task, read_data[i + 1], sizeof(instance_string), (vm_address_t*)&__read_bytes, &data_cnt);
                    if (kr == KERN_SUCCESS)
                    {
                        if (memcmp(__read_bytes, instance_string, sizeof(instance_string)) == 0)
                        {
                            root_table = search_start + (i * 8);
                            i = (search_size/8);
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)__read_bytes, sizeof(instance_string));
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)read_bytes, sizeof(root_string));
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
            unsigned char string_length = vm_read_1byte_value(task, read_data[i]);
            char* string = NULL;
            kr = vm_read(task, read_data[i] + 1, string_length, (vm_address_t*)&string, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                if (string)
                {
                    if (strcmp(search_string, string) == 0)
                    {
                        root_table_string = read_data[i];
                        i = (RBX_ROOT_TABLE_SIZE/8);
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)string, string_length);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, RBX_ROOT_TABLE_SIZE);
    }
    return root_table_string;
}
