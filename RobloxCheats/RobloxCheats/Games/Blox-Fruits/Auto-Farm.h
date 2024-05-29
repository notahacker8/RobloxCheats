

/*
@TODO: Check "if (vm_read_uint32_value(task, stats_frame + 0x598) != 0x3f000000)" using BitSlicer
 */

vm_address_t blox_fruits_find_first_alive_enemy_by_name(task_t task,
                                                        vm_address_t enemies_folder,
                                                        char* name,
                                                        char is_partial_name)
{
    vm_address_t __enemy = 0;
    long cc = 0;
    rbx_child_t* children = rbx_instance_get_children(task, enemies_folder, &cc);
    if (children)
    {
        for (long i = 0 ; i < cc ; i++)
        {
            vm_address_t enemy = children[i].child_address;
            vm_address_t humanoid = rbx_instance_find_first_child_of_class(task, enemy, "Humanoid");
            if (humanoid)
            {
                long ednl = 0;
                char* edn = rbx_humanoid_get_displayname(task, humanoid, &ednl);
                if (edn)
                {
                    char is__ = false;
                    if (is_partial_name)
                    {
                        if (strstr(edn, name))
                        {
                            is__ = true;
                        }
                    }
                    else
                    {
                        if (strcmp(edn, name) == 0)
                        {
                            is__ = true;
                        }
                    }
                    if (is__)
                    {
                        /*vm_address_t head = rbx_instance_find_first_child(task, enemy, "Head");
                        vm_address_t obj = rbx_instance_find_first_child_of_class(task, head, "ParticleEmitter");
                        if (obj == 0)
                        {
                            __enemy = enemy;
                            i = cc;
                        }*/
                        float health = rbx_humanoid_get_health(task, humanoid);
                        {
                            if (health > 0)
                            {
                                __enemy = enemy;
                                i = cc;
                            }
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)edn, ednl);
                }
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, cc * sizeof(rbx_child_t));
    }
    return __enemy;
}

vm_address_t blox_fruits_find_first_alive_ship(task_t task, vm_address_t enemies_folder)
{
    vm_address_t __enemy = 0;
    long cc = 0;
    rbx_child_t* children = rbx_instance_get_children(task, enemies_folder, &cc);
    if (children)
    {
        for (long i = 0 ; i < cc ; i++)
        {
            vm_address_t enemy = children[i].child_address;
            vm_address_t vs = rbx_instance_find_first_child_of_class(task, enemy, "VehicleSeat");
            if (vs)
            {
                vm_address_t health_iv = rbx_instance_find_first_child(task, enemy, "Health");
                long health = rbx_intvalue_get_value(task, health_iv);
                if (health > 0)
                {
                    __enemy = enemy;
                    i = cc;
                }
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, cc * sizeof(rbx_child_t));
    }
    return __enemy;
}


typedef struct
{
    char* enemy_name;
    char is_partial_name;
    char* quest_giver_name;
    vector3_t quest_giver_proxy_pos;
    vector3_t enemy_area_proxy_pos;
    char use_proxy_positons;
    vector3_t pos_offset;
    float my_lkvec_offset;
    char tool_key;
    char racev4_tool_key;
    char should_do_quest;
    char auto_enable_aura;
    char auto_race_transform;
    char auto_click;
    char auto_v3;
    char auto_tool_equip;
    float max_teleport_range;
    float hitbox_size;
    useconds_t teleport_cooldown;
    float lhk_health;
    char lhk_tool_key;
    char lhk_attack_key;
    useconds_t lhk_attack_duration;
    char lhk_default_enabled;
    char lhk_v4_enabled;
    char chronological_mode_enabled;
    char auto_upgrade;
    char terminate_on_sea_finish;
    char antiafk;
    char target_is_player;
    char v4_ghoul_dash;
    char check_mammoth_transform;
    char check_trex_transform;
}
bfa_info_t;


void blox_fruits_auto_farm(task_t task, bfa_info_t __bfa_info)
{
    printf("\n- BLOX FRUITS (AUTO FARM) -\n");
    printf("\tMinimize window if in chronological mode/doing quest");
    printf("\tCheck \"if (vm_read_uint32_value(task, stats_frame + 0x598) != 0x3f000000)\" using BitSlicer");
    pid_t pid = -1;
    pid_for_task(task, &pid);
    
    LIBESP_DYLIB_SETUP
    
    static bfa_info_t bfa_info;
    bfa_info = __bfa_info;
    float new_grav = 100;
    if (bfa_info.chronological_mode_enabled)
    {
        new_grav = 0;
        rbx_smooth_fly(task, 1);
    }
    else
    {
        rbx_smooth_fly(task, 0);
    }
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    vm_address_t game = rbx_find_game_address(task);
    rbx_print_descendants(task, game, 0, 0);
    long place_id = rbx_get_placeid(task);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    //ANTI_AFK

    vm_address_t data_folder = rbx_instance_find_first_child(task, local_player, "Data");
    vm_address_t level_iv = rbx_instance_find_first_child(task, data_folder, "Level");
    vm_address_t stats_folder = rbx_instance_find_first_child(task, data_folder, "Stats");
    
    vm_address_t demon_fruit_folder = rbx_instance_find_first_child(task, stats_folder, "Demon Fruit");
    vm_address_t defense_folder = rbx_instance_find_first_child(task, stats_folder, "Defense");
    vm_address_t melee_folder = rbx_instance_find_first_child(task, stats_folder, "Melee");
    vm_address_t gun_folder = rbx_instance_find_first_child(task, stats_folder, "Gun");
    vm_address_t sword_folder = rbx_instance_find_first_child(task, stats_folder, "Sword");
    
    vm_address_t demon_fruit_level_iv = rbx_instance_find_first_child(task, demon_fruit_folder, "Level");
    vm_address_t defense_level_iv = rbx_instance_find_first_child(task, defense_folder, "Level");
    vm_address_t melee_level_iv = rbx_instance_find_first_child(task, melee_folder, "Level");
    vm_address_t gun_level_iv = rbx_instance_find_first_child(task, gun_folder, "Level");
    vm_address_t sword_level_iv = rbx_instance_find_first_child(task, sword_folder, "Level");
    
    static long blox_fruit_stat = 0;
    static long defense_stat = 0;
    static long melee_stat = 0;
    static long gun_stat = 0;
    static long sword_stat = 0;
    
    static long level = 0;
    static long stat_points_available = 0;
    
    vm_address_t player_gui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
    vm_address_t main_screengui = rbx_instance_find_first_child(task, player_gui, "Main");
    vm_address_t transformationhud = rbx_instance_find_first_child(task, player_gui, "TransformationHUD");
    vm_address_t transformationhud_il = rbx_instance_find_first_child(task, transformationhud, "ImageLabel");
    vm_address_t quest_frame = rbx_instance_find_first_child(task, main_screengui, "Quest");
    vm_address_t quest_container_frame = rbx_instance_find_first_child(task, quest_frame, "Container");
    vm_address_t quest_title_frame = rbx_instance_find_first_child(task, quest_container_frame, "QuestTitle");
    vm_address_t quest_title_title_tl = rbx_instance_find_first_child(task, quest_title_frame, "Title");
    vm_address_t stats_button = rbx_instance_find_first_child(task, main_screengui, "StatsButton");
    vm_address_t stats_frame = rbx_instance_find_first_child(task, main_screengui, "Stats");
    rbx_print_descendants(task, stats_frame, 0, 0);
    rbx_print_descendants(task, quest_frame, 0, 3);
    rbx_print_descendants(task, transformationhud, 0, 0);
    //rbx_print_descendants(task, transformationhud, 0, 3);
    //vm_address_t badgeservice = rbx_instance_find_first_child_of_class(task, game, "BadgeService");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    //vm_address_t lighting_service = rbx_instance_find_first_child_of_class(task, game, "Lighting");
    //vm_address_t replicatedstorage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t npcs_folder = rbx_instance_find_first_child(task, workspace, "NPCs");
    vm_address_t enemies_folder = rbx_instance_find_first_child(task, workspace, "Enemies");
    vm_address_t boats_folder = rbx_instance_find_first_child(task, workspace, "Boats");
    rbx_print_descendants(task, npcs_folder, 0, 1);
    rbx_print_descendants(task, enemies_folder, 0, 1);
    rbx_print_descendants(task, boats_folder, 0, 1);
    //rbx_print_descendants(task, data_folder, 0, 3);
    
    static vm_address_t my_boat = 0;
    static vm_address_t my_boat_seat = 0;
    static vm_address_t my_boat_seat_cframe_address = 0;
    static vm_address_t my_character = 0;
    static vm_address_t race_transformed_bv = 0;
    static vm_address_t race_energy_nv = 0;
    static vm_address_t my_hum = 0;
    static vm_address_t my_hum_current_seatpart = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    static vm_address_t target_enemy = 0;
    static vm_address_t target_enemy_hrp = 0;
    static vm_address_t target_enemy_hum = 0;
    static vm_address_t quest_giver = 0;
    static vm_address_t quest_giver_part = 0;
    
    quest_giver = rbx_instance_find_first_child(task, npcs_folder, bfa_info.quest_giver_name);
    quest_giver_part = rbx_instance_find_first_child(task, quest_giver, "Head");
    
    static vector3_t dest_pos;
    static vector3_t new_pos;
    
    static char should_write_cframe = false;
    
    shared_memory->INPUT_COUNT = 10;
    shared_memory->INPUT_USLEEP_TIME = 5000;
    
    static char old_race_transformed = false;
    
    static char* required_quest_title_substring = "";
    
    static char can_write_cframe = true;
    
    static const long max_level = 2550;
    static const long first_sea_place_id = 2753915549;
    static const long second_sea_place_id = 4442272183;
    static const long third_sea_place_id = 7449423635;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            can_write_cframe = true;
            sleep(20);
            can_write_cframe = false;
            usleep(500000);
            can_write_cframe = true;
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            /*rbx_print_descendants(task, boats_folder, 0, 1);
            rbx_print_descendants(task, my_boat, 0, 0);
            rbx_print_descendants(task, my_boat_seat, 0, 0);*/
            {
                long child_count = 0;
                rbx_child_t* children = rbx_instance_get_children(task, enemies_folder, &child_count);
                if (children)
                {
                    for (long i = 0 ; i < child_count ; i++)
                    {
                        vm_address_t enemy = children[i].child_address;
                        vm_address_t enemy_hrp = rbx_instance_find_first_child(task, enemy, "HumanoidRootPart");
                        if (rbx_instance_is_valid_child(task, enemy_hrp))
                        {
                            float sz = bfa_info.hitbox_size;
                            rbx_basepart_set_size(task, enemy_hrp, (vector3_t){.x = sz, .y = sz, .z = sz});
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
            }
            {
                vm_address_t __my_boat = 0;
                vm_address_t __my_boat_seat = 0;
                long child_count = 0;
                rbx_child_t* children = rbx_instance_get_children(task, boats_folder, &child_count);
                if (children)
                {
                    for (long i = 0 ; i < child_count ; i++)
                    {
                        vm_address_t boat = children[i].child_address;
                        vm_address_t boat_owner_ov = rbx_instance_find_first_child(task, boat, "Owner");
                        vm_address_t owner = rbx_objectvalue_get_value(task, boat_owner_ov);
                        if (owner == local_player)
                        {
                            __my_boat = boat;
                            __my_boat_seat = vm_read_uint64_value(task, __my_boat + RBX_MODEL_PRIMARYPART_OFFSET);
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
                }
                my_boat_seat = __my_boat_seat;
                my_boat = __my_boat;
            }
            if (bfa_info.should_do_quest)
            {
                quest_giver = rbx_instance_find_first_child(task, npcs_folder, bfa_info.quest_giver_name);
                quest_giver_part = rbx_instance_find_first_child(task, quest_giver, "Head");
            }
            //rbx_print_descendants(task, quest_giver, 0, 1);
            my_character = rbx_player_get_character(task, local_player);
            race_transformed_bv = rbx_instance_find_first_child(task, my_character, "RaceTransformed");
            race_energy_nv = rbx_instance_find_first_child(task, my_character, "RaceEnergy");
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            my_hum = rbx_instance_find_first_child_of_class(task, my_character, "Humanoid");
            my_hum_current_seatpart = rbx_humanoid_get_seatpart(task, my_hum);
            //rbx_print_descendants(task, rbx_instance_get_parent(task, my_hum_seatpart), 0, 1);
            my_hrp_cframe_address = rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            my_boat_seat_cframe_address = rbx_basepart_get_properties_address(task, my_boat_seat) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            if (bfa_info.target_is_player)
            {
                vm_address_t plr = rbx_instance_find_first_child_partial(task, players_service, bfa_info.enemy_name);
                target_enemy = rbx_player_get_character(task, plr);
            }
            else
            {
                target_enemy = blox_fruits_find_first_alive_enemy_by_name(task, enemies_folder, bfa_info.enemy_name, bfa_info.is_partial_name);
            }
            //target_enemy = blox_fruits_find_first_alive_enemy_in_range(task, enemies_folder, my_hrp, 150);
           /* {
           long target_plr = rbx_instance_find_first_child_partial(task, players_service, "block");
            target_enemy = rbx_player_get_character(task, target_plr);
            }*/
            target_enemy_hrp = rbx_instance_find_first_child(task, target_enemy, "HumanoidRootPart");
            target_enemy_hum = rbx_instance_find_first_child(task, target_enemy, "Humanoid");
            //rbx_print_descendants(task, my_character, 0, 1);
            sleep(1);
        }
    });
    
    
#pragma mark - Inputs -
    
    
    /*
     Open Menu: 43.742188, 186.695312
     Open Stats: 44.082031, 225.828125
     Stats Combat: 577.878906, 356.562500
     Stats Defense: 577.871094, 317.261719 (y - 40)
     */
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
#pragma mark stats
            //printf("n\n");
            if (bfa_info.chronological_mode_enabled)
            {
            get_stats: {}
                level = rbx_intvalue_get_value(task, level_iv);
                blox_fruit_stat = rbx_intvalue_get_value(task, demon_fruit_level_iv);
                defense_stat = rbx_intvalue_get_value(task, defense_level_iv);
                melee_stat = rbx_intvalue_get_value(task, melee_level_iv);
                gun_stat = rbx_intvalue_get_value(task, gun_level_iv);
                sword_stat = rbx_intvalue_get_value(task, sword_level_iv);
                stat_points_available = ((level * 3) + 2) - (blox_fruit_stat + defense_stat + melee_stat + gun_stat + sword_stat);
                if (melee_stat >= max_level && defense_stat >= max_level)
                {
                    stat_points_available = 0;
                }
                if (stat_points_available > 0)
                {
                    if (rbx_frame_get_visible(task, stats_button) == 0)
                    {
                        SEND_MOUSEMOVE_YIELDING(0, 45, 185);
                        SEND_LEFTCLICK_YIELDING(1, 10000);
                    }
                    if (vm_read_uint32_value(task, stats_frame + 0x598) != 0x3f000000)
                    {
                        SEND_MOUSEMOVE_YIELDING(0, 45, 225);
                        SEND_LEFTCLICK_YIELDING(1, 10000);
                        sleep(1);
                    }
                    if (melee_stat < max_level)
                    {
                        SEND_MOUSEMOVE_YIELDING(0, 575, 355); //Combat
                    }
                    else
                    {
                        SEND_MOUSEMOVE_YIELDING(0, 575, 315); //Defense
                    }
                    SEND_LEFTCLICK_YIELDING(1, 10000);
                    usleep(30000);
                    goto get_stats;
                }
                if (stat_points_available < 1)
                {
                    if (rbx_frame_get_visible(task, stats_button) == 1)
                    {
                        SEND_MOUSEMOVE_YIELDING(0, 45, 185);
                        SEND_LEFTCLICK_YIELDING(1, 10000);
                    }
                    SEND_MOUSEMOVE_YIELDING(0, 710, 310);
                }
                
                if (bfa_info.terminate_on_sea_finish)
                {
                    char st = false;
                    if (place_id == first_sea_place_id && level >= 700)
                    {
                        st =true;
                    }
                    if (place_id == second_sea_place_id && level >= 1500)
                    {
                        st = true;
                    }
                    if (st)
                    {
                        printf("Sea Finished. Terminating...\n");
                        sleep(5);
                        char cmd[200];
                        bzero(cmd, 200);
                        sprintf(cmd, "kill -9 %d\n", pid);
                        system(cmd);
                    }
                }
                
#pragma mark first sea quests
                
                vector3_t npcs_pos = {.x = 0};
                vector3_t quest_pos = {.x = 0};
                bfa_info.use_proxy_positons = true;
                if (place_id == first_sea_place_id)
                {
                if (level <= 10)
                {
                    quest_pos = (vector3_t){
                        .x = 1081.592285,
                        .y = 16.455151,
                        .z = 1497.886841
                    };
                    npcs_pos = (vector3_t){
                        .x = 1186.082275,
                        .y = 16.614384,
                        .z = 1585.881226
                    };
                    bfa_info.quest_giver_name = "Bandit Quest Giver";
                    bfa_info.enemy_name = "Bandit";
                    required_quest_title_substring = "Bandit";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 30)
                {
                    quest_pos = (vector3_t){
                        .x = -1518.787354,
                        .y = 32.236282,
                        .z = 184.308167
                    };
                    npcs_pos = (vector3_t){
                        .x = -1518.787354,
                        .y = 32.236282,
                        .z = 184.308167
                    };
                    bfa_info.quest_giver_name = "Adventurer";
                    bfa_info.enemy_name = "Monkey";
                    required_quest_title_substring = "Monkey";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 60)
                {
                    quest_pos = (vector3_t){
                        .x = -1232.146118,
                        .y = 29.217583,
                        .z = 3882.886719
                    };
                    npcs_pos = (vector3_t){
                        .x = -1232.146118,
                        .y = 29.217583,
                        .z = 3882.886719
                    };
                    bfa_info.quest_giver_name = "Pirate Adventurer";
                    bfa_info.enemy_name = "Pirate";
                    required_quest_title_substring = "Pirate";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 90)
                {
                    quest_pos = (vector3_t){
                        .x = 901.158813,
                        .y = 35.285278,
                        .z = 4475.660156
                    };
                    npcs_pos = (vector3_t){
                        .x = 901.158813,
                        .y = 35.285278,
                        .z = 4475.660156
                    };
                    bfa_info.quest_giver_name = "Desert Adventurer";
                    bfa_info.enemy_name = "Desert Bandit";
                    required_quest_title_substring = "Desert Bandit";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 120)
                {
                    quest_pos = (vector3_t){
                        .x = 1378.126587,
                        .y = 98.886246,
                        .z = -1356.290894
                    };
                    npcs_pos = (vector3_t){
                        .x = 1378.126587,
                        .y = 98.886246,
                        .z = -1356.290894
                    };
                    bfa_info.quest_giver_name = "Villager";
                    bfa_info.enemy_name = "Snow Bandit";
                    required_quest_title_substring = "Snow Bandit";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 150)
                {
                    quest_pos = (vector3_t){
                        .x = -4974.159668,
                        .y = 51.868279,
                        .z = 4284.174316
                    };
                    npcs_pos = (vector3_t){
                        .x = -4678.793945,
                        .y = 45.871655,
                        .z = 4166.718750
                    };
                    bfa_info.quest_giver_name = "Marine";
                    bfa_info.enemy_name = "Chief Petty Officer";
                    required_quest_title_substring = "Chief Petty";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 190)
                {
                    quest_pos = (vector3_t){
                        .x = -4885.493652,
                        .y = 729.643433,
                        .z = -2628.769775
                    };
                    npcs_pos = (vector3_t){
                        .x = -5042.559082,
                        .y = 338.344269,
                        .z = -2770.786377
                    };
                    bfa_info.quest_giver_name = "Sky Adventurer";
                    bfa_info.enemy_name = "Sky Bandit";
                    required_quest_title_substring = "Sky Bandit";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 250)
                {
                    quest_pos = (vector3_t){
                        .x = 5349.780273,
                        .y = 37.855186,
                        .z = 454.386017
                    };
                    npcs_pos = (vector3_t){
                        .x = 5349.780273,
                        .y = 37.855186,
                        .z = 454.386017
                    };
                    bfa_info.quest_giver_name = "Jail Keeper";
                    bfa_info.enemy_name = "Prisoner [Lv. 190]";
                    required_quest_title_substring = "Prisoner";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 300)
                {
                    quest_pos = (vector3_t){
                        .x = -1583.492188,
                        .y = 9.989564,
                        .z = -2948.500732
                    };
                    npcs_pos = (vector3_t){
                        .x = -1904.062256,
                        .y = 7.470627,
                        .z = -2782.347168
                    };
                    bfa_info.quest_giver_name = "Colosseum Quest Giver";
                    bfa_info.enemy_name = "Toga Warrior";
                    required_quest_title_substring = "Toga";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 450)
                {
                    quest_pos = (vector3_t){
                        .x = -5374.840332,
                        .y = 42.409229,
                        .z = 8516.323242
                    };
                    npcs_pos = (vector3_t){
                        .x = -5500.678711,
                        .y = 30.892994,
                        .z = 8403.943359
                    };
                    bfa_info.quest_giver_name = "The Mayor";
                    bfa_info.enemy_name = "Soldier";
                    required_quest_title_substring = "Soldier";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 525)
                {
                    quest_pos = (vector3_t){
                        .x = -4662.605957,
                        .y = 856.612793,
                        .z = -1876.839844
                    };
                    npcs_pos = (vector3_t){
                        .x = -4662.605957,
                        .y = 856.612793,
                        .z = -1876.839844
                    };
                    bfa_info.quest_giver_name = "Mole";
                    bfa_info.enemy_name = "Guard";
                    required_quest_title_substring = "Guard";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 625)
                {
                    quest_pos = (vector3_t){
                        .x = -7838.250488,
                        .y = 5639.371582,
                        .z = -1451.375977
                    };
                    npcs_pos = (vector3_t){
                        .x = -7606.259766,
                        .y = 5632.208984,
                        .z = -1448.226685
                    };
                    bfa_info.quest_giver_name = "Sky Quest Giver 2";
                    bfa_info.enemy_name = "Squad";
                    required_quest_title_substring = "Squad";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 700)
                {
                    quest_pos = (vector3_t){
                        .x = 5237.404297,
                        .y = 95.443939,
                        .z = 4095.508057
                    };
                    npcs_pos = (vector3_t){
                        .x = 5720.773926,
                        .y = 114.401520,
                        .z = 3929.161621
                    };
                    bfa_info.quest_giver_name = "Freezeburg Quest Giver";
                    bfa_info.enemy_name = "Pirate";
                    required_quest_title_substring = "Pirate";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                }
                
#pragma mark second sea quests
                
                if (place_id == second_sea_place_id)
                {
                if (level <= 775)
                {
                    quest_pos = (vector3_t){
                        .x = -453.167419,
                        .y = 104.297569,
                        .z = 1952.440063
                    };
                    npcs_pos = (vector3_t){
                        .x = -745.459900,
                        .y = 76.519997,
                        .z = 2323.208496
                    };
                    bfa_info.quest_giver_name = "Area 1 Quest Giver";
                    bfa_info.enemy_name = "Raider";
                    required_quest_title_substring = "Raider";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                
                if (level <= 875)
                {
                    quest_pos = (vector3_t){
                        .x = 650.924683,
                        .y = 99.325760,
                        .z = 1017.853271
                    };
                    npcs_pos = (vector3_t){
                        .x = 911.061035,
                        .y = 121.650681,
                        .z = 1208.480713
                    };
                    bfa_info.quest_giver_name = "Area 2 Quest Giver";
                    bfa_info.enemy_name = "Swan Pirate";
                    required_quest_title_substring = "Swan Pirate";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                
                if (level <= 950)
                {
                    quest_pos = (vector3_t){
                        .x = -2553.217041,
                        .y = 88.100739,
                        .z = -3170.038086
                    };
                    npcs_pos = (vector3_t){
                        .x = -2717.939453,
                        .y = 77.002541,
                        .z = -3024.507324
                    };
                    bfa_info.quest_giver_name = "Marine Quest Giver";
                    bfa_info.enemy_name = "Marine Lieutenant";
                    required_quest_title_substring = "Marine Lieutenant";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 1000)
                {
                    quest_pos = (vector3_t){
                        .x = -5546.716797,
                        .y = 72.193848,
                        .z = -849.992188
                    };
                    npcs_pos = (vector3_t){
                        .x = -5546.716797,
                        .y = 72.193848,
                        .z = -849.992188
                    };
                    bfa_info.quest_giver_name = "Graveyard Quest Giver";
                    bfa_info.enemy_name = "Zombie";
                    required_quest_title_substring = "Zombie";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 1100)
                {
                    quest_pos = (vector3_t){
                        .x = 594.854187,
                        .y = 421.238129,
                        .z = -5429.633301
                    };
                    npcs_pos = (vector3_t){
                        .x = 594.854187,
                        .y = 421.238129,
                        .z = -5429.633301
                    };
                    bfa_info.quest_giver_name = "Snow Quest Giver";
                    bfa_info.enemy_name = "Snow Trooper";
                    required_quest_title_substring = "Snow Trooper";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 1175)
                {
                    quest_pos = (vector3_t){
                        .x = -6019.563965,
                        .y = 37.528542,
                        .z = -4904.718262
                    };
                    npcs_pos = (vector3_t){
                        .x = -6417.221191,
                        .y = 44.803143,
                        .z = -5758.039062
                    };
                    bfa_info.quest_giver_name = "Ice Quest Giver";
                    bfa_info.enemy_name = "Lab Sub";
                    required_quest_title_substring = "Lab Sub";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 1350)
                {
                    quest_pos = (vector3_t){
                        .x = -5406.783691,
                        .y = 21.679173,
                        .z = -5236.295410
                    };
                    npcs_pos = (vector3_t){
                        .x = -5467.923340,
                        .y = 80.215126,
                        .z = -5971.798828
                    };
                    bfa_info.quest_giver_name = "Fire Quest Giver";
                    bfa_info.enemy_name = "Magma Ninja";
                    required_quest_title_substring = "Magma Ninja";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 1425)
                {
                    quest_pos = (vector3_t){
                        .x = 5725.965332,
                        .y = 56.985760,
                        .z = -6422.795410
                    };
                    npcs_pos = (vector3_t){
                        .x = 6011.233887,
                        .y = 32.910599,
                        .z = -6239.959473
                    };
                    bfa_info.quest_giver_name = "Frost Quest Giver";
                    bfa_info.enemy_name = "Arctic Warrior";
                    required_quest_title_substring = "Arctic Warrior";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                if (level <= 1500)
                {
                    quest_pos = (vector3_t){
                        .x = -3022.297852,
                        .y = 275.334015,
                        .z = -10222.347656
                    };
                    npcs_pos = (vector3_t){
                        .x = -3113.688232,
                        .y = 29.721907,
                        .z = -9764.374023
                    };
                    bfa_info.quest_giver_name = "Forgotten Quest Giver";
                    bfa_info.enemy_name = "Sea Soldier";
                    required_quest_title_substring = "Sea Soldier";
                    bfa_info.is_partial_name = true;
                    goto __break;
                }
                }
                
#pragma mark third sea quests
                
                if (place_id == third_sea_place_id)
                {
                    if (level <= 1625)
                    {
                        quest_pos = (vector3_t){
                            .x = -278.201233,
                            .y = 63.809563,
                            .z = 5539.109863
                        };
                        npcs_pos = (vector3_t){
                            .x = -278.201233,
                            .y = 63.809563,
                            .z = 5539.109863
                        };
                        bfa_info.quest_giver_name = "Pirate Port Quest Giver";
                        bfa_info.enemy_name = "Million";
                        required_quest_title_substring = "Million";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 1775)
                    {
                        quest_pos = (vector3_t){
                            .x = 5477.424316,
                            .y = 763.923767,
                            .z = 777.149902
                        };
                        npcs_pos = (vector3_t){
                            .x = 4805.555664,
                            .y = 707.876038,
                            .z = 787.426453
                        };
                        bfa_info.quest_giver_name = "Amazon Area 2 Quest Giver";
                        bfa_info.enemy_name = "Female Islander";
                        required_quest_title_substring = "Female Islander";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 1825)
                    {
                        quest_pos = (vector3_t){
                            .x = -10441.927734,
                            .y = 355.970215,
                            .z = -8419.674805
                        };
                        npcs_pos = (vector3_t){
                            .x = -10441.927734,
                            .y = 355.970215,
                            .z = -8419.674805
                        };
                        bfa_info.quest_giver_name = "Turtle Adventure Quest Giver";
                        bfa_info.enemy_name = "Fishman Raider";
                        required_quest_title_substring = "Fishman Raider";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 1900)
                    {
                        quest_pos = (vector3_t){
                            .x = -13303.062500,
                            .y = 380.825684,
                            .z = -7749.374512
                        };
                        npcs_pos = (vector3_t){
                            .x = -13303.062500,
                            .y = 380.825684,
                            .z = -7749.374512
                        };
                        bfa_info.quest_giver_name = "Deep Forest Quest Giver";
                        bfa_info.enemy_name = "Forest Pirate";
                        required_quest_title_substring = "Forest Pirate";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 1975)
                    {
                        quest_pos = (vector3_t){
                            .x = -12818.881836,
                            .y = 445.671417,
                            .z = -9864.243164
                        };
                        npcs_pos = (vector3_t){
                            .x = -12160.884766,
                            .y = 352.808502,
                            .z = -10541.344727
                        };
                        bfa_info.quest_giver_name = "Deep Forest Area 2 Quest Giver";
                        bfa_info.enemy_name = "Jungle Pirate";
                        required_quest_title_substring = "Jungle Pirate";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 2025)
                    {
                        quest_pos = (vector3_t){
                            .x = -9393.199219,
                            .y = 141.508209,
                            .z = 5563.291504
                        };
                        npcs_pos = (vector3_t){
                            .x = -8760.085938,
                            .y = 163.405838,
                            .z = 5992.185547
                        };
                        bfa_info.quest_giver_name = "Haunted Castle Quest Giver 1";
                        bfa_info.enemy_name = "Reborn Skel";
                        required_quest_title_substring = "Reborn Skel";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 2075)
                    {
                        quest_pos = (vector3_t){
                            .x = -9508.838867,
                            .y = 184.844025,
                            .z = 6122.614258
                        };
                        npcs_pos = (vector3_t){
                            .x = -9508.838867,
                            .y = 184.844025,
                            .z = 6122.614258
                        };
                        bfa_info.quest_giver_name = "Haunted Castle Quest Giver 2";
                        bfa_info.enemy_name = "Demonic Soul";
                        required_quest_title_substring = "Demonic Soul";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 2125)
                    {
                        quest_pos = (vector3_t){
                            .x = -2133.213135,
                            .y = 54.831066,
                            .z = -10191.605469
                        };
                        npcs_pos = (vector3_t){
                            .x = -2133.213135,
                            .y = 54.831066,
                            .z = -10191.605469
                        };
                        bfa_info.quest_giver_name = "Peanut Quest Giver";
                        bfa_info.enemy_name = "Scout";
                        required_quest_title_substring = "Scout";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 2200)
                    {
                        quest_pos = (vector3_t){
                            .x = -860.502747,
                            .y = 120.427795,
                            .z = -10933.659180
                        };
                        npcs_pos = (vector3_t){
                            .x = -860.502747,
                            .y = 120.427795,
                            .z = -10933.659180
                        };
                        bfa_info.quest_giver_name = "Ice Cream Quest Giver";
                        bfa_info.enemy_name = "Chef";
                        required_quest_title_substring = "Chef";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 2300)
                    {
                        quest_pos = (vector3_t){
                            .x = -2175.797363,
                            .y = 59.663216,
                            .z = -12021.165039
                        };
                        npcs_pos = (vector3_t){
                            .x = -2175.797363,
                            .y = 59.663216,
                            .z = -12021.165039
                        };
                        bfa_info.quest_giver_name = "Cake Quest Giver 1";
                        bfa_info.enemy_name = "Crafter";
                        required_quest_title_substring = "Crafter";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 2400)
                    {
                        quest_pos = (vector3_t){
                            .x = 118.058289,
                            .y = 62.554642,
                            .z = -12224.746094
                        };
                        npcs_pos = (vector3_t){
                            .x = 118.058289,
                            .y = 62.554642,
                            .z = -12224.746094
                        };
                        bfa_info.quest_giver_name = "Chocolate Quest Giver 1";
                        bfa_info.enemy_name = "Warrior";
                        required_quest_title_substring = "Warrior";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                    if (level <= 2550)
                    {
                        quest_pos = (vector3_t){
                            .x = -1295.760620,
                            .y = 77.121674,
                            .z = -14524.354492
                        };
                        npcs_pos = (vector3_t){
                            .x = -1295.760620,
                            .y = 77.121674,
                            .z = -14524.354492
                        };
                        bfa_info.quest_giver_name = "Candy Cane Quest Giver";
                        bfa_info.enemy_name = "Candy Pirate";
                        required_quest_title_substring = "Candy Pirate";
                        bfa_info.is_partial_name = true;
                        goto __break;
                    }
                }
                
                bfa_info.quest_giver_name = "";
                bfa_info.enemy_name = "";
                required_quest_title_substring = "";
                bfa_info.is_partial_name = false;
                bfa_info.use_proxy_positons = false;
            __break: {}
                //printf("%s\n", bfa_info.quest_giver_name);
                bfa_info.enemy_area_proxy_pos = npcs_pos;
                bfa_info.quest_giver_proxy_pos = quest_pos;
                //printf("%ld\n", stat_points_available);
            }
#pragma mark attacking
            while (!my_character)
            {
                sleep(1);
            }
            rbx_camera_set_camera_subject(task, camera, my_character);
            vm_address_t hasbuso_folder = rbx_instance_find_first_child(task, my_character, "HasBuso");
            //rbx_print_descendants(task, tool, 0, 1);
            double race_energy = rbx_numbervalue_get_value(task, race_energy_nv);
            char race_transformed = rbx_boolvalue_get_value(task, race_transformed_bv);
            if (bfa_info.check_mammoth_transform)
            {
                vm_address_t a = rbx_instance_find_last_child_of_class_and_name(task, my_character, "Accessory", "Mammoth");
                if (!a) { SEND_KEYPRESS_YIELDING(2, 'v', 10000); }
            }
            if (bfa_info.check_trex_transform)
            {
                vm_address_t a = rbx_instance_find_last_child_of_class_and_name(task, my_character, "Accessory", "TRex");
                if (!a) { SEND_KEYPRESS_YIELDING(2, 'v', 10000); }
            }
            if (bfa_info.auto_enable_aura && (!hasbuso_folder))
            {
                SEND_KEYPRESS_YIELDING(2, 'j', 10000);
            }
            char tool_key = bfa_info.tool_key;
            if (bfa_info.auto_race_transform)
            {
                if (truncl(race_energy) == 1)
                {
                    if (bfa_info.check_mammoth_transform)
                    {
                        vm_address_t a = rbx_instance_find_last_child_of_class_and_name(task, my_character, "Accessory", "Mammoth");
                        if (a) { SEND_KEYPRESS_YIELDING(2, 'v', 10000); }
                    }
                    if (bfa_info.check_trex_transform)
                    {
                        vm_address_t a = rbx_instance_find_last_child_of_class_and_name(task, my_character, "Accessory", "TRex");
                        if (a) { SEND_KEYPRESS_YIELDING(2, 'v', 10000); }
                    }
                    SEND_KEYPRESS_YIELDING(2, 'y', 10000);
                    if (bfa_info.auto_tool_equip)
                    {
                        tool_key = bfa_info.racev4_tool_key;
                        SEND_KEYPRESS_YIELDING(2, tool_key, 10000);
                        //transformed = true;
                    }
                }
            }
            
            char do_last_hit_kill_check = false;
            
            if (race_transformed)
            {
                tool_key = bfa_info.racev4_tool_key;
                if (bfa_info.lhk_v4_enabled)
                {
                    do_last_hit_kill_check = true;
                }
            }
            else
            {
                tool_key = bfa_info.tool_key;
                if (bfa_info.lhk_default_enabled)
                {
                    do_last_hit_kill_check = true;
                }
                if ((old_race_transformed == true) && (bfa_info.tool_key != bfa_info.racev4_tool_key))
                {
                    SEND_KEYPRESS_YIELDING(2, tool_key, 10000);
                }
            }
            
            COPY_VAR(target_enemy_hum)
            COPY_VAR(target_enemy)
            
            if (do_last_hit_kill_check)
            {
                if (__target_enemy_hum)
                {
                    float health = rbx_humanoid_get_health(task, __target_enemy_hum);
                    if (bfa_info.lhk_health >= health && health > 0)
                    {
                        rbx_camera_set_camera_subject(task, camera, __target_enemy);
                        SEND_KEYPRESS_YIELDING(2, bfa_info.lhk_tool_key, 10000);
                        if (bfa_info.lhk_attack_key == 0)
                        {
                            SEND_LEFTCLICK_YIELDING(1, bfa_info.lhk_attack_duration);
                        }
                        else
                        {
                            SEND_KEYPRESS_YIELDING(2, bfa_info.lhk_attack_key, bfa_info.lhk_attack_duration);
                            /*if (bfa_info.lhk_attack_key == 'z')
                            {
                                bfa_info.lhk_attack_key = 'x';
                                bfa_info.lhk_attack_duration = 1000000;
                            }
                            else if (bfa_info.lhk_attack_key == 'x')
                            {
                                bfa_info.lhk_attack_key = 'c';
                                bfa_info.lhk_attack_duration = 1000000;
                            }
                            else if (bfa_info.lhk_attack_key == 'c')
                            {
                                bfa_info.lhk_attack_key = 'v';
                                bfa_info.lhk_attack_duration = 1000000;
                            }
                            else if (bfa_info.lhk_attack_key == 'v')
                            {
                                bfa_info.lhk_attack_key = 'z';
                                bfa_info.lhk_attack_duration = 3000000;
                            }*/
                        }
                        SEND_KEYPRESS_YIELDING(2, tool_key, 10000);
                        rbx_camera_set_camera_subject(task, camera, my_character);
                    }
                }
            }
            
            vm_address_t tool = rbx_instance_find_first_child_of_class(task, my_character, "Tool");
            if ((!tool) && bfa_info.auto_tool_equip)
            {
                SEND_KEYPRESS_YIELDING(2, tool_key, 10000);
            }
            if (bfa_info.auto_click == true && (target_enemy_hrp || quest_giver_part))
            {
                SEND_LEFTCLICK_YIELDING(1, 10000)
            }
            if (bfa_info.v4_ghoul_dash && race_transformed)
            {
                SEND_KEYPRESS_YIELDING(2, 'q', 10000);
            }
            if (bfa_info.auto_v3)
            {
                SEND_KEYPRESS_YIELDING(2, 't', 10000);
            }
            old_race_transformed = race_transformed;
            
            usleep(200000);
            //printf("s\n");
        }
    });
    
    
#pragma mark - Teleport -
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            COPY_VAR(my_hrp)
            COPY_VAR(my_hum)
            //printf("%f\n", rbx_humanoid_get_health(task, __my_hum));
            if (rbx_instance_is_valid_child(task, __my_hrp) && (rbx_humanoid_get_health(task, __my_hum) > 0.0))
            {
                rbx_basepart_set_gravity(task, __my_hrp, new_grav);
                rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, __my_hrp);
                float dist_dif = vector3_dist_dif(my_hrp_cframe.pos, dest_pos);
                if (dist_dif > bfa_info.max_teleport_range)
                {
                    vector3_t added = vector3_mul_num(vector3_direction_to_from(dest_pos, my_hrp_cframe.pos), bfa_info.max_teleport_range);
                    new_pos = vector3_add(my_hrp_cframe.pos, added);
                }
                else
                {
                    new_pos = dest_pos;
                }
                if (bfa_info.use_proxy_positons)
                {
                    should_write_cframe = true;
                }
                else
                {
                    if (quest_giver_part || target_enemy_hrp)
                    {
                        should_write_cframe = true;
                    }
                    else
                    {
                        should_write_cframe = false;
                    }
                }
                //should_write_cframe = true;
            }
            else
            {
                should_write_cframe = false;
                sleep(1);
            }
            usleep(bfa_info.teleport_cooldown);
        }
    });
    
    
#pragma mark - Target Find -

    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (my_hrp)
            {
                rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
                vector3_t __dest_pos = my_hrp_cframe.pos;
#pragma mark do quest
                if (bfa_info.should_do_quest)
                {
                    char __cs = false;
                    {
                        long qt_l = 0;
                        char* quest_title_str = rbx_textlabel_get_text(task, quest_title_title_tl, &qt_l);
                        if (quest_title_str)
                        {
                            //printf("%s\n", quest_title_str);
                            if (strstr(quest_title_str, required_quest_title_substring))
                            {
                                __cs = true;
                            }
                            vm_deallocate(mach_task_self_, (vm_address_t)quest_title_str, qt_l);
                        }
                    }
                    //printf("%d\n", __cs);
                    
                    /*
                     Quest was actually taken, start targeting enemies
                     */
                    if (rbx_frame_get_visible(task, quest_frame) == true && __cs)
                    {
                        //rbx_print_descendants(task, target_enemy_hrp, 0, 0);
                        if (rbx_instance_is_valid_child(task, target_enemy_hrp))
                        {
                            __dest_pos = vector3_add(vector3_add(rbx_basepart_get_cframe(task, target_enemy_hrp).pos, bfa_info.pos_offset), vector3_mul_num(rbx_cframe_get_look_vector(my_hrp_cframe), bfa_info.my_lkvec_offset));
                        }
                        else
                        {
                            if (bfa_info.use_proxy_positons)
                            {
                                __dest_pos = bfa_info.enemy_area_proxy_pos;
                            }
                        }
                    }
                    else
                    {
                        /*
                         Go to quest giver
                         */
                        //rbx_print_descendants(task, quest_giver_part, 0, 0);
                        if (quest_giver_part)
                        {
                            __dest_pos = rbx_basepart_get_cframe(task, quest_giver_part).pos;
                        }
                        else
                        {
                            if (bfa_info.use_proxy_positons)
                            {
                                __dest_pos = bfa_info.quest_giver_proxy_pos;
                            }
                        }
                    }
                }
#pragma mark don't do quest
                else
                {
                    /*
                     Target enemies
                     */
                    if (rbx_instance_is_valid_child(task, target_enemy_hrp))
                    {
                        __dest_pos = vector3_add(vector3_add(rbx_basepart_get_cframe(task, target_enemy_hrp).pos, bfa_info.pos_offset), vector3_mul_num(rbx_cframe_get_look_vector(my_hrp_cframe), bfa_info.my_lkvec_offset));
                    }
                    else
                    {
                        if (bfa_info.use_proxy_positons)
                        {
                            __dest_pos = bfa_info.enemy_area_proxy_pos;
                        }
                    }
                }
                dest_pos = __dest_pos;
            }
            usleep(10000);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (should_write_cframe && can_write_cframe)
            {
                vm_write(task, my_hrp_cframe_address + 0x24, (vm_offset_t)&new_pos, 12);
                usleep(8);
            }
            else
            {
                usleep(1000);
            }
        }
    });
    
    
}


