

void blox_fruits_cheat(task_t task)
{
    printf("- BLOX FRUITS -\n");
    
    LIBESP_DYLIB_SETUP
    
    static int fruit_count = 0;
    static vm_address_t fruit_models[MAX_ESP_COUNT];
    
    static ESP_Color mirage_esp_color = {.r = 1, .g = 0, .b = 0.8, .a = 1};
    static ESP_Color fruit_esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    static ESP_Color red_flower_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color blue_flower_esp_color = {.r = 0, .g = 0, .b = 1, .a = 1};
    static ESP_Color quest_torches_esp_color = {.r = 1, .g = 0.3, .b = 0, .a = 1};
    static ESP_Color custom_haki_esp_color = {.r = 1, .g = 1, .b = 1, .a = 1};
    static ESP_Color target_esp_color1 = {.r = 0, .g = 0.75, .b = 0.75, .a = 1};
    static ESP_Color target_esp_color2 = {.r = 1, .g = 0, .b = 0.3, .a = 1};
    
    static vector3_t factory_raid_core_pos = {.x = 432, .y = 211, .z = -430};
    
    static int esp_part_count = 0;
    static ESP_Color esp_part_colors[MAX_ESP_COUNT];
    static char esp_part_names[MAX_ESP_COUNT * MAX_ESP_TEXT_LENGTH];
    static vm_address_t esp_parts[MAX_ESP_COUNT];
    
    static useconds_t esput = 1200;
    static useconds_t fut = 100000;
    static useconds_t iut = 100000;

    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = esput;
    
    shared_memory->FUNCTION_USLEEP_TIME = fut;
    shared_memory->INPUT_USLEEP_TIME = iut;
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    vm_address_t instance_set_parent_func = task_base_address + RBX_INSTANCE_PARENT_SETTER_METHOD_OFFSET;
    vm_address_t humanoid_set_health_func = task_base_address + RBX_HUMANOID_HEALTH_SETTER_METHOD_OFFSET;
    
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    //rbx_print_descendants(task, players_service, 0, 1);
    //vm_address_t replicated_storage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t badgeservice = rbx_instance_find_first_child_of_class(task, game, "BadgeService");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t enemies_folder = rbx_instance_find_first_child(task, workspace, "Enemies");
    vm_address_t lighting_service = rbx_instance_find_first_child_of_class(task, game, "Lighting");
    vm_address_t sky = rbx_instance_find_first_child_of_class(task, lighting_service, "Sky");
    static char* fullmoon_assetid = "9709149431"; //http://www.roblox.com/asset/?id=9709149431
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    //rbx_print_descendants(task, workspace, 0, 4);
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    
    static vm_address_t map_model = 0;
    static vm_address_t floating_turtle_island = 0;
    static vm_address_t floating_turtle_questtorches_model = 0;
    
    static vm_address_t my_character = 0;
    static vm_address_t my_humanoid = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    
    static vm_address_t closest_hrp_to_mouse = 0;
    static vm_address_t closest_hrp_to_mouse_cframe_address = 0;
    
    static vm_address_t closest_chest = 0;
    
    static vm_address_t mystic_island = 0;
    static vector3_t vel;
    static float fly_speed = 275.0f;
    static float aimbot_pixel_radius = 220;
    static float aimbot_max_dist = 500;
    static float aimbot_max_tp_dist = 150;
    
    static char fly_enabled = false;
    static char hide_map = false;
    static char aimbot_enabled = true;
    static char tp_aimbot_enabled = true;
    static char raid_assist_enabled = false;
    static char chest_farm_enabled = false;
    static char show_torches = false;
    
    static char is_full_moon = false;
    static char mirage_island_exists = false;
    
    static const float window_inset = 100;
    
    static Input mouse_aimbot_input = {.type = INPUT_TYPE_MOUSE_MOVE};
    
    shared_memory->FUNCTION_QUEUE_COUNT = 5;
    shared_memory->INPUT_QUEUE_COUNT = 1;
    
    
    
    
#pragma mark - Object Loops -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            long mtid_len = 0;
            //rbx_print_descendants(task, sky, 0, 0);
            char* mtid = rbx_sky_get_moontextureid(task, sky, &mtid_len);
            if (mtid)
            {
                if (strstr(mtid, fullmoon_assetid))
                {
                    int ctt = rbx_lighting_get_clocktime_tick(task, lighting_service);
                    int hour = ctt/RBX_LIGHTING_CLOCKTIME_HOUR_TICK_COUNT;
                    if (hour > 12 || hour < 5)
                    {
                        is_full_moon = true;
                    }
                    else
                    {
                        is_full_moon = false;
                    }
                }
                else
                {
                    is_full_moon = false;
                }
                vm_deallocate(mach_task_self_, (vm_address_t)mtid, mtid_len);
            }
            else
            {
                is_full_moon = false;
            }
            my_character = rbx_player_get_character(task, local_player);
            //rbx_print_descendants(task, my_character, 0, 1);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
           // rbx_cframe_t hc = rbx_basepart_get_cframe(task, my_hrp);
            //printf("%f, %f, %f", hc.pos.x, hc.pos.y, hc.pos.z);
            //rbx_print_descendants(task, my_hrp, 0, 0);
            if (my_hrp)
            {
                my_hrp_cframe_address = rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            }
            else
            {
                my_hrp_cframe_address = 0;
            }
            my_humanoid = rbx_instance_find_first_child_of_class(task, my_character, "Humanoid");
            map_model = rbx_instance_find_first_child(task, workspace, "Map");
            //rbx_print_descendants(task, workspace, 0, 1);
            floating_turtle_island = rbx_instance_find_first_child(task, map_model, "Turtle");
            floating_turtle_questtorches_model = rbx_instance_find_first_child(task, floating_turtle_island, "QuestTorches");
            if (hide_map == true)
            {
                if (map_model)
                {
                    shared_memory->FUNCTION_QUEUE[0].address = instance_set_parent_func;
                    shared_memory->FUNCTION_QUEUE[0].type = 2;
                    shared_memory->FUNCTION_QUEUE[0].async_type = FUNCTION_ASYNC_TYPE_MAIN;
                    *((vm_address_t*)(shared_memory->FUNCTION_QUEUE[0].arguments)) = map_model;
                    *((vm_address_t*)(shared_memory->FUNCTION_QUEUE[0].arguments + 8)) = badgeservice;
                    shared_memory->FUNCTION_QUEUE[0].should_execute = true;
                    shared_memory->FUNCTION_QUEUE_FINISHED = false;
                }
            }
            if (hide_map == false)
            {
                if (!map_model)
                {
                    vm_address_t __map_model = rbx_instance_find_first_child(task, badgeservice, "Map");
                    if (__map_model)
                    {
                        shared_memory->FUNCTION_QUEUE[1].address = instance_set_parent_func;
                        shared_memory->FUNCTION_QUEUE[1].type = 2;
                        shared_memory->FUNCTION_QUEUE[1].async_type = FUNCTION_ASYNC_TYPE_MAIN;
                        *((vm_address_t*)(shared_memory->FUNCTION_QUEUE[1].arguments)) = __map_model;
                        *((vm_address_t*)(shared_memory->FUNCTION_QUEUE[1].arguments + 8)) = workspace;
                        shared_memory->FUNCTION_QUEUE[1].should_execute = true;
                        shared_memory->FUNCTION_QUEUE_FINISHED = false;
                    }
                }
            }
            mystic_island = rbx_instance_find_first_child(task, map_model, "MysticIsland");
            if (mystic_island)
            {
                mirage_island_exists = true;
            }
            else
            {
                mirage_island_exists = false;
            }
            //rbx_print_descendants(task, temple_of_time, 0, 1);
            
            vm_address_t lighting_layers = rbx_instance_find_first_child(task, lighting_service, "LightingLayers");
            vm_address_t miragefog_atm = rbx_instance_find_first_child(task, lighting_layers, "MirageFog");
            vm_address_t mfinv = rbx_instance_find_first_child_of_class(task, miragefog_atm, "NumberValue"); //Intensity
            if (mfinv)
            {
                rbx_numbervalue_set_value(task, mfinv, -100000.0f);
            }
            sleep(1);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            int esp_part_index = 0;
            int fruit_index = 0;
            
            if (mystic_island)
            {
                vm_address_t part = vm_read_8byte_value(task, mystic_island + RBX_MODEL_PRIMARYPART_OFFSET);
                if (part)
                {
                    esp_part_colors[esp_part_index] = mirage_esp_color;
                    esp_parts[esp_part_index] = part;
                    bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                    memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), "Mirage Island", strlen("Mirage Island"));
                    esp_part_index++;
                }
            }
            
            long child_count = 0;
            rbx_child_t* children = rbx_instance_get_children(task, workspace, &child_count);
            if (children)
            {
                for (long i = 0 ; i < child_count ; i++)
                {
                    vm_address_t object = children[i].child_address;
                    long name_len = 0;
                    char* name = rbx_instance_get_name(task, object, &name_len);
                    //printf("%s\n", name);
                    if (name)
                    {
                        if (strstr(name, "Fruit"))
                        {
                            vm_address_t _fruit = rbx_instance_find_first_child(task, object, "Fruit");
                            if (_fruit)
                            {
                                vm_address_t handle = rbx_instance_find_first_child(task, object, "Handle");
                                esp_part_colors[esp_part_index] = fruit_esp_color;
                                esp_parts[esp_part_index] = handle;
                                fruit_models[fruit_index] = object;
                                if (name)
                                {
                                    bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                                    memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), name, name_len);
                                }
                                esp_part_index++;
                                fruit_index++;
                            }
                        }
                        if (strcmp(name, "Flower1") == 0)
                        {
                            esp_part_colors[esp_part_index] = blue_flower_esp_color;
                            esp_parts[esp_part_index] = object;
                            
                            bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                            memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), name, name_len);
                            
                            esp_part_index++;
                        }
                        if (strcmp(name, "Flower2") == 0)
                        {
                            esp_part_colors[esp_part_index] = red_flower_esp_color;
                            esp_parts[esp_part_index] = object;
                            
                            bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                            memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), name, name_len);
                            
                            esp_part_index++;
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                    }
                    usleep(2500);
                }
                vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
            }
            fruit_count = fruit_index;
            
            
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
                        long name_len = 0;
                        char* name = rbx_instance_get_name(task, object, &name_len);
                        if (name && h)
                        {
                            float sz = 35;
                            rbx_basepart_set_size(task, h, (vector3_t){.x = sz,. y = sz, .z = sz});
                            vm_address_t df = rbx_instance_find_first_child(task, object, "Data");
                            vm_address_t lvl_iv = rbx_instance_find_first_child(task, df, "Level");
                            //rbx_print_descendants(task, object, 0, 1);
                            long lvl = rbx_intvalue_get_value(task, lvl_iv);
                            //rbx_basepart_set_size(task, h, (vector3_t){.x = 17, .y = 17, .z = 17});
                            esp_part_colors[esp_part_index] = custom_haki_esp_color;
                            esp_parts[esp_part_index] = h;
                            
                            char str[MAX_ESP_TEXT_LENGTH];
                            bzero(str, MAX_ESP_TEXT_LENGTH);
                            sprintf(str, "%s [Lv. %ld]", name, lvl);
                            
                            bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                            memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), str, strlen(str));
                            
                            esp_part_index++;
                            vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
            }
            
            
            
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
                            esp_part_colors[esp_part_index] = quest_torches_esp_color;
                            esp_parts[esp_part_index] = object;
                            
                            bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                            memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), name, name_len);
                            
                            esp_part_index++;
                            vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
            }
            
            esp_part_count = esp_part_index;
            usleep(300000);
        }
    });
    
    
    
    
    
