


void phantom_forces_cheat(task_t task)
{
    printf("- PHANTOM FORCES -\n");
    
    LIBESP_DYLIB_SETUP
    
    static char aimbot_enabled = true;
    
    const int max_player_count = 32;
    
    static float aimbot_size = 15;
    static float max_dist = 400;
    
    static float inacc = 0;
    
    static float target_character_part_volume = 2 * 2 * 1; //Heads is 1 * 1 * 1
    
    static useconds_t esput = 1000;
    
    ESP_GLOBAL_VARIABLES;

    shared_memory->ESP_COUNT = max_player_count * 2;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = esput;

    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    //rbx_print_descendants(task, workspace, 0, 3);
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    //rbx_print_descendants(task, camera, 0, 1);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t teams_service = rbx_instance_find_first_child_of_class(task, game, "Teams");
    vm_address_t phantoms_team = rbx_instance_find_first_child(task, teams_service, "Phantoms");
    vm_address_t ghosts_team = rbx_instance_find_first_child(task, teams_service, "Ghosts");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static vm_address_t my_character = 0;
    static vm_address_t my_head = 0;
    static vm_address_t my_hrp = 0;
    
    static vm_address_t closest_enemy_torso = 0;
    static vm_address_t closest_enemy_torso_cframe_address = 0;
    
    static int cframe_write_mode = 0;
    
    static vm_address_t enemy_team_folder = 0;
    
    static vm_address_t enemy_torsos[200];
    
    static vm_address_t trigger = 0;
    static vm_address_t trigger_cframe_address = 0;
    
    static vm_address_t handle = 0;
    static vm_address_t handle_cframe_address = 0;
    
    static vm_address_t camera_part = 0;
    static vm_address_t camera_part_cframe_address = 0;
    
    static int __t__idx = 0;
    
    static float new_trigger_cframe_r20 = 0.0f;
    static float new_trigger_cframe_r21 = 0.0f;
    static float new_trigger_cframe_r22 = 0.0f;
    
    static float new_cam_part_cframe_r20 = 0.0f;
    static float new_cam_part_cframe_r21 = 0.0f;
    static float new_cam_part_cframe_r22 = 0.0f;
    
    static long current_enemy_count = 0;
    static useconds_t cframe_write_usleep_time_1 = 1000;
    static useconds_t cframe_write_usleep_time_2 = 1000;

    static ESP_Color esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color target_color = {.r = 1, .g = 0, .b = 1, .a = 1};
    
    static long camera_child_count = 0;
    

#pragma mark - Object Loops -
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            my_character = rbx_instance_get_parent(task, rbx_camera_get_camera_subject(task, camera));
            my_hrp = rbx_model_get_primarypart(task, my_character);
            
            vm_address_t players_folder = rbx_instance_find_first_child_of_pointee_and_name(task, task_base_address, workspace, RBX_OBJECT_Folder_POINTEE_OFFSET, "Players");
            enemy_team_folder = rbx_instance_child_at_index(task, players_folder, __t__idx);
            
            vm_address_t new_trigger = 0;
            vm_address_t new_trigger_cframe_address = 0;
            
            vm_address_t new_handle = 0;
            vm_address_t new_handle_cframe_address = 0;
            
            vm_address_t new_camera_part = 0;
            vm_address_t new_camera_part_cframe_address = 0;
            
            rbx_child_t* camera_children = rbx_instance_get_children(task, camera, &camera_child_count);
            if (camera_children)
            {
                if (camera_child_count > 0)
                {
                    vm_address_t main = camera_children[camera_child_count - 1].child_address;
                    new_camera_part = rbx_instance_find_first_child_by_pointee(task, camera, task_base_address, RBX_OBJECT_Part_POINTEE_OFFSET);
                    new_camera_part_cframe_address = rbx_basepart_get_properties_address(task, camera_part) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                    
                    FOR_EACH_CHILD(main, i, {
                        if (rbx_instance_is_named(task, child, "Handle"))
                        {
                            new_handle = child;
                            new_handle_cframe_address = rbx_basepart_get_properties_address(task, handle) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                        }
                        if (vm_read_uint64_t_value(task, child) == task_base_address + RBX_OBJECT_Part_POINTEE_OFFSET)
                        {
                            vm_address_t motor6d = rbx_instance_find_first_child_by_pointee(task, child, task_base_address, RBX_OBJECT_Motor6D_POINTEE_OFFSET);
                            if (motor6d)
                            {
                                new_trigger = child;
                                new_trigger_cframe_address = rbx_basepart_get_properties_address(task, trigger) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                                i = child_count;
                            }
                        }
                    })
                    
                }
                vm_deallocate(mach_task_self_, (vm_address_t)camera_children, camera_child_count * sizeof(rbx_child_t));
            }
            
            handle = new_handle;
            handle_cframe_address = new_handle_cframe_address;
            
            camera_part = new_camera_part;
            camera_part_cframe_address = new_camera_part_cframe_address;
            
            trigger = new_trigger;
            trigger_cframe_address = new_trigger_cframe_address;
            
            usleep(100000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            
            vm_address_t __cet = 0;
            vm_address_t __cet_cfa = 0;
            
            float old_delta_dist = aimbot_size;
            float old_dist = handle ? 25 : max_dist;
            
            int esp_part_index = 0;
            int enemy_torso_index = 0;
            
            FOR_EACH_CHILD(enemy_team_folder, i, {
                vm_address_t enemy = child;
                //rbx_print_descendants(task, enemy, 0, 3);
                if (enemy)
                {
                    //vm_address_t torso = rbx_instance_find_first_child_of_class(task, enemy, "Part");
                    //vm_address_t torso = pf_get_character_part_by_volume(task, task_base_address, enemy, target_character_part_volume);
                    vm_address_t torso = rbx_instance_find_first_child_by_pointee(task, enemy, task_base_address, RBX_OBJECT_Part_POINTEE_OFFSET);
                    
                    char enemy_name_text[100];
                    bzero(enemy_name_text, 100);
                    
                    FOR_EACH_DESCENDANT(task, enemy, 1000, 4, i, {
                        //if (rbx_instance_is_a(task, obj, "TextLabel"))
                        if (rbx_instance_get_pointee_offset(task, obj, task_base_address) == RBX_OBJECT_TextLabel_POINTEE_OFFSET)
                        {
                            long enemy_name_l = 0;
                            char* enemy_name = rbx_textlabel_get_text(task, obj, &enemy_name_l);
                            if (enemy_name)
                            {
                                strcpy(enemy_name_text, enemy_name);
                                vm_deallocate(mach_task_self_, (vm_address_t)enemy_name, enemy_name_l);
                            }
                            i = descendant_count;
                        }
                    })
                    
                    if (rbx_instance_is_valid_child(task, torso))
                    {
                        
                        APPEND_ESP_OBJ(torso, esp_color, enemy_name_text)
                        
                        enemy_torsos[enemy_torso_index++] = torso;
                        
                        rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, torso);
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        float dist;
                        const float depth = rbx_get_camera_relative_depth(camera_cframe, torso_cframe.pos, &dist);
                        
                        const vector3_t camera_look_vector = rbx_cframe_get_look_vector(camera_cframe);
                        const vector3_t f_offset = vector3_mul_num(camera_look_vector, depth);
                        const vector3_t f_pos = vector3_add(camera_cframe.pos, f_offset);
                        const float delta_dist = vector3_dist_dif(f_pos, torso_cframe.pos);
                        
                        if (old_dist > dist && old_delta_dist > delta_dist)
                        {
                            __cet = torso;
                            __cet_cfa = rbx_basepart_get_properties_address(task, __cet) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                            //old_dist = dist;
                            old_delta_dist = delta_dist;
                        }
                    
                    }
                }
            })
            
            closest_enemy_torso = __cet;
            closest_enemy_torso_cframe_address = __cet_cfa;
            
            current_enemy_count = enemy_torso_index;
            esp_part_count = esp_part_index;

            usleep(100000);
            //rbx_print_descendants(task, enemy_team_folder, 0, 1);
        }
    });
    
    
    

    
