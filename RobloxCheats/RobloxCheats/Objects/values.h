


long rbx_intvalue_get_value(task_t task, vm_address_t intvalue)
{
    return vm_read_8byte_value(task, intvalue + RBX_INTVALUE_VALUE_OFFSET);
}

void rbx_intvalue_set_value(task_t task, vm_address_t intvalue, long value)
{
    vm_write(task, intvalue + RBX_INTVALUE_VALUE_OFFSET, (vm_offset_t)&value, 8);
}

double rbx_numbervalue_get_value(task_t task, vm_address_t numbervalue)
{
    long_double_u u;
    u.l = vm_read_8byte_value(task, numbervalue + RBX_INTVALUE_VALUE_OFFSET);
    return u.d;
}

void rbx_numbervalue_set_value(task_t task, vm_address_t numbervalue, double value)
{
    vm_write(task, numbervalue + RBX_INTVALUE_VALUE_OFFSET, (vm_offset_t)&value, 8);
}

char rbx_boolvalue_get_value(task_t task, vm_address_t boolvalue)
{
    return vm_read_1byte_value(task, boolvalue + RBX_INTVALUE_VALUE_OFFSET);
}
