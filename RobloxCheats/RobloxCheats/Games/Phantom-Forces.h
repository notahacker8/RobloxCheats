
#define RBX_PHANTOM_FORCES_MAX_PLAYER_COUNT 32

void phantom_forces_cheat(task_t task)
{
    printf("- PHANTOM FORCES -\n");
    static mach_msg_type_number_t data_cnt;
    
    void* handle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
    vm_offset_t esp_enabled_offset = gdso(handle, s_load_address, "ESP_ENABLED");
    vm_offset_t window_w_offset = gdso(handle, s_load_address, "WINDOW_W");
    vm_offset_t window_h_offset = gdso(handle, s_load_address, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = gdso(handle, s_load_address, "ESP_USLEEP_TIME");
    
    vm_offset_t esp_box_hidden_array_offset = gdso(handle, s_load_address, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = gdso(handle, s_load_address, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_box_color_array_offset = gdso(handle, s_load_address, "ESP_BOX_COLOR_ARRAY");
    vm_offset_t esp_box_border_width_array_offset = gdso(handle, s_load_address, "ESP_BOX_BORDER_WIDTH_ARRAY");
    
    dlclose(handle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB__);
    
    char esp_enabled = true;
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, 1);
    
    vm_address_t esp_box_hidden_array = load_address + esp_box_hidden_array_offset;
    vm_address_t esp_box_frame_array = load_address + esp_box_frame_array_offset;
    vm_address_t esp_box_color_array = load_address + esp_box_color_array_offset;
    vm_address_t esp_box_border_width_array = load_address + esp_box_border_width_array_offset;

    static float window_w;
    static float window_h;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t teams_service = rbx_instance_find_first_child_of_class(task, game, "Teams");
    vm_address_t phantoms_team = rbx_instance_find_first_child(task, teams_service, "Phantoms");
    vm_address_t ghosts_team = rbx_instance_find_first_child(task, teams_service, "Ghosts");
    
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    useconds_t esput = 500;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    static vm_address_t enemy_torsos[RBX_PHANTOM_FORCES_MAX_PLAYER_COUNT];
    static long enemy_count = 0;
    
    static ESP_Color esp_color;
    esp_color.r = 0;
    esp_color.g = 1;
    esp_color.b = 0;
    esp_color.a = 1;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            vm_address_t players_folder = rbx_instance_find_first_child(task, workspace, "Players");
            vm_address_t blue_team_folder = rbx_instance_find_first_child(task, players_folder, "Bright blue");
            vm_address_t orange_team_folder = rbx_instance_find_first_child(task, players_folder, "Bright orange");
            vm_address_t my_team = rbx_player_get_team(task, local_player);
            int enemy_torso_index = 0;
            if (my_team == phantoms_team)
            {
                rbx_child_t* enemies_list = rbx_instance_get_children(task, orange_team_folder, &enemy_count);
                if (enemies_list)
                {
                    for (int i = 0 ; i < enemy_count ; i++)
                    {
                        vm_address_t enemy = enemies_list[i].child_address;
                        if (enemy)
                        {
                            vm_address_t torso = rbx_instance_find_first_child(task, enemy, "Torso");
                            if (torso)
                            {
                                enemy_torsos[enemy_torso_index] = torso;
                                enemy_torso_index++;
                            }
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)enemies_list, enemy_count * sizeof(rbx_child_t));
                }
            }
            if (my_team == ghosts_team)
            {
                rbx_child_t* enemies_list = rbx_instance_get_children(task, blue_team_folder, &enemy_count);
                if (enemies_list)
                {
                    for (int i = 0 ; i < enemy_count ; i++)
                    {
                        vm_address_t enemy = enemies_list[i].child_address;
                        if (enemy)
                        {
                            vm_address_t torso = rbx_instance_find_first_child(task, enemy, "Torso");
                            if (torso)
                            {
                                enemy_torsos[enemy_torso_index] = torso;
                                enemy_torso_index++;
                            }
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)enemies_list, enemy_count * sizeof(rbx_child_t));
                }
            }
            
            kern_return_t kr;
            vm_address_t read_data;
            kr = vm_read(task, load_address + window_w_offset, 4, &read_data, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                window_w = *(float*)read_data;
                vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
            }
            
            kr = vm_read(task, load_address + window_h_offset, 4, &read_data, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                window_h = *(float*)read_data;
                vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
            }

            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (enemy_count > 0)
            {
                int esp_index = 0;
                for (long i = 0 ; i < enemy_count ; i++)
                {
                    usleep(esput/enemy_count);
                    
                    vm_address_t enemy_torso = enemy_torsos[i];
                    if (enemy_torso)
                    {
                        vm_address_t parent = rbx_instance_get_parent(task, enemy_torso);
                        if (parent)
                        {
                            rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, enemy_torso);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            rbx_draw_esp_box(task, torso_cframe.pos,
                                             camera_cframe, esp_box_hidden_array,
                                             esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, 2,
                                             esp_color, fov, 3, 3, 0, 0, window_w, window_h, esp_index, true);
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
    
    
    
    
    
}
