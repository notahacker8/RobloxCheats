
vector3_t rbx_humanoid_get_move_direction(task_t task, vm_address_t humanoid)
{
    return vm_read_vector3_value(task, humanoid + RBX_HUMANOID_MOVE_DIRECTION_OFFSET);
}

char rbx_humanoid_get_state(task_t task, vm_address_t humanoid)
{
    return vm_read_1byte_value(task, humanoid + 0x808);
}

void rbx_humanoid_set_state(task_t task, vm_address_t humanoid, char state)
{
    vm_write(task, humanoid + 0x808, (vm_offset_t)&state, 1);
}


/*
 sit: 0x184
 */
