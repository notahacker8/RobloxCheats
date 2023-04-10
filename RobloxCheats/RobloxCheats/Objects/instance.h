


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


vm_address_t rbx_instance_get_self(task_t task, vm_address_t instance)
{
    return vm_read_8byte_value(task, instance + RBX_INSTANCE_SELF_OFFSET);
}

vm_address_t rbx_instance_get_parent(task_t task, vm_address_t instance)
{
    return vm_read_8byte_value(task, instance + RBX_INSTANCE_PARENT_OFFSET);
}

char* rbx_instance_get_name(task_t task, vm_address_t instance, unsigned char* inout_len)
{
    static mach_msg_type_number_t data_cnt;
    unsigned char name_length = 0;
    char* name = NULL;
    
    vm_address_t name_address = vm_read_8byte_value(task, instance + RBX_INSTANCE_NAME_OFFSET);
    if (name_address)
    {
        name_length = vm_read_1byte_value(task, name_address)/2;
        vm_read(task, name_address + 1, name_length, (vm_address_t*)&name, &data_cnt);
    }
    *inout_len = name_length;

    return name;
}

char* rbx_instance_get_class_name(task_t task, vm_address_t instance, unsigned char* inout_len)
{
    static mach_msg_type_number_t data_cnt;
    unsigned char class_name_length = 0;
    
    char* class_name = NULL;
    
    vm_address_t ci_ptr = vm_read_8byte_value(task, instance + RBX_INSTANCE_CLASS_INFO_OFFSET);
    if (ci_ptr)
    {
        vm_address_t class_name_addr = vm_read_8byte_value(task, ci_ptr + 8);
        if (class_name_addr)
        {
            class_name_length = vm_read_1byte_value(task, class_name_addr)/2;
            vm_read(task, class_name_addr + 1, class_name_length, (vm_address_t*)&class_name, &data_cnt);
        }
    }
    
    *inout_len = class_name_length/2;
    
    return class_name;
}

char rbx_instance_is_a(task_t task, vm_address_t instance, char* classname)
{
    char isa = false;
    unsigned char cnl = 0;
    char* cn = NULL;
    cn = rbx_instance_get_class_name(task, instance, &cnl);
    if (cn)
    {
        if (strcmp(cn, classname) == 0)
        {
            isa = true;
        }
        vm_deallocate(mach_task_self_, (vm_address_t)cn, cnl);
    }
    return isa;
}

rbx_child_t* rbx_instance_get_children(task_t task, vm_address_t instance, long* inout_child_count)
{
    static mach_msg_type_number_t data_cnt;
    
    rbx_child_t* children = NULL;
    long child_count = 0;
    
    vm_address_t children_info_ptr = vm_read_8byte_value(task, instance + RBX_INSTANCE_CHILDREN_OFFSET);
    if (children_info_ptr)
    {
        vm_address_t child_list_start = vm_read_8byte_value(task, children_info_ptr);
        vm_address_t child_list_end = vm_read_8byte_value(task, children_info_ptr + 8);
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

vm_address_t rbx_instance_find_first_child_of_class(task_t task,
                                                    vm_address_t instance,
                                                    char* class_name)
{
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = 0 ; i < child_count ; i++)
        {
            vm_address_t child = children[i].child_address;
            unsigned char cname_len;
            char* cname = rbx_instance_get_class_name(task, child, &cname_len);
            if (cname != NULL)
            {
                if (strcmp(cname, class_name) == 0)
                {
                    found_child = child;
                    i = child_count;   //break the loop, without skipping the deallocating part;
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
            unsigned char _name_len;
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


vm_address_t rbx_find_game_address(task_t task)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t game = 0;
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    struct segment_command_64 __DATA_seg;
    task_mach_segment_info(task, task_base_address, "__DATA", &__DATA_seg);
    
    vm_address_t* read_data;
    
    vm_address_t search_start = task_base_address + __DATA_seg.fileoff;
    vm_size_t search_size = __DATA_seg.vmsize;
    
    if (vm_read(task, search_start, search_size, (vm_address_t*)&read_data, &data_cnt) == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (search_size/8) ; i++)
        {
            vm_address_t pg = read_data[i];
            if (pg % 0x100 == 0x18)
            {
                if (rbx_instance_get_self(task, pg) == pg)
                {
                    unsigned char cname_len;
                    unsigned char name_len;
                    char* cname = rbx_instance_get_class_name(task, pg, &cname_len);
                    char* name = rbx_instance_get_name(task, pg, &name_len);
                    if (cname && name)
                    {
                        if (strcmp("DataModel", cname) == 0)
                        {
                            if (strcmp("Game", name) == 0)
                            {
                                game = pg;
                                i = (search_size/8); //break loop without skipping the deallocation.
                            }
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)cname, cname_len);
                        vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                    }
                }
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, search_size);
    }
    return game;
}

























void rbx_print_descendants(task_t task, vm_address_t instance, int current_recurse_count, int max_recurse_count)
{
    if (instance == 0 || current_recurse_count > max_recurse_count)
    {
        return;
    }
    long cc;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &cc);
    unsigned char _cname_len;
    char* _class_name = rbx_instance_get_class_name(task, instance, &_cname_len);
    unsigned char _name_len;
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
    vm_deallocate(mach_task_self_, (vm_address_t)_name, _name_len);
    vm_deallocate(mach_task_self_, (vm_address_t)_class_name, _cname_len);
}


void rbx_instance_for_each_descendant(task_t task, vm_address_t instance, void func(task_t, vm_address_t))
{
    func(task, instance);
    long cc;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &cc);
    if (children)
    {
        for (long i = 0 ; i < cc ; i++)
        {
            vm_address_t child = children[i].child_address;
            rbx_instance_for_each_descendant(task, child, func);
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, cc * sizeof(rbx_child_t));
    }
}
