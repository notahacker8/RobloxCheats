


void field_of_battle_general_killer(task_t task)
{
    LIBESP_DYLIB_SETUP
    
    shared_memory->FUNCTION_USLEEP_TIME = 100;
    shared_memory->FUNCTION_COUNT = 10;
    
    shared_memory->INPUT_COUNT = 10;
    shared_memory->INPUT_USLEEP_TIME = 50000;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t stats = rbx_instance_find_first_child(task, game, "Stats");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t rs = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    //rbx_print_descendants(task, rs, 0, 3);
    vm_address_t characters_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Characters");
    
    static vm_address_t my_character = 0;
    static vm_address_t my_humanoid = 0;
    static vm_address_t backpack = 0;
    
    static vm_address_t general_torso = 0;
    static vm_address_t general_torso_cframe_address = 0;
    static int handle_count = 0;
    static vm_address_t handles[200];
    static vm_address_t handle_cframe_addresses[200];
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    
    static vector3_t new_hrp_pos;
    
    static vm_address_t orc_char_folder = 0 ;
    static vm_address_t human_char_folder = 0;
    static vm_address_t neutral_char_folder = 0 ;
    static vm_address_t undead_char_folder = 0;
    static vm_address_t demon_char_folder = 0;

    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    vm_address_t addr = task_base_address + 0x1330558 ;//RBX_SET_PARENT_HOOK_OFFSET;
    vm_address_t addr2 = task_base_address + 0x1c99568; //tool activate() calls this
    
    static vm_address_t tools_equipped[100];
    static int equipped_tool_count = 0;
    static int tool_activation_index = 0;
    
    static char hunt_neutral = false;
    static char hunt_demon = false;
    
    
    static ESP_BKPT bkpt1 = {.hit = false};
    bkpt1.trip_address = addr;
    shared_memory->BREAKPOINTS[0] = bkpt1;
    
    static char can_attack = true;
    static char can_set_cframes = false;
    static char show_npc = false;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            orc_char_folder = rbx_instance_find_first_child(task, characters_folder, "Orc");
            human_char_folder = rbx_instance_find_first_child(task, characters_folder, "Human");
            neutral_char_folder = rbx_instance_find_first_child(task, characters_folder, "Neutral");
            undead_char_folder = rbx_instance_find_first_child(task, characters_folder, "Undead");
            demon_char_folder = rbx_instance_find_first_child(task, characters_folder, "Demon");

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
            }
            if (my_char_folder == orc_char_folder)
            {
                enemy_char_folder = human_char_folder;
            }
           // if (rbx_instance_find_first_child_partial(task, neutral_char_folder, "Caste") rbx_instance_find_first_child_partial(task, neutral_char_folder, "V"))
            if (hunt_neutral)
            {
                enemy_char_folder = neutral_char_folder;
            }
            if (hunt_demon)
            {
                enemy_char_folder = demon_char_folder;
            }
            //
            
            long l = 0;
            rbx_child_t* c = rbx_instance_get_children(task, enemy_char_folder, &l);
            if (c)
            {
                //for (long i = 0 ; i < l ; i++) //Get the general first
                for (long i = l ; i > -1 ; i--) //Get the lower npc first
                {
                    vm_address_t n = c[i].child_address;
                    //rbx_print_descendants(task, n, 0, 1);
                    vm_address_t isnpc = rbx_instance_find_first_child(task, n, "IsNpc");
                    bool isnpc_value = rbx_boolvalue_get_value(task, isnpc);
                    //printf("%d\n", isnpc_value);
                    if (isnpc && isnpc_value == true)
                    {
                        vm_address_t h = rbx_instance_find_first_child(task, n, "Humanoid");
                        //rbx_print_descendants(task, h, 0, 1);
                        //vm_address_t w = rbx_instance_find_first_child_of_class(task, h, "Weld");
                        if (rbx_humanoid_get_health(task, h) > 0)
                        {
                            general = n;
                            i = -1; //Get the lower npc first
                            //i = l; //Get the general first
                        }
                        else
                        {
                            if (n == general && rbx_instance_is_valid_child(task, n))
                            {
                                /*can_attack = false;
                                sleep(1);
                                can_attack = true;*/
                            }
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)c, l * sizeof(rbx_child_t));
            }
            if (general)
            {
                rbx_print_descendants(task, general, 0, 0);
                general_torso = rbx_instance_find_first_child(task, general, "Torso");
                general_torso_cframe_address = rbx_basepart_get_properties_address(task, general_torso) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                if (show_npc)
                {
                    if (general && rbx_humanoid_get_health(task, my_humanoid) > 0)
                    {
                        rbx_camera_set_camera_subject(task, camera, general);
                    }
                    else
                    {
                        rbx_camera_set_camera_subject(task, camera, my_hrp);
                    }
                }
                else
                {
                    rbx_camera_set_camera_subject(task, camera, my_hrp);
                }
            }
            else
            {
                general_torso = 0;
            }
            usleep(500000);
        }
    });
    
    
    //edit_exec_memory(task, __BRK_0__, addr, 4);
    
    //0x19d500
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        int orig = -788216833;
        for (;;)
        {
            if (shared_memory->APP_IS_ACTIVE && shared_memory->KEYS_DOWN['p'])
            {
                    //printf("setting bpt\n");
                    edit_exec_memory(task, __BRK_0__, addr, 4);
                   // parent_hijack_enabled = true;
            }
            else
            {
                //printf("removing bpt\n");
                //parent_hijack_enabled = false;
                edit_exec_memory(task, (char*)&orig, addr, 4);
            }
            usleep(1000);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        int orig = -788216833;
        for (;;)
        {
            if (shared_memory->BREAKPOINTS[0].hit)
            {
                vm_address_t x0 = shared_memory->BREAKPOINTS[0].MCTX.__ss.__x[0];
                vm_address_t x1 = shared_memory->BREAKPOINTS[0].MCTX.__ss.__x[1];
                if (x0 == stats)
                {
                    vm_address_t tool = 0;
                    FOR_EACH_CHILD(backpack, {
                        if (rbx_instance_is_a(task, child, "Tool"))
                        {
                            vm_address_t wtype = rbx_instance_find_first_child(task, child, "WeaponType");
                            // rbx_print_descendants(task, wtype, 0, 0);
                            long str_l = 0;
                            char* str = rbx_stringvalue_get_value(task, wtype, &str_l);
                            if (str)
                            {
                                if (strcmp(str, "Hammer") == 0 ||
                                    strcmp(str, "Axe") == 0 ||
                                    strcmp(str, "Sword") == 0 ||
                                    strcmp(str, "Spear") == 0 ||
                                    strcmp(str, "Tool") == 0)
                                {
                                    tool = child;
                                    //rbx_print_descendants(task, tool, 0, 1);
                                    
                                }
                                vm_deallocate(mach_task_self_, (vm_address_t)str, str_l);
                            }
                        }
                    })
                    if (rbx_instance_is_valid_child(task, tool) && rbx_instance_is_valid_child(task, my_character))
                    {
                        shared_memory->BREAKPOINTS[0].MCTX.__ss.__x[0] = tool;
                        shared_memory->BREAKPOINTS[0].MCTX.__ss.__x[1] = my_character;
                        usleep(10);
                    }
                }
                edit_exec_memory(task, (char*)&orig, addr, 4);
                shared_memory->BREAKPOINTS[0].hit = false;
            }
            usleep(10);
        }
    });
     
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            int __tool_idx = 0;
            int h_idx = 0;
            
            COPY_BUF(tools_equipped);
            COPY_BUF(handles);
            COPY_BUF(handle_cframe_addresses);
            FOR_EACH_CHILD(my_character, {
                //vm_address_t child = children[i].child_address;
                if (rbx_instance_is_a(task, child, "Tool"))
                {
                    __tools_equipped[__tool_idx++] = child;
                    vm_address_t h = rbx_instance_find_first_child(task, child, "Handle");
                    if (h)
                    {
                        __handles[h_idx] = h;
                        __handle_cframe_addresses[h_idx] = rbx_basepart_get_properties_address(task, h) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                        h_idx++;
                    }
                }
            })
            memcpy(tools_equipped, __tools_equipped, sizeof(tools_equipped));
            memcpy(handles, __handles, sizeof(handles));
            memcpy(handle_cframe_addresses, __handle_cframe_addresses, sizeof(handle_cframe_addresses));
            handle_count = h_idx;
            equipped_tool_count = __tool_idx;
            sleep(2);
        }
    });
     
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (shared_memory->APP_IS_ACTIVE && shared_memory->KEYS_DOWN['m'])
            {
                show_npc = true;
                sleep(1);
                can_set_cframes = true;
                for (int i = 0 ; i < equipped_tool_count ; i++)
                {
                    vm_address_t tool = tools_equipped[i];
                    if (rbx_instance_is_valid_child(task, tool) && can_attack)
                    {
                        //int a = 5;
                        //vm_write(task, tool + 0x410, &a, 4);
                        shared_memory->FUNCTION_QUEUE[2].address = task_base_address+ RBX_TOOL_ACTIVATE_METHOD_OFFSET; //0x1c87048; //0x1c87094; //RBX_TOOL_ACTIVATE_METHOD_OFFSET;
                        shared_memory->FUNCTION_QUEUE[2].type = 1;
                        shared_memory->FUNCTION_QUEUE[2].async_type = FUNCTION_SYNC_TYPE_MAIN;
                        
                        char args[48];
                        memcpy(args, &tool, 8);
                        
                        memcpy(shared_memory->FUNCTION_QUEUE[2].arguments, args, 48);
                        shared_memory->FUNCTION_QUEUE[2].should_execute = true;
                        while (shared_memory->FUNCTION_QUEUE[2].should_execute)
                        {
                            usleep(10);
                        }
                    }
                }
                usleep(10000);
                can_set_cframes = false;
            }
            else
            {
                show_npc = false;
                usleep(100000);
            }
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        float p = -1;
        for (;;)
        {
            if (rbx_instance_is_valid_child(task, general_torso))
            {
                p = -p;
                new_hrp_pos = vector3_add((vector3_t){.x = 0, .y = 2000, .z = 0}, rbx_basepart_get_cframe(task, general_torso).pos);
            }
            //rbx_basepart_set_gravity(task, my_hrp, 0);
            usleep(1000);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^
    {
        rbx_cframe_t cf = DEFAULT_CFRAME;
        for (;;)
        {
            usleep(30);
            if (shared_memory->KEYS_DOWN['m'])
            {
                if (my_hrp)
                {
                    //cf.pos = new_hrp_pos;
                    //rbx_basepart_set_cframe(task, my_hrp, cf);
                    vm_write(task, my_hrp_cframe_address + 0x24, (vm_address_t)&new_hrp_pos, 12);
                }
            }
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (shared_memory->APP_IS_ACTIVE && shared_memory->KEYS_DOWN['m'])
            {
                //those_who_remain_aimbot_patch(task, task_base_address, true);
                COPY_BUF(handles)
                COPY_VAR(handle_count)
                COPY_VAR(general_torso)
                those_who_remain_aimbot_patch(task, task_base_address, true);
                for (int j = 0 ; j < 30 ; j++)
                {
                    for (int i = 0 ; i < __handle_count ; i++)
                    {
                        if (can_attack)
                        {
                            rbx_basepart_set_cframe(task, __handles[i], rbx_basepart_get_cframe(task, __general_torso));
                        }
                    }
                }
                those_who_remain_aimbot_patch(task, task_base_address, false);
            }
            else
            {
                //those_who_remain_aimbot_patch(task, task_base_address, false);
            }
            usleep(100000);
        }
    });
    
    static char fly_enabled = false;
    static float fly_speed = 200;
    static float new_gravity = 200;
    
    BASIC_FLY_ENGINE(BASIC_FLY_CONTROL_CODE, 300)
    
