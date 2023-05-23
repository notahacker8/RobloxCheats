
#define RBX_PHANTOM_FORCES_MAX_PLAYER_COUNT 32

void phantom_forces_cheat(task_t task)
{
    printf("- PHANTOM FORCES (AIMBOT + ESP + MELEE) -\n");
    
    void* dlhandle = dlopen(__LIBESP_DYLIB_PATH__, RTLD_NOW);
    if (!dlhandle)
    {
        printf("%s\n", dlerror());
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
    
    vm_address_t load_address =  task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    if (!load_address)
    {
        printf("Couldn't find libESP.dylib in task %d\n", task);
        exit(0);
    }
    
    static char esp_enabled = true;
    static char aimbot_enabled = true;
    
    const static int max_player_count = 32;
    
    static float max_delta_dist = 8;
    static float max_dist = 500;
    
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
    
    
    useconds_t esput = 1000;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    static vm_address_t closest_enemy_torso;
    static char closest_enemy_exists = false;
    static char is_holding_melee = false;
    static int cframe_write_mode = 0;
    
    static vm_address_t enemy_torsos[max_player_count];
    
    static vm_address_t trigger = 0;
    static vm_address_t flame = 0;
    static vm_address_t trigger_cframe_address = 0;
    static vm_address_t flame_cframe_address = 0;
    
    static float new_trigger_cframe_r20 = 0.0f;
    static float new_trigger_cframe_r21 = 0.0f;
    static float new_trigger_cframe_r22 = 0.0f;
    static vector3_t new_flame_pos = {.x = 0, .y = 0, .z = 0};
    
    static long current_enemy_count = 0;
    static useconds_t aimbot_usleep_time = 75;
    static useconds_t cframe_write_usleep_time = 5;

    static ESP_Color esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    static ESP_Color target_color = {.r = 1, .g = 0, .b = 1, .a = 1};
    
    static long camera_child_count = 0;
    
    
    

#pragma mark - Object Loop -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            window_w = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_w_offset))).f;
            window_h = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_h_offset))).f;
            
            vm_address_t players_folder = rbx_instance_find_first_child(task, workspace, "Players");
            vm_address_t blue_team_folder = rbx_instance_find_first_child(task, players_folder, "Bright blue");
            vm_address_t orange_team_folder = rbx_instance_find_first_child(task, players_folder, "Bright orange");
            vm_address_t my_team = rbx_player_get_team(task, local_player);
            
            rbx_child_t* camera_children = rbx_instance_get_children(task, camera, &camera_child_count);
            if (camera_children)
            {
                if (camera_child_count > 2)
                {
                    vm_address_t main = camera_children[2].child_address;
                    vm_address_t handle = rbx_instance_find_first_child(task, main, "Handle");
                    if (handle)
                    {
                        flame = rbx_instance_find_first_child(task, main, "Flame");
                        if (rbx_instance_get_parent(task, flame) && rbx_instance_get_self(task, flame) == flame)
                        {
                            flame_cframe_address = rbx_basepart_get_properties_address(task, flame) + RBX_PART_PROPERTIES_CFRAME_OFFSET;
                        }
                        else
                        {
                            flame_cframe_address = 0;
                        }
                        is_holding_melee = true;
                    }
                    else
                    {
                        is_holding_melee = false;
                    }
                    trigger = rbx_instance_find_first_child(task, main, "Trigger");
                    if (rbx_instance_get_parent(task, trigger) && rbx_instance_get_self(task, trigger) == trigger)
                    {
                        trigger_cframe_address = rbx_basepart_get_properties_address(task, trigger) + RBX_PART_PROPERTIES_CFRAME_OFFSET;
                    }
                    else
                    {
                        trigger_cframe_address = 0;
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)camera_children, camera_child_count * sizeof(rbx_child_t));
            }
            else
            {
                flame = 0;
                trigger = 0;
                flame_cframe_address = 0;
                trigger_cframe_address = 0;
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
            float old_delta_dist = max_delta_dist;
            float old_dist = max_dist;
            
            int enemy_torso_index = 0;
            long enemy_count = 0;
            rbx_child_t* enemies_list = rbx_instance_get_children(task, enemy_team_folder, &enemy_count);
            if (enemies_list)
            {
                for (int i = 0 ; i < enemy_count ; i++)
                {
                    vm_address_t enemy = enemies_list[i].child_address;
                    if (enemy)
                    {
                        vm_address_t torso = rbx_instance_find_first_child(task, enemy, "Torso");
                        vm_address_t parent = rbx_instance_get_parent(task, torso);
                        vm_address_t instance_self = rbx_instance_get_self(task, torso);
                        if (torso && parent && instance_self == torso)
                        {
                            enemy_torsos[enemy_torso_index++] = torso;
                            
                            rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, torso);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            float dist;
                            const float depth = rbx_get_camera_relative_depth(camera_cframe, torso_cframe.pos, &dist);
                            
                            if (!is_holding_melee)
                            {
                                const vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);
                                const vector3_t f_offset = vector3_mul_num(camera_look_vector, depth);
                                const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                                const float delta_dist = vector3_dist_dif(f_pos, torso_cframe.pos);
                                
                                if (old_dist > dist && old_delta_dist > delta_dist)
                                {
                                    __cee = true;
                                    //old_dist = dist;
                                    old_delta_dist = delta_dist;
                                    closest_enemy_torso = torso;
                                }
                            }
                            else
                            {
                                if (dist < 15) //Max is 22 before the game stops registering hits with melees.
                                {
                                    closest_enemy_torso = torso;
                                    __cee = true;
                                }
                            }
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)enemies_list, enemy_count * sizeof(rbx_child_t));
            }
            closest_enemy_exists = __cee;
            current_enemy_count = enemy_torso_index;

            usleep(300000);
        }
    });
    
    
    

    
