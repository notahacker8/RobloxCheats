
void tds_intermission_lobby(task_t task, tds_intermission_lobby_options_t options)
{
    static int pid = -1;
    pid_for_task(task, &pid);
    if (options.auto_set_prio)
    {
        setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_MAX);
    }
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        sleep(60);
        char cmd[200];
        bzero(cmd, 200);
        sprintf(cmd, "kill -9 %d", pid);
        system(cmd);
        exit(RBX_TDS_EXIT_CODE_GAME_TERMINATED);
    });
    
    printf("- TOWER DEFENSE SIMULATOR (INTERMISSION-MANAGER) -\n");
    printf("Make sure your window is as small as possible!!!\n");
    LIBESP_DYLIB_SETUP
    
    shared_memory->INPUT_COUNT = 10;
    shared_memory->INPUT_USLEEP_TIME = 1000;

    shared_memory->WINDOW_CAPTURE_USLEEP_TIME = 10000;
    
_get_globals:{};
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    //rbx_print_descendants(task, workspace, 0, 1);
    vm_address_t trash_folder = rbx_instance_find_first_child(task, workspace, "Trash");
    //rbx_print_descendants(task, workspace, 0, 1);
    static vm_address_t intermissionlobby_model = 0;
    intermissionlobby_model = rbx_instance_find_first_child_partial(task, workspace, "IntermissionLobby");
    vm_address_t intermissionlobby_boards_folder = rbx_instance_find_first_child(task, intermissionlobby_model, "Boards");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t replicated_storage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t playergui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
    vm_address_t reactgameintermission_gui = rbx_instance_find_first_child(task, playergui, "ReactGameIntermission");
    
    vm_address_t veto_image_button = rbx_instance_find_first_descendant(task, reactgameintermission_gui, "veto", "ImageButton", 10, 10000);
    vm_address_t ready_image_button = rbx_instance_find_first_descendant(task, reactgameintermission_gui, "ready", "ImageButton", 10, 10000);
    
    vm_address_t veto_text_tl = rbx_instance_find_first_child_of_class(task, veto_image_button, "TextLabel");
    vm_address_t ready_text_tl = rbx_instance_find_first_child_of_class(task, ready_image_button, "TextLabel");
    
    if (!rbx_instance_is_valid_child(task, ready_text_tl) && !intermissionlobby_model)
    {
        printf("Global variables not fully found, retrying...\n");
        sleep(5);
        goto _get_globals;
    }
    
    //printf("Got globals\n");
    
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hum = 0;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            my_character = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            my_hum = rbx_instance_find_first_child_of_class(task, my_character, "Humanoid");
            intermissionlobby_model = rbx_instance_find_first_child_partial(task, workspace, "IntermissionLobby");
            sleep(1);
        }
    });
    
    TDS_WAIT_WHILE_COND(!my_hrp)
    
    //printf("hrp loaded\n");
    
    static char map_found = false;
    static vm_address_t voteplatform = 0;
    static rbx_cframe_t new_cframe;
    
