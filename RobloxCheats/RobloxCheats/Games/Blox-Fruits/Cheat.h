
#define BLOX_FRUITS_ITEM_COLLECTION_MODE_NONE 0
#define BLOX_FRUITS_ITEM_COLLECTION_MODE_CHESTS_ONLY 1


void blox_fruits_expand_hitbox(task_t task, vm_address_t task_base_address, vm_address_t character, vector3_t hb, char full)
{
    if (full)
    {
        FOR_EACH_DESCENDANT(task, character, 10000, 3, i, {
            
            if (rbx_instance_is_a_basepart(task, obj, task_base_address))
            {
                rbx_basepart_set_size(task, obj, hb);
            }
        })
    }
    else
    {
        rbx_basepart_set_size(task, rbx_instance_find_first_child_by_pointee(task, character, task_base_address, RBX_OBJECT_MeshPart_POINTEE_OFFSET), hb);
    }
}


void blox_fruits_cheat(task_t task)
{
    printf("\n- BLOX FRUITS -\n");
    pid_t pid = -1;
    pid_for_task(task, &pid);
    
    EXIT_IF_TASK_TERMINATED
    
    LIBESP_DYLIB_SETUP
    
#pragma mark - Globals -
    
    static ESP_Color kitsune_esp_color = {.r = 0.2, .g = 0.8, .b = 1, .a = 1};
    static ESP_Color mirage_esp_color = {.r = 1, .g = 0, .b = 0.8, .a = 1};
    static ESP_Color fruit_esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    static ESP_Color red_flower_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color blue_flower_esp_color = {.r = 0, .g = 0, .b = 1, .a = 1};
    static ESP_Color quest_torches_esp_color = {.r = 1, .g = 0.3, .b = 0, .a = 1};
    static ESP_Color yellow_esp_color = {.r = 1, .g = 1, .b = 0, .a = 1};
    static ESP_Color custom_haki_esp_color = {.r = 1, .g = 1, .b = 1, .a = 1};
    static ESP_Color target_esp_color1 = {.r = 0, .g = 0.75, .b = 0.75, .a = 1};
    static ESP_Color target_esp_color2 = {.r = 1, .g = 0, .b = 0.3, .a = 1};
    
    static ESP_Color chalice_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            ESP_Color c = chalice_esp_color;
            if (c.r >= 1 && c.g <= 1)
            {
                c.b -= 0.1;
                c.g += 0.1;
            }
            if (c.g >= 1 && c.b <= 1)
            {
                c.r -= 0.1;
                c.b += 0.1;
            }
            if (c.b >= 1 && c.r <= 1)
            {
                c.g -= 0.1;
                c.r += 0.1;
            }
            chalice_esp_color = c;
            usleep(100000);
        }
    });
    
    static float bail_y = 5000.0f;
    
    ESP_GLOBAL_VARIABLES
    
    static char sidebar_text[MAX_ESP_TEXT_LENGTH];
    
    static useconds_t esput = 1500;
    
    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = esput;
    strcpy(shared_memory->ESP_TEXT_FONT_NAME, "Menlo");
    
    shared_memory->INPUT_COUNT = 1;
    shared_memory->INPUT_USLEEP_TIME = 1000;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    //rbx_print_descendants(task, robloxpromptgui, 0, 10);
    //vm_address_t replicated_storage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    /*vm_address_t coregui = rbx_instance_find_first_child(task, game, "CoreGui");
     vm_address_t topbarapp = rbx_instance_find_first_child(task, coregui, "TopBarApp");
     vm_address_t gamepadmenu = rbx_instance_find_first_child(task, topbarapp, "GamepadMenu");
     vm_address_t menu = rbx_instance_find_first_child(task, gamepadmenu, "Menu");
     vm_address_t leaderboard = rbx_instance_find_first_child(task, menu, "Leaderboard");
     rbx_print_descendants(task, leaderboard, 0, 3);*/
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t chestmodels_folder= rbx_instance_find_first_child(task, workspace, "ChestModels");
    vm_address_t rocks_folder = rbx_instance_find_first_child(task, workspace, "Rocks");
    //rbx_print_descendants(task, rbx_model_get_primarypart(task, 0x458db6f80), 0, 3);
    vm_address_t map_model = rbx_instance_find_first_child(task, workspace, "Map");
    vm_address_t raidmap_model = rbx_instance_find_first_child(task, map_model, "RaidMap");
    //rbx_print_descendants(task, workspace, 0, 4);
    //sleep(100000);
    //rbx_print_descendants(task, raidmap_model, 0, 2);
    vm_address_t jungle_model = rbx_instance_find_first_child(task, map_model, "Jungle");
    vm_address_t jungle_quest_plates_model = rbx_instance_find_first_child(task, jungle_model, "QuestPlates");
    static vm_address_t jungle_buttons[5];
    static int jungle_button_count = 0;
    FOR_EACH_CHILD(jungle_quest_plates_model, i, {
        vm_address_t b = rbx_instance_find_first_child(task, child, "Button");
        if (b)
        {
            jungle_buttons[jungle_button_count++] = b;
        }
    })
    //rbx_print_descendants(task, jungle_quest_plates_model, 0, 2);
    //vector3_t s = rbx_basepart_get_size(task, waterbase_plane);
    //printf("%f, %f, %f\n", s.x, s.y, s.z);
    vm_address_t _worldorigin_folder = rbx_instance_find_first_child(task, workspace, "_WorldOrigin");
    vm_address_t locations_folder = rbx_instance_find_first_child(task, _worldorigin_folder, "Locations");
    vm_address_t npcs_folder = rbx_instance_find_first_child(task, workspace, "NPCs");
    vm_address_t enemies_folder = rbx_instance_find_first_child(task, workspace, "Enemies");
    //rbx_print_descendants(task, workspace, 0, 5);
    vm_address_t seaevents_folder = rbx_instance_find_first_child(task, workspace, "SeaEvents");
    vm_address_t seabeasts_folder = rbx_instance_find_first_child(task, workspace, "SeaBeasts");
    
    vm_address_t boats_folder = rbx_instance_find_first_child(task, workspace, "Boats");
    vm_address_t lighting_service = rbx_instance_find_first_child_of_class(task, game, "Lighting");
    vm_address_t sky = rbx_instance_find_first_child_of_class(task, lighting_service, "Sky");
    static char* waxing_halfmoon_assetid = "9709143733"; //http://www.roblox.com/asset/?id=9709143733
    static char* fullmoon_assetid = "9709149431"; //http://www.roblox.com/asset/?id=9709149431
    static char* waxing_gibbous_assetid = "9709149052"; //http://www.roblox.com/asset/?id=9709149052
    static int current_clocktime = 0;
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t player_gui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
    vm_address_t main_screengui = rbx_instance_find_first_child(task, player_gui, "Main");
    vm_address_t main_minimal_screengui = rbx_instance_find_first_child(task, player_gui, "Main (minimal)");
    vm_address_t chooseteamframe = rbx_instance_find_first_child(task, main_minimal_screengui, "ChooseTeam");
    vm_address_t fastmodebutton = rbx_instance_find_first_child(task, chooseteamframe, "FastModeButton");
    //rbx_print_descendants(task, main_minimal_screengui, 0, 2);
    
    /*FOR_EACH_DESCENDANT(workspace, 100000, 100, {
     if (rbx_instance_is_a(task, obj, "ProximityPrompt"))
     {
     vm_address_t obj_p = rbx_instance_get_parent(task, obj);
     rbx_print_descendants(task, rbx_instance_get_parent(task, obj_p), 0, 0);
     }
     })*/
    
    rbx_print_descendants(task, local_player, 0, 0);
    //rbx_print_descendants(task, workspace, 0, 1);
    static vm_address_t legendary_sword_dealer_part = 0;
    static vm_address_t advanced_fruit_dealer_part = 0;
    static vm_address_t minktrial_endpoint = 0;
    static vm_address_t floating_turtle_island = 0;
    static vm_address_t floating_turtle_questtorches_model = 0;
    static vm_address_t mfinv = 0;
    static vm_address_t dfinv = 0;
    
    static vm_address_t rift_dimension_stagepart = 0;
    static vector3_t rift_dimension_return_pos;
    static char returning_from_rift = false;
    
    static vm_address_t my_character = 0;
    static vm_address_t my_character_parts[200];
    static long my_character_part_count = 0;
    static vm_address_t my_humanoid = 0;
    static vm_address_t my_humanoid_current_seat_part = 0;
    static vm_address_t my_humanoid_current_seat_part_cframe_address = 0;
    static vm_address_t my_hrp = 0;
    static vector3_t og_hitbox_size = {.x = 20, .y= 20, .z = 20};
    static vector3_t new_hitbox_size = {.x = 70, .y= 70, .z = 70};
    static vector3_t old_plr_hitbox_size = {.x = 4, .y= 4, .z = 4};
    static vector3_t new_plr_hitbox_size = {.x = 70, .y= 70, .z = 70};
    static vector3_t current_plr_hitbox_size = {.x = 7, .y= 7, .z = 7};
    static vector3_t new_sb_hitbox_size = {.x = 170, .y= 170, .z = 170};
    static bool big_hitbox_enabled = true;
    static vm_address_t my_head = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    static vm_address_t my_boat, my_boat_seat;
    
    static vm_address_t energy_icv = 0;
    
    static vm_address_t other_player_hrps[200];
    static vm_address_t other_player_hums[200];
    static char other_player_names[200 * MAX_ESP_TEXT_LENGTH];
    static int other_player_count = 0;
    
    static vm_address_t hrps_with_chalice[200];
    static int chalice_count = 0;
    
    
    static vm_address_t enemies[200];
    static vm_address_t enemy_hums[200];
    static vm_address_t enemy_hrps[200];
    static vm_address_t enemy_first_meshparts[200];
    static int enemy_count = 0;
    
    static vm_address_t piranhas[200];
    static vm_address_t piranha_hrps[200];
    static int piranha_count;
    
    static vm_address_t elite_enemy_hrp = 0;
    
    static vm_address_t berries[200];
    static int berry_count = 0;
    
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
    
    static vm_address_t chests[200];
    static int chest_count = 0;
    
    static int item_collection_mode = BLOX_FRUITS_ITEM_COLLECTION_MODE_NONE;
    static vm_address_t items[200];
    static int item_count = 0;
    static vm_address_t closest_item = 0;
    
    static vm_address_t mystic_island = 0;
    static vm_address_t mirage_gear = 0;
    
    static float fly_speed = 275.0f;
    static float aimbot_pixel_radius = 150;
    static float aimbot_max_dist = 250;
    static float aimbot_max_tp_dist = 100;
    static float boat_y = 2000;
    //static vector3_t new_npc_hitbox_size = {.x = 30, .y = 30, .z = 30};
    
    static char noclip_enabled = false;
    static char fly_enabled = false;
    static char boatfly_enabled = true;
    static char raid_assist_enabled = false;
    
    static char aimbot_enabled = true;
    static char tp_aimbot_enabled = true;
    
    static char high_boat = false;
    
    static char item_collect_enabled = false;
    static char show_torches = false;
    
    static char lock_keybind_enabled = true;
    static char fly_keybind_enabled = true;
    static char boat_keybind_enabled = true;
    static char bail_keybind_enabled = true;
    static char priority_keybind_enabled = true;
    
    static char circle_boat_enabled = false;
    static vector3_t circle_boat_pos = {};
    static float circle_boat_angle = 0;
    static float circle_boat_radius = 5000;
    
    static pid_t active_app_pid = -1;
    static char low_prio = false;
    
    static char loop_goto_plr_enabled = false;
    static rbx_cframe_t goto_plr_cframe = {};
    
    static char dungeon_mode_enabled = false;
    static char boathacks_enabled = true;

    static vm_address_t dungeon_boss = 0;
    static vm_address_t dungeon_boss_hum = 0;
    static vm_address_t dungeon_boss_hrp = 0;
    
    static vm_address_t frozenheart_model = 0;
    static vm_address_t frozenheart_model_pp = 0;
    
    static char dragonstorm_equipped = false;
    
    static char is_waxing_halfmoon = false;
    static char is_waxing_gibbous = false;
    static char is_full_moon = false;
    static char mirage_island_exists = false;
    static char kitsune_island_exists = false;
    static char frozen_dimension_exists = false;
    static char prehistoric_island_exists = false;
    static char chalice_or_fist_exists = false;
    
    //For islands closest to us.
    static rbx_cframe_t frozen_dimension_cframe = {};
    static rbx_cframe_t mirage_island_cframe = {};
    static rbx_cframe_t prehistoric_island_cframe = {};
    
    static const float window_inset = 100;
    
    static char lock_onto_npc = false;
    static vm_address_t npc_hrp_to_lock_on_to = 0;
    static char lock_npc_name[100] = "";
    static vm_address_t lock_npc_hum = 0;
    static float npc_lock_y_offset = 0;
    
    static char circleboat_stop_for_frozen_dimension = true;
    static char circleboat_stop_for_mirage_island = false;
    static char circleboat_stop_for_prehistoric_island = false;
    
    
    long placeid = rbx_get_placeid(task);
    if (placeid == 73902483975735)
    {
        printf("ENABLED DUNGEON MODE\n");
        dungeon_mode_enabled = true;
        aimbot_enabled = false;
        esp_enabled = false;
        npc_lock_y_offset = 0;
        strcpy(lock_npc_name, "[Dungeon Boss]");
        lock_onto_npc = true;
        POST_NOTIF("DUNGEON MODE ENABLED", "If bosses disappear, rejoin.")
    }
    else
    {
        POST_NOTIF("Blox Fruits", "You may receive other notifications.")
    }
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        MEMORY_SEARCH_HEADER(VM_MEMORY_IOACCELERATOR)
        for (long i = 16 ; i < (size - 8 - 16) ; i += 8)
        {
            MEMORY_SEARCH_LOCAL_VARIABLE_FILTER_HEADER(var_type == 3)
            double read_double = *(double*)(read_data + i - 0x10);
            if (read_double == 3.0)
            {
                char nm[100]; bzero(nm, sizeof(nm)); int nm_l = 0;
                rbx_local_variable_get_string_value(task, var_name_ptr, nm, &nm_l);
                if (strcmp(nm, "OverheatLimit") == 0)
                {
                    double ___f = 999999999;
                    vm_write(task, var_ptr, (vm_address_t)&___f, 8); //0x45b3564a8 OverheatLimit (type 3) = 3.000000
                    double ___f0 = 0.015;
                    vm_write(task, var_ptr + (RBX_LOCAL_VARIABLE_SIZE * 3), (vm_address_t)&___f0, 8); //0x45b356508 Cooldown (type 3) = 0.080000
                    
                    //for (int x = -20 ; x < 20 ; x++)
                    //{  rbx_print_local_variable(task, var_ptr + (x * RBX_LOCAL_VARIABLE_SIZE), 0, 0); }
                }
            }
            MEMORY_SEARCH_LOCAL_VARIABLE_FILTER_FOOTER
        }
        MEMORY_SEARCH_FOOTER
    });
    
    
    ANTI_AFK
    
