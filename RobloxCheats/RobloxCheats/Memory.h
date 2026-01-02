


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
        memcpy(buf, (unsigned char*)read_data, size);
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, size);
    }
    return kr;
}


VM_READ_FUNCTION_CREATE(vm_address_t);
VM_READ_FUNCTION_CREATE(uint64_t);
VM_READ_FUNCTION_CREATE(uint32_t);
VM_READ_FUNCTION_CREATE(uint16_t);
VM_READ_FUNCTION_CREATE(uint8_t);
VM_READ_FUNCTION_CREATE(bool);
VM_READ_FUNCTION_CREATE(double);
VM_READ_FUNCTION_CREATE(float);
VM_READ_FUNCTION_CREATE(vector3_t);
VM_READ_FUNCTION_CREATE(rbx_cframe_t);


#define VM_TASK_BASE_ADDRESS_NO_ASLR 0x100000000

/*
 May need constant updating
 */
float vm_read_rbx_encrypted_float(task_t task, vm_address_t address)
{
    long x8 = vm_read_uint64_t_value(task, address);
    int w9 = vm_read_uint32_t_value(task, x8);
    int w8 = (int)x8 ^ (int)w9;
    return *((float*)(&w8));
}

/*
 base() + 0xA16588    ldr x8, [x0, #0x340]    08 A0 41 F9
 base() + 0xA1658C    ldr w9, [x8]    09 01 40 B9
 base() + 0xA16590    eor w8, w9, w8    28 01 08 4A
 base() + 0xA16594    fmov s0, w8    00 01 27 1E
 */

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
    unsigned char short_length_x2 = vm_read_uint8_t_value(task, address);
    if (short_length_x2 == vm_read_uint64_t_value(task, address))
    {
        long long_length = vm_read_uint64_t_value(task, address + 8);
        vm_address_t str_ptr = vm_read_uint64_t_value(task, address + 16);
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
    char* text = NULL;
    mach_msg_type_number_t data_cnt = 0;
    long length = 0;
    vm_address_t str_ptr = 0;
    
    unsigned char short_length = vm_read_uint8_t_value(task, address + 0x17);
    //printf("%d\n", short_length);
    if (short_length < 23)
    {
        str_ptr = address;
        length = short_length;
    }
    else
    {
        str_ptr = vm_read_uint64_t_value(task, address);
        //printf("%p\n", str_ptr);
        if (is_valid_pointer(task, str_ptr) == true)
        {
            length = vm_read_uint64_t_value(task, address + 8);
        }
        else
        {
            length = 0;
        }
    }
    *inout_length = length;
    vm_read(task, str_ptr, length, (vm_offset_t*)&text, &data_cnt);
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
                        //printf("%s\n", img_file_path);
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

vm_address_t task_get_image_address_by_index(const task_t task,
                                            int i)
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
            //for (int i = 0 ; i < info_count ; i++)
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
                        //printf("%s\n", img_file_path);
                        //if (strcmp(image_path, img_file_path) == 0)
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
                        vm_deallocate(mach_task_self_, (vm_address_t)sect, sizeof(struct section_64));
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


vm_offset_t scan_bytes_offset(task_t task,
                              vm_address_t address,
                              vm_size_t scan_size,
                              char* known_bytes,
                              vm_size_t known_bytes_length,
                              int alignment)
{
    vm_offset_t offset = 0;
    char* read_data = NULL;
    mach_msg_type_number_t data_cnt;
    kern_return_t kr = vm_read(task, address, scan_size, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (scan_size - known_bytes_length) ; i++)
        {
            if (i % alignment == 0)
            {
                if (memcmp(read_data + i, known_bytes, known_bytes_length) == 0)
                {
                    offset = i;
                    i = (scan_size - known_bytes_length);
                }
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, scan_size);
    }
    return offset;
}





uint32_t encode_b(uint64_t from, uint64_t to) {
    int64_t offset = (int64_t)to - (int64_t)(from);
    return 0x14000000 | ((offset >> 2) & 0x03FFFFFF);
}

uint32_t encode_adrp_x17(uint64_t pc, uint64_t target) {
    int64_t page_offset = ((target & ~0xFFFULL) - (pc & ~0xFFFULL)) >> 12;
    uint32_t immlo = page_offset & 0x3;
    uint32_t immhi = (page_offset >> 2) & 0x7FFFF;
    uint32_t rd = 17; // x17
    return 0x90000000 | (immhi << 5) | (rd) | (immlo << 29);
}

uint32_t encode_add_x17(uint16_t imm12) {
    uint32_t rd = 17, rn = 17;
    return 0x91000000 | ((imm12 & 0xFFF) << 10) | (rn << 5) | rd;
}




void compare_buffers(const char *a, const char *b, size_t len) {
    size_t i = 0;

    while (i < len) {
        if (a[i] != b[i]) {
            // Start of a difference block
            size_t start = i;
            size_t diff_len = 0;

            // Advance until the bytes match again or we reach the end
            while (i < len && a[i] != b[i]) {
                i++;
                diff_len++;
            }

            // Print block information
            printf("+ %p, length %zu: | ", start, diff_len);
            // Print bytes from buffer B
            for (size_t j = start; j < start + diff_len; j++)
                printf("%02X ", (unsigned char)b[j]);
            printf("\n");
        } else {
            i++; // bytes match, move on
        }
    }
}

