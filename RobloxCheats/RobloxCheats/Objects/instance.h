
#pragma mark - structs -

typedef struct
{
    vm_address_t child_address;
    char __pad__[8];
}
rbx_child_t;

typedef struct
{
    vm_address_t* first_child_ptr;     // + 0x0
    vm_address_t* last_child_ptr;      // + 0x8
}
rbx_children_info_t;

typedef struct
{
    char __pad__1__[0x8];     // + 0x0
    char* class_name;         // + 0x8
}
rbx_class_info_t;

typedef struct
{
    char __pad__1__[0x8];                       // + 0x0
    void* instance_self;                        // + 0x8
    char __pad__2__[0x8];                       // + 0x10
    void* class_info_ptr;                       // + 0x18
    char __pad__3__[0x20];                      // + 0x20
    char* name;                                 // + 0x40
    void* children_info_ptr;                    // + 0x48
    char __pad__5__[0x8];                       // + 0x50
    void* parent;                               // + 0x58
}
rbx_instance_t;


#pragma mark - functions -

vm_address_t rbx_instance_get_self(task_t task, vm_address_t instance)
{
    return vm_read_uint64_t_value(task, instance + RBX_INSTANCE_SELF_OFFSET);
}

vm_address_t rbx_instance_get_parent(task_t task, vm_address_t instance)
{
    return vm_read_uint64_t_value(task, instance + RBX_INSTANCE_PARENT_OFFSET);
}


char* rbx_instance_get_name(task_t task, vm_address_t instance, long* inout_len)
{
    vm_address_t name_address = vm_read_uint64_t_value(task, instance + RBX_INSTANCE_NAME_OFFSET);
    return vm_read_rbx_string(task, name_address, inout_len);
}


char* rbx_instance_get_class_name(task_t task, vm_address_t instance, long* inout_len)
{
    char* str = NULL;
    vm_address_t ci_ptr = vm_read_uint64_t_value(task, instance + RBX_INSTANCE_CLASS_INFO_OFFSET);
    if (ci_ptr)
    {
        vm_address_t class_name_addr = vm_read_uint64_t_value(task, ci_ptr + 8);
        str = vm_read_rbx_string(task, class_name_addr, inout_len);
    }
    return str;
}


char rbx_instance_is_a(task_t task, vm_address_t instance, char* classname)
{
    char isa = false;
    long cnl = 0;
    char* cn = NULL;
    cn = rbx_instance_get_class_name(task, instance, &cnl);
    if (cn)
    {
        isa = (strcmp(cn, classname) == 0);
        vm_deallocate(mach_task_self_, (vm_address_t)cn, cnl);
    }
    return isa;
}

char rbx_instance_is_a_partial(task_t task, vm_address_t instance, char* partial_classname)
{
    char isa = false;
    long cnl = 0;
    char* cn = NULL;
    cn = rbx_instance_get_class_name(task, instance, &cnl);
    if (cn)
    {
        isa = !(strstr(cn, partial_classname) == 0);
        vm_deallocate(mach_task_self_, (vm_address_t)cn, cnl);
    }
    return isa;
}

char rbx_instance_is_named(task_t task, vm_address_t instance, char* name)
{
    char is = false;
    long nl = 0;
    char* n = NULL;
    n = rbx_instance_get_name(task, instance, &nl);
    if (n)
    {
        is = (strcmp(n, name) == 0);
        vm_deallocate(mach_task_self_, (vm_address_t)n, nl);
    }
    return is;
}


rbx_child_t* rbx_instance_get_children(task_t task, vm_address_t instance, long* inout_child_count)
{
    static mach_msg_type_number_t data_cnt;
    
    rbx_child_t* children = NULL;
    long child_count = 0;
    
    vm_address_t children_info_ptr = vm_read_uint64_t_value(task, instance + RBX_INSTANCE_CHILDREN_OFFSET);
    if (children_info_ptr)
    {
        vm_address_t child_list_start = vm_read_uint64_t_value(task, children_info_ptr);
        vm_address_t child_list_end = vm_read_uint64_t_value(task, children_info_ptr + 8);
        //printf("%p, %p\n", child_list_start, child_list_end);
        vm_size_t size = (child_list_end - child_list_start);
        kern_return_t kr = vm_read(task, child_list_start, size, (vm_address_t*)&children, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            child_count = size/sizeof(rbx_child_t);
        }
    }
    
    *inout_child_count = child_count;
    return children;
}

#define FOR_EACH_CHILD(instance, code)\
{\
long child_count = 0;\
rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);\
if (children)\
{\
    for (long i = 0 ; i < child_count ; i++)\
    {\
        vm_address_t child = children[i].child_address;\
code\
    }\
    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));\
}\
}

