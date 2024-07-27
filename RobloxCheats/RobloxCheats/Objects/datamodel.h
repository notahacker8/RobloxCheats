
#define RBX_JOBID_LENGTH 36

/*
vm_address_t rbx_find_game_address(task_t task)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t game = 0;
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    struct segment_command_64 __DATA_seg = task_get_seg(task, task_base_address, "__DATA");
    
    vm_address_t* read_data;
    
    vm_address_t search_start = task_base_address + __DATA_seg.fileoff;
    vm_size_t search_size = __DATA_seg.vmsize;
    
    
    if (vm_read(task, search_start, search_size, (vm_address_t*)&read_data, &data_cnt) == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (search_size/8) ; i++)
        {
            vm_address_t pg = read_data[i];
            if (pg > 0x7fffff)
            {
                if (rbx_instance_get_self(task, pg) == pg && rbx_instance_get_parent(task, pg) == 0)
                {
                    long cname_len = 0;
                    long name_len = 0;
                    char* cname = rbx_instance_get_class_name(task, pg, &cname_len);
                    char* name = rbx_instance_get_name(task, pg, &name_len);
                    if (cname && name)
                    {
                        if (strcmp("DataModel", cname) == 0)
                        {
                            if (strcmp("Game", name) == 0)
                            {
                                game = pg;
                                i = (search_size/8); //break loop without skipping the deallocation.
                            }
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)cname, cname_len);
                        vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                    }
                }
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, search_size);
    }
     
    return game;
}
 */

vm_address_t rbx_find_game_address_testing(task_t task)
{
    vm_address_t game_address = 0;
    vm_address_t name_address = 0;
    
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
        if (info.user_tag == VM_MEMORY_MALLOC_NANO)
        {
            //printf("[%d]: %p - %p\n", info.user_tag, address, address+size);
            char kb[6] = {0xd6, 0x47, 0x61, 0x6D, 0x65, 0x00};
            vm_offset_t off = scan_bytes_offset(task, address, size, kb, sizeof(kb), 0x1);
            if (off)
            {
                name_address = address + off + 1;
            }
        }
        address+=size;
    }
    address = 0;
    size = 0;
    depth = 0;
    kr = 0;
    while (kr == KERN_SUCCESS)
    {
        memset (&info, 0, sizeof (info));
        kr = vm_region_recurse_64 (task, &address, &size,
                                   &depth, (vm_region_recurse_info_t) &info, &info_count);
        if (info.user_tag == VM_MEMORY_MALLOC_SMALL)
        {
            vm_offset_t off = scan_bytes_offset(task, address, size, (char*)&name_address, 8, 8);
            if (off)
            {
                vm_address_t pga = address + off - RBX_INSTANCE_NAME_OFFSET;
                //printf("%p\n", pga);
                if (rbx_instance_get_self(task, pga) == pga)
                {
                    game_address = pga;
                }
            }
            //printf("%p - %p\n", address, address+size);
        }
        address+=size;
    }
    return game_address;
}


vm_address_t rbx_find_game_address(task_t task)
{
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    vm_address_t game_address = 0;
    vm_address_t s = task_base_address + RBX_WHAT_GAME_POINTS_TO_OFFSET;
    
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
        if (info.user_tag == VM_MEMORY_MALLOC_SMALL)
        {
            //printf("[%d]: %p - %p\n", info.user_tag, address, address+size);
            vm_address_t read_data;
            mach_msg_type_number_t data_cnt;
            if (vm_read(task, address, size, &read_data, &data_cnt) == KERN_SUCCESS)
            {
                for (long i = 0 ; i < (data_cnt/8) ; i++)
                {
                    if (((vm_address_t*)read_data)[i] == s)
                    {
                        vm_offset_t off = i * 8;
                        vm_address_t pga = address + off;
                        //printf("%p\n", pga);
                        long n_l = 0;
                        char* n = rbx_instance_get_name(task, pga, &n_l);
                        if (n)
                        {
                            if (strcmp(n, "Game") == 0)
                            {
                                game_address = pga;
                                //printf("game: %p\n", pga);
                            }
                            vm_deallocate(mach_task_self_, (vm_address_t)n, n_l);
                        }
                    }
                }
                vm_deallocate(mach_task_self_, read_data, data_cnt);
            }

        }
        address+=size;
    }
    return game_address;
}


vm_address_t rbx_find_game_address_yield(task_t task, int sleep_time)
{
    vm_address_t game = 0;
    while (game == 0)
    {
        game = rbx_find_game_address(task);
        sleep(sleep_time);
    }
    return game;
}


char* rbx_datamodel_get_jobid(task_t task, vm_address_t datamodel, long* inout_len)
{
    return vm_read_rbx_string(task, datamodel + RBX_DATAMODEL_JOBID_OFFSET, inout_len);
}


unsigned long rbx_get_placeid(task_t task)
{
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    return vm_read_uint64_value(task, task_base_address + RBX_PLACEID_OFFSET);
}


void rbx_print_game_info(task_t task)
{
    vm_address_t game = rbx_find_game_address(task);
    rbx_print_descendants(task, game, 0, 0);
    printf("\nPlaceId: %lu\n", rbx_get_placeid(task));
    long jobid_len = 0;
    char* jobid = rbx_datamodel_get_jobid(task, game, &jobid_len);
    if (jobid)
    {
        printf("JobId/ServerId: %36s\n\n", jobid);
        vm_deallocate(mach_task_self_, (vm_address_t)jobid, jobid_len);
    }
}
