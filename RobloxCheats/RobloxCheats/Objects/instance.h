


typedef struct
{
    vm_address_t child_address;
    vm_address_t __pad__1__;
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
    char __pad__3__[0x18];                      // + 0x20
    bool archivable;                            // + 0x38
    char __pad__4__[0x7];                       // + 0x39
    char* name;                                 // + 0x40
    void* children_info_ptr;                    // + 0x48
    char __pad__5__[0x8];                       // + 0x50
    void* parent;                               // + 0x58
}
rbx_instance_t;


vm_address_t rbx_instance_get_parent(task_t task, vm_address_t instance)
{
    if (instance == 0) { return 0; }
    return vm_read_8byte_value(task, instance + 0x58);
}

char* rbx_instance_get_name(task_t task, vm_address_t instance, char* inout_len)
{
    if (instance == 0) { return NULL; }
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    kern_return_t kr;
    
    char name_length;
    vm_address_t name_address;
    char* name = NULL;
    
    kr = vm_read(task, instance + 0x40, 8, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        name_address = *(vm_address_t*)read_data;
        vm_deallocate(mach_task_self_, read_data, 8);
        kr = vm_read(task, name_address, 1, (vm_offset_t*)&read_data, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            name_length = *(char*)read_data;
            name_length /= 2;
            *inout_len = name_length;
            vm_deallocate(mach_task_self_, read_data, 1);
            kr = vm_read(task, name_address + 1, name_length, (vm_offset_t*)&name, &data_cnt);
        }
    }
    

    return name;
}

char* rbx_instance_get_class_name(task_t task, vm_address_t instance, char* inout_len)
{
    if (instance == 0) { return NULL; }
    mach_msg_type_number_t data_cnt;
    kern_return_t kr;
    vm_address_t read_data;
    
    char class_name_length;
    vm_address_t class_name_address;
    vm_address_t ci_ptr;
    
    char* class_name = NULL;
    
    kr = vm_read(task, instance + 0x18, 8, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        ci_ptr = *(vm_address_t*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 8);
        kr = vm_read(task, ci_ptr + 8, 8, (vm_offset_t*)&read_data, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            class_name_address = *(vm_address_t*)read_data;
            vm_deallocate(mach_task_self_, read_data, 8);
            kr = vm_read(task, class_name_address, 1, (vm_offset_t*)&read_data, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                class_name_length = *(char*)read_data;
                class_name_length /= 2;
                *inout_len = class_name_length;
                vm_deallocate(mach_task_self_, read_data, 1);
                kr = vm_read(task, class_name_address + 1, class_name_length, (vm_offset_t*)&class_name, &data_cnt);
            }
        }
    }
    
    return class_name;
}


rbx_child_t* rbx_instance_get_children(task_t task, vm_address_t instance, long* inout_child_count)
{
    if (instance == 0) { return NULL; }
    mach_msg_type_number_t data_cnt;
    
    kern_return_t kr;
    
    rbx_child_t* children = NULL;
    vm_address_t read_data;

    kr = vm_read(task, instance + 0x48, 8, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        vm_address_t children_info_ptr = *(vm_address_t*)read_data;
        vm_deallocate(mach_task_self_, read_data, 8);
        if (children_info_ptr != 0)
        {
            kr = vm_read(task, children_info_ptr, 16, (vm_offset_t*)&read_data, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                vm_address_t children_info_start = ((vm_address_t*)read_data)[0];
                vm_address_t children_info_end = ((vm_address_t*)read_data)[1];
                vm_deallocate(mach_task_self_, read_data, 16);
                
                vm_size_t size = children_info_end - children_info_start;
                kr = vm_read(task, children_info_start, size, (vm_offset_t*)&read_data, &data_cnt);
                if (kr == KERN_SUCCESS)
                {
                    *inout_child_count = size/16;
                    children = (rbx_child_t*)read_data;
                }
            }
        }
    }
    
    return children;
}

vm_address_t rbx_instance_find_first_child_of_class(task_t task,
                                                    vm_address_t instance,
                                                    char* class_name)
{
    if (instance == 0) { return 0; }
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = 0 ; i < child_count ; i++)
        {
            vm_address_t child = children[i].child_address;
            char cname_len;
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
    if (instance == 0) { return 0; }
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = 0 ; i < child_count ; i++)
        {
            vm_address_t child = children[i].child_address;
            char _name_len;
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

vm_address_t rbx_instance_find_first_child_of_class_and_name(task_t task,
                                                             vm_address_t instance,
                                                             char* class_name,
                                                             char* name)
{
    if (instance == 0) { return 0; }
    long child_count = 0;
    vm_address_t found_child = 0;
    rbx_child_t* children = rbx_instance_get_children(task, instance, &child_count);
    if (children)
    {
        for (long i = 0 ; i < child_count ; i++)
        {
            vm_address_t child = children[i].child_address;
            char _name_len;
            char* _name = rbx_instance_get_name(task, child, &_name_len);
            if (_name != NULL)
            {
                if (strcmp(_name, name) == 0)
                {
                    char _cname_len;
                    char* _cname = rbx_instance_get_class_name(task, child, &_cname_len);
                    if (_cname)
                    {
                        if (strcmp(_cname, class_name) == 0)
                        {
                            found_child = child;
                            i = child_count;
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)_cname, _cname_len);
                    }
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
    vm_address_t base = get_base_address(task);
    vm_address_t** regions = task_get_regions(task, 3);
    vm_address_t data_start = regions[0][1];
    vm_address_t data_end = regions[1][2];
    vm_address_t* possible_games;
    free(regions);
    vm_size_t size = (data_end - data_start);
    vm_read(task, data_start, size, (vm_address_t*)&possible_games, &data_cnt);
    for (long i = 0 ; i < (size/8) ; i++)
    {
        vm_address_t pg = possible_games[i];
        if (pg > base * 3)
        {
            kern_return_t kr;
            char* check;
            kr = vm_read(task, pg, 1, (vm_address_t*)&check, &data_cnt);
            vm_deallocate(mach_task_self_, (vm_address_t)check, 1);
            if (kr == KERN_SUCCESS)
            {
                char cname_len;
                char name_len;
                char* cname = rbx_instance_get_class_name(task, pg, &cname_len);
                char* name = rbx_instance_get_name(task, pg, &name_len);
                if (cname && name)
                {
                    if (strcmp("DataModel", cname) == 0)
                    {
                        if (strcmp("Game", name) == 0)
                        {
                            game = pg;
                            i = (size/8); //break loop without skipping the deallocation.
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)cname, cname_len);
                vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
            }
        }
    }
    vm_deallocate(mach_task_self_, (vm_address_t)possible_games, size);
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
    char _cname_len;
    char* _class_name = rbx_instance_get_class_name(task, instance, &_cname_len);
    char _name_len;
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

