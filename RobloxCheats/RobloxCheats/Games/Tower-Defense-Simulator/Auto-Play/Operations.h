



#pragma mark - Operations -



void tds_autofarm_operation(int argc, char** argv, tds_autoplay_options_t tao, tds_main_lobby_options_t tmlo, tds_intermission_lobby_options_t tilo)
{
    
    
    if (argc > 1 && (strcmp(argv[1], "lobby") == 0))
    {
        pid_t pid = -1;
        task_t task = get_roblox_task_port(&pid, 0, true);
        tds_join_game_from_lobby(task, tmlo);
    }
    else if (argc > 1 && (strcmp(argv[1], "interm") == 0))
    {
        pid_t pid = -1;
        task_t task = get_roblox_task_port(&pid, 0, true);
        tds_intermission_lobby(task, tilo);
    }
    else if (argc > 1 && (strcmp(argv[1], "play") == 0))
    {
        pid_t pid = -1;
        task_t task = get_roblox_task_port(&pid, 0, true);
        tds_play_round(task, tao);
    }
    else
    {
        printf("options:\n\t0: loopjoin\n\t1: loopautofarm\n\t2: clean up\n");
        char str[100];
        fgets(str, 100, stdin);
        if (strcmp(str, "0\xa") == 0)
        {
            for (;;)
            {
                pid_t pid = -1;
                task_t task = get_roblox_task_port(&pid, 0, true);
                if (!task)
                {
                    rbx_launch_game(__COOKIE_TXT_PATH__, RBX_TDS_LOBBY_PLACE_ID_STR, NULL);
                }
                sleep(1);
            }
        }
        if (strcmp(str, "1\xa") == 0)
        {
            for (;;)
            {
                char cmd[200];
                sprintf(cmd, "%s %s", argv[0], "play");
                int exit_code = -1;
                int r = system(cmd);
                if (WIFEXITED(r))
                {
                    exit_code = WEXITSTATUS(r);
                };
                if (exit_code == RBX_TDS_EXIT_CODE_ROUND_ENDED_TO_MAIN_LOBBY)
                {
                    sprintf(cmd, "%s %s", argv[0], "lobby");
                    system(cmd);
                }
                else if (exit_code == RBX_TDS_EXIT_CODE_ROUND_ENDED_TO_INTERMISSION)
                {
                    sprintf(cmd, "%s %s", argv[0], "interm");
                    system(cmd);
                }
                sleep(1);
            }
        }
        if (strcmp(str, "2\xa") == 0)
        {
            printf("cleaning up...\n");
            system("killall -9 RobloxPlayer");
            system("killall -9 RobloxCheats");
        }
        printf("invalid option, exiting...\n");
        exit(0);
    }
}



void tds_molten_scout_death_farm_operation(int argc, char** argv, char is_golden_scout)
{
    printf("Make sure your tower slots are:\n");
    printf("[1]: Scout\n");
    tds_autoplay_options_t tao =
    {
        .map_dict = molten_with_scouts_map_dict,
        .map_count = molten_with_scouts_map_count,
        .autofarm_type = RBX_TDS_AUTOFARM_TYPE_MOLTEN_WITH_SCOUTS_DEATH_FARM,
        .auto_set_prio = true,
        .auto_get_map_placements = true,
        .is_golden_scout = is_golden_scout,
        .should_return_to_lobby = false,
    };
    tds_intermission_lobby_options_t tilo =
    {
        .modifiers_list = (char *[]){"hidden", "glass", "limit", "exploding", "committed"},
        .modif_count = 5,
        .auto_set_prio = true,
        .close_game_on_failure = true,
        .map_dict = tao.map_dict,
        .map_count = tao.map_count,
    };
    tds_main_lobby_options_t tmlo =
    {
        .mode = RBX_TDS_SURVIVAL_MODE,
        .difficulty = RBX_TDS_MOLTEN_DIFFICULTY,
        .auto_set_prio = true,
    };
    tds_autofarm_operation(argc, argv, tao, tmlo, tilo);
}


