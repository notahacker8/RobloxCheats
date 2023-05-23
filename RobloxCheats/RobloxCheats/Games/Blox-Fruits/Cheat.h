


void blox_fruits_cheat(task_t task)
{
    printf("- BLOX FRUITS (ESP) -\n");
    
    void* dlhandle = dlopen(__LIBESP_DYLIB_PATH__, RTLD_NOW);
    if (!dlhandle)
    {
        printf("%s\n", dlerror());
    }
    vm_address_t s_load_address = task_get_image_address_by_path(mach_task_self_, __LIBESP_DYLIB_PATH__);
    
    vm_offset_t left_mouse_down_offset = gdso(dlhandle, s_load_address, "LEFT_MOUSE_DOWN");
    vm_offset_t keys_down_offset = gdso(dlhandle, s_load_address, "KEYS_DOWN");
    vm_offset_t esp_count_offset = gdso(dlhandle, s_load_address, "ESP_COUNT");
    vm_offset_t esp_enabled_offset = gdso(dlhandle, s_load_address, "ESP_ENABLED");
    vm_offset_t window_w_offset = gdso(dlhandle, s_load_address, "WINDOW_W");
    vm_offset_t window_h_offset = gdso(dlhandle, s_load_address, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = gdso(dlhandle, s_load_address, "ESP_USLEEP_TIME");
    
    vm_offset_t function_queue_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE");
    vm_offset_t function_queue_count_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE_COUNT");
    vm_offset_t function_queue_finished_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE_FINISHED");
    vm_offset_t function_usleep_time_offset = gdso(dlhandle, s_load_address, "FUNCTION_USLEEP_TIME");
    
    vm_offset_t esp_box_hidden_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_box_color_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_COLOR_ARRAY");
    vm_offset_t esp_box_text_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_TEXT_ARRAY");
    vm_offset_t esp_box_border_width_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_BORDER_WIDTH_ARRAY");
    
    static int fruit_count = 0;
    static vm_address_t fruit_models[MAX_ESP_COUNT];
    
    static ESP_Color sea_beast_esp_color = {.r = 1, .g = 0, .b = 0.8, .a = 1};
    static ESP_Color fruit_esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    static ESP_Color red_flower_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color blue_flower_esp_color = {.r = 0, .g = 0, .b = 1, .a = 1};
    static ESP_Color apple_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color banana_esp_color = {.r = 1, .g = 1, .b = 0, .a = 1};
    static ESP_Color pineapple_esp_color = {.r = 0.5, .g = 1, .b = 0, .a = 1};
    
    static int esp_part_count = 0;
    static ESP_Color esp_part_colors[MAX_ESP_COUNT];
    static char esp_part_names[MAX_ESP_COUNT * MAX_ESP_TEXT_LENGTH];
    static vm_address_t esp_parts[MAX_ESP_COUNT];
    
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    if (!load_address)
    {
        printf("Couldn't find libESP.dylib in task %d\n", task);
        exit(0);
    }
    
    char esp_enabled = true;
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, (int)sizeof(char));
    int esp_count = 20;
    vm_write(task, load_address + esp_count_offset, (vm_offset_t)&esp_count, (int)sizeof(int));
    
    vm_address_t esp_box_text_array = load_address + esp_box_text_array_offset;
    vm_address_t esp_box_hidden_array = load_address + esp_box_hidden_array_offset;
    vm_address_t esp_box_frame_array = load_address + esp_box_frame_array_offset;
    vm_address_t esp_box_color_array = load_address + esp_box_color_array_offset;
    vm_address_t esp_box_border_width_array = load_address + esp_box_border_width_array_offset;
    
    useconds_t esput = 1000;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    static char is_w_pressed = false;
    static char is_a_pressed = false;
    static char is_s_pressed = false;
    static char is_d_pressed = false;

    static float window_w;
    static float window_h;
    
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    useconds_t fut = 100000;
    vm_write(task, load_address + function_usleep_time_offset, (vm_address_t)&fut, sizeof(useconds_t));
    
    vm_address_t instance_set_parent_func = task_base_address + RBX_INSTANCE_PARENT_SETTER_METHOD_OFFSET;
    
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t badgeservice = rbx_instance_find_first_child_of_class(task, game, "BadgeService");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t lighting_service = rbx_instance_find_first_child_of_class(task, game, "Lighting");
    vm_address_t replicatedstorage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t npcs_folder = rbx_instance_find_first_child(task, workspace, "NPCs");
    vm_address_t enemies_folder = rbx_instance_find_first_child(task, workspace, "Enemies");
    vm_address_t seabeasts_folder = rbx_instance_find_first_child(task, workspace, "SeaBeasts");
    vm_address_t seaevents_folder = rbx_instance_find_first_child(task, workspace, "SeaEvents");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    //rbx_print_descendants(task, npcs_folder, 0, 1);
    //[Model] Master of Auras (0x7faeed9661a0) (child count: 23)
    //rbx_print_descendants(task, lighting_service, 0, 3);
    //rbx_print_descendants(task, game, 0, 1);
    //rbx_print_descendants(task, map_model, 0, 1);
    
    //[Model] MysticIsland (0x7fa65a156080) (child count: 220)
    
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static vm_address_t map_model = 0 ;
    
    static vm_address_t mfinv = 0;
    static vm_address_t character = 0;
    static vm_address_t humanoid = 0;
    static vm_address_t hrp = 0;
    static vm_address_t mystic_island = 0;
    static vm_address_t proxy_hrps[100];
    static int proxy_hrp_count = 0;
    static vector3_t vel;
    static float fly_speed = 275.0f;
    
    static char fly_enabled = true;
    static float npc_aura_dist = 100.0f;
    static float npc_aura_forward_dist = 42.0f;
    static char npc_aura_enabled = false;
    static char hide_map = false;
    
    
    
    
    
    
    
    
#pragma mark - Object Loops -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            rbx_player_set_last_input_timestamp(task, local_player, 999999999); //Anti-Afk
            
            window_w = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_w_offset))).f;
            window_h = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_h_offset))).f;
            character = rbx_player_get_character(task, local_player);
            hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
            //rbx_cframe_t hrpcf = rbx_basepart_get_cframe(task, hrp);
            //printf("%f, %f, %f\n", hrpcf.pos.x, hrpcf.pos.y, hrpcf.pos.z);
            humanoid = rbx_instance_find_first_child_of_class(task, character, "Humanoid");
            map_model = rbx_instance_find_first_child(task, workspace, "Map");
            if (hide_map == true)
            {
                if (map_model)
                {
                    RemoteFunctionCall remote_call = {.address = instance_set_parent_func, .type = 2, .finished = false};
                    ((vm_address_t*)(remote_call.arguments))[0] = map_model;
                    ((vm_address_t*)(remote_call.arguments + 8))[0] = badgeservice;
                    
                    int count = 1;
                    char finished = 0;
                    vm_write(task, load_address + function_queue_offset, (vm_address_t)&remote_call, sizeof(RemoteFunctionCall));
                    vm_write(task, load_address + function_queue_count_offset, (vm_address_t)&count, sizeof(int));
                    vm_write(task, load_address + function_queue_finished_offset, (vm_address_t)&finished, sizeof(char));
                    wait_until_queue_finished(task, load_address + function_queue_finished_offset, 1000);
                }
            }
            if (hide_map == false)
            {
                if (!map_model)
                {
                    vm_address_t __map_model = rbx_instance_find_first_child(task, badgeservice, "Map");
                    if (__map_model)
                    {
                        RemoteFunctionCall remote_call = {.address = instance_set_parent_func, .type = 2, .finished = false};
                        ((vm_address_t*)(remote_call.arguments))[0] = __map_model;
                        ((vm_address_t*)(remote_call.arguments + 8))[0] = workspace;
                        
                        int count = 1;
                        char finished = 0;
                        vm_write(task, load_address + function_queue_offset, (vm_address_t)&remote_call, sizeof(RemoteFunctionCall));
                        vm_write(task, load_address + function_queue_count_offset, (vm_address_t)&count, sizeof(int));
                        vm_write(task, load_address + function_queue_finished_offset, (vm_address_t)&finished, sizeof(char));
                        wait_until_queue_finished(task, load_address + function_queue_finished_offset, 1000);
                    }
                }
            }
            mystic_island = rbx_instance_find_first_child(task, map_model, "MysticIsland");
            
            vm_address_t lighting_layers = rbx_instance_find_first_child(task, lighting_service, "LightingLayers");
            vm_address_t miragefog_atm = rbx_instance_find_first_child(task, lighting_layers, "MirageFog");
            mfinv = rbx_instance_find_first_child_of_class(task, miragefog_atm, "NumberValue");
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
                    esp_part_colors[esp_part_index] = sea_beast_esp_color;
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
                        if (strstr(name, "Spawner"))
                        {
                            vm_address_t banana_tool = rbx_instance_find_first_child(task, object, "Banana");
                            vm_address_t apple_tool = rbx_instance_find_first_child(task, object, "Apple");
                            vm_address_t pineapple_tool = rbx_instance_find_first_child(task, object, "Pineapple");
                            if (banana_tool)
                            {
                                vm_address_t handle = rbx_instance_find_first_child(task, banana_tool, "Handle");
                                if (handle)
                                {
                                    esp_part_colors[esp_part_index] = banana_esp_color;
                                    esp_parts[esp_part_index] = handle;
                                    bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                                    memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), "Banana", strlen("Banana"));
                                    esp_part_index++;
                                }
                            }
                            if (apple_tool)
                            {
                                vm_address_t handle = rbx_instance_find_first_child(task, apple_tool, "Handle");
                                if (handle)
                                {
                                    esp_part_colors[esp_part_index] = apple_esp_color;
                                    esp_parts[esp_part_index] = handle;
                                    bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                                    memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), "Apple", strlen("Apple"));
                                    esp_part_index++;
                                }
                            }
                            if (pineapple_tool)
                            {
                                vm_address_t handle = rbx_instance_find_first_child(task, pineapple_tool, "Handle");
                                if (handle)
                                {
                                    esp_part_colors[esp_part_index] = pineapple_esp_color;
                                    esp_parts[esp_part_index] = handle;
                                    bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                                    memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), "Pineapple", strlen("Pineapple"));
                                    esp_part_index++;
                                }
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
                    usleep(1000);
                }
                vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
            }
            fruit_count = fruit_index;
            esp_part_count = esp_part_index;
            usleep(100000);
        }
    });
    
    
    
    
    
    
