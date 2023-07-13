


kern_return_t vm_read_byte_array(task_t task,
                                 vm_address_t address,
                                 vm_size_t size,
                                 void* buf)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    kern_return_t kr = vm_read(task, address, size, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        memcpy(buf, (char*)read_data, size);
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, size);
    }
    return kr;
}


unsigned long vm_read_8byte_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    long __data = 0;
    kern_return_t kr = vm_read(task, address, 8, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        __data = *(long*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 8);
    }
    return __data;
}

unsigned int vm_read_4byte_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    int __data = 0;
    kern_return_t kr = vm_read(task, address, sizeof(int), (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        __data = *(int*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
    }
    return __data;
}

float vm_read_rbx_encrypted_float(task_t task, vm_address_t address)
{
    long a = vm_read_8byte_value(task, address);
    int b = vm_read_4byte_value(task, a);
    return ((int_float_u){.i = (int)(b + a)}).f;
}

unsigned char vm_read_1byte_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    char __data = 0;
    kern_return_t kr = vm_read(task, address, 1, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        __data = *(char*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 1);
    }
    return __data;
}



char is_valid_pointer(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    char valid = false;
    kern_return_t kr = vm_read(task, address, 1, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        valid = true;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 1);
    }
    return valid;
}



typedef struct
{
    unsigned char length_x2;
    unsigned char bytes[];
}
rbx_short_string_t;

typedef struct
{
    unsigned char former_length_x2;
    unsigned char __pad1__[7];
    long new_length;
    char* bytes;
}
rbx_long_string_t;

#ifdef ROBLOX_ROSETTA_VERSION

VM_DEALLOCABLE char* vm_read_rbx_string(task_t task, vm_address_t address, long* inout_len)
{
    mach_msg_type_number_t data_cnt;
    char* text = NULL;
    unsigned char short_length_x2 = vm_read_1byte_value(task, address);
    if (short_length_x2 == vm_read_8byte_value(task, address))
    {
        long long_length = vm_read_8byte_value(task, address + 8);
        vm_address_t str_ptr = vm_read_8byte_value(task, address + 16);
        if (is_valid_pointer(task, str_ptr))
        {
            *inout_len = long_length;
            vm_read(task, str_ptr, long_length, (vm_offset_t*)&text, &data_cnt);
        }
    }
    else
    {
        long len = short_length_x2/2;
        *inout_len = len;
        vm_read(task, address + 1, len, (vm_offset_t*)&text, &data_cnt);
    }
    return text;
}

#endif

#ifdef ROBLOX_APPLE_VERSION

VM_DEALLOCABLE char* vm_read_rbx_string(task_t task, vm_address_t address, long* inout_length)
{
    mach_msg_type_number_t data_cnt = 0;
    vm_address_t str_ptr = vm_read_8byte_value(task, address);
    vm_address_t next_ptr = vm_read_8byte_value(task, address + 8);
    char* text = NULL;
    long length = 0;
    if (is_valid_pointer(task, str_ptr) && !is_valid_pointer(task, next_ptr))
    {
        length = vm_read_8byte_value(task, address + 8);
        address = str_ptr;
    }
    else
    {
        unsigned char byte = vm_read_1byte_value(task, address);
        while (byte != 0)
        {
            length++;
            byte = vm_read_1byte_value(task, address + length);
        }
    }
    *inout_length = length;
    vm_read(task, address, length, (vm_offset_t*)&text, &data_cnt);
    return text;
}

#endif

