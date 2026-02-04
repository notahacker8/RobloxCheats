

void anti_afk_cheat(task_t task)
{
    GENERIC_CHEAT_STARTER
    ANTI_AFK
    EXIT_IF_TASK_TERMINATED
    rbx_print_descendants(task, local_player, 0, 0);
}


#pragma mark - GENERIC -


void generic_cheat(task_t task)
{
    int pid = -1;
    pid_for_task(task, &pid);
    
    printf("\n- GENERIC CHEAT FOR GENERAL USE -\n");
    LIBESP_DYLIB_SETUP
    
    GENERIC_CHEAT_STARTER
    
    
    static char esp_objs_1_appended_names[MAX_ESP_TEXT_LENGTH * MAX_ESP_COUNT];
    static vm_address_t esp_objs_1_parts[300];
    static long esp_objs_1_count = 0;
    
    //rbx_print_descendants(task, game, 0, 10000);
    
    static char auto_search = false;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            long esp_objs_1_index = 0;
            if (auto_search)
            {
                FOR_EACH_DESCENDANT(task, workspace, 10000000, 100, i, {
                    vm_address_t pointee_offset = vm_read_uint64_t_value(task, obj) - task_base_address;
                    if (pointee_offset == RBX_OBJECT_ClickDetector_POINTEE_OFFSET ||
                        pointee_offset == RBX_OBJECT_TouchTransmitter_POINTEE_OFFSET ||
                        pointee_offset == RBX_OBJECT_ProximityPrompt_POINTEE_OFFSET)
                    {
                        if (pointee_offset == RBX_OBJECT_ProximityPrompt_POINTEE_OFFSET)
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
                                strcat(text, " \"");
                                strcat(text, ot);
                                strcat(text, "\"");
                                vm_deallocate(mach_task_self_, (vm_address_t)ot, ot_l);
                            }
                            sprintf(esp_objs_1_appended_names + (esp_objs_1_index * MAX_ESP_TEXT_LENGTH), "[%s]", text);
                        }
                        else
                        {
                            bzero(esp_objs_1_appended_names + (esp_objs_1_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                        }
                        vm_address_t obj_p = rbx_instance_get_parent(task, obj);
                        vm_address_t parent_pointee_offset = vm_read_uint64_t_value(task, obj_p) - task_base_address;
                        
                        if (rbx_pointee_offset_is_a_basepart(parent_pointee_offset))
                        {
                            if (esp_objs_1_index < sizeof(esp_objs_1_parts)/8)
                            {
                                esp_objs_1_parts[esp_objs_1_index] = obj_p;
                                esp_objs_1_index++;
                            }
                        }
                        else
                        {
                            if (parent_pointee_offset == RBX_OBJECT_Model_POINTEE_OFFSET)
                            {
                                vm_address_t pp = rbx_model_get_primarypart(task, obj_p);
                                if (pp)
                                {
                                    if (esp_objs_1_index < sizeof(esp_objs_1_parts)/8)
                                    {
                                        esp_objs_1_parts[esp_objs_1_index] = pp;
                                        esp_objs_1_index++;
                                    }
                                }
                                else
                                {
                                    vm_address_t __part = 0;
                                    FOR_EACH_CHILD(obj_p,  x, {
                                        if (rbx_instance_is_a_basepart(task, child, task_base_address))
                                        {
                                            __part = child;
                                            x = child_count;
                                        }
                                    })
                                    if (esp_objs_1_index < sizeof(esp_objs_1_parts)/8)
                                    {
                                        esp_objs_1_parts[esp_objs_1_index] = __part;
                                        esp_objs_1_index++;
                                    }
                                }
                            }
                        }
                    }
                    usleep(3);
                })
                sleep(10);
            }
            else
            {
                sleep(1);
            }
            esp_objs_1_count = esp_objs_1_index;
        }
    });
    
    
    static char fly_enabled = false;
    static float fly_speed = 200;
    
    NOCLIP_VARIABLES
    
    noclip_enabled = false;
    
    ESP_GLOBAL_VARIABLES
    static ESP_Color red_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    
    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = 1000;
    
    static ESP_Color default_esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    static ESP_Color item_esp_color = {.r = 0, .g = 0, .b = 1, .a = 1};
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(2);
            //rbx_print_descendants(task, bp, 0, 1);
            int esp_part_index = 0;
            //printf("%d\n", clickable_count);
            FOR_EACH_CHILD(players_service, i, {
                vm_address_t player = child;
                if (player != local_player)
                {
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
                    APPEND_ESP_OBJ(obj, color, text);
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
    
    static char sidebar_text[200];
    
    
    BASIC_ESP_ENGINE({}, {}, 30, {
        bzero(sidebar_text, sizeof(sidebar_text));
        if (fly_enabled) { strcat(sidebar_text, "FLY ENABLED"); } strcat(sidebar_text, "\n");
        if (noclip_enabled) { strcat(sidebar_text, "NOCLIPPING"); } strcat(sidebar_text, "\n");
        {
            //ESP_Frame frame = (ESP_Frame){.x = shared_memory -> WINDOW_W - 200, .y = (shared_memory->WINDOW_H - 300) * 0.5, .w = 200, .h = 300};
            NSRect frame;
            frame.origin.x = shared_memory -> WINDOW_W - 200;
            frame.origin.y = (shared_memory->WINDOW_H - 300) * 0.5;
            frame.size.width = 200;
            frame.size.height = 300;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), sidebar_text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = red_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
    });
    
    
#pragma mark f
    
    PRESS_ONCE_KEYBIND_CREATE('f', {
        fly_enabled = !fly_enabled;
        rbx_smooth_fly_patch(task, task_base_address, fly_enabled);
    }, true, "fly");
    
#pragma mark n
    
    PRESS_ONCE_KEYBIND_CREATE('n', {
        noclip_enabled = !noclip_enabled;
    }, true, "noclip");
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("fly", "", {
            fly_enabled = true;
            rbx_smooth_fly_patch(task, task_base_address, true);
            printf("ENABLED FLY\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("nofly", "", {
            fly_enabled = false;
            rbx_smooth_fly_patch(task, task_base_address, false);
            printf("DISABLED FLY\n");
        })
        INSERT_STDIN_MULTIARG_INPUT("flyspeed %f", "", {
            printf("SET FLY SPEED TO %f\n", fly_speed);
        }, 1, &fly_speed);
        INSERT_STDIN_INPUT_SIMPLE("coords", "", {
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
        })
        INSERT_STDIN_INPUT_SIMPLE("mousecoords", "", {
            printf("PRINTING MOUSE COORDINATES...\n");
            printf("%f, %f\n", shared_memory->MOUSE_X, shared_memory->MOUSE_Y);
        })
        INSERT_STDIN_INPUT_SIMPLE("noclip", "", {
            noclip_enabled = true;
            printf("NOCLIP ENABLED\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("clip", "", {
            noclip_enabled = false;
            printf("NOCLIP DISABLED\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("esp", "", {
            esp_enabled = true;
            printf("ESP ENABLED\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("noesp", "", {
            esp_enabled = false;
            printf("ESP DISABLED\n");
        })
        INSERT_STDIN_INPUT_SIMPLE("camcoords", "", {
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
        })
        INSERT_STDIN_INPUT_SIMPLE("placeid", "", {
            printf("PlaceId: %ld\n", rbx_get_placeid(task));
            long len = 0;
            char* jobid = rbx_datamodel_get_jobid(task, game, &len);
            if (jobid)
            {
                printf("JobId: %s\n", jobid);
                vm_deallocate(mach_task_self_, (vm_address_t)jobid, len);
            }
        })
        INSERT_STDIN_INPUT_SIMPLE("autoscan", "", auto_search = true; printf("ENABLED AUTOSCAN\n");)
        INSERT_STDIN_INPUT_SIMPLE("noautoscan", "", auto_search = false; printf("DISABLED AUTOSCAN\n");)
        INSERT_STDIN_INPUT_SIMPLE("quit", "", exit(0);)
    })
    
}


#pragma mark - ANTI AFK ALL -


//Anti AFK for all roblox processes
void anti_afk_all(void)
{
    static task_t tasks[100];
    static vm_address_t addys[100];
    static int task_count = 0;
    static char should_update = true;
    static char auto_update = false;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (should_update)
            {
                pid_t pids[4096];
                int pid_count = pids_by_name("RobloxPlayer", pids);
                
                int t_idx = 0;
                COPY_BUF(tasks);
                for (int i = 0 ; i < pid_count ; i++)
                {
                    task_t task;
                    if (task_for_pid(mach_task_self(), pids[i], &task) == KERN_SUCCESS)
                    {
                        vm_address_t game = rbx_find_game_address(task);
                        vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
                        vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
                        rbx_print_descendants(task, local_player, 0, 0);
                        addys[t_idx] = local_player;
                        tasks[t_idx] = task;
                        t_idx++;
                    }
                }
                task_count = t_idx;
                should_update = false;
                printf("updated tasks and addresses\n");
            }
            else
            {
                sleep(1);
            }
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            for (int i = 0 ; i < task_count ; i++)
            {
                if (rbx_instance_is_valid_child(tasks[i], addys[i]) && rbx_instance_is_a(tasks[i], addys[i], "Player"))
                {
                    rbx_player_set_last_input_timestamp(tasks[i], addys[i], 999999);
                }
                else
                {
                    addys[i] = 0;
                }
            }
            sleep(3);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (auto_update)
            {
                should_update = true;
                sleep(60 * 20);
            }
            sleep(1);
        }
    });
    
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("update", "", should_update = true;)
    })
}

