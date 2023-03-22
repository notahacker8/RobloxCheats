

#define RBX_FOB_ALL_LEGENDARY_FILTER if (gv > 31)
#define RBX_FOB_BAD_LEGENDARY_FILTER if (gv == 35 || gv == 36) //Spirit and Dragon Bone go brrrrrr..
#define RBX_FOB_GOOD_LEGENDARY_FILTER if (gv > 31 && gv != 35 && gv != 36)



void field_of_battle_collect_legendary_gem(task_t task)
{
    printf("- FIELD OF BATTLE (LEGENDARY GEM COLLECTOR) -\n");

    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t projectiles_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Projectiles");
    
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static char should_exit = false;
    static char gem_found = false;
    static vm_address_t gem = 0;
    static vector3_t gem_pos;
    gem_pos = vector3_init(-2050, 758, -1940); //Middle of lobby, up where the gems fall.
    
    static char* gem_dictionary[100];
    gem_dictionary[1] = "Red Diamond";
    gem_dictionary[2] = "Grandidierite";
    gem_dictionary[3] = "Poudretteite";
    gem_dictionary[4] = "Benitoite";
    gem_dictionary[5] = "Tanzanite";
    gem_dictionary[6] = "Alexandrite";
    gem_dictionary[7] = "Diamond";
    gem_dictionary[8] = "Sapphire";
    gem_dictionary[9] = "Emerald";
    gem_dictionary[10] = "Ruby";
    gem_dictionary[11] = "Lapis Lazuli";
    gem_dictionary[12] = "Topaz";
    gem_dictionary[13] = "Garnet";
    gem_dictionary[14] = "Aquamarine";
    gem_dictionary[15] = "Spinel";
    gem_dictionary[16] = "Amber";
    gem_dictionary[17] = "Titanite";
    gem_dictionary[18] = "Tourmaline";
    gem_dictionary[19] = "Kunzite";
    gem_dictionary[20] = "Amethyst";
    gem_dictionary[21] = "Citrine";
    gem_dictionary[22] = "Peridot";
    gem_dictionary[24] = "Onyx";
    gem_dictionary[25] = "Turquoise";
    gem_dictionary[27] = "Feldspar";
    gem_dictionary[28] = "Jade";
    gem_dictionary[29] = "Nephrite";
    gem_dictionary[30] = "Olivine";
    gem_dictionary[31] = "Copal";
    gem_dictionary[32] = "Mithril";
    gem_dictionary[33] = "Demonite";
    gem_dictionary[34] = "Fury Stone";
    gem_dictionary[35] = "Dragon Bone";
    gem_dictionary[36] = "Spirit Shard";
    gem_dictionary[42] = "Titan Core";
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (gem_found == true)
            {
                vm_address_t gem_parent = rbx_instance_get_parent(task, gem);
                if (gem_parent)
                {
                    rbx_cframe_t gem_cf = rbx_basepart_get_cframe(task, gem);
                    gem_pos = gem_cf.pos;
                }
                else
                {
                    gem_pos.y = -999999.0f;
                    should_exit = true;
                }
            }
            usleep(10);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            
            vm_address_t character = rbx_player_get_character(task, local_player);
            vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
            rbx_basepart_set_gravity(task, hrp, 0);
            rbx_cframe_t cf = rbx_basepart_get_cframe(task, hrp);
            cf.pos = gem_pos;
            rbx_basepart_set_cframe(task, hrp, &cf);
            
            long projectile_count = 0;
            rbx_child_t* projectiles = rbx_instance_get_children(task, projectiles_folder, &projectile_count);
            if (projectiles)
            {
                
                if (gem_found == false)
                {
                    printf(" > SCANNING GEMS...\n");
                }
                else
                {
                    if (should_exit == true)
                    {
                        printf(" > GEM COLLECTED SUCCESSFULLY!\n > EXITING...\n");
                        exit(0);
                    }
                }
                
                for (long i = 0 ; i < projectile_count ; i++)
                {
                    vm_address_t projectile = projectiles[i].child_address;
                    vm_address_t gem_type = rbx_instance_find_first_child(task, projectile, "GemType");
                    if (gem_type)
                    {
                        long gv = rbx_intvalue_get_value(task, gem_type);
                        RBX_FOB_ALL_LEGENDARY_FILTER
                        {
                            printf(" > GEM FOUND!!! (GemType: %ld) (Name: %s)\n", gv, gem_dictionary[gv]);
                            if (gem_found == false)
                            {
                                gem = projectile;
                            }
                            gem_found = true;
                        }
                    }
                }
                
                vm_deallocate(mach_task_self_, (vm_address_t)projectiles, projectile_count * sizeof(rbx_child_t));
            }
            usleep(20000);
        }
    });
   
    
}

