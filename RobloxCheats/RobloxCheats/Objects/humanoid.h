

char* rbx_humanoid_get_displayname(task_t task, vm_address_t humanoid, long* inout_len)
{
    return vm_read_rbx_string(task, humanoid + RBX_HUMANOID_DISPLAYNAME_OFFSET, inout_len);
}

float rbx_humanoid_get_health(task_t task, vm_address_t humanoid)
{
    //return vm_read_rbx_encrypted_float(task, humanoid + RBX_HUMANOID_HEALTH_OFFSET);
    return vm_read_float_value(task, humanoid + RBX_HUMANOID_HEALTH_OFFSET);
}

vm_address_t rbx_humanoid_get_seatpart(task_t task, vm_address_t humanoid)
{
    return vm_read_uint64_value(task, humanoid + RBX_HUMANOID_SEATPART_OFFSET);
}