#pragma mark - Proxy Enemy Loop -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (npc_aura_enabled)
            {
                int proxy_hrp_index = 0;
                
                long child_count = 0;
                rbx_child_t* children = rbx_instance_get_children(task, enemies_folder, &child_count);
                if (children)
                {
                    for (long i = 0 ; i < child_count ; i++)
                    {
                        vm_address_t object = children[i].child_address;
                        vm_address_t enemy_hrp = rbx_instance_find_first_child(task, object, "HumanoidRootPart");
                        if (enemy_hrp)
                        {
                            rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, hrp);
                            rbx_cframe_t enemy_hrp_cframe = rbx_basepart_get_cframe(task, enemy_hrp);
                            if (vector3_dist_dif(my_hrp_cframe.pos, enemy_hrp_cframe.pos) < npc_aura_dist)
                            {
                                proxy_hrps[proxy_hrp_index++] = enemy_hrp;
                            }
                        }
                        usleep(100);
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
                proxy_hrp_count = proxy_hrp_index;
            }
            usleep(100000);
        }
    });
    
    
    static useconds_t kill_aura_usleep_time = 100;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (npc_aura_enabled)
            {
                if (proxy_hrp_count > 0)
                {
                    char lmd = vm_read_1byte_value(task, load_address + left_mouse_down_offset);
                    if (lmd)
                    {
                        kill_aura_usleep_time = 10;
                        //rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, hrp);
                        rbx_cframe_t new_cframe = my_hrp_cframe;
                        new_cframe.pos = vector3_add(new_cframe.pos, vector3_mul_num(rbx_cframe_get_look_vector(my_hrp_cframe), npc_aura_forward_dist));
                        for (int i = 0 ; i < proxy_hrp_count ; i++)
                        {
                            if (i < 100)
                            {
                                rbx_basepart_set_cframe(task, proxy_hrps[i], new_cframe);
                            }
                        }
                    }
                    else
                    {
                        kill_aura_usleep_time = 100;
                    }
                }
                else
                {
                    kill_aura_usleep_time = 1000;
                }
            }
            else
            {
                kill_aura_usleep_time = 10000;
            }
            usleep(kill_aura_usleep_time);
        }
    });
    
    
    
    
    