void fob_equip_tool(task_t task,
                    vm_address_t load_address,
                    vm_address_t input_queue_offset,
                    vm_address_t input_queue_count_offset,
                    vm_address_t input_queue_finished_offset,
                    int keycode)
{
    static int input_count = 2;
    Input inputs[input_count];
    inputs[0].type = 1;
    inputs[0].duration = 10000;
    inputs[0].keycode = keycode;
    inputs[0].window_index = 0;
    
    inputs[1].type = 2;
    inputs[1].duration = 0;
    inputs[1].keycode = keycode;
    inputs[1].window_index = 0;
    
    char f = 0;
    vm_write(task, load_address + input_queue_offset, (vm_address_t)inputs, (int)sizeof(inputs));
    vm_write(task, load_address + input_queue_count_offset, (vm_address_t)&input_count, sizeof(int));
    vm_write(task, load_address + input_queue_finished_offset, (vm_address_t)&f, 1);
    
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
    
}

void fob_click(task_t task,
               vm_address_t load_address,
               vm_address_t input_queue_offset,
               vm_address_t input_queue_count_offset,
               vm_address_t input_queue_finished_offset)
{
    static int input_count = 2;
    Input inputs[input_count];
    
    inputs[0].type = 3;
    inputs[0].duration = 10000;
    inputs[0].window_index = 0;
    inputs[0].x = 200;
    inputs[0].y = 200;
    
    inputs[1].type = 4;
    inputs[1].duration = 0;
    inputs[1].window_index = 0;
    inputs[1].x = 200;
    inputs[1].y = 200;
    
    char f = 0;
    vm_write(task, load_address + input_queue_offset, (vm_address_t)inputs, (int)sizeof(inputs));
    vm_write(task, load_address + input_queue_count_offset, (vm_address_t)&input_count, sizeof(int));
    vm_write(task, load_address + input_queue_finished_offset, (vm_address_t)&f, 1);
    
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
}

#define RBX_FOB_RED_NOSED_FILTER_HEADER                     char npc_namelen; char* npc_name = rbx_instance_get_name(task, n, &npc_namelen); if (npc_name) { if (strstr(npc_name, "Red Nose")) {
#define RBX_FOB_RED_NOSED_FILTER_ENDER } vm_deallocate(mach_task_self_, (vm_address_t)npc_name, npc_namelen); }


void field_of_battle_auto_farm(task_t task)
{
    
    void* dlhandle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
    vm_offset_t input_usleep_time_offset = gdso(dlhandle, s_load_address, "INPUT_USLEEP_TIME");
    vm_offset_t input_queue_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE");
    vm_offset_t input_queue_count_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE_COUNT");
    vm_offset_t input_queue_finished_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE_FINISHED");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB__);
    int iut = 100000;
    vm_write(task, load_address + input_usleep_time_offset, (vm_address_t)&iut, sizeof(useconds_t));
    
    printf("- FIELD OF BATTLE (AUTO FARM) -\n");
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t backpack = rbx_instance_find_first_child(task, local_player, "Backpack");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t characters_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Characters");
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
    
    static rbx_cframe_t tool_grip;
    tool_grip.r0 = 1;
    tool_grip.r11 = 1;
    tool_grip.r22 = 1;
    tool_grip.pos.y = y_offset;
    
    static char npc_exists = false;
    static char safe_to_attack = true; //Lag switch-kick prevention
    static char is_going_afk = false;

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
            handle_cframe_address = rbx_basepart_get_properties_address(task, handle) + RBX_PART_PROPERTIES_CFRAME_OFFSET;
            
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
                    vm_address_t isnpc = rbx_instance_find_first_child(task, n, "IsNpc");
                    bool isnpc_value = rbx_boolvalue_get_value(task, isnpc);
                    //FOB_RED_NOSED_FILTER_HEADER
                    if (isnpc && isnpc_value == true)
                    {
                        vm_address_t h = rbx_instance_find_first_child(task, n, "Head");
                        vm_address_t w = rbx_instance_find_first_child_of_class(task, h, "Weld");
                        if (w)
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
                    //FOB_RED_NOSED_FILTER_ENDER
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
            if (server_player_count == 1)
            {
                rbx_camera_set_camera_subject(task, camera, npc);
                rbx_cframe_t _npc_cf = rbx_basepart_get_cframe(task, npc_torso);
                rbx_cframe_t my_hrp_cf = rbx_basepart_get_cframe(task, my_hrp);
                my_hrp_cf.pos.x = _npc_cf.pos.x;
                my_hrp_cf.pos.y = _npc_cf.pos.y + y_offset;
                my_hrp_cf.pos.z = _npc_cf.pos.z;
                rbx_basepart_set_cframe(task, my_hrp, &my_hrp_cf);
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

    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^
    {
        for (;;)
        {
            
            rbx_player_set_last_input_timestamp(task, local_player, 0.0f); //So we don't get kicked for being 'AFK'
            rbx_basepart_set_gravity(task, my_hrp, 0);
            
            rbx_child_t* plrs = rbx_instance_get_children(task, players_service, &server_player_count);
            vm_deallocate(mach_task_self_, (vm_address_t)plrs, server_player_count * sizeof(rbx_child_t));
            
            if (server_player_count > 1) //If someone else joins
            {
                if (is_going_afk == false)
                {
                    is_going_afk = true;
                    rbx_basepart_set_gravity(task, my_hrp, 200);
                    rbx_cframe_t cf = npc_cf;
                    cf.pos.y -= 999999.0f;
                    rbx_basepart_set_cframe(task, my_hrp, &cf);
                    printf("MULTIPLE PLAYERS DETECTED!!!\n > GOING AFK...\n");
                }
            }
            else
            {
                is_going_afk = false;
                if (char_tool == 0)
                {
                    ///https://eastmanreference.com/complete-list-of-applescript-key-codes
                    ///I press 5 to equip my sword (Heaven's Edge)
                    fob_equip_tool(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, 23);
                }
                
                if (npc_exists && safe_to_attack) //So we don't hit anything on accident (to prevent getting caught "lag switching")
                {
                    fob_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset);
                    for (int i = 0 ; i < 50000 ; i++)
                    {
                        vm_write(task, handle_cframe_address, (vm_offset_t)&npc_cf, sizeof(npc_cf));
                    }
                }
                
            }
            
            usleep(target_switch_delay_usleep_time);
        }
    });
}



