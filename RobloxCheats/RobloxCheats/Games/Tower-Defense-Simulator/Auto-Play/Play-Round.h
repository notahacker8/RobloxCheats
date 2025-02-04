



void tds_play_round(task_t task, long mode_type, char should_return_to_lobby, char should_exit_when_finished)
{
    pid_t pid = -1;
    printf("- TOWER DEFENSE SIMULATOR (AUTO-PLAY) -\n");
    printf("Make sure your window is as small as possible!!!\n");
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    LIBESP_DYLIB_SETUP
    
    shared_memory->INPUT_QUEUE_COUNT = 3;
    shared_memory->INPUT_USLEEP_TIME = 10000;
    
get_globals: {};
    vm_address_t game = rbx_find_game_address(task);
    // game = 0x2c9450740;
    //rbx_print_descendants(task, game, 0, 0);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
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
    vm_address_t playergui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
    vm_address_t gamegui = rbx_instance_find_first_child(task, playergui, "GameGui");
    vm_address_t roactupgrades_sg = rbx_instance_find_first_child(task, playergui, "RoactUpgrades");
    //rbx_print_descendants(task, rs_state_folder, 0, 3);
    
    vm_address_t hotbar_frame = rbx_instance_find_first_child(task, gamegui, "Hotbar");
    vm_address_t stats_frame = rbx_instance_find_first_child(task, hotbar_frame, "Stats");
    vm_address_t cash_il = rbx_instance_find_first_child(task, stats_frame, "Cash");
    vm_address_t cash_amount_tl = rbx_instance_find_first_child(task, cash_il, "Amount");
    
    vm_address_t health_frame = rbx_instance_find_first_child(task, gamegui, "Health");
    vm_address_t wave_tl = rbx_instance_find_first_child(task, health_frame, "Wave");
    
    vm_address_t roactupgrades_upgrades_frame = rbx_instance_find_first_child(task, roactupgrades_sg, "upgrades");
    vm_address_t roactupgrades_content_frame = rbx_instance_find_first_child(task, roactupgrades_upgrades_frame, "content");
    vm_address_t roactupgrades_top_frame = rbx_instance_find_first_child(task, roactupgrades_content_frame, "top");
    vm_address_t roactupgrades_level_frame = rbx_instance_find_first_child(task, roactupgrades_top_frame, "level");
    vm_address_t roactupgrades_level_content_tl = rbx_instance_find_first_child(task, roactupgrades_level_frame, "content");
    
    //rbx_print_descendants(task, roactupgrades_upgrades_frame, 0, 1);
    if (!roactupgrades_level_content_tl)
    {
        sleep(3);
        goto get_globals;
    }
    //sleep(1000000);
    
    static tds_tower_placement_t* placements = NULL;
    
    long map_name_l = 0;
    char* map_name = rbx_stringvalue_get_value(task, map_stringvalue, &map_name_l);
    if (map_name)
    {
        printf("Map: %s\n", map_name);
        char** map_list = NULL;
        tds_tower_placement_t** placements_list = NULL;
        long list_length = 0;
        
        if (mode_type == RBX_TDS_MODE_TYPE_MOLTEN)
        {
            printf("Mode: Molten\n");
            printf("Make sure you have the following loadout:\n");
            printf("\t1. Farm\n");
            printf("\t2. DJ\n");
            printf("\t3. Commander\n");
            printf("\t4. Normal Soldier\n");
            printf("\t5. Normal Minigunner\n");
            map_list = tds_molten_map_list;
            placements_list = tds_molten_map_placements_list;
            list_length = sizeof(tds_molten_map_placements_list)/sizeof(void*);
        }
        if (mode_type == RBX_TDS_MODE_TYPE_HARDCORE)
        {
            printf("Mode: Hardcore\n");
            printf("Make sure you have the following loadout:\n");
            printf("\t1. Golden Soldier\n");
            map_list = tds_hardcore_map_list;
            placements_list = tds_hardcore_map_placements_list;
            list_length = sizeof(tds_hardcore_map_placements_list)/sizeof(void*);
        }
        if (mode_type == RBX_TDS_MODE_TYPE_HARDCORE_ALT1)
        {
            printf("Mode: Hardcore\n");
            printf("Make sure you have the following loadout:\n");
            printf("\t1. Normal/Golden Pyromancer\n");
            map_list = tds_hardcore_alt1_map_list;
            placements_list = tds_hardcore_alt1_map_placements_list;
            list_length = sizeof(tds_hardcore_alt1_map_placements_list)/sizeof(void*);
        }
        if (placements_list && map_list)
        {
            for (long i = 0 ; i < list_length ; i++)
            {
                if (strcmp(map_list[i], map_name) == 0)
                {
                    placements = placements_list[i];
                    i = list_length;
                }
            }
        }
        if (placements == NULL)
        {
            printf("Error: Couldn't find placement list for map.\n");
        }
        vm_deallocate(mach_task_self_, (vm_address_t)map_name, map_name_l);
    }
    
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    
    
    static long cash = 0;
    static long wave = 0;
    static long time = 0;
    static long health = 0;
    
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
            if (camera && my_hrp)
            {
                rbx_basepart_set_gravity(task, my_hrp, 0);
                //rbx_basepart_set_cframe(task, my_hrp, new_hrp_cframe);
                vm_write(task, my_hrp_cframe_address, (vm_offset_t)&new_hrp_cframe, sizeof(rbx_cframe_t));
                rbx_camera_set_camera_subject(task, camera, 0);
            }
            usleep(50);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            if (should_set_camera_cframe)
            {
                rbx_camera_set_cframe(task, camera, new_camera_cframe);
                usleep(3);
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
            //rbx_print_descendants(task, pickups_model, 0, 1);
            long child_count = 0;
            rbx_child_t* children = rbx_instance_get_children(task, pickups_model, &child_count);
            if (children)
            {
                for (long i = 0 ; i < child_count ; i++)
                {
                    vm_address_t object = children[i].child_address;
                    if (rbx_instance_is_a(task, object, "MeshPart"))
                    {
                        if (rbx_instance_get_parent(task, object))
                        {
                            new_hrp_cframe = rbx_basepart_get_cframe(task, object);
                            usleep(200000);
                        }
                    }
                }
            }
            new_hrp_cframe = really_high_cframe;
            sleep(1);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            //rbx_print_descendants(task, hotbar_frame, 0, 1);
            long wave_str_l = 0;
            char* wave_str = rbx_textlabel_get_text(task, wave_tl, &wave_str_l);
            if (wave_str)
            {
                wave = atol(wave_str + 5); //Wave %ld
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
            
            if (should_return_to_lobby)
            {
                long music_str_l = 0;
                char* music_str = rbx_stringvalue_get_value(task, music_stringvalue, &music_str_l);
                if (music_str)
                {
                    if (strcmp(music_str, "Triumph") == 0)
                    {
                        printf("Returning to lobby...\n");
                        TDS_RETURN_TO_LOBBY_CLICK
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)music_str, music_str_l);
                }
                if (health < 1 && wave > 0)
                {
                    printf("Returning to lobby...\n");
                    TDS_RETURN_TO_LOBBY_CLICK
                }
            }
            if (should_exit_when_finished)
            {
                if (vm_read_uint64_t_value(task, task_base_address + RBX_PLACEID_OFFSET) == RBX_TDS_LOBBY_PLACE_ID)
                {
                    exit(0);
                }
            }
            //printf("wave: %ld\t\tcash: %ld\t\ttime: %ld\n", wave, cash, time);
            usleep(100000);
        }
    });
    
    
    
    
    
