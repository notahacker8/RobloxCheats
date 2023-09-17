
#define RBX_JOBID_LENGTH 36


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
            if (rbx_instance_get_self(task, pg) == pg)
            {
                long cname_len;
                long name_len;
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
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, search_size);
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
    return vm_read_8byte_value(task, task_base_address + RBX_PLACEID_OFFSET);
}


void rbx_print_game_info(void)
{
    task_t task = get_roblox_task_port();
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
