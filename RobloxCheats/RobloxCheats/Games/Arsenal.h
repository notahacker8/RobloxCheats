

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
    
    static ESP_Color esp_color = {.r = 0, .g = 1, .b = 0, .a = 0.5};
    static ESP_Color target_color = {.r = 1, .g = 0, .b = 1, .a = 0.5};
    
    ESP_GLOBAL_VARIABLES

    shared_memory->ESP_COUNT = 32 * 3; //we only draw boxes on players, Nothing else
    shared_memory->ESP_ENABLED = esp_enabled;
    shared_memory->ESP_USLEEP_TIME = 1000;
    
    static long enemy_count = 0;
    static vm_address_t enemy_torsos[MAX_ESP_COUNT];
    static vm_address_t closest_enemy_torso = 0;
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            int esp_part_index = 0;
            
            my_character = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            
            vm_address_t __cet = 0; //Stands for Closest Enemy Torso
            float old_delta_dist = max_delta_dist;
            float old_dist = max_dist;
            
            int enemy_index = 0;
            
            FOR_EACH_CHILD(players_service, i, {
                vm_address_t player = child;
                vm_address_t plr_team = rbx_player_get_team(task, player);
                vm_address_t my_team = rbx_player_get_team(task, local_player);
                if (plr_team != my_team)
                //if (player != local_player)
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
                            long nm_l = 0;
                            char* nm = rbx_instance_get_name(task, player, &nm_l);
                            if (nm)
                            {
                                APPEND_ESP_OBJ(hrp, esp_color, nm);
                                vm_deallocate(mach_task_self(), (vm_address_t)nm, nm_l);
                            }
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
            })
            
            closest_enemy_torso = __cet;
            enemy_count = enemy_index;
            
            esp_part_count = esp_part_index;
            usleep(200000);
        }
    });
    
    BASIC_ESP_ENGINE({}, {
        if (part == closest_enemy_torso && (last_resort_enabled || aimbot_enabled))
        {
            esp_color = target_color;
            border_width = 3;
        }
    }, 100, {});
    
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
    
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("esp", "", {
            esp_enabled = true;
            printf("ENABLED ESP\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("noesp", "", {
            esp_enabled = false;
            printf("DISABLED ESP\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("aimbot", "", {
            aimbot_enabled = true;
            printf("ENABLED AIMBOT\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("noaimbot", "", {
            aimbot_enabled = false;
            printf("DISABLED AIMBOT\n");
        })
        INSERT_STDIN_MULTIARG_INPUT("aimbotsize %f", "", {
        }, 1, &max_delta_dist);
        INSERT_STDIN_INPUT_SIMPLE("lr", "", {
            last_resort_enabled = true;
            printf("ENABLED LAST RESORT\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("nolr", "", {
            last_resort_enabled = false;
            printf("DISABLED LAST RESORT\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("quit", "", exit(0);)
    })
    
}
