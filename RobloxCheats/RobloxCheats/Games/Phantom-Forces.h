
#define RBX_PHANTOM_FORCES_MAX_PLAYER_COUNT 32

void phantom_forces_cheat(task_t task)
{
    printf("- PHANTOM FORCES (AIMBOT & ESP) -\n");

    static mach_msg_type_number_t data_cnt;
    
    void* dlhandle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
    vm_offset_t left_mouse_down_offset = gdso(dlhandle, s_load_address, "LEFT_MOUSE_DOWN");
    vm_offset_t esp_enabled_offset = gdso(dlhandle, s_load_address, "ESP_ENABLED");
    vm_offset_t esp_count_offset = gdso(dlhandle, s_load_address, "ESP_COUNT");
    vm_offset_t window_w_offset = gdso(dlhandle, s_load_address, "WINDOW_W");
    vm_offset_t window_h_offset = gdso(dlhandle, s_load_address, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = gdso(dlhandle, s_load_address, "ESP_USLEEP_TIME");
    
    vm_offset_t esp_box_hidden_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_box_color_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_COLOR_ARRAY");
    vm_offset_t esp_box_border_width_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_BORDER_WIDTH_ARRAY");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB__);
    
    static char esp_enabled = true;
    static char aimbot_enabled = true;
    
    static int max_player_count = RBX_PHANTOM_FORCES_MAX_PLAYER_COUNT;
    
    static const float max_delta_ratio = 0.135;
    static const float max_dist = 500;
    
    vm_write(task, load_address + esp_count_offset, (vm_offset_t)&max_player_count, 4);
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
    
    static vm_address_t closest_enemy_torso;
    static char closest_enemy_exists = false;
    
    static vm_address_t enemy_torsos[RBX_PHANTOM_FORCES_MAX_PLAYER_COUNT];
    static vm_address_t trigger = 0;
    static vm_address_t trigger_cframe_address = 0;
    
    static long enemy_count = 0;
    
    static float new_trigger_cframe_r20;
    static float new_trigger_cframe_r21;
    static float new_trigger_cframe_r22;
    static useconds_t aimbot_usleep_time = 75;

    static ESP_Color esp_color;
    esp_color.r = 0;
    esp_color.g = 1;
    esp_color.b = 0;
    esp_color.a = 1;
    
    static ESP_Color target_color = {.r = 1, .g = 0, .b = 1, .a = 1};
    
    rbx_print_descendants(task, workspace, 0, 2);
    rbx_print_descendants(task, camera, 0, 3);
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            vm_address_t players_folder = rbx_instance_find_first_child(task, workspace, "Players");
            vm_address_t blue_team_folder = rbx_instance_find_first_child(task, players_folder, "Bright blue");
            vm_address_t orange_team_folder = rbx_instance_find_first_child(task, players_folder, "Bright orange");
            vm_address_t my_team = rbx_player_get_team(task, local_player);
            
            long camera_child_count = 0;
            rbx_child_t* camera_children = rbx_instance_get_children(task, camera, &camera_child_count);
            if (camera_children)
            {
                vm_address_t main = camera_children[2].child_address;
                trigger = rbx_instance_find_first_child(task, main, "Trigger");
                if (trigger)
                {
                    trigger_cframe_address = rbx_basepart_get_properties_address(task, trigger) + RBX_PART_PROPERTIES_CFRAME_OFFSET;
                }
                vm_deallocate(mach_task_self_, (vm_address_t)camera_children, camera_child_count * sizeof(rbx_child_t));
            }
            
            vm_address_t enemy_team_folder = 0;
            
            if (my_team == phantoms_team)
            {
                enemy_team_folder = orange_team_folder;
            }
            if (my_team == ghosts_team)
            {
                enemy_team_folder = blue_team_folder;
            }
            
            bool __cee = false;
            float old_delta_ratio = max_delta_ratio;
            float old_dist = max_dist;
            int enemy_torso_index = 0;
            
            rbx_child_t* enemies_list = rbx_instance_get_children(task, enemy_team_folder, &enemy_count);
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
                            
                            rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, torso);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            const vector3_t camera_look_vector = rbx_get_cframe_look_vector(camera_cframe);
                            const float dist = vector3_dist_dif(torso_cframe.pos, camera_cframe.pos);
                            const vector3_t f_offset = vector3_mul_num(camera_look_vector, dist);
                            const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                            const float delta_dist = vector3_dist_dif(f_pos, torso_cframe.pos);
                            const float delta_ratio = (delta_dist/dist);
                            
                            if (old_dist > dist && old_delta_ratio > delta_ratio)
                            {
                                __cee = true;
                                //old_delta_ratio = delta_ratio;
                                old_dist = dist;
                                closest_enemy_torso = torso;
                            }
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)enemies_list, enemy_count * sizeof(rbx_child_t));
            }
            closest_enemy_exists = __cee;
            
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

            usleep(300000);
        }
    });
    
    if (esp_enabled)
    {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                       {
            for (;;)
            {
                if (enemy_count > 0 && enemy_count < (RBX_PHANTOM_FORCES_MAX_PLAYER_COUNT + 1))
                {
                    int esp_index = 0;
                    for (long i = 0 ; i < enemy_count ; i++)
                    {
                        ESP_Color color = esp_color;
                        float border_width = 2;
                        vm_address_t enemy_torso = enemy_torsos[i];
                        if (enemy_torso)
                        {
                            if (enemy_torso == closest_enemy_torso && closest_enemy_exists)
                            {
                                color = target_color;
                                border_width = 3;
                            }
                            vm_address_t parent = rbx_instance_get_parent(task, enemy_torso);
                            if (parent)
                            {
                                rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, enemy_torso);
                                rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                                float fov = rbx_camera_get_field_of_view(task, camera);
                                if (esp_index < MAX_ESP_COUNT)
                                {
                                    rbx_draw_esp_box(task, torso_cframe.pos,
                                                     camera_cframe, esp_box_hidden_array,
                                                     esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, border_width,
                                                     color, fov, 3, 3, 0, 0, window_w, window_h, esp_index, true);
                                    esp_index++;
                                }
                            }
                        }
                        usleep(esput/enemy_count);
                        
                    }
                    if (esp_index < MAX_ESP_COUNT)
                    {
                        char hiddens[MAX_ESP_COUNT - esp_index];
                        memset(hiddens, 1, sizeof(hiddens));
                        vm_write(task, esp_box_hidden_array + esp_index, (vm_address_t)hiddens, (int)sizeof(hiddens));
                    }
                }
                else
                {
                    sleep(1);
                }
            }
        });
    }
    
    /*
     0x10EB36D18    str q1, [x2]    41 00 80 3D
     0x10EB36D1C    str q5, [x2, #0x10]    45 04 80 3D
     0x10EB36D20    str q7, [x2, #0x20]    47 08 80 3D
     */
    

        
       
    if (aimbot_enabled)
    {
        
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^
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
                    if (closest_enemy_exists == true && closest_enemy_torso)
                    {
                        aimbot_usleep_time = 50;
                        if (lmd == true)
                        {
                            aimbot_usleep_time = 0;
                            rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, closest_enemy_torso);
                            torso_cframe.pos.y -= 0.25;
                            rbx_cframe_t trigger_cframe = rbx_basepart_get_cframe(task, trigger);
                            
                            vector3_t new_look_vector = vector3_unit(torso_cframe.pos, trigger_cframe.pos);
                            
                            new_trigger_cframe_r20 = -new_look_vector.x;
                            new_trigger_cframe_r21 = -new_look_vector.y;
                            new_trigger_cframe_r22 = -new_look_vector.z;
                            
                            vm_write(task, trigger_cframe_address + 0x8, (vm_offset_t)&new_trigger_cframe_r20, 4);
                            vm_write(task, trigger_cframe_address + 0x14, (vm_offset_t)&new_trigger_cframe_r21, 4);
                            vm_write(task, trigger_cframe_address + 0x20, (vm_offset_t)&new_trigger_cframe_r22, 4);
                        }
                    }
                    else
                    {
                        aimbot_usleep_time = 4000;
                    }
                }
                usleep(aimbot_usleep_time);
            }
        });
    }
        
}