#pragma mark - FARMS -


bfa_info_t pirate_raid =
{
    .enemy_name = "Tanky",
    .is_partial_name = true,
    .should_do_quest = false,
    .quest_giver_name = "",
    .pos_offset = {.x = 0, .y = -30, .z = 0},
    .auto_click = true,
    .auto_enable_aura = true,
    .auto_tool_equip = false,
    .auto_race_transform = false,
    .max_teleport_range = 50,
    .hitbox_size = 70,
    .teleport_cooldown = 200000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .use_proxy_positons = false,
    .chronological_mode_enabled = false,
    .antiafk = true,
    .target_is_player = false,
};
bfa_info_t factory_raid =
{
    .enemy_name = "Core",
    .is_partial_name = true,
    .should_do_quest = false,
    .quest_giver_name = "",
    .pos_offset = {.x = 0, .y = -30, .z = 0},
    .auto_click = true,
    .auto_enable_aura = true,
    .auto_tool_equip = false,
    .auto_race_transform = false,
    .max_teleport_range = 50,
    .hitbox_size = 70,
    .teleport_cooldown = 200000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .use_proxy_positons = false,
    .chronological_mode_enabled = false,
    .antiafk = true,
    .target_is_player = false,
};
bfa_info_t conjured_cocoa_farm =
{
    .enemy_name = "Cocoa Warrior",
    .is_partial_name = true,
    .should_do_quest = false,
    .quest_giver_name = "",
    .pos_offset = {.x = 0, .y = 25, .z = 0},
    .terminate_on_sea_finish = false,
    .tool_key = '1',
    .racev4_tool_key = '1',
    .auto_click = true,
    .auto_race_transform = false,
    .auto_enable_aura = true,
    .auto_tool_equip = true,
    .max_teleport_range = 70,
    .hitbox_size = 60,
    .teleport_cooldown = 300000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .use_proxy_positons = true,
    .enemy_area_proxy_pos = {
        .x = -15.069235,
        .y = 73.656662,
        .z = -12235.264648
    },
    .chronological_mode_enabled = false,
    .antiafk = true,
    .target_is_player = false,
};
/**
 For leveling up from scratch.
 */
