





unsigned long vm_read_8byte_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    long __data = 0;
    kern_return_t kr;
    kr = vm_read(task, address, 8, (vm_offset_t*)&read_data, &data_cnt);
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
    kern_return_t kr;
    kr = vm_read(task, address, sizeof(int), (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        __data = *(int*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
    }
    return __data;
}

unsigned char vm_read_1byte_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    char __data = 0;
    kern_return_t kr;
    kr = vm_read(task, address, 1, (vm_offset_t*)&read_data, &data_cnt);
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
        kr = vm_read(task, dyld_info.all_image_info_addr, sizeof(struct dyld_all_image_infos), (vm_address_t*)&infos, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            vm_address_t info_array_addr = (vm_address_t)infos -> infoArray;
            int info_count = infos -> infoArrayCount;
            for (int i = 0 ; i < info_count ; i++)
            {
                vm_address_t dii_ptr = info_array_addr + (i * sizeof(struct dyld_image_info));
                struct dyld_image_info* dii;
                kr = vm_read(task, dii_ptr, sizeof(struct dyld_image_info), (vm_address_t*)&dii, &data_cnt);
                if (kr == KERN_SUCCESS)
                {
                    vm_address_t img_file_path_ptr = (vm_address_t)(dii -> imageFilePath);
                    vm_address_t img_ld_address = (vm_address_t)(dii -> imageLoadAddress);
                    char* img_file_path;
                    kr = vm_read(task, img_file_path_ptr, PATH_MAX, (vm_address_t*)&img_file_path, &data_cnt);
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



void task_mach_segment_info(task_t task,
                            vm_address_t task_base_address,
                            char* segname,
                            struct segment_command_64* inout_seg)
{
    vm_address_t address = task_base_address;
    static mach_msg_type_number_t data_cnt = 0;
    struct mach_header_64* header;
    
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
                    if (strncmp(segcmd -> segname, segname, 16) == 0)
                    {
                        memcpy(inout_seg, segcmd, sizeof(struct segment_command_64));
                        i = header -> ncmds;
                    }
                }
                address += segcmd -> cmdsize;
                vm_deallocate(mach_task_self_, (vm_address_t)segcmd, sizeof(struct segment_command_64));
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)header, sizeof(struct mach_header_64));
    }
}
