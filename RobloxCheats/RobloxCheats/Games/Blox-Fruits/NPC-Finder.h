

/*
 Second Sea Aura Master Rough Locations:
 
 -1970, 54, -2615
 -201, 81, -5035
 6009, 497, -6772
 -3483, 297, -10007
 -5172, 187, -5397
 903, 139, 33096
 
Third Sea Aura Master Rough Locations:
 
 //Indexes according to wiki
-104, 240, 5794 [0]
 -13262, 536, -10540 [1]
 -13554, 554, -10160 [2]
 -4964, 363, -3025 [3]
 -11802, 785, -9505 [4]
 2620, 197, -7669 [5]
 5745, 803, 236 [6]
 */


void blox_fruits_find_master_of_auras(task_t task)
{
    printf("- BLOX FRUITS (MASTER OF AURAS FINDER) -\n");
    printf("USE THIS ONLY IN THE THIRD SEA!!!\n");
    
    
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
    vm_address_t npcs_folder = rbx_instance_find_first_child(task, workspace, "NPCs");
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
    
    static int rough_pos_index = 2;
    static vector3_t rough_pos_list[7];
    rough_pos_list[0] = (vector3_t){.x = -104, .y = 240, .z = 5794};
    rough_pos_list[1] = (vector3_t){.x = -4964, .y = 363, .z = -3025};
    rough_pos_list[2] = (vector3_t){.x = -13262, .y = 536, .z = -10540};
    rough_pos_list[3] = (vector3_t){.x = -13554, .y = 554, .z = -10160};
    rough_pos_list[4] = (vector3_t){.x = -11802, .y = 785, .z = -9505};
    rough_pos_list[5] = (vector3_t){.x = 2620, .y = 197, .z = -7669};
    rough_pos_list[6] = (vector3_t){.x = 5745, .y = 803, .z = 236};
    
    static vm_address_t character = 0;
    static vm_address_t hrp = 0;
    static char master_found = false;
    
    static vector3_t target_pos;
    target_pos = rough_pos_list[rough_pos_index];

    static vm_address_t master_head = 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            character = rbx_player_get_character(task, local_player);
            hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
            vm_address_t master = rbx_instance_find_first_child(task, npcs_folder, "Master of Auras");
            if (master)
            {
                printf("MASTER FOUND!!!\n");
                master_head = rbx_instance_find_first_child(task, master, "Head");
                master_found = true;
            }
            else
            {
                master_found = false;
            }
            rbx_player_set_last_input_timestamp(task, local_player, 999999999); //Anti-Afk
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (master_found)
            {
                target_pos = rbx_basepart_get_cframe(task, master_head).pos;
            }
            else
            {
                rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, hrp);
                float dist_dif = vector3_dist_dif(hrp_cframe.pos, target_pos);
                if (dist_dif < 20.0f)
                {
                    rough_pos_index++;
                    if (rough_pos_index > 6)
                    {
                        rough_pos_index = 0;
                    }
                    target_pos = rough_pos_list[rough_pos_index];
                }
            }
            usleep(10000);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {

            vector3_t vel = vector3_unit(target_pos, rbx_basepart_get_cframe(task, hrp).pos);
            vel = vector3_mul_num(vel, 275.0f);
            rbx_basepart_set_gravity(task, hrp, 0.0f);
            rbx_basepart_set_velocity(task, hrp, vel);
            //NSLog(@"");
            //rbx_print_descendants(task, chest, 0, 0);
            usleep(500);
        }
    });
}