bfa_info_t chrono_farm =
{
    .enemy_name = "",
    .is_partial_name = true,
    .should_do_quest = true,
    .quest_giver_name = "",
    .pos_offset = {.x = 0, .y = 25, .z = 0},
    .tool_key = '1',
    .racev4_tool_key = '1',
    .auto_race_transform = false,
    .auto_click = true,
    .auto_tool_equip = true,
    .auto_enable_aura = true,
    .max_teleport_range = 70,
    .hitbox_size = 60,
    .teleport_cooldown = 300000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .use_proxy_positons = true,
    .chronological_mode_enabled = true,
    .terminate_on_sea_finish = true,
    .antiafk = false,
    .target_is_player = false,
};

bfa_info_t law_farm =
{
    .enemy_name = "Order",
    .is_partial_name = true,
    .should_do_quest = false,
    .quest_giver_name = "",
    .pos_offset = {.x = 10, .y = 40, .z = 10},
    .terminate_on_sea_finish = false,
    .tool_key = '1',
    .racev4_tool_key = '1',
    .auto_click = true,
    .auto_race_transform = true,
    .auto_enable_aura = true,
    .auto_tool_equip = true,
    .max_teleport_range = 100,
    .hitbox_size = 150,
    .teleport_cooldown = 3000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .use_proxy_positons = false,
    .chronological_mode_enabled = false,
    .antiafk = true,
    .target_is_player = false,
};

