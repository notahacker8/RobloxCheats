
/*
void func(task_t task, vm_address_t instance)
{
    if (rbx_instance_is_a(task, instance, "Part") ||
        rbx_instance_is_a(task, instance, "MeshPart") ||
        rbx_instance_is_a(task, instance, "WedgePart") ||
        rbx_instance_is_a(task, instance, "CornerWedgePart") ||
        rbx_instance_is_a(task, instance, "TrussPart") ||
        rbx_instance_is_a(task, instance, "SpawnLocation") ||
        rbx_instance_is_a(task, instance, "Seat") ||
        rbx_instance_is_a(task, instance, "VehicleSeat"))
    {
        rbx_basepart_set_cancollide(task, instance, false);
    }
}
*/

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

vm_address_t blox_fruits_find_quest_giver(task_t task, vm_address_t npcs_folder)
{
    vm_address_t quest_giver = 0;
    long folder_child_count = 0;
    rbx_child_t* folder_children = rbx_instance_get_children(task, npcs_folder, &folder_child_count);
    for (long i = 0 ; i < folder_child_count ; i++)
    {
        vm_address_t character = folder_children[i].child_address;
        vm_address_t head = rbx_instance_find_first_child(task, character, "Head");
        vm_address_t questbbg = rbx_instance_find_first_child(task, head, "QuestBBG");
        vm_address_t title = rbx_instance_find_first_child(task, questbbg, "Title");
        
        char title_string_length = vm_read_1byte_value(task, title + RBX_TEXT_LABEL_SHORT_TEXT_OFFSET)/2;
        char* title_string = NULL;
        mach_msg_type_number_t data_cnt;
        vm_read(task, title + RBX_TEXT_LABEL_SHORT_TEXT_OFFSET + 1, title_string_length, (vm_offset_t*)&title_string, &data_cnt);
        if (title_string)
        {
            if (strcmp(title_string, "QUEST") == 0)
            {
                quest_giver = character;
            }
            vm_deallocate(mach_task_self_, (vm_address_t)title_string, title_string_length);
        }
    }
    return quest_giver;
}





void blox_fruits_left_click(task_t task,
                            vm_address_t load_address,
                            vm_address_t input_queue_offset,
                            vm_address_t input_queue_count_offset,
                            vm_address_t input_queue_finished_offset,
                            NSPoint point)
{
    static const int input_count = 3;
    Input inputs[input_count];
    
    inputs[0] = (Input){.type = 0, .duration = 0, .window_index = 0, .x = point.x, .y = point.y};
    inputs[1] = (Input){.type = 3, .duration = 10000, .window_index = 0, .x = point.x, .y = point.y};
    inputs[2] = (Input){.type = 4, .duration = 0, .window_index = 0, .x = point.x, .y = point.y};
    
    char f = 0;
    vm_write(task, load_address + input_queue_offset, (vm_address_t)inputs, (int)sizeof(inputs));
    vm_write(task, load_address + input_queue_count_offset, (vm_address_t)&input_count, sizeof(int));
    vm_write(task, load_address + input_queue_finished_offset, (vm_address_t)&f, 1);
    
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
}

void blox_fruits_simple_keypress(task_t task,
                                 vm_address_t load_address,
                                 vm_address_t input_queue_offset,
                                 vm_address_t input_queue_count_offset,
                                 vm_address_t input_queue_finished_offset,
                                 int keycode)
{
    static int input_count = 2;
    Input inputs[input_count];
    inputs[0] = (Input){.type = 1, .duration = 10000, .keycode = keycode, .window_index = 0};
    inputs[1] = (Input){.type = 2, .duration = 0, .keycode = keycode, .window_index = 0};
    
    char f = 0;
    vm_write(task, load_address + input_queue_offset, (vm_address_t)inputs, (int)sizeof(inputs));
    vm_write(task, load_address + input_queue_count_offset, (vm_address_t)&input_count, sizeof(int));
    vm_write(task, load_address + input_queue_finished_offset, (vm_address_t)&f, 1);
    
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
    
}


void blox_fruits_chest_collect(task_t task)
{
    printf(" - BLOX FRUITS (CHEST & FRUIT COLLECT) - \n");
    
    void* dlhandle = dlopen(__INJECTED_DYLIB_PATH__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB_PATH__);
    
    vm_offset_t should_quit_offset = gdso(dlhandle, s_load_address, "SHOULD_QUIT");
    vm_offset_t input_queue_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE");
    vm_offset_t input_queue_count_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE_COUNT");
    vm_offset_t input_queue_finished_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE_FINISHED");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB_PATH__);
    
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
            blox_fruits_simple_keypress(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, 48); //tab to close the leaderboard
        }
        //Enter the game
        blox_fruits_left_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, fast_mode_button_xy);
        blox_fruits_left_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, marines_team_button_xy);
        sleep(1);
    }

    /*
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
     */
 
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
            char b = false;
            vm_write(task, fruit_tool + 0x365, (vm_offset_t)&b, 1);
            
            sleep(1);
            blox_fruits_left_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, store_fruit_button_xy);
            sleep(1);
            blox_fruits_left_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, store_fruit_button_xy);
            sleep(1);
            blox_fruits_left_click(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, store_fruit_button_xy);
            sleep(1);
            blox_fruits_simple_keypress(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, 51); //delete to drop
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

