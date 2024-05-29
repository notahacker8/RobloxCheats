


long rbx_intvalue_get_value(task_t task, vm_address_t intvalue)
{
    return vm_read_uint64_value(task, intvalue + RBX_INTVALUE_VALUE_OFFSET);
}

void rbx_intvalue_set_value(task_t task, vm_address_t intvalue, long value)
{
    vm_write(task, intvalue + RBX_INTVALUE_VALUE_OFFSET, (vm_offset_t)&value, 8);
}



double rbx_numbervalue_get_value(task_t task, vm_address_t numbervalue)
{
    long a = vm_read_uint64_value(task, numbervalue + RBX_INTVALUE_VALUE_OFFSET);
    return *((double*)(&a));
}

void rbx_numbervalue_set_value(task_t task, vm_address_t numbervalue, double value)
{
    vm_write(task, numbervalue + RBX_INTVALUE_VALUE_OFFSET, (vm_offset_t)&value, 8);
}


vm_address_t rbx_objectvalue_get_value(task_t task, vm_address_t value)
{
    return vm_read_uint64_value(task, value + RBX_INTVALUE_VALUE_OFFSET);
}

void rbx_objectvalue_set_value(task_t task, vm_address_t objectvalue, double value)
{
    vm_write(task, objectvalue + RBX_INTVALUE_VALUE_OFFSET, (vm_offset_t)&value, 8);
}



char rbx_boolvalue_get_value(task_t task, vm_address_t boolvalue)
{
    return vm_read_uint8_value(task, boolvalue + RBX_INTVALUE_VALUE_OFFSET + 8);
}

void rbx_boolvalue_set_value(task_t task, vm_address_t boolvalue, char value)
{
    vm_write(task, boolvalue + RBX_INTVALUE_VALUE_OFFSET + 8, (vm_offset_t)&value, 1);
}

char* rbx_stringvalue_get_value(task_t task, vm_address_t stringvalue, long* inout_length)
{
    return vm_read_rbx_string(task, stringvalue + RBX_INTVALUE_VALUE_OFFSET + 8, inout_length);
}