vm_address_t rbx_instance_find_first_child_of_class(task_t task,
                                                    vm_address_t instance,
                                                    char* class_name)
{
    vm_address_t found_child = 0;
    FOR_EACH_CHILD(instance, {
        long cname_len;
        char* cname = rbx_instance_get_class_name(task, child, &cname_len);
        if (cname != NULL)
        {
            //printf("%s\n", cname);
            if (strcmp(cname, class_name) == 0)
            {
                found_child = child;
                i = child_count;   //break the loop, without skipping the deallocating part;
            }
            vm_deallocate(mach_task_self_, (vm_address_t)cname, cname_len);
        }
    })
    return found_child;
}


vm_address_t rbx_instance_find_last_child_of_class(task_t task,
                                                    vm_address_t instance,
                                                    char* class_name)
{
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = (child_count - 1) ; i > -1 ; i--)
        {
            vm_address_t child = children[i].child_address;
            //printf("%p\n", child);
            long cname_len;
            char* cname = rbx_instance_get_class_name(task, child, &cname_len);
            if (cname != NULL)
            {
                //printf("%s\n", cname);
                if (strcmp(cname, class_name) == 0)
                {
                    found_child = child;
                    i = -1;   //break the loop, without skipping the deallocating part;
                }
                vm_deallocate(mach_task_self_, (vm_address_t)cname, cname_len);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
    }
    return found_child;
}

vm_address_t rbx_instance_find_last_child_of_class_and_name(task_t task,
                                                            vm_address_t instance,
                                                            char* class_name,
                                                            char* name)
{
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = (child_count - 1) ; i > -1 ; i--)
        {
            vm_address_t child = children[i].child_address;
            //printf("%p\n", child);
            long cname_len;
            char* cname = rbx_instance_get_class_name(task, child, &cname_len);
            if (cname != NULL)
            {
                //printf("%s\n", cname);
                if (strcmp(cname, class_name) == 0)
                {
                    long _name_len;
                    char* _name = rbx_instance_get_name(task, child, &_name_len);
                    if (strcmp(_name, name) == 0)
                    {
                        found_child = child;
                        i = -1;   //break the loop, without skipping the deallocating part;
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)_name, _name_len);
                }
                vm_deallocate(mach_task_self_, (vm_address_t)cname, cname_len);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
    }
    return found_child;
}


vm_address_t rbx_instance_find_first_child(task_t task,
                                           vm_address_t instance,
                                           char* name)
{
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = 0 ; i < child_count ; i++)
        {
            vm_address_t child = children[i].child_address;
            long _name_len;
            char* _name = rbx_instance_get_name(task, child, &_name_len);
            if (_name != NULL)
            {
                if (strcmp(_name, name) == 0)
                {
                    found_child = child;
                    i = child_count;  //break the loop, without skipping the deallocating part.
                }
                vm_deallocate(mach_task_self_, (vm_address_t)_name, _name_len);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
    }
    return found_child;
}


vm_address_t rbx_instance_find_child_at_index(task_t task,
                                              vm_address_t instance,
                                              char* name,
                                              int index)
{
    int __c = 0;
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = 0 ; i < child_count ; i++)
        {
            vm_address_t child = children[i].child_address;
            long _name_len;
            char* _name = rbx_instance_get_name(task, child, &_name_len);
            if (_name != NULL)
            {
                if (strcmp(_name, name) == 0)
                {
                    if (__c == index)
                    {
                        found_child = child;
                        i = child_count;  //break the loop, without skipping the deallocating part.
                    }
                    __c++;
                }
                vm_deallocate(mach_task_self_, (vm_address_t)_name, _name_len);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
    }
    return found_child;
}

vm_address_t rbx_instance_find_child_of_class_at_index(task_t task,
                                                       vm_address_t instance,
                                                       char* class_name,
                                                       int index)
{
    int __c = 0;
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = 0 ; i < child_count ; i++)
        {
            vm_address_t child = children[i].child_address;
            long _cname_len;
            char* _cname = rbx_instance_get_class_name(task, child, &_cname_len);
            if (_cname != NULL)
            {
                if (strcmp(_cname, class_name) == 0)
                {
                    if (__c == index)
                    {
                        found_child = child;
                        i = child_count;  //break the loop, without skipping the deallocating part.
                    }
                    __c++;
                }
                vm_deallocate(mach_task_self_, (vm_address_t)_cname, _cname_len);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
    }
    return found_child;
}



