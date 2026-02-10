

#pragma mark - GENERIC -


void generic_cheat(task_t task, char use_presets)
{
    int pid = -1;
    pid_for_task(task, &pid);
    
    printf("\n- GENERIC CHEAT FOR GENERAL USE -\n");
    LIBESP_DYLIB_SETUP
    GENERIC_CHEAT_STARTER
    ANTI_AFK
    anti_afk_enabled = false; //too lazy to update offset, also the game crashes after you leave if it's not updated.
    EXIT_IF_TASK_TERMINATED
    
    long placeid = rbx_get_placeid(task);
    
    //General descendant search
    static char esp_objs_1_appended_names[MAX_ESP_TEXT_LENGTH * 1000];
    static vm_address_t esp_objs_1_parts[1000];
    static long esp_objs_1_count = 0;
    
    
    //Custom
    static char esp_objs_2_appended_names[MAX_ESP_TEXT_LENGTH * 1000];
    static vm_address_t esp_objs_2_parts[1000];
    static ESP_Color esp_objs_2_colors[1000];
    static long esp_objs_2_count = 0;
    
    
    static char auto_search = false;
    static int obj_search_include_click_detectors = true;
    static int obj_search_include_touch_interest = true;
    static int obj_search_include_proximity_prompts = true;
    static float obj_search_dist_limit = 9999; //unused.
    static int obj_search_use_dist_limit = false;
    
    static char camera_aimbot_aiming_override = false; //if we want to aim without putting the left mouse down.
    static char camera_aimbot_enabled = false;
    static float aimbot_size = 20;
    static char aimbot_do_hp_check = true;
    static float max_aimbot_dist = 9999;
    
    static char hitboxes_enabled = false;
    
    static vm_address_t ignore_teams[100];
    static int ignore_teams_count = 0;
    
    static vm_address_t include_teams[100];
    static int include_teams_count = 0;
    
    static ESP_Color target_color = {.r = 1, .g = 0, .b = 1, .a = 1};
    static vm_address_t closest_head = 0;
    
    static char character_part_name[200];
    strcpy(character_part_name, "HumanoidRootPart");
    
    static char obj_filter_ingore_containing_str[200][200];
    static int ofs_ignore_count = 0;  //ofs = object filter string
    static char obj_filter_include_containing_str[200][200];
    static int ofs_include_count = 0;
    
    
#pragma mark Object Search
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            long esp_objs_1_index = 0;
            if (auto_search)
            {
                FOR_EACH_DESCENDANT(task, workspace, 10000000, 100, i, {
                    vm_address_t pointee_offset = rbx_instance_get_pointee_offset(task, obj, task_base_address);
                    if ((pointee_offset == RBX_OBJECT_ClickDetector_POINTEE_OFFSET && obj_search_include_click_detectors) ||
                        (pointee_offset == RBX_OBJECT_TouchTransmitter_POINTEE_OFFSET && obj_search_include_touch_interest) ||
                        (pointee_offset == RBX_OBJECT_ProximityPrompt_POINTEE_OFFSET && obj_search_include_proximity_prompts))
                    {
                        if (pointee_offset == RBX_OBJECT_ProximityPrompt_POINTEE_OFFSET && obj_search_include_proximity_prompts)
                        {
                            char text[MAX_ESP_TEXT_LENGTH];
                            bzero(text, sizeof(text));
                            
                            long at_l = 0;
                            char* at = rbx_proximityprompt_get_actiontext(task, obj, &at_l);
                            if (at)
                            {
                                strcat(text, at);
                                vm_deallocate(mach_task_self_, (vm_address_t)at, at_l);
                            }
                            long ot_l = 0;
                            char* ot = rbx_proximityprompt_get_objecttext(task, obj, &ot_l);
                            if (ot)
                            {
                                strcat(text, " \"");
                                strcat(text, ot);
                                strcat(text, "\"");
                                vm_deallocate(mach_task_self_, (vm_address_t)ot, ot_l);
                            }
                            sprintf(esp_objs_1_appended_names + (esp_objs_1_index * MAX_ESP_TEXT_LENGTH), "{%s}", text);
                        }
                        else
                        {
                            bzero(esp_objs_1_appended_names + (esp_objs_1_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                        }
                        
                        vm_address_t obj_p = rbx_instance_get_parent(task, obj);
                        vm_address_t parent_pointee_offset = vm_read_uint64_t_value(task, obj_p) - task_base_address;
                        
                        
                        
                        if (rbx_pointee_offset_is_a_basepart(parent_pointee_offset))
                        {
                            if (!(obj_search_use_dist_limit && !(distance_between_baseparts(task, obj_p, my_hrp) < obj_search_dist_limit)))
                            {
                                if (esp_objs_1_index < sizeof(esp_objs_1_parts)/8)
                                {
                                    esp_objs_1_parts[esp_objs_1_index++] = obj_p;
                                }
                            }
                        }
                        else
                        {
                            if (parent_pointee_offset == RBX_OBJECT_Model_POINTEE_OFFSET || parent_pointee_offset == RBX_OBJECT_Tool_POINTEE_OFFSET)
                            {
                                char text[MAX_ESP_TEXT_LENGTH];
                                bzero(text, sizeof(text));
                                long nm_l = 0;
                                char* nm = rbx_instance_get_name(task, obj_p, &nm_l);
                                if (nm)
                                {
                                    sprintf(esp_objs_1_appended_names + (esp_objs_1_index * MAX_ESP_TEXT_LENGTH), "%s -> [%s]",
                                            esp_objs_1_appended_names + (esp_objs_1_index * MAX_ESP_TEXT_LENGTH), nm);
                                    vm_deallocate(mach_task_self_, (vm_address_t)nm, nm_l);
                                }
                                
                                vm_address_t pp = rbx_model_get_primarypart(task, obj_p);
                                if (rbx_instance_is_valid_child(task, pp))
                                {
                                    if (!(obj_search_use_dist_limit && !(distance_between_baseparts(task, pp, my_hrp) < obj_search_dist_limit)))
                                    {
                                        if (esp_objs_1_index < sizeof(esp_objs_1_parts)/8)
                                        {
                                            esp_objs_1_parts[esp_objs_1_index++] = pp;
                                        }
                                    }
                                }
                                else
                                {
                                    vm_address_t __part = 0;
                                    FOR_EACH_CHILD(obj_p,  x, {
                                        if (rbx_instance_is_a_basepart(task, child, task_base_address))
                                        {
                                            __part = child;
                                            x = child_count;
                                        }
                                    })
                                    if (!(obj_search_use_dist_limit && !(distance_between_baseparts(task, __part, my_hrp) < obj_search_dist_limit)))
                                    {
                                        if (esp_objs_1_index < sizeof(esp_objs_1_parts)/8)
                                        {
                                            esp_objs_1_parts[esp_objs_1_index++] = __part;
                                        }
                                    }
                                    
                                }
                            }
                        }
                    }
                    usleep(3);
                })
                sleep(10);
            }
            else
            {
                sleep(1);
            }
            esp_objs_1_count = esp_objs_1_index;
        }
    });

    