#pragma mark - ESP -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (esp_enabled &&
                current_enemy_count > 0 &&
                current_enemy_count < (RBX_PHANTOM_FORCES_MAX_PLAYER_COUNT + 1))
            {
                int esp_index = 0;
                for (long i = 0 ; i < current_enemy_count ; i++)
                {
                    ESP_Color color = esp_color;
                    float border_width = 2;
                    vm_address_t enemy_torso = enemy_torsos[i];
                    if (enemy_torso)
                    {
                        if (enemy_torso == closest_enemy_torso && closest_enemy_exists && aimbot_enabled)
                        {
                            color = target_color;
                            border_width = 3;
                        }
                        vm_address_t parent = rbx_instance_get_parent(task, enemy_torso);
                        vm_address_t instance_self = rbx_instance_get_self(task, enemy_torso);
                        if (parent && instance_self == enemy_torso)
                        {
                            rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, enemy_torso);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            rbx_draw_esp_box(task, torso_cframe.pos,
                                             camera_cframe, esp_box_hidden_array,
                                             esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, border_width,
                                             color, fov, 3, 3, 0, 0, window_w, window_h, esp_index, true);
                            esp_index++;
                        }
                    }
                    usleep(esput/current_enemy_count);
                    
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
    
    
    
    
    
#pragma mark - Aimbot + Melee -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (aimbot_enabled &&
                closest_enemy_exists &&
                closest_enemy_torso &&
                (flame || trigger) &&
                camera_child_count > 2)
            {
                char lmd = vm_read_1byte_value(task, load_address + left_mouse_down_offset);
                if (lmd)
                {
                    cframe_write_usleep_time = 0;
                    aimbot_usleep_time = 50;
                    rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, closest_enemy_torso);
                    if (!is_holding_melee)
                    {
                        rbx_cframe_t trigger_cframe = rbx_basepart_get_cframe(task, trigger);
                        torso_cframe.pos.y -= 0.20;
                        vector3_t torso_velocity = rbx_basepart_get_velocity(task, closest_enemy_torso);
                        torso_cframe.pos = vector3_add(torso_cframe.pos, torso_velocity);
                        vector3_t new_look_vector = vector3_unit(torso_cframe.pos, trigger_cframe.pos);
                        
                        new_trigger_cframe_r20 = -new_look_vector.x;
                        new_trigger_cframe_r21 = -new_look_vector.y;
                        new_trigger_cframe_r22 = -new_look_vector.z;
                        cframe_write_mode = 1;
                    }
                    else
                    {
                        new_flame_pos = torso_cframe.pos;
                        cframe_write_mode = 2;
                    }
                }
                else
                {
                    cframe_write_usleep_time = 100;
                    aimbot_usleep_time = 50;
                    cframe_write_mode = 0;
                }
            }
            else
            {
                cframe_write_usleep_time = 100;
                cframe_write_mode = 0;
                aimbot_usleep_time = 4000;
            }
            usleep(aimbot_usleep_time);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^
                   {
        for (;;)
        {
            if (cframe_write_mode == 1)
            {
                vm_write(task, trigger_cframe_address + 0x8, (vm_offset_t)&new_trigger_cframe_r20, 4);
                vm_write(task, trigger_cframe_address + 0x14, (vm_offset_t)&new_trigger_cframe_r21, 4);
                vm_write(task, trigger_cframe_address + 0x20, (vm_offset_t)&new_trigger_cframe_r22, 4);
            }
            if (cframe_write_mode == 2)
            {
                vm_write(task, flame_cframe_address + 0x24, (vm_offset_t)&new_flame_pos, sizeof(vector3_t));
            }
            usleep(cframe_write_usleep_time);
        }
    });
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    
    
    
    
    printf("List of stdin commands:\n");
    printf("\tesp\n");
    printf("\tnoesp\n");
    printf("\taimbot\n");
    printf("\tnoaimbot\n");
    printf("\taimbotsize [(float)]\n");
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
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
    
}
