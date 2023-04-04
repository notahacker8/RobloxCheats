
#define RBX_ERLC_MAX_PLAYER_COUNT 35

void emergency_response_cheat(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    
    void* dlhandle = dlopen(__INJECTED_DYLIB_PATH__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB_PATH__);
    
    vm_offset_t keys_down_offset = gdso(dlhandle, s_load_address, "KEYS_DOWN");
    vm_offset_t left_mouse_down_offset = gdso(dlhandle, s_load_address, "LEFT_MOUSE_DOWN");
    vm_offset_t esp_enabled_offset = gdso(dlhandle, s_load_address, "ESP_ENABLED");
    vm_offset_t window_w_offset = gdso(dlhandle, s_load_address, "WINDOW_W");
    vm_offset_t window_h_offset = gdso(dlhandle, s_load_address, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = gdso(dlhandle, s_load_address, "ESP_USLEEP_TIME");
    vm_offset_t esp_count_offset = gdso(dlhandle, s_load_address, "ESP_COUNT");
    
    vm_offset_t esp_box_hidden_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_box_color_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_COLOR_ARRAY");
    vm_offset_t esp_box_border_width_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_BORDER_WIDTH_ARRAY");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB_PATH__);
    
    char esp_enabled = true;
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, 1);
    
    vm_address_t esp_box_hidden_array = load_address + esp_box_hidden_array_offset;
    vm_address_t esp_box_frame_array = load_address + esp_box_frame_array_offset;
    vm_address_t esp_box_color_array = load_address + esp_box_color_array_offset;
    vm_address_t esp_box_border_width_array = load_address + esp_box_border_width_array_offset;

    static float window_w;
    static float window_h;
    
    static char is_w_pressed = false;
    static char is_a_pressed = false;
    static char is_s_pressed = false;
    static char is_d_pressed = false;
    
    useconds_t esput = 1000;
    static int esp_count = 100;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    vm_write(task, load_address + esp_count_offset, (vm_offset_t)&esp_count, 4);
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static long current_player_count = 0;
    static ESP_Color player_team_esp_colors[RBX_ERLC_MAX_PLAYER_COUNT];
    static vm_address_t player_torsos[RBX_ERLC_MAX_PLAYER_COUNT];
    static vm_address_t my_char = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_humanoid = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    static vector3_t vel;
    
    static vm_address_t closest_enemy_head;
    static char closest_enemy_exists = false;
    static const float max_delta_ratio = 0.245;
    static float fly_speed = 75;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            window_w = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_w_offset))).f;
            window_h = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_h_offset))).f;
            
            bool __cee = false;
            float old_dist = 999;
            float old_delta_ratio = max_delta_ratio;
            
            long plr_count = 0;
            rbx_child_t* player_list = rbx_instance_get_children(task, players_service, &plr_count);
            long __plr_count = 0;
            
            my_char = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_char, "HumanoidRootPart");
            my_humanoid = rbx_instance_find_first_child_of_class(task, my_char, "Humanoid");
            my_hrp_cframe_address = rbx_basepart_get_properties_address(task, my_hrp) + RBX_PART_PROPERTIES_CFRAME_OFFSET;
            rbx_basepart_set_gravity(task, my_hrp, 80);
            
            if (player_list)
            {
                for (long i = 0 ; i < plr_count ; i++)
                {
                    vm_address_t player = player_list[i].child_address;
                    if (player != local_player)
                    {
                        vm_address_t team = rbx_player_get_team(task, player);
                        vm_address_t character = rbx_player_get_character(task, player);
                        vm_address_t RAGDOLLED_bool_value = rbx_instance_find_first_child(task, character, "RAGDOLLED");
                        vm_address_t head = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
                        if (head && !RAGDOLLED_bool_value)
                        {
                            player_torsos[__plr_count] = head;
                            rbx_brickcolor_info_t color_info = rbx_brick_color_index(rbx_team_get_brick_color(task, team));
                            player_team_esp_colors[__plr_count].r = ((float)color_info.r)/255;
                            player_team_esp_colors[__plr_count].g = ((float)color_info.g)/255;
                            player_team_esp_colors[__plr_count].b = ((float)color_info.b)/255;
                            player_team_esp_colors[__plr_count].a = 1;
                            __plr_count++;
                            
                            rbx_cframe_t head_cframe = rbx_basepart_get_cframe(task, head);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            const vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);
                            const float dist = vector3_dist_dif(head_cframe.pos, camera_cframe.pos);
                            const vector3_t f_offset = vector3_mul_num(camera_look_vector, dist);
                            const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                            const float delta_dist = vector3_dist_dif(f_pos, head_cframe.pos);
                            const float delta_ratio = (delta_dist/dist);
                            if (old_dist > dist && old_delta_ratio > delta_ratio)
                            {
                                __cee = true;
                                old_dist = dist;
                                //old_delta_ratio = delta_ratio
                                closest_enemy_head = head;
                            }
                        }
                    }
                }
            }
            current_player_count = __plr_count;
            closest_enemy_exists = __cee;
            vm_deallocate(mach_task_self_, (vm_address_t)player_list, current_player_count * sizeof(rbx_child_t));
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(1000);
            
            is_w_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 'w');
            is_a_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 'a');
            is_s_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 's');
            is_d_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 'd');
            
            rbx_cframe_t cf = rbx_camera_get_cframe(task, camera);
            rbx_basepart_set_gravity(task, my_hrp, 0.0f);
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
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(100);
            rbx_basepart_set_velocity(task, my_hrp, vel);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (current_player_count > 0)
            {
                int esp_index = 0;
                for (long i = 0 ; i < current_player_count ; i++)
                {
                    usleep(esput/current_player_count);
                    
                    vm_address_t torso = player_torsos[i];
                    if (torso)
                    {
                        vm_address_t parent = rbx_instance_get_parent(task, torso);
                        if (parent)
                        {
                            rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, torso);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            ESP_Color esp_color = player_team_esp_colors[i];
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            rbx_draw_esp_box(task, torso_cframe.pos,
                                             camera_cframe, esp_box_hidden_array,
                                             esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, 2,
                                             esp_color, fov, 20, 20, 0, 0, window_w, window_h, esp_index, false);
                            esp_index++;
                        }
                    }
                }
                char hiddens[MAX_ESP_COUNT - esp_index];
                memset(hiddens, 1, sizeof(hiddens));
                vm_write(task, esp_box_hidden_array + esp_index, (vm_address_t)hiddens, (int)sizeof(hiddens));
                
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
            vm_address_t read_data;
            kern_return_t kr;
            kr = vm_read(task, load_address + left_mouse_down_offset, 1, (vm_offset_t*)&read_data, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                char lmd = *(char*)read_data;
                vm_deallocate(mach_task_self_, read_data, 1);
                if (lmd == true && closest_enemy_exists == true && closest_enemy_head)
                {
                    rbx_cframe_t head_cframe = rbx_basepart_get_cframe(task, closest_enemy_head);
                    rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                    const vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);
                    const float dist = vector3_dist_dif(head_cframe.pos, camera_cframe.pos);
                    const vector3_t f_offset = vector3_mul_num(camera_look_vector, dist);
                    const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                    const float delta_dist = vector3_dist_dif(f_pos, head_cframe.pos);
                    const float delta_ratio = (delta_dist/dist);
                    
                    if (max_delta_ratio > delta_ratio)
                    {
                        vector3_t new_look_vector = vector3_unit(head_cframe.pos, camera_cframe.pos);
                        camera_cframe.r20 = -new_look_vector.x;
                        camera_cframe.r21 = -new_look_vector.y;
                        camera_cframe.r22 = -new_look_vector.z;
                        rbx_camera_set_cframe(task, camera, &camera_cframe);
                    }
                }
            }
            usleep(50);
        }
    });
    
}
