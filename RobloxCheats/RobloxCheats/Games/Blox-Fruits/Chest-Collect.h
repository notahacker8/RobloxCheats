


void blox_fruits_chest_farm(task_t task)
{
    printf("- BLOX FRUITS (CHEST COLLECT) -\n");
    printf("To restore the map, use 'blox_fruits'\n");
    
    void* dlhandle = dlopen(__LIBESP_DYLIB_PATH__, RTLD_NOW);
    if (!dlhandle)
    {
        printf("%s\n", dlerror());
    }
    vm_address_t s_load_address = task_get_image_address_by_path(mach_task_self_, __LIBESP_DYLIB_PATH__);

    vm_offset_t function_queue_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE");
    vm_offset_t function_queue_count_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE_COUNT");
    vm_offset_t function_queue_finished_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE_FINISHED");
    vm_offset_t function_usleep_time_offset = gdso(dlhandle, s_load_address, "FUNCTION_USLEEP_TIME");
    
    dlclose(dlhandle);
    
    vm_address_t load_address = task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    if (!load_address)
    {
        printf("Couldn't find libESP.dylib in task %d\n", task);
        exit(0);
    }
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    useconds_t fut = 100000;
    vm_write(task, load_address + function_usleep_time_offset, (vm_address_t)&fut, sizeof(useconds_t));
    
    vm_address_t instance_set_parent_func = task_base_address + RBX_INSTANCE_PARENT_SETTER_METHOD_OFFSET;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t badgeservice = rbx_instance_find_first_child_of_class(task, game, "BadgeService");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t map_model = rbx_instance_find_first_child(task, workspace, "Map");
    
    if (map_model)
    {
        RemoteFunctionCall remote_call = {.address = instance_set_parent_func, .type = 2, .finished = false};
        ((vm_address_t*)(remote_call.arguments))[0] = map_model;
        ((vm_address_t*)(remote_call.arguments + 8))[0] = badgeservice;
        
        int count = 1;
        char finished = 0;
        vm_write(task, load_address + function_queue_offset, (vm_address_t)&remote_call, sizeof(RemoteFunctionCall));
        vm_write(task, load_address + function_queue_count_offset, (vm_address_t)&count, sizeof(int));
        vm_write(task, load_address + function_queue_finished_offset, (vm_address_t)&finished, sizeof(char));
        wait_until_queue_finished(task, load_address + function_queue_finished_offset, 1000);
    }
    
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static vm_address_t character = 0;
    static vm_address_t humanoid = 0;
    static vm_address_t hrp = 0;
    static vector3_t vel;
    static float fly_speed = 275;
    
    static vm_address_t closest_chest = 0;
    static char closest_chest_exists = false;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            character = rbx_player_get_character(task, local_player);
            hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
            humanoid = rbx_instance_find_first_child_of_class(task, character, "Humanoid");
            rbx_player_set_last_input_timestamp(task, local_player, 999999999); //Anti-Afk
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            float old_dist = 20000.0f;
            vm_address_t __closest_chest = 0;
            char __ce = false;
            
            long workspace_child_count = 0;
            rbx_child_t* workspace_children = rbx_instance_get_children(task, workspace, &workspace_child_count);
            if (workspace_children)
            {
                for (long i = 0 ; i < workspace_child_count ; i++)
                {
                    vm_address_t object = workspace_children[i].child_address;
                    if (object)
                    {
                        long object_name_len = 0;
                        char* object_name = rbx_instance_get_name(task, object, &object_name_len);
                        char is_chest = false;
                        if (object_name)
                        {
                            if (strcmp(object_name, "Chest3") == 0)
                            {
                                is_chest = true;
                            }
                            if (strcmp(object_name, "Chest2") == 0)
                            {
                                is_chest = true;
                            }
                            if (strcmp(object_name, "Chest1") == 0)
                            {
                                is_chest = true;
                            }
                            vm_deallocate(mach_task_self_, (vm_address_t)object_name, object_name_len);
                        }
                        if (is_chest && hrp)
                        {
                            rbx_cframe_t object_cframe = rbx_basepart_get_cframe(task, object);
                            rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, hrp);
                            float dist = vector3_dist_dif(hrp_cframe.pos, object_cframe.pos);
                            if (dist < old_dist)
                            {
                                old_dist = dist;
                                __ce = true;
                                __closest_chest = object;
                            }
                        }
                    }
                    usleep(100);
                }
                vm_deallocate(mach_task_self_, (vm_address_t)workspace_children, workspace_child_count * sizeof(rbx_child_t));
            }
            closest_chest = __closest_chest;
            closest_chest_exists = __ce;
            usleep(100000);
        }
    });

    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (closest_chest && closest_chest_exists)
            {
                vm_address_t parent = rbx_instance_get_parent(task, closest_chest);
                vm_address_t instance_self = rbx_instance_get_self(task, closest_chest);
                if (parent && instance_self == closest_chest)
                {
                    vel = vector3_unit(rbx_basepart_get_cframe(task, closest_chest).pos, rbx_basepart_get_cframe(task, hrp).pos);
                    vel = vector3_mul_num(vel, fly_speed);
                    rbx_basepart_set_gravity(task, hrp, 0.0f);
                    rbx_basepart_set_velocity(task, hrp, vel);
                }
            }
            usleep(500);
        }
    });
    
    
}







