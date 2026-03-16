

#pragma mark - FARMS -


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
    .req_quest_substring = "",
    .pos_offset = {.x = 0, .y = 25, .z = 0},
    .my_lkvec_offset = 0,
    .tool_key = '1',
    .racev4_tool_key = '1',
    .auto_race_transform = true,
    .auto_click = true,
    .click_duration = 10000,
    .v4_click_duration = 10000,
    .auto_tool_equip = true,
    .auto_enable_aura = true,
    .max_teleport_range = 40,
    .hitbox_size = 60,
    .teleport_cooldown = 100000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .use_proxy_positons = true,
    .chronological_mode_enabled = true,
    .terminate_on_sea_finish = false,
    .choose_team = true,
    .choose_marine = false,
    .antiafk = true,
    .target_is_player = false,
    .low_prio = true,
    .delete_map_model = false,
    .use_weapon_names = false,
    .weapon_name = "ELECTRIC CLAW",
    .v4_weapon_name = "CURSED DUAL KATANA",
    .lhk_weapon_name = "PORTAL",
    .lhk_health = 3000,
};

bfa_info_t second_sea_money_farm =
{
    .enemy_name = "Swan Pirate",
    .is_partial_name = true,
    .should_do_quest = true,
    .quest_giver_name = "Area 2 Quest Giver",
    .req_quest_substring = "Swan Pirate",
    .pos_offset = {.x = 0, .y = 25, .z = 0},
    .enemy_area_proxy_pos = (vector3_t){
        .x = 911.061035,
        .y = 121.650681,
        .z = 1208.480713
    },
        .quest_giver_proxy_pos = (vector3_t){
            .x = 650.924683,
            .y = 99.325760,
            .z = 1017.853271
        },
        .my_lkvec_offset = 0,
        .tool_key = '1',
        .racev4_tool_key = '1',
        .auto_race_transform = true,
        .auto_click = true,
        .click_duration = 10000,
        .v4_click_duration = 10000,
        .auto_tool_equip = true,
        .auto_enable_aura = true,
        .max_teleport_range = 70,
        .hitbox_size = 60,
        .teleport_cooldown = 100000,
        .lhk_default_enabled = false,
        .lhk_v4_enabled = false,
        .use_proxy_positons = true,
        .chronological_mode_enabled = false,
        .terminate_on_sea_finish = false,
        .antiafk = true,
        .target_is_player = false,
        .low_prio = true,
        .delete_map_model = false,
        .choose_team = true,
};


bfa_info_t minitusk_farm =
{
    .enemy_name = "Mytho",
    .is_partial_name = true,
    .enemy_area_proxy_pos = (vector3_t){ .x = -13620.387695, .y = 470.041534,  .z = -6992.828125},
    .use_proxy_positons = true,
    .should_do_quest = false,
    .quest_giver_name = "",
    .pos_offset = {.x = 0, .y = 25, .z = 0},
    .my_lkvec_offset = 0,
    .terminate_on_sea_finish = false,
    .tool_key = '3',
    .racev4_tool_key = '3',
    .auto_click = true,
    .click_duration = 10000,
    .v4_click_duration = 10000,
    .auto_race_transform = true,
    .auto_v3 = true,
    .auto_enable_aura = true,
    .auto_tool_equip = true,
    .max_teleport_range = 40,
    .hitbox_size = 60,
    .teleport_cooldown = 50000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .lhk_tool_key = '2',
    .v4_ghoul_dash = false,
    .lhk_attack_duration = 100000,
    .lhk_health = 2200,
    .lhk_v4_health = 3500,
    .chronological_mode_enabled = false,
    .antiafk = true,
    .target_is_player = false,
    .check_mammoth_transform = false,
    .check_trex_transform = false,
    .check_leopard_transform = false,
    .low_prio = true,
    .use_weapon_names = true,
    .weapon_name = "DRAGON BREATH",
    .v4_weapon_name = "CURSED DUAL KATANA",
    .set_cam_looking_down = false,
};