#pragma mark - Object Loops -
    
    
    
    
#pragma mark Instances [1]
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            rbx_numbervalue_set_value(task, mfinv, -100000.0f);
            rbx_numbervalue_set_value(task, dfinv, -100000.0f);
            usleep(100000);
        }
    });
    
    
#pragma mark Instances [2]
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            //rbx_print_descendants(task, seabeasts_folder, 0, 3);
            legendary_sword_dealer_part = rbx_instance_find_first_child(task, rbx_instance_find_first_child_partial(task, npcs_folder, "Legendary Sword Dealer"), "Head");
            advanced_fruit_dealer_part = rbx_instance_find_first_child(task, rbx_instance_find_first_child_partial(task, npcs_folder, "Advanced Fruit Dealer"), "Head");
            my_humanoid_current_seat_part = rbx_humanoid_get_seatpart(task, my_humanoid);
            //rbx_print_descendants(task,legendary_sword_dealer_part, 0, 0);
            if (rbx_instance_is_valid_child(task, my_humanoid_current_seat_part))
            {
                my_humanoid_current_seat_part_cframe_address = rbx_basepart_get_properties_address(task, my_humanoid_current_seat_part) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            }
            else
            {
                high_boat = false;
                my_humanoid_current_seat_part_cframe_address = 0;
            }
            rift_dimension_stagepart = rbx_instance_find_first_child(task, locations_folder, "Dimensional Rift");
            {
                vector3_t p = rbx_basepart_get_cframe(task, rift_dimension_stagepart).pos;
                p.y += 200;
                rift_dimension_return_pos = p;
            }
            
            if (lock_onto_npc)
            {
                if (my_hrp && (rbx_humanoid_get_health(task, my_humanoid) > 0))
                {
                    vm_address_t __npc_hrp_to_lock_on_to = 0;
                    vm_address_t __lock_npc_hum = 0;
                    
                    FOR_EACH_CHILD(enemies_folder, i, {
                        //rbx_print_descendants(task, child, 0, 0);
                        vm_address_t hum = rbx_instance_find_first_child_by_pointee(task, child, task_base_address, RBX_OBJECT_Humanoid_POINTEE_OFFSET);
                        if ((hum && (rbx_humanoid_get_health(task, hum) > 0)) ||
                            (child == dungeon_boss && (rbx_instance_get_child_count(task, players_service) > 1)))
                        {
                            vm_address_t hrp = rbx_instance_find_first_child(task, child, "HumanoidRootPart");
                            rbx_cframe_t cf = rbx_basepart_get_cframe(task, hrp);
                            float max_dist = 15000;
                            rbx_cframe_t my_hrp_cf = vm_read_rbx_cframe_t_value(task, my_hrp_cframe_address);
                            if (vector3_dist_dif(cf.pos, my_hrp_cf.pos) < max_dist)
                            {
                                long dn_l = 0;
                                char* dn = rbx_humanoid_get_displayname(task, hum, &dn_l);
                                //printf("%s\n", dn);
                                if (dn)
                                {
                                    if (strstr(dn, lock_npc_name) && !strstr(dn, "'s Shadow"))
                                    {
                                        if (dungeon_mode_enabled && strstr(dn, "Gas Knight"))
                                        {
                                            vm_address_t it_bv = rbx_instance_find_first_child(task, child, "IsTransformed");
                                            if (rbx_boolvalue_get_value(task, it_bv) == true)
                                            {
                                                __npc_hrp_to_lock_on_to = hrp;
                                                __lock_npc_hum = hum;
                                                i = child_count;
                                            }
                                        }
                                        else
                                        {
                                            __npc_hrp_to_lock_on_to = hrp;
                                            __lock_npc_hum = hum;
                                            i = child_count;
                                        }
                                    }
                                    
                                    vm_deallocate(mach_task_self_, (vm_address_t)dn, dn_l);
                                }
                            }
                        }
                    })
                    
                    lock_npc_hum = __lock_npc_hum;
                    npc_hrp_to_lock_on_to = __npc_hrp_to_lock_on_to;
                }
                else
                {
                    lock_npc_hum = 0;
                    npc_hrp_to_lock_on_to = 0;
                }
            }
            //rbx_cframe_t cf = rbx_basepart_get_cframe(task, my_hrp);
            //printf("%f, %f, %f\n", cf.pos.x, cf.pos.y, cf.pos.z);
            sleep(1);
        }
    });
    
    
