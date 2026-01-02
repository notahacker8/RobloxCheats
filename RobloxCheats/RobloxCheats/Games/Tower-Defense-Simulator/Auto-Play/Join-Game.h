


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
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        sleep(15);
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
                
                NSImage* text_img_crop1 = CropNSImage(img1, NSMakeRect(662, 784, 651, 128));
                
                NSString* text = ReadTextFromNSImage(text_img_crop1, 0.25);
                
                if ([text containsString:@"Squad"])
                {
                    TDS_LOBBY_SOLO_CLICK
                }
                else if ([text containsString:@"Difficulty"])
                {
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