#pragma mark Aimbot
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            float old_dist = max_aimbot_dist;
            float old_delta_dist = aimbot_size;
            vm_address_t my_team = rbx_player_get_team(task, local_player);
            vm_address_t __ch = 0;
            FOR_EACH_CHILD(players_service, i, {
                vm_address_t player = child;
                vm_address_t player_team = rbx_player_get_team(task, player);
                char is_on_ignored = false;
                char is_on_included = false;
                for (int x = 0 ; x < ignore_teams_count ; x++)
                {
                    if (player_team == ignore_teams[x]) {is_on_ignored = true;}
                }
                for (int x = 0 ; x < include_teams_count ; x++)
                {
                    if (player_team == include_teams[x]) {is_on_included = true;}
                }
                
                if (player != local_player)
                {
                    if ((player_team != my_team && my_team != 0) || my_team == 0)
                    {
                        if ((is_on_included && !is_on_ignored) || (ignore_teams_count == 0 && include_teams_count == 0))
                        {
                            vm_address_t character = rbx_player_get_character(task, player);
                            vm_address_t head = rbx_instance_find_first_child(task, character, character_part_name);
                            vm_address_t hum = rbx_instance_find_first_child_by_pointee(task, character, task_base_address, RBX_OBJECT_Humanoid_POINTEE_OFFSET);
                            if (rbx_humanoid_get_health(task, hum) > 0 && aimbot_do_hp_check)
                            {
                                rbx_cframe_t cam_cf = rbx_camera_get_cframe(task, camera);
                                rbx_cframe_t head_cf = rbx_basepart_get_cframe(task, head);
                                
                                float dist = 0;
                                const float depth = rbx_get_camera_relative_depth(cam_cf, head_cf.pos, &dist);
                                if (depth > 0)
                                {
                                    const vector3_t camera_look_vector = rbx_cframe_get_look_vector(cam_cf);
                                    const vector3_t f_offset = vector3_mul_num(camera_look_vector, (depth));
                                    const vector3_t f_pos = vector3_add(cam_cf.pos, f_offset);
                                    const float delta_dist = vector3_dist_dif(f_pos, head_cf.pos);
                                    
                                    if ((old_dist > dist) && (old_delta_dist > delta_dist))
                                    {
                                        __ch = head;
                                        //old_dist = dist;
                                        old_delta_dist = delta_dist;
                                    }
                                }
                            }
                        }
                    }
                }
            })
            closest_head = __ch;
            //rbx_print_descendants(task, closest_head, 0, 0);
            //printf("END\n");
            usleep(100000);
        }
    });
    
    static useconds_t aimbot_usleep_time = 75;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (camera_aimbot_enabled)
            {
                if (rbx_instance_is_valid_child(task, closest_head))
                {
                    if ((shared_memory->LEFT_MOUSE_DOWN == true || shared_memory ->LEFT_MOUSE_DOWN_ONCE) || camera_aimbot_aiming_override)
                    {
                        aimbot_usleep_time = 10;
                        rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, closest_head);
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        
                        float dist;
                        const float depth = rbx_get_camera_relative_depth(camera_cframe, torso_cframe.pos, &dist);
                        if (depth > 0)
                        {
                            const vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);
                            const vector3_t f_offset = vector3_mul_num(camera_look_vector, (depth));
                            const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                            const float delta_dist = vector3_dist_dif(f_pos, torso_cframe.pos);
                            
                            if ((max_aimbot_dist > dist) && (aimbot_size > delta_dist))
                            {
                                vector3_t new_lv = vector3_direction_to_from(torso_cframe.pos, camera_cframe.pos);
                                camera_cframe.r20 = -new_lv.x;
                                camera_cframe.r21 = -new_lv.y;
                                camera_cframe.r22 = -new_lv.z;
                                rbx_camera_set_cframe(task, camera, camera_cframe);
                                
                            }
                        }
                    }
                    else
                    {
                        aimbot_usleep_time = 50;
                    }
                }
                else
                {
                    aimbot_usleep_time = 4000;
                }
                usleep(aimbot_usleep_time);
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
            if (shared_memory->LEFT_MOUSE_DOWN_ONCE)
            {
                usleep(100000);
                shared_memory->LEFT_MOUSE_DOWN_ONCE = false;
            }
            usleep(10000);
        }
    });
    
    
    ESP_GLOBAL_VARIABLES
    static ESP_Color red_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    
    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = 1000;
    
    static ESP_Color default_esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    static ESP_Color item_esp_color = {.r = 0, .g = 0, .b = 1, .a = 1};
    
    
