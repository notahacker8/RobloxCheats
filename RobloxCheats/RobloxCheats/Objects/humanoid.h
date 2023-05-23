


vector3_t rbx_humanoid_get_move_direction(task_t task, vm_address_t humanoid)
{
    return vm_read_vector3_value(task, humanoid + RBX_HUMANOID_MOVEDIRECTION_OFFSET);
}

float rbx_humanoid_get_health(task_t task, vm_address_t humanoid)
{
    return vm_read_rbx_encrypted_float(task, humanoid + RBX_HUMANOID_ENCRYPTED_HEALTH_OFFSET);
}

float rbx_humanoid_get_maxhealth(task_t task, vm_address_t humanoid)
{
    return vm_read_rbx_encrypted_float(task, humanoid + RBX_HUMANOID_ENCRYPTED_MAXHEALTH_OFFSET);
}

char* rbx_humanoid_get_displayname(task_t task, vm_address_t humanoid, long* inout_len)
{
    return vm_read_rbx_string(task, humanoid + RBX_HUMANOID_DISPLAYNAME_OFFSET, inout_len);
}

