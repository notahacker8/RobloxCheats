


void tds_play_round(task_t task, tds_autoplay_options_t options)
{
    pid_t pid = -1;
    pid_for_task(task, &pid);
    if (options.auto_set_prio)
    {
        setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_DARWIN_BG);
    }
    if (options.should_return_to_lobby)
    {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
            int mins = 25;
            sleep(60 * mins);
            printf("%d minutes have passed, terminating...\n", mins);
            char cmd[200];
            bzero(cmd, 200);
            sprintf(cmd, "kill -9 %d", pid);
            system(cmd);
            exit(RBX_TDS_EXIT_CODE_GAME_TERMINATED);
        });
    }
    printf("- TOWER DEFENSE SIMULATOR (AUTO-PLAY) -\n");
    printf("Make sure your window is as small as possible!!!\n");
    printf("Make sure your performance stats are OFF!!!\n");
    
    LIBESP_DYLIB_SETUP
    
    shared_memory->INPUT_COUNT = 5;
    shared_memory->INPUT_USLEEP_TIME = 10000;

    shared_memory->WINDOW_CAPTURE_USLEEP_TIME = 10000;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t coregui = rbx_instance_find_first_child_of_class(task, game, "CoreGui");
    
    vm_address_t pickups_model = rbx_instance_find_first_child(task, workspace, "Pickups");
    vm_address_t music_stringvalue = rbx_instance_find_first_child(task, workspace, "Music");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t towers_folder = rbx_instance_find_first_child(task, workspace, "Towers");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t replicated_storage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t rs_state_folder = rbx_instance_find_first_child(task, replicated_storage, "State");
    vm_address_t health_folder = rbx_instance_find_first_child(task, rs_state_folder, "Health");
    vm_address_t current_health_intvalue = rbx_instance_find_first_child(task, health_folder, "Current");
    vm_address_t timer_folder = rbx_instance_find_first_child(task, rs_state_folder, "Timer");
    vm_address_t time_intvalue = rbx_instance_find_first_child(task, timer_folder, "Time");
    vm_address_t map_stringvalue = rbx_instance_find_first_child(task, rs_state_folder, "Map");
    
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t coins_intvalue = rbx_instance_find_first_child(task, local_player, "Coins");
    vm_address_t gems_intvalue = rbx_instance_find_first_child(task, local_player, "Gems");
    vm_address_t level_intvalue = rbx_instance_find_first_child(task, local_player, "Level");
    vm_address_t playergui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
    
    vm_address_t reactoverridesvote_gui = rbx_instance_find_first_child(task, playergui, "ReactOverridesVote");
    vm_address_t reactuniversalhotbar_gui = rbx_instance_find_first_child(task, playergui, "ReactUniversalHotbar");
    vm_address_t reactgametopgamedisplay_gui = rbx_instance_find_first_child(task, playergui, "ReactGameTopGameDisplay");
    vm_address_t reactupgrades_gui = rbx_instance_find_first_child(task, playergui, "ReactUpgrades");
    vm_address_t reactgamenewrewards_gui = rbx_instance_find_first_child(task, playergui, "ReactGameNewRewards");
    vm_address_t reactgameintermission_gui = rbx_instance_find_first_child(task, playergui, "ReactGameIntermission");

    static vm_address_t vote_prompt_tl = 0;
    static vm_address_t vote_count_tl = 0;
    
    static vm_address_t wave_frame = 0;
    static vm_address_t wave_value_tl = 0;
    
    static vm_address_t cash_frame = 0;
    static vm_address_t cash_amount_tl = 0;
    
    static vm_address_t gameover_frame = 0;
    static vm_address_t playagain_frame = 0;
    static vm_address_t restartmatch_tl = 0;
    
    static vm_address_t intermission_ready_tl = 0;
    
    static char img_verif_can_skip = false;
    static char img_verif_tower_selected = false;
    
    static char img_verif_did_go_to_intermission = false;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            shared_memory->DID_CAPTURE_WINDOW = false;
            shared_memory->SHOULD_CAPTURE_WINDOW = true;
            while (shared_memory->DID_CAPTURE_WINDOW == false)
            {
                usleep(10000);
            }
            //printf("received capture\n");
            @autoreleasepool {
                //NSImage* img1 = NSImageFromPNGFile(@"~/Desktop/window_capture_1.png");
                
                //WritePNGFromNSImage(img1, @"~/Desktop/window_capture.png");
                //NSImage* img2 = readPNG("~/Desktop/window_capture_2.png");
                NSImage* img1 = [[NSImage alloc] initWithData:[NSData dataWithBytes:shared_memory->WINDOW_CAPTURE_DATA length:shared_memory->WINDOW_CAPTURE_DATA_LENGTH]];
                //WritePNGFromNSImage(img1, @"~/Desktop/window_capture.png");
                
                NSImage* skipwave_green_crop = CropNSImage(img1, NSMakeRect(842, 1010, 17, 16));
                
                NSImage* tower_level_crop = CropNSImage(img1, NSMakeRect(51, 605, 88, 28));
                NSImage* intermission_ready_green_btn_crop = CropNSImage(img1, NSMakeRect(696, 170, 38, 23));
                
                //WritePNGFromNSImage(intermission_ready_green_btn_crop, @"~/Desktop/window_capture_crop.png");
                
                //printf("processed crops\n");
                
                ESP_Color avg = AverageColorOfNSImage(skipwave_green_crop);
                img_verif_can_skip = (avg.g > 0.9 && COLORMAG(avg) > 0.9);
                
                avg = AverageColorOfNSImage(intermission_ready_green_btn_crop);
                img_verif_did_go_to_intermission = (avg.g == 1 &&
                                                    avg.r > 0.13 && avg.r < 0.14 &&
                                                    avg.b > 0.20 && avg.b < 0.22 &&
                                                    avg.a == 1);
                
                
                img_verif_tower_selected = [ReadTextFromNSImage(tower_level_crop, 0.25) containsString:@"Level"];
                
                //printf("%f, %f, %f, %f, %f\n", avg.r, avg.g, avg.b, avg.a, COLORMAG(avg));
                
            }
            sleep(1);
        }
    });
    //sleep(10000);
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            if (rbx_get_placeid(task) == 0)
            {
                sleep(10);
                if (rbx_get_placeid(task) == 0)
                {
                    printf("PlaceID is 0 after 10s (possible load failure), exiting...\n");
                    char cmd[200];
                    bzero(cmd, 200);
                    sprintf(cmd, "kill -9 %d", pid);
                    system(cmd);
                    exit(RBX_TDS_EXIT_CODE_GAME_TERMINATED);
                }
            }
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            if (img_verif_did_go_to_intermission)
            {
                printf("Currently in intermission lobby, exiting...\n");
                exit(RBX_TDS_EXIT_CODE_ROUND_ENDED_TO_INTERMISSION);
            }
            if (rbx_get_placeid(task) == RBX_TDS_LOBBY_PLACE_ID)
            {
                printf("Currently in main lobby, exiting...\n");
                exit(RBX_TDS_EXIT_CODE_ROUND_ENDED_TO_MAIN_LOBBY);
            }
            if (rbx_check_if_recieved_grey_prompt(task, coregui))
            {
                printf("Possible disconnect, exiting...\n");
                exit(RBX_TDS_EXIT_CODE_ROUND_ENDED_TO_MAIN_LOBBY);
            }
            
            vote_prompt_tl = rbx_instance_find_first_descendant(task, reactoverridesvote_gui, "prompt", "TextLabel", 10, 100);
            vote_count_tl = rbx_instance_find_first_descendant(task, reactoverridesvote_gui, "count", "TextLabel", 10, 100);
            
            wave_frame = rbx_instance_find_first_descendant(task, reactgametopgamedisplay_gui, "wave", "Frame", 5, 1000);
            wave_value_tl = rbx_instance_find_first_descendant(task, wave_frame, "value", "TextLabel", 5, 1000);
            
            cash_frame = rbx_instance_find_first_descendant(task, reactuniversalhotbar_gui, "cash", "Frame", 100, 10000);
            cash_amount_tl = rbx_instance_find_first_child(task, cash_frame, "amount");
            
            vm_address_t reactgamenewrewards_frame = rbx_instance_find_first_child_of_class(task, reactgamenewrewards_gui, "Frame");
            gameover_frame = rbx_instance_find_first_child(task, reactgamenewrewards_frame, "gameOver");
            playagain_frame = rbx_instance_find_first_descendant(task, gameover_frame, "PlayAgain", "Frame", 100, 10000);
            restartmatch_tl = rbx_instance_find_first_descendant(task, playagain_frame, "text", "TextLabel", 100, 10000);
            
            sleep(2);
        }
    });
    
    sleep(2);
    
    ANTI_AFK;
    
    static tds_tower_placement_t* placements = NULL;
    
    if (options.auto_get_map_placements)
    {
        for (long x = 0 ; x < options.map_count ; x++)
        {
            if (rbx_stringvalue_equals_value(task, map_stringvalue, options.map_dict[x * 2]))
            {
                printf("Map found: %s\n",  (char*)options.map_dict[x * 2]);
                placements = options.map_dict[(x * 2) + 1];
                x = options.map_count;
            }
        }
    }
    else
    {
        placements = options.t_placements;
    }
    
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    
    
    static long cash = 0;
    static long wave = 0;
    static long time = 0;
    static long health = 0;
    
    static int rounds_completed = 0;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        sleep(5);
        for (;;)
        {
            if (health == 0)
            {
                rounds_completed++;
                printf("Round ended @ wave %ld | Completed %d round(s)\n", wave, rounds_completed);
                while (health == 0)
                {
                    sleep(1);
                }
            }
            sleep(1);
        }
    });
    
    static rbx_cframe_t really_high_cframe = {
        .r0 = 1, .r1 = 0, .r2 = 0,
        .r10 = 0, .r11 = 1, .r12 = 0,
        .r20 = 0, .r21 = 0, .r22 = 1,
        .pos = {.x = 0, .y = 10000, .z = 0}
    };
    static rbx_cframe_t new_hrp_cframe = {
        .r0 = 1, .r1 = 0, .r2 = 0,
        .r10 = 0, .r11 = 1, .r12 = 0,
        .r20 = 0, .r21 = 0, .r22 = 1,
        .pos = {.x = 0, .y = 10000, .z = 0}
    };
    static rbx_cframe_t new_camera_cframe = {
        .r0 = -1, .r1 = 0, .r2 = 0,
        .r10 = 0, .r11 = 0, .r12 = 1,
        .r20 = 0, .r21 = 1, .r22 = 0,
    };
    static char should_set_camera_cframe = false;
    
    
    
    
    
    