#pragma mark Instances [3]
    
    BASIC_NOCLIP_PART_GETTER
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            my_character = rbx_player_get_character(task, local_player);
            //rbx_print_descendants(task, my_character, 0, 2);
            //rbx_print_descendants(task, locations_folder, 0, 1);
            //energy_icv = rbx_instance_find_first_child(task, my_character, "Energy");
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            my_head = rbx_instance_find_first_child(task, my_character, "Head");
            my_humanoid = rbx_instance_find_first_child_by_pointee(task, my_character, task_base_address, RBX_OBJECT_Humanoid_POINTEE_OFFSET);
            
            //rbx_humanoid_set_health(task, my_humanoid, 1);
            //rbx_camera_set_camera_subject(task, camera, my_head);
            if (my_hrp)
            {
                my_hrp_cframe_address = rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
               // printf("%p\n", my_hrp_cframe_address);
            }
            else
            {
                my_hrp_cframe_address = 0;
            }
            
            char __is_full_moon = false;
            char __is_waxing_gibbous = false;
            char __is_waxing_halfmoon = false;
            
            long mtid_len = 0;
            //rbx_print_descendants(task,map_model, 0, 3);
            char* mtid = rbx_sky_get_moontextureid(task, sky, &mtid_len);
            if (mtid)
            {
                //printf("%s\n", mtid);
                int ctt = rbx_lighting_get_clocktime_tick(task, lighting_service);
                float hour = ((float)ctt)/((float)RBX_LIGHTING_CLOCKTIME_HOUR_TICK_COUNT);
                current_clocktime = ctt;
                
                if (strstr(mtid, fullmoon_assetid) || kitsune_island_exists)
                {
                    //The game changes the next night's moon after noon, and 'the full moon ends' at 5 am that night.
                    if (hour > 12 || hour < 5)
                    {
                        __is_full_moon = true;
                    }
                }
                else if (strstr(mtid, waxing_gibbous_assetid))
                {
                    __is_waxing_gibbous = true;
                }
                else if (strstr(mtid, waxing_halfmoon_assetid))
                {
                    __is_waxing_halfmoon = true;
                }
                vm_deallocate(mach_task_self_, (vm_address_t)mtid, mtid_len);
            }
            
            is_full_moon = __is_full_moon;
            is_waxing_gibbous = __is_waxing_gibbous;
            is_waxing_halfmoon = __is_waxing_halfmoon;
            
            //rbx_print_descendants(task, my_character, 0, 1);
            //rbx_print_descendants(task, rbx_instance_find_last_child_of_class_and_name(task, my_character, "Accessory", "Mammoth"), 0, 1);
            
            //rift_dimension_stagepart = rbx_instance_find_first_child(task, map_model, "StagePart");
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
            //rbx_print_descendants(task, 0x14d8a0c30, 0, 2);
            //rbx_camera_set_camera_subject(task, camera, 0x17cb6ab40);
            /*
             _WorldOrigin : Folder
             Locations : Model
             */
            {
                vm_address_t __mg = 0;
                FOR_EACH_CHILD(mystic_island, i, {
                    if (vm_read_uint64_t_value(task, child) == task_base_address + RBX_OBJECT_MeshPart_POINTEE_OFFSET)
                    //if (rbx_instance_is_a(task, child, "MeshPart"))
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
                })
                mirage_gear = __mg;
            }
            
            vm_address_t lighting_layers = rbx_instance_find_first_child(task, lighting_service, "LightingLayers");
            vm_address_t miragefog_atm = rbx_instance_find_first_child(task, lighting_layers, "MirageFog");
            vm_address_t darkfog_atm = rbx_instance_find_first_child(task, lighting_layers, "DarkFog");
            
            mfinv = rbx_instance_find_first_child_by_pointee(task, miragefog_atm, task_base_address, RBX_OBJECT_NumberValue_POINTEE_OFFSET); //Intensity
            dfinv = rbx_instance_find_first_child_by_pointee(task, darkfog_atm, task_base_address, RBX_OBJECT_NumberValue_POINTEE_OFFSET); //Intensity
            
            vm_address_t old_my_boat_seat = my_boat_seat;
            {
                vm_address_t __my_boat = 0;
                vm_address_t __my_boat_seat = 0;
                FOR_EACH_CHILD(boats_folder, i, {
                    vm_address_t boat = child;
                    vm_address_t boat_owner_ov = rbx_instance_find_first_child(task, boat, "Owner");
                    vm_address_t owner = rbx_objectvalue_get_value(task, boat_owner_ov);
                    if (owner == local_player)
                    {
                        __my_boat = boat;
                        __my_boat_seat = rbx_model_get_primarypart(task, __my_boat);
                    }
                })
                my_boat_seat = __my_boat_seat;
                my_boat = __my_boat;
            }
            if (old_my_boat_seat && (my_boat_seat == 0))
            {
                POST_NOTIF("Boat", "Your boat has died.")
            }
            
            frozenheart_model = rbx_instance_find_first_child(task, map_model, "FrozenHeart");
            frozenheart_model_pp = rbx_model_get_primarypart(task, frozenheart_model);
            
            sleep(2);
        }
    });
    
    
    
