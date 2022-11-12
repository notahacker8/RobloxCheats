
#define RBX_ARSENAL_MAX_PLAYER_COUNT 16

void arsenal_cheat(task_t task)
{
    printf("- ARSENAL -\n");
    static mach_msg_type_number_t data_cnt;
    
    void* handle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
    vm_offset_t left_mouse_down_offset = gdso(handle, s_load_address, "LEFT_MOUSE_DOWN");
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
    
    useconds_t esput = 500;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static vm_address_t enemy_heads[RBX_ARSENAL_MAX_PLAYER_COUNT];
    static long enemy_count = 0;
    
    static vm_address_t closest_enemy_head;
    static char closest_enemy_exists = false;
    static const float max_delta_ratio = 0.175;
    
    static ESP_Color esp_color;
    esp_color.r = 0;
    esp_color.g = 1;
    esp_color.b = 0;
    esp_color.a = 1;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
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
            
            bool __cee = false;
            float old_dist = 999999;
            int enemy_head_index = 0;
            
            long player_count = 0;
            rbx_child_t* player_list = rbx_instance_get_children(task, players_service, &player_count);
            
            if (player_list)
            {
                for (int i = 0 ; i < player_count ; i++)
                {
                    vm_address_t player = player_list[i].child_address;
                    if (player)
                    {
                        
                        vm_address_t my_team = rbx_player_get_team(task, local_player);
                        vm_address_t team = rbx_player_get_team(task, player);
                        if (team != my_team)
                        {
                            vm_address_t character = rbx_player_get_character(task, player);
                            if (character)
                            {
                                vm_address_t head = rbx_instance_find_first_child(task, character, "Head");
                                if (head)
                                {
                                    enemy_heads[enemy_head_index] = head;
                                    enemy_head_index++;
                                    rbx_cframe_t head_cframe = rbx_basepart_get_cframe(task, head);
                                    rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                                    const vector3_t camera_look_vector = rbx_get_cframe_look_vector(camera_cframe);
                                    const float dist = vector3_dist_dif(head_cframe.pos, camera_cframe.pos);
                                    const vector3_t f_offset = vector3_mul_num(camera_look_vector, dist);
                                    const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                                    const float delta_dist = vector3_dist_dif(f_pos, head_cframe.pos);
                                    const float delta_ratio = (delta_dist/dist);
                                    if (old_dist > dist)
                                    {
                                        if (max_delta_ratio > delta_ratio)
                                        {
                                            __cee = true;
                                            old_dist = dist;
                                            closest_enemy_head = head;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)player_list, player_count * sizeof(rbx_child_t));
            }
            closest_enemy_exists = __cee;
            enemy_count = enemy_head_index;
            usleep(300000);
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
                    const vector3_t camera_look_vector = rbx_get_cframe_look_vector(camera_cframe);
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
            usleep(300);
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
                    
                    vm_address_t enemy_head = enemy_heads[i];
                    if (enemy_head)
                    {
                        vm_address_t parent = rbx_instance_get_parent(task, enemy_head);
                        if (parent)
                        {
                            rbx_cframe_t head_cframe = rbx_basepart_get_cframe(task, enemy_head);
                            //printf("%p [%d] (%f %f %f)\n", enemy_head, i, head_cframe.pos.x, head_cframe.pos.y, head_cframe.pos.z);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            rbx_draw_esp_box(task, head_cframe.pos,
                                             camera_cframe, esp_box_hidden_array,
                                             esp_box_frame_array, esp_box_color_array,
                                             esp_color, fov, 3, 3, 0, 0, window_w, window_h, esp_index, true);
                            float border_width = 2;
                            vm_write(task, esp_box_border_width_array + (esp_index * 4), (vm_address_t)&border_width, 4);
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