#pragma mark - Raid Assist -
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(500000);
            if (raid_assist_enabled)
            {
                long cc = 0;
                rbx_child_t* children = rbx_instance_get_children(task, enemies_folder, &cc);
                if (children)
                {
                    for (long i = 0 ; i < cc ; i++)
                    {
                        vm_address_t enemy = children[i].child_address;
                        vm_address_t humanoid = rbx_instance_find_first_child_of_class(task, enemy, "Humanoid");
                        if (humanoid)
                        {
                            while (shared_memory->FUNCTION_QUEUE[2].should_execute == true) { usleep(10000); }
                            shared_memory->FUNCTION_QUEUE[2].address = humanoid_set_health_func;
                            shared_memory->FUNCTION_QUEUE[2].type = 7;
                            shared_memory->FUNCTION_QUEUE[2].async_type = FUNCTION_ASYNC_TYPE_MAIN;
                            *((vm_address_t*)(shared_memory->FUNCTION_QUEUE[2].arguments)) = humanoid;
                            *((float*)(shared_memory->FUNCTION_QUEUE[2].arguments + 8)) = 0.0f;
                            shared_memory->FUNCTION_QUEUE[2].should_execute = true;
                            shared_memory->FUNCTION_QUEUE_FINISHED = false;
                            while (shared_memory->FUNCTION_QUEUE[2].should_execute == true) { usleep(10000); }
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, cc * sizeof(rbx_child_t));
                }
            }
        }
    });
    
    
    
    
    
