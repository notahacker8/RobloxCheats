

vm_address_t blox_fruits_find_first_fruit(task_t task, vm_address_t workspace)
{
    vm_address_t fruit = 0;
    long workspace_child_count = 0;
    rbx_child_t* workspace_children = rbx_instance_get_children(task, workspace, &workspace_child_count);
    for (long i = 0 ; i < workspace_child_count ; i++)
    {
        vm_address_t object = workspace_children[i].child_address;
        vm_address_t _fruit = rbx_instance_find_first_child(task, object, "Fruit");
        if (_fruit)
        {
            fruit = object;
            i = workspace_child_count;
        }
    }
    vm_deallocate(mach_task_self_, (vm_address_t)workspace_children, workspace_child_count * sizeof(rbx_child_t));
    return fruit;
}



void blox_fruits_chest_collect(task_t task)
{
    printf(" - BLOX FRUITS (CHEST & FRUIT COLLECT) - \n");
    printf("Size of Roblox window needs to be as SMALL AS POSSIBLE for this to work!!!\n");
    
    void* dlhandle = dlopen(__LIBESP_DYLIB_PATH__, RTLD_NOW);
    
    vm_address_t s_load_address = task_get_image_address_by_path(mach_task_self_, __LIBESP_DYLIB_PATH__);
    
    vm_offset_t should_quit_offset = gdso(dlhandle, s_load_address, "SHOULD_QUIT");
    vm_offset_t input_queue_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE");
    vm_offset_t input_queue_count_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE_COUNT");
    vm_offset_t input_queue_finished_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE_FINISHED");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        sleep(80);
        char sq = true;
        vm_write(task, load_address + should_quit_offset, (vm_offset_t)&sq, 1);
        exit(0);
    });
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players, "Player");
    
    static vm_address_t character = 0;
    static vm_address_t hrp = 0;
    
    //Start Screen
    NSPoint fast_mode_button_xy = {.x = 430, .y = 80};
    NSPoint marines_team_button_xy = {.x = 470, .y = 290};
    
    //Tool prompt (When we click while holding a physical fruit)
    NSPoint store_fruit_button_xy = {.x = 705, .y = 235};
    
    char leaderboard_closed = false;
    
    if (!local_player)
    {
        //This is how we have to terminate the RobloxPlayer process when we launch it from this program...
        char sq = true;
        vm_write(task, load_address + should_quit_offset, (vm_offset_t)&sq, 1);
    }
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            character = rbx_player_get_character(task, local_player);
            hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
            rbx_basepart_set_gravity(task, hrp, 0);
            sleep(1);
        }
    });
    
    while (rbx_player_get_team(task, local_player) == 0)
    {
        //Only press 'tab' once so we don't open the leaderboard again.
        if (!leaderboard_closed)
        {
            leaderboard_closed = true;
            send_simple_keypress(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, 48, 100000); //tab to close the leaderboard
        }
        //Enter the game
        send_mouse_move(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, fast_mode_button_xy);
        send_left_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, fast_mode_button_xy, 10000);
        send_mouse_move(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, marines_team_button_xy);
        send_left_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, marines_team_button_xy, 10000);
        sleep(1);
    }

    
    for (;;)
    {
        vm_address_t chest = rbx_instance_find_first_child(task, workspace, "Chest3");
        if (!chest)
        {
            chest = rbx_instance_find_first_child(task, workspace, "Chest2");
            if (!chest)
            {
                chest = rbx_instance_find_first_child(task, workspace, "Chest1");
                if (!chest)
                {
                    break;
                }
            }
        }
        if (chest)
        {
            rbx_cframe_t chest_cf = rbx_basepart_get_cframe(task, chest);
            chest_cf.pos.y += 1;
            rbx_basepart_set_cframe(task, hrp, chest_cf);
        }
        usleep(100000);
    }
 
    NSLog(@"Finished collecting chests.\n");
    for (;;)
    {
        vm_address_t fruit = blox_fruits_find_first_fruit(task, workspace);
        if (fruit)
        {
            rbx_camera_set_camera_subject(task, camera, fruit);
            rbx_basepart_set_cframe(task, hrp, rbx_camera_get_focus(task, camera));
        }
        vm_address_t fruit_tool = rbx_instance_find_first_child_of_class(task, character, "Tool");
        if (fruit_tool)
        {
            rbx_tool_set_canbedropped(task, fruit_tool, false);
            
            sleep(1);
            send_mouse_move(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, store_fruit_button_xy);
            send_left_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, store_fruit_button_xy, 100000);
            sleep(1);
            send_left_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, store_fruit_button_xy, 100000);
            sleep(1);
            send_left_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, store_fruit_button_xy, 100000);
            sleep(1);
            send_simple_keypress(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, 51, 100000); //delete to drop
        }
        if (!fruit)
        {
            break;
        }
        usleep(100000);
    }
    NSLog(@"Finished collecting fruits.\n");
    
    char sq = true;
    vm_write(task, load_address + should_quit_offset, (vm_offset_t)&sq, 1);
}
