

void arsenal_cheat(task_t task)
{
    printf("- ARSENAL (AIMBOT + ESP) -\n");
    
    void* dlhandle = dlopen(__LIBESP_DYLIB_PATH__, RTLD_NOW);
    if (!dlhandle)
    {
        printf("failed to dlopen()\n");
    }
    
    vm_address_t s_load_address = task_get_image_address_by_path(mach_task_self_, __LIBESP_DYLIB_PATH__);
    
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
    
    vm_address_t load_address = task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    if (!load_address)
    {
        printf("Couldn't find libESP.dylib in task %d\n", task);
        exit(0);
    }
    
    static char esp_enabled = true;
    static char aimbot_enabled = true;
    
    const static int max_player_count = 16;
    
    
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, sizeof(char));
    int esp_count = max_player_count;
    vm_write(task, load_address + esp_count_offset, (vm_offset_t)&esp_count, sizeof(int));
    
    vm_address_t esp_box_hidden_array = load_address + esp_box_hidden_array_offset;
    vm_address_t esp_box_frame_array = load_address + esp_box_frame_array_offset;
    vm_address_t esp_box_color_array = load_address + esp_box_color_array_offset;
    vm_address_t esp_box_border_width_array = load_address + esp_box_border_width_array_offset;

    static float window_w;
    static float window_h;
    
    useconds_t esput = 1000;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t player_gui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
    vm_address_t nametags_folder = rbx_instance_find_first_child(task, player_gui, "Nametags");
    
    //rbx_print_descendants(task, local_player, 0, 1);
    
    static vm_address_t enemy_heads[max_player_count];
    static long enemy_count = 0;
    
    static vm_address_t closest_enemy_head;
    static char closest_enemy_exists = false;
    
    static float max_dist = 500;
    static float max_delta_dist = 10;
    static float interpolation = 5;
    
    static ESP_Color esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    static ESP_Color target_color = {.r = 1, .g = 0, .b = 1, .a = 1};

    static useconds_t trigger_usleep_time = 75;
    
    
    
    
#pragma mark - Object Loop -
    
    

    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            window_w = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_w_offset))).f;
            window_h = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_h_offset))).f;
            
            bool __cee = false;
            float old_dist = max_dist;
            float old_delta_dist = max_delta_dist;
            int enemy_head_index = 0;
            
            long player_count = 0;
            rbx_child_t* player_list = rbx_instance_get_children(task, players_service, &player_count);
            if (player_list)
            {
                for (int i = 0 ; i < player_count ; i++)
                {
                    vm_address_t player = player_list[i].child_address;
                    if (player && player != local_player)
                    {
                        long pnl = 0;
                        char* pn = rbx_instance_get_name(task, player, &pnl);
                        if (pn)
                        {
                            if (!rbx_instance_find_first_child(task, nametags_folder, pn))
                            {
                                vm_address_t character = rbx_player_get_character(task, player);
                                if (character)
                                {
                                    //vm_address_t head = rbx_instance_find_first_child(task, character, "Head");
                                    //Getting caught is pretty easy when we stack headshots.
                                    vm_address_t head = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
                                    if (head)
                                    {
                                        vm_address_t parent = rbx_instance_get_parent(task, head);
                                        vm_address_t instance_self = rbx_instance_get_self(task, head);
                                        if (parent && instance_self == head)
                                        {
                                            enemy_heads[enemy_head_index++] = head;
                                            rbx_cframe_t head_cframe = rbx_basepart_get_cframe(task, head);
                                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                                            vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);
                                            float dist;
                                            const float depth = rbx_get_camera_relative_depth(camera_cframe, head_cframe.pos, &dist);
                                            const vector3_t f_offset = vector3_mul_num(camera_look_vector, depth);
                                            const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                                            const float delta_dist = vector3_dist_dif(f_pos, head_cframe.pos);
                                            if (old_dist > dist && old_delta_dist > delta_dist)
                                            {
                                                __cee = true;
                                                //old_dist = dist;
                                                old_delta_dist = delta_dist;
                                                closest_enemy_head = head;
                                            }
                                        }
                                    }
                                }
                            }
                            vm_deallocate(mach_task_self_, (vm_address_t)pn, pnl);
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)player_list, player_count * sizeof(rbx_child_t));
            }
            closest_enemy_exists = __cee;
            enemy_count = enemy_head_index;
            usleep(200000);
        }
    });
    
    
    
    
    
