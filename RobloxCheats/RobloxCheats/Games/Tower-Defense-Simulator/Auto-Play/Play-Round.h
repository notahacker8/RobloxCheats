

tds_tower_placement_t molten_toyboard_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 20.270269, .y = 16.088120, .z = 7.738850}},    // [0] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.977617, .y = 16.088120, .z = 2.037308}},    // [1] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 15.126860, .y = 16.088120, .z = 5.124984}},    // [2] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 15.139826, .y = 16.088120, .z = 8.214342}},    // [3] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 15.131845, .y = 16.088120, .z = 11.300273}},    // [4] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.439785, .y = 16.088120, .z = 3.717776}},    // [5] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.530451, .y = 16.088120, .z = 6.727867}},    // [6] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.472331, .y = 16.088120, .z = 9.802664}},    // [7] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.472021, .y = 16.088120, .z = 12.837420}},    // [8] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.890431, .y = 16.088120, .z = 2.056655}},    // [9] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.806384, .y = 16.088120, .z = 5.239740}},    // [10] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.864370, .y = 16.088120, .z = 8.305523}},    // [11] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.801282, .y = 16.088120, .z = 11.345956}},    // [12] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.799468, .y = 16.088120, .z = 14.347593}},    // [13] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 15.137880, .y = 16.088120, .z = 14.325818}},    // [14] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 20.210766, .y = 16.088120, .z = 4.654331}},    // [15] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 22.842014, .y = 16.088120, .z = 6.164732}},    // [16] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 20.212135, .y = 16.088120, .z = 1.563836}},    // [17] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 22.860445, .y = 16.088120, .z = 3.000720}},    // [18] Default
};

tds_tower_placement_t hardcore_crossroads_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 10.707596, .y = 2.349998, .z = 11.349027}},    // [0] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 2.836248, .y = 2.350000, .z = -2.589659}},    // [1] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = -2.536528, .y = 2.350008, .z = -2.723598}},    // [2] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 2.797542, .y = 2.349999, .z = 2.564460}},    // [3] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = -2.513418, .y = 2.349999, .z = 2.660728}},    // [4] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 2.586267, .y = 2.350001, .z = -5.625978}},    // [5] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = -2.509926, .y = 2.350008, .z = -5.767704}},    // [6] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = -2.531588, .y = 2.349999, .z = 5.664416}},    // [7] Default
};