#pragma mark - Chest Collect -
    
    
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (chest_farm_enabled)
            {
                float old_dist = 20000.0f;
                vm_address_t __closest_chest = 0;
                char __ce = false;
                
                long workspace_child_count = 0;
                rbx_child_t* workspace_children = rbx_instance_get_children(task, workspace, &workspace_child_count);
                if (workspace_children)
                {
                    for (long i = 0 ; i < workspace_child_count ; i++)
                    {
                        vm_address_t object = workspace_children[i].child_address;
                        if (object)
                        {
                            long object_name_len = 0;
                            char* object_name = rbx_instance_get_name(task, object, &object_name_len);
                            char is_chest = false;
                            if (object_name)
                            {
                                if (strcmp(object_name, "Chest3") == 0)
                                {
                                    is_chest = true;
                                }
                                if (strcmp(object_name, "Chest2") == 0)
                                {
                                    is_chest = true;
                                }
                                if (strcmp(object_name, "Chest1") == 0)
                                {
                                    is_chest = true;
                                }
                                vm_deallocate(mach_task_self_, (vm_address_t)object_name, object_name_len);
                            }
                            if (is_chest && my_hrp)
                            {
                                rbx_cframe_t object_cframe = rbx_basepart_get_cframe(task, object);
                                rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
                                float dist = vector3_dist_dif(my_hrp_cframe.pos, object_cframe.pos);
                                if (dist < old_dist)
                                {
                                    old_dist = dist;
                                    __ce = true;
                                    __closest_chest = object;
                                }
                            }
                        }
                        usleep(100);
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)workspace_children, workspace_child_count * sizeof(rbx_child_t));
                }
                closest_chest = __closest_chest;
                usleep(10000);
            }
            else
            {
                sleep(1);
            }
        }
    });
    
    
    
    
   
    