#pragma mark Instances [4]
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            int other_player_index = 0;
            COPY_BUF(other_player_hums)
            COPY_BUF(other_player_hrps)
            COPY_BUF(other_player_names)
            
            COPY_BUF(hrps_with_chalice)
            
            char __chalice_status = 0;
            int chalice_index = 0;
            FOR_EACH_CHILD(players_service, i, {
                vm_address_t player = child;
                if (player != local_player)
                {
                    vm_address_t c = rbx_player_get_character(task, player);
                    vm_address_t hrp = rbx_instance_find_first_child(task, c, "HumanoidRootPart");
                    vm_address_t hum = rbx_instance_find_first_child(task, c, "Humanoid");
                    vm_address_t bp = rbx_instance_find_first_child(task, player, "Backpack");
                    
                    char has_item = false;
                    if (rbx_instance_find_first_child_partial(task, bp, "Chalice") || rbx_instance_find_first_child_partial(task, bp, "Fist of"))
                    {
                        __chalice_status = true;
                        has_item = true;
                    }
                    if (rbx_instance_find_first_child_partial(task, c, "Chalice") || rbx_instance_find_first_child_partial(task, c, "Fist of"))
                    {
                        __chalice_status = true;
                        has_item = true;
                    }
                    
                    long name_len = 0;
                    char* name = rbx_instance_get_name(task, player, &name_len);
                    if (name)
                    {
                        if (hrp && hum && rbx_humanoid_get_health(task, hum) > 0)
                        {
                            vm_address_t data_folder = rbx_instance_find_first_child(task, player, "Data");
                            //rbx_print_descendants(task, df, 0, 1);
                            vm_address_t race_sv = rbx_instance_find_first_child(task, data_folder, "Race");
                            vm_address_t lvl_iv = rbx_instance_find_first_child(task, data_folder, "Level");
                            
                            long race_sl = 0;
                            char* race = rbx_stringvalue_get_value(task, race_sv, &race_sl);
                            long lvl = rbx_intvalue_get_value(task, lvl_iv);
                            
                            if (race)
                            {
                                sprintf(__other_player_names + (other_player_index * MAX_ESP_TEXT_LENGTH), "%s [Lv. %ld] [Race: %s]", name, lvl, race);
                                vm_deallocate(mach_task_self_, (vm_address_t)race, race_sl);
                            }
                            
                            //ESP_Color color = custom_haki_esp_color;
                            if (has_item) {
                                //color = chalice_esp_color;
                                __hrps_with_chalice[chalice_index] = hrp;
                                chalice_index++;
                            }
                            
                            __other_player_hrps[other_player_index] = hrp;
                            __other_player_hums[other_player_index] = hum;
                            other_player_index++;
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                    }
                }
            })
            chalice_or_fist_exists = __chalice_status;
            
            chalice_count = chalice_index;
            memcpy(hrps_with_chalice, __hrps_with_chalice, sizeof(hrps_with_chalice));
            
            other_player_count = other_player_index;
            memcpy(other_player_hrps, __other_player_hrps, sizeof(other_player_hrps));
            memcpy(other_player_hums, __other_player_hums, sizeof(other_player_hums));
            memcpy(other_player_names, __other_player_names, sizeof(other_player_names));
            
            
            
            sleep(4);
        }
    });
    
    
    
    
#pragma mark Instances [5]
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            int chest_index = 0;
            FOR_EACH_CHILD(chestmodels_folder, i, {
                vm_address_t pushbox = rbx_instance_find_first_child(task, child, "PushBox");
                chests[chest_index++] = pushbox;
            })
            chest_count = chest_index;
            
            if (item_collection_mode == BLOX_FRUITS_ITEM_COLLECTION_MODE_CHESTS_ONLY)
            {
                memcpy(items, chests, sizeof(chests));
                item_count = chest_count;
            }
            
            sleep(3);
        }
    });
    
    
#pragma mark Instances [6]
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            vm_address_t my_tool = rbx_instance_find_first_child_by_pointee(task, my_character, task_base_address, RBX_OBJECT_Tool_POINTEE_OFFSET);
            //rbx_print_descendants(task, my_tool, 0, 1);
            if (rbx_instance_is_named(task, my_tool, "Dragonstorm"))
            {
                dragonstorm_equipped = true;
                tp_aimbot_enabled = false;
                //current_plr_hitbox_size = new_plr_hitbox_size;
            }
            else
            {
                dragonstorm_equipped = false;
                tp_aimbot_enabled = true;
                if (!dungeon_mode_enabled)
                {
                    for (int i = 0 ; i < other_player_count ; i++)
                    {
                        rbx_basepart_set_size(task, other_player_hrps[i], old_plr_hitbox_size);
                    }
                    FOR_EACH_DESCENDANT(task, rbx_instance_find_first_child(task, my_character, "EquippedWeapon"), 10000, 2, i, {
                        //if (rbx_instance_is_a(task, obj, "Part") || rbx_instance_is_a(task, obj, "MeshPart"))
                        vm_address_t pointee_offset = vm_read_uint64_t_value(task, obj) - task_base_address;
                        if (pointee_offset == RBX_OBJECT_MeshPart_POINTEE_OFFSET || pointee_offset == RBX_OBJECT_Part_POINTEE_OFFSET)
                        {
                            rbx_basepart_set_size(task, obj, og_hitbox_size);
                        }
                    })
                }
            }
            sleep(1);
        }
    });
    
    
#pragma mark GUN HITBOXES
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (dragonstorm_equipped)
            {
                rbx_cframe_t my_hrp_cf = rbx_basepart_get_cframe(task, my_hrp);
                if (!dungeon_mode_enabled)
                {
                    for (int i = 0 ; i < other_player_count ; i++)
                    {
                        vm_address_t op_hrp = other_player_hrps[i];
                        rbx_cframe_t op_hrp_cf = rbx_basepart_get_cframe(task, op_hrp);
                        
                        float f = vector3_dist_dif(my_hrp_cf.pos, op_hrp_cf.pos);
                        float __f = (f > 20) ? f - 10 : 10;
                        vector3_t new_sz = (f > 80) ? new_plr_hitbox_size : (vector3_t){.x = __f, .y = __f, .z = __f};
                        rbx_basepart_set_size(task, op_hrp, new_sz);
                    }
                }
                for (int i = 0 ; i < enemy_count ; i++)
                {
                    vm_address_t e_hrp = enemy_hrps[i];
                    rbx_cframe_t e_hrp_cf = rbx_basepart_get_cframe(task, e_hrp);
                    
                    float f = vector3_dist_dif(my_hrp_cf.pos, e_hrp_cf.pos);
                    float __f = (f > 20) ? f - 10 : 10;
                    vector3_t new_sz = (f > 80) ? new_plr_hitbox_size : (vector3_t){.x = __f, .y = __f, .z = __f};
                    rbx_basepart_set_size(task, e_hrp, new_sz);
                }
            }
            else
            {
                usleep(300000);
            }
            usleep(40000);
        }
    });
    
    
    
    
    
    
