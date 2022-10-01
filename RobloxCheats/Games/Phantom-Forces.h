


void phantom_forces_cheat(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    
    void* handle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
    vm_offset_t esp_enabled_offset = gdso(handle, s_load_address, "ESP_ENABLED");
    //vm_offset_t aimbot_x_offset = gdso(handle, s_load_address, "AIMBOT_X");
    //vm_offset_t aimbot_y_offset = gdso(handle, s_load_address, "AIMBOT_Y");
    vm_offset_t window_w_offset = gdso(handle, s_load_address, "WINDOW_W");
    vm_offset_t window_h_offset = gdso(handle, s_load_address, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = gdso(handle, s_load_address, "ESP_USLEEP_TIME");
    vm_offset_t esp_box_hidden_array_offset = gdso(handle, s_load_address, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = gdso(handle, s_load_address, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_box_color_array_offset = gdso(handle, s_load_address, "ESP_BOX_COLOR_ARRAY");
    
    dlclose(handle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB__);
    
    char esp_enabled = true;
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, 1);
    
    vm_address_t esp_box_hidden_array = load_address + esp_box_hidden_array_offset;
    vm_address_t esp_box_frame_array = load_address + esp_box_frame_array_offset;
    vm_address_t esp_box_color_array = load_address + esp_box_color_array_offset;

    
    static float window_w;
    static float window_h;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t teams_service = rbx_instance_find_first_child_of_class(task, game, "Teams");
    vm_address_t phantoms_team = rbx_instance_find_first_child(task, teams_service, "Phantoms");
    vm_address_t ghosts_team = rbx_instance_find_first_child(task, teams_service, "Ghosts");
    
    vm_address_t players_folder = rbx_instance_find_first_child(task, workspace, "Players");
    vm_address_t blue_team_folder = rbx_instance_find_first_child(task, players_folder, "Bright blue");
    vm_address_t orange_team_folder = rbx_instance_find_first_child(task, players_folder, "Bright orange");
    
    long plr_count = 0;
    vm_address_t* plrs = rbx_instance_get_children(task, players_service, &plr_count);
    vm_address_t my_plr = plrs[0];
    
    useconds_t esput = 1000;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    static vm_address_t* used_player_array = NULL;
    static long used_player_count = 0;
    
    static ESP_Color color;
    color.r = 0;
    color.g = 1;
    color.b = 0;
    color.a = 1;
    
    static ESP_Frame frame;
    
    static float resize = 0;
    static float mag = 0;
    static float max_mag = 300;
    static float min_mag = 10;
    static float max_resize = 30;
    float resize_factor = (max_resize/(max_mag - min_mag));
    
    static vm_address_t my_team = 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            my_team = rbx_player_get_team(task, my_plr);
            if (my_team == phantoms_team)
            {
                if (used_player_array)
                {
                    free(used_player_array);
                }
                used_player_array = rbx_instance_get_children(task, orange_team_folder, &used_player_count);
            }
            if (my_team == ghosts_team)
            {
                if (used_player_array)
                {
                    free(used_player_array);
                }
                used_player_array = rbx_instance_get_children(task, blue_team_folder, &used_player_count);
            }
            
            vm_read(task, load_address + window_w_offset, 4, &read_data, &data_cnt);
            window_w = *(float*)read_data;
            vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
            
            vm_read(task, load_address + window_h_offset, 4, &read_data, &data_cnt);
            window_h = *(float*)read_data;
            vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
            
            sleep(1);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (used_player_count > 0)
            {
                for (long i = 0 ; i < used_player_count ; i++)
                {
                    vm_address_t player = 0;
                    vm_address_t head = 0;
                    usleep(esput/used_player_count);
                    if (used_player_array)
                    {
                        player = used_player_array[i];
                    }
                    if (player)
                    {
                        head = rbx_instance_find_first_child(task, player, "Head");
                    }
                    if (head)
                    {
                        rbx_cframe_t head_cframe = rbx_basepart_get_cframe(task, head);
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        custom_rbx_world_to_screen_info_t wtsi;
                        
                        register float fov = rbx_camera_get_field_of_view(task, camera);
                        
                        wtsi = rbx_world_to_screen_point(camera_cframe, fov, head_cframe.pos, window_w, window_h);
                        char hidden = true;
                        if (isnan(wtsi.x) || isnan(wtsi.y))
                        {
                            hidden = true;
                            wtsi.x = 0;
                            wtsi.y = 0;
                            vm_write(task, esp_box_hidden_array + i, (vm_offset_t)&hidden, 1);
                        }
                        else
                        {
                            mag = wtsi.magnitude;
                            if (wtsi.magnitude > max_mag)
                            {
                                mag = max_mag;
                            }
                            if (wtsi.magnitude < min_mag)
                            {
                                mag = min_mag;
                            }
                            resize = (((max_mag - min_mag) - mag) * resize_factor);
                            
                            hidden = false;
                            vm_write(task, esp_box_hidden_array + i, (vm_offset_t)&hidden, 1);
                            
                            vm_offset_t f_o = (i * sizeof(ESP_Frame));
                            vm_offset_t c_o = (i * sizeof(ESP_Color));
                            
                            
                            frame.x = wtsi.x - (4 + resize/2);
                            frame.y = wtsi.y -  (4 + resize/2);
                            frame.w = 8 + resize;
                            frame.h = 8 + resize;
                            
                            vm_write(task, esp_box_frame_array + f_o, (vm_address_t)&frame, sizeof(ESP_Frame));
                            vm_write(task, esp_box_color_array + c_o, (vm_address_t)&color, sizeof(ESP_Color));
                        }
                    }
                    player = 0;
                    head = 0;
                }
            }
            else
            {
                usleep(99999);
            }
            
            
            
        }
    });
    
    
    
    
    
    
    
    
}
