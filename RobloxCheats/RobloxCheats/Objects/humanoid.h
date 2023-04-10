
vector3_t rbx_humanoid_get_move_direction(task_t task, vm_address_t humanoid)
{
    return vm_read_vector3_value(task, humanoid + RBX_HUMANOID_MOVE_DIRECTION_OFFSET);
}