#pragma mark ESP Object Loop
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            //printf("S\n");
            int esp_part_index = 0;
            if (esp_enabled)
            {
                int fruit_index = 0;
                
                {
                    char __mirage_island_exists = false;
                    char __prehistoric_island_exists = false;
                    char __kitsune_island_exists = false;
                    char __frozen_dimension_exists = false;

                    FOR_EACH_CHILD(locations_folder, i, {
                        vm_address_t loc = child;
                        long n_l = 0;
                        char* n = rbx_instance_get_name(task, loc, &n_l);
                        if (n)
                        {
                            if (strcmp(n, "Kitsune Island") == 0)
                            {
                                APPEND_ESP_OBJ(loc, kitsune_esp_color, n);
                                __kitsune_island_exists = true;
                            }
                            else if (strcmp(n, "Mirage Island") == 0)
                            {
                                APPEND_ESP_OBJ(loc, mirage_esp_color, n);
                                float d = distance_between_baseparts(task, my_boat_seat, loc);
                                if (circle_boat_enabled && (d < 10000))
                                {
                                    POST_NOTIF("Mirage Island", "Mirage Island has spawned!")
                                    mirage_island_cframe = rbx_basepart_get_cframe(task, loc);
                                }
                                __mirage_island_exists = true;
                            }
                            else if (strcmp(n, "Frozen Dimension") == 0)
                            {
                                APPEND_ESP_OBJ(loc, blue_flower_esp_color, n);
                                float d = distance_between_baseparts(task, my_boat_seat, loc);
                                if (circle_boat_enabled && (d < 10000))
                                {
                                    POST_NOTIF("Frozen Dimension", "The Frozen Dimension has spawned!")
                                    frozen_dimension_cframe = rbx_basepart_get_cframe(task, loc);
                                }
                                __frozen_dimension_exists = true;
                            }
                            else if (strcmp(n, "Prehistoric Island") == 0)
                            {
                                APPEND_ESP_OBJ(loc, quest_torches_esp_color, n);
                                float d = distance_between_baseparts(task, my_boat_seat, loc);
                                if (circle_boat_enabled && (d < 10000))
                                {
                                    POST_NOTIF("Prehistoric Island", "Prehistoric Island has spawned!")
                                    prehistoric_island_cframe = rbx_basepart_get_cframe(task, loc);
                                }
                                __prehistoric_island_exists = true;
                            }
                            else if (strcmp(n, "Treasure Island") == 0)
                            {
                                APPEND_ESP_OBJ(loc, yellow_esp_color, n);
                            }
                            else
                            {
                                if (frozenheart_model)
                                {
                                    if (strcmp(n, "Tiki Outpost") == 0)
                                    {
                                        APPEND_ESP_OBJ(loc, yellow_esp_color, n);
                                    }
                                }
                            }
  
                            vm_deallocate(mach_task_self_, (vm_address_t)n, n_l);
                        }
                    })
                    mirage_island_exists = __mirage_island_exists;
                    prehistoric_island_exists = __prehistoric_island_exists;
                    frozen_dimension_exists = __frozen_dimension_exists;
                    kitsune_island_exists = __kitsune_island_exists;
                    
                }
                
                //APPEND_ESP_OBJ(rbx_instance_find_last_child_of_class(task, locations_folder, "Part"), quest_torches_esp_color, "?????");
                APPEND_ESP_OBJ(frozenheart_model_pp, yellow_esp_color, "LEVI HEART")
                APPEND_ESP_OBJ(elite_enemy_hrp, yellow_esp_color, "ELITE ENEMY")
                APPEND_ESP_OBJ(advanced_fruit_dealer_part, yellow_esp_color, "ADVANCED FRUIT DEALER")
                APPEND_ESP_OBJ(legendary_sword_dealer_part, yellow_esp_color, "LEGENDARY SWORD DEALER")
                
                APPEND_ESP_OBJ(my_boat_seat, quest_torches_esp_color, "MY BOAT");
                APPEND_ESP_OBJ(minktrial_endpoint, quest_torches_esp_color, "ENDPOINT");
                
                APPEND_ESP_OBJ(mirage_gear, mirage_esp_color, "GEAR")
                //printf("-- MIRAGE GEAR --, %f, %f, %f\n", sz.x, sz.y, sz.z);
                //rbx_print_descendants(task, mirage_gear, 0, 0);
                
                FOR_EACH_CHILD(workspace, i, {
                    vm_address_t object = child;
                    long name_len = 0;
                    char* name = rbx_instance_get_name(task, object, &name_len);
                    if (name)
                    {
                        if (strcmp(name, "EmberTemplate") == 0)
                        {
                            //rbx_print_descendants(task, object, 0, 0);
                            vm_address_t _part = rbx_instance_find_first_child(task, object, "Part");
                            //rbx_print_descendants(task, _part, 0, 1);
                            if (_part)
                            {
                                vm_address_t cf_addr = rbx_basepart_get_properties_address(task, _part) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                                for (int x = 0 ; x < 100000 ; x++)
                                {
                                    //rbx_basepart_set_cframe(task, _part, vm_read_rbx_cframe_t_value(task, my_hrp_cframe_address));
                                    rbx_cframe_t new_cf =vm_read_rbx_cframe_t_value(task, my_hrp_cframe_address);
                                    vm_write(task, cf_addr, (vm_address_t)&new_cf, sizeof(rbx_cframe_t));
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
                })
                
                //rbx_print_descendants(task, chestmodels_folder, 0, 1);
                
                fruit_count = fruit_index;
                
                
                if (show_torches)
                {
                    FOR_EACH_CHILD(floating_turtle_questtorches_model, i, {
                        long name_len = 0;
                        char* name = rbx_instance_get_name(task, child, &name_len);
                        if (name)
                        {
                            APPEND_ESP_OBJ(child, quest_torches_esp_color, name)
                            vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                        }
                    })
                }
                
                if (jungle_button_count > 0)
                {
                    for (int i = 0 ; i < jungle_button_count ; i++)
                    {
                        vm_address_t button = jungle_buttons[i];
                        char name[200];
                        sprintf(name, "Button%d", i);
                        APPEND_ESP_OBJ(button, quest_torches_esp_color, name);
                    }
                }
                
                COPY_VAR(other_player_count)
                COPY_BUF(other_player_hrps)
                COPY_BUF(other_player_names)
                for (int i = 0 ; i < __other_player_count ; i++)
                {
                    APPEND_ESP_OBJ(__other_player_hrps[i], custom_haki_esp_color, __other_player_names + (i * MAX_ESP_TEXT_LENGTH));
                }
                
            }
            //printf("F\n");
            esp_part_count = esp_part_index;
            sleep(1);
        }
    });
    
    
    
    
    
#pragma mark - Keybinds -
    
    
    
    
    
#pragma mark m
    
    
    PRESS_ONCE_KEYBIND_CREATE('m', {
        if (item_collect_enabled == true)
        {
            noclip_enabled = false;
        }
        fly_enabled = !fly_enabled;
        if (fly_enabled == false)
        {
            item_collect_enabled = false;
        }
        rbx_smooth_fly_patch(task, task_base_address,fly_enabled);
    }, fly_keybind_enabled, "fly")
    
#pragma mark =
    
    PRESS_ONCE_KEYBIND_CREATE('=', {
        boatfly_enabled = !boatfly_enabled;
    }, boat_keybind_enabled && boathacks_enabled, "fly boat")
    
#pragma mark b
    
   PRESS_ONCE_KEYBIND_CREATE('b', {
       if (my_hrp && locking_on_enemy)
       {
           bailing = !bailing;
       }
       else
       {
           bailing = false;
       }
   }, bail_keybind_enabled, "bail (while locking)")
    
    
#pragma mark p
    
    PRESS_ONCE_KEYBIND_CREATE('p', {
        low_prio = !low_prio;
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
    }, priority_keybind_enabled, "set priority")
    
#pragma mark l
    
    PRESS_ONCE_KEYBIND_CREATE('l', {
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
    }, lock_keybind_enabled, "lock onto enemy")
    
    
#pragma mark k
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(10000);
            if (shared_memory->KEYS_DOWN['k'] && rift_dimension_stagepart)
            {
                returning_from_rift = true;
            }
            else
            {
                returning_from_rift = false;
            }
        }
    });
    
    
    
    
    
#pragma mark - NoClip -
    
    
    
    
    
    
    BASIC_NOCLIP_ENGINE
    
    
    
    
    
    
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
                            if (!((item_collection_mode == BLOX_FRUITS_ITEM_COLLECTION_MODE_CHESTS_ONLY) && !rbx_instance_find_first_child(task, part, "TouchInterest")))
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
                //rbx_print_descendants(task, closest_item, 0, 0);
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
                usleep(100000);
            }
            usleep(1);
        }
    });
    
    
#pragma mark - Rift Return -
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (returning_from_rift && !bailing && my_hrp_cframe_address)
            {
                vm_write(task, my_hrp_cframe_address + 0x24, (vm_address_t)&rift_dimension_return_pos, 12);
            }
            else
            {
                usleep(100000);
            }
            usleep(50);
        }
    });
    
    
    
    
    