#pragma mark - Aimbot -
    
    
    
    
    
    
    
    static int cframe_write_mode = 0;
    static int cframe_write_usleep_time = 1000;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            /*if (cframe_write_mode == 1)
            {
                vm_address_t __chtmca = closest_hrp_to_mouse_cframe_address;
                vm_address_t __mhca = my_hrp_cframe_address;
                if (__mhca && __chtmca)
                {
                    vm_copy(task, __chtmca, sizeof(rbx_cframe_t), __mhca);
                }
            }
            if (cframe_write_mode == 2)
            {
                vm_address_t __chtmca = closest_hrp_to_mouse_cframe_address;
                vm_address_t __mhca = my_hrp_cframe_address;
                if (__mhca && __chtmca)
                {
                    rbx_cframe_t cf = vm_read_rbx_cframe_value(task, __chtmca);
                    cf.pos.y -= 1;
                    vm_write(task, __mhca, (vm_address_t)&cf, sizeof(cf));
                }
            }*/
            if (cframe_write_mode == 3)
            {
                if (closest_hrp_to_mouse)
                {
                vm_address_t __chtmca = closest_hrp_to_mouse_cframe_address;
                vm_address_t __mhca = my_hrp_cframe_address;
                if (__mhca && __chtmca)
                {
                    rbx_cframe_t cf = vm_read_rbx_cframe_value(task, __chtmca);
                    rbx_cframe_t ccf = rbx_camera_get_cframe(task, camera);
                    cf.pos = vector3_add(cf.pos, vector3_mul_num(rbx_cframe_get_look_vector(ccf), -5));
                    vm_write(task, __mhca, (vm_address_t)&cf, sizeof(cf));
                }
                }
            }
            usleep(cframe_write_usleep_time);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (aimbot_enabled)
            {
                long child_count = 0;
                rbx_child_t* children = rbx_instance_get_children(task, players_service, &child_count);
                float __old_dist_from_mouse = aimbot_pixel_radius;
                vm_address_t __old_target_hrp = 0;
                char __target_exists = false;
                if (children)
                {
                    for (long i = 0 ; i < child_count ; i++)
                    {
                        vm_address_t player = children[i].child_address;
                        if (player && player != local_player)
                        {
                            vm_address_t character = rbx_player_get_character(task, player);
                            vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
                            if (hrp)
                            {
                                rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
                                rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, hrp);
                                if (vector3_dist_dif(my_hrp_cframe.pos, hrp_cframe.pos) < aimbot_max_dist)
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
                                                __old_dist_from_mouse = dist_from_mouse;
                                                __old_target_hrp = hrp;
                                                __target_exists = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
                closest_hrp_to_mouse = __old_target_hrp;
                if (closest_hrp_to_mouse)
                {
                closest_hrp_to_mouse_cframe_address = rbx_basepart_get_properties_address(task, closest_hrp_to_mouse) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                }
                else
                {
                    closest_hrp_to_mouse_cframe_address = 0;
                }
                //rbx_print_descendants(task, __old_target_hrp, 0, 0);
                //printf("%f\n", __old_dist_from_center);
            }
            usleep(200000);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (aimbot_enabled)
            {
                if (closest_hrp_to_mouse)
                {
                    unsigned char* keys_down = shared_memory->KEYS_DOWN;
                    if (keys_down['z'] ||
                        keys_down['x'] ||
                        keys_down['c'] ||
                        keys_down['v'])
                    {
                        
                        iut = 10;
                        shared_memory->INPUT_USLEEP_TIME = iut;
                        
                        rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, closest_hrp_to_mouse);
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        float camera_fov = rbx_camera_get_field_of_view(task, camera);
                        float window_w = shared_memory->WINDOW_W;
                        float window_h = shared_memory->WINDOW_H;
                        custom_rbx_world_to_screen_info_t wtsi = rbx_world_to_screen_point(camera_cframe, camera_fov, hrp_cframe.pos, window_w, window_h, 1, 1);
                        if (wtsi.x > window_inset && wtsi.x < (window_w - window_inset))
                        {
                            if (wtsi.y > window_inset && wtsi.y < (window_h - window_inset))
                            {
                                mouse_aimbot_input.x = wtsi.x;
                                mouse_aimbot_input.y = wtsi.y;
                                mouse_aimbot_input.should_execute = true;
                                shared_memory->INPUT_QUEUE[0] = mouse_aimbot_input;
                                shared_memory->INPUT_QUEUE_FINISHED = false;
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
                iut = 100000;
                shared_memory->INPUT_USLEEP_TIME = iut;
                sleep(1);
            }
        }
    });
    
    
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
                        cframe_write_mode = 3;
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
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            /*long child_count = 0;
            rbx_child_t* children = rbx_instance_get_children(task, my_character, &child_count);
            if (children)
            {
                for (long i = 0 ; i < child_count ; i++)
                {
                    vm_address_t object = children[i].child_address;
                    if (rbx_instance_is_a(task, object, "Part") ||
                        rbx_instance_is_a(task, object, "MeshPart"))
                    {
                        rbx_basepart_set_cancollide(task, object, false);
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
            }*/
            int esp_index = 0;
            int __esp_part_count = esp_part_count;
            if (__esp_part_count > 0 && __esp_part_count < MAX_ESP_COUNT)
            {
                for (int i = 0 ; i < __esp_part_count ; i++)
                {
                    vm_address_t part = esp_parts[i];
                    if (part)
                    {
                        vm_address_t parent = rbx_instance_get_parent(task, part);
                        vm_address_t instance_self = rbx_instance_get_self(task, part);
                        //rbx_print_descendants(task, instance_self, 0, 0);
                        if (parent && instance_self == part)
                        {
                            rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            rbx_cframe_t part_cframe = rbx_basepart_get_cframe(task, part);
                            float dist = vector3_dist_dif(part_cframe.pos, my_hrp_cframe.pos);
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            ESP_Color esp_color = esp_part_colors[i];
                            if (part == closest_hrp_to_mouse)
                            {
                                if (dist < 100)
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
                                esp_color.a = 0.75;
                            }
                            float window_w = shared_memory->WINDOW_W;
                            float window_h = shared_memory->WINDOW_H;
                            ESP_Frame scaled_frame = rbx_draw_esp_box_on_3d_point(task, part_cframe.pos, camera_cframe, shared_memory, 2, esp_color, fov, 20, 20, 0, 0, window_w, window_h, esp_index, false);
                            esp_index++;
                            rbx_draw_esp_box_on_3d_point(task, part_cframe.pos,
                                             camera_cframe, shared_memory, 0,
                                             esp_color, fov, 500, 40, 0, scaled_frame.h + 10, window_w, window_h, esp_index, false);
                            char* part_name = esp_part_names + (MAX_ESP_TEXT_LENGTH * i);
                            char text[MAX_ESP_TEXT_LENGTH];
                            bzero(text, MAX_ESP_TEXT_LENGTH);
                            sprintf(text, "%s [%.0f]", part_name, dist);
                            //vm_write(task, dylib_esp_box_text_array + (esp_index * MAX_ESP_TEXT_LENGTH), (vm_address_t)text, MAX_ESP_TEXT_LENGTH);
                            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
                            esp_index++;
                        }
                    }
                    usleep(esput/__esp_part_count);
                }
                if (is_full_moon == true)
                {
                    ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 200, .w = shared_memory->WINDOW_W, .h = 40};
                    shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
                    char text[MAX_ESP_TEXT_LENGTH];
                    bzero(text, MAX_ESP_TEXT_LENGTH);
                    sprintf(text, "%s", "FULL MOON");
                    strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
                    shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
                    shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = (ESP_Color){.r = 1, .g = 0, .b = 0, .a = 1};
                    shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
                    esp_index++;
                }
                if (mirage_island_exists == true)
                {
                    ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 240, .w = shared_memory->WINDOW_W, .h = 40};
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
                memset(shared_memory->ESP_BOX_HIDDEN_ARRAY + esp_index, 1, MAX_ESP_COUNT - esp_index);
            }
            else
            {
                usleep(esput);
            }
        }
    });
    
    
    
    
    
    
