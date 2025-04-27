


void generic_cheat(task_t task)
{
    int pid = -1;
    pid_for_task(task, &pid);
    
    printf("\n- GENERIC CHEAT FOR GENERAL USE -\n");
    LIBESP_DYLIB_SETUP
    
    GENERIC_CHEAT_STARTER
    
    rbx_print_descendants(task, workspace, 0, 1);
    
    static char esp_objs_1_appended_names[300 * MAX_ESP_COUNT];
    static vm_address_t esp_objs_1_parts[300];
    static long esp_objs_1_count = 0;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            long esp_objs_1_index = 0;
            FOR_EACH_DESCENDANT(workspace, 10000000, 100, {
                if (rbx_instance_is_a(task, obj, "ClickDetector") ||
                    rbx_instance_is_a(task, obj, "TouchTransmitter") ||
                    rbx_instance_is_a(task, obj, "ProximityPrompt"))
                {
                    if (rbx_instance_is_a(task, obj, "ProximityPrompt"))
                    {
                        char text[MAX_ESP_TEXT_LENGTH];
                        bzero(text, sizeof(text));
                        
                        long at_l = 0;
                        char* at = rbx_proximityprompt_get_actiontext(task, obj, &at_l);
                        if (at)
                        {
                            strcat(text, at);
                            vm_deallocate(mach_task_self_, (vm_address_t)at, at_l);
                        }
                        long ot_l = 0;
                        char* ot = rbx_proximityprompt_get_objecttext(task, obj, &ot_l);
                        if (ot)
                        {
                            strcat(text, " ");
                            strcat(text, ot);
                            vm_deallocate(mach_task_self_, (vm_address_t)ot, ot_l);
                        }
                        sprintf(esp_objs_1_appended_names + (esp_objs_1_index * MAX_ESP_TEXT_LENGTH), "[%s]", text);
                    }
                    else
                    {
                        bzero(esp_objs_1_appended_names + (esp_objs_1_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                    }
                    vm_address_t obj_p = rbx_instance_get_parent(task, obj);
                    if (rbx_instance_is_a_partial(task, obj_p, "Union") || rbx_instance_is_a_partial(task, obj_p, "Part"))
                    {
                        if (esp_objs_1_index < sizeof(esp_objs_1_parts)/8)
                        {
                            esp_objs_1_parts[esp_objs_1_index] = obj_p;
                            esp_objs_1_index++;
                        }
                    }
                    else
                    {
                        if (rbx_instance_is_a(task, obj_p, "Model"))
                        {
                            vm_address_t pp = rbx_model_get_primarypart(task, obj_p);
                            if (rbx_instance_is_a_partial(task, obj_p, "Union") || rbx_instance_is_a_partial(task, obj_p, "Part"))
                            {
                                if (esp_objs_1_index < sizeof(esp_objs_1_parts)/8)
                                {
                                    esp_objs_1_parts[esp_objs_1_index] = pp;
                                    esp_objs_1_index++;
                                }
                            }
                        }
                    }
                }
                if (rbx_instance_is_a(task, obj, "BillboardGui"))
                {
                    /* long str_l = 0;
                     char* str = rbx_textlabel_get_text(task, obj, &str_l);
                     if (str)
                     {
                     //vm_address_t p = rbx_instance_get_parent(task, rbx_instance_get_parent(task, obj));
                     rbx_print_descendants(task, rbx_instance_get_parent(task, rbx_instance_get_parent(task, obj)), 0, 0);
                     vm_deallocate(mach_task_self_, (vm_address_t)str, str_l);
                     }*/
                    //rbx_print_descendants(task, obj, 0, 1);
                    //rbx_print_descendants(task, obj, 0, 2);
                }
                if (rbx_instance_is_a(task, obj, "Humanoid"))
                {
                    if (obj != my_hum)
                    {
                        //rbx_humanoid_set_health(task, obj, 0);
                    }
                }
            })
            esp_objs_1_count = esp_objs_1_index;
            sleep(10);
        }
    });
    
    
    static char fly_enabled = false;
    static float fly_speed = 100;
    
    static char esp_enabled = true;
    
    static float new_gravity = 200;
    
    NOCLIP_VARIABLES
    
    noclip_enabled = false;
    
    ESP_GLOBAL_VARIABLES
    
    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = 1000;
    
    static ESP_Color default_esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    static ESP_Color item_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(2);
            //rbx_print_descendants(task, bp, 0, 1);
            int esp_part_index = 0;
            //printf("%d\n", clickable_count);
            FOR_EACH_CHILD(players_service, {
                vm_address_t player = child;
                if (player != local_player && rbx_instance_is_a(task, player, "Player"))
                {
                    //vm_address_t bp = rbx_instance_find_first_child_of_class(task, object, "Backpack");
                    //rbx_print_descendants(task, player, 0, 1);
                    vm_address_t character = rbx_player_get_character(task, player);
                    vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
                    
                    long player_name_l = 0;
                    char* player_name = rbx_instance_get_name(task, player, &player_name_l);
                    if (player_name)
                    {
                        ESP_Color esp_color;
                        esp_color.r = 1;
                        esp_color.g = 1;
                        esp_color.b = 1;
                        esp_color.a = 1;
                        
                        vm_address_t team = rbx_player_get_team(task, player);
                        //rbx_print_descendants(task, team, 0, 0);
                        if (team)
                        {
                            int num = rbx_team_get_brick_color(task, team);
                            //printf("%d\n", num);
                            rbx_brickcolor_info_t bc = rbx_brick_color_index(num);
                            esp_color.b = (float)bc.b/255;
                            esp_color.r = (float)bc.r/255;
                            esp_color.g = (float)bc.g/255;
                            //printf("bc %d, %d, %d\n", bc.r, bc.b, bc.g);
                            //printf("espcolor %f, %f, %f\n", esp_color.r, esp_color.b, esp_color.g);
                        }
                        else
                        {
                            esp_color = default_esp_color;
                        }
                        APPEND_ESP_OBJ(hrp, esp_color, player_name);
                        vm_deallocate(mach_task_self_, (vm_address_t)player_name, player_name_l);
                    }
                }
            })
            COPY_BUF(esp_objs_1_appended_names)
            COPY_BUF(esp_objs_1_parts)
            COPY_VAR(esp_objs_1_count)
            for (int x = 0 ; x < __esp_objs_1_count ; x++)
            {
                vm_address_t obj = __esp_objs_1_parts[x];
                long name_l = 0;
                char* name = rbx_instance_get_name(task, obj, &name_l);
                if (name)
                {
                    char text[MAX_ESP_TEXT_LENGTH];
                    sprintf(text, "%s %s", name, __esp_objs_1_appended_names + (x * MAX_ESP_TEXT_LENGTH));
                    ESP_Color color = item_esp_color;
                    //APPEND_ESP_OBJ(obj, color, text);
                    //printf("%s\n", text);
                    vm_deallocate(mach_task_self_, (vm_address_t)name, name_l);
                }
            }
            esp_part_count = esp_part_index;
        }
    });
    
    
    BASIC_NOCLIP_PART_GETTER
    
    BASIC_NOCLIP_ENGINE
    
    BASIC_FLY_ENGINE(BASIC_FLY_CONTROL_CODE, 300)
    
    BASIC_ESP_ENGINE({}, 30, {
        //printf("rendering %d parts\n", __esp_part_count);
    });
    
    
#pragma mark m
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(500);
            if (true)
            {
                if (shared_memory->KEYS_DOWN['f'] == true)
                {
                    fly_enabled = !fly_enabled;
                    rbx_smooth_fly(task, fly_enabled);
                    while (shared_memory->KEYS_DOWN['f'] == true)
                    {
                        usleep(500);
                    }
                }
            }
            else
            {
                sleep(1);
            }
        }
    });
    
    
    
    
    
    
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