#pragma mark - Aimbot -
    
    

    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (aimbot_enabled &&
                closest_enemy_exists &&
                closest_enemy_head)
            {
                char lmd = vm_read_1byte_value(task, load_address + left_mouse_down_offset);
                if (lmd)
                {
                    trigger_usleep_time = 3;
                    
                    rbx_cframe_t head_cframe = rbx_basepart_get_cframe(task, closest_enemy_head);
                    rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                    vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);
                    float dist;
                    const float depth = rbx_get_camera_relative_depth(camera_cframe, head_cframe.pos, &dist);
                    const vector3_t f_offset = vector3_mul_num(camera_look_vector, depth);
                    const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                    const float delta_dist = vector3_dist_dif(f_pos, head_cframe.pos);
                    if (max_delta_dist > delta_dist)
                    {
                        vector3_t new_look_vector = vector3_unit(head_cframe.pos, camera_cframe.pos);
                        vector3_t inc_look_vector = vector3_div_num(vector3_sub(new_look_vector, camera_look_vector), interpolation);
                        camera_cframe.r20 += -inc_look_vector.x;
                        camera_cframe.r21 += -inc_look_vector.y;
                        camera_cframe.r22 += -inc_look_vector.z;
                        rbx_camera_set_cframe(task, camera, camera_cframe);
                    }
                    
                }
                else
                {
                    trigger_usleep_time = 30;
                }
            }
            else
            {
                trigger_usleep_time = 75;
            }
            usleep(trigger_usleep_time);
        }
    });
    
    
    
    
    
#pragma mark - ESP -
    
    

    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (esp_enabled &&
                enemy_count > 0 &&
                enemy_count < (max_player_count + 1))
            {
                int esp_index = 0;
                for (long i = 0 ; i < enemy_count ; i++)
                {
                    ESP_Color color = esp_color;
                    float border_width = 2;
                    vm_address_t enemy_head = enemy_heads[i];
                    if (enemy_head)
                    {
                        if (aimbot_enabled && enemy_head == closest_enemy_head && closest_enemy_exists)
                        {
                            color = target_color;
                            border_width = 3;
                        }
                        vm_address_t parent = rbx_instance_get_parent(task, enemy_head);
                        vm_address_t instance_self = rbx_instance_get_self(task, enemy_head);
                        if (parent && instance_self == enemy_head)
                        {
                            rbx_cframe_t head_cframe = rbx_basepart_get_cframe(task, enemy_head);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            rbx_draw_esp_box(task, head_cframe.pos,
                                             camera_cframe, esp_box_hidden_array,
                                             esp_box_frame_array, esp_box_color_array,esp_box_border_width_array, border_width,
                                             color, fov, 3, 3, 0, 0, window_w, window_h, esp_index, true);
                            esp_index++;
                        }
                    }
                    usleep(esput/enemy_count);
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
    
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    
    
    
    
    printf("List of stdin commands:\n");
    printf("\tesp\n");
    printf("\tnoesp\n");
    printf("\taimbot\n");
    printf("\tnoaimbot\n");
    printf("\taimbotsize [float]\n");
    printf("\taimbotinterpolation [float]\n");
    printf("\tquit\n");
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            char str[100];
            bzero(str, 100);
            fgets(str, 100, stdin);
            
            if (strcmp(str, "esp\xa") == 0)
            {
                esp_enabled = true;
                printf("ENABLED ESP\n");
            }
            if (strcmp(str, "noesp\xa") == 0)
            {
                esp_enabled = false;
                printf("DISABLED ESP\n");
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
            if (strncmp(str, "aimbotsize ", strlen("aimbotsize ")) == 0)
            {
                sscanf(str, "aimbotsize %f", &max_delta_dist);
                printf("SET AIMBOT SIZE TO %f STUD(S)\n", max_delta_dist);
            }
            if (strncmp(str, "aimbotinterpolation ", strlen("aimbotinterpolation ")) == 0)
            {
                sscanf(str, "aimbotinterpolation %f", &interpolation);
                printf("SET AIMBOT INTERPOLATOION TO %f ADJUSTMENT(S)\n", interpolation);
            }
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
    
    
}