vm_address_t task_get_image_address_by_path(const task_t task,
                                            const char* image_path)
{
    vm_address_t image_address = 0;
    
    struct task_dyld_info dyld_info;
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    kern_return_t kr = task_info(task, TASK_DYLD_INFO, (task_info_t)&dyld_info, &count);
    static mach_msg_type_number_t data_cnt = 0;
    if (kr == KERN_SUCCESS)
    {
        struct dyld_all_image_infos* infos;
        kern_return_t kr = vm_read(task, dyld_info.all_image_info_addr, sizeof(struct dyld_all_image_infos), (vm_address_t*)&infos, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            vm_address_t info_array_addr = (vm_address_t)infos -> infoArray;
            int info_count = infos -> infoArrayCount;
            for (int i = 0 ; i < info_count ; i++)
            {
                vm_address_t dii_ptr = info_array_addr + (i * sizeof(struct dyld_image_info));
                struct dyld_image_info* dii;
                kern_return_t kr = vm_read(task, dii_ptr, sizeof(struct dyld_image_info), (vm_address_t*)&dii, &data_cnt);
                if (kr == KERN_SUCCESS)
                {
                    vm_address_t img_file_path_ptr = (vm_address_t)(dii -> imageFilePath);
                    vm_address_t img_ld_address = (vm_address_t)(dii -> imageLoadAddress);
                    char* img_file_path;
                    kern_return_t kr = vm_read(task, img_file_path_ptr, PATH_MAX, (vm_address_t*)&img_file_path, &data_cnt);
                    if (kr == KERN_SUCCESS)
                    {
                        if (strcmp(image_path, img_file_path) == 0)
                        {
                            image_address = img_ld_address;
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)img_file_path, PATH_MAX);
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)dii, sizeof(struct dyld_image_info));
                }
            }
            vm_deallocate(mach_task_self_, (vm_address_t)infos, sizeof(struct dyld_all_image_infos));
        }
    }
    
    return image_address;
}




struct section_64 task_get_sect(task_t task,
                                vm_address_t task_base_address,
                                char* segname,
                                char* sectname)
{
    mach_msg_type_number_t data_cnt = 0;
    vm_address_t address = task_base_address;
    struct mach_header_64* header;
    struct section_64 ret_struct;
    bzero(&ret_struct, sizeof(ret_struct));
    kern_return_t kr = vm_read(task, address, sizeof(struct mach_header_64), (vm_address_t*)&header, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        address += sizeof(struct mach_header_64);
        for (int i = 0 ; i < header -> ncmds ; i++)
        {
            struct segment_command_64* segcmd;
            kr = vm_read(task, address, sizeof(struct segment_command_64), (vm_address_t*)&segcmd, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                if (segcmd -> cmd == LC_SEGMENT_64)
                {
                    //printf("%p\t%s\n", segcmd->fileoff + task_base_address, segcmd->segname);
                    vm_address_t a = address + sizeof(struct segment_command_64);
                    for (int _i = 0 ; _i < segcmd -> nsects ; _i++)
                    {
                        struct section_64* sect;
                        kr = vm_read(task, a, sizeof(struct section_64), (vm_address_t*)&sect, &data_cnt);
                        //printf("%p\t%s, %s\n", sect->offset + task_base_address, sect->segname, sect->sectname);
                        if (strcmp(segname, sect->segname) == 0 && strcmp(sectname, sect->sectname) == 0)
                        {
                            ret_struct = *sect;
                        }
                        a += sizeof(struct section_64);
                    }
                }
                address += segcmd -> cmdsize;
                vm_deallocate(mach_task_self_, (vm_address_t)segcmd, sizeof(struct segment_command_64));
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)header, sizeof(struct mach_header_64));
    }
    return ret_struct;
}

struct segment_command_64 task_get_seg(task_t task,
                                       vm_address_t task_base_address,
                                       char* segname)
{
    mach_msg_type_number_t data_cnt = 0;
    vm_address_t address = task_base_address;
    struct mach_header_64* header;
    struct segment_command_64 ret_struct;
    bzero(&ret_struct, sizeof(ret_struct));
    kern_return_t kr = vm_read(task, address, sizeof(struct mach_header_64), (vm_address_t*)&header, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        address += sizeof(struct mach_header_64);
        for (int i = 0 ; i < header -> ncmds ; i++)
        {
            struct segment_command_64* segcmd;
            kr = vm_read(task, address, sizeof(struct segment_command_64), (vm_address_t*)&segcmd, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                if (segcmd -> cmd == LC_SEGMENT_64)
                {
                    if (strcmp(segname, segcmd->segname) == 0)
                    {
                        ret_struct = *segcmd;
                    }
                }
                address += segcmd -> cmdsize;
                vm_deallocate(mach_task_self_, (vm_address_t)segcmd, sizeof(struct segment_command_64));
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)header, sizeof(struct mach_header_64));
    }
    return ret_struct;
}


