
#define RBX_JAILBREAK_MAX_PLAYER_COUNT 30

void jailbreak_cheat(task_t task)
{
    
    printf(" - JAILBREAK (ESP) - \n");
    
    void* dlhandle = dlopen(__LIBESP_DYLIB_PATH__, RTLD_NOW);
    if (!dlhandle)
    {
        printf("%s\n", dlerror());
    }
    
    vm_address_t s_load_address = task_get_image_address_by_path(mach_task_self_, __LIBESP_DYLIB_PATH__);
    
    vm_offset_t keys_down_offset = gdso(dlhandle, s_load_address, "KEYS_DOWN");
    vm_offset_t esp_enabled_offset = gdso(dlhandle, s_load_address, "ESP_ENABLED");
    vm_offset_t esp_count_offset = gdso(dlhandle, s_load_address, "ESP_COUNT");
    vm_offset_t window_w_offset = gdso(dlhandle, s_load_address, "WINDOW_W");
    vm_offset_t window_h_offset = gdso(dlhandle, s_load_address, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = gdso(dlhandle, s_load_address, "ESP_USLEEP_TIME");
    
    vm_offset_t esp_box_hidden_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_box_color_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_COLOR_ARRAY");
    vm_offset_t esp_box_text_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_TEXT_ARRAY");
    vm_offset_t esp_box_border_width_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_BORDER_WIDTH_ARRAY");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    if (!load_address)
    {
        printf("Couldn't find libESP.dylib in task %d\n", task);
        exit(0);
    }
    
    static char esp_enabled = true;
    
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, (int)sizeof(char));
    int esp_count = RBX_JAILBREAK_MAX_PLAYER_COUNT * 2;
    vm_write(task, load_address + esp_count_offset, (vm_offset_t)&esp_count, (int)sizeof(int));
    
    vm_address_t esp_box_text_array = load_address + esp_box_text_array_offset;
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
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static long current_player_count = 0;
    static ESP_Color player_team_esp_colors[RBX_JAILBREAK_MAX_PLAYER_COUNT];
    static vm_address_t player_torsos[RBX_JAILBREAK_MAX_PLAYER_COUNT];
    static char player_names[RBX_JAILBREAK_MAX_PLAYER_COUNT][MAX_ESP_TEXT_LENGTH];
    
    static vm_address_t my_char = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_humanoid = 0;
    static vector3_t vel;
    
    static float fly_speed = 100;
    static float fly_enabled = false;
    
    
    
    
    
#pragma mark - Object Loop -
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            window_w = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_w_offset))).f;
            window_h = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_h_offset))).f;
            
            my_char = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_char, "HumanoidRootPart");
            my_humanoid = rbx_instance_find_first_child_of_class(task, my_char, "Humanoid");
            
            long player_count = 0;
            rbx_child_t* player_list = rbx_instance_get_children(task, players_service, &player_count);
            
            int player_index = 0;
            if (player_list)
            {
                for (long i = 0 ; i < player_count ; i++)
                {
                    vm_address_t player = player_list[i].child_address;
                    //rbx_print_descendants(task, player, 0, 0);
                    if (player != local_player)
                    {
                        vm_address_t team = rbx_player_get_team(task, player);
                        vm_address_t character = rbx_player_get_character(task, player);
                        vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
                        if (hrp)
                        {
                            vm_address_t instance_self = rbx_instance_get_self(task, hrp);
                            vm_address_t parent = rbx_instance_get_parent(task, hrp);
                            if (parent && instance_self == hrp)
                            {
                                long display_name_length;
                                char* display_name = rbx_player_get_displayname(task, player, &display_name_length);
                                if (display_name)
                                {
                                    //printf("%s\n", display_name);
                                    strcpy(player_names[player_index], display_name);
                                    vm_deallocate(mach_task_self_, (vm_address_t)display_name, display_name_length);
                                }
                                player_torsos[player_index] = hrp;
                                rbx_brickcolor_info_t color_info = rbx_brick_color_index(rbx_team_get_brick_color(task, team));
                                player_team_esp_colors[player_index].r = ((float)color_info.r)/255;
                                player_team_esp_colors[player_index].g = ((float)color_info.g)/255;
                                player_team_esp_colors[player_index].b = ((float)color_info.b)/255;
                                player_team_esp_colors[player_index].a = 1;
                                player_index++;
                            }
                        }
                    }
                }
            }
            current_player_count = player_index;
            vm_deallocate(mach_task_self_, (vm_address_t)player_list, player_count * sizeof(rbx_child_t));
            sleep(1);
        }
    });
    
    
    
    
    
#pragma mark - Fly -
    
    
    
    
    
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
            if (fly_enabled)
            {
                rbx_basepart_set_gravity(task, my_hrp, 0.0f);
                rbx_basepart_set_velocity(task, my_hrp, vel);
            }
            else
            {
                rbx_basepart_set_gravity(task, my_hrp, 200.0f);
            }
            usleep(100);
        }
    });
    
    
    
    
    
#pragma mark - ESP -
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (current_player_count > 0 && current_player_count < (RBX_JAILBREAK_MAX_PLAYER_COUNT + 1))
            {
                int var_index = 0;
                int esp_index = 0;
                
                for (long i = 0 ; i < current_player_count ; i++)
                {
                    usleep(esput/current_player_count);
                    
                    vm_address_t torso = player_torsos[i];
                    if (torso)
                    {
                        vm_address_t instance_self = rbx_instance_get_self(task, torso);
                        vm_address_t parent = rbx_instance_get_parent(task, torso);
                        if (parent && instance_self == torso)
                        {
                            rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, torso);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            
                            ESP_Color esp_color = player_team_esp_colors[var_index];
                            char* player_name = player_names[var_index];
                            
                            ESP_Frame scaled_frame = rbx_draw_esp_box(task, torso_cframe.pos,
                                             camera_cframe, esp_box_hidden_array,
                                             esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, 2,
                                             esp_color, fov, 10, 10, 0, 0, window_w, window_h, esp_index, true);
                            esp_index++;
                            
                            rbx_draw_esp_box(task, torso_cframe.pos,
                                             camera_cframe, esp_box_hidden_array,
                                             esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, 0,
                                             esp_color, fov, 200, 40, 0, scaled_frame.h + 10, window_w, window_h, esp_index, false);
                            vm_write(task, esp_box_text_array + (esp_index * MAX_ESP_TEXT_LENGTH), (vm_address_t)player_name, MAX_ESP_TEXT_LENGTH);
                            
                            esp_index++;
                            
                            var_index++;
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
   
    
}