_find_maps: {};
    //printf("Finding maps\n");
    FOR_EACH_CHILD(intermissionlobby_boards_folder, i, {
        //rbx_print_descendants(task, child, 0, 6);
        vm_address_t mapdisplay_surfacegui = rbx_instance_find_first_descendant(task, child, "MapDisplay", "SurfaceGui", 10, 10000);
        vm_address_t title_tl = rbx_instance_find_first_child_of_class(task, mapdisplay_surfacegui, "TextLabel");
        {
            for (long x = 0 ; x < options.map_count ; x++)
            {
                if (rbx_textlabel_equals_text(task, title_tl, options.map_dict[x * 2]))
                {
                    printf("Map found: %s\n",  (char*)options.map_dict[x * 2]);
                    voteplatform = rbx_instance_find_first_descendant(task, child, "VotePlatform", "Part", 10, 10000);
                    new_cframe = rbx_basepart_get_cframe(task, my_hrp);
                    new_cframe.pos = rbx_basepart_get_cframe(task, voteplatform).pos;
                    new_cframe.pos.y += 4;
                    map_found = true;
                    x = options.map_count;
                    i = child_count;
                }
            }
        }
    })
    
    if (!map_found && !rbx_textlabel_contains_text(task, veto_text_tl, "1/1"))
    {
        printf("Desirable map not found, voting veto...\n");
        TDS_INTERMISSION_VETO_CLICK;
        sleep(2);
        goto _find_maps;
    }
    
    if (!map_found)
    {
        printf("Failed to find map. Exiting...\n");
        if (options.close_game_on_failure)
        {
            char cmd[200];
            bzero(cmd, 200);
            sprintf(cmd, "kill -9 %d", pid);
            system(cmd);
            exit(RBX_TDS_EXIT_CODE_GAME_TERMINATED);
        }
        exit(0);
    }
    
    static char modifiers_selected = false;
    
    if (options.modif_count > 0 && options.modifiers_list)
    {
        
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
            for (;;)
            {
                shared_memory->DID_CAPTURE_WINDOW = false;
                shared_memory->SHOULD_CAPTURE_WINDOW = true;
                while (shared_memory->DID_CAPTURE_WINDOW == false)
                {
                    usleep(10000);
                }
                @autoreleasepool
                {
                    NSImage* img1 = [[NSImage alloc] initWithData:[NSData dataWithBytes:shared_memory->WINDOW_CAPTURE_DATA length:shared_memory->WINDOW_CAPTURE_DATA_LENGTH]];
                    
                    NSImage* modifier_button_crop = CropNSImage(img1, NSMakeRect(154, 54, 24, 22));
                    NSImage* modifier_text_crop = CropNSImage(img1, NSMakeRect(545, 836, 258, 50));
                    
                    ESP_Color avg_color = AverageColorOfNSImage(modifier_button_crop);
                    if (avg_color.a == 1 && COLORMAG(avg_color) == 0 && !modifiers_selected)
                    {
                        printf("Clicking modifiers...\n");
                        TDS_INTERMISSION_MODIFIERS_CLICK
                    }
                    if ([ReadTextFromNSImage(modifier_text_crop, 0.25) containsString:@"Game"])
                    {
                        printf("Searching for modifiers\n");
                        for (int i = 0 ; i < options.modif_count ; i++)
                        {
                            TDS_INTERMISSION_MODIFIERS_SEARCH_CLICK;
                            sleep(1);
                            send_first_responder_text_yielding(shared_memory, 4, options.modifiers_list[i]);
                            sleep(1);
                            TDS_INTERMISSION_MODIFIERS_TOP_CENTER_MODIFIER_CLICK
                            sleep(1);
                            TDS_INTERMISSION_MODIFIERS_SEARCH_CLICK;
                        }
                        
                        sleep(1);
                        
                        TDS_INTERMISSION_VOTE_FOR_MODIFIERS_CLICK
                        
                        modifiers_selected = true;
                        
                    }
                    //printf("%f, %f, %f, %f, %f\n", avg_color.r, avg_color.g, avg_color.b, avg_color.a, COLORMAG(avg_color));
                    //WritePNGFromNSImage(modifier_button_crop, @"~/Desktop/window_cap_crop.png");
                }
                sleep(1);
            }
        });
        
        TDS_WAIT_WHILE_COND(!modifiers_selected);
    }
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        while (intermissionlobby_model)
        {
            rbx_basepart_set_cframe(task, my_hrp, new_cframe);
            usleep(3);
        }
    });
    
    while (!rbx_instance_find_last_child_of_class_and_name(task, trash_folder, "MeshPart", "Crown"))
    {
        send_keypress_yielding(shared_memory, 4, keycode_for_character('e'), "e", 2000000);
        sleep(1);
    }
    
    while (intermissionlobby_model)
    {
        TDS_INTERMISSION_READY_CLICK;
        sleep(1);
    }
    
    printf("Finished intermission\n");
    exit(0);
}

