


typedef struct
{
    char __pad1__[8];
    vm_address_t get_method;
    char __pad2__[8];
    vm_address_t set_method;
}
rbx_get_set_method_t;

typedef struct
{
    vm_address_t some_address; // + 0x0
    char* property_name; // + 0x0 + 8
    char __pad1__[0x20]; // + 0x8 + 8
    rbx_class_info_t* class_info; // + 0x28 + 8
    char __pad2__[0x18]; // + 0x30 + 8
    rbx_get_set_method_t* get_set_1; // + 0x48 + 8
    rbx_get_set_method_t* get_set_2; // + 0x50 + 8
    char __pad3__[0x10]; // + 0x58 + 8
    vm_address_t unique_method_1; // + 0x68 + 8
    vm_address_t unique_method_2; // + 0x70 + 8
}
rbx_function_object_t;




#define RBX_METHOD_TYPE_GETTER 1
#define RBX_METHOD_TYPE_SETTER 2
#define RBX_METHOD_TYPE_NEITHER 3


vm_address_t rbx_find_method_offset(task_t task,
                                    vm_address_t root_table,
                                    char* class_name,
                                    char* property_name,
                                    int methtype)
{
    static mach_msg_type_number_t data_cnt;
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    struct segment_command_64 __DATA_seg;
    task_mach_segment_info(task, task_base_address, "__DATA", &__DATA_seg);
    
    struct segment_command_64 __TEXT_seg;
    task_mach_segment_info(task, task_base_address, "__TEXT", &__TEXT_seg);
    
    
    vm_address_t* read_data;
    
    vm_address_t search_start = task_base_address + __DATA_seg.fileoff;
    vm_size_t search_size = __DATA_seg.vmsize;
    vm_address_t search_end = search_start + search_size;
    
    vm_offset_t func_offset = 0;
    
    vm_address_t property_name_string_addr = rbx_root_table_lookup(task, root_table, property_name);
    
    if (property_name_string_addr)
    {
        if (vm_read(task, search_start, search_size, (vm_address_t*)&read_data, &data_cnt) == KERN_SUCCESS)
        {
            for (long i = 0 ; i < (search_size/8) ; i++)
            {
                if (read_data[i] == property_name_string_addr)
                {
                    vm_address_t current_address = search_start + (i * 8);
                    if (!((current_address < (root_table + RBX_ROOT_TABLE_SIZE)) && (current_address > (root_table - 1))))
                    {
                        vm_address_t class_info_ptr = vm_read_8byte_value(task, current_address + 0x28);
                        if (class_info_ptr > (search_start) && class_info_ptr < (search_end))
                        {
                            vm_address_t class_name_string_addr = rbx_root_table_lookup(task, root_table, class_name);
                            if (class_name_string_addr)
                            {
                                vm_address_t class_name_ptr = vm_read_8byte_value(task, class_info_ptr + 8);
                                if (class_name_ptr == class_name_string_addr)
                                {
                                    //printf("%p\n", current_address);
                                    if (methtype == RBX_METHOD_TYPE_NEITHER)
                                    {
                                        vm_address_t func_address = vm_read_8byte_value(task, current_address + 0x68);
                                        //printf("%p\n", func_address);
                                        if (func_address > (task_base_address + __TEXT_seg.fileoff) &&
                                            func_address < (task_base_address + __TEXT_seg.fileoff + __TEXT_seg.vmsize))
                                        {
                                            func_offset = func_address - task_base_address;
                                        }
                                        else
                                        {
                                            func_address = vm_read_8byte_value(task, current_address + 0x70);
                                            if (func_address > (task_base_address + __TEXT_seg.fileoff) &&
                                                func_address < (task_base_address + __TEXT_seg.fileoff + __TEXT_seg.vmsize))
                                            {
                                                func_offset = func_address - task_base_address;
                                            }
                                        }
                                    }
                                    if (methtype == RBX_METHOD_TYPE_GETTER || methtype == RBX_METHOD_TYPE_SETTER)
                                    {
                                        vm_address_t method_info_ptr = vm_read_8byte_value(task, current_address + 0x48);
                                        if (method_info_ptr < search_end)
                                        {
                                            method_info_ptr = vm_read_8byte_value(task, current_address + 0x50);
                                        }
                                        if (method_info_ptr > search_end)
                                        {
                                            vm_offset_t o = 0x0;
                                            if (methtype == RBX_METHOD_TYPE_GETTER)
                                            {
                                                o = 0x8;
                                            }
                                            if (methtype == RBX_METHOD_TYPE_SETTER)
                                            {
                                                o = 0x18;
                                            }
                                            vm_address_t func_address = vm_read_8byte_value(task, method_info_ptr + o);
                                            if (func_address > (task_base_address + __TEXT_seg.fileoff) &&
                                                func_address < (task_base_address + __TEXT_seg.fileoff + __TEXT_seg.vmsize))
                                            {
                                                func_offset = func_address - task_base_address;
                                            }
                                        }
                                    }
                                    //printf("%p\n", current_address);
                                }
                            }
                            else
                            {
                                fprintf(stderr, "error: could not find string '%s' in <<<ROOT>>> table (%p)\n", class_name, (void*)root_table);
                            }
                        }
                    }
                }
            }
            vm_deallocate(mach_task_self_, (vm_address_t)read_data, search_size);
        }
    }
    else
    {
        fprintf(stderr, "error: could not find string '%s' in <<<ROOT>>> table (%p)\n", property_name, (void*)root_table);
    }
    
    return func_offset;
    
}