#pragma mark - Loops -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            if (should_set_camera_cframe)
            {
                rbx_camera_set_cframe(task, camera, new_camera_cframe);
                usleep(5);
            }
            else
            {
                usleep(10000);
            }
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            my_character = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            my_hrp_cframe_address = rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            sleep(3);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            //rbx_print_descendants(task, pickups_model, 0, 2);
            FOR_EACH_CHILD(pickups_model, i, {
                vm_address_t object = child;
                if (vm_read_uint64_t_value(task, object) == task_base_address + RBX_OBJECT_MeshPart_POINTEE_OFFSET)
                {
                    if (rbx_instance_get_parent(task, object))
                    {
                        rbx_cframe_t cf = rbx_basepart_get_cframe(task, my_hrp);
                        cf.pos = rbx_basepart_get_cframe(task, object).pos;
                        if (cf.pos.y < 100000)
                        {
                            for (int x = 0 ; x < 100 ; x++)
                            {
                                
                                rbx_basepart_set_cframe(task, my_hrp, cf);
                            }
                        }
                        sleep(1);
                    }
                }
            })
            sleep(1);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            //rbx_print_descendants(task, hotbar_frame, 0, 1);
            long wave_str_l = 0;
            char* wave_str = rbx_textlabel_get_text(task, wave_value_tl, &wave_str_l);
            if (wave_str)
            {
                wave = atol(wave_str);
                vm_deallocate(mach_task_self_, (vm_address_t)wave_str, wave_str_l);
            }
            
            long cash_str_l = 0;
            char* cash_str = rbx_textlabel_get_text(task, cash_amount_tl, &cash_str_l);
            if (cash_str)
            {
                int __i = 0;
                char c[strlen(cash_str) + 1];
                bzero(c, sizeof(c));
                for (int i = 0 ; i < strlen(cash_str) ; i++)
                {
                    if (isdigit(cash_str[i]))
                    {
                        c[__i++] = cash_str[i];
                    }
                }
                cash = atol(c);
                
                vm_deallocate(mach_task_self_, (vm_address_t)cash_str, cash_str_l);
            }
            
            time = rbx_intvalue_get_value(task, time_intvalue);
            health = rbx_intvalue_get_value(task, current_health_intvalue);
            
            
            //printf("wave: %ld\t\tcash: %ld\t\ttime: %ld\t\thealth: %ld\n", wave, cash, time, health);
            
            usleep(100000);
        }
    });
    
    
    static int molten_with_scouts_total_placement_count = 16;
    static int hardcore_total_placement_count = 8;
    
    static char restarting = false;
    
    static long hardcore_tracked_tower_data[8];
    for (int i = 0 ; i < 8 ; i++) {hardcore_tracked_tower_data[i] = -1;}
    
    ///For molten
    static int upgrade_lvl = 0;
    static int placement_lvl = 0;
    static int tower_to_lvl_index = 0;
    
    static tower_upgrade_info_t tui;
    
    static tower_upgrade_info_t scout_tui = {.max_lvl = 4, .lvl_costs = {150, 50, 200, 950, 2500}};
    static tower_upgrade_info_t gold_scout_tui = {.max_lvl = 4, .lvl_costs = {250, 150, 600, 2250, 3400}};
    static tower_upgrade_info_t shotgunner_tui = {.max_lvl = 4, .lvl_costs = {300, 150, 950, 2500, 6500}};
    static tower_upgrade_info_t freezer_tui = {.max_lvl = 4, .lvl_costs = {650, 225, 650, 2000, 4500}};
    
    static tower_upgrade_info_t hc_trapper_tui = {.max_lvl = 4, .lvl_costs = {750, 750, 2250, 7500, 20250}};
    static tower_upgrade_info_t hc_militant_tui = {.max_lvl = 4, .lvl_costs = {900, 225, 1575, 4687, 8625}};
    
    if (options.autofarm_type == RBX_TDS_AUTOFARM_TYPE_MOLTEN_WITH_SCOUTS_DEATH_FARM)
    {
        if (options.is_golden_scout)
        {
            tui = gold_scout_tui;
            placement_lvl = 1;
        }
        else
        {
            tui = scout_tui;
            placement_lvl = 2;
        }
        upgrade_lvl = placement_lvl + 1;
    }
    else if (options.autofarm_type == RBX_TDS_AUTOFARM_TYPE_MOLTEN_WITH_SHOTGUNNERS_DEATH_FARM)
    {
        placement_lvl = 2;
        upgrade_lvl = 3;
    }
    
    ///For molten with shotgunners
    static int initial_shotgunners_upgrade_index = 0;
    static int initial_shotgunners_upgrade_lvl = 1;
    static tower_upgrade_info_t* mws_tuis[3] = {NULL, &shotgunner_tui, &freezer_tui};
    
    
