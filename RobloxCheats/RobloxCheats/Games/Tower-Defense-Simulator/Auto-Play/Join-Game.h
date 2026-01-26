


void tds_join_game_from_lobby(task_t task, tds_main_lobby_options_t options)
{
    static int pid = -1;
    pid_for_task(task, &pid);
    if (options.auto_set_prio)
    {
        setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_MAX);
    }
    printf("- TOWER DEFENSE SIMULATOR (AUTO-JOIN) -\n");
    printf("Make sure your window is as small as possible!!!\n");
    LIBESP_DYLIB_SETUP
    
    shared_memory->INPUT_COUNT = 5;
    shared_memory->INPUT_USLEEP_TIME = 1000;

    shared_memory->WINDOW_CAPTURE_USLEEP_TIME = 1000;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        sleep(60);
        char cmd[200];
        bzero(cmd, 200);
        sprintf(cmd, "kill -9 %d", pid);
        system(cmd);
        exit(RBX_TDS_EXIT_CODE_GAME_TERMINATED);
    });
    
    
    static vm_address_t game = 0;
    static vm_address_t players_service = 0;
    static vm_address_t local_player = 0;
    static vm_address_t playergui = 0;
    static vm_address_t reactoverridestopbar_screengui = 0;
    static vm_address_t topbar_frame = 0;
    static vm_address_t items_frame = 0;
    static vm_address_t rewards_tb = 0;
    
    static vm_address_t reward_tl = 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        sleep(15);
        game = rbx_find_game_address(task);
        players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
        local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
        playergui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
        reactoverridestopbar_screengui = rbx_instance_find_first_child(task, playergui, "ReactOverridesTopBar");
        topbar_frame = rbx_instance_find_first_child(task, reactoverridestopbar_screengui, "topBar");
        items_frame = rbx_instance_find_first_child(task, topbar_frame, "items");
        rewards_tb = rbx_instance_find_first_child(task, items_frame, "rewards");
        
        reward_tl = rbx_instance_find_first_descendant(task, rewards_tb, "label", "TextLabel", 10, 10000);
        
        //rbx_print_textlabel_descendants(task, reward_tl, 1, 1, 100);
        
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
                
                //For game options
                NSImage* text_img_crop1 = CropNSImage(img1, NSMakeRect(662, 784, 651, 128));
                
                NSString* text = ReadTextFromNSImage(text_img_crop1, 0.25);
                
                NSImage* playtime_text_img_crop1 = CropNSImage(img1, NSMakeRect(569, 783, 250, 44));
                NSString* pt_text = ReadTextFromNSImage(playtime_text_img_crop1, 0.5);
                
                //printf("lobby text 1: %s\n", [text cString]);
                
                NSImage* yellow_claim_btn_img_crop = CropNSImage(img1, NSMakeRect(696, 192, 30, 26));
                ESP_Color avg = AverageColorOfNSImage(yellow_claim_btn_img_crop);
                char yellow_claim_button_visible = (avg.r > 0.99 && avg.r < 1 &&
                                                   avg.g > 0.86 && avg.g < 0.9 &&
                                                   avg.b > 0.03 && avg.b < 0.04 &&
                                                   avg.a == 1 &&
                                                   COLORMAG(avg) > 1.3 && COLORMAG(avg) < 1.34);
                
                if (yellow_claim_button_visible)
                {
                    printf("IMG crop yellow enough, claiming...\n");
                    TDS_LOBBY_PLAYTIME_REWARD_YELLOW_CLAIM_BUTTON_CLICK
                }
                
                if (rbx_textlabel_contains_text(task, reward_tl, "Claim"))
                {
                    if ([pt_text containsString:@"Free"])
                    {
                        printf("Claiming rewards...\n");
                        TDS_LOBBY_PLAYTIME_REWARD_1_CLAIM_CLICK
                        TDS_LOBBY_PLAYTIME_REWARD_2_CLAIM_CLICK
                        TDS_LOBBY_PLAYTIME_REWARD_3_CLAIM_CLICK
                        TDS_LOBBY_PLAYTIME_REWARD_4_CLAIM_CLICK
                        TDS_LOBBY_PLAYTIME_REWARD_5_CLAIM_CLICK
                        TDS_LOBBY_PLAYTIME_REWARD_6_CLAIM_CLICK
                    }
                    else
                    {
                       
                        if (yellow_claim_button_visible)
                        {
                        }
                        else
                        {
                            printf("Opening playtime rewards\n");
                            TDS_LOBBY_PLAYTIME_REWARD_CLAIM_OPEN_CLICK
                        }
                        //0.996079, 0.870588, 0.039215, 1.000000, 1.323493
                        //printf("%f, %f, %f, %f, %f\n", avg.r, avg.g, avg.b, avg.a, COLORMAG(avg));
                    }
                }
                else
                {
                    printf("No rewards left to claim, starting game...\n");
                    if ([pt_text containsString:@"Free"])
                    {
                        printf("Closing playtime rewards\n");
                        TDS_LOBBY_PLAYTIME_REWARD_CLAIM_OPEN_CLICK
                    }
                    
                    if ([text containsString:@"Squad"])
                    {
                        printf("Selecting Squad\n");
                        TDS_LOBBY_SOLO_CLICK
                    }
                    else if ([text containsString:@"Difficulty"])
                    {
                        printf("Selecting Difficulty\n");
                        if (options.mode == RBX_TDS_SURVIVAL_MODE)
                        {
                            if (options.difficulty == RBX_TDS_EASY_DIFFICULTY)
                            {
                                TDS_LOBBY_EASY_CLICK
                            }
                            if (options.difficulty == RBX_TDS_MOLTEN_DIFFICULTY)
                            {
                                TDS_LOBBY_MOLTEN_CLICK
                            }
                        }
                    }
                    else if ([text containsString:@"Gamemode"])
                    {
                        printf("Selecting Gamemode\n");
                        if (options.mode == RBX_TDS_SURVIVAL_MODE)
                        {
                            TDS_LOBBY_SURVIVAL_CLICK
                        }
                        else if (options.mode == RBX_TDS_HARDCORE_MODE)
                        {
                            TDS_LOBBY_HARDCORE_CLICK
                        }
                    }
                    else
                    {
                        TDS_LOBBY_PLAY_GAME_CLICK
                    }
                }
            }
            
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            sleep(1);
            if (rbx_get_placeid(task) == RBX_TDS_LOBBY_PLACE_ID)
            {
                
            }
            else
            {
                printf("No longer in lobby. Exiting...\n");
                exit(0);
            }
        }
    });
        
}