void fob_troll(task_t task)
{
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t og_char = rbx_player_get_character(task, local_player);
    vm_address_t ogcharhrp = rbx_instance_find_first_child(task, og_char, "HumanoidRootPart");
    vm_address_t backpack = rbx_instance_find_first_child(task, local_player, "Backpack");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t characters_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Characters");
    vm_address_t orc_char_folder = rbx_instance_find_first_child(task, characters_folder, "Orc");
    vm_address_t human_char_folder = rbx_instance_find_first_child(task, characters_folder, "Human");
    vm_address_t gen = rbx_instance_find_first_child(task, orc_char_folder, "Orc General");
    vm_address_t genhead = rbx_instance_find_first_child(task, gen, "Head");
    rbx_cframe_t gencf = rbx_basepart_get_cframe(task, genhead);
    rbx_basepart_set_cframe(task, ogcharhrp, &gencf);
    rbx_camera_set_camera_subject(task, camera, gen);
    rbx_player_set_character(task, local_player, gen);
}


//rbx_print_descendants(task, unbreakable_folder, 0);
//rbx_print_children_profiles(task, unbreakable_folder);

/*
long unbreakable_count = 0;
vm_address_t* unbreakables = rbx_instance_get_children(task, unbreakable_folder, &unbreakable_count);
for (long i = 0 ; i < unbreakable_count ; i++)
{
    vm_address_t unbreakable = unbreakables[i];
    char* u_name;
    char u_name_len;
    if (unbreakable)
    {
        if (strcmp(rbx_instance_get_name(task, unbreakable, &u_name_len), "Ore") == 0)
        {
            long child_count = 0;
            vm_address_t* children = rbx_instance_get_children(task, unbreakable, &child_count);
            for (long x = 0 ; x < child_count ; x++)
            {
                vm_address_t child = children[x];
                if (child)
                {
                    char* c_name;
                    char c_name_len;
                    if (strcmp(rbx_instance_get_name(task, child, &c_name_len), "Gem") == 0)
                    {
                        vm_address_t gem = child;
                        vm_address_t hitby = rbx_instance_find_first_child(task, gem, "HitBy");
                        vm_address_t gemtype = rbx_instance_find_first_child(task, gem, "GemType");
                        long gtv = rbx_intvalue_get_value(task, gemtype);
                        if (gem_dictionary[gtv] == NULL)
                        {
                            if (gtv != 0)
                            {
                                printf("unlisted gem type: %ld\n", gtv);
                            }
                        }
                        if (hitby)
                        {
                            vm_address_t hitby_value = rbx_intvalue_get_value(task, hitby);
                            char pnl;
                            char* player_name = rbx_instance_get_name(task, hitby_value, &pnl);
                            if (player_name)
                            {
                                if (strcmp(player_name, "blockmincer") == 0)
                                {
                                    printf("GemType: %ld (%s)\n", gtv, gem_dictionary[gtv]);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
 */
