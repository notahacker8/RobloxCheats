

char* rbx_humanoid_get_displayname(task_t task, vm_address_t humanoid, long* inout_len)
{
    return vm_read_rbx_string(task, humanoid + RBX_HUMANOID_DISPLAYNAME_OFFSET, inout_len);
}

