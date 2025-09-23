

#define RBX_LOCAL_VARIABLE_SIZE 0x20




int rbx_local_variable_get_type(task_t task, vm_address_t var)
{
    return vm_read_uint32_t_value(task, var + 12);
}

void rbx_local_variable_get_string_value(task_t task, vm_address_t address, char* buf, int* inout_len)
{
    int str_len = vm_read_uint32_t_value(task, address + 20);
    if (str_len < 100)
    {
        vm_read_byte_array(task, address + 24, str_len, buf);
        *inout_len = str_len;
    }
}

void rbx_local_variable_get_name(task_t task, vm_address_t var, char* buf, int* inout_len)
{
    vm_address_t a = vm_read_uint64_t_value(task,  var + 0x10);
    rbx_local_variable_get_string_value(task, a, buf, inout_len);
}

