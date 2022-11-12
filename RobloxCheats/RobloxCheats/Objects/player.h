

vm_address_t rbx_player_get_team(task_t task, vm_address_t player)
{
    return vm_read_8byte_value(task, player + RBX_PLAYER_TEAM_OFFSET);
}

vm_address_t rbx_player_get_character(task_t task, vm_address_t player)
{
    return vm_read_8byte_value(task, player + RBX_PLAYER_CHARACTER_OFFSET);
}