#pragma mark - Placing Sequence -
    
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            if (placements == NULL)
            {
                sleep(1);
                continue;
            }
            
            
            
            
#pragma mark - Molten -
            
            
            
            
            
            if (mode_type == RBX_TDS_MODE_TYPE_MOLTEN)
            {
                while (wave >= 0)
                {
                    while (wave == 0 && cash == 0)
                    {
                        printf("SELECTING MOLTEN MODE...\n");
                        TDS_MOLTEN_CLICK
                        sleep(1);
                    }
                    //Wave 0
                    TDS_PLACE_TOWER_SAFE(placements[0]) //Place soldier
                    TDS_WAIT_FOR_TOWER_LOAD(0)
                    TDS_PLACE_TOWER_SAFE(placements[1]) //Place farm
                    TDS_WAIT_FOR_TOWER_LOAD(1)
                    TDS_WAIT_WHILE_COND(wave == 0)
                    //Wave 1
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 1)
                    //Wave 2
                    TDS_PLACE_TOWER_SAFE(placements[2])
                    TDS_WAIT_FOR_TOWER_LOAD(2)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_SELECT_TOWER_SAFE(placements[1])
                    TDS_UPGRADE_UNTIL_LEVEL(1);
                    TDS_WAIT_WHILE_COND(wave == 2)
                    //Wave 3
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_SELECT_TOWER_SAFE(placements[1])
                    TDS_UPGRADE_UNTIL_LEVEL(2);
                    TDS_WAIT_WHILE_COND(wave == 3)
                    //Wave 4
                    TDS_PLACE_TOWER_SAFE(placements[3]) //Place farm
                    TDS_WAIT_FOR_TOWER_LOAD(3)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_UPGRADE_UNTIL_LEVEL(1);
                    TDS_PLACE_TOWER_SAFE(placements[4]) //Place farm
                    TDS_WAIT_FOR_TOWER_LOAD(4)
                    TDS_UPGRADE_UNTIL_LEVEL(1);
                    TDS_WAIT_WHILE_COND(wave == 4)
                    //Wave 5
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_SELECT_TOWER_SAFE(placements[1])
                    TDS_UPGRADE_UNTIL_LEVEL(3);
                    TDS_WAIT_WHILE_COND(wave == 5)
                    //Wave 6
                    TDS_PLACE_TOWER_SAFE(placements[5]) //Place soldier
                    TDS_WAIT_FOR_TOWER_LOAD(5)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_SELECT_TOWER_SAFE(placements[3])
                    TDS_UPGRADE_UNTIL_LEVEL(2);
                    TDS_PLACE_TOWER_SAFE(placements[6])
                    TDS_WAIT_FOR_TOWER_LOAD(6)
                    TDS_UPGRADE_UNTIL_LEVEL(1);
                    TDS_WAIT_WHILE_COND(wave == 6)
                    //Wave 7
                    TDS_SELECT_TOWER_SAFE(placements[3])
                    TDS_UPGRADE_UNTIL_LEVEL(3);
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_SELECT_TOWER_SAFE(placements[4])
                    TDS_UPGRADE_UNTIL_LEVEL(2);
                    TDS_WAIT_WHILE_COND(wave == 7)
                    //Wave 8
                    TDS_SELECT_TOWER_SAFE(placements[4])
                    TDS_UPGRADE_UNTIL_LEVEL(3);
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_SELECT_TOWER_SAFE(placements[6])
                    TDS_UPGRADE_UNTIL_LEVEL(2);
                    TDS_WAIT_WHILE_COND(wave == 8)
                    //Wave 9
                    TDS_SELECT_TOWER_SAFE(placements[6])
                    TDS_UPGRADE_UNTIL_LEVEL(3);
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 9)
                    //Wave 10
                    TDS_PLACE_TOWER_SAFE(placements[7])
                    TDS_WAIT_FOR_TOWER_LOAD(7);
                    TDS_UPGRADE_UNTIL_LEVEL(1)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_PLACE_TOWER_SAFE(placements[8])
                    TDS_WAIT_FOR_TOWER_LOAD(8);
                    TDS_UPGRADE_UNTIL_LEVEL(3)
                    TDS_PLACE_TOWER_SAFE(placements[9])
                    TDS_WAIT_FOR_TOWER_LOAD(9);
                    TDS_UPGRADE_UNTIL_LEVEL(1)
                    TDS_WAIT_WHILE_COND(wave == 10)
                    //Wave 11
                    TDS_SELECT_TOWER_SAFE(placements[9])
                    TDS_UPGRADE_UNTIL_LEVEL(3)
                    TDS_PLACE_TOWER_SAFE(placements[10])
                    TDS_WAIT_FOR_TOWER_LOAD(10);
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_UPGRADE_UNTIL_LEVEL(3)
                    TDS_PLACE_TOWER_SAFE(placements[11])
                    TDS_WAIT_FOR_TOWER_LOAD(11);
                    TDS_UPGRADE_UNTIL_LEVEL(1)
                    TDS_WAIT_WHILE_COND(wave == 11)
                    //Wave 12
                    TDS_SELECT_TOWER_SAFE(placements[7])
                    TDS_UPGRADE_UNTIL_LEVEL(2)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 12)
                    //Wave 13
                    TDS_SELECT_TOWER_SAFE(placements[7])
                    TDS_UPGRADE_UNTIL_LEVEL(3)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 13)
                    //Wave 14
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 14)
                    //Wave 15
                    TDS_SELECT_TOWER_SAFE(placements[7])
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 15)
                    //Wave 16
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 16)
                    //Wave 17
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 17)
                    //Wave 18
                    TDS_PLACE_TOWER_SAFE(placements[12])
                    TDS_UPGRADE_UNTIL_LEVEL(5)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 18)
                    //Wave 19
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 19)
                    //Wave 20
                    TDS_PLACE_TOWER_SAFE(placements[13])
                    TDS_UPGRADE_UNTIL_LEVEL(2)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 20)
                    //Wave 21
                    TDS_SELECT_TOWER_SAFE(placements[13])
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 21)
                    //Wave 22
                    TDS_PLACE_TOWER_SAFE(placements[14]);
                    TDS_UPGRADE_UNTIL_LEVEL(2)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 22)
                    //Wave 23
                    TDS_SELECT_TOWER_SAFE(placements[14]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 23)
                    //Wave 24
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 24)
                    //Wave 25
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 25)
                    //Wave 26
                    TDS_PLACE_TOWER_SAFE(placements[15]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 26)
                    //Wave 27
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 27)
                    //Wave 28
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 28)
                    //Wave 29
                    TDS_PLACE_TOWER_SAFE(placements[16]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 29)
                    //Wave 30
                    TDS_PLACE_TOWER_SAFE(placements[17]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 54)
                    TDS_WAIT_WHILE_COND(wave == 30)
                    //Wave 31
                    TDS_PLACE_TOWER_SAFE(placements[18]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 54)
                    TDS_WAIT_WHILE_COND(wave == 31)
                    //Wave 32
                    TDS_PLACE_TOWER_SAFE(placements[19]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 54)
                    TDS_WAIT_WHILE_COND(wave == 32)
                    //Wave 33
                    TDS_PLACE_TOWER_SAFE(placements[20]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 54)
                    TDS_WAIT_WHILE_COND(wave == 33)
                    //Wave 34
                    TDS_PLACE_TOWER_SAFE(placements[21]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 54)
                    TDS_WAIT_WHILE_COND(wave == 34)
                    //Wave 35
                    TDS_PLACE_TOWER_SAFE(placements[22]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 54)
                    TDS_WAIT_WHILE_COND(wave == 35)
                    //Wave 36
                    TDS_PLACE_TOWER_SAFE(placements[23]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 54)
                    TDS_WAIT_WHILE_COND(wave == 36)
                    //Wave 37
                    TDS_PLACE_TOWER_SAFE(placements[24]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 54)
                    TDS_WAIT_WHILE_COND(wave == 37)
                    //Wave 38
                    TDS_PLACE_TOWER_SAFE(placements[25]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 54)
                    TDS_WAIT_WHILE_COND(wave == 38)
                    //Wave 39
                    TDS_PLACE_TOWER_SAFE(placements[26]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_PLACE_TOWER_SAFE(placements[27]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 54)
                    TDS_WAIT_WHILE_COND(wave == 36)
                    //Wave 40
                    TDS_PLACE_TOWER_SAFE(placements[28]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_PLACE_TOWER_SAFE(placements[29]);
                    TDS_UPGRADE_UNTIL_LEVEL(4)
                    TDS_SELECT_TOWER_SAFE(placements[13]);

                    while (wave == 40 && time < 6000)
                    {
                        TDS_KEYPRESS('f');
                        sleep(1);
                    }
                    TDS_WAIT_WHILE_COND(wave == 40);
                    sleep(1);
                }
            }
            
            
            
            
