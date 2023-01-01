


long rbx_intvalue_get_value(task_t task, vm_address_t intvalue)
{
    return vm_read_8byte_value(task, intvalue + RBX_INT_VALUE_VALUE_OFFSET);
}

double rbx_numbervalue_get_value(task_t task, vm_address_t numbervalue)
{
    long_double_u u;
    u.l = vm_read_8byte_value(task, numbervalue + RBX_INT_VALUE_VALUE_OFFSET);
    return u.d;
}

char rbx_boolvalue_get_value(task_t task, vm_address_t boolvalue)
{
    return vm_read_1byte_value(task, boolvalue + RBX_INT_VALUE_VALUE_OFFSET);
}
