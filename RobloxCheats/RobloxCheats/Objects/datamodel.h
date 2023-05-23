
#define RBX_JOBID_LENGTH 36


vm_address_t rbx_find_game_address(task_t task)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t game = 0;
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    struct segment_command_64 __DATA_seg;
    task_mach_segment_info(task, task_base_address, "__DATA", &__DATA_seg);
    
    vm_address_t* read_data;
    
    vm_address_t search_start = task_base_address + __DATA_seg.fileoff;
    vm_size_t search_size = __DATA_seg.vmsize;
    
    if (vm_read(task, search_start, search_size, (vm_address_t*)&read_data, &data_cnt) == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (search_size/8) ; i++)
        {
            vm_address_t pg = read_data[i];
            if (pg % 0x100 == 0x18) //The game address is always 0x********18 for some reason.
            {
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
        }
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, search_size);
    }
    return game;
}



char* rbx_datamodel_get_jobid(task_t task, vm_address_t datamodel)
{
    static mach_msg_type_number_t data_cnt;
    char* jobid = NULL;
    vm_address_t jobid_ptr = vm_read_8byte_value(task, datamodel + RBX_DATAMODEL_JOBID_OFFSET);
    if (jobid_ptr)
    {
        vm_read(task, jobid_ptr, RBX_JOBID_LENGTH, (vm_offset_t*)&jobid, &data_cnt);
    }
    return jobid;
}


unsigned long rbx_get_placeid(task_t task)
{
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    return vm_read_8byte_value(task, task_base_address + RBX_PLACEID_OFFSET);
}


void rbx_print_game_info(task_t task)
{
    vm_address_t game = rbx_find_game_address(task);
    printf("PlaceId: %lu\n", rbx_get_placeid(task));
    printf("JobId/ServerId: %36s\n", rbx_datamodel_get_jobid(task, game));
}
//4ffc37d0-a8b2-4e59-acc3-19cf588650d0
