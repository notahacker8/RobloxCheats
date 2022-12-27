


char* rbx_stringvalue_get_short_value(task_t task, vm_address_t stringvalue, char* inout_len)
{
    static mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    char length;
    char* value;
    kern_return_t kr;
    kr = vm_read(task, stringvalue + RBX_STRING_VALUE_SHORT_VALUE_OFFSET, 1, (vm_offset_t*)&read_data, &data_cnt);
    _KR_
    length = *(char*)read_data;
    vm_deallocate(mach_task_self_, read_data, 1);
    
    kr = vm_read(task, stringvalue + RBX_STRING_VALUE_SHORT_VALUE_OFFSET + 1, length/2, (vm_offset_t*)&value, &data_cnt);
    _KR_
    *inout_len = length/2;

    return value;
}

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
    static mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    kern_return_t kr;
    char value;
    kr = vm_read(task, boolvalue + RBX_INT_VALUE_VALUE_OFFSET, 1, (vm_offset_t*)&read_data, &data_cnt);
    _KR_
    value = *(char*)read_data;
    vm_deallocate(mach_task_self_, (vm_address_t)read_data, 1);
    return value;
}
