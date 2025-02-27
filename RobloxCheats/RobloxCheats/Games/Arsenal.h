

void arsenal_cheat(task_t task)
{
    printf("\n- ARSENAL -\n");
    LIBESP_DYLIB_SETUP
    
    vm_address_t game = rbx_find_game_address(task);
    //rbx_print_descendants(task, game, 0, 1);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static char aimbot_enabled = true;
    static char last_resort_enabled = false;
    
    static float max_delta_dist = 8;
    static float max_dist = 400;
    
    static char esp_enabled = true;
    static useconds_t esput = 1200;
    
    static ESP_Color esp_color = {.r = 0, .g = 1, .b = 0, .a = 0.5};
    static ESP_Color target_color = {.r = 1, .g = 0, .b = 1, .a = 0.5};

    shared_memory->ESP_COUNT = 32; //we only draw boxes on players, Nothing else
    shared_memory->ESP_ENABLED = esp_enabled;
    shared_memory->ESP_USLEEP_TIME = esput;
    
    static long enemy_count = 0;
    static vm_address_t enemy_torsos[MAX_ESP_COUNT];
    static vm_address_t closest_enemy_torso = 0;
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            
            my_character = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            
            vm_address_t __cet = 0; //Stands for Closest Enemy Torso
            float old_delta_dist = max_delta_dist;
            float old_dist = max_dist;
            
            long enemy_index = 0;
            long child_count = 0;
            rbx_child_t* players_list = rbx_instance_get_children(task, players_service, &child_count);
            if (players_list)
            {
                for (long i = 0 ; i < child_count ; i++)
                {
                    vm_address_t player = players_list[i].child_address;
                    vm_address_t plr_team = rbx_player_get_team(task, player);
                    vm_address_t my_team = rbx_player_get_team(task, local_player);
                    if (plr_team != my_team)
                    {
                        vm_address_t character = rbx_player_get_character(task, player);
                        //rbx_print_descendants(task, character, 0, 1);
                        vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
                        if (hrp)
                        {
                            vm_address_t parent = rbx_instance_get_parent(task, hrp);
                            vm_address_t instance_self = rbx_instance_get_self(task, hrp);
                            if (hrp && parent && instance_self == hrp)
                            {
                                enemy_torsos[enemy_index++] = hrp;
                                rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, hrp);
                                rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                                
                                float dist;
                                const float depth = rbx_get_camera_relative_depth(camera_cframe, torso_cframe.pos, &dist);
                                
                                const vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);
                                const vector3_t f_offset = vector3_mul_num(camera_look_vector, depth);
                                const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                                const float delta_dist = vector3_dist_dif(f_pos, torso_cframe.pos);
                                
                                if (old_dist > dist && old_delta_dist > delta_dist)
                                {
                                    __cet = hrp;
                                    //old_dist = dist;
                                    old_delta_dist = delta_dist;
                                }
                            
                            }
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)players_list, child_count * sizeof(rbx_child_t));
            }
            closest_enemy_torso = __cet;
            enemy_count = enemy_index;
            enemy_count = enemy_index;
            usleep(200000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            COPY_VAR(enemy_count)
            if (esp_enabled && __enemy_count > 0)
            {
                int esp_index = 0;
                for (long i = 0 ; i < __enemy_count ; i++)
                {
                    ESP_Color color = esp_color;
                    float border_width = 2;
                    vm_address_t enemy_torso = enemy_torsos[i];
                    if (enemy_torso)
                    {
                        if (closest_enemy_torso && enemy_torso == closest_enemy_torso &&
                            (last_resort_enabled || aimbot_enabled))
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
                            rbx_draw_esp_box_on_3d_point(task, torso_cframe.pos,
                                             camera_cframe, shared_memory, border_width,
                                             color, fov, 3, 3, 0, 0, shared_memory->WINDOW_W, shared_memory->WINDOW_H, esp_index, true);
                            esp_index++;
                        }
                    }
                    usleep(esput/__enemy_count);
                    
                }
                memset(shared_memory->ESP_BOX_HIDDEN_ARRAY + esp_index, 1, MAX_ESP_COUNT - esp_index);
            }
            else
            {
                sleep(1);
            }
        }
    });
    
    static useconds_t aimbot_usleep_time = 75;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (aimbot_enabled || last_resort_enabled)
            {
                if (rbx_instance_is_valid_child(task, closest_enemy_torso))
                {
                    if (shared_memory->LEFT_MOUSE_DOWN == true)
                    {
                        aimbot_usleep_time = 10;
                        rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, closest_enemy_torso);
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        
                        float dist;
                        const float depth = rbx_get_camera_relative_depth(camera_cframe, torso_cframe.pos, &dist);
                        
                        const vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);
                        const vector3_t f_offset = vector3_mul_num(camera_look_vector, depth);
                        const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                        const float delta_dist = vector3_dist_dif(f_pos, torso_cframe.pos);
                        
                        if (max_dist > dist && max_delta_dist > delta_dist)
                        {
                            if (aimbot_enabled)
                            {
                                vector3_t new_lv = vector3_direction_to_from(torso_cframe.pos, camera_cframe.pos);
                                camera_cframe.r20 = -new_lv.x;
                                camera_cframe.r21 = -new_lv.y;
                                camera_cframe.r22 = -new_lv.z;
                                rbx_camera_set_cframe(task, camera, camera_cframe);
                            }
                            if (last_resort_enabled)
                            {
                                rbx_cframe_t new_cf = torso_cframe;
                                vector3_t torso_lv = rbx_cframe_get_look_vector(new_cf);
                                new_cf.pos = vector3_add(new_cf.pos, vector3_mul_num(torso_lv, -5));
                                rbx_basepart_set_cframe(task, my_hrp, new_cf);
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
    
    
#pragma mark - stdin Inputs -
    
    
    
    printf("List of stdin commands:\n");
    printf("\tesp\n");
    printf("\tnoesp\n");
    printf("\taimbot\n");
    printf("\tnoaimbot\n");
    printf("\tlr\n");
    printf("\tnolr\n");
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
            if (strcmp(str, "lr\xa") == 0)
            {
                last_resort_enabled = true;
                printf("ENABLED LAST RESORT\n");
            }
            if (strcmp(str, "nolr\xa") == 0)
            {
                last_resort_enabled = false;
                printf("DISABLED LAST RESORT\n");
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