#pragma mark f
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(500);
            if (true)
            {
                if (shared_memory->KEYS_DOWN['f'] == true)
                {
                    fly_enabled = !fly_enabled;
                    rbx_smooth_fly(task, fly_enabled);
                    while (shared_memory->KEYS_DOWN['f'] == true)
                    {
                        usleep(500);
                    }
                }
            }
            else
            {
                sleep(1);
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
            if (strcmp(str, "f\xa") == 0)
            {
                rbx_smooth_fly(task, true);
            }
            if (strcmp(str, "nf\xa") == 0)
            {
                rbx_smooth_fly(task, false);
            }
            if (strcmp(str, "neu\xa") == 0)
            {
                hunt_neutral = true;
            }
            if (strcmp(str, "dem\xa") == 0)
            {
                hunt_demon = true;
            }
            if (strcmp(str, "reset\xa") == 0)
            {
                hunt_demon = false;
                hunt_neutral = false;
            }
            if (strcmp(str, "st\xa") == 0)
            {
                show_npc = !show_npc;
            }
        }
    });
    
}






















void bpt_test(task_t task)
{
    LIBESP_DYLIB_SETUP
    
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    vm_address_t addr = task_base_address + 0x1330558 ;//RBX_SET_PARENT_HOOK_OFFSET;
    vm_address_t addr2 = task_base_address + 0x1c99568; //RBX_SET_PARENT_HOOK_OFFSET;
    
    shared_memory->FUNCTION_USLEEP_TIME = 100;
    shared_memory->FUNCTION_COUNT = 10;
    
    shared_memory->INPUT_COUNT = 10;
    shared_memory->INPUT_USLEEP_TIME = 50000;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t stats = rbx_instance_find_first_child(task, game, "Stats");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    rbx_print_descendants(task, workspace, 0, 1);
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t characters_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Characters");
    
    static vm_address_t my_character = 0;
    static vm_address_t my_humanoid = 0;
    static vm_address_t backpack = 0;
    
    my_character = rbx_player_get_character(task, local_player);
    backpack = rbx_instance_find_first_child_of_class(task, local_player, "Backpack");
    vm_address_t tool = rbx_instance_find_first_child_of_class(task, backpack, "Tool");
    
    
    mach_port_t exc_port;
    KR(mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &exc_port))
    KR(mach_port_insert_right(mach_task_self(), exc_port, exc_port, MACH_MSG_TYPE_MAKE_SEND))
    exception_mask_t mask = EXC_MASK_BREAKPOINT; // breakpoints only
    exception_behavior_t behavior = EXCEPTION_STATE_IDENTITY;
    thread_state_flavor_t flavor = ARM_THREAD_STATE64;
    KR(task_set_exception_ports(task, mask, exc_port, behavior, flavor));
    
    for (;;)
    {
        edit_exec_memory(task, __BRK_0__, addr, 4);
        
        __Request__exception_raise_state_identity_t msg;
        mach_msg((void*)&msg, MACH_RCV_MSG, 0, sizeof(msg), exc_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
        arm_thread_state64_t* state = (arm_thread_state64_t*)msg.old_state;
        
        my_character = rbx_player_get_character(task, local_player);
        backpack = rbx_instance_find_first_child_of_class(task, local_player, "Backpack");
        vm_address_t tool = rbx_instance_find_first_child_of_class(task, backpack, "Tool");
        
        rbx_print_descendants(task, state->__x[0], 0, 0);
        if (tool)
        {
            state->__x[0] = tool;
            state->__x[1] = my_character;
            
            //KR(thread_set_state(msg.thread.name, ARM_THREAD_STATE64, state, ARM_THREAD_STATE64_COUNT));
        }
        
        int orig = -788216833;
        edit_exec_memory(task, (vm_address_t)&orig, addr, 4);
        
        __Reply__exception_raise_state_identity_t reply;
        bzero(&reply, sizeof(reply));
        memcpy(&reply.Head, &msg.Head, sizeof(msg.Head));
        memcpy(&reply.NDR, &msg.NDR, sizeof(msg.NDR));
        reply.Head.msgh_size = sizeof(__Reply__exception_raise_state_identity_t);
        reply.Head.msgh_bits = MACH_MSGH_BITS(MACH_MSGH_BITS_REMOTE(msg.Head.msgh_bits), 0);
        reply.Head.msgh_remote_port = msg.Head.msgh_remote_port; // must be request port
        reply.Head.msgh_local_port = MACH_PORT_NULL;            // replies never receive anything
        reply.Head.msgh_id = 2406;            // reply ID;
        
        mach_msg((void*)&reply, MACH_SEND_MSG, reply.Head.msgh_size, 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
        
        
        usleep(5000);
    }
}



void hook_test(task_t task)
{
    LIBESP_DYLIB_SETUP
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    vm_address_t addr = task_base_address + 0x1330558 ;//RBX_SET_PARENT_HOOK_OFFSET;
    vm_address_t addr2 = task_base_address + 0x1c99568; //RBX_SET_PARENT_HOOK_OFFSET;
    
    shared_memory->FUNCTION_USLEEP_TIME = 100;
    shared_memory->FUNCTION_COUNT = 10;
    
    shared_memory->INPUT_COUNT = 10;
    shared_memory->INPUT_USLEEP_TIME = 50000;
    
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
    
    
    /*
     sub     sp, sp, #32
     stp     x14, x15, [sp, #0]
     stp     x16, x17, [sp, #16]
     
     adr x17, new_x0_ptr
     ldr x17, [x17]
     adr x16, new_x1_ptr
     ldr x16, [x16]
     adr x15, active_ptr
     ldr x15, [x15]
     ldr  x14, [x15]
     cbz  x14, if_zero
     ldr  x0, [x17]
     ldr  x1, [x16]
     
    if_zero:
     ldp     x16, x17, [sp, #16]
     ldp     x14, x15, [sp, #0]
     add     sp, sp, #32
     
     nop
     nop
     
     new_x0_ptr: .quad 0x0
     new_x1_ptr: .quad 0x0
     active_ptr: .quad 0x0
     */
    
    char hook_code[] = "\xff\x83\x00\xd1\xee\x3f\x00\xa9\xf0\x47\x01\xa9\xf1\x01\x00\x10\x31\x02\x40\xf9\xf0\x01\x00\x10\x10\x02\x40\xf9\xef\x01\x00\x10\xef\x01\x40\xf9\xee\x01\x40\xf9\x6e\x00\x00\xb4\x20\x02\x40\xf9\x01\x02\x40\xf9\xf0\x47\x41\xa9\xee\x3f\x40\xa9\xff\x83\x00\x91\x1f\x20\x03\xd5\x1f\x20\x03\xd5\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    char hook_code_size = sizeof(hook_code) - 1;
    
    vm_address_t new_x0_ptr = vm_allocate_rwx(task, NULL, 8 * 3);
    vm_address_t new_x1_ptr = new_x0_ptr + 8;
    vm_address_t active_ptr = new_x0_ptr + 16;
    
    memcpy(hook_code + hook_code_size - 24, &new_x0_ptr, 8);
    memcpy(hook_code + hook_code_size - 16, &new_x1_ptr, 8);
    memcpy(hook_code + hook_code_size - 8, &active_ptr, 8);
    
    
    vm_address_t hook_mem = vm_allocate_rwx(task, hook_code, hook_code_size);
    uint32_t og_inst = vm_read_uint32_t_value(task, addr);
    uint32_t jmpto_b = encode_b(addr, hook_mem);
    uint32_t jmpback_b = encode_b(hook_mem + hook_code_size - 28, addr + 4);
    
    vm_write(task, hook_mem + hook_code_size - 32, (vm_address_t)&og_inst, 4);
    vm_write(task, hook_mem + hook_code_size - 28, (vm_address_t)&jmpback_b, 4);
    
    vm_protect(task, hook_mem, hook_code_size, FALSE, VM_PROT_READ|VM_PROT_EXECUTE);
    
    task_suspend(task);
    vm_protect(task, addr, 4, FALSE, VM_PROT_READ|VM_PROT_WRITE|VM_PROT_COPY);
    vm_write(task, addr, (vm_address_t)&jmpto_b, 4);
    vm_protect(task, addr, 4, FALSE, VM_PROT_READ|VM_PROT_EXECUTE);
    task_resume(task);
    
    my_character = rbx_player_get_character(task, local_player);
    backpack = rbx_instance_find_first_child_of_class(task, local_player, "Backpack");
    vm_address_t tool = rbx_instance_find_first_child_of_class(task, backpack, "Tool");
    
    vm_write(task, new_x0_ptr, &tool, 8);
    vm_write(task, new_x1_ptr, &my_character, 8);
    
    printf("%p\n", hook_mem);
    printf("%p\n", active_ptr);
    
    //VM_FLAGS_ANYWHERE
}

