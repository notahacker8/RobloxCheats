
#define BLOX_FRUITS_ITEM_COLLECTION_MODE_NONE 0
#define BLOX_FRUITS_ITEM_COLLECTION_MODE_CHESTS_ONLY 1
#define BLOX_FRUITS_ITEM_COLLECTION_MODE_EMBERS_ONLY 2
#define BLOX_FRUITS_ITEM_COLLECTION_MODE_MIRAGE_CHESTS_ONLY 3





void blox_fruits_cheat(task_t task)
{
    
    printf("\n- BLOX FRUITS -\n");
    pid_t pid = -1;
    pid_for_task(task, &pid);
    LIBESP_DYLIB_SETUP
    
    [NSEvent addGlobalMonitorForEventsMatchingMask:NSEventMaskKeyDown handler:^(NSEvent *event) {
            // Get the key code of the pressed key
            NSUInteger keyCode = [event keyCode];
            NSLog(@"Key pressed: %lu", (unsigned long)keyCode);
        }];
    
#pragma mark - Globals -
    
    static ESP_Color kitsune_esp_color = {.r = 0.2, .g = 0.8, .b = 1, .a = 1};
    static ESP_Color mirage_esp_color = {.r = 1, .g = 0, .b = 0.8, .a = 1};
    static ESP_Color fruit_esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    static ESP_Color red_flower_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color blue_flower_esp_color = {.r = 0, .g = 0, .b = 1, .a = 1};
    static ESP_Color quest_torches_esp_color = {.r = 1, .g = 0.3, .b = 0, .a = 1};
    static ESP_Color custom_haki_esp_color = {.r = 1, .g = 1, .b = 1, .a = 1};
    static ESP_Color target_esp_color1 = {.r = 0, .g = 0.75, .b = 0.75, .a = 1};
    static ESP_Color target_esp_color2 = {.r = 1, .g = 0, .b = 0.3, .a = 1};
    
    static float bail_y = 5000.0f;
    
    static float new_gravity = 100;
    
ESP_GLOBAL_VARIABLES
    
    static char sidebar_text[MAX_ESP_TEXT_LENGTH];
    
    static useconds_t esput = 1500;
    static useconds_t fut = 200000;
    static useconds_t iut = 200000;

    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = esput;
    strcpy(shared_memory->ESP_TEXT_FONT_NAME, "Menlo");
    
    shared_memory->FUNCTION_USLEEP_TIME = fut;
    shared_memory->INPUT_USLEEP_TIME = iut;
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    vm_address_t humanoid_set_walkspeed_func = task_base_address + RBX_HUMANOID_WALKSPEED_SETTER_METHOD_OFFSET;
    vm_address_t humanoid_set_health_func = task_base_address + RBX_HUMANOID_HEALTH_SETTER_METHOD_OFFSET;
    vm_address_t tool_activate_func = task_base_address + RBX_TOOL_ACTIVATE_METHOD_OFFSET;
    
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    //vm_address_t replicated_storage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t map_model = rbx_instance_find_first_child(task, workspace, "Map");
    vm_address_t raidmap_model = rbx_instance_find_first_child(task, map_model, "RaidMap");
    //rbx_print_descendants(task, map_model, 0, 1);
    //rbx_print_descendants(task, raidmap_model, 0, 2);
    //vm_address_t waterbase_plane = rbx_instance_find_first_child(task, map_model, "WaterBase-Plane");
    //vector3_t s = rbx_basepart_get_size(task, waterbase_plane);
    //printf("%f, %f, %f\n", s.x, s.y, s.z);
    vm_address_t _worldorigin_folder = rbx_instance_find_first_child(task, workspace, "_WorldOrigin");
    vm_address_t locations_folder = rbx_instance_find_first_child(task, _worldorigin_folder, "Locations");
    vm_address_t npcs_folder = rbx_instance_find_first_child(task, workspace, "NPCs");
    vm_address_t enemies_folder = rbx_instance_find_first_child(task, workspace, "Enemies");
    vm_address_t seaevents_folder = rbx_instance_find_first_child(task, workspace, "SeaEvents");
    vm_address_t seabeasts_folder = rbx_instance_find_first_child(task, workspace, "SeaBeasts");
    vm_address_t boats_folder = rbx_instance_find_first_child(task, workspace, "Boats");
    vm_address_t lighting_service = rbx_instance_find_first_child_of_class(task, game, "Lighting");
    vm_address_t sky = rbx_instance_find_first_child_of_class(task, lighting_service, "Sky");
    static char* fullmoon_assetid = "9709149431"; //http://www.roblox.com/asset/?id=9709149431
    static char* waxing_gibbous_assetid = "9709149052"; //http://www.roblox.com/asset/?id=9709149052
    static int current_clocktime = 0;
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    rbx_print_descendants(task, local_player, 0, 0);
    //rbx_print_descendants(task, workspace, 0, 100);
    static vm_address_t minktrial_endpoint = 0;
    static vm_address_t floating_turtle_island = 0;
    static vm_address_t floating_turtle_questtorches_model = 0;
    static vm_address_t mfinv = 0;
    static vm_address_t dfinv = 0;
    
    static vm_address_t my_character = 0;
    static vm_address_t my_character_parts[200];
    static long my_character_part_count = 0;
    static vm_address_t my_humanoid = 0;
    static vm_address_t my_humanoid_current_seat_part = 0;
    static vm_address_t my_humanoid_current_seat_part_cframe_address = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    static vm_address_t my_boat, my_boat_seat;
    
    static vm_address_t energy_icv = 0;
    
    static vm_address_t other_player_hrps[200];
    static vm_address_t other_player_hums[200];
    static int other_player_count = 0;
    
    static vm_address_t npc_hums[200];
    static vm_address_t npc_hrps[200];
    static int npc_hrp_count = 0;
    
    static char locking_on_enemy = false;
    static vm_address_t closest_hum_to_mouse = 0;
    static vm_address_t closest_hrp_to_mouse = 0;
    static vm_address_t closest_hrp_to_mouse_cframe_address = 0;
    static float closest_hrp_to_mouse_dist;
    static vm_address_t hrp_to_lock_on = 0;
    static vm_address_t hum_to_lock_on = 0;
    static vm_address_t hrp_to_lock_on_cframe_address = 0;
    
    static char bailing = false;
    
    static vm_address_t fruit_models[200];
    static int fruit_count = 0;
    
    static vm_address_t mirage_chests[200];
    static int mirage_chest_count = 0;
    
    static vm_address_t chests[200];
    static int chest_count = 0;
    
    static vm_address_t embers[200];
    static int ember_count = 0;
    
    static int item_collection_mode = BLOX_FRUITS_ITEM_COLLECTION_MODE_NONE;
    static vm_address_t items[200];
    static int item_count = 0;
    static char item_cancollide_check_enabled = false;
    static vm_address_t closest_item = 0;
    
    static vm_address_t mystic_island = 0;
    static vm_address_t mystic_island_chests_folder = 0;
    static vm_address_t mirage_gear = 0;
    static vm_address_t kitsune_island = 0;

    static float fly_speed = 275.0f;
    static float aimbot_pixel_radius = 150;
    static float aimbot_max_dist = 250;
    static float aimbot_max_tp_dist = 150;
    static float boat_y = 1000;
    static vector3_t new_npc_hitbox_size = {.x = 120, .y = 120, .z = 120};
    
    static char esp_enabled = true;
    static char noclip_enabled = false;
    static char fly_enabled = false;
    static char boatfly_enabled = true;
    
    static char aimbot_enabled = true;
    static char tp_aimbot_enabled = true;
    
    static char high_boat = false;
    
    static char raid_assist_enabled = false;
    static char item_collect_enabled = false;
    static char show_torches = false;
    static char auto_raid_enabled = false;
    
    static vm_address_t next_raid_island_part = 0;
    
    static char lock_keybind_enabled = true;
    static char fly_keybind_enabled = true;
    static char boat_keybind_enabled = true;
    static char bail_keybind_enabled = true;
    static char priority_keybind_enabled = true;
    
    static pid_t active_app_pid = 0;
    active_app_pid = pid;
    
    static char low_prio = false;
    
    static char is_waxing_gibbous = false;
    static char is_full_moon = false;
    static char mirage_island_exists = false;
    static char kitsune_island_exists = false;
    
    static const float window_inset = 100;
    
    static Input mouse_aimbot_input = {.type = INPUT_TYPE_MOUSE_MOVE};
    
    shared_memory->FUNCTION_COUNT = 7;
    shared_memory->INPUT_COUNT = 1;
    
    static char controller_player_name[100] = "";
    static vm_address_t controller_player = 0;
    
    static char is_levi_bot_program = false;
    static int levi_bot_cannon_seat_index = 0;
    static vm_address_t levi_bot_target_seat = 0;
    static char levi_bot_should_follow_seat = false;
    
    ANTI_AFK
    
#pragma mark - Object Loops -
    
    
#pragma mark Instances [2]
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            {
                rbx_numbervalue_set_value(task, mfinv, -100000.0f);
                rbx_numbervalue_set_value(task, dfinv, -100000.0f);
                my_humanoid_current_seat_part = rbx_humanoid_get_seatpart(task, my_humanoid);
                if (rbx_instance_is_valid_child(task, my_humanoid_current_seat_part))
                {
                    my_humanoid_current_seat_part_cframe_address = rbx_basepart_get_properties_address(task, my_humanoid_current_seat_part) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                }
                else
                {
                    high_boat = false;
                    my_humanoid_current_seat_part_cframe_address = 0;
                }
            }
            usleep(100000);
        }
    });
    