#pragma mark - ESP -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (mfinv)
            {
                rbx_numbervalue_set_value(task, mfinv, 0.0f);
            }
            int esp_index = 0;
            if (esp_part_count > 0 && esp_part_count < MAX_ESP_COUNT)
            {
                for (int i = 0 ; i < esp_part_count ; i++)
                {
                    vm_address_t part = esp_parts[i];
                    if (part)
                    {
                        vm_address_t parent = rbx_instance_get_parent(task, part);
                        vm_address_t instance_self = rbx_instance_get_self(task, part);
                        //rbx_print_descendants(task, instance_self, 0, 0);
                        if (parent && instance_self == part)
                        {
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            ESP_Color esp_color = esp_part_colors[i];
                            rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, hrp);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            rbx_cframe_t part_cframe = rbx_basepart_get_cframe(task, part);
                            ESP_Frame scaled_frame = rbx_draw_esp_box(task, part_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, 2, esp_color, fov, 20, 20, 0, 0, window_w, window_h, esp_index, false);
                            esp_index++;
                            rbx_draw_esp_box(task, part_cframe.pos,
                                             camera_cframe, esp_box_hidden_array,
                                             esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, 0,
                                             esp_color, fov, 200, 40, 0, scaled_frame.h + 10, window_w, window_h, esp_index, false);
                            char* part_name = esp_part_names + (MAX_ESP_TEXT_LENGTH * i);
                            char text[MAX_ESP_TEXT_LENGTH];
                            bzero(text, MAX_ESP_TEXT_LENGTH);
                            sprintf(text, "%s [%.0f]", part_name, vector3_dist_dif(part_cframe.pos, hrp_cframe.pos));
                            vm_write(task, esp_box_text_array + (esp_index * MAX_ESP_TEXT_LENGTH), (vm_address_t)text, MAX_ESP_TEXT_LENGTH);
                            esp_index++;
                        }
                    }
                    usleep((esput)/(esp_part_count + 1));
                }
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
            usleep(1000);
            if (fly_enabled)
            {
                is_w_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 'w');
                is_a_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 'a');
                is_s_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 's');
                is_d_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 'd');
                
                rbx_cframe_t cf = rbx_camera_get_cframe(task, camera);
                vector3_t lv = rbx_cframe_get_look_vector(cf);
                vector3_t rv = rbx_cframe_get_right_vector(cf);
                
                bzero(&vel, sizeof(vector3_t));
                
                if (is_w_pressed)
                {
                    vel = vector3_add(vector3_mul_num(lv, 1), vel);
                }
                if (is_a_pressed)
                {
                    vel = vector3_add(vector3_mul_num(rv, -1), vel);
                }
                if (is_s_pressed)
                {
                    vel = vector3_add(vector3_mul_num(lv, -1), vel);
                }
                if (is_d_pressed)
                {
                    vel = vector3_add(vector3_mul_num(rv, 1), vel);
                }
                float magnitude = vector3_magnitude(vel);
                if (!isnan(magnitude) && magnitude > 0.0f)
                {
                    vel = vector3_div_num(vel,  magnitude);
                }
                vel = vector3_mul_num(vel, fly_speed);
            }
        }
    });
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (fly_enabled)
            {
                rbx_basepart_set_gravity(task, hrp, 0.0f);
                rbx_basepart_set_velocity(task, hrp, vel);
            }
            else
            {
                rbx_basepart_set_gravity(task, hrp, 200.0f);
            }
            usleep(500);
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
                sleep(2);
                for (int i = 0 ; i < fruit_count ; i++)
                {
                    vm_address_t fm = fruit_models[i]; //fruit_model
                    long fmnl = 0; //fruit_model_name_length
                    char* fmn = rbx_instance_get_name(task, fm, &fmnl); //fruit_model_name
                    if (fmn)
                    {
                        if (strcmp(fmn, "Fruit ") == 0) //Fruits that have no explicit name are named like this.
                        {
                            rbx_camera_set_camera_subject(task, camera, fm);
                            sleep(3);
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)fmn, fmnl);
                    }
                }
                rbx_camera_set_camera_subject(task, camera, humanoid);
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
    printf("\tnpcaura\n");
    printf("\tnpcauraforwarddist [(float)]\n");
    printf("\tnonpcaura\n");
    printf("\tshowfruits\n");
    printf("\tshowmap\n");
    printf("\thidemap\n");
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
                fly_enabled = false;
                printf("DISABLED FLY\n");
            }
            if (strcmp(str, "npcaura\xa") == 0)
            {
                npc_aura_enabled = true;
                printf("ENABLED NPC AURA\n");
            }
            if (strncmp(str, "npcauraforwarddist ", strlen("npcauraforwarddist ")) == 0)
            {
                sscanf(str, "npcauraforwarddist %f", &npc_aura_forward_dist);
                printf("SET NPC AURA FORWARD DISTANCE TO %f\n", npc_aura_forward_dist);
            }
            if (strcmp(str, "nonpcaura\xa") == 0)
            {
                npc_aura_enabled = false;
                printf("DISABLED NPC AURA\n");
            }
            if (strcmp(str, "showmap\xa") == 0)
            {
                hide_map = false;
                printf("SHOWING MAP....\n");
            }
            if (strcmp(str, "hidemap\xa") == 0)
            {
                hide_map = true;
                printf("HIDING MAP...\n");
            }
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
    
}

