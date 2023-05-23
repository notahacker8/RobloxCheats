


void blox_fruits_auto_hunt(task_t task)
{
    printf("- BLOX FRUITS (AUTO HUNT) -\n");
    printf("To restore the map, use 'blox_fruits'\n");
    printf("Must use Buddha for max effectiveness!!!\n");
    
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
    //rbx_print_descendants(task, game, 0, 10);
    
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    
    static vm_address_t target_player = 0;
    static vm_address_t target_character = 0;
    static vm_address_t target_hrp = 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            my_character = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            rbx_player_set_last_input_timestamp(task, local_player, 999999999); //Anti-Afk
            target_character = rbx_player_get_character(task, target_player);
            target_hrp = rbx_instance_find_first_child(task, target_character, "HumanoidRootPart");
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (my_hrp && target_hrp)
            {
                rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
                vector3_t target_pos = rbx_basepart_get_cframe(task, target_hrp).pos;
                target_pos.y += 25.0f;
                
                vector3_t dir = vector3_unit(target_pos, my_hrp_cframe.pos);
                vector3_t vel = vector3_mul_num(dir, 275.0f);
                rbx_basepart_set_gravity(task, my_hrp, 0.0f);
                rbx_basepart_set_velocity(task, my_hrp, vel);
            }
            usleep(30);
        }
    });
    







#pragma mark - stdin Inputs -





    
    printf("Input partial (or full) username to hunt: \n");
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            char str[100];
            bzero(str, 100);
            fgets(str, 100, stdin);
            *strstr(str, "\xa") = 0;
            target_player = rbx_players_get_player_by_partial_name(task, players_service, str);
            printf("Hunting after: ");
            rbx_print_descendants(task, target_player, 0, 0);
        }
    });

}

