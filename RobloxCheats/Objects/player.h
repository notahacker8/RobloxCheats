

vm_address_t rbx_player_get_team(task_t task, vm_address_t player)
{
    if (player == 0) { return 0; }
    vm_address_t* team;
    vm_address_t _team;
    kern_return_t kr;
    static mach_msg_type_number_t data_cnt;
    kr = vm_read(task, player + RBX_PLAYER_TEAM_OFFSET, 8, (vm_offset_t*)&team, &data_cnt);
    _KR_;
    _team = *team;
    vm_deallocate(mach_task_self_, (vm_address_t)team, 8);
    return _team;
}

vm_address_t rbx_player_get_character(task_t task, vm_address_t player)
{
    if (player == 0) { return 0; }
    vm_address_t* team;
    vm_address_t _team;
    kern_return_t kr;
    static mach_msg_type_number_t data_cnt;
    kr = vm_read(task, player + RBX_PLAYER_CHARACTER_OFFSET, 8, (vm_offset_t*)&team, &data_cnt);
    _KR_;
    _team = *team;
    vm_deallocate(mach_task_self_, (vm_address_t)team, 8);
    return _team;
}
