


RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(intvalue, value, uint64_t, RBX_INTVALUE_VALUE_OFFSET)
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(intvalue, value, uint64_t, RBX_INTVALUE_VALUE_OFFSET)

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(numbervalue, value, double, RBX_INTVALUE_VALUE_OFFSET)
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(numbervalue, value, double, RBX_INTVALUE_VALUE_OFFSET)

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(objectvalue, value, vm_address_t, RBX_INTVALUE_VALUE_OFFSET)
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(objectvalue, value, vm_address_t, RBX_INTVALUE_VALUE_OFFSET)

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(boolvalue, value, bool, RBX_INTVALUE_VALUE_OFFSET)
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(boolvalue, value, bool, RBX_INTVALUE_VALUE_OFFSET)

RBX_CREATE_BASIC_STRING_PROPERTY_GET_FUNCTION(stringvalue, value, RBX_STRINGVALUE_VALUE_OFFSET)



char rbx_stringvalue_contains_value(task_t task, vm_address_t sv, char* substr)
{
    char is = false;
    long str_l = 0;
    char* str = rbx_stringvalue_get_value(task, sv, &str_l);
    if (str)
    {
        if (strstr(str, substr))
        {
            is = true;
        }
        vm_deallocate(mach_task_self(), (vm_address_t)str, str_l);
    }
    return is;
}


char rbx_stringvalue_equals_value(task_t task, vm_address_t sv, char* eqstr)
{
    char is = false;
    long str_l = 0;
    char* str = rbx_stringvalue_get_value(task, sv, &str_l);
    if (str)
    {
        if (strcmp(str, eqstr) == 0)
        {
            is = true;
        }
        vm_deallocate(mach_task_self(), (vm_address_t)str, str_l);
    }
    return is;
}