#pragma mark Instances [1]
    
    BASIC_NOCLIP_PART_GETTER
    
    /*dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            rbx_intvalue_set_value(task, energy_icv, 12845);
            usleep(100);
        }
    });*/
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            my_character = rbx_player_get_character(task, local_player);
            //rbx_print_descendants(task, my_character, 0, 1);
            //rbx_print_descendants(task, locations_folder, 0, 1);
            //energy_icv = rbx_instance_find_first_child(task, my_character, "Energy");
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            my_humanoid = rbx_instance_find_first_child_of_class(task, my_character, "Humanoid");
            if (my_hrp)
            {
                my_hrp_cframe_address = rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                //printf("%p\n", my_hrp_cframe_address);
            }
            else
            {
                my_hrp_cframe_address = 0;
            }
            
            long mtid_len = 0;
            //rbx_print_descendants(task,map_model, 0, 3);
            char* mtid = rbx_sky_get_moontextureid(task, sky, &mtid_len);
            if (mtid)
            {
                //printf("%s\n", mtid);
                int ctt = rbx_lighting_get_clocktime_tick(task, lighting_service);
                float hour = ((float)ctt)/((float)RBX_LIGHTING_CLOCKTIME_HOUR_TICK_COUNT);
                current_clocktime = ctt;
                if (strstr(mtid, fullmoon_assetid))
                {
                    //The game changes the next night's moon after noon, and 'the full moon ends' at 5 am that night.
                    if (hour > 12 || hour < 5)
                    {
                        is_full_moon = true;
                        is_waxing_gibbous = false;
                    }
                    else
                    {
                        is_full_moon = false;
                    }
                }
                else
                {
                    if (strstr(mtid, waxing_gibbous_assetid))
                    {
                        is_waxing_gibbous = true;
                    }
                    else
                    {
                        is_waxing_gibbous = false;
                    }
                    is_full_moon = false;
                }
                vm_deallocate(mach_task_self_, (vm_address_t)mtid, mtid_len);
            }
            else
            {
                is_full_moon = false;
            }
            vm_address_t next_raid_island = rbx_instance_find_last_child_of_class(task, raidmap_model, "Model");
            next_raid_island_part = rbx_instance_find_last_child_of_class(task, next_raid_island, "Part");
            //rbx_print_descendants(task, my_character, 0, 1);
            //rbx_print_descendants(task, rbx_instance_find_last_child_of_class_and_name(task, my_character, "Accessory", "Mammoth"), 0, 1);
            {
                long child_count = 0;
                rbx_child_t* children = rbx_instance_get_children(task, enemies_folder, &child_count);
                int __b = 0;
                int __i = 0;
                if (children)
                {
                    for (long i = 0 ; i < child_count ; i++)
                    {
                        vm_address_t enemy = children[i].child_address;
                        vm_address_t enemy_hrp = rbx_instance_find_first_child(task, enemy, "HumanoidRootPart");
                        vm_address_t enemy_hum = rbx_instance_find_first_child(task, enemy, "Humanoid");
                        
                        rbx_basepart_set_size(task, enemy_hrp, new_npc_hitbox_size);
                        
                            if (__i < 200)
                            {
                                npc_hrps[__i] = enemy_hrp;
                                npc_hums[__i] = enemy_hum;
                                __i++;
                            }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
                npc_hrp_count = __i;
            }
            controller_player = rbx_instance_find_first_child(task, players_service, controller_player_name);
            
            vm_address_t minktrial_model = rbx_instance_find_first_child(task, map_model, "MinkTrial");
            if (minktrial_model)
            {
                //This is actually the enpoint for some reason.
                minktrial_endpoint = rbx_instance_find_first_child(task, workspace, "StartPoint");
            }
            //rbx_print_descendants(task, map_model, 0, 1);
            floating_turtle_island = rbx_instance_find_first_child(task, map_model, "Turtle");
            floating_turtle_questtorches_model = rbx_instance_find_first_child(task, floating_turtle_island, "QuestTorches");
            mystic_island = rbx_instance_find_first_child(task, map_model, "MysticIsland");
            mystic_island_chests_folder = rbx_instance_find_first_child(task, mystic_island, "Chests");
            kitsune_island = rbx_instance_find_first_child(task, map_model, "KitsuneIsland");
            //rbx_print_descendants(task, 0x14d8a0c30, 0, 2);
            //rbx_camera_set_camera_subject(task, camera, 0x17cb6ab40);
            /*
             _WorldOrigin : Folder
             Locations : Model
             */
            vm_address_t __mg = 0;
            if (mystic_island){
                mirage_island_exists = true;
                    long child_count = 0;
                    rbx_child_t* children = rbx_instance_get_children(task,mystic_island, &child_count);
                    if (children)
                    {
                        for (long i = 0 ; i < child_count ; i++)
                        {
                            vm_address_t child = children[i].child_address;
                            if (rbx_instance_is_a(task, child, "MeshPart"))
                            {
                                vector3_t sz = rbx_basepart_get_size(task, child);
                                if (truncf(sz.x) == 3 && truncf(sz.y) == 0 && truncf(sz.z) == 3)
                                {
                                    __mg = child;
                                    i = child_count;
                                }
                                //printf("%f %f %f\n", sz.x, sz.y, sz.z);
                                ///3.236792, 0.443407, 3.236792 is the real size
                            }
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                    }
            }else{mirage_island_exists = false;}
            mirage_gear = __mg;
            
            if (kitsune_island){kitsune_island_exists = true;}else{kitsune_island_exists = false;}
            
            vm_address_t lighting_layers = rbx_instance_find_first_child(task, lighting_service, "LightingLayers");
            vm_address_t miragefog_atm = rbx_instance_find_first_child(task, lighting_layers, "MirageFog");
            vm_address_t darkfog_atm = rbx_instance_find_first_child(task, lighting_layers, "DarkFog");

            mfinv = rbx_instance_find_first_child_of_class(task, miragefog_atm, "NumberValue"); //Intensity
            dfinv = rbx_instance_find_first_child_of_class(task, darkfog_atm, "NumberValue"); //Intensity
            
            {
                vm_address_t __my_boat = 0;
                vm_address_t __my_boat_seat = 0;
                long child_count = 0;
                rbx_child_t* children = rbx_instance_get_children(task, boats_folder, &child_count);
                if (children)
                {
                    for (long i = 0 ; i < child_count ; i++)
                    {
                        vm_address_t boat = children[i].child_address;
                        vm_address_t boat_owner_ov = rbx_instance_find_first_child(task, boat, "Owner");
                        vm_address_t owner = rbx_objectvalue_get_value(task, boat_owner_ov);
                        if (owner == local_player)
                        {
                            __my_boat = boat;
                            __my_boat_seat = vm_read_uint64_value(task, __my_boat + RBX_MODEL_PRIMARYPART_OFFSET);
                        }
                        if (is_levi_bot_program)
                        {
                            if (owner == controller_player)
                            {
                                vm_address_t cannon = rbx_instance_find_child_at_index(task, boat, "Cannon", levi_bot_cannon_seat_index);
                                levi_bot_target_seat = rbx_instance_find_last_child_of_class(task, cannon, "Seat");
                                levi_bot_should_follow_seat = true;
                            }
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
                my_boat_seat = __my_boat_seat;
                my_boat = __my_boat;
            }
            sleep(2);
        }
    });
    
#pragma mark ESP Object Loop
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            //printf("S\n");
            int esp_part_index = 0;
            int fruit_index = 0;
            int chest_index = 0;
            int mirage_chest_index = 0;
            int ember_index = 0;
            
            /*{
                vm_address_t o = rbx_instance_find_first_child(task, workspace, "_WorldOrigin");
                vm_address_t o1 = rbx_instance_find_first_child(task, o, "Locations");
                vm_address_t l = rbx_instance_find_first_child(task, o1, "l'Église de Prophétie");
                //APPEND_ESP_OBJ(l, quest_torches_esp_color, "ROOM");
                rbx_print_descendants(task, o1, 0, 1);
            }*/
            
            APPEND_ESP_OBJ(rbx_instance_find_last_child_of_class(task, locations_folder, "Part"), quest_torches_esp_color, "?????");
            
            APPEND_ESP_OBJ(my_boat_seat, quest_torches_esp_color, "MY BOAT");
            APPEND_ESP_OBJ(minktrial_endpoint, quest_torches_esp_color, "ENDPOINT");
            
            if (mystic_island)
            {
                vm_address_t part = vm_read_uint64_value(task, mystic_island + RBX_MODEL_PRIMARYPART_OFFSET);
                APPEND_ESP_OBJ(part, mirage_esp_color, "Mirage Island")
                {
                    long child_count = 0;
                    rbx_child_t* children = rbx_instance_get_children(task, mystic_island_chests_folder, &child_count);
                    if (children)
                    {
                        for (long i = 0 ; i < child_count ; i++)
                        {
                            if (mirage_chest_index < 200)
                            {
                                vm_address_t object = children[i].child_address;
                                //if (rbx_basepart_get_cancollide(task, object) == 1)
                                //{
                                    //rbx_print_descendants(task, object, 0, 0);
                                    mirage_chests[mirage_chest_index++] = object;
                                //}
                            }
                        }
                    }
                }
                    //vector3_t sz = rbx_basepart_get_size(task, mirage_gear);
                APPEND_ESP_OBJ(mirage_gear, mirage_esp_color, "GEAR")
                    //printf("-- MIRAGE GEAR --, %f, %f, %f\n", sz.x, sz.y, sz.z);
                    //rbx_print_descendants(task, mirage_gear, 0, 0);
            }
            if (kitsune_island)
            {
                vm_address_t part = rbx_instance_find_first_child_of_class(task, kitsune_island, "Part");
                APPEND_ESP_OBJ(part, kitsune_esp_color, "Kitsune Island");
            }
            
            //rbx_print_descendants(task, workspace, 0, 3);
            long child_count = 0;
            rbx_child_t* children = rbx_instance_get_children(task, workspace, &child_count);
            if (children)
            {
                for (long i = 0 ; i < child_count ; i++)
                {
                    vm_address_t object = children[i].child_address;
                    long name_len = 0;
                    char* name = rbx_instance_get_name(task, object, &name_len);
                    if (name)
                    {
                        //printf("%s\n", name);
                        if (strstr(name, "Chest"))
                        {
                            if (chest_index < 200)
                            {
                                //if (rbx_basepart_get_cancollide(task, object) == 1)
                                //{
                                    chests[chest_index++] = object;
                                //}
                            }
                        }
                        if (strcmp(name, "EmberTemplate") == 0)
                        {
                            //rbx_print_descendants(task, object, 0, 0);
                            if (ember_index < 200)
                            {
                                vm_address_t _part = rbx_instance_find_first_child(task, object, "Part");
                                //rbx_print_descendants(task, _part, 0, 1);
                                if (_part)
                                {
                                    embers[ember_index++] = _part;
                                }
                            }
                        }
                        if (strstr(name, "Fruit"))
                        {
                            vm_address_t _fruit = rbx_instance_find_first_child(task, object, "Fruit");
                            if (_fruit)
                            {
                                vm_address_t handle = rbx_instance_find_first_child(task, object, "Handle");
                                APPEND_ESP_OBJ(handle, fruit_esp_color, name)
                                fruit_models[fruit_index++] = object;
                            }
                        }
                        if (strcmp(name, "Flower1") == 0)
                        {
                            APPEND_ESP_OBJ(object, blue_flower_esp_color, name);
                        }
                        if (strcmp(name, "Flower2") == 0)
                        {
                            APPEND_ESP_OBJ(object, red_flower_esp_color, name);
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
            }
            fruit_count = fruit_index;
            chest_count = chest_index;
            mirage_chest_count = mirage_chest_index;
            ember_count = ember_index;
            
            if (item_collection_mode == BLOX_FRUITS_ITEM_COLLECTION_MODE_CHESTS_ONLY)
            {
                memcpy(items, chests, sizeof(chests));
                item_count = chest_count;
                item_cancollide_check_enabled = true;
            }
            else if (item_collection_mode == BLOX_FRUITS_ITEM_COLLECTION_MODE_MIRAGE_CHESTS_ONLY)
            {
                memcpy(items, mirage_chests, sizeof(chests));
                item_count = mirage_chest_count;
                item_cancollide_check_enabled = true;
            }
            else if (item_collection_mode == BLOX_FRUITS_ITEM_COLLECTION_MODE_EMBERS_ONLY)
            {
                memcpy(items, embers, sizeof(embers));
                item_count = ember_count;
                item_cancollide_check_enabled = false;
            }
            else
            {
                item_cancollide_check_enabled = false;
            }
            
            int other_player_index = 0;
            COPY_BUF(other_player_hums)
            COPY_BUF(other_player_hrps)
            
            child_count = 0;
            children = rbx_instance_get_children(task, players_service, &child_count);
            if (children)
            {
                for (long i = 0 ; i < child_count ; i++)
                {
                    vm_address_t object = children[i].child_address;
                    if (object != local_player)
                    {
                        vm_address_t c = rbx_player_get_character(task, object);
                        //rbx_print_descendants(task, c, 0, 1);
                        vm_address_t h = rbx_instance_find_first_child(task, c, "HumanoidRootPart");
                        vm_address_t hum = rbx_instance_find_first_child(task, c, "Humanoid");
                        long name_len = 0;
                        char* name = rbx_instance_get_name(task, object, &name_len);
                        if (name && h && hum)
                        {
                            float sz = 30;
                            rbx_basepart_set_size(task, h, (vector3_t){.x = sz,. y = sz, .z = sz});
                            
                            vm_address_t df = rbx_instance_find_first_child(task, object, "Data");
                            //rbx_print_descendants(task, df, 0, 1);
                            vm_address_t race_sv = rbx_instance_find_first_child(task, df, "Race");
                            vm_address_t lvl_iv = rbx_instance_find_first_child(task, df, "Level");
                            long race_sl = 0;
                            char* race = rbx_stringvalue_get_value(task, race_sv, &race_sl);
                            //rbx_print_descendants(task, object, 0, 1);
                            long lvl = rbx_intvalue_get_value(task, lvl_iv);
                            esp_part_colors[esp_part_index] = custom_haki_esp_color;
                            esp_parts[esp_part_index] = h;
                            
                            __other_player_hrps[other_player_index] = h;
                            __other_player_hums[other_player_index] = hum;
                            other_player_index++;
                            
                            char str[MAX_ESP_TEXT_LENGTH];
                            bzero(str, MAX_ESP_TEXT_LENGTH);
                            sprintf(str, "%s [Lv. %ld] [Race: %s]", name, lvl, race);
                            
                            if (race)
                            {
                                vm_deallocate(mach_task_self_, (vm_address_t)race, race_sl);
                            }
                            
                            bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                            memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), str, strlen(str));
                            
                            esp_part_index++;
                            vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
            }
            
            other_player_count = other_player_index;
            memcpy(other_player_hrps, __other_player_hrps, sizeof(other_player_hrps));
            memcpy(other_player_hums, __other_player_hums, sizeof(other_player_hums));
            
            if (show_torches)
            {
                child_count = 0;
                children = rbx_instance_get_children(task, floating_turtle_questtorches_model, &child_count);
                if (children)
                {
                    for (long i = 0 ; i < child_count ; i++)
                    {
                        vm_address_t object = children[i].child_address;
                        long name_len = 0;
                        char* name = rbx_instance_get_name(task, object, &name_len);
                        if (name)
                        {
                            APPEND_ESP_OBJ(object, quest_torches_esp_color, name)
                            vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
            }
            //printf("F\n");
            esp_part_count = esp_part_index;
            sleep(4);
        }
    });
    
    
    
    
    
#pragma mark - Keybinds -
    
    
#pragma mark m
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(500);
            if (fly_keybind_enabled)
            {
                if (shared_memory->KEYS_DOWN['m'] == true)
                {
                    if (item_collect_enabled == true)
                    {
                        noclip_enabled = false;
                    }
                    fly_enabled = !fly_enabled;
                    if (fly_enabled == false)
                    {
                        item_collect_enabled = false;
                    }
                    rbx_smooth_fly(task, fly_enabled);
                    while (shared_memory->KEYS_DOWN['m'] == true)
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
    
#pragma mark n
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(500);
            if (boat_keybind_enabled)
            {
                if (shared_memory->KEYS_DOWN['n'] == true)
                {
                    if (my_humanoid_current_seat_part)
                    {
                        high_boat = !high_boat;
                        while (shared_memory->KEYS_DOWN['n'] == true)
                        {
                            usleep(500);
                        }
                    }
                    else
                    {
                        high_boat = false;
                    }
                }
            }
            else
            {
                sleep(1);
            }
        }
    });
    
#pragma mark b
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(500);
            if (bail_keybind_enabled)
            {
                if (shared_memory->KEYS_DOWN['b'] == true)
                {
                    if (my_hrp && locking_on_enemy)
                    {
                        bailing = !bailing;
                    }
                    else
                    {
                        bailing = false;
                    }
                    while (shared_memory->KEYS_DOWN['b'] == true)
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
    
    
#pragma mark p
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(500);
            if (priority_keybind_enabled)
            {
                if (shared_memory->KEYS_DOWN['p'] == true)
                {
                    low_prio = !low_prio;
                }
                if (low_prio)
                {
                    setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_DARWIN_BG);
                    //printf("LOW PRIO\n");
                }
                else
                {
                    setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_MAX);
                    //printf("HIGH PRIO\n");
                }
                
                while (shared_memory->KEYS_DOWN['p'] == true)
                {
                    usleep(500);
                }
            }
            else
            {
                sleep(1);
            }
        }
    });
    
#pragma mark l
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(500);
            if (lock_keybind_enabled)
            {
                if (shared_memory->KEYS_DOWN['l'] == true)
                {
                    if (closest_hrp_to_mouse && closest_hrp_to_mouse_cframe_address && (closest_hrp_to_mouse_dist < aimbot_max_dist))
                    {
                        if (rbx_instance_is_valid_child(task, closest_hrp_to_mouse))
                        {
                            locking_on_enemy = !locking_on_enemy;
                            hrp_to_lock_on = closest_hrp_to_mouse;
                            hum_to_lock_on = closest_hum_to_mouse;
                            hrp_to_lock_on_cframe_address = closest_hrp_to_mouse_cframe_address;
                        }
                        else
                        {
                            locking_on_enemy = false;
                        }
                    }
                    else
                    {
                        locking_on_enemy = false;
                    }
                    while (shared_memory->KEYS_DOWN['l'] == true)
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
    
    
    
    
    
#pragma mark - NoClip -
    
    

    
    
    
   BASIC_NOCLIP_ENGINE
    

    
    
    
    
#pragma mark - Raid Assist -
    
    
    /*
     Apparently, we have some sort of ownership over the NPC's.
     If we set the health to 0, they will just die?
     This only works if you AREN'T super laggy!!!
     */
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(1);
            if (raid_assist_enabled)
            {
                COPY_VAR(npc_hrp_count)
                COPY_BUF(npc_hums)
                for (long i = 0 ; i < __npc_hrp_count ; i++)
                {
                    vm_address_t humanoid = __npc_hums[i];
                    if (rbx_instance_is_valid_child(task, humanoid))
                    {
                        while (shared_memory->FUNCTION_QUEUE[2].should_execute == true) { usleep(10000); }
                        shared_memory->FUNCTION_QUEUE[2].address = humanoid_set_health_func;
                        shared_memory->FUNCTION_QUEUE[2].type = 7;
                        shared_memory->FUNCTION_QUEUE[2].async_type = FUNCTION_ASYNC_TYPE_MAIN;
                        *((vm_address_t*)(shared_memory->FUNCTION_QUEUE[2].arguments)) = humanoid;
                        *((float*)(shared_memory->FUNCTION_QUEUE[2].arguments + 8)) = 0.0f;
                        shared_memory->FUNCTION_QUEUE[2].should_execute = true;
                        while (shared_memory->FUNCTION_QUEUE[2].should_execute == true) { usleep(10000); }
                    }
                }
            }
        }
    });
    
    
    
    
    
#pragma mark - Item Collect -
    
    
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (item_collect_enabled)
            {
                float old_dist = 20000.0f; //Anything further than this is probably seperate from the "main world"
                vm_address_t __closest_item = 0;
                COPY_VAR(item_count)
                COPY_BUF(items)
                for (long i = 0 ; i < __item_count ; i++)
                {
                    vm_address_t part = __items[i];
                    if (rbx_instance_is_valid_child(task, part))
                    {
                        if (my_hrp)
                        {
                            if ((!item_cancollide_check_enabled) ||
                                (item_cancollide_check_enabled && rbx_basepart_get_cancollide(task, part) == 1))
                            {
                                //rbx_print_descendants(task, part, 0, 0);
                                rbx_cframe_t object_cframe = rbx_basepart_get_cframe(task, part);
                                rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
                                float dist = vector3_dist_dif(my_hrp_cframe.pos, object_cframe.pos);
                                if (dist < old_dist)
                                {
                                    old_dist = dist;
                                    __closest_item = part;
                                }
                            }
                        }
                    }
                }
                closest_item = __closest_item;
                rbx_print_descendants(task, closest_item, 0, 1);
                usleep(500000);
            }
            else
            {
                sleep(1);
            }
        }
    });
    
    