#pragma mark - Aimbot / Lock -
    
    
    
    
#pragma mark - CFrame writes -
    
    
    static int cframe_write_mode = 0;
    static int cframe_write_usleep_time = 1000;
    static int lock_usleep_time = 5;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (cframe_write_mode == 1 && !returning_from_rift && !(lock_onto_npc && npc_hrp_to_lock_on_to))
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
            if (locking_on_enemy && !bailing && !returning_from_rift && !(loop_goto_plr_enabled) && !(lock_onto_npc && npc_hrp_to_lock_on_to))
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
    
    
    static char should_set_cframe = false;
    static vector3_t new_pos = {};
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            char __sscf = false;
            if (lock_onto_npc)
            {
                COPY_VAR(npc_hrp_to_lock_on_to)
                if (__npc_hrp_to_lock_on_to && rbx_instance_is_valid_child(task, __npc_hrp_to_lock_on_to))
                {
                    if ((rbx_humanoid_get_health(task, lock_npc_hum) > 0) ||
                        (__npc_hrp_to_lock_on_to == dungeon_boss_hrp && (rbx_instance_get_child_count(task, players_service) > 1)))
                    {
                        rbx_cframe_t cf = rbx_basepart_get_cframe(task, __npc_hrp_to_lock_on_to);
                        cf.pos.y += npc_lock_y_offset;
                        rbx_cframe_t my_hrp_cf = rbx_basepart_get_cframe(task, my_hrp);
                        
                        float d = vector3_dist_dif(cf.pos, my_hrp_cf.pos);
                        float __m = 40;
                        float f = (__m > d) ? d : __m;
                        vector3_t add = vector3_mul_num(vector3_normalized(vector3_sub(cf.pos, my_hrp_cf.pos)), f);
                        new_pos = vector3_add(my_hrp_cf.pos, add);
                        __sscf = true;
                    }
                    else
                    {
                        npc_hrp_to_lock_on_to = 0;
                    }
                }
            }
            else
            {
                usleep(200000);
            }
            should_set_cframe = __sscf;
            usleep(50000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (should_set_cframe)
            {
                vm_write(task, my_hrp_cframe_address + 0x24, (vm_address_t)&new_pos, sizeof(vector3_t));
            }
            else
            {
                usleep(100000);
            }
            usleep(10);
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
            usleep(100000);
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
                        rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, closest_hrp_to_mouse);
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        float camera_fov = rbx_camera_get_field_of_view(task, camera);
                        float window_w = shared_memory->WINDOW_W;
                        float window_h = shared_memory->WINDOW_H;
                        //printf("%f, %f, %f, %f\n", window_x, window_y, window_w, window_h);
                        custom_rbx_world_to_screen_info_t wtsi = rbx_world_to_screen_point(camera_cframe, camera_fov, hrp_cframe.pos, window_w, window_h, 1, 1);
                        if (wtsi.x > window_inset && wtsi.x < (window_w - window_inset))
                        {
                            if (wtsi.y > window_inset && wtsi.y < (window_h - window_inset))
                            {
                                mousemove(shared_memory, NSMakePoint(wtsi.x, wtsi.y));
                            }
                        }
                        usleep(10);
                    }
                    else
                    {
                        usleep(100);
                    }
                }
                else
                {
                    usleep(1000);
                }
            }
            else
            {
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
                        usleep(10);
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
    
    
    BASIC_ESP_ENGINE({}, {
        
        char has_chalice = false;
        for (int x = 0 ; x < chalice_count ; x++)
        {
            if (part == hrps_with_chalice[x])
            {
                x = chalice_count;
                has_chalice = true;
            }
        }
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
        else if (has_chalice)
        {
            esp_color = chalice_esp_color;
        }
        else
        {
            esp_color.a = 0.90;
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
            NSRect frame = {.origin.x = shared_memory->WINDOW_H};
            frame.origin.x = 0;
            frame.origin.y = shared_memory->WINDOW_H - 240;
            frame.size.width = shared_memory->WINDOW_W;
            frame.size.height = 40;
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
            NSRect frame;
            frame.origin.x = 0;
            frame.origin.y = shared_memory->WINDOW_H - 255;
            frame.size.width = shared_memory->WINDOW_W;
            frame.size.height = 40;
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
        if (frozen_dimension_exists)
        {
            //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 280, .w = shared_memory->WINDOW_W, .h = 40};
            NSRect frame;
            frame.origin.x = 0;
            frame.origin.y = shared_memory->WINDOW_H - 270;
            frame.size.width = shared_memory->WINDOW_W;
            frame.size.height = 40;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            char text[MAX_ESP_TEXT_LENGTH];
            bzero(text, MAX_ESP_TEXT_LENGTH);
            sprintf(text, "%s", "FROZEN DIMENSION");
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = blue_flower_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
        if (prehistoric_island_exists)
        {
            //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 280, .w = shared_memory->WINDOW_W, .h = 40};
            NSRect frame;
            frame.origin.x = 0;
            frame.origin.y = shared_memory->WINDOW_H - 285;
            frame.size.width = shared_memory->WINDOW_W;
            frame.size.height = 40;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            char text[MAX_ESP_TEXT_LENGTH];
            bzero(text, MAX_ESP_TEXT_LENGTH);
            sprintf(text, "%s", "PREHISTORIC ISLAND");
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = quest_torches_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
        if (chalice_or_fist_exists)
        {
            //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 280, .w = shared_memory->WINDOW_W, .h = 40};
            NSRect frame;
            frame.origin.x = 0;
            frame.origin.y = shared_memory->WINDOW_H - 300;
            frame.size.width = shared_memory->WINDOW_W;
            frame.size.height = 40;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            char text[MAX_ESP_TEXT_LENGTH];
            bzero(text, MAX_ESP_TEXT_LENGTH);
            sprintf(text, "%s", "CHALICE/FIST ALERT");
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = chalice_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
        if ((is_full_moon || is_waxing_gibbous || is_waxing_halfmoon))
        {
            //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 200, .w = shared_memory->WINDOW_W, .h = 40};
            NSRect frame;
            frame.origin.x = 0;
            frame.origin.y = shared_memory->WINDOW_H - 200;
            frame.size.width = shared_memory->WINDOW_W;
            frame.size.height = 40;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            
            char text[MAX_ESP_TEXT_LENGTH];
            bzero(text, MAX_ESP_TEXT_LENGTH);
            char* _m = "FULL MOON";
            if (is_waxing_gibbous){_m = "WAXING GIBBOUS";}
            if (is_waxing_halfmoon){_m = "WAXING HALF-MOON";}
            sprintf(text, "%s [%.1f]\n", _m, ((float)current_clocktime/RBX_LIGHTING_CLOCKTIME_HOUR_TICK_COUNT));
            
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = red_flower_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
        {
            //ESP_Frame frame = (ESP_Frame){.x = shared_memory -> WINDOW_W - 200, .y = (shared_memory->WINDOW_H - 300) * 0.5, .w = 200, .h = 300};
            NSRect frame;
            frame.origin.x = shared_memory -> WINDOW_W - 200;
            frame.origin.y = (shared_memory->WINDOW_H - 300) * 0.5;
            frame.size.width = 200;
            frame.size.height = 300;
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
        if (item_collect_enabled && __closest_item)
        {
            vel = vector3_direction_to_from(rbx_basepart_get_cframe(task, __closest_item).pos, rbx_basepart_get_cframe(task, __my_hrp).pos);
            vel = vector3_mul_num(vel, fly_speed);
            rbx_basepart_set_velocity(task, __my_hrp, vel);
        }
        else if (fly_enabled)
        {
            BASIC_FLY_CONTROL_CODE
        }
    }, 300)
    
    
    
#pragma mark - NPC Kill Aura -
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(2);
            COPY_BUF(enemies)
            COPY_BUF(enemy_hrps)
            COPY_BUF(enemy_hums)
            COPY_BUF(enemy_first_meshparts)
            int __i = 0;
            int piranha_index = 0;
            FOR_EACH_CHILD(enemies_folder, i, {
                vm_address_t enemy = child;
                //rbx_print_descendants(task, enemy, 0, 1);
                vm_address_t enemy_hum = rbx_instance_find_first_child_by_pointee(task, enemy, task_base_address, RBX_OBJECT_Humanoid_POINTEE_OFFSET);
                if (enemy_hum)
                {
                    vm_address_t enemy_hrp = rbx_instance_find_first_child(task, enemy, "HumanoidRootPart");
                    vm_address_t enemy_fmp = rbx_instance_find_first_child_by_pointee(task, enemy, task_base_address, RBX_OBJECT_MeshPart_POINTEE_OFFSET);
                    
                    long dn_l = 0;
                    char* dn = rbx_humanoid_get_displayname(task, enemy_hum, &dn_l);
                    if (dn)
                    {
                        if (strstr(dn, "Piranha"))
                        {
                            piranhas[piranha_index] = enemy;
                            piranha_hrps[piranha_index] = enemy_hrp;
                            piranha_index++;
                        }
                        if (!(dungeon_mode_enabled && (strstr(dn, "'s Shadow") || strstr(dn, "'s Friendo"))))
                        {
                            __enemies[__i] = enemy;
                            __enemy_hrps[__i] = enemy_hrp;
                            __enemy_hums[__i] = enemy_hum;
                            __enemy_first_meshparts[__i] = enemy_fmp;
                            __i++;
                        }
                        else
                        {
                            rbx_basepart_set_size(task, enemy_hrp, current_plr_hitbox_size);
                        }
                        vm_deallocate(mach_task_self(), (vm_address_t)dn, dn_l);
                    }
                }
            })
            memcpy(enemies, __enemies, sizeof(enemies));
            memcpy(enemy_hrps, __enemy_hrps, sizeof(enemy_hrps));
            memcpy(enemy_hums, __enemy_hums, sizeof(enemy_hums));
            memcpy(enemy_first_meshparts, __enemy_first_meshparts, sizeof(enemy_first_meshparts));
            
            piranha_count = piranha_index;
            enemy_count = __i;
        }
    });
   
    /*
     dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(3);
            FOR_EACH_CHILD(seabeasts_folder, {
                vm_address_t enemy = child;
                rbx_print_descendants(task, enemy, 0, 2);
                vm_address_t enemy_fmp = rbx_instance_find_first_child(task, enemy, "RootPart");
                rbx_basepart_set_size(task, enemy_fmp, new_sb_hitbox_size);
            })
        }
    });
    */
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        vector3_t hb = {.x = 0.1, .y = 0.1, .z = 0.1};
        for (;;)
        {
            COPY_BUF(enemies)
            COPY_BUF(enemy_hums)
            COPY_VAR(enemy_count)
            for (int i = 0 ; i < __enemy_count ; i++)
            {
                vm_address_t enemy = __enemies[i];
                vm_address_t hum = __enemy_hums[i];
                if (!(rbx_humanoid_get_health(task, hum) > 0))
                {
                    blox_fruits_expand_hitbox(task, task_base_address, enemy, hb, true);
                }
            }
            usleep(300000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(1);
            vm_address_t ee_hrp = 0;
            
            COPY_BUF(enemies)
            COPY_BUF(enemy_hums)
            COPY_BUF(enemy_hrps)
            COPY_BUF(enemy_first_meshparts)
            COPY_VAR(enemy_count)
            for (int i = 0 ; i < __enemy_count ; i++)
            {
                //vm_address_t enemy = __enemies[i];
                vm_address_t enemy_hrp = __enemy_hrps[i];
                vm_address_t enemy_hum = __enemy_hums[i];
                
                long e_name_l = 0;
                char* e_name = rbx_humanoid_get_displayname(task, enemy_hum, &e_name_l);
                if (e_name)
                {
                    
                    if (strstr(e_name, "Diablo") || strstr(e_name, "Deandre") || strstr(e_name, "Urban"))
                    {
                        ee_hrp = enemy_hrp;
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)e_name, e_name_l);
                }
            }

            elite_enemy_hrp = ee_hrp;
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(100000);
            if (raid_assist_enabled)
            {
                COPY_BUF(enemy_hums)
                COPY_BUF(enemy_hrps)
                COPY_VAR(enemy_count)
                for (int i = 0 ; i < __enemy_count ; i++)
                {
                    vm_address_t hum = __enemy_hums[i];
                    if (rbx_instance_is_valid_child(task, hum))
                    {
                        long dn_l = 0;
                        char* dn = rbx_humanoid_get_displayname(task, hum, &dn_l);
                        if (dn)
                        {
                            if (strstr(dn, "Master [") ||
                                strstr(dn, "Lava Golem") ||
                                strstr(dn, "Piranha") ||
                                strstr(dn, "User [") ||
                                strstr(dn, "Shocker [") ||
                                strstr(dn, "Bone Breaker ["))
                            {
                                rbx_humanoid_set_health(task, hum, 0);
                            }
                            vm_deallocate(mach_task_self_, (vm_address_t)dn, dn_l);
                        }
                    }
                }
            }
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(1);
            vm_address_t __dungeon_boss = 0;
            vm_address_t __dungeon_boss_hum = 0;
            vm_address_t __dungeon_boss_hrp = 0;
            if (dungeon_mode_enabled)
            {
                COPY_BUF(enemies)
                COPY_BUF(enemy_hums)
                COPY_BUF(enemy_hrps)
                COPY_VAR(enemy_count)
                for (int i = 0 ; i < __enemy_count ; i++)
                {
                    vm_address_t enemy = __enemies[i];
                    vm_address_t hum = __enemy_hums[i];
                    vm_address_t hrp = __enemy_hrps[i];
                    if (rbx_instance_is_valid_child(task, hum))
                    {
                        long dn_l = 0;
                        char* dn = rbx_humanoid_get_displayname(task, hum, &dn_l);
                        if (dn)
                        {
                            if (strstr(dn, "[Dungeon Boss]"))
                            {
                                vm_address_t it_bv = rbx_instance_find_first_child(task, enemy, "IsTransformed");
                                //For some reason, the gas knight cant be killed when untransformed.
                                if (strstr(dn, "Gas Knight"))
                                {
                                    if (rbx_boolvalue_get_value(task, it_bv) == true)
                                    {
                                        //rbx_humanoid_set_health(task, hum, 0);
                                        __dungeon_boss_hrp = hrp;
                                        __dungeon_boss_hum = hum;
                                        __dungeon_boss = enemy;
                                    }
                                }
                                else
                                {
                                    //rbx_humanoid_set_health(task, hum, 0);
                                    __dungeon_boss_hrp = hrp;
                                    __dungeon_boss_hum = hum;
                                    __dungeon_boss = enemy;
                                }
                            }
                            vm_deallocate(mach_task_self_, (vm_address_t)dn, dn_l);
                        }
                    }
                }
            }
            dungeon_boss_hrp = __dungeon_boss_hrp;
            dungeon_boss_hum = __dungeon_boss_hum;
            dungeon_boss = __dungeon_boss;
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        vector3_t hb = {.x =0.0001, .y =0.0001,.z =0.0001};
        for (;;)
        {
            if (!dungeon_mode_enabled)
            {
                sleep(3);
            }
            else
            {
                COPY_VAR(dungeon_boss_hum)
                COPY_VAR(lock_npc_hum)
                if (__dungeon_boss_hum && (dungeon_boss_hum == __lock_npc_hum))
                    //if (__dungeon_boss_hum)
                {
                    //printf("dungeon boss hum\n");
                    if (rbx_instance_is_valid_child(task, __dungeon_boss_hum))
                    {
                        rbx_humanoid_set_health(task, __dungeon_boss_hum, 0);
                        //vector3_t pos = rbx_basepart_get_cframe(task, dungeon_boss_hrp).pos;
                        //printf("%f, %f, %f\n", pos.x, pos.y, pos.z);
                        //blox_fruits_expand_hitbox(task, dungeon_boss, hb, true);
                    }
                }
                else
                {
                    usleep(500000);
                }
            }
            usleep(100);
        }
    });
    
    
    
    
#pragma mark - Boatfly (+circleboat) -
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (boathacks_enabled && boatfly_enabled && rbx_instance_get_parent(task, rbx_instance_get_parent(task, my_humanoid_current_seat_part)) == boats_folder)
            {
                rbx_cframe_t cf = rbx_basepart_get_cframe(task, my_humanoid_current_seat_part);
                cf.pos.y = 30;
                
                if (circle_boat_enabled)
                {
                    if (vector3_dist_dif(circle_boat_pos, rbx_basepart_get_cframe(task, my_humanoid_current_seat_part).pos) > (circle_boat_radius * 1.5))
                    {
                        circle_boat_enabled = false;
                        printf("disabled circle boat: too far from point\n");
                    }
                }
                
                COPY_VAR(frozen_dimension_exists);
                COPY_VAR(mirage_island_exists);
                COPY_VAR(prehistoric_island_exists);
                
                char cond = (__frozen_dimension_exists);
                rbx_cframe_t cf_to_go_to = frozen_dimension_cframe;
                
                if (circleboat_stop_for_frozen_dimension) {cf_to_go_to = frozen_dimension_cframe; cond = (__frozen_dimension_exists);}
                if (circleboat_stop_for_mirage_island) {cf_to_go_to = mirage_island_cframe; cond = (__mirage_island_exists);}
                if (circleboat_stop_for_prehistoric_island) {cf_to_go_to = prehistoric_island_cframe; cond = (__prehistoric_island_exists);}
                
                //200 for frozen dimension, 2000 for prehistoric, 3000 for mirage
                float min_dist = 1000;
                if (__frozen_dimension_exists && circleboat_stop_for_frozen_dimension)
                {
                    min_dist = 200;
                }
                if (__prehistoric_island_exists && circleboat_stop_for_prehistoric_island)
                {
                    min_dist = 1500;
                }
                if (__mirage_island_exists && circleboat_stop_for_mirage_island)
                {
                    min_dist = 2500;
                }
                ; //200 for frozen dimension, 2000 for prehistoric, 3000 for mirage
                
                if (circle_boat_enabled && !cond)
                {
                    cf.pos.y = 190;
                    float x_diff = cf.pos.x - circle_boat_pos.x;
                    float z_diff = cf.pos.z - circle_boat_pos.z;
                    
                    circle_boat_angle = atan2f(x_diff, z_diff);
                    if (circle_boat_angle > (2 * PI)) {circle_boat_angle = circle_boat_angle - (2 * PI);}
                    circle_boat_angle += (2 * PI)/45000;
                    
                    float _x = sinf(circle_boat_angle);
                    float _z = cosf(circle_boat_angle);
                    float mag = sqrt(_x * _x + _z * _z);
                    
                    _x = _x * circle_boat_radius / mag;
                    _z = _z * circle_boat_radius / mag;
                    
                    //printf("%f\n", atan2f(x_diff, z_diff) * 180/PI);
                    
                    cf.pos.x = circle_boat_pos.x + _x;
                    cf.pos.z = circle_boat_pos.z + _z;
                }
                else
                {
                    vector3_t add = {};
                    bzero(&add, sizeof(vector3_t));
                    rbx_cframe_t ccf = rbx_camera_get_cframe(task, camera);
                    
                    if (cond && circle_boat_enabled)
                    {
                        float d = vector3_dist_dif(cf_to_go_to.pos, cf.pos);
                        if (d > min_dist)
                        {
                            add = vector3_sub(cf_to_go_to.pos, cf.pos);
                            add.y = 0;
                        }
                    }
                    else
                    {
                        vector3_t lv = rbx_cframe_get_look_vector(ccf);
                        vector3_t rv = rbx_cframe_get_right_vector(ccf);
                        
                        unsigned char* keys_down = shared_memory->KEYS_DOWN;
                        
                        if (keys_down['w']) { add = vector3_add(vector3_mul_num(lv, 1), add); }
                        if (keys_down['a']) { add = vector3_add(vector3_mul_num(rv, -1), add); }
                        if (keys_down['s']) { add = vector3_add(vector3_mul_num(lv, -1), add); }
                        if (keys_down['d']) { add = vector3_add(vector3_mul_num(rv, 1), add); }
                    }
                    
                    
                    float magnitude = vector3_magnitude(add);
                    if (!isnan(magnitude) && magnitude > 0.0f)
                    {
                        add = vector3_div_num(add,  magnitude);
                    }
                    add = vector3_mul_num(add, 0.6);
                    cf.pos = vector3_add(cf.pos, add);
                }
                //rbx_basepart_set_cframe(task, my_humanoid_current_seat_part, cf);
                vm_write(task, my_humanoid_current_seat_part_cframe_address, (vm_address_t)&cf, sizeof(cf));
            }
            else
            {
                sleep(1);
            }
            usleep(50);
        }
    });
    
    
    
    
