

vm_address_t rbx_player_get_team(task_t task, vm_address_t player)
{
    return vm_read_8byte_value(task, player + RBX_PLAYER_TEAM_OFFSET);
}

vm_address_t rbx_player_get_character(task_t task, vm_address_t player)
{
    return vm_read_8byte_value(task, player + RBX_PLAYER_CHARACTER_OFFSET);
}

void rbx_player_set_character(task_t task, vm_address_t player, vm_address_t new_character)
{
    vm_write(task, player + RBX_PLAYER_CHARACTER_OFFSET, (vm_offset_t)&new_character, sizeof(vm_address_t));
}

char* rbx_player_get_display_name(task_t task, vm_address_t player, char* inout_len)
{
    if (player == 0) { return NULL; }
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    kern_return_t kr;
    
    unsigned char name_length;
    vm_address_t name_address = player + RBX_PLAYER_DISPLAY_NAME_OFFSET;
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

void rbx_player_set_last_input_timestamp(task_t task, vm_address_t player, double new_value)
{
    vm_write(task, player + RBX_PLAYER_LAST_INPUT_TIMESTAMP_OFFSET, (vm_offset_t)&new_value, sizeof(double));
}