#pragma mark Gameplay loop
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            COPY_VAR(restartmatch_tl)
            COPY_VAR(vote_count_tl)
            
#pragma mark Molten with scouts
            if (options.autofarm_type == RBX_TDS_AUTOFARM_TYPE_MOLTEN_WITH_SCOUTS_DEATH_FARM)
            {
                if (wave == 0 && rbx_textlabel_contains_text(task, __vote_count_tl, "0/1") && tds_get_tower_count(task, towers_folder) > 0)
                {
                    printf("Readying...\n");
                    TDS_READY_CLICK;
                }
                else if (rbx_textlabel_contains_text(task, __restartmatch_tl, "0/1"))
                {
                    while (rbx_textlabel_contains_text(task, __restartmatch_tl, "0/1"))
                    {
                        printf("Restarting...\n");
                        TDS_RESTART_CLICK;
                        sleep(1);
                    }
                    restarting = true;
                }
                else if (options.should_return_to_lobby && (rbx_textlabel_contains_text(task, __restartmatch_tl, "Play") || (health == 0 && wave != 0)))
                {
                    while (rbx_get_placeid(task) != RBX_TDS_LOBBY_PLACE_ID)
                    {
                        printf("Returning to main lobby...\n");
                        TDS_RETURN_TO_LOBBY_CLICK
                        sleep(2);
                    }
                    exit(RBX_TDS_EXIT_CODE_ROUND_ENDED_TO_MAIN_LOBBY);
                }
                else if (wave > 0 && rbx_textlabel_contains_text(task, __vote_count_tl, "0/1") && img_verif_can_skip)
                {
                    printf("Attempting to skip...\n");
                    TDS_SKIP_CLICK;
                }
                else
                {
                    if (restarting)
                    {
                        printf("Waiting for towers to clear and wave to reset...\n");
                        if (options.is_golden_scout)
                        {
                            placement_lvl = 1;
                        }
                        else
                        {
                            placement_lvl = 2;
                        }
                        upgrade_lvl = placement_lvl + 1;
                        tower_to_lvl_index = 0;
                        TDS_WAIT_WHILE_COND(tds_get_tower_count(task, towers_folder) != 0)
                        TDS_WAIT_WHILE_COND(!(wave == 0))
                        restarting = false;
                    }
                    long current_tower_count = tds_get_tower_count(task, towers_folder);
                    if (cash >= sum_from_to(tui.lvl_costs, 0, placement_lvl) && (current_tower_count < molten_with_scouts_total_placement_count))
                    {
                        TDS_PLACE_TOWER(placements[current_tower_count]);
                        TDS_UPGRADE_TOWER_UNTIL_LEVEL(task, shared_memory, reactupgrades_gui, placement_lvl);
                        TDS_DESELECT_TOWER()
                    }
                    if (cash >= tui.lvl_costs[upgrade_lvl] &&
                        (current_tower_count == molten_with_scouts_total_placement_count) &&
                        (tower_to_lvl_index < molten_with_scouts_total_placement_count))
                    {
                        TDS_SELECT_TOWER(placements[tower_to_lvl_index]);
                        long lvl = TDS_UPGRADE_TOWER_UNTIL_LEVEL(task, shared_memory, reactupgrades_gui, upgrade_lvl);
                        TDS_DESELECT_TOWER()
                        if (lvl >= upgrade_lvl)
                        {
                            tower_to_lvl_index++;
                        }
                    }
                    if (tower_to_lvl_index == molten_with_scouts_total_placement_count && upgrade_lvl < tui.max_lvl)
                    {
                        upgrade_lvl++;
                        tower_to_lvl_index = 0;
                    }
                }
            }