bfa_info_t raid_boss =
{
    .enemy_name = "indra",
    .is_partial_name = true,
    .should_do_quest = false,
    .quest_giver_name = "",
    .pos_offset = {.x = 0, .y = -58, .z = 0},
    .terminate_on_sea_finish = false,
    .tool_key = '1',
    .racev4_tool_key = '1',
    .auto_click = true,
    .auto_race_transform = true,
    .auto_enable_aura = true,
    .auto_tool_equip = true,
    .max_teleport_range = 100,
    .hitbox_size = 150,
    .teleport_cooldown = 3000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .use_proxy_positons = false,
    .chronological_mode_enabled = false,
    .antiafk = false,
    .target_is_player = false,
};

bfa_info_t follow_me =
{
    .enemy_name = "blockmincer",
    .is_partial_name = true,
    .should_do_quest = false,
    .quest_giver_name = "",
    .pos_offset = {.x = 0, .y = 0, .z = 0},
    .terminate_on_sea_finish = false,
    .tool_key = '1',
    .racev4_tool_key = '1',
    .auto_click = false,
    .auto_race_transform = false,
    .auto_enable_aura = false,
    .auto_tool_equip = false,
    .max_teleport_range = 70,
    .hitbox_size = 150,
    .teleport_cooldown = 300000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .use_proxy_positons = false,
    .chronological_mode_enabled = false,
    .antiafk = true,
    .target_is_player = true,
};

///Just for testing
bfa_info_t autofarm_info =
{
    .enemy_name = "Pirate",
    .is_partial_name = true,
    .should_do_quest = false,
    .quest_giver_name = "",
    .pos_offset = {.x = 0, .y = 15, .z = 0},
    .my_lkvec_offset = 0,
    .terminate_on_sea_finish = false,
    .tool_key = '1',
    .racev4_tool_key = '1',
    .auto_click = true,
    .auto_race_transform = true,
    .auto_v3 = true,
    .auto_enable_aura = true,
    .auto_tool_equip = true,
    .max_teleport_range = 20,
    .hitbox_size = 100,
    .teleport_cooldown = 100000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .lhk_tool_key = '3',
    .v4_ghoul_dash = true,
    .lhk_attack_key = 0,
    .lhk_attack_duration = 1000000,
    .lhk_health = 10000,
    .use_proxy_positons = false,
    .chronological_mode_enabled = false,
    .antiafk = true,
    .target_is_player = false,
    .check_mammoth_transform = false,
    .check_trex_transform = true,
};