#pragma mark Hitboxes
    
    
    static vm_address_t hitbox_heads[400];
    static int hitbox_heads_count = 0;
    static float new_hitbox_f = 45;
    static vector3_t new_hitbox_size = {.x = 45, .y = 45, .z = 45};
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            int hhi = 0;
            if (hitboxes_enabled)
            {
                vm_address_t my_team = rbx_player_get_team(task, local_player);
                FOR_EACH_CHILD(players_service, i, {
                    vm_address_t player = child;
                    vm_address_t player_team = rbx_player_get_team(task, player);
                    char is_on_ignored = false;
                    char is_on_included = false;
                    for (int x = 0 ; x < ignore_teams_count ; x++)
                    {
                        if (player_team == ignore_teams[x]) {is_on_ignored = true;}
                    }
                    for (int x = 0 ; x < include_teams_count ; x++)
                    {
                        if (player_team == include_teams[x]) {is_on_included = true;}
                    }
                    
                    if (player != local_player)
                    {
                        vm_address_t character = rbx_player_get_character(task, player);
                        vm_address_t head = rbx_instance_find_first_child(task, character, character_part_name);
                        char __is = false;
                        if ((player_team != my_team && my_team != 0) || my_team == 0)
                        {
                            if ((is_on_included && !is_on_ignored) || (ignore_teams_count == 0 && include_teams_count == 0))
                            {
                                hitbox_heads[hhi++] = head;
                                __is = true;
                            }
                        }
                        if (!__is)
                        {
                            rbx_basepart_set_size(task, head, (vector3_t){.x = 1, .y = 1, .z = 1});
                        }
                        
                    }
                })
            }
            hitbox_heads_count = hhi;
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            COPY_BUF(hitbox_heads)
            COPY_VAR(hitbox_heads_count)
            rbx_cframe_t my_hrp_cf = rbx_basepart_get_cframe(task, my_hrp);
            
            for (int i = 0 ; i < __hitbox_heads_count ; i++)
            {
                vm_address_t e_h =  __hitbox_heads[i];
                rbx_cframe_t e_h_cf = rbx_basepart_get_cframe(task, e_h);
                
                float f = vector3_dist_dif(my_hrp_cf.pos, e_h_cf.pos);
                float __f = (f > 20) ? f - 10 : 10;
                vector3_t new_sz = (f > new_hitbox_size.z + 10) ? new_hitbox_size : (vector3_t){.x = __f, .y = __f, .z = __f};
                rbx_basepart_set_cancollide(task, e_h, false);
                rbx_basepart_set_size(task, e_h, new_sz);
            }
            if (!hitboxes_enabled)
            {
                sleep(1);
            }
            usleep(1000);
        }
    });
    
    

    