#pragma mark Molten with shotgunners
            if (options.autofarm_type == RBX_TDS_AUTOFARM_TYPE_MOLTEN_WITH_SHOTGUNNERS_DEATH_FARM)
            {
                if (wave == 0 && rbx_textlabel_contains_text(task, __vote_count_tl, "0/1") && tds_get_tower_count(task, towers_folder) == 2)
                {
                    printf("Readying...\n");
                    TDS_READY_CLICK;
                }
                else if (rbx_textlabel_contains_text(task, __restartmatch_tl, "0/1"))
                {
                    while (rbx_textlabel_contains_text(task, __restartmatch_tl, "0/1"))
                    {
                        printf("Restarting...\n");
                        TDS_RESTART_CLICK;
                        sleep(1);
                    }
                    restarting = true;
                }
                else if (options.should_return_to_lobby && (rbx_textlabel_contains_text(task, __restartmatch_tl, "Play") || (health == 0 && wave != 0)))
                {
                    while (rbx_get_placeid(task) != RBX_TDS_LOBBY_PLACE_ID)
                    {
                        printf("Returning to main lobby...\n");
                        TDS_RETURN_TO_LOBBY_CLICK
                        sleep(2);
                    }
                    exit(RBX_TDS_EXIT_CODE_ROUND_ENDED_TO_MAIN_LOBBY);
                }
                else if (wave > 0 && rbx_textlabel_contains_text(task, __vote_count_tl, "0/1") && img_verif_can_skip)
                {
                    printf("Attempting to skip...\n");
                    TDS_SKIP_CLICK;
                }
                else
                {
                    if (restarting)
                    {
                        printf("Waiting for towers to clear and wave to reset...\n");
                        initial_shotgunners_upgrade_index = 0;
                        initial_shotgunners_upgrade_lvl = 1;
                        upgrade_lvl = 3;
                        placement_lvl = 2;
                        tower_to_lvl_index = 0;
                        TDS_WAIT_WHILE_COND(tds_get_tower_count(task, towers_folder) != 0)
                        TDS_WAIT_WHILE_COND(!(wave == 0))
                        restarting = false;
                    }
                    long current_tower_count = tds_get_tower_count(task, towers_folder);
                    if (current_tower_count < 2)
                    {
                        if (cash >= shotgunner_tui.lvl_costs[0])
                        {
                            TDS_PLACE_TOWER(placements[current_tower_count]);
                            TDS_UPGRADE_TOWER_UNTIL_LEVEL(task, shared_memory, reactupgrades_gui, 0);
                            TDS_DESELECT_TOWER()
                        }
                    }
                    if (current_tower_count == 2 && initial_shotgunners_upgrade_lvl < 3)
                    {
                        if (cash >= shotgunner_tui.lvl_costs[initial_shotgunners_upgrade_lvl])
                        {
                            TDS_SELECT_TOWER(placements[initial_shotgunners_upgrade_index]);
                            long lvl = TDS_UPGRADE_TOWER_UNTIL_LEVEL(task, shared_memory, reactupgrades_gui, initial_shotgunners_upgrade_lvl);
                            TDS_DESELECT_TOWER()
                            if (lvl >= initial_shotgunners_upgrade_lvl)
                            {
                                initial_shotgunners_upgrade_index++;
                            }
                            if (initial_shotgunners_upgrade_index == 2)
                            {
                                initial_shotgunners_upgrade_lvl++;
                                initial_shotgunners_upgrade_index = 0;
                            }
                        }
                    }
                    if (current_tower_count < 12 && initial_shotgunners_upgrade_lvl > 2)
                    {
                        if (cash >= sum_from_to(mws_tuis[placements[current_tower_count].type]->lvl_costs, 0, placement_lvl))
                        {
                            TDS_PLACE_TOWER(placements[current_tower_count]);
                            TDS_UPGRADE_TOWER_UNTIL_LEVEL(task, shared_memory, reactupgrades_gui, placement_lvl);
                            TDS_DESELECT_TOWER()
                        }
                    }
                    if (current_tower_count == 12 && tower_to_lvl_index < 12)
                    {
                        if (cash >= mws_tuis[placements[tower_to_lvl_index].type]->lvl_costs[upgrade_lvl])
                        {
                            TDS_SELECT_TOWER(placements[tower_to_lvl_index]);
                            long lvl = TDS_UPGRADE_TOWER_UNTIL_LEVEL(task, shared_memory, reactupgrades_gui, upgrade_lvl);
                            TDS_DESELECT_TOWER()
                            if (lvl >= upgrade_lvl)
                            {
                                tower_to_lvl_index++;
                            }
                        }
                    }
                    if (tower_to_lvl_index == 12 && upgrade_lvl < 4)
                    {
                        upgrade_lvl++;
                        tower_to_lvl_index = 0;
                    }
                }
            }