vm_address_t find_rosetta_arm_func_from_x86(task_t task,
                                            vm_address_t x86_func_addr,
                                            vm_address_t* ribm_ptr,
                                            vm_address_t* r_ptr)
{
    static mach_msg_type_number_t data_cnt;
    
    vm_address_t arm_func = 0;
    
    kern_return_t kr = KERN_SUCCESS;
    struct vm_region_submap_info_64 info;
    vm_address_t address = 0;
    vm_size_t size = 0;
    natural_t depth = 0;
    mach_msg_type_number_t info_count = VM_REGION_SUBMAP_INFO_COUNT_64;
    while (kr == KERN_SUCCESS)
    {
        memset (&info, 0, sizeof (info));
        kr = vm_region_recurse_64 (task, &address, &size,
                                   &depth, (vm_region_recurse_info_t) &info, &info_count);
        if (info.user_tag == VM_MEMORY_ROSETTA_INDIRECT_BRANCH_MAP)
        {
            vm_address_t* read_data;
            int err = vm_read(task, address, size, (vm_address_t*)&read_data, &data_cnt);
            if (!err)
            {
                for (long i = 0 ; i < ((size/8) - 1) ; i++)
                {
                    vm_address_t a = read_data[i];
                    if (a == x86_func_addr)
                    {
                        if (ribm_ptr)
                        {
                            *ribm_ptr = address + ((i + 1) * 8);
                        }
                        arm_func = read_data[i + 1];
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)read_data, size);
            }
        }
        if (info.user_tag == VM_MEMORY_ROSETTA)
        {
            vm_address_t* read_data;
            int err = vm_read(task, address, size, (vm_address_t*)&read_data, &data_cnt);
            if (!err)
            {
                for (long i = 0 ; i < ((size/8) - 2) ; i++)
                {
                    vm_address_t a = read_data[i];
                    if (a == x86_func_addr)
                    {
                        if (r_ptr)
                        {
                            *r_ptr = address + ((i + 2) * 8);
                        }
                        arm_func = read_data[i + 2];
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)read_data, size);
            }
        }
        address += size;
    }
    return arm_func;
}



vm_address_t find_bytes_in_rosetta(task_t task,
                                   char* bytes,
                                   long bytes_length)
{
    static mach_msg_type_number_t data_cnt;
    vm_address_t ret_addr = 0;
    kern_return_t kr = KERN_SUCCESS;
    struct vm_region_submap_info_64 info;
    vm_address_t address = 0;
    vm_size_t size = 0;
    natural_t depth = 0;
    mach_msg_type_number_t info_count = VM_REGION_SUBMAP_INFO_COUNT_64;
    while (kr == KERN_SUCCESS)
    {
        memset (&info, 0, sizeof (info));
        kr = vm_region_recurse_64 (task, &address, &size,
                                   &depth, (vm_region_recurse_info_t) &info, &info_count);
        if (info.user_tag == VM_MEMORY_ROSETTA_EXECUTABLE_HEAP)
        {
            vm_address_t read_data;
            int err = vm_read(task, address, size, (vm_address_t*)&read_data, &data_cnt);
            if (!err)
            {
                for (long i = 0 ; i < (size - bytes_length) ; i++)
                {
                    if (memcmp((char*)(read_data + i), bytes, bytes_length) == 0)
                    {
                        ret_addr = address + i;
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)read_data, size);
            }
        }
        address += size;
    }
    return ret_addr;
}

vm_address_t vm_allocate_rwx(task_t task, unsigned char* data, unsigned int size)
{
    vm_address_t a = 0;
    vm_allocate(task, &a, size, VM_FLAGS_ANYWHERE);
    vm_protect(task, a, size, FALSE, VM_PROT_ALL);
    if (data != NULL)
    {
        vm_write(task, a, (vm_offset_t)data, size);
    }
    return a;
}