vm_address_t rbx_instance_find_last_child(task_t task,
                                           vm_address_t instance,
                                           char* name)
{
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = (child_count - 1) ; i > -1 ; i--)
        {
            vm_address_t child = children[i].child_address;
            long _name_len;
            char* _name = rbx_instance_get_name(task, child, &_name_len);
            if (_name != NULL)
            {
                if (strcmp(_name, name) == 0)
                {
                    found_child = child;
                    i = -1;  //break the loop, without skipping the deallocating part.
                }
                vm_deallocate(mach_task_self_, (vm_address_t)_name, _name_len);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
    }
    return found_child;
}

vm_address_t rbx_instance_find_first_child_partial(task_t task,
                                           vm_address_t instance,
                                           char* partial_name)
{
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = 0 ; i < child_count ; i++)
        {
            vm_address_t child = children[i].child_address;
            long _name_len;
            char* _name = rbx_instance_get_name(task, child, &_name_len);
            if (_name != NULL)
            {
                if (strstr(_name, partial_name))
                {
                    found_child = child;
                    i = child_count;  //break the loop, without skipping the deallocating part.
                }
                vm_deallocate(mach_task_self_, (vm_address_t)_name, _name_len);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
    }
    return found_child;
}


/*
 Only works for non-DataModel instances.
 */
char rbx_instance_is_valid_child(task_t task, vm_address_t instance)
{
    return (instance && rbx_instance_get_parent(task, instance) && (rbx_instance_get_self(task, instance) == instance));
}


















void rbx_print_descendants(task_t task, vm_address_t instance, int current_recurse_count, int max_recurse_count)
{
    if (instance == 0 || current_recurse_count > max_recurse_count)
    {
        return;
    }
    long cc;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &cc);
    long _cname_len;
    char* _class_name = rbx_instance_get_class_name(task, instance, &_cname_len);
    long _name_len;
    char* _name = rbx_instance_get_name(task, instance, &_name_len);
    for (int i = 0 ; i < current_recurse_count ; i++)
    {
        printf("\t");
    }
    if (children == 0)
    {
        cc = 0;
    }
    printf("[%s] %s (%p) (child count: %lu)\n", _class_name, _name, (void*)instance, cc);
    if (children)
    {
        for (long i = 0 ; i < cc ; i++)
        {
            vm_address_t child = children[i].child_address;
            rbx_print_descendants(task, child, current_recurse_count + 1, max_recurse_count);
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, cc * sizeof(rbx_child_t));
    }
    if (_name)
    {
    vm_deallocate(mach_task_self_, (vm_address_t)_name, _name_len);
    }
    if (_class_name)
    {
    vm_deallocate(mach_task_self_, (vm_address_t)_class_name, _cname_len);
    }
}



void rbx_get_descendants(task_t task,
                         vm_address_t instance,
                         int current_recurse_count,
                         int max_recurse_count,
                         vm_address_t* array,
                         long* current_d_count,
                         long max_d_count)
{
    if (instance == 0 || current_recurse_count > max_recurse_count || ((max_d_count - 1) < *current_d_count))
    {
        return;
    }
    FOR_EACH_CHILD(instance, {
        if ((max_d_count) > *current_d_count)
        {
            array[*current_d_count] = child;
            (*current_d_count)++;
            rbx_get_descendants(task, child, current_recurse_count + 1, max_recurse_count, array, current_d_count, max_d_count);
        }
        else
        {
            i = child_count;
        }
    })
}


#define FOR_EACH_DESCENDANT(instance, max_desc_count, child_depth, code)\
{\
vm_address_t* descendants = malloc(max_desc_count * 8);\
long descendant_count = 0;\
rbx_get_descendants(task, instance, 0, child_depth, descendants, &descendant_count, max_desc_count);\
for (long i = 0 ; i < descendant_count ; i++)\
{\
    vm_address_t obj = descendants[i];\
code\
}\
free(descendants);\
}





void rbx_print_ancestors(task_t task, vm_address_t instance, int count)
{
    vm_address_t p = instance;
    for (int i = 0 ; i < count + 1; i++)
    {
        rbx_print_descendants(task, p, 0, 0);
        p = rbx_instance_get_parent(task, p);
    }
}


vm_address_t rbx_instance_find_first_descendant(task_t task, vm_address_t instance, char* name, char* class_name, int depth, long max_d_count)
{
    vm_address_t desc = 0;
    FOR_EACH_DESCENDANT(instance, max_d_count, depth, {
        if (rbx_instance_is_a(task, obj, class_name))
        {
            if (rbx_instance_is_named(task, obj, name))
            {
                desc = obj;
                i = descendant_count;
            }
        }
    })
    return desc;
}
