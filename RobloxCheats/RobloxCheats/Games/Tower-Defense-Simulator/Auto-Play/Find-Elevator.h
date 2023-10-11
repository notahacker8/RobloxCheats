
void tds_find_elevator(task_t task, long elevator_type, char** map_list, long list_length)
{
    printf("- TOWER DEFENSE SIMULATOR (ELEVATOR-FIND) -\n");
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    LIBESP_DYLIB_SETUP
    shared_memory->INPUT_QUEUE_COUNT = 3;
    shared_memory->INPUT_USLEEP_TIME = 50000;
    
get_globals: {}
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t elevators_folder = rbx_instance_find_first_child(task, workspace, "Elevators");
    
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t playergui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
    vm_address_t lobbygui = rbx_instance_find_first_child(task, playergui, "LobbyGui");
    vm_address_t elevator_frame = rbx_instance_find_first_child(task, lobbygui, "Elevator");
    vm_address_t elevator_content_il = rbx_instance_find_first_child(task, elevator_frame, "Content");
    
    if (!elevator_content_il)
    {
        sleep(3);
        goto get_globals;
    }
    
    static useconds_t check_usleep_time = 100000;
    
    static vm_address_t chosen_elevator = 0;
    static vm_address_t chosen_elevator_players_intvalue = 0;
    
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            my_character = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            
            long place_id = vm_read_8byte_value(task, task_base_address + RBX_PLACEID_OFFSET);
            if (place_id == RBX_TDS_LOBBY_PLACE_ID)
            {
                if (rbx_frame_get_visible(task, elevator_content_il) == false)
                {
                    check_usleep_time = 100000;
                    long elevator_count = 0;
                    rbx_child_t* elevators = rbx_instance_get_children(task, elevators_folder, &elevator_count);
                    if (elevators)
                    {
                        for (long i = 0 ; i < elevator_count ; i++)
                        {
                            vm_address_t elevator = elevators[i].child_address;
                            //Hardcore elevators have 8 children, normal have 7
                            long __elevator_type = 0;
                            {
                                rbx_child_t* c = rbx_instance_get_children(task, elevator, &__elevator_type);
                                vm_deallocate(mach_task_self_, (vm_address_t)c, __elevator_type * sizeof(rbx_child_t));
                            }
                            if (__elevator_type == elevator_type)
                            {
                                vm_address_t state_folder = rbx_instance_find_first_child(task, elevator, "State");
                                vm_address_t players_intvalue = rbx_instance_find_first_child(task, state_folder, "Players");
                                vm_address_t map_folder = rbx_instance_find_first_child(task, state_folder, "Map");
                                vm_address_t map_title_string_value = rbx_instance_find_first_child(task, map_folder, "Title");
                                
                                long map_name_l = 0;
                                char* map_name = rbx_stringvalue_get_value(task, map_title_string_value, &map_name_l);
                                if (map_name)
                                {
                                    for (long _i = 0 ; _i < list_length ; _i++)
                                    {
                                        if (strcmp(map_name, map_list[_i]) == 0)
                                        {
                                            if (rbx_intvalue_get_value(task, players_intvalue) == 0)
                                            {
                                                vm_address_t part = rbx_instance_find_first_child(task, elevator, "Touch");
                                                vm_address_t screen_part = rbx_instance_find_first_child(task, elevator, "Screen");
                                                vm_address_t surfacegui = rbx_instance_find_first_child(task, screen_part, "SurfaceGui");
                                                vm_address_t status_frame = rbx_instance_find_first_child(task, surfacegui, "Status");
                                                vm_address_t state_tl = rbx_instance_find_first_child(task, status_frame, "State");
                                                //rbx_print_descendants(task, elevator, 0, 4);
                                                long state_text_len = 0;
                                                char* state_text = rbx_textlabel_get_text(task, state_tl, &state_text_len);
                                                if (state_text)
                                                {
                                                    printf("Elevator state: %s\n", state_text);
                                                    if (strstr(state_text, "Teleporting") == NULL)
                                                    {
                                                        for (int __i = 0 ; __i < 100 ; __i++)
                                                        {
                                                            rbx_basepart_set_cframe(task, my_hrp, rbx_basepart_get_cframe(task, part));
                                                        }
                                                        chosen_elevator = elevator;
                                                        chosen_elevator_players_intvalue = players_intvalue;
                                                        _i = list_length;
                                                    }
                                                    vm_deallocate(mach_task_self_, (vm_address_t)state_text, state_text_len);
                                                }
                                            }
                                        }
                                    }
                                    vm_deallocate(mach_task_self_, (vm_address_t)map_name, map_name_l);
                                }
                                //rbx_print_descendants(task, map_folder, 0, 1);
                            }
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)elevators, elevator_count * sizeof(rbx_child_t));
                    }
                }
                else
                {
                    if (chosen_elevator && chosen_elevator_players_intvalue)
                    {
                        check_usleep_time = 10000;
                        long __v = rbx_intvalue_get_value(task, chosen_elevator_players_intvalue);
                        printf("Players in elevator: %ld\n", __v);
                        if (__v > 1)
                        {
                            TDS_EXIT_ELEVATOR_CLICK
                            chosen_elevator_players_intvalue = 0;
                            chosen_elevator = 0;
                        }
                    }
                }
            }
            else
            {
                printf("Successfully entered game...\n");
                exit(0);
            }
            usleep(check_usleep_time);
        }
    });
    
}
