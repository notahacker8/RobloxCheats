
vm_address_t pf_get_character_part_by_volume(task_t task, vm_address_t character, float volume)
{
    //rbx_print_descendants(task, character, 0, 1);
    vm_address_t part = 0;
    long child_count = 0;
    rbx_child_t* children = rbx_instance_get_children(task, character, &child_count);
    if (children)
    {
        for (long i = 0 ; i < child_count ; i++)
        {
            vm_address_t child = children[i].child_address;
            if (rbx_instance_is_a(task, child, "Part"))
            {
                vector3_t size = rbx_basepart_get_size(task, child);
                float __volume = size.x * size.y * size.z;
                if (__volume == volume)
                {
                    part = child;
                    i = child_count;
                }
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
    }
    return part;
}


void phantom_forces_cheat(task_t task)
{
    printf("- PHANTOM FORCES -\n");
    
    LIBESP_DYLIB_SETUP
    
    static char aimbot_enabled = true;
    
    const int max_player_count = 32;
    
    static float max_delta_dist = 15;
    static float max_dist = 400;
    
    static float inacc = 0;
    
    static float target_character_part_volume = 2 * 2 * 1; //Heads is 1 * 1 * 1
    
    static useconds_t esput = 1000;
    
    ESP_GLOBAL_VARIABLES;

    shared_memory->ESP_COUNT = max_player_count;
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
    
    static vm_address_t closest_enemy_torso;
    static int cframe_write_mode = 0;
    
    static vm_address_t enemy_team_folder = 0;
    
    static vm_address_t enemy_torsos[max_player_count];
    
    static vm_address_t trigger = 0;
    static vm_address_t trigger_cframe_address = 0;
    
    static vm_address_t camera_part = 0;
    static vm_address_t camera_part_cframe_address = 0;
    
    static int __t__0 = 0;
    static int __t__1 = 1;
    
    static float new_trigger_cframe_r20 = 0.0f;
    static float new_trigger_cframe_r21 = 0.0f;
    static float new_trigger_cframe_r22 = 0.0f;
    
    static float new_cam_part_cframe_r20 = 0.0f;
    static float new_cam_part_cframe_r21 = 0.0f;
    static float new_cam_part_cframe_r22 = 0.0f;
    
    static long current_enemy_count = 0;
    static useconds_t cframe_write_usleep_time = 5;

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
            //rbx_print_descendants(task, my_character, 0, 3);
            vm_address_t players_folder = rbx_instance_find_first_child(task, workspace, "Players");
            
            vm_address_t blue_team_folder = 0;
            vm_address_t orange_team_folder = 0;
            long tfcc = 0;
            rbx_child_t* team_folders = rbx_instance_get_children(task, players_folder, &tfcc);
            if (team_folders)
            {
                orange_team_folder = team_folders[__t__0].child_address;
                blue_team_folder = team_folders[__t__1].child_address;
                vm_deallocate(mach_task_self_, (vm_address_t)team_folders, tfcc * sizeof(rbx_child_t));
            }
            vm_address_t my_team = rbx_player_get_team(task, local_player);
            //rbx_print_descendants(task, players_folder, 0, 1);
            //rbx_print_descendants(task, my_team, 0, 0);
            
            rbx_child_t* camera_children = rbx_instance_get_children(task, camera, &camera_child_count);
            if (camera_children)
            {
                if (camera_child_count > 0)
                {
                    //rbx_print_descendants(task, camera, 0, 2);
                    vm_address_t main = camera_children[camera_child_count - 1].child_address;
                    camera_part = rbx_instance_find_first_child(task, camera, "Part");
                    //rbx_print_descendants(task, main, 0, 2);
                    long child_count = 0;
                    //long p_i = 0;
                    rbx_child_t* children = rbx_instance_get_children(task, main, &child_count);
                    if (children)
                    {
                        for (long i = 0 ; i < child_count ; i++)
                        {
                            vm_address_t obj = children[i].child_address;
                            if (rbx_instance_is_a(task, obj, "Part") == true)
                            {
                                //rbx_print_descendants(task, obj, 0, 0);
                                vm_address_t motor6d = rbx_instance_find_first_child_of_class(task, obj, "Motor6D");
                                if (motor6d)
                                {
                                   /* for (int x = 0 ; x < 100 ; x++)
                                    {
                                        vm_address_t v = vm_read_uint64_t_value(task, motor6d + (x * 8));
                                        if (rbx_instance_is_valid_child(task, v))
                                        {
                                            //printf("%p: ", x * 8); rbx_print_descendants(task, v, 0, 0);
                                        }
                                    }*/
                                    trigger = obj;
                                    i = child_count;
                                }
                            }
                            
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                    }
                    
                    if (rbx_instance_is_valid_child(task, trigger))
                    {
                        trigger_cframe_address = rbx_basepart_get_properties_address(task, trigger) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                    }
                    else
                    {
                        trigger = 0;
                        trigger_cframe_address = 0;
                    }
                    
                    if (rbx_instance_is_valid_child(task, camera_part))
                    {
                        camera_part_cframe_address = rbx_basepart_get_properties_address(task, camera_part) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                    }
                    else
                    {
                        camera_part = 0;
                        camera_part_cframe_address = 0;
                    }
                    
                }
                vm_deallocate(mach_task_self_, (vm_address_t)camera_children, camera_child_count * sizeof(rbx_child_t));
            }
            else
            {
                trigger = 0;
                trigger_cframe_address = 0;
            }
            
            vm_address_t __enemy_team_folder = 0;
            
            if (my_team == phantoms_team)
            {
                __enemy_team_folder = orange_team_folder;
            }
            if (my_team == ghosts_team)
            {
                __enemy_team_folder = blue_team_folder;
            }
            enemy_team_folder = __enemy_team_folder;
            usleep(200000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            
            vm_address_t __cet = 0;
            float old_delta_dist = max_delta_dist;
            float old_dist = max_dist;
            
            int esp_part_index = 0;
            int enemy_torso_index = 0;
            long enemy_count = 0;
            rbx_child_t* enemies_list = rbx_instance_get_children(task, enemy_team_folder, &enemy_count);
            if (enemies_list)
            {
                //printf("enemy count: %d\n", enemy_count);
                for (int i = 0 ; i < enemy_count ; i++)
                {
                    vm_address_t enemy = enemies_list[i].child_address;
                    //rbx_print_descendants(task, enemy, 0, 3);
                    if (enemy)
                    {
                        vm_address_t torso = rbx_instance_find_first_child_of_class(task, enemy, "Part");
                        vm_address_t parent = rbx_instance_get_parent(task, torso);
                        vm_address_t instance_self = rbx_instance_get_self(task, torso);
                        
                        char enemy_name_text[100];
                        bzero(enemy_name_text, 100);
                        
                        FOR_EACH_DESCENDANT(task, enemy, 1000, 4, {
                            if (rbx_instance_is_a(task, obj, "TextLabel"))
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
                        
                        if (torso && parent && instance_self == torso)
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
                                //old_dist = dist;
                                old_delta_dist = delta_dist;
                            }
                        
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)enemies_list, enemy_count * sizeof(rbx_child_t));
            }
            closest_enemy_torso = __cet;
            current_enemy_count = enemy_torso_index;
            esp_part_count = esp_part_index;

            usleep(300000);
            //rbx_print_descendants(task, enemy_team_folder, 0, 1);
        }
    });
    
    
    

    
#pragma mark - ESP -
    
    
    
    BASIC_ESP_ENGINE({
        should_scale = true;
        object_height = 3;
        object_width = 3;
        y_offset = 6;
        if (closest_enemy_torso && part == closest_enemy_torso && aimbot_enabled)
        {
            esp_color = target_color;
            border_width = 2;
        }
    }, 50, {
        
    });
    
    
    
    
#pragma mark - Aimbot + Melee -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        srand((int)time(NULL));
        for (;;)
        {
            if (aimbot_enabled)
            {
                if (closest_enemy_torso &&
                    camera_child_count > 2)
                {
                    if (shared_memory->LEFT_MOUSE_DOWN == true)
                    //if(true)
                    {
                        cframe_write_usleep_time = 0;
                        
                        int rnum1 = (rand() % (10 * (int)inacc)) - (5 * (int)inacc);
                        float rnum1_f = ((float)rnum1)/10;
                        int rnum2 = (rand() % (10 * (int)inacc)) - (5 * (int)inacc);
                        float rnum2_f = ((float)rnum2)/10;
                        int rnum3 = (rand() % (10 * (int)inacc)) - (5 * (int)inacc);
                        float rnum3_f = ((float)rnum3)/10;
                        
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        vector3_t camera_lv = rbx_cframe_get_look_vector(camera_cframe);
                        rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, closest_enemy_torso);
                        rbx_cframe_t trigger_cframe = rbx_basepart_get_cframe(task, trigger);
                        rbx_cframe_t cam_part_cframe = rbx_basepart_get_cframe(task, camera_part);
                        ///torso_cframe.pos.y -= 0.25;
                        
                        if (inacc > 0)
                        {
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
                        
                        
                        cframe_write_mode = 1;
                        
                    }
                    else
                    {
                        cframe_write_usleep_time = 100;
                        usleep(5);
                        cframe_write_mode = 0;
                    }
                }
                else
                {
                    cframe_write_usleep_time = 100;
                    cframe_write_mode = 0;
                    usleep(10000);
                }
            }
            else
            {
                sleep(1);
            }
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
    });*/
    
    
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
            usleep(cframe_write_usleep_time);
        }
    });
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    
    
    
    
    printf("List of stdin commands:\n");
    printf("\tesp\n");
    printf("\tnoesp\n");
    printf("\taimbot\n");
    printf("\tteamswitch\n");
    printf("\tnoaimbot\n");
    printf("\taimbotsize [(float)]\n");
    printf("\tinacc [(float)]\n");
    printf("\theads\n");
    printf("\ttorsos\n");
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
            if (strcmp(str, "teamswitch\xa") == 0 || strcmp(str, "ts\xa") == 0)
            {
                __t__0 = abs(--__t__0);
                __t__1 = abs(--__t__1);
                printf("SWITCHED TARGET TEAM\n");
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
            if (strncmp(str, "inacc ", strlen("inacc ")) == 0)
            {
                sscanf(str, "inacc %f", &inacc);
                printf("SET INACCURACY TO %f STUD(S)\n", inacc);
            }
            if (strcmp(str, "heads\xa") == 0)
            {
                target_character_part_volume = 1;
                printf("TARGETTING HEADS\n");
            }
            if (strcmp(str, "torsos\xa") == 0)
            {
                target_character_part_volume = 4;
                printf("TARGETTING TORSOS\n");
            }
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
    
}
