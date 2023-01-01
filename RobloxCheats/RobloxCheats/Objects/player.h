

vm_address_t rbx_player_get_team(task_t task, vm_address_t player)
{
    return vm_read_8byte_value(task, player + RBX_PLAYER_TEAM_OFFSET);
}

vm_address_t rbx_player_get_character(task_t task, vm_address_t player)
{
    return vm_read_8byte_value(task, player + RBX_PLAYER_CHARACTER_OFFSET);
}

char* rbx_player_get_display_name(task_t task, vm_address_t player, char* inout_len)
{
    if (player == 0) { return NULL; }
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    kern_return_t kr;
    
    char name_length;
    vm_address_t name_address = player + 0x1b0;
    char* name = NULL;
    
    kr = vm_read(task, name_address, 1, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        name_length = *(char*)read_data;
        name_length /= 2;
        *inout_len = name_length;
        vm_deallocate(mach_task_self_, read_data, 1);
        kr = vm_read(task, name_address + 1, name_length, (vm_offset_t*)&name, &data_cnt);
    }
    

    return name;
}