#pragma mark - High-Boat -
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (high_boat && my_humanoid_current_seat_part)
            {
                vm_write(task, my_humanoid_current_seat_part_cframe_address + 0x28, (vm_address_t)&boat_y, 4);
            }
            else
            {
                usleep(300000);
            }
            usleep(2);
        }
    });
    
    
    
#pragma mark - Bail -
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (bailing && my_hrp_cframe_address)
            {
                vm_write(task, my_hrp_cframe_address + 0x28, (vm_address_t)&bail_y, 4);
            }
            else
            {
                usleep(10000);
            }
            usleep(1);
        }
    });
    
    
   
    
#pragma mark - Aimbot / Lock -
    
    
    
    
#pragma mark CFrame writes
    
    
    static int cframe_write_mode = 0;
    static int cframe_write_usleep_time = 1000;
    static int lock_usleep_time = 10;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (cframe_write_mode == 1)
            {
                vm_address_t __chtmca = closest_hrp_to_mouse_cframe_address;
                vm_address_t __mhca = my_hrp_cframe_address;
                if (__mhca && __chtmca)
                {
                    vm_copy(task, __chtmca, sizeof(rbx_cframe_t), __mhca);
                }
            }
            usleep(cframe_write_usleep_time);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (locking_on_enemy && !bailing)
            {
                if (rbx_instance_is_valid_child(task, hrp_to_lock_on))
                {
                    vm_address_t __chtmca = hrp_to_lock_on_cframe_address;
                    vm_address_t __mhca = my_hrp_cframe_address;
                    if (__mhca && __chtmca)
                    {
                        vm_copy(task, __chtmca, sizeof(rbx_cframe_t), __mhca);
                    }
                }
            }
            else
            {
                usleep(10000);
            }
            usleep(lock_usleep_time);
        }
    });
    
    