///Just for testing
bfa_info_t autofarm_info =
{
    .enemy_name = "Mummy",
    .is_partial_name = true,
    .enemy_area_proxy_pos = (vector3_t){.x = -9505.707031, .y = 6.359823, .z = 6130.452148},
    .use_proxy_positons = true,
    .should_do_quest = false,
    .quest_giver_name = "",
    .pos_offset = {.x = 0, .y = 10, .z = 0},
    .terminate_on_sea_finish = false,
    .tool_key = '3',
    .racev4_tool_key = '3',
    .auto_click = true,
    .click_duration = 1000,
    .v4_click_duration = 1000,
    .auto_race_transform = true,
    .auto_v3 = true,
    .auto_enable_aura = true,
    .auto_tool_equip = true,
    .max_teleport_range = 20,
    .hitbox_size = 20,
    .teleport_cooldown = 50000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = true,
    .lhk_tool_key = '2',
    .v4_ghoul_dash = false,
    .lhk_attack_duration = 100000,
    .lhk_health = 2200,
    .lhk_v4_health = 4500,
    .lhk_attack_keys = "zxf",
    .chronological_mode_enabled = false,
    .antiafk = true,
    .target_is_player = false,
    .check_mammoth_transform = false,
    .check_trex_transform = false,
    .check_leopard_transform = false,
    .low_prio = true,
    .use_weapon_names = true,
    .weapon_name = "CURSED DUAL KATANA",
    .v4_weapon_name = "CURSED DUAL KATANA",
    .lhk_weapon_name = "PORTAL",
    .set_cam_looking_down = true,
};




bfa_info_t draco_mastery =
{
    .enemy_name = "Crew Archer",
    .is_partial_name = true,
    .should_do_quest = false,
    .enemy_area_proxy_pos = (vector3_t){.x = 6759.175293, .y = 484.408508, .z = 403.992188 },
    .use_proxy_positons = true,
    .quest_giver_name = "",
    .pos_offset = {.x = 0, .y = 25, .z = 0},
    .my_lkvec_offset = 0,
    .terminate_on_sea_finish = false,
    .tool_key = '3',
    .racev4_tool_key = '3',
    .auto_click = true,
    .click_duration = 10000,
    .v4_click_duration = 10000,
    .auto_race_transform = true,
    .auto_v3 = true,
    .auto_enable_aura = true,
    .auto_tool_equip = true,
    .max_teleport_range = 60,
    .hitbox_size = 60,
    .teleport_cooldown = 200000,
    .lhk_default_enabled = false,
    .lhk_v4_enabled = false,
    .lhk_tool_key = '4',
    .v4_ghoul_dash = false,
    .lhk_attack_duration = 1000000,
    .lhk_health = 2200,
    .lhk_v4_health = 6000,
    .chronological_mode_enabled = false,
    .antiafk = true,
    .target_is_player = false,
    .check_mammoth_transform = false,
    .check_trex_transform = false,
    .check_leopard_transform = false,
    .low_prio = false,
    .use_weapon_names = true,
    .weapon_name = "ELECTRIC CLAW",
    .v4_weapon_name = "CURSED DUAL KATANA",
};



#pragma mark - Operations -


void blox_fruits_autofarm_operation(int argc, char** argv, int mins_per_acc, bfa_info_t bfa_info)
{
    
    
    if (argc > 1 && (strcmp(argv[1], "autofarm") == 0))
    {
        pid_t pid = -1;
        task_t task = get_roblox_task_port(&pid, 0, true);
        blox_fruits_auto_farm(task, bfa_info);
    }
    else
    {
        printf("options:\n\t0: loopjoin\n\t1: loopautofarm\n\t2: switch cookies loop\n\t3: clean up\n");
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
                    rbx_launch_game(__COOKIE_TXT_PATH__, "2753915549", NULL);
                }
                sleep(1);
            }
        }
        if (strcmp(str, "1\xa") == 0)
        {
            for (;;)
            {
                char cmd[200];
                sprintf(cmd, "%s %s", argv[0], "autofarm");
                system(cmd);
                sleep(1);
            }
        }
        if (strcmp(str, "2\xa") == 0)
        {
            printf("cookie writer\n");
            for(;;)
            {
                sleep(1);
                for (int i = 0 ; i < 13 ; i++)
                {
                    char path[PATH_MAX];
                    bzero(path, PATH_MAX);
                    sprintf(path, "%s%d.txt", "~/Documents/RobloxCookies/",i);
                    long length = 0;
                    char* cookie_data = read_file(path, &length);
                    if (cookie_data)
                    {
                        write_to_file(__COOKIE_TXT_PATH__, cookie_data, length);
                        free(cookie_data);
                    }
                    printf("%s", path);
                    sleep(60 * mins_per_acc);
                    system("killall -9 RobloxPlayer");
                    printf("terminated roblox instances\n");
                }
            }
        }
        if (strcmp(str, "3\xa") == 0)
        {
            printf("cleaning up...\n");
            system("killall -9 RobloxPlayer");
            system("killall -9 RobloxCheats");
        }
        printf("invalid option, exiting...\n");
        exit(0);
    }
}