#pragma mark piranhahunt
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            COPY_VAR(piranha_count)
            if (__piranha_count)
            {
                if (shared_memory->LEFT_MOUSE_DOWN)
                {
                    COPY_BUF(piranha_hrps)
                    rbx_cframe_t my_hrp_cf = rbx_basepart_get_cframe(task, my_hrp);
                    rbx_cframe_t cam_cf = rbx_camera_get_cframe(task, camera);
                    vector3_t cam_lv = rbx_cframe_get_look_vector(cam_cf);
                    float f = 120;
                    rbx_cframe_t new_cf = my_hrp_cf;
                    new_cf.pos = vector3_add(new_cf.pos, vector3_mul_num(cam_lv, f));
                    for (int i = 0 ; i < __piranha_count ; i++)
                    {
                        vm_address_t p_hrp = __piranha_hrps[i];
                        rbx_basepart_set_cframe(task, p_hrp, new_cf);
                        usleep(2);
                    }
                }
                else
                {
                    usleep(10000);
                }
            }
            else
            {
                sleep(1);
            }
        }
    });
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("fly", "", printf("ENABLED FLY\n"); fly_enabled = true;)
        INSERT_STDIN_INPUT_SIMPLE("nofly", "", printf("DISABLED FLY\n"); fly_enabled = false;)
        INSERT_STDIN_MULTIARG_INPUT("flyspeed %f", "", {printf("SET FLY SPEED TO %f\n", fly_speed);}, 1, &fly_speed);
        INSERT_STDIN_INPUT_SIMPLE("esp", "",printf("ENABLED ESP...\n"); esp_enabled = true;)
        INSERT_STDIN_INPUT_SIMPLE("noesp", "", printf("DISABLED ESP\n"); esp_enabled = false;)
        INSERT_STDIN_INPUT_SIMPLE("clip", "", printf("DISABLED NOCLIP\n"); noclip_enabled = false;)
        INSERT_STDIN_INPUT_SIMPLE("noclip", "", printf("ENABLED NOCLIP\n"); noclip_enabled = true;)
        INSERT_STDIN_INPUT_SIMPLE("sf", "//show fruit", {
            printf("SHOWING MOST RECENT FRUIT...\n");
            vm_address_t s = rbx_instance_find_first_child(task, workspace, "Fruit ");
            if (s)
            {
                rbx_camera_set_camera_subject(task, camera, s);
                sleep(3);
            }
            rbx_camera_set_camera_subject(task, camera, my_humanoid);
        })
        INSERT_STDIN_INPUT_SIMPLE("aimbot", "", printf("ENABLED AIMBOT\n"); aimbot_enabled = true;)
        INSERT_STDIN_INPUT_SIMPLE("noaimbot", "", printf("DISABLED AIMBOT\n"); aimbot_enabled = false;)
        INSERT_STDIN_INPUT_SIMPLE("showtorches", "", printf("SHOWING TORCHES...\n"); show_torches = true;)
        INSERT_STDIN_INPUT_SIMPLE("hidetorches", "", printf("HIDING TORCHES...\n"); show_torches = false;)
        INSERT_STDIN_INPUT_SIMPLE("chests", "", {
            rbx_smooth_fly_patch(task, task_base_address, 1);
            noclip_enabled = true;
            fly_enabled = true;
            item_collect_enabled = true;
            item_collection_mode = BLOX_FRUITS_ITEM_COLLECTION_MODE_CHESTS_ONLY;
            printf("COLLECTING ONLY CHESTS\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("circleboat", "", {
            if (my_humanoid_current_seat_part)
            {
                circle_boat_pos = rbx_basepart_get_cframe(task, my_humanoid_current_seat_part).pos;
                circle_boat_pos.x -= circle_boat_radius;
                circle_boat_enabled = true;
                circle_boat_angle = PI/2;
                printf("ENABLED CIRCLE BOAT\n");
            }
            else
            {
                printf("Error: no boat seat found\n");
            }
        })
        INSERT_STDIN_INPUT_SIMPLE("nocircleboat", "", printf("DISABLED CIRCLEBOAT\n"); circle_boat_enabled = false;)
        INSERT_STDIN_INPUT_SIMPLE("raidassist", "", printf("ENABLED RAID ASSIST\n"); raid_assist_enabled = true;)
        INSERT_STDIN_INPUT_SIMPLE("noraidassist", "", printf("DISABLED RAID ASSIST\n"); raid_assist_enabled = false;)
        INSERT_STDIN_INPUT_SIMPLE("boathacks", "", printf("ENABLED BOAT HACKS\n"); boathacks_enabled = true;)
        INSERT_STDIN_INPUT_SIMPLE("noboathacks", "", printf("DISABLED BOAT HACKS\n"); boathacks_enabled = false;)
        
        {
            char name[2000];
            INSERT_STDIN_MULTIARG_INPUT("loopgoto %s",  "",{
                goto_plr_cframe = rbx_basepart_get_cframe(task, rbx_instance_find_first_child(task, rbx_player_get_character(task, rbx_instance_find_first_child_partial(task, players_service, name)), "HumanoidRootPart"));
                loop_goto_plr_enabled = true;
                printf("ENABLED LOOPGOTO\n");
            }, 1, name)
        }
        INSERT_STDIN_INPUT_SIMPLE("noloopgoto", "", printf("DISABLED LOOPGOTO\n"); loop_goto_plr_enabled = false;)
        
        INSERT_STDIN_MULTIARG_INPUT("npclock %s %f", "//lock into npc of name, with y offset", {
            printf("LOCKING ONTO NPC: %s with %f Y offset\n", lock_npc_name, npc_lock_y_offset);
            lock_onto_npc = true;
        }, 2, lock_npc_name, &npc_lock_y_offset)
        
        INSERT_STDIN_INPUT_SIMPLE("nonpclock", "", printf("DISABLED NPC LOCK\n"); lock_onto_npc = false;)
        INSERT_STDIN_INPUT_SIMPLE("dungeon",  "", printf("ENABLED DUNGEON MODE\n"); dungeon_mode_enabled = true; aimbot_enabled = false; esp_enabled = false; npc_lock_y_offset = 0 ; strcpy(lock_npc_name, "[Dungeon Boss]"); lock_onto_npc = true;)
        INSERT_STDIN_INPUT_SIMPLE("nodungeon", "", {
            printf("DISABLED DUNGEON MODE\n");
            dungeon_mode_enabled = false;
            npc_lock_y_offset = 0 ;
            strcpy(lock_npc_name, "");
            lock_onto_npc = false;
        })
        INSERT_STDIN_INPUT_SIMPLE("minktrial", "", {
            printf("DOING MINK TRIAL\n");
            if (minktrial_endpoint)
            {
                for (int x = 0 ; x < 100 ; x++)
                {
                    rbx_basepart_set_cframe(task, my_hrp, rbx_basepart_get_cframe(task, minktrial_endpoint));
                }
            }
        })
        INSERT_STDIN_INPUT_SIMPLE("hunt_frozen_dimension", "", {
            circleboat_stop_for_frozen_dimension = true; circleboat_stop_for_prehistoric_island = false; circleboat_stop_for_mirage_island = false;
            printf("HUNTING FOR FROZEN DIMENSION ONLY\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("hunt_prehistoric", "", {
            circleboat_stop_for_frozen_dimension = false; circleboat_stop_for_prehistoric_island = true; circleboat_stop_for_mirage_island = false;
            printf("HUNTING FOR PREHISTORIC ISLAND ONLY\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("hunt_mirage", "", {
            circleboat_stop_for_frozen_dimension = false; circleboat_stop_for_prehistoric_island = false; circleboat_stop_for_mirage_island = true;
            printf("HUNTING FOR MIRAGE ISLAND ONLY\n");
        })
    })
    
   
    
}
