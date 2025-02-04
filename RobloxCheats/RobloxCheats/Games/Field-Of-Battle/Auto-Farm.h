

void field_of_battle_auto_farm(task_t task, char sword_key)
{
    printf("- FIELD OF BATTLE (AUTO FARM) -\n");
    
    LIBESP_DYLIB_SETUP
    
    useconds_t iut = 10000;
    
    shared_memory -> INPUT_COUNT = 3;
    shared_memory -> INPUT_USLEEP_TIME = iut;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t characters_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Characters");
    rbx_print_descendants(task, characters_folder, 0, 1);
    vm_address_t orc_char_folder = rbx_instance_find_first_child(task, characters_folder, "Orc");
    vm_address_t human_char_folder = rbx_instance_find_first_child(task, characters_folder, "Human");
    
    static float y_offset = -40;
    static vm_address_t enemy_char_folder = 0;
    static vm_address_t prev_npc = 0;
    static vm_address_t npc = 0;
    static vm_address_t npc_torso = 0;
    static vm_address_t npc_head = 0;
    static vm_address_t my_char = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t char_tool = 0;
    static vm_address_t delay1numbervalue = 0;
    
    static long server_player_count = 0;
    
    static double delay1numbervalue_value = 0.0f;
    static useconds_t target_switch_delay_usleep_time = 0;
    
    static vm_address_t handle = 0;
    static vm_address_t handle_cframe_address = 0;
    static rbx_cframe_t npc_cf;
    static vector3_t npc_pos;
    
    static char npc_exists = false;
    static char safe_to_attack = true; //Lag switch-kick prevention
    static char is_going_afk = false;
    
    static char is_only_player_check_enabled = true;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            char __npc_exists = false;
            my_char = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_char, "HumanoidRootPart");
            char_tool = rbx_instance_find_first_child_of_class(task, my_char, "Tool");
            delay1numbervalue = rbx_instance_find_first_child(task, char_tool, "Delay1");
            delay1numbervalue_value = rbx_numbervalue_get_value(task, delay1numbervalue);
            
            if (delay1numbervalue)
            {
                target_switch_delay_usleep_time = (useconds_t)(delay1numbervalue_value * 1000000.0f);
            }
            else
            {
                target_switch_delay_usleep_time = 1000000;
            }
            handle = rbx_instance_find_first_child(task, char_tool, "Handle");
            handle_cframe_address = rbx_basepart_get_properties_address(task, handle) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            
            enemy_char_folder = 0;
            if (rbx_instance_get_parent(task, my_char) == orc_char_folder)
            {
                enemy_char_folder = human_char_folder;
            }
            if (rbx_instance_get_parent(task, my_char) == human_char_folder)
            {
                enemy_char_folder = orc_char_folder;
            }
            
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
                            vm_address_t t = rbx_instance_find_first_child(task, n, "Torso");
                            __npc_exists = true;
                            if (npc != n)
                            {
                                safe_to_attack = false;
                                prev_npc = npc;
                                usleep(target_switch_delay_usleep_time); //Wait between switching targets so we don't get caught "lag switching"
                                safe_to_attack = true;
                            }
                            npc = n;
                            npc_head = h;
                            npc_torso = t;
                            i = -1; //Get the lower npc first
                            //i = l; //Get the general first
                        }
                    }
                }
            }
            vm_deallocate(mach_task_self_, (vm_address_t)c, l * sizeof(rbx_child_t));
            npc_exists = __npc_exists;
            usleep(200000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(500);
            if (server_player_count == 1 || !is_only_player_check_enabled)
            {
                rbx_camera_set_camera_subject(task, camera, npc);
                rbx_cframe_t _npc_cf = rbx_basepart_get_cframe(task, npc_torso);
                rbx_cframe_t my_hrp_cf = rbx_basepart_get_cframe(task, my_hrp);
                my_hrp_cf.pos.x = _npc_cf.pos.x;
                my_hrp_cf.pos.y = _npc_cf.pos.y + y_offset;
                my_hrp_cf.pos.z = _npc_cf.pos.z;
                rbx_basepart_set_cframe(task, my_hrp, my_hrp_cf);
                npc_cf = _npc_cf;
                npc_pos = _npc_cf.pos;
            }
            else
            {
                rbx_camera_set_camera_subject(task, camera, my_char);
            }
            //rbx_print_descendants(task, prev_npc, 0, 0);
            //rbx_print_descendants(task, npc, 0, 0);
        }
    });

    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^
    {
        for (;;)
        {
            
            rbx_basepart_set_gravity(task, my_hrp, 0);
            
            rbx_child_t* plrs = rbx_instance_get_children(task, players_service, &server_player_count);
            vm_deallocate(mach_task_self_, (vm_address_t)plrs, server_player_count * sizeof(rbx_child_t));
            
            
            if (server_player_count > 1 && is_only_player_check_enabled) //If someone else joins
            {
                if (is_going_afk == false)
                {
                    rbx_basepart_set_gravity(task, my_hrp, 200);
                    rbx_cframe_t cf = npc_cf;
                    cf.pos.y -= 999999.0f;
                    rbx_basepart_set_cframe(task, my_hrp, cf);
                    printf("MULTIPLE PLAYERS DETECTED!!!\n > GOING AFK...\n");
                    NSLog(@"Current status: AFK");
                }
                is_going_afk = true;
            }
            else
            {
                //click
                if (is_going_afk == true)
                {
                    is_going_afk = false;
                    NSLog(@"Current status: Active");
                }
                if (char_tool == 0)
                {
                    printf("Equipping Sword...\n");
                    //Equip our tool/sword.
                    send_keypress_prechecking(shared_memory, 2, keycode_for_character(sword_key), &sword_key, 100000);
                }
                
                if (npc_exists && safe_to_attack) //So we don't hit anything on accident (to prevent getting caught "lag switching")
                {
                    {
                        send_leftclick_prechecking(shared_memory, 1, shared_memory->MOUSE_X, shared_memory->MOUSE_Y, 10000);
                    }
                    //Teleport the sword to the enemy.
                    for (int i = 0 ; i < 100000 ; i++)
                    {
                        vm_write(task, handle_cframe_address, (vm_offset_t)&npc_cf, sizeof(npc_cf));
                    }
                    
                }
                
            }
            
            usleep(target_switch_delay_usleep_time);
        }
    });
    
    
}