#pragma mark - Hardcore -
            
            
            
            
            
            if (mode_type == RBX_TDS_MODE_TYPE_HARDCORE)
            {
                while (wave > 0)
                {
                    //Wave 1
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 10)
                    TDS_PLACE_TOWER_SAFE(placements[0])
                    TDS_WAIT_FOR_TOWER_LOAD(0)
                    TDS_UPGRADE_UNTIL_LEVEL(1)
                    TDS_WAIT_WHILE_COND(wave == 1)
                    //Wave 2
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 14)
                    TDS_WAIT_WHILE_COND(wave == 2)
                    //Wave 3
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 14)
                    TDS_WAIT_WHILE_COND(wave == 3)
                    //Wave 4
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 17)
                    TDS_WAIT_WHILE_COND(wave == 4)
                    //Wave 5
                    TDS_PLACE_TOWER_SAFE(placements[1])
                    TDS_WAIT_FOR_TOWER_LOAD(1)
                    TDS_UPGRADE_UNTIL_LEVEL(1)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 5)
                    //Wave 6
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 6)
                    //Wave 7
                    TDS_SELECT_TOWER_SAFE(placements[0])
                    TDS_UPGRADE_UNTIL_LEVEL(2)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 30)
                    TDS_WAIT_WHILE_COND(wave == 7)
                    //Wave 8
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 8)
                    //Wave 9
                    TDS_SELECT_TOWER_SAFE(placements[1])
                    TDS_UPGRADE_UNTIL_LEVEL(2)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 9)
                    //Wave 10
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 10)
                    //Wave 11
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 11)
                    //Wave 12
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 12)
                    //Wave 13
                    TDS_SELECT_TOWER_SAFE(placements[0])
                    TDS_UPGRADE_UNTIL_LEVEL(3)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 13)
                    //Wave 14
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 24)
                    TDS_WAIT_WHILE_COND(wave == 14)
                    //Wave 15
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 27)
                    TDS_WAIT_WHILE_COND(wave == 15)
                    //Wave 16
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 40)
                    TDS_WAIT_WHILE_COND(wave == 16)
                    //Wave 17
                    TDS_WAIT_WHILE_COND(wave == 17)
                    sleep(1);
                }
            }
            
            
            
            