#pragma mark - AUTO THROTTLE ALL -

//Lower priority of all roblox processes if they are in background
void auto_throttle_all(char use_cputhrottle)
{
    //printf("bg auto\n");
    system("killall -9 cputhrottle_remake");
    
    static int main_pid = -1;
    
    static int roblox_pids[200];
    static int roblox_pid_count = 0;
    
    static int front_pid = -1;
    static char roblox_is_front = false;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            @autoreleasepool {
                int new_rbx_pid_c = 0;
                ProcessSerialNumber psn = {0, kNoProcess};
                while (GetNextProcess(&psn) == noErr)
                {
                    CFStringRef procNameRef = NULL;
                    if (CopyProcessName(&psn, &procNameRef) == noErr)
                    {
                        NSString *procName = (__bridge_transfer NSString *)procNameRef;
                        if ([procName isEqualToString:@"Roblox"])
                        {
                            pid_t pid = -1;
                            GetProcessPID(&psn, &pid);
                            roblox_pids[new_rbx_pid_c] = pid;
                            new_rbx_pid_c++;
                        }
                    }
                }
                roblox_pid_count = new_rbx_pid_c;
            }
            //printf("front: %d\n", front_pid);
            //printf("roblox count: %d\n", roblox_pid_count);
            sleep(2);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            @autoreleasepool
            {
                ProcessSerialNumber frontPSN;
                GetFrontProcess(&frontPSN);
                GetProcessPID(&frontPSN, &front_pid);
                CFStringRef procNameRef = NULL;
                char nrif = 0;
                if (CopyProcessName(&frontPSN, &procNameRef) == noErr)
                {
                    NSString *procName = (__bridge_transfer NSString *)procNameRef;
                    if ([procName isEqualToString:@"Roblox"])
                    {
                        nrif = true;
                    }
                }
                roblox_is_front = nrif;
            }
            usleep(5000);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        
        //key: Roblox, value: cputhrottle
        NSMutableDictionary<NSNumber*, NSNumber*>* dict = [NSMutableDictionary dictionary];
        
        for (;;)
        {
            pid_t child_pids[10000]; bzero(child_pids, sizeof(child_pids));
            int cpid_count = pids_by_parent_pid(child_pids, getpid());
            //printf("%d\n", cpid_count);
            for (NSNumber* key in [dict allKeys])
            {
                //NSLog(@"key: %@, value: %@", key, dict[key]);
                if (!isProcessAlive(key.intValue))
                {
                    [dict removeObjectForKey:key];
                }
                if (dict[key])
                {
                    char e = false;
                    for (int  i = 0 ; i < cpid_count ; i++)
                    {
                        if (child_pids[i] == dict[key].intValue)
                        {
                            e = true;
                        }
                    }
                    if (!e)
                    {
                        printf("deleted a cputhrottle reference due to absence\n");
                        [dict removeObjectForKey:key];
                    }
                }
            }
            COPY_BUF(roblox_pids)
            COPY_VAR(roblox_pid_count)
            COPY_VAR(front_pid)
            COPY_VAR(roblox_is_front)
            for (int i = 0 ; i < __roblox_pid_count ; i++)
            {
                int pid = __roblox_pids[i];
                if (pid == __front_pid || (pid == main_pid && !__roblox_is_front))
                {
                    setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_MAX);
                    if (use_cputhrottle)
                    {
                        if (dict[@(pid)])
                        {
                            kill(dict[@(pid)].intValue, SIGKILL);
                            [dict removeObjectForKey:@(pid)];
                            printf("deleted cputhrottle for proc %d\n", pid);
                        }
                    }
                }
                else
                {
                    if (pid != main_pid || (pid == main_pid && __roblox_is_front && __front_pid != main_pid))
                    {
                        setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_DARWIN_BG);
                        if (use_cputhrottle)
                        {
                            if (!dict[@(pid)])
                            {
                                char ps[2000];
                                bzero(ps, 2000);
                                sprintf(ps, "%d", pid);
                                char *argv[] = {__CPUTHROTTLE_PATH__, ps, "13", NULL};
                                pid_t ctpid = -1;
                                if (posix_spawn(&ctpid, __CPUTHROTTLE_PATH__, NULL, NULL, argv, NULL) == 0)
                                {
                                    dict[@(pid)] = @(ctpid);
                                    printf("started cputhrottle for proc %d\n", pid);
                                }
                            }
                        }
                    }
                }
            }
            usleep(5000);
        }
    });
    

    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_MULTIARG_INPUT("main %d",  "", {printf("set main roblox process to %d\n", main_pid);}, 1, &main_pid);
    })
    
}