#pragma mark Target Find
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (aimbot_enabled)
            {
                float __old_dist_from_my_hrp = aimbot_max_tp_dist;
                float __old_dist_from_mouse = aimbot_pixel_radius;
                vm_address_t __old_target_hrp = 0;
                vm_address_t __old_target_hum = 0;
                
                COPY_VAR(other_player_count)
                COPY_BUF(other_player_hrps)
                COPY_BUF(other_player_hums)
                
                for (long i = 0 ; i < __other_player_count ; i++)
                {
                    vm_address_t hrp = __other_player_hrps[i];
                    vm_address_t hum = __other_player_hums[i];
                    if (rbx_instance_is_valid_child(task, hrp) && rbx_instance_is_valid_child(task, hum))
                    {
                        char is_alive = false;
                        is_alive = (rbx_humanoid_get_health(task, hum) > 0);
                        if (is_alive)
                        {
                            rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
                            rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, hrp);
                            float dist_from_hrp = vector3_dist_dif(my_hrp_cframe.pos, hrp_cframe.pos);
                            if (dist_from_hrp < aimbot_max_dist)
                            {
                                rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                                float camera_fov = rbx_camera_get_field_of_view(task, camera);
                                float window_w = shared_memory->WINDOW_W;
                                float window_h = shared_memory->WINDOW_H;
                                custom_rbx_world_to_screen_info_t wtsi = rbx_world_to_screen_point(camera_cframe, camera_fov, hrp_cframe.pos, window_w, window_h, 1, 1);
                                if (wtsi.x > window_inset && wtsi.x < (window_w - window_inset))
                                {
                                    if (wtsi.y > window_inset && wtsi.y < (window_h - window_inset))
                                    {
                                        
                                        float mouse_x = shared_memory->MOUSE_X;
                                        float mouse_y = shared_memory->MOUSE_Y;
                                        
                                        float dist_from_mouse = sqrtf(pow(wtsi.x - mouse_x, 2) + pow(wtsi.y - mouse_y, 2));
                                        if (dist_from_mouse < __old_dist_from_mouse)
                                        {
                                            __old_dist_from_my_hrp = dist_from_hrp;
                                            __old_dist_from_mouse = dist_from_mouse;
                                            __old_target_hrp = hrp;
                                            __old_target_hum = hum;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                closest_hrp_to_mouse = __old_target_hrp;
                closest_hum_to_mouse = __old_target_hum;
                closest_hrp_to_mouse_dist = __old_dist_from_my_hrp;
                if (rbx_instance_is_valid_child(task, closest_hrp_to_mouse))
                {
                    closest_hrp_to_mouse_cframe_address = rbx_basepart_get_properties_address(task, closest_hrp_to_mouse) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                }
                else
                {
                    closest_hrp_to_mouse_cframe_address = 0;
                }
                
                if (rbx_humanoid_get_health(task, closest_hum_to_mouse) <= 0)
                {
                    closest_hrp_to_mouse = 0;
                    closest_hrp_to_mouse_cframe_address = 0;
                }
                if (rbx_humanoid_get_health(task, hum_to_lock_on) <= 0)
                {
                    locking_on_enemy = false;
                    hrp_to_lock_on = 0;
                    hrp_to_lock_on_cframe_address = 0;
                }
                if (bailing && !locking_on_enemy)
                {
                    bailing = false;
                }
                //rbx_print_descendants(task, __old_target_hrp, 0, 0);
                //printf("%f\n", __old_dist_from_center);
            }
            else
            {
                hrp_to_lock_on = 0;
                hrp_to_lock_on_cframe_address = 0;
                closest_hrp_to_mouse = 0;
                closest_hrp_to_mouse_cframe_address = 0;
            }
            usleep(200000);
        }
    });
    
#pragma mark Mouse Aimbot
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (aimbot_enabled)
            {
                if (closest_hrp_to_mouse)
                {
                    unsigned char* keys_down = shared_memory->KEYS_DOWN;
                    if ((keys_down['z'] ||
                        keys_down['x'] ||
                        keys_down['c'] ||
                        keys_down['v']) && !shared_memory->RIGHT_MOUSE_DOWN)
                    {
                        
                        //iut = 10;
                        //shared_memory->INPUT_USLEEP_TIME = iut;
                        
                        rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, closest_hrp_to_mouse);
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        float camera_fov = rbx_camera_get_field_of_view(task, camera);
                        float window_w = shared_memory->WINDOW_W;
                        float window_h = shared_memory->WINDOW_H;
                        float window_x = shared_memory->WINDOW_X;
                        float window_y = shared_memory->WINDOW_Y;
                        //printf("%f, %f, %f, %f\n", window_x, window_y, window_w, window_h);
                        custom_rbx_world_to_screen_info_t wtsi = rbx_world_to_screen_point(camera_cframe, camera_fov, hrp_cframe.pos, window_w, window_h, 1, 1);
                        if (wtsi.x > window_inset && wtsi.x < (window_w - window_inset))
                        {
                            if (wtsi.y > window_inset && wtsi.y < (window_h - window_inset))
                            {
                                mouse_aimbot_input.x = wtsi.x;
                                mouse_aimbot_input.y = wtsi.y;
                                mouse_aimbot_input.should_execute = true;
                                shared_memory->INPUT_QUEUE[0] = mouse_aimbot_input;
                                while (shared_memory->INPUT_QUEUE[0].should_execute == true) {}
                                //moveMouseOnWindow(window_x, window_y, window_w, window_h, NSMakePoint(wtsi.x, wtsi.y));
                            }
                        }
                        usleep(10);
                    }
                    else
                    {
                        iut = 1000;
                        shared_memory->INPUT_USLEEP_TIME = iut;
                        usleep(100);
                    }
                }
                else
                {
                    iut = 10000;
                   shared_memory->INPUT_USLEEP_TIME = iut;
                    usleep(1000);
                }
            }
            else
            {
                iut = 200000;
                shared_memory->INPUT_USLEEP_TIME = iut;
                sleep(1);
            }
        }
    });
    
    