void tds_easy_gold_scout_win_farm_operation(int argc, char** argv)
{
    printf("Make sure your tower slots are:\n");
    printf("[1]: Gold Scout\n");
    tds_autoplay_options_t tao =
    {
        .map_dict = molten_with_scouts_map_dict,
        .map_count = molten_with_scouts_map_count,
        .autofarm_type = RBX_TDS_AUTOFARM_TYPE_MOLTEN_WITH_SCOUTS_DEATH_FARM,
        .auto_set_prio = true,
        .auto_get_map_placements = true,
        .is_golden_scout = true,
        .should_return_to_lobby = true,
    };
    tds_intermission_lobby_options_t tilo =
    {
        .modifiers_list = (char *[]){"hidden", "glass", "limit", "exploding", "committed"},
        .modif_count = 5,
        .auto_set_prio = true,
        .close_game_on_failure = true,
        .map_dict = tao.map_dict,
        .map_count = tao.map_count,
    };
    tds_main_lobby_options_t tmlo =
    {
        .mode = RBX_TDS_SURVIVAL_MODE,
        .difficulty = RBX_TDS_EASY_DIFFICULTY,
        .auto_set_prio = true,
    };
    tds_autofarm_operation(argc, argv, tao, tmlo, tilo);
}


void tds_molten_shotgunner_death_farm_operation(int argc, char** argv)
{
    printf("Make sure your tower slots are:\n");
    printf("[1]: Shotgunner\n");
    printf("[2]: Freezer\n");
    tds_autoplay_options_t tao =
    {
        .map_dict = molten_with_shotgunners_map_dict,
        .map_count = molten_with_shotgunners_map_count,
        .autofarm_type = RBX_TDS_AUTOFARM_TYPE_MOLTEN_WITH_SHOTGUNNERS_DEATH_FARM,
        .auto_set_prio = true,
        .auto_get_map_placements = true,
        .is_golden_scout = false,
        .should_return_to_lobby = false,
    };
    tds_intermission_lobby_options_t tilo =
    {
        .modifiers_list = (char *[]){"hidden", "glass", "limit", "exploding", "committed"},
        .modif_count = 5,
        .auto_set_prio = true,
        .close_game_on_failure = true,
        .map_dict = tao.map_dict,
        .map_count = tao.map_count,
    };
    tds_main_lobby_options_t tmlo =
    {
        .mode = RBX_TDS_SURVIVAL_MODE,
        .difficulty = RBX_TDS_MOLTEN_DIFFICULTY,
        .auto_set_prio = true,
    };
    tds_autofarm_operation(argc, argv, tao, tmlo, tilo);
}


void tds_hardcore_death_farm_operation(int argc, char** argv)
{
    printf("Make sure your tower slots are:\n");
    printf("[1]: Trapper\n");
    printf("[2]: Militant\n");
    tds_autoplay_options_t tao =
    {
        .map_dict = hardcore_map_dict,
        .map_count = hardcore_map_count,
        .autofarm_type = RBX_TDS_AUTOFARM_TYPE_HARDCORE_DEATH_FARM,
        .auto_set_prio = true,
        .auto_get_map_placements = true,
        .is_golden_scout = false,
        .should_return_to_lobby = false,
    };
    tds_intermission_lobby_options_t tilo =
    {
        .modifiers_list = NULL,
        .modif_count = 0,
        .auto_set_prio = true,
        .close_game_on_failure = true,
        .map_dict = tao.map_dict,
        .map_count = tao.map_count,
    };
    tds_main_lobby_options_t tmlo =
    {
        .mode = RBX_TDS_HARDCORE_MODE,
        .auto_set_prio = true,
    };
    tds_autofarm_operation(argc, argv, tao, tmlo, tilo);
}