#pragma mark - Fly -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(500);
            if (chest_farm_enabled)
            {
                vel = vector3_unit(rbx_basepart_get_cframe(task, closest_chest).pos, rbx_basepart_get_cframe(task, my_hrp).pos);
                vel = vector3_mul_num(vel, fly_speed);
                rbx_basepart_set_velocity(task, my_hrp, vel);
                rbx_basepart_set_gravity(task, my_hrp, 0.0f);
                
            }
            else
            {
                if (fly_enabled)
                {
                    
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
                    
                    rbx_basepart_set_velocity(task, my_hrp, vel);
                    rbx_basepart_set_gravity(task, my_hrp, 0.0f);
                }
                else
                {
                    rbx_basepart_set_gravity(task, my_hrp, 100.0f);
                }
            }
        }
    });
    
    
    
    
    
    
    
#pragma mark - Fruit Cycle -
    
    
    
    
    
    
    
    static char should_cycle_through_fruits = false;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (should_cycle_through_fruits)
            {
                sleep(2); //Give some time to switch back to the Roblox window.
                for (int i = 0 ; i < fruit_count ; i++)
                {
                    vm_address_t fm = fruit_models[i]; //fruit_model
                    long fmnl = 0; //fruit_model_name_length
                    char* fmn = rbx_instance_get_name(task, fm, &fmnl); //fruit_model_name
                    if (fmn)
                    {
                        if (strcmp(fmn, "Fruit ") == 0) //Fruits that have just spawned are named this.
                        {
                            rbx_camera_set_camera_subject(task, camera, fm);
                            //rbx_print_descendants(task, fm, 0, 2);
                            sleep(3);
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)fmn, fmnl);
                    }
                }
                rbx_camera_set_camera_subject(task, camera, my_humanoid);
                should_cycle_through_fruits = false;
            }
            usleep(100000);
        }
    });
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    
    
    
    
    printf("List of stdin commands:\n");
    printf("\tfly\n");
    printf("\tflyspeed [(float)]\n");
    printf("\tnofly\n");
    printf("\taimbot\n");
    printf("\tnoaimbot\n");
    printf("\tshowfruits\n");
    printf("\tshowmap\n");
    printf("\thidemap\n");
    printf("\tshowtorches\n");
    printf("\thidetorches\n");
    printf("\traidassist\n");
    printf("\tnoraidassist\n");
    printf("\tchestfarm\n");
    printf("\tnochestfarm\n");
    printf("\tquit\n");
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            char str[100];
            bzero(str, 100);
            fgets(str, 100, stdin);
            
            if (strcmp(str, "showfruits\xa") == 0)
            {
                printf("CYCLING CAMERA THROUGH FRUITS...\n");
                should_cycle_through_fruits = true;
            }
            if (strcmp(str, "fly\xa") == 0)
            {
                rbx_smooth_fly(task, true);
                fly_enabled = true;
                printf("ENABLED FLY\n");
            }
            if (strncmp(str, "flyspeed ", strlen("flyspeed ")) == 0)
            {
                sscanf(str, "flyspeed %f", &fly_speed);
                printf("SET FLY SPEED TO %f\n", fly_speed);
            }
            if (strcmp(str, "nofly\xa") == 0)
            {
                rbx_smooth_fly(task, false);
                fly_enabled = false;
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
            if (strcmp(str, "showmap\xa") == 0)
            {
                hide_map = false;
                printf("SHOWING MAP...\n");
            }
            if (strcmp(str, "hidemap\xa") == 0)
            {
                hide_map = true;
                printf("HIDING MAP...\n");
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
            if (strcmp(str, "chestfarm\xa") == 0)
            {
                chest_farm_enabled = true;
                printf("ENABLED CHEST FARM\n");
            }
            if (strcmp(str, "nochestfarm\xa") == 0)
            {
                chest_farm_enabled = false;
                printf("DISABLED CHEST FARM\n");
            }
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
    
}

