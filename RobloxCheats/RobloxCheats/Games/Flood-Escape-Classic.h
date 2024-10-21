


void generic_cheat(task_t task)
{
    int pid = -1;
    pid_for_task(task, &pid);
    
    printf("\n- GENERIC CHEAT FOR GENERAL USE -\n");
    LIBESP_DYLIB_SETUP
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players, "Player");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    //rbx_print_descendants(task, workspace, 0, 1);
    
    /*
    vm_address_t mushroom_world = rbx_instance_find_first_child(task, workspace, "MushroomWorld");
    vm_address_t hole = rbx_instance_find_first_child(task, mushroom_world, "Hole 8");
    vm_address_t truffle = rbx_instance_find_first_child(task, hole, "Truffle");
    vm_address_t hole_part = rbx_instance_find_first_child(task, hole, "Hole");
    
    rbx_cframe_t new_cf = rbx_basepart_get_cframe(task, hole_part);
    
    rbx_print_descendants(task, hole, 0, 2);
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            rbx_basepart_set_cframe(task, truffle, new_cf);
            usleep(100);
        }
    });
*/
    
    //rbx_print_descendants(task, hole_8, 0, 1);
    /*
    vm_address_t descendants[100000] = {};
    int descendant_count = 0;
    
    static vm_address_t clickables[1000] = {};
    long clickable_count = 0;
    
    rbx_get_descendants(task, workspace, 0, 100, descendants, &descendant_count, sizeof(descendants)/8);
    for (int i = 0 ; i < sizeof(descendants)/8 ; i++)
    {
        vm_address_t obj = descendants[i];
        if (rbx_instance_is_a(task, obj, "ClickDetector"))
        {
            vm_address_t clickable = rbx_instance_get_parent(task, obj);
            long name_l = 0;
            char* name = rbx_instance_get_name(task, clickable, &name_l);
            if (strstr(name, "paper") || strstr(name, "memoir"))
            {
                rbx_print_descendants(task, clickable, 0, 0);
                clickables[clickable_count] = clickable;
                clickable_count++;
            }
        }
        //rbx_print_descendants(task, descendants[i], 0, 0);
    }
    
    
    printf("\nobjects recieved: %d\n", descendant_count);*/
    
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    
    static char fly_enabled = false;
    static float fly_speed = 250;
    
    static int esput = 1000;
    static char esp_enabled = true;
    static char noclip_enabled = false;
    
    static float new_gravity = 180;
    
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_character_parts[200];
    static long my_character_part_count = 0;
    
    ESP_GLOBAL_VARIABLES
    
    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = esp_enabled;
    shared_memory->ESP_USLEEP_TIME = esput;
    
    ESP_Color esp_color = {.r = 1, .g = 0, .b = 1, .a = 1};
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(2);
            int esp_part_index = 0;
            //printf("%d\n", clickable_count);
            {
                long child_count = 0;
                rbx_child_t* children = rbx_instance_get_children(task, players, &child_count);
                if (children)
                {
                    int __i = 0;
                    for (long i = 0 ; i < child_count ; i++)
                    {
                        vm_address_t object = children[i].child_address;
                        if (object != local_player)
                        {
                            vm_address_t character = rbx_player_get_character(task, object);
                            vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
                            
                            long player_name_l = 0;
                            char* player_name = rbx_instance_get_name(task, object, &player_name_l);
                            if (player_name)
                            {
                                ESP_Color esp_color = {.r = 1, .g = 1, .b = 1, .a = 1};
                                vm_address_t team = rbx_player_get_team(task, object);
                                if (team)
                                {
                                    int num = rbx_team_get_brick_color(task, team);
                                    rbx_brickcolor_info_t bc = rbx_brick_color_index(num);
                                    esp_color.b = bc.b;
                                    esp_color.r = bc.r;
                                    esp_color.g = bc.g;
                                }
                                else
                                {
                                    
                                }
                                APPEND_ESP_OBJ(hrp, esp_color, player_name);
                                vm_deallocate(mach_task_self_, (vm_address_t)player_name, player_name_l);
                            }
                        }
                    }
                    my_character_part_count = __i;
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
            }
            //vm_address_t o = rbx_instance_find_first_child(task, workspace, "Wedge");
            //APPEND_ESP_OBJ(hole_part, esp_color, "X")
            esp_part_count = esp_part_index;
            //printf("%d\n", esp_part_count);
        }
    });


    BASIC_NOCLIP_PART_GETTER
    
    BASIC_NOCLIP_ENGINE
    
    BASIC_FLY_ENGINE(BASIC_FLY_CONTROL_CODE, 300)
    
    BASIC_ESP_ENGINE({}, 30, {});
    
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    
    printf("List of stdin commands:\n");
    printf("\tfly\n");
    printf("\tflyspeed [(float)]\n");
    printf("\tnofly\n");
    printf("\tgrav [(float)]\n");
    printf("\tcoords\n");
    printf("\tcamcoords\n");
    printf("\tmousecoords\n");
    printf("\tnoclip\n");
    printf("\tclip\n");
    printf("\tplaceid\n");
    printf("\tquit\n");
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            char str[100];
            bzero(str, 100);
            fgets(str, 100, stdin);
            if (strcmp(str, "fly\xa") == 0)
            {
                rbx_smooth_fly(task, true);
                fly_enabled = true;
                printf("ENABLED FLY\n");
            }
            if (strncmp(str, "flyspeed ", strlen("flyspeed ")) == 0)
            {
                sscanf(str, "flyspeed %f", &fly_speed);
                printf("SET FLY SPEED TO %f\n", fly_speed);
            }
            if (strcmp(str, "nofly\xa") == 0)
            {
                rbx_smooth_fly(task, false);
                fly_enabled = false;
                printf("DISABLED FLY\n");
            }
            if (strncmp(str, "grav ", strlen("grav ")) == 0)
            {
                sscanf(str, "grav %f", &new_gravity);
                printf("GRAVITY SET TO %f\n", new_gravity);
            }
            if (strcmp(str, "coords\xa") == 0)
            {
                printf("PRINTING PLAYER COORDINATES...\n");
                if (my_hrp)
                {
                    vector3_t pos = rbx_basepart_get_cframe(task, my_hrp).pos;
                    printf("vector3_t pos = {\n\t.x = %f,\n\t.y = %f,\n\t.z = %f\n};\n", pos.x, pos.y, pos.z);
                }
                else
                {
                    printf("ERROR: HUMANOID ROOT PART NOT FOUND\n");
                }
            }
            if (strcmp(str, "mousecoords\xa") == 0)
            {
                printf("PRINTING MOUSE COORDINATES...\n");
                if (my_hrp)
                {
                    printf("%f, %f\n", shared_memory->MOUSE_X, shared_memory->MOUSE_Y);
                }
                else
                {
                    printf("ERROR: HUMANOID ROOT PART NOT FOUND\n");
                }
            }
            if (strcmp(str, "noclip\xa") == 0)
            {
                noclip_enabled = true;
                printf("NOCLIP ENABLED\n");
            }
            if (strcmp(str, "clip\xa") == 0)
            {
                noclip_enabled = false;
                printf("NOCLIP DISABLED\n");
            }
            if (strcmp(str, "esp\xa") == 0)
            {
                esp_enabled = true;
                printf("ESP ENABLED\n");
            }
            if (strcmp(str, "noesp\xa") == 0)
            {
                esp_enabled = false;
                printf("ESP DISABLED\n");
            }
            if (strcmp(str, "camcoords\xa") == 0)
            {
                printf("PRINTING CAMERA COORDINATES...\n");
                if (camera)
                {
                    rbx_cframe_t cframe = rbx_camera_get_cframe(task, camera);
                    printf("rbx_cframe_t cframe = {\n\t.r0 = %f, .r10 = %f, .r20 = %f,\n\t.r1 = %f, .r11 = %f, .r21 = %f,\n\t.r2 = %f, .r12 = %f, .r22 = %f,\n\t.pos = {.x = %f, .y = %f, .z = %f}\n};\n", cframe.r0, cframe.r10, cframe.r20, cframe.r1, cframe.r11, cframe.r21, cframe.r2, cframe.r12, cframe.r22, cframe.pos.x, cframe.pos.y, cframe.pos.z);
                }
                else
                {
                    printf("ERROR: CAMERA NOT FOUND\n");
                }
            }
            if (strcmp(str, "placeid\xa") == 0)
            {
                printf("PlaceId: %ld\n", rbx_get_placeid(task));
                long len = 0;
                char* jobid = rbx_datamodel_get_jobid(task, game, &len);
                if (jobid)
                {
                    printf("JobId: %s\n", jobid);
                    vm_deallocate(mach_task_self_, (vm_address_t)jobid, len);
                }
            }
            if (strcmp(str, "lp\xa") == 0)
            {
                setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_DARWIN_BG);
                printf("Set Low Priority\n");
            }
            if (strcmp(str, "hp\xa") == 0)
            {
                setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_MAX);
                printf("Set High Priority\n");
            }
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
    
}
