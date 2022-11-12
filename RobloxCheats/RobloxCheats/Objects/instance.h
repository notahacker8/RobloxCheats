


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
    return vm_read_8byte_value(task, instance + 0x58);
}

char* rbx_instance_get_name(task_t task, vm_address_t instance, char* inout_len)
{
    char* name_length;
    char _name_length;
    
    vm_address_t* name_address;
    vm_address_t _name_address;
    
    char* name;
    static mach_msg_type_number_t data_cnt;
    kern_return_t kr;
    kr = vm_read(task, instance + 0x40, 8, (vm_offset_t*)&name_address, &data_cnt);
    _KR_
    _name_address = *name_address;
    kr = vm_read(task, _name_address, 1, (vm_offset_t*)&name_length, &data_cnt);
    vm_deallocate(mach_task_self_, (vm_address_t)name_address, 8);
    _KR_
    _name_length = *name_length;
    vm_deallocate(mach_task_self_, (vm_address_t)name_length, 1);
    kr = vm_read(task, _name_address + 1, _name_length/2, (vm_offset_t*)&name, &data_cnt);
    _KR_
    *inout_len = _name_length/2;

    return name;
}

char* rbx_instance_get_class_name(task_t task, vm_address_t instance, char* inout_len)
{
    char class_name_length;
    vm_address_t class_name_address;
    vm_address_t ci_ptr;
    
    vm_address_t read_data;
    
    char* class_name;
    
    kern_return_t kr;
    static mach_msg_type_number_t data_cnt;
    kr = vm_read(task, instance + 0x18, 8, (vm_offset_t*)&read_data, &data_cnt);
    _KR_
    ci_ptr = *(vm_address_t*)read_data;
    vm_deallocate(mach_task_self_, (vm_address_t)read_data, 8);
    kr = vm_read(task, ci_ptr + 8, 8, (vm_offset_t*)&read_data, &data_cnt);
    _KR_
    class_name_address = *(vm_address_t*)read_data;
    vm_deallocate(mach_task_self_, read_data, 8);
    kr = vm_read(task, class_name_address, 1, (vm_offset_t*)&read_data, &data_cnt);
    _KR_
    class_name_length = *(char*)read_data;
    vm_deallocate(mach_task_self_, read_data, 1);
    kr = vm_read(task, class_name_address + 1, class_name_length/2, (vm_offset_t*)&class_name, &data_cnt);
    _KR_
    *inout_len = class_name_length/2;
    return class_name;
}


rbx_child_t* rbx_instance_get_children(task_t task, vm_address_t instance, long* inout_child_count)
{
    if (instance == 0) { return NULL; }
    static mach_msg_type_number_t data_cnt;
    
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
                    i = child_count; //break;
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
                    i = child_count; //break;
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
    static mach_msg_type_number_t data_cnt;
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
                            i = (size/8); //break;
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


vm_offset_t rbx_get_place_id(task_t task)
{
    vm_address_t read_data;
    mach_msg_type_number_t data_cnt;
    kern_return_t kr = 0;
    vm_address_t _id = 0;
    kr = vm_read(task, get_base_address(task) + RBX_PLACE_ID_OFFSET, 8, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        _id = *(vm_address_t*)read_data;
        vm_deallocate(mach_task_self_, read_data, 8);
    }
    return _id;
}




















/*
void rbx_print_children_profiles(task_t task, vm_address_t instance)
{
    if (instance == 0)
    {
        return;
    }
    long cc;
    vm_address_t* children = rbx_instance_get_children(task, instance, &cc);
    char _cname_len;
    char* _class_name = rbx_instance_get_class_name(task, instance, &_cname_len);
    char _name_len;
    char* _name = rbx_instance_get_name(task, instance, &_name_len);
    printf("[%s] %s (%p) (child count: %lu)\n", _class_name, _name, (void*)instance, cc);
    if (children)
    {
        for (long i = 0 ; i < cc ; i++)
        {
            vm_address_t child = children[i];
            char cname_len;
            char* class_name = rbx_instance_get_class_name(task, child, &cname_len);
            char name_len;
            char* name = rbx_instance_get_name(task, child, &name_len);
            long cc = rbx_instance_get_child_count(task, child);
            printf("\t[%s] %s (%p) (child count: %lu)\n", class_name, name, (void*)child, cc);
        }
        free(children);
    }
}


void rbx_print_descendants(task_t task, vm_address_t instance, int recurse_count)
{
    if (instance == 0)
    {
        return;
    }
    long cc;
    vm_address_t* children = rbx_instance_get_children(task, instance, &cc);
    char _cname_len;
    char* _class_name = rbx_instance_get_class_name(task, instance, &_cname_len);
    char _name_len;
    char* _name = rbx_instance_get_name(task, instance, &_name_len);
    for (int i = 0 ; i < recurse_count ; i++)
    {
        printf("\t");
    }
    printf("[%s] %s (%p) (child count: %lu)\n", _class_name, _name, (void*)instance, cc);
    if (children)
    {
        for (long i = 0 ; i < cc ; i++)
        {
            vm_address_t child = children[i];
            char cname_len;
            char* class_name = rbx_instance_get_class_name(task, child, &cname_len);
            char name_len;
            char* name = rbx_instance_get_name(task, child, &name_len);
            long cc = rbx_instance_get_child_count(task, child);
            if (cc > 0)
            {
                rbx_print_descendants(task, child, recurse_count + 1);
            } else
            {
                for (int x = 0 ; x < recurse_count ; x++)
                {
                    printf("\t");
                }
                printf("\t[%s] %s (%p) (child count: %lu)\n", class_name, name, (void*)child, cc);
            }
        }
        free(children);
    }
}
*/
