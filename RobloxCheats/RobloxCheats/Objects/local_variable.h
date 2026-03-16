

#define RBX_LOCAL_VARIABLE_SIZE 0x20


/*
 types:
 3: Number
 6: Dictionary/Array
 1: Bool
 8: CFrame/Instance (I think any user-defined 8 byte value)
 */


typedef struct {
    vm_address_t value;
    int pad_0;
    int type;
}
rbx_tuple_member_t
;


typedef struct {
    rbx_tuple_member_t data;
    vm_address_t var_name;
    uint64_t pad;
}
rbx_local_variable_t;


typedef struct {
    vm_address_t wtf;
    int pad;
    int count;
    ///uses rbx_tuple_member_t
    vm_address_t array;
}
rbx_tuple_array_t;
;

typedef struct {
    vm_address_t wtf;
    int pad;
    int count;
    ///uses rbx_local_variable_t
    vm_address_t dictionary;
}
rbx_dictionary_t;
;


VM_READ_FUNCTION_CREATE(rbx_tuple_member_t);
VM_READ_FUNCTION_CREATE(rbx_local_variable_t);


int rbx_local_variable_get_type(task_t task, vm_address_t var)
{
    return vm_read_uint32_t_value(task, var + 12);
}

void rbx_local_variable_get_string_value(task_t task, vm_address_t address, char* buf, int* inout_len)
{
    int str_len = vm_read_uint32_t_value(task, address + 20);
    if (str_len < 100) //Arbitrary size just in case...
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

/** The value is address is [var] */
rbx_cframe_t rbx_local_variable_get_cframe_value(task_t task, vm_address_t address)
{
    return vm_read_rbx_cframe_t_value(task, address + 16);
}

/** The value is address is [var] */
vm_address_t rbx_local_variable_get_instance_value(task_t task, vm_address_t address)
{
    return vm_read_vm_address_t_value(task, address + 16);
}
/*
 vm_address_t rbx_local_variable_get_dictionary_value(task_t task, vm_address_t address)
 {
 vm_address_t
 }
 */


VM_DEALLOCABLE void* rbx_local_variable_read_tuple(task_t task, vm_address_t address, long* inout_count, char* is_dict)
{
    mach_msg_type_number_t dataCnt;
    void* ret;
    int count = vm_read_uint32_t_value(task, address + 12);
    long array_address = vm_read_vm_address_t_value(task, address + 0x10);
    if (array_address)
    {
        *is_dict = 0;
        vm_read(task, array_address, count * sizeof(rbx_tuple_member_t), (vm_address_t*)&ret, &dataCnt);
    }
    else
    {
        array_address = vm_read_vm_address_t_value(task, address + 0x18);
        vm_read(task, array_address, count * sizeof(rbx_local_variable_t), (vm_address_t*)&ret, &dataCnt);
        *is_dict = 1;
    }
    *inout_count = count;
    return ret;
}

vm_address_t rbx_local_variable_get_array_or_dictionary_address(task_t task, vm_address_t address, long* inout_count, char* is_dict)
{
    int count = 0;
    long array_address = vm_read_vm_address_t_value(task, address + 0x10);
    if (array_address)
    {
        count = vm_read_uint32_t_value(task, address + 8);
        *is_dict = false;
    }
    else
    {
        *is_dict = true;
        count = vm_read_uint32_t_value(task, address + 12);
        array_address = vm_read_vm_address_t_value(task, address + 0x18);
    }
    *inout_count = count;
    return array_address;
}

char rbx_is_valid_local_variable(task_t task, vm_address_t var_ptr)
{
    char __is = false;
    int type = rbx_local_variable_get_type(task, var_ptr);
    if (is_valid_pointer(task, var_ptr) && is_valid_pointer(task, var_ptr + 0x10) && type >= 0 && type < 13)
    {
        __is = true;
    }
    return __is;
}


void rbx_print_local_variable(task_t task, vm_address_t var, int tab_index, char* set_name)
{
    if (tab_index > 2)
    {
        return;
    }
    for (int i = 0; i < tab_index ; i++)
    {
        printf("\t");
    }
    int type = rbx_local_variable_get_type(task, var);
    char name[100]; bzero(name, 100); int name_l = 0;
    if (!set_name)
    {
        rbx_local_variable_get_name(task, var, name, &name_l);
    }
    else
    {
        strcpy(name, set_name);
    }
    char format_str[100] = "%p %s (type %d) = %s\n";
    char data_str[1000];
    bzero(data_str, 1000);
    char print_str[1000];
    bzero(print_str, 1000);
    
    char is_dict = 0;
    long count = 0;
    vm_address_t array_addr = 0;
    
    if (type == 3)
    {
        sprintf(data_str, "%f", vm_read_double_value(task, var));
    }
    else if (type == 1)
    {
        sprintf(data_str, "%d", vm_read_uint8_t_value(task, var));
    }
    else if (type == 5)
    {
        char val[100]; bzero(val, 100); int val_l = 0;
        rbx_local_variable_get_string_value(task, vm_read_vm_address_t_value(task, var), val, &val_l);
        sprintf(data_str, "\"%s\"", val);
    }
    else if (type == 6)
    {
        array_addr = rbx_local_variable_get_array_or_dictionary_address(task, vm_read_vm_address_t_value(task, var), &count, &is_dict);
        if (array_addr)
        {
            if (count < 1000)
            {
                if (is_dict)
                {
                    sprintf(data_str, "%s", "[Dictionary]");
                }
                else
                {
                    sprintf(data_str, "%s", "[Array]");
                }
            }
        }
    }
    else
    {
        sprintf(data_str, "%s", "[Unknown]");
    }
    
    sprintf(print_str, format_str, var, name, type, data_str);
    printf("%s", print_str);
    
    if (array_addr)
    {
        if (count < 1000)
        {
            if (is_dict)
            {
                tab_index++;
                int c = 0;
                int i = 0;
                while (c < count)
                {
                    vm_address_t var_ptr = array_addr + i * sizeof(rbx_local_variable_t);
                    if (rbx_is_valid_local_variable(task, var_ptr))
                    {
                        rbx_print_local_variable(task, array_addr + i * sizeof(rbx_local_variable_t), tab_index, NULL);
                        c++;
                    }
                    else
                    {
                    }
                    i++;
                }
            }
            else
            {
                tab_index++;
                //printf("Array: \n");
                for (int i = 0 ; i < count ; i++)
                {
                    //printf("printing subvar: %p\n", array_addr + i * sizeof(rbx_tuple_member_t));
                    char nm[100]; bzero(nm, 100); sprintf(nm, "[%d]", i);
                    rbx_print_local_variable(task, array_addr + i * sizeof(rbx_tuple_member_t), tab_index, nm);
                }
            }
        }
    }
    
}
