

void field_of_battle_general_killer(task_t task)
{
    LIBESP_DYLIB_SETUP
    
    shared_memory->FUNCTION_USLEEP_TIME = 1000;
    shared_memory->FUNCTION_COUNT = 10;
    
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t stats = rbx_instance_find_first_child(task, game, "Stats");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    //rbx_print_descendants(task, backpack, 0, 1);
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t characters_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Characters");
    
    static vm_address_t my_character = 0;
    static vm_address_t my_humanoid = 0;
    static vm_address_t backpack = 0;
    
    static char fly_enabled = false;
    static float fly_speed = 100;
    
    static vm_address_t general_torso = 0;
    static vm_address_t general_torso_cframe_address = 0;
    static int handle_count = 0;
    static vm_address_t handle_cframe_addresses[200];
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    
    static char set_parent_hijack_enabled = false;
    static char tool_activate_loop_enabled = false;
    
    static vector3_t new_hrp_pos;
    
    vm_address_t orc_char_folder = rbx_instance_find_first_child(task, characters_folder, "Orc");
    rbx_print_descendants(task, orc_char_folder, 0, 1);
    vm_address_t human_char_folder = rbx_instance_find_first_child(task, characters_folder, "Human");
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    vm_address_t addr = task_base_address + RBX_SET_PARENT_HOOK_OFFSET;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (set_parent_hijack_enabled)
            {
                int orig = vm_read_uint32_t_value(task, addr);
                task_suspend(task);
                vm_protect(task, addr, 4, FALSE, VM_PROT_READ|VM_PROT_WRITE|VM_PROT_COPY);
                vm_write(task, addr, (vm_address_t)__BRK_0__, 4);
                vm_protect(task, addr, 4, FALSE, VM_PROT_READ|VM_PROT_EXECUTE);
                task_resume(task);
                
                ESP_BKPT bkpt1;
                bkpt1.trip_address = addr;
                bkpt1.hit = false;
                shared_memory->BREAKPOINTS[0] = bkpt1;
                
                while (shared_memory->BREAKPOINTS[0].hit == false)
                {
                    usleep(1);
                }
                bkpt1.MCTX = shared_memory->BREAKPOINTS[0].MCTX;
                //rbx_print_descendants(task, c, 0, 0);
                //rbx_print_descendants(task, p, 0, 0);
                long child_count = 0;
                rbx_child_t* children = rbx_instance_get_children(task, backpack, &child_count);
                if (children)
                {
                    for (int i = 0 ; i < child_count ; i++)
                    {
                        vm_address_t child = children[i].child_address;
                        if (rbx_instance_is_a(task, child, "Tool"))
                        {
                            vm_address_t wtype = rbx_instance_find_first_child(task, child, "WeaponType");
                            //rbx_print_descendants(task, wtype, 0, 0);
                            long str_l = 0;
                            char* str = rbx_stringvalue_get_value(task, wtype, &str_l);
                            if (str)
                            {
                                if (strcmp(str, "Hammer") == 0 ||
                                    strcmp(str, "Axe") == 0 ||
                                    strcmp(str, "Sword") == 0)
                                {
                                    shared_memory->BREAKPOINTS[0].MCTX.__ss.__x[0] = child;
                                    shared_memory->BREAKPOINTS[0].MCTX.__ss.__x[1] = my_character;
                                }
                                else
                                {
                                    if (rbx_instance_is_named(task, child, "Heavens Edge") ||
                                        rbx_instance_is_named(task, child, "Dark Assassin Blade") ||
                                        rbx_instance_is_named(task, child, "Pure Energy"))
                                    {
                                        shared_memory->BREAKPOINTS[0].MCTX.__ss.__x[0] = child;
                                        shared_memory->BREAKPOINTS[0].MCTX.__ss.__x[1] = my_character;
                                    }
                                }
                                vm_deallocate(mach_task_self_, (vm_address_t)str, str_l);
                            }
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
                
                task_suspend(task);
                vm_protect(task, addr, 4, FALSE, VM_PROT_READ|VM_PROT_WRITE|VM_PROT_COPY);
                vm_write(task, addr, (vm_address_t)&orig, 4);
                vm_protect(task, addr, 4, FALSE, VM_PROT_READ|VM_PROT_EXECUTE);
                task_resume(task);
            }
            usleep(100000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            int h_idx = 0;
            long child_count = 0;
            rbx_child_t* children = rbx_instance_get_children(task, my_character, &child_count);
            COPY_BUF(handle_cframe_addresses);
            if (children)
            {
                for (int i = 0 ; i < child_count ; i++)
                {
                    vm_address_t child = children[i].child_address;
                    if (rbx_instance_is_a(task, child, "Tool"))
                    {
                        vm_address_t h = rbx_instance_find_first_child(task, child, "Handle");
                        if (h)
                        {
                            __handle_cframe_addresses[h_idx] = rbx_basepart_get_properties_address(task, h) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                            h_idx++;
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
            }
            memcpy(handle_cframe_addresses, __handle_cframe_addresses, sizeof(handle_cframe_addresses));
            handle_count = h_idx;
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (tool_activate_loop_enabled && shared_memory->KEYS_DOWN[' '])
            {
                //rbx_print_descendants(task, my_character, 0, 1);
                long child_count = 0;
                rbx_child_t* children = rbx_instance_get_children(task, my_character, &child_count);
                if (children)
                {
                    for (int i = 0 ; i < child_count ; i++)
                    {
                        vm_address_t child = children[i].child_address;
                        if (rbx_instance_is_a(task, child, "Tool"))
                        {
                            shared_memory->FUNCTION_QUEUE[2].address = task_base_address + RBX_TOOL_ACTIVATE_METHOD_OFFSET;
                            shared_memory->FUNCTION_QUEUE[2].type = 2;
                            shared_memory->FUNCTION_QUEUE[2].async_type = FUNCTION_ASYNC_TYPE_MAIN;
                            
                            char args[48];
                            memcpy(args, &child, 8);
                            
                            memcpy(shared_memory->FUNCTION_QUEUE[2].arguments, args, 48);
                            shared_memory->FUNCTION_QUEUE[2].should_execute = true;
                            while (shared_memory->FUNCTION_QUEUE[2].should_execute)
                            {
                                usleep(1000);
                            }
                            usleep(100000);
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
            }
            usleep(100000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            backpack = rbx_instance_find_first_child(task, local_player, "Backpack");
            my_character = rbx_player_get_character(task, local_player);
            //rbx_print_descendants(task, my_character, 0, 2);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            my_humanoid = rbx_instance_find_first_child(task, my_character, "Humanoid");
            //rbx_basepart_set_gravity(task, my_hrp, 30);
            my_hrp_cframe_address = rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            vm_address_t my_char_folder = rbx_instance_get_parent(task, my_character);
            vm_address_t general = 0;
            vm_address_t enemy_char_folder = 0;
            if (my_char_folder == human_char_folder)
            {
                enemy_char_folder = orc_char_folder;
                general = rbx_instance_find_first_child(task, orc_char_folder, "Orc General");
            }
            if (my_char_folder == orc_char_folder)
            {
                enemy_char_folder = human_char_folder;
                general = rbx_instance_find_first_child(task, human_char_folder, "Human General");
            }
            general_torso = rbx_instance_find_first_child(task, general, "Torso");
            general_torso_cframe_address = rbx_basepart_get_properties_address(task, general_torso) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            //rbx_camera_set_camera_subject(task, camera, general);
            usleep(200000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            new_hrp_pos = vector3_add((vector3_t){.x = 0, .y = -60, .z = 0}, rbx_basepart_get_cframe(task, general_torso).pos);
            usleep(10000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^
    {
        for (;;)
        {
            usleep(5);
            if (general_torso && handle_count && shared_memory->KEYS_DOWN[' '])
            {
                for (int i = 0 ; i < 10000 ; i++)
                {
                    for (int _i = 0; _i < handle_count ; _i++)
                    {
                        vm_copy(task, general_torso_cframe_address, sizeof(rbx_cframe_t), handle_cframe_addresses[_i]);
                    }
                }
            }
            if (my_hrp && general_torso)
            {
                rbx_basepart_set_gravity(task, my_hrp, 0);
                vm_write(task, my_hrp_cframe_address + 0x24, (vm_address_t)&new_hrp_pos, 12);
            }
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(500);
            COPY_VAR(my_hrp)
            if (fly_enabled)
            {
                vector3_t vel;
                rbx_cframe_t cf = rbx_camera_get_cframe(task, camera);
                vector3_t lv = rbx_cframe_get_look_vector(cf);
                vector3_t rv = rbx_cframe_get_right_vector(cf);
                
                bzero(&vel, sizeof(vector3_t));
                
                unsigned char* keys_down = shared_memory->KEYS_DOWN;
                
                if (keys_down['w']) { vel = vector3_add(vector3_mul_num(lv, 1), vel); }
                if (keys_down['a']) { vel = vector3_add(vector3_mul_num(rv, -1), vel); }
                if (keys_down['s']) { vel = vector3_add(vector3_mul_num(lv, -1), vel); }
                if (keys_down['d']) { vel = vector3_add(vector3_mul_num(rv, 1), vel); }
                
                float magnitude = vector3_magnitude(vel);
                if (!isnan(magnitude) && magnitude > 0.0f)
                {
                    vel = vector3_div_num(vel,  magnitude);
                }
                vel = vector3_mul_num(vel, fly_speed);
                
                rbx_basepart_set_velocity(task, __my_hrp, vel);
                rbx_basepart_set_gravity(task, __my_hrp, 0.0f);
            }
            else
            {
                rbx_basepart_set_gravity(task, __my_hrp, 200);
            }
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            char str[100];
            bzero(str, 100);
            fgets(str, 100, stdin);
            
            if (strcmp(str, "sg\xa") == 0)
            {
                printf("SHOWING GENERAL...\n");
                vm_address_t s = general_torso;
                if (s)
                {
                    rbx_camera_set_camera_subject(task, camera, s);
                }
            }
            if (strcmp(str, "p\xa") == 0)
            {
                set_parent_hijack_enabled = !set_parent_hijack_enabled;
                printf("NEW PARENT HIJACK STATUS: %d\n", set_parent_hijack_enabled);
            }
            if (strcmp(str, "t\xa") == 0)
            {
                tool_activate_loop_enabled = !tool_activate_loop_enabled;
                printf("NEW TOOL LOOP STATUS: %d\n", tool_activate_loop_enabled);
            }
            if (strcmp(str, "fly\xa") == 0)
            {
                fly_enabled = true;
                rbx_smooth_fly(task, fly_enabled);
                printf("ENABLED FLY");
            }
            if (strcmp(str, "nofly\xa") == 0)
            {
                fly_enabled = false;
                rbx_smooth_fly(task, fly_enabled);
                printf("DISABLED FLY");
            }
        }
    });
    
}