#pragma mark TP Aimbot
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (tp_aimbot_enabled)
            {
                if(shared_memory->LEFT_MOUSE_DOWN == true)
                {
                    rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, closest_hrp_to_mouse);
                    rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
                    float dist = vector3_dist_dif(hrp_cframe.pos, my_hrp_cframe.pos);
                    
                    if (closest_hrp_to_mouse &&
                        my_hrp_cframe_address &&
                        closest_hrp_to_mouse_cframe_address &&
                        (dist < aimbot_max_tp_dist))
                    {
                        cframe_write_mode = 1;
                        cframe_write_usleep_time = 8;
                        usleep(8);
                    }
                    else
                    {
                        cframe_write_mode = 0;
                        cframe_write_usleep_time = 100;
                        usleep(100);
                    }
                }
                else
                {
                    cframe_write_mode = 0;
                    cframe_write_usleep_time = 500;
                    usleep(300);
                }
            }
            else
            {
                cframe_write_mode = 0;
                cframe_write_usleep_time = 10000;
                sleep(1);
            }
        }
    });
    
    
    
    
#pragma mark - ESP -
    
    
    BASIC_ESP_ENGINE({
        if (part == closest_hrp_to_mouse)
        {
            if (dist < aimbot_max_tp_dist)
            {
                esp_color = target_esp_color2;
            }
            else
            {
                esp_color = target_esp_color1;
            }
        }
        else
        {
            esp_color.a = 0.95;
        }
    }, 200, {
        
        bzero(sidebar_text, sizeof(sidebar_text));
        if (fly_enabled)
        {
            strcat(sidebar_text, "FLY ENABLED");
        }
        strcat(sidebar_text, "\n");
        if (locking_on_enemy)
        {
            strcat(sidebar_text, "LOCKING ON ENEMY");
        }
        strcat(sidebar_text, "\n");
        if (bailing)
        {
            strcat(sidebar_text, "BAILING");
        }
        strcat(sidebar_text, "\n");
        if (high_boat)
        {
            strcat(sidebar_text, "HIGH BOAT");
        }
        strcat(sidebar_text, "\n");
        if (low_prio)
        {
            strcat(sidebar_text, "LOW PRIO");
        }
        strcat(sidebar_text, "\n");
        if (mirage_island_exists)
        {
            //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 240, .w = shared_memory->WINDOW_W, .h = 40};
            ESP_Frame frame;
            frame.x = 0;
            frame.y = shared_memory->WINDOW_H - 240;
            frame.w = shared_memory->WINDOW_W;
            frame.h = 40;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            char text[MAX_ESP_TEXT_LENGTH];
            bzero(text, MAX_ESP_TEXT_LENGTH);
            sprintf(text, "%s", "MIRAGE ISLAND");
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = mirage_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
        if (kitsune_island_exists)
        {
            //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 280, .w = shared_memory->WINDOW_W, .h = 40};
            ESP_Frame frame;
            frame.x = 0;
            frame.y = shared_memory->WINDOW_H - 280;
            frame.w = shared_memory->WINDOW_W;
            frame.h = 40;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            char text[MAX_ESP_TEXT_LENGTH];
            bzero(text, MAX_ESP_TEXT_LENGTH);
            sprintf(text, "%s", "KITSUNE ISLAND");
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = kitsune_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
        if ((is_full_moon || is_waxing_gibbous))
        {
            //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 200, .w = shared_memory->WINDOW_W, .h = 40};
            ESP_Frame frame;
            frame.x = 0;
            frame.y = shared_memory->WINDOW_H - 200;
            frame.w = shared_memory->WINDOW_W;
            frame.h = 40;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            
            char text[MAX_ESP_TEXT_LENGTH];
            bzero(text, MAX_ESP_TEXT_LENGTH);
            char* _m = "FULL MOON";
            if (is_waxing_gibbous){_m = "WAXING GIBBOUS";}
            sprintf(text, "%s [%.1f]\n", _m, ((float)current_clocktime/RBX_LIGHTING_CLOCKTIME_HOUR_TICK_COUNT));
            
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = red_flower_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
        {
            //ESP_Frame frame = (ESP_Frame){.x = shared_memory -> WINDOW_W - 200, .y = (shared_memory->WINDOW_H - 300) * 0.5, .w = 200, .h = 300};
            ESP_Frame frame;
            frame.x = shared_memory -> WINDOW_W - 200;
            frame.y = (shared_memory->WINDOW_H - 300) * 0.5;
            frame.w = 200;
            frame.h = 300;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), sidebar_text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = red_flower_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
        
    })
    
    
    
    
    
    
#pragma mark - Fly -
    
    
    
    
    BASIC_FLY_ENGINE({
        COPY_VAR(closest_item)
        if (is_levi_bot_program && levi_bot_should_follow_seat && levi_bot_target_seat &&
            (rbx_humanoid_get_seatpart(task, my_humanoid) != levi_bot_target_seat))
        {
            vel = vector3_direction_to_from(rbx_basepart_get_cframe(task, levi_bot_target_seat).pos, rbx_basepart_get_cframe(task, __my_hrp).pos);
            vel = vector3_mul_num(vel, fly_speed);
            rbx_basepart_set_velocity(task, __my_hrp, vel);
            rbx_basepart_set_gravity(task, __my_hrp, 0.0f);
        }
        else if (auto_raid_enabled && next_raid_island_part)
        {
            vel = vector3_direction_to_from(vector3_add(rbx_basepart_get_cframe(task, next_raid_island_part).pos, (vector3_t){.x = 0, .y = 70 , .z = 0}), rbx_basepart_get_cframe(task, __my_hrp).pos);
            vel = vector3_mul_num(vel, fly_speed);
            rbx_basepart_set_velocity(task, __my_hrp, vel);
            rbx_basepart_set_gravity(task, __my_hrp, 0.0f);
        }
        else if (item_collect_enabled && __closest_item)
        {
            vel = vector3_direction_to_from(rbx_basepart_get_cframe(task, __closest_item).pos, rbx_basepart_get_cframe(task, __my_hrp).pos);
            vel = vector3_mul_num(vel, fly_speed);
            rbx_basepart_set_velocity(task, __my_hrp, vel);
            rbx_basepart_set_gravity(task, __my_hrp, 0.0f);
        }
        else if (fly_enabled)
        {
            BASIC_FLY_CONTROL_CODE
        }
    }, 300)
     
    
    
#pragma mark - Boatfly -
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            vm_address_t mbs = my_boat_seat;
            if (boatfly_enabled && mbs && !is_levi_bot_program && rbx_humanoid_get_seatpart(task, my_humanoid) == mbs)
            {
                vector3_t add = {};
                
                rbx_cframe_t ccf = rbx_camera_get_cframe(task, camera);
                rbx_cframe_t cf = rbx_basepart_get_cframe(task, mbs);
                cf.pos.y = 30;
                vector3_t lv = rbx_cframe_get_look_vector(ccf);
                vector3_t rv = rbx_cframe_get_right_vector(ccf);
                
                bzero(&add, sizeof(vector3_t));
                
                unsigned char* keys_down = shared_memory->KEYS_DOWN;
                
                if (keys_down['w']) { add = vector3_add(vector3_mul_num(lv, 1), add); }
                if (keys_down['a']) { add = vector3_add(vector3_mul_num(rv, -1), add); }
                if (keys_down['s']) { add = vector3_add(vector3_mul_num(lv, -1), add); }
                if (keys_down['d']) { add = vector3_add(vector3_mul_num(rv, 1), add); }
                
                float magnitude = vector3_magnitude(add);
                if (!isnan(magnitude) && magnitude > 0.0f)
                {
                    add = vector3_div_num(add,  magnitude);
                }
                add = vector3_mul_num(add, 1.2);
                cf.pos = vector3_add(cf.pos, add);
                rbx_basepart_set_cframe(task, mbs, cf);
            }
            else
            {
                sleep(1);
            }
            usleep(350);
        }
    });
    
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    
    
    
    
    printf("List of stdin commands:\n");
    printf("\tfly\n");
    printf("\tflyspeed [(float)]\n");
    printf("\tnofly\n");
    printf("\tesp\n");
    printf("\tnoesp\n");
    printf("\tclip\n");
    printf("\tnoclip\n");
    printf("\taimbot\n");
    printf("\tnoaimbot\n");
    printf("\tshowfruits\n");
    printf("\tshowtorches\n");
    printf("\thidetorches\n");
    printf("\traidassist\n");
    printf("\tnoraidassist\n");
    printf("\tembers\n");
    printf("\tchests\n");
    printf("\tmiragechests\n");
    printf("\tlockut [(int)] //Lock usleep time\n");
    printf("\tlp //Low Priority\n");
    printf("\thp //High Priority\n");
    printf("\tquit\n");
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            char str[100];
            bzero(str, 100);
            fgets(str, 100, stdin);
            
            if (strcmp(str, "sf\xa") == 0)
            {
                printf("SHOWING MOST RECENT FRUIT...\n");
                vm_address_t s = rbx_instance_find_first_child(task, workspace, "Fruit ");
                if (s)
                {
                    rbx_camera_set_camera_subject(task, camera, s);
                    sleep(3);
                }
                rbx_camera_set_camera_subject(task, camera, my_humanoid);
            }
            if (strcmp(str, "fly\xa") == 0)
            {
                fly_enabled = true;
                rbx_smooth_fly(task, fly_enabled);
                printf("ENABLED FLY\n");
            }
            if (strncmp(str, "flyspeed ", strlen("flyspeed ")) == 0)
            {
                sscanf(str, "flyspeed %f", &fly_speed);
                printf("SET FLY SPEED TO %f\n", fly_speed);
            }
            if (strcmp(str, "nofly\xa") == 0)
            {
                item_collect_enabled = false;
                fly_enabled = false;
                rbx_smooth_fly(task, fly_enabled);
                printf("DISABLED FLY\n");
            }
            if (strcmp(str, "aimbot\xa") == 0)
            {
                aimbot_enabled = true;
                printf("ENABLED AIMBOT\n");
            }
            if (strcmp(str, "noaimbot\xa") == 0)
            {
                aimbot_enabled = false;
                printf("DISABLED AIMBOT\n");
            }
            if (strcmp(str, "showtorches\xa") == 0)
            {
                show_torches = true;
                printf("SHOWING TORCHES...\n");
            }
            if (strcmp(str, "hidetorches\xa") == 0)
            {
                show_torches = false;
                printf("HIDING TORCHES...\n");
            }
            if (strcmp(str, "raidassist\xa") == 0)
            {
                raid_assist_enabled = true;
                printf("ENABLED RAID ASSIST\n");
            }
            if (strcmp(str, "noraidassist\xa") == 0)
            {
                raid_assist_enabled = false;
                printf("DISABLED RAID ASSIST\n");
            }
            if (strcmp(str, "autoraid\xa") == 0)
            {
                noclip_enabled = true;
                auto_raid_enabled = true;
                raid_assist_enabled = true;
                fly_enabled = true;
                rbx_smooth_fly(task, fly_enabled);
                printf("ENABLED AUTO RAID\n");
            }
            if (strcmp(str, "noautoraid\xa") == 0)
            {
                noclip_enabled = false;
                auto_raid_enabled = false;
                raid_assist_enabled = false;
                fly_enabled = false;
                rbx_smooth_fly(task, fly_enabled);
                printf("DISABLED AUTO RAID\n");
            }
            if (strcmp(str, "embers\xa") == 0)
            {
                rbx_smooth_fly(task, 1);
                noclip_enabled = true;
                fly_enabled = true;
                item_collect_enabled = true;
                item_collection_mode = BLOX_FRUITS_ITEM_COLLECTION_MODE_EMBERS_ONLY;
                printf("COLLECTING ONLY EMBERS\n");
            }
            if (strcmp(str, "chests\xa") == 0)
            {
                rbx_smooth_fly(task, 1);
                noclip_enabled = true;
                fly_enabled = true;
                item_collect_enabled = true;
                item_collection_mode = BLOX_FRUITS_ITEM_COLLECTION_MODE_CHESTS_ONLY;
                printf("COLLECTING ONLY CHESTS\n");
            }
            if (strcmp(str, "miragechests\xa") == 0)
            {
                rbx_smooth_fly(task, 1);
                noclip_enabled = true;
                fly_enabled = true;
                item_collect_enabled = true;
                item_collection_mode = BLOX_FRUITS_ITEM_COLLECTION_MODE_MIRAGE_CHESTS_ONLY;
                printf("COLLECTING ONLY MIRAGE ISLAND CHESTS\n");
            }
            if (strcmp(str, "esp\xa") == 0)
            {
                esp_enabled = true;
                printf("ENABLED ESP\n");
            }
            if (strcmp(str, "noesp\xa") == 0)
            {
                esp_enabled = false;
                printf("DISABLED ESP\n");
            }
            if (strcmp(str, "clip\xa") == 0)
            {
                noclip_enabled = false;
                printf("DISABLED NOCLIP\n");
            }
            if (strcmp(str, "noclip\xa") == 0)
            {
                noclip_enabled = true;
                printf("ENABLED NOCLIP\n");
            }
            if (strncmp(str, "lockut ", strlen("lockut ")) == 0)
            {
                sscanf(str, "lockut %d", &lock_usleep_time);
                printf("SET LOCK USLEEP TIME TO %d\n", lock_usleep_time);
            }
            if (strcmp(str, "hp\xa") == 0)
            {
                setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_MAX);
                printf("HIGH PRIO\n");
            }
            if (strcmp(str, "lp\xa") == 0)
            {
                setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_DARWIN_BG);
                printf("LOW PRIO\n");
            }
            if (strcmp(str, "levibot\xa") == 0)
            {
                noclip_enabled = true;
                fly_enabled = true;
                rbx_smooth_fly(task, fly_enabled);
                is_levi_bot_program = true;
                printf("BECOMING LEVI BOT PROGRAM...\n");
            }
            if (strncmp(str, "leviseat ", strlen("leviseat ")) == 0)
            {
                sscanf(str, "leviseat %d", &levi_bot_cannon_seat_index);
                printf("SET LEVI BOT SEAT INDEX TO %d\n", levi_bot_cannon_seat_index);
            }
            if (strncmp(str, "ctrlr ", strlen("ctrlr ")) == 0)
            {
                sscanf(str, "ctrlr %s", controller_player_name);
                printf("SET CONTROLLER PLAYER TO %s\n", controller_player_name);
            }
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
        
        
    });

}
