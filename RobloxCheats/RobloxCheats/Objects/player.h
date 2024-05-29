

vm_address_t rbx_player_get_team(task_t task, vm_address_t player)
{
    return vm_read_uint64_value(task, player + RBX_PLAYER_TEAM_OFFSET);
}

vm_address_t rbx_player_get_character(task_t task, vm_address_t player)
{
    return vm_read_uint64_value(task, player + RBX_PLAYER_CHARACTER_OFFSET);
}

void rbx_player_set_character(task_t task, vm_address_t player, vm_address_t new_character)
{
    vm_write(task, player + RBX_PLAYER_CHARACTER_OFFSET, (vm_offset_t)&new_character, sizeof(vm_address_t));
}

char* rbx_player_get_displayname(task_t task, vm_address_t player, long* inout_len)
{
    return vm_read_rbx_string(task, player + RBX_PLAYER_DISPLAYNAME_OFFSET, inout_len);
}

double rbx_player_get_last_input_timestamp(task_t task, vm_address_t player)
{
    return vm_read_double_value(task, player + RBX_PLAYER_LAST_INPUT_TIMESTAMP_OFFSET);
}

void rbx_player_set_last_input_timestamp(task_t task, vm_address_t player, double newvalue)
{
    vm_write(task, player + RBX_PLAYER_LAST_INPUT_TIMESTAMP_OFFSET, (vm_address_t)&newvalue, 8);
}