#pragma mark ESP Objects
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(300000);
            //sleep(1);
            //rbx_print_descendants(task, players_service, 0, 1);
            int esp_part_index = 0;
            //printf("%d\n", clickable_count);
            FOR_EACH_CHILD(players_service, i, {
                vm_address_t player = child;
                if (player != local_player)
                {
                    vm_address_t character = rbx_player_get_character(task, player);
                    vm_address_t hrp = rbx_instance_find_first_child(task, character, character_part_name);
                    vm_address_t hum = rbx_instance_find_first_child_by_pointee(task, character, task_base_address, RBX_OBJECT_Humanoid_POINTEE_OFFSET);
                    //rbx_print_descendants(task, character, 0, 1);
                    //printf("%p\n", character);
                    long player_name_l = 0;
                    char* player_name = rbx_instance_get_name(task, player, &player_name_l);
                    if (player_name)
                    {
                        ESP_Color esp_color;
                        esp_color.r = 1;
                        esp_color.g = 1;
                        esp_color.b = 1;
                        esp_color.a = 1;
                        
                        vm_address_t team = rbx_player_get_team(task, player);
                        //rbx_print_descendants(task, team, 0, 0);
                        if (team)
                        {
                            int num = rbx_team_get_brick_color(task, team);
                            //printf("%d\n", num);
                            rbx_brickcolor_info_t bc = rbx_brick_color_index(num);
                            esp_color.b = (float)bc.b/255;
                            esp_color.r = (float)bc.r/255;
                            esp_color.g = (float)bc.g/255;
                            //printf("bc %d, %d, %d\n", bc.r, bc.b, bc.g);
                            //printf("espcolor %f, %f, %f\n", esp_color.r, esp_color.b, esp_color.g);
                        }
                        else
                        {
                            esp_color = default_esp_color;
                        }
                        char txt[200];
                        bzero(txt, 200);
                        sprintf(txt, "%s (HP: %.0f)", player_name, rbx_humanoid_get_health(task, hum));
                        APPEND_ESP_OBJ(hrp, esp_color, txt);
                        vm_deallocate(mach_task_self_, (vm_address_t)player_name, player_name_l);
                    }
                }
            })
            if (auto_search)
            {
                COPY_BUF(esp_objs_1_appended_names)
                COPY_BUF(esp_objs_1_parts)
                COPY_VAR(esp_objs_1_count)
                for (int x = 0 ; x < __esp_objs_1_count ; x++)
                {
                    if (esp_part_index < (MAX_ESP_COUNT - 10)/2) //each box has a title box and actual box, so 2 esp boxes per "box"
                    {
                        vm_address_t obj = __esp_objs_1_parts[x];
                        long name_l = 0;
                        char* name = rbx_instance_get_name(task, obj, &name_l);
                        if (name)
                        {
                            char text[MAX_ESP_TEXT_LENGTH];
                            sprintf(text, "%s %s", name, __esp_objs_1_appended_names + (x * MAX_ESP_TEXT_LENGTH));
                            char __can = true;
                            for (int k = 0 ; k < ofs_ignore_count ; k++)
                            {
                                if (strstr(text, obj_filter_ingore_containing_str[k]))
                                {
                                    __can = false;
                                }
                            }
                            for (int k = 0 ; k < ofs_include_count ; k++)
                            {
                                if (strstr(text, obj_filter_include_containing_str[k]))
                                {
                                    __can = true;
                                }
                            }
                            if( __can)
                            {
                                ESP_Color color = item_esp_color;
                                APPEND_ESP_OBJ(obj, color, text);
                            }
                            //printf("%s\n", text);
                            vm_deallocate(mach_task_self_, (vm_address_t)name, name_l);
                        }
                    }
                }
            }
            {
                COPY_BUF(esp_objs_2_appended_names)
                COPY_BUF(esp_objs_2_parts)
                COPY_BUF(esp_objs_2_colors)
                COPY_VAR(esp_objs_2_count)
                for (int x = 0 ; x < __esp_objs_2_count ; x++)
                {
                    if (esp_part_index < (MAX_ESP_COUNT - 10)/2)
                    {
                        APPEND_ESP_OBJ(__esp_objs_2_parts[x], __esp_objs_2_colors[x], __esp_objs_2_appended_names + (x * MAX_ESP_TEXT_LENGTH))
                    }
                }
            }
            //printf("%d\n", esp_part_index);
            esp_part_count = esp_part_index;
        }
    });
    
    static char sidebar_text[200];
    static char sidebar_notes_text[200];
    
    static char fly_enabled = false;
    static float fly_speed = 200;
    
    NOCLIP_VARIABLES
    
    noclip_enabled = false;
    
    BASIC_NOCLIP_PART_GETTER
    BASIC_NOCLIP_ENGINE
    BASIC_FLY_ENGINE(BASIC_FLY_CONTROL_CODE, 300)
    
    BASIC_ESP_ENGINE({}, {
        if (closest_head && part == closest_head && camera_aimbot_enabled)
        {
            esp_color = target_color;
            border_width = 2;
        }
    }, 30, {
        bzero(sidebar_text, sizeof(sidebar_text));
        if (fly_enabled) { strcat(sidebar_text, "FLY ENABLED"); } strcat(sidebar_text, "\n");
        if (noclip_enabled) { strcat(sidebar_text, "NOCLIPPING"); } strcat(sidebar_text, "\n");
        strcat(sidebar_text, "\n");
        strcat(sidebar_text, sidebar_notes_text);
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
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = red_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
    });
    
    
    