#pragma mark - ESP -
    
    
    
    BASIC_ESP_ENGINE({}, {
        should_scale = true;
        object_height = 3;
        object_width = 3;
        y_offset = 6;
        if (closest_enemy_torso && part == closest_enemy_torso && aimbot_enabled)
        {
            esp_color = target_color;
            border_width = 2;
        }
    }, 100, {
        
    });
    
    
    
    
#pragma mark - Aimbot -
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            int __new_cf_wm = 0;
            int __new_cf_ut_1 = 10000;
            int __new_cf_ut_2 = 10000;
            if (aimbot_enabled)
            {
                if (closest_enemy_torso)
                {
                    if (shared_memory->LEFT_MOUSE_DOWN || shared_memory->LEFT_MOUSE_DOWN_ONCE)
                    {
                        if (handle)
                        {
                            __new_cf_ut_2 = 1;
                            __new_cf_wm = 2;
                            
                        }
                        else
                        {
                            
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            //vector3_t camera_lv = rbx_cframe_get_look_vector(camera_cframe);
                            rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, closest_enemy_torso);
                            rbx_cframe_t trigger_cframe = rbx_basepart_get_cframe(task, trigger);
                           // rbx_cframe_t cam_part_cframe = rbx_basepart_get_cframe(task, camera_part);
                            ///torso_cframe.pos.y -= 0.25;
                            
                            if (inacc > 0)
                            {
                                int rnum1 = (rand() % (10 * (int)inacc)) - (5 * (int)inacc);
                                float rnum1_f = ((float)rnum1)/10;
                                int rnum2 = (rand() % (10 * (int)inacc)) - (5 * (int)inacc);
                                float rnum2_f = ((float)rnum2)/10;
                                int rnum3 = (rand() % (10 * (int)inacc)) - (5 * (int)inacc);
                                float rnum3_f = ((float)rnum3)/10;
                                
                                torso_cframe.pos.x += rnum1_f;
                                torso_cframe.pos.y += rnum2_f;
                                torso_cframe.pos.z += rnum3_f;
                            }
                            
                            //printf("%f, %f, %f\n", rnum1_f, rnum2_f, rnum3_f);
                            
                            vector3_t torso_velocity = rbx_basepart_get_velocity(task, closest_enemy_torso);
                            torso_cframe.pos = vector3_add(torso_cframe.pos, torso_velocity);
                            
                            vector3_t new_trigger_look_vector = vector3_direction_to_from(torso_cframe.pos, trigger_cframe.pos);
                            
                            new_trigger_look_vector = vector3_normalized(new_trigger_look_vector);
                            new_trigger_cframe_r20 = -new_trigger_look_vector.x;
                            new_trigger_cframe_r21 = -new_trigger_look_vector.y;
                            new_trigger_cframe_r22 = -new_trigger_look_vector.z;
                            
                            
                            
                            vector3_t ncplv = vector3_direction_to_from(torso_cframe.pos, camera_cframe.pos);
                            float yaw = asin(ncplv.y) * (180/PI);
                            float cam_yaw = asin(rbx_cframe_get_look_vector(trigger_cframe).y) * (180/PI);
                            float new_lv_y = sin((-(cam_yaw - yaw))/(180/PI));
                            //new_lv_y += -sin(0.025 / (180/PI));  //recoil offset
                            ncplv.y = 0;
                            ncplv = vector3_normalized(ncplv);
                            vector3_t new_cam_part_look_vector = ncplv;
                            new_cam_part_look_vector.y = new_lv_y;
                            
                            new_cam_part_cframe_r20 = -new_cam_part_look_vector.x;
                            new_cam_part_cframe_r21 = -new_cam_part_look_vector.y;
                            new_cam_part_cframe_r22 = -new_cam_part_look_vector.z;
                            
                            __new_cf_ut_1 = 0;
                            __new_cf_wm = 1;
                        }
                        usleep(5);
                    }
                    else
                    {
                        __new_cf_ut_1 = handle ? 10000 : 3;
                        __new_cf_ut_2 = handle ? 3 : 10000;
                        usleep(10);
                    }
                }
                else
                {
                    usleep(10000);
                }
            }
            else
            {
                sleep(1);
            }
            cframe_write_usleep_time_1 = __new_cf_ut_1;
            cframe_write_usleep_time_2 = __new_cf_ut_2;
            cframe_write_mode = __new_cf_wm;
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (shared_memory->LEFT_MOUSE_DOWN_ONCE)
            {
                usleep(200000);
                shared_memory->LEFT_MOUSE_DOWN_ONCE = false;
            }
            usleep(1000);
        }
    });
    
    
    
    /*
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
            usleep(cframe_write_usleep_time);
        }
    });
    */
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (cframe_write_mode == 1)
            {
                vm_write(task, camera_part_cframe_address + 0x8, (vm_offset_t)&new_cam_part_cframe_r20, 4);
                vm_write(task, camera_part_cframe_address + 0x14, (vm_offset_t)&new_cam_part_cframe_r21, 4);
                vm_write(task, camera_part_cframe_address + 0x20, (vm_offset_t)&new_cam_part_cframe_r22, 4);
            }
            usleep(cframe_write_usleep_time_1);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (cframe_write_mode == 2)
            {
                vm_copy(task, closest_enemy_torso_cframe_address, sizeof(rbx_cframe_t), handle_cframe_address);
            }
            usleep(cframe_write_usleep_time_2);
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
            printf("SET AIMBOT SIZE TO %f\n", aimbot_size);
        }, 1, &aimbot_size);
        INSERT_STDIN_MULTIARG_INPUT("inacc %f", "", {
            printf("SET AIMBOT INACCURACY TO %f\n", inacc);
        }, 1, &inacc);
        INSERT_STDIN_INPUT_SIMPLE("heads", "//ignored", {
            target_character_part_volume = 1;
            printf("TARGETTING HEADS\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("torsos", "//ignored", {
            target_character_part_volume = 4;
            printf("TARGETTING TORSOS\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("ts", "//switch target team", {
            __t__idx = !__t__idx;
            printf("SWITCHED TARGET TEAM\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("quit", "", exit(0);)
    })
    
    
}