#pragma mark Hardcore
            if (options.autofarm_type == RBX_TDS_AUTOFARM_TYPE_HARDCORE_DEATH_FARM)
            {
                if (wave == 0 && rbx_textlabel_contains_text(task, __vote_count_tl, "0/1"))
                {
                    printf("Readying...\n");
                    TDS_READY_CLICK;
                }
                else if (rbx_textlabel_contains_text(task, __restartmatch_tl, "0/1"))
                {
                    while (rbx_textlabel_contains_text(task, __restartmatch_tl, "0/1"))
                    {
                        printf("Restarting...\n");
                        TDS_RESTART_CLICK;
                        sleep(1);
                    }
                    restarting = true;
                }
                else if (options.should_return_to_lobby && (rbx_textlabel_contains_text(task, __restartmatch_tl, "Play") || (health == 0 && wave != 0)))
                {
                    while (rbx_get_placeid(task) != RBX_TDS_LOBBY_PLACE_ID)
                    {
                        printf("Returning to main lobby...\n");
                        TDS_RETURN_TO_LOBBY_CLICK
                        sleep(2);
                    }
                    exit(RBX_TDS_EXIT_CODE_ROUND_ENDED_TO_MAIN_LOBBY);
                }
                else if (wave > 0 && rbx_textlabel_contains_text(task, __vote_count_tl, "0/1") && img_verif_can_skip)
                {
                    printf("Attempting to skip...\n");
                    TDS_SKIP_CLICK;
                }
                else
                {
                    if (restarting)
                    {
                        for (int i = 0 ; i < 8 ; i++) {hardcore_tracked_tower_data[i] = -1;}
                        TDS_WAIT_WHILE_COND(tds_get_tower_count(task, towers_folder) != 0)
                        TDS_WAIT_WHILE_COND(!(wave == 0))
                        TDS_WAIT_WHILE_COND(!(cash < 800))
                        restarting = false;
                    }
                    long current_tower_count = tds_get_tower_count(task, towers_folder);
                    
#define TDS_HC_PLACE_TOWER(idx)\
TDS_PLACE_TOWER(placements[idx]);\
if (tds_get_tower_count(task, towers_folder) == current_tower_count + 1)\
{\
    hardcore_tracked_tower_data[current_tower_count] = 0;\
}\
TDS_DESELECT_TOWER()
                    
#define TDS_HC_UPGRADE_TOWER(idx)\
TDS_DESELECT_TOWER()\
TDS_SELECT_TOWER(placements[idx])\
hardcore_tracked_tower_data[idx] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);\
TDS_DESELECT_TOWER()
                    
                    if (current_tower_count == 0)
                    {
                        //Place 1st militant
                        if (hardcore_tracked_tower_data[0] == -1 && (cash >= hc_militant_tui.lvl_costs[0]))
                        {
                           TDS_HC_PLACE_TOWER(0)
                        }
                    }
                    if (current_tower_count == 1)
                    {
                        if (hardcore_tracked_tower_data[0] == 0 && (cash >= hc_militant_tui.lvl_costs[1]))
                        {
                            TDS_HC_UPGRADE_TOWER(0);
                        }
                        if (hardcore_tracked_tower_data[0] == 1 && (cash >= hc_militant_tui.lvl_costs[2]))
                        {
                            TDS_HC_UPGRADE_TOWER(0);
                        }
                        //Place 1st trapper
                        if (hardcore_tracked_tower_data[0] == 2 && (cash >= hc_trapper_tui.lvl_costs[0]))
                        {
                           TDS_HC_PLACE_TOWER(1)
                        }
                    }
                    if (current_tower_count == 2)
                    {
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 0 && (cash >= hc_trapper_tui.lvl_costs[1]))
                        {
                            TDS_HC_UPGRADE_TOWER(current_tower_count - 1)
                        }
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 1 && (cash >= hc_trapper_tui.lvl_costs[2]))
                        {
                            TDS_HC_UPGRADE_TOWER(current_tower_count - 1)
                        }
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 2 && (cash >= hc_trapper_tui.lvl_costs[0]))
                        {
                            TDS_HC_PLACE_TOWER(current_tower_count)
                        }
                    }
                    if (current_tower_count == 3)
                    {
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 0 && (cash >= hc_trapper_tui.lvl_costs[1]))
                        {
                            TDS_HC_UPGRADE_TOWER(current_tower_count - 1)
                        }
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 1 && (cash >= hc_trapper_tui.lvl_costs[2]))
                        {
                            TDS_HC_UPGRADE_TOWER(current_tower_count - 1)
                        }
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 2 && (cash >= hc_militant_tui.lvl_costs[3]) && hardcore_tracked_tower_data[0] == 2)
                        {
                            TDS_HC_UPGRADE_TOWER(0)
                        }
                        if (hardcore_tracked_tower_data[0] == 3 && (cash >= hc_trapper_tui.lvl_costs[0]))
                        {
                            TDS_HC_PLACE_TOWER(current_tower_count)
                        }
                    }
                    if (current_tower_count == 4)
                    {
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 0 && (cash >= hc_trapper_tui.lvl_costs[1]))
                        {
                            TDS_HC_UPGRADE_TOWER(current_tower_count - 1)
                        }
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 1 && (cash >= hc_trapper_tui.lvl_costs[2]))
                        {
                            TDS_HC_UPGRADE_TOWER(current_tower_count - 1)
                        }
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 2 && (cash >= hc_trapper_tui.lvl_costs[0]))
                        {
                            TDS_HC_PLACE_TOWER(current_tower_count)
                        }
                    }
                    if (current_tower_count == 5)
                    {
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 0 && (cash >= hc_trapper_tui.lvl_costs[1]))
                        {
                            TDS_HC_UPGRADE_TOWER(current_tower_count - 1)
                        }
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 1 && (cash >= hc_trapper_tui.lvl_costs[2]))
                        {
                            TDS_HC_UPGRADE_TOWER(current_tower_count - 1)
                        }
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 2 && (cash >= hc_militant_tui.lvl_costs[0]))
                        {
                            TDS_HC_PLACE_TOWER(current_tower_count)
                        }
                    }
                    if (current_tower_count == 6)
                    {
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 0 && (cash >= hc_militant_tui.lvl_costs[1]))
                        {
                            TDS_HC_UPGRADE_TOWER(current_tower_count - 1)
                        }
                        if (hardcore_tracked_tower_data[current_tower_count - 1] == 1 && (cash >= hc_militant_tui.lvl_costs[2]))
                        {
                            TDS_HC_UPGRADE_TOWER(current_tower_count - 1)
                        }
                    }
                }
            }
            usleep(200000);
        }
    });
    
    
    
}




void tds_collect(task_t task)
{
    LIBESP_DYLIB_SETUP
    GENERIC_CHEAT_STARTER
    
    shared_memory->INPUT_COUNT = 5;
    shared_memory->INPUT_USLEEP_TIME = 1000;

    shared_memory->WINDOW_CAPTURE_USLEEP_TIME = 1000;
    
    ANTI_AFK
    
    for (;;)
    {
        sleep(1);
        TDS_KEYPRESS('f');
    }
}
