
vm_address_t pf_get_torso(task_t task, vm_address_t character)
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
                float volume = size.x * size.y * size.z;
                if (volume == 2 * 2 * 1) //Dimensions of torso;
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
    
    
    static char esp_enabled = true;
    static char aimbot_enabled = true;
    
    const int max_player_count = 32;
    
    static float max_delta_dist = 8;
    static float max_dist = 400;
    
    static useconds_t esput = 1000;

    shared_memory->ESP_COUNT = max_player_count;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = esput;

    
    vm_address_t game = rbx_find_game_address(task);
    //rbx_print_descendants(task, game, 0, 10);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t teams_service = rbx_instance_find_first_child_of_class(task, game, "Teams");
    vm_address_t phantoms_team = rbx_instance_find_first_child(task, teams_service, "Phantoms");
    vm_address_t ghosts_team = rbx_instance_find_first_child(task, teams_service, "Ghosts");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");

    
    static vm_address_t closest_enemy_torso;
    static int cframe_write_mode = 0;
    
    static vm_address_t enemy_torsos[max_player_count];
    
    static vm_address_t trigger = 0;
    static vm_address_t trigger_cframe_address = 0;
    
    static int __t__0 = 0;
    static int __t__1 = 1;
    
    static float new_trigger_cframe_r20 = 0.0f;
    static float new_trigger_cframe_r21 = 0.0f;
    static float new_trigger_cframe_r22 = 0.0f;
    
    static long current_enemy_count = 0;
    static useconds_t aimbot_usleep_time = 75;
    static useconds_t cframe_write_usleep_time = 5;

    static ESP_Color esp_color = {.r = 0, .g = 1, .b = 0, .a = 0.5};
    static ESP_Color target_color = {.r = 1, .g = 0, .b = 1, .a = 0.5};
    
    static long camera_child_count = 0;
    
    
    

#pragma mark - Object Loops -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
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
                if (camera_child_count > 2)
                {
                    vm_address_t main = camera_children[2].child_address;
                    trigger = rbx_instance_find_first_child(task, main, "Trigger");
                    if (rbx_instance_get_parent(task, trigger) && rbx_instance_get_self(task, trigger) == trigger)
                    {
                        trigger_cframe_address = rbx_basepart_get_properties_address(task, trigger) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
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
                trigger = 0;
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
            vm_address_t __cet = 0;
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
                        vm_address_t torso = pf_get_torso(task, enemy);
                        vm_address_t parent = rbx_instance_get_parent(task, torso);
                        vm_address_t instance_self = rbx_instance_get_self(task, torso);
                        if (torso && parent && instance_self == torso)
                        {
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
                                __cee = true;
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

            usleep(200000);
        }
    });
    
    
    

    
#pragma mark - ESP -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            //printf("%f, %f\n", shared_memory->MOUSE_X, shared_memory->MOUSE_Y);
            long __current_enemy_count = current_enemy_count;
            if (esp_enabled &&
                __current_enemy_count > 0 &&
                __current_enemy_count < (max_player_count + 1))
            {
                int esp_index = 0;
                for (long i = 0 ; i < __current_enemy_count ; i++)
                {
                    ESP_Color color = esp_color;
                    float border_width = 2;
                    vm_address_t enemy_torso = enemy_torsos[i];
                    if (enemy_torso)
                    {
                        if (closest_enemy_torso && enemy_torso == closest_enemy_torso && aimbot_enabled)
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
                            //printf("%f, %f", f.x, f.y); rbx_print_descendants(task, enemy_torso, 0, 0);
                            esp_index++;
                        }
                    }
                    usleep(esput/__current_enemy_count);
                    
                }
                memset(shared_memory->ESP_BOX_HIDDEN_ARRAY + esp_index, 1, MAX_ESP_COUNT - esp_index);
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
            if (aimbot_enabled)
            {
                if (closest_enemy_torso &&
                    camera_child_count > 2)
                {
                    if (shared_memory->LEFT_MOUSE_DOWN == true)
                    {
                        cframe_write_usleep_time = 0;
                        aimbot_usleep_time = 50;
                        rbx_cframe_t torso_cframe = rbx_basepart_get_cframe(task, closest_enemy_torso);
                        rbx_cframe_t trigger_cframe = rbx_basepart_get_cframe(task, trigger);
                        torso_cframe.pos.y -= 0.25;
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
            else
            {
                sleep(1);
            }
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
            if (strcmp(str, "teamswitch\xa") == 0)
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
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
    
}