#pragma mark f
    
    PRESS_ONCE_KEYBIND_CREATE('f', {
        fly_enabled = !fly_enabled;
        rbx_smooth_fly_patch(task, task_base_address, fly_enabled);
    }, true, "fly");
    
    
#pragma mark n
    
    PRESS_ONCE_KEYBIND_CREATE('n', {
        noclip_enabled = !noclip_enabled;
    }, true, "noclip");
    
    
    
#pragma mark - stdin Inputs -
    
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("fly", "", {
            fly_enabled = true;
            rbx_smooth_fly_patch(task, task_base_address, true);
            printf("ENABLED FLY\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("nofly", "", {
            fly_enabled = false;
            rbx_smooth_fly_patch(task, task_base_address, false);
            printf("DISABLED FLY\n");
        })
        INSERT_STDIN_MULTIARG_INPUT("flyspeed %f", "", {
            printf("SET FLY SPEED TO %f\n", fly_speed);
        }, 1, &fly_speed);
        INSERT_STDIN_INPUT_SIMPLE("coords", "", {
            printf("PRINTING PLAYER COORDINATES...\n");
            if (my_hrp)
            {
                vector3_t pos = rbx_basepart_get_cframe(task, my_hrp).pos;
                printf("vector3_t pos = {\n\t.x = %f,\n\t.y = %f,\n\t.z = %f\n};\n", pos.x, pos.y, pos.z);
            }
            else
            {
                printf("ERROR: HUMANOID ROOT PART NOT FOUND\n");
            }
        })
        INSERT_STDIN_INPUT_SIMPLE("mousecoords", "", {
            printf("PRINTING MOUSE COORDINATES...\n");
            printf("%f, %f\n", shared_memory->MOUSE_X, shared_memory->MOUSE_Y);
        })
        INSERT_STDIN_INPUT_SIMPLE("noclip", "", {
            noclip_enabled = true;
            printf("NOCLIP ENABLED\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("clip", "", {
            noclip_enabled = false;
            printf("NOCLIP DISABLED\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("afk", "", {
            anti_afk_enabled = true;
            printf("ANTI-AFK ENABLED\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("noafk", "", {
            anti_afk_enabled = false;
            printf("ANTI-AFK DISABLED\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("esp", "", {
            esp_enabled = true;
            printf("ESP ENABLED\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("noesp", "", {
            esp_enabled = false;
            printf("ESP DISABLED\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("camcoords", "", {
            printf("PRINTING CAMERA COORDINATES...\n");
            if (camera)
            {
                rbx_cframe_t cframe = rbx_camera_get_cframe(task, camera);
                printf("rbx_cframe_t cframe = {\n\t.r0 = %f, .r10 = %f, .r20 = %f,\n\t.r1 = %f, .r11 = %f, .r21 = %f,\n\t.r2 = %f, .r12 = %f, .r22 = %f,\n\t.pos = {.x = %f, .y = %f, .z = %f}\n};\n", cframe.r0, cframe.r10, cframe.r20, cframe.r1, cframe.r11, cframe.r21, cframe.r2, cframe.r12, cframe.r22, cframe.pos.x, cframe.pos.y, cframe.pos.z);
            }
            else
            {
                printf("ERROR: CAMERA NOT FOUND\n");
            }
        })
        INSERT_STDIN_INPUT_SIMPLE("placeid", "", {
            printf("PlaceId: %ld\n", rbx_get_placeid(task));
            long len = 0;
            char* jobid = rbx_datamodel_get_jobid(task, game, &len);
            if (jobid)
            {
                printf("JobId: %s\n", jobid);
                vm_deallocate(mach_task_self_, (vm_address_t)jobid, len);
            }
        })
        INSERT_STDIN_INPUT_SIMPLE("autoscan", "//show parts that you can interact with", auto_search = true; printf("ENABLED AUTOSCAN\n");)
        INSERT_STDIN_INPUT_SIMPLE("noautoscan", "", auto_search = false; printf("DISABLED AUTOSCAN\n");)
        {
            vm_address_t a = 0;
            int b = 0;
            INSERT_STDIN_MULTIARG_INPUT("pd %p %d", "//print descendants of address (child, depth)", {
                rbx_print_descendants(task, a, 0, b);
            }, 2, (void**)&a, &b);
        }
        {
            vm_address_t a = 0;
            int b = 0;
            INSERT_STDIN_MULTIARG_INPUT("pa %p %d", "//print ancestors of address (child, height)", {
                rbx_print_ancestors(task, a, b);
            }, 2, (void**)&a, &b);
        }
        INSERT_STDIN_INPUT_SIMPLE("print_esp_parts", "", {
            COPY_VAR(esp_part_count)
            COPY_BUF(esp_parts)
            COPY_BUF(esp_part_names)
            for (int i = 0 ; i < __esp_part_count ; i++)
            {
                printf("[%d] %s \t", i, __esp_part_names + (i * MAX_ESP_TEXT_LENGTH));
                rbx_print_descendants(task, __esp_parts[i], 0, 0);
            }
        })
        INSERT_STDIN_INPUT_SIMPLE("printteams", "", {
            rbx_print_descendants(task, teams_service, 0, 1);
        })
        {
            char nm[200];
            INSERT_STDIN_MULTIARG_INPUT("aimbot_includeteam %s", "", {
                vm_address_t t = rbx_instance_find_first_child_partial(task, teams_service, nm);
                if (t)
                {
                    include_teams[include_teams_count++] = t;
                    printf("added include team: "); rbx_print_descendants(task, t, 0, 0);
                }
                else
                {
                    printf("team \"%s\" not found", nm);
                }
            }, 1, nm)
        }
        
        {
            char nm[200];
            INSERT_STDIN_MULTIARG_INPUT("aimbot_ignoreteam %s", "", {
                vm_address_t t = rbx_instance_find_first_child_partial(task, teams_service, nm);
                if (t)
                {
                    ignore_teams[ignore_teams_count++] = t;
                    printf("added ignore team: "); rbx_print_descendants(task, t, 0, 0);
                }
                else
                {
                    printf("team \"%s\" not found", nm);
                }
            }, 1, nm)
        }
        INSERT_STDIN_MULTIARG_INPUT("cpn %s", "//character part name", {
            printf("Set aimbot/hitbox part to: %s\n", character_part_name);
        }, 1, character_part_name)
        INSERT_STDIN_INPUT_SIMPLE("cameraaimbot", "", {
            printf("ENABLED CAMERA AIMBOT\n");
            camera_aimbot_enabled = true;
        })
        INSERT_STDIN_INPUT_SIMPLE("nocameraaimbot", "", {
            printf("DISABLED CAMERA AIMBOT\n");
            camera_aimbot_enabled = false;
        })
        //INSERT_STDINMULTIARG
        INSERT_STDIN_MULTIARG_INPUT("aimbotsize %f", "", {
            printf("Set aimbot size to %f\n", aimbot_size);
        }, 1, &aimbot_size);
        INSERT_STDIN_MULTIARG_INPUT("hitboxes %f", "//size", {
            new_hitbox_size.x = new_hitbox_f;
            new_hitbox_size.y = new_hitbox_f;
            new_hitbox_size.z = new_hitbox_f;
            hitboxes_enabled = true;
            printf("Set hitbox size to %f\n", new_hitbox_f);
        }, 1, &new_hitbox_f)
        INSERT_STDIN_INPUT_SIMPLE("nohitboxes", "", {
            printf("DISABLED HITBOXES\n");
            hitboxes_enabled = false;
        })
        {
            INSERT_STDIN_MULTIARG_INPUT("of_ign %s", "//object filter ignore [partial string]", {
                ofs_ignore_count++;
            }, 1, obj_filter_ingore_containing_str[ofs_ignore_count])
        }
        {
            INSERT_STDIN_MULTIARG_INPUT("of_inc %s", "//object filter include [partial string]", {
                ofs_include_count++;
            }, 1, obj_filter_include_containing_str[ofs_include_count])
        }
        INSERT_STDIN_MULTIARG_INPUT("scansettings %d %d %d %d %f", "//search for [click detectors] [touch transmitters] [proximity prompts] [use dist limit] (1 or 0) [dist limit] (float)", {}, 5, &obj_search_include_click_detectors, &obj_search_include_touch_interest, &obj_search_include_proximity_prompts, &obj_search_use_dist_limit, &obj_search_dist_limit);
        INSERT_STDIN_INPUT_SIMPLE("printgame", "", {
            rbx_print_descendants(task, game, 0, 1);
        })
        {
            vm_address_t a = 0;
            int b = 0;
            INSERT_STDIN_MULTIARG_INPUT("viewpart %p %d", "//[part addr] [seconds] - also works for models", {
                vm_address_t old_cs = rbx_camera_get_camera_subject(task, camera);
                rbx_camera_set_camera_subject(task, camera, a);
                sleep(b);
                rbx_camera_set_camera_subject(task, camera, old_cs);
                
            }, 2, (void**)&a, &b)
        }
        {
            vm_address_t a = 0;
            INSERT_STDIN_MULTIARG_INPUT("gotopart %p", "//[part addr]", {
                for (int i = 0 ; i < 100 ; i++)
                {
                    rbx_basepart_set_cframe(task, my_hrp, rbx_basepart_get_cframe(task, a));
                }
            }, 1, (void**)&a)
        }
        INSERT_STDIN_INPUT_SIMPLE("quit", "", exit(0);)
    })
    
    
    
    
    
#pragma mark - Presets / Additions -
    
    
    if (!use_presets)
    {
        return;
    }
    
    
    
#pragma mark Containment Breach
    
    if (placeid == CONTAINMENT_BREACH_PLACE_ID)
    {
        POST_NOTIF("Generic cheat", "Using preset for Containment Breach")
        camera_aimbot_enabled = false;
        strcpy(character_part_name, "HumanoidRootPart");
        new_hitbox_size = (vector3_t){.x = 20, .y = 20, .z = 20};
        hitboxes_enabled = true;
        fly_speed = 20;
        
        static ESP_Color s_nav_esp_color = (ESP_Color){.r = 0, .g = 1, .b = 0, .a = 1};
        static ESP_Color keycard_colors[7] = {
            {0, 0, 0, 1},                     // index 0 placeholder (unused)
            {1.0f, 1.0f, 0.0f, 1.0f},         // yellow
            {1.0f, 0.75f, 0.0f, 1.0f},        // yellow-orange
            {1.0f, 0.5f, 0.0f, 1.0f},         // deep orange
            {1.0f, 0.25f, 0.0f, 1.0f},        // orange-red
            {1.0f, 0.0f, 0.0f, 1.0f},         // red
            {0.0f, 0.0f, 1.0f, 1.0f}          // blue
        };
        
        static vm_address_t click_detectors[20000];
        static int cd_count = 0;
        
        static vm_address_t text_labels[20000];
        static int tl_count = 0;
        
        LOW_PRIO_LOOP_HEADER
        int cdi = 0;
        int tli = 0;
        FOR_EACH_DESCENDANT(task, workspace, 10000000, 10, i, {
            vm_address_t pointee_offset = rbx_instance_get_pointee_offset(task, obj, task_base_address);
            if (pointee_offset == RBX_OBJECT_ClickDetector_POINTEE_OFFSET)
            {
                click_detectors[cdi++] = obj;
            }
            if (pointee_offset == RBX_OBJECT_TextLabel_POINTEE_OFFSET)
            {
                text_labels[tli++] = obj;
            }
        })
        tl_count = tli;
        cd_count = cdi;
        sleep(20);
        LOW_PRIO_LOOP_FOOTER
        
        
        LOW_PRIO_LOOP_HEADER
        int esp_objs_2_idx = 0;
        COPY_BUF(click_detectors)
        COPY_VAR(cd_count)
        COPY_BUF(text_labels)
        COPY_VAR(tl_count)
        bzero(sidebar_notes_text, sizeof(sidebar_notes_text));
        for (int i = 0 ; i < __cd_count ; i++)
        {
            vm_address_t obj = __click_detectors[i];
            vm_address_t p = rbx_instance_get_parent(task, obj);
            vm_address_t m = rbx_instance_get_parent(task, p);
            if (distance_between_baseparts(task, p, my_hrp) < 300)
            {
                long nm_l = 0;
                char* nm = rbx_instance_get_name(task, m, &nm_l);
                if (nm)
                {
                    char __is = false;
                    ESP_Color color = red_esp_color;
                    if (strcmp(nm, "S-nav") == 0)
                    {
                        __is = true;
                        color = s_nav_esp_color;
                    }
                    if ((nm[0] == 'L' && nm[2] == 0) || (nm[3] == 'L' && nm[5] == 0))
                    {
                        char keycard_lvl = atoi(nm + strlen(nm) - 1);
                        __is = true;
                        color = keycard_colors[keycard_lvl];
                    }
                    if (__is)
                    {
                        esp_objs_2_parts[esp_objs_2_idx] = p;
                        esp_objs_2_colors[esp_objs_2_idx] = color;
                        strcpy(esp_objs_2_appended_names + (esp_objs_2_idx * MAX_ESP_TEXT_LENGTH), nm);
                        esp_objs_2_idx++;
                    }
                    vm_deallocate(mach_task_self(), (vm_address_t)nm, nm_l);
                }
                
            }
        }
        for (int i = 0 ; i < __tl_count ; i++)
        {
            vm_address_t tl = __text_labels[i];
            vm_address_t part = 0;
            FOR_EACH_ANCESTOR(task, tl, {
                if (rbx_instance_is_a_basepart(task, ancestor, task_base_address))
                {
                    part = ancestor;
                }
            })
            if (part)
            {
                if (rbx_instance_is_named(task, part, "CodedDocument"))
                {
                    long str_l = 0;
                    char* str = rbx_textlabel_get_text(task, tl, &str_l);
                    if (str)
                    {
                        sprintf(sidebar_notes_text, "Weapons code: %s\n", str);
                        //printf("Weapons code is: %s\n", str);
                        vm_deallocate(mach_task_self(), (vm_address_t)str, str_l);
                    }
                }
                /*if (distance_between_baseparts(task, part, my_hrp) < 600)
                {
                    long str_l = 0;
                    char* str = rbx_textlabel_get_text(task, tl, &str_l);
                    if (str)
                    {
                        esp_objs_2_parts[esp_objs_2_idx] = part;
                        esp_objs_2_colors[esp_objs_2_idx] = red_esp_color;
                        strcpy(esp_objs_2_appended_names + (esp_objs_2_idx * MAX_ESP_TEXT_LENGTH), str);
                        esp_objs_2_idx++;
                        vm_deallocate(mach_task_self(), (vm_address_t)str, str_l);
                    }
                }*/
            }
        }
        esp_objs_2_count = esp_objs_2_idx;
        //printf("%d\n", esp_objs_2_count);
        sleep(5);
        
        LOW_PRIO_LOOP_FOOTER
        
    }
#pragma mark Rivals
    if (placeid == RIVALS_MATCH_PLACE_ID)
    {
        POST_NOTIF("Generic cheat", "Using preset for Rivals (In-Match)")
        camera_aimbot_enabled = true;
        aimbot_size = 20;
    }
#pragma mark Arsenal
    if (placeid == ARSENAL_PLACE_ID)
    {
        POST_NOTIF("Generic cheat", "Using preset for Arsenal")
        camera_aimbot_enabled = true;
        aimbot_size = 20;
        strcpy(character_part_name, "Head");
        new_hitbox_size = (vector3_t){.x = 20, .y = 20, .z = 20};
        hitboxes_enabled = true;
    }
#pragma mark Red vs. Blue Tycoon
    if (placeid == RED_VS_BLUE_TYCOON_PLACE_ID)
    {
        POST_NOTIF("Generic cheat", "Using preset for Red vs. Blue Tycoon")
        camera_aimbot_enabled = true;
        aimbot_size = 20;
        strcpy(character_part_name, "HumanoidRootPart");
    }
#pragma mark Prison Life
    if (placeid == PRISON_LIFE_PLACE_ID)
    {
        POST_NOTIF("Generic cheat", "Using preset for Prison Life")
        camera_aimbot_enabled = true;
        aimbot_size = 20;
        strcpy(character_part_name, "HumanoidRootPart");
    }
}