void tds_play_round(task_t task, long mode_type, char should_return_to_lobby, char should_exit_when_finished)
{
    pid_t pid = -1;
    printf("- TOWER DEFENSE SIMULATOR (AUTO-PLAY) -\n");
    printf("Make sure your window is as small as possible!!!\n");
    vm_address_t task_base_address = task_get_image_address_by_index(task, 0);
    LIBESP_DYLIB_SETUP
    
    shared_memory->INPUT_COUNT = 3;
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
    //rbx_print_ancestors(task, towers_folder, 0);
    /*FOR_EACH_DESCENDANT(playergui, 100000, 10, {
        long str_l = 0;
        char* str = rbx_textlabel_get_text(task, obj, &str_l);
        if (str){
            //printf("%s : ", str); rbx_print_descendants(task, obj, 0, 0);
            vm_deallocate(mach_task_self_, (vm_address_t)str, str_l);
        }
    });*/
    //rbx_print_descendants(task, towers_folder, 0, 4);
    //int v = rbx_intvalue_get_value(task, rbx_instance_find_first_descendant(task, towers_folder, "Upgrade", "IntValue", 100, 10000));
    //printf("%d\n", v);
    //rbx_print_ancestors(task, 0x175429000, 10);
    //sleep(999999);
    vm_address_t reactoverridesvote_gui = rbx_instance_find_first_child(task, playergui, "ReactOverridesVote");
    vm_address_t reactuniversalhotbar_gui = rbx_instance_find_first_child(task, playergui, "ReactUniversalHotbar");
    vm_address_t reactgametopgamedisplay_gui = rbx_instance_find_first_child(task, playergui, "ReactGameTopGameDisplay");
    vm_address_t reactupgrades_gui = rbx_instance_find_first_child(task, playergui, "ReactUpgrades");
    
    //vm_address_t leveltext_tl = rbx_instance_find_first_descendant(task, reactupgrades_gui, "levelText", "TextLabel", 1000, 100000);
    
    vm_address_t wave_frame = rbx_instance_find_first_descendant(task, reactgametopgamedisplay_gui, "wave", "Frame", 5, 1000);
    vm_address_t wave_value_tl = rbx_instance_find_first_descendant(task, wave_frame, "value", "TextLabel", 5, 1000);
    
    vm_address_t cash_frame = rbx_instance_find_first_descendant(task, reactuniversalhotbar_gui, "cash", "Frame", 100, 10000);
    vm_address_t cash_amount_tl = rbx_instance_find_first_child(task, cash_frame, "amount");
    //rbx_print_descendants(task, cash_frame, 0, 5);
    vm_address_t vote_prompt_tl = rbx_instance_find_first_descendant(task, reactoverridesvote_gui, "prompt", "TextLabel", 10, 100);
    //vm_address_t vote_frame = rbx_instance_get_parent(task, vote_prompt_tl);
    //printf("%s\n", rbx_textlabel_get_text(task, wave_value_tl, malloc(8)));
    
    rbx_print_descendants(task, vote_prompt_tl, 0, 1);
    
    if (!vote_prompt_tl)
    {
        //printf("no up\n");
        sleep(3);
        goto get_globals;
    }
    //sleep(1000000);
    
    static tds_tower_placement_t* placements = NULL;
    
   /* long map_name_l = 0;
    char* map_name = rbx_stringvalue_get_value(task, map_stringvalue, &map_name_l);
    if (map_name)
    {
        printf("Map: %s\n", map_name);
        char** map_list = NULL;
        tds_tower_placement_t** placements_list = NULL;
        long list_length = 0;
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
    }*/
    
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
                usleep(1);
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
    
    
    static int molten_total_placement_count = sizeof(molten_toyboard_placements)/sizeof(tds_tower_placement_t);
    static int hardcore_total_placement_count = sizeof(hardcore_crossroads_placements)/sizeof(tds_tower_placement_t);
    
    static char restarting = false;
    static int tower_to_lvl_index = 0;
    
    static long hardcore_tracked_tower_data[sizeof(hardcore_crossroads_placements)/sizeof(tds_tower_placement_t)];
    for (int i = 0 ; i < sizeof(hardcore_tracked_tower_data) ; i++) {hardcore_tracked_tower_data[i] = -1;}
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
        if (mode_type == RBX_TDS_MODE_TYPE_MOLTEN_TOYBOARD_DEATH_FARM)
        {
            placements = molten_toyboard_placements;
            if (tds_get_can_skip(task, vote_prompt_tl))
            {
                TDS_SKIP_CLICK;
                long str_l = 0;
                char* str = rbx_textlabel_get_text(task, vote_prompt_tl, &str_l);
                if (str)
                {
                    if (strstr(str, "Restart"))
                    {
                        restarting = true;
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)str, str_l);
                }
            }
            else
            {
                if (restarting)
                {
                    tower_to_lvl_index = 0;
                    TDS_WAIT_WHILE_COND(tds_get_tower_count(task, towers_folder) != 0)
                    TDS_WAIT_WHILE_COND(!(wave == 0))
                    restarting = false;
                }
                long current_tower_count = tds_get_tower_count(task, towers_folder);
                //printf("count: %ld\n", current_tower_count);
                if (cash >= 300 && (current_tower_count < molten_total_placement_count))
                {
                    TDS_PLACE_TOWER(placements[current_tower_count]);
                }
                if (cash >= 1175 && (current_tower_count == molten_total_placement_count))
                {
                    TDS_SELECT_TOWER(placements[tower_to_lvl_index]);
                    long lvl = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 2);
                    //printf("[%d] NOW AT LVL %d\n", tower_to_lvl_index, lvl);
                    TDS_DESELECT_TOWER()
                    if (lvl >= 2)
                    {
                        tower_to_lvl_index++;
                    }
                }
            }
        }
        if (mode_type == RBX_TDS_MODE_TYPE_HARDCORE_CROSSROADS_DEATH_FARM)
        {
            placements = hardcore_crossroads_placements;
            if (tds_get_can_skip(task, vote_prompt_tl))
            {
                TDS_SKIP_CLICK;
                long str_l = 0;
                char* str = rbx_textlabel_get_text(task, vote_prompt_tl, &str_l);
                if (str)
                {
                    if (strstr(str, "Restart"))
                    {
                        restarting = true;
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)str, str_l);
                }
            }
            else
            {
                if (restarting)
                {
                    for (int i = 0 ; i < sizeof(hardcore_tracked_tower_data) ; i++) {hardcore_tracked_tower_data[i] = -1;}
                    tower_to_lvl_index = 0;
                    TDS_WAIT_WHILE_COND(tds_get_tower_count(task, towers_folder) != 0)
                    TDS_WAIT_WHILE_COND(!(wave == 0))
                    restarting = false;
                }
                
                long current_tower_count = tds_get_tower_count(task, towers_folder);

                //Place Crook boss
                if (cash >= 900 && (current_tower_count == 0) && hardcore_tracked_tower_data[0] == -1)
                {
                    TDS_PLACE_TOWER(placements[0]);
                    sleep(1);
                    if (tds_get_tower_count(task, towers_folder) == 1)
                    {
                        hardcore_tracked_tower_data[0] = 0;
                        TDS_DESELECT_TOWER()
                    }
                }
                //Place 1st trapper
                if (cash >= 750 && (current_tower_count == 1) && hardcore_tracked_tower_data[1] == -1 && hardcore_tracked_tower_data[0] == 0)
                {
                    TDS_PLACE_TOWER(placements[1]);
                    sleep(1);
                    if (tds_get_tower_count(task, towers_folder) == 2)
                    {
                        hardcore_tracked_tower_data[1] = 0;
                        TDS_DESELECT_TOWER()
                    }
                }
                //Level crook boss to 1
                if (cash >= 450 && (current_tower_count == 2) && hardcore_tracked_tower_data[0] == 0 && hardcore_tracked_tower_data[1] == 0)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[0])
                    hardcore_tracked_tower_data[0] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
                //Level crook boss to 2
                if (cash >= 1350 && (current_tower_count == 2) && hardcore_tracked_tower_data[0] == 1 && hardcore_tracked_tower_data[1] == 0)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[0])
                    hardcore_tracked_tower_data[0] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
                //Upgrade 1st trapper
                if (cash >= 750 && (current_tower_count == 2) && hardcore_tracked_tower_data[1] == 0 && hardcore_tracked_tower_data[0] == 2)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[1])
                    hardcore_tracked_tower_data[1] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
                if (cash >= 2250 && (current_tower_count == 2) && hardcore_tracked_tower_data[1] == 1 && hardcore_tracked_tower_data[0] == 2)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[1])
                    hardcore_tracked_tower_data[1] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
                //Place 2nd trapper
                if (cash >= 750 && (current_tower_count == 2) && (hardcore_tracked_tower_data[2] == -1) && (hardcore_tracked_tower_data[1] == 2))
                {
                    TDS_PLACE_TOWER(placements[2]);
                    sleep(1);
                    if (tds_get_tower_count(task, towers_folder) == 3)
                    {
                        hardcore_tracked_tower_data[2] = 0;
                        TDS_DESELECT_TOWER()
                    }
                }
                if (cash >= 750 && (current_tower_count == 3) && hardcore_tracked_tower_data[2] == 0 && hardcore_tracked_tower_data[1] == 2)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[2])
                    hardcore_tracked_tower_data[2] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
                if (cash >= 2250 && (current_tower_count == 3) && hardcore_tracked_tower_data[2] == 1 && hardcore_tracked_tower_data[1] == 2)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[2])
                    hardcore_tracked_tower_data[2] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
                //Upgrade crook boss lvl 3
                if (cash >= 6375 && (current_tower_count == 3) && hardcore_tracked_tower_data[0] == 2 && hardcore_tracked_tower_data[2] == 2)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[0])
                    hardcore_tracked_tower_data[0] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
                //Place 3rd trapper
                if (cash >= 750 && (current_tower_count == 3) && (hardcore_tracked_tower_data[3] == -1) && (hardcore_tracked_tower_data[0] == 3))
                {
                    TDS_PLACE_TOWER(placements[3]);
                    sleep(1);
                    if (tds_get_tower_count(task, towers_folder) == 4)
                    {
                        hardcore_tracked_tower_data[3] = 0;
                        TDS_DESELECT_TOWER()
                    }
                }
                if (cash >= 750 && (current_tower_count == 4) && hardcore_tracked_tower_data[3] == 0 && hardcore_tracked_tower_data[0] == 3)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[3])
                    hardcore_tracked_tower_data[3] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
                if (cash >= 2250 && (current_tower_count == 4) && hardcore_tracked_tower_data[3] == 1 && hardcore_tracked_tower_data[0] == 3)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[3])
                    hardcore_tracked_tower_data[3] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
                //Place 4th trapper
                if (cash >= 750 && (current_tower_count == 4) && (hardcore_tracked_tower_data[4] == -1) && (hardcore_tracked_tower_data[3] == 2))
                {
                    TDS_PLACE_TOWER(placements[4]);
                    sleep(1);
                    if (tds_get_tower_count(task, towers_folder) == 5)
                    {
                        hardcore_tracked_tower_data[4] = 0;
                        TDS_DESELECT_TOWER()
                    }
                }
                if (cash >= 750 && (current_tower_count == 5) && hardcore_tracked_tower_data[4] == 0 && hardcore_tracked_tower_data[3] == 2)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[4])
                    hardcore_tracked_tower_data[4] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
                if (cash >= 2250 && (current_tower_count == 5) && hardcore_tracked_tower_data[4] == 1 && hardcore_tracked_tower_data[3] == 2)
                {
                    TDS_DESELECT_TOWER()
                    TDS_SELECT_TOWER(placements[4])
                    hardcore_tracked_tower_data[4] = TDS_UPGRADE_TOWER(task, shared_memory, reactupgrades_gui, 1);
                    TDS_DESELECT_TOWER()
                }
            }
        }
        sleep(1);
        }
    });
    

    
}