#pragma mark - Hardcore (ALT1) -
            
            
            if (mode_type == RBX_TDS_MODE_TYPE_HARDCORE_ALT1)
            {
                while (wave > 0)
                {
                    //Wave 1
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 10)
                    TDS_WAIT_WHILE_COND(wave == 1)
                    //Wave 2
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 14)
                    TDS_WAIT_WHILE_COND(wave == 2)
                    //Wave 3
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 14)
                    TDS_WAIT_WHILE_COND(wave == 3)
                    //Wave 4
                    TDS_PLACE_TOWER_SAFE(placements[0])
                    TDS_WAIT_FOR_TOWER_LOAD(0)
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 17)
                    TDS_WAIT_WHILE_COND(wave == 4)
                    //Wave 5
                    TDS_SELECT_TOWER_SAFE(placements[0])
                    TDS_KEYPRESS('e')
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 5)
                    //Wave 6
                    TDS_SELECT_TOWER_SAFE(placements[0])
                    TDS_KEYPRESS('e')
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 6)
                    //Wave 7
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 30)
                    TDS_WAIT_WHILE_COND(wave == 7)
                    //Wave 8
                    TDS_SELECT_TOWER_SAFE(placements[0])
                    TDS_KEYPRESS('e')
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 8)
                    //Wave 9
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 9)
                    //Wave 10
                    TDS_SELECT_TOWER_SAFE(placements[0])
                    TDS_KEYPRESS('e')
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 10)
                    //Wave 11
                    TDS_SELECT_TOWER_SAFE(placements[0])
                    TDS_KEYPRESS('e')
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 11)
                    //Wave 12
                    TDS_SELECT_TOWER_SAFE(placements[0])
                    TDS_KEYPRESS('e')
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 12)
                    //Wave 13
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 20)
                    TDS_WAIT_WHILE_COND(wave == 13)
                    //Wave 14
                    TDS_WAIT_WHILE_COND_AND_SKIP(time > 5 || time == 0, 24)
                    TDS_WAIT_WHILE_COND(wave == 14)
                }
            }
            
            usleep(100000);
        }
    });
    
}

