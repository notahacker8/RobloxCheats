

#define TDS_HARDCORE_MODE_CHEAT 5
#define TDS_MOLTEN_MODE_CHEAT 2


void tower_defense_simulator_cheat(task_t task)
{
    
    static int cheat_mode = TDS_HARDCORE_MODE_CHEAT;
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    
    void* handle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
    vm_offset_t aimbot_enabled_offset = gdso(handle, s_load_address, "AIMBOT_ENABLED");
    vm_offset_t esp_enabled_offset = gdso(handle, s_load_address, "ESP_ENABLED");
    vm_offset_t aimbot_x_offset = gdso(handle, s_load_address, "AIMBOT_X");
    vm_offset_t aimbot_y_offset = gdso(handle, s_load_address, "AIMBOT_Y");
    vm_offset_t window_w_offset = gdso(handle, s_load_address, "WINDOW_W");
    vm_offset_t window_h_offset = gdso(handle, s_load_address, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = gdso(handle, s_load_address, "ESP_USLEEP_TIME");
    vm_offset_t aimbot_usleep_time_offset = gdso(handle, s_load_address, "AIMBOT_USLEEP_TIME");
    vm_offset_t esp_box_hidden_array_offset = gdso(handle, s_load_address, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = gdso(handle, s_load_address, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_color_array_offset = gdso(handle, s_load_address, "ESP_BOX_COLOR_ARRAY");
    vm_offset_t input_queue_offset = gdso(handle, s_load_address, "INPUT_QUEUE");
    vm_offset_t input_queue_count_offset = gdso(handle, s_load_address, "INPUT_QUEUE_COUNT");
    vm_offset_t input_queue_finished_offset = gdso(handle, s_load_address, "INPUT_QUEUE_FINISHED");
    
    dlclose(handle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB__);
    
    static vm_address_t game = 0;
    static vm_address_t rs = 0;
    static vm_address_t workspace = 0;
    static vm_address_t towers_folder = 0;
    static vm_address_t camera = 0;
    static vm_address_t players_service = 0;
    static vm_address_t local_player = 0;
    
    static vm_address_t plr_gui = 0;
    static vm_address_t game_gui = 0;
    static vm_address_t hotbar = 0;
    static vm_address_t hotbar_stats_frame = 0;
    static vm_address_t cash_image_label = 0;
    static vm_address_t cash_amount_text_label = 0;
    static vm_address_t health_frame = 0;
    static vm_address_t wave_text_label = 0;
    
    static vm_address_t local_character = 0;
    static vm_address_t hrp = 0;
    static vm_address_t state_folder = 0;
    static vm_address_t health_folder = 0;
    static vm_address_t current_health = 0;
    static vm_address_t max_health = 0;
    static vm_address_t wave_int_value = 0;
    static vm_address_t voting_folder = 0;
    static vm_address_t timer_folder = 0;
    static vm_address_t timer_enabled = 0;
    static vm_address_t time_int_value = 0;
    static vm_address_t voting_enabled = 0;
    vm_offset_t tds_place_id = rbx_get_place_id(task);
    
    static vector3_t hardcore_wait_pos;
    hardcore_wait_pos.x = -40;
    hardcore_wait_pos.y = 6;
    hardcore_wait_pos.z = 0;
    
    static vector3_t survival_wait_pos_1;
    survival_wait_pos_1.x = 128;
    survival_wait_pos_1.y = 4;
    survival_wait_pos_1.z = 75;
    
    static vector3_t survival_wait_pos_2;
    survival_wait_pos_2.x = 194;
    survival_wait_pos_2.y = 4;
    survival_wait_pos_2.z = 111;
    
    static vector3_t autumn_falling_set_pos;
    autumn_falling_set_pos.x = -31.837585;
    autumn_falling_set_pos.y = 7.612520;
    autumn_falling_set_pos.z = 11;
    
    static Input inputs[100];
    
    static char* new_hrp_location_reached;
    new_hrp_location_reached = malloc(1);
    *new_hrp_location_reached = true;
    
    static char* new_camera_location_reached;
    new_camera_location_reached = malloc(1);
    *new_camera_location_reached = true;
    
    
    init_tds_input_stacks();
    printf("%d\n", hardcore_input_stack_counts[0]);
cheat_start: printf("%s\n", "STARTING SCRIPT...");
    game = rbx_find_game_address(task);
    workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    local_character = rbx_instance_find_first_child(task, workspace, "blockmincer");
    hrp = rbx_instance_find_first_child(task, local_character, "HumanoidRootPart");
    rbx_basepart_set_gravity(task, hrp, 60);
    int hardcore_stack = 0;
    int molten_stack = 0;
    vector3_t wait_pos;
    bzero(&wait_pos, sizeof(vector3_t));
    if (cheat_mode == TDS_HARDCORE_MODE_CHEAT)
    {
        wait_pos = hardcore_wait_pos;
    }
    if (cheat_mode == TDS_MOLTEN_MODE_CHEAT)
    {
        wait_pos = survival_wait_pos_1;
    }
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
    printf("MOVING TO AREA...\n");
    tds_w_jump_move_to_pos(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, camera, hrp, wait_pos, 3);
    sleep(1);
    vm_address_t elevators_folder = rbx_instance_find_first_child(task, workspace, "Elevators");
    long elevator_count = 0;
    vm_address_t* elevators = rbx_instance_get_children(task, elevators_folder, &elevator_count);
    vm_address_t hardcore_elevators[2];
    hardcore_elevators[0] = elevators[2];
    hardcore_elevators[1] = elevators[3];
    vm_address_t normal_elevators[8];
    normal_elevators[0] = elevators[1];
    normal_elevators[1] = elevators[4];
    normal_elevators[2] = elevators[5];
    normal_elevators[3] = elevators[6];
    normal_elevators[4] = elevators[7];
    normal_elevators[5] = elevators[8];
    normal_elevators[6] = elevators[9];
    normal_elevators[7] = elevators[10];
    free(elevators);
    for (;;)
    {
        if (cheat_mode == TDS_HARDCORE_MODE_CHEAT)
        {
            for (int i = 0 ; i < 2 ; i++)
            {
                vm_address_t e = hardcore_elevators[i];
                vm_address_t state = rbx_instance_find_first_child(task, e, "State");
                vm_address_t lift_model = rbx_instance_find_first_child(task, e, "Lift");
                vm_address_t main_part = rbx_instance_find_first_child(task, lift_model, "Main part");
                vm_address_t map_folder = rbx_instance_find_first_child(task, state, "Map");
                vm_address_t title = rbx_instance_find_first_child(task, map_folder, "Title");
                char title_value_length = 0;
                char* title_value = rbx_stringvalue_get_short_value(task, title, &title_value_length);
                if (strcmp(title_value, "Autumn Falling") == 0)
                {
                    printf("MAP LOCATED...\n");
                    rbx_cframe_t cf = rbx_basepart_get_cframe(task, main_part);
                    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
                    tds_w_move_to_pos(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, camera, hrp, cf.pos, 3);
                    hardcore_stack = 0;
                    goto found_elevator;
                }
                vm_deallocate(mach_task_self_, (vm_address_t)title_value, title_value_length);
            }
        }
        if (cheat_mode == TDS_MOLTEN_MODE_CHEAT)
        {
            for (int i = 0 ; i < 8 ; i++)
            {
                vm_address_t e = normal_elevators[i];
                vm_address_t state = rbx_instance_find_first_child(task, e, "State");
                vm_address_t lift_model = rbx_instance_find_first_child(task, e, "Lift");
                vm_address_t main_part = rbx_instance_find_first_child(task, lift_model, "Main part");
                vm_address_t map_folder = rbx_instance_find_first_child(task, state, "Map");
                vm_address_t title = rbx_instance_find_first_child(task, map_folder, "Title");
                char title_value_length = 0;
                char* title_value = rbx_stringvalue_get_short_value(task, title, &title_value_length);
                if (strcmp(title_value, "Autumn Falling") == 0)
                {
                    printf("MAP LOCATED...\n");
                    rbx_cframe_t cf = rbx_basepart_get_cframe(task, main_part);
                    if (i == 0 || i == 1 || i == 4 || i == 5)
                    {
                        wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
                        tds_w_move_to_pos(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, camera, hrp, survival_wait_pos_1, 3);
                    }
                    if (i == 2 || i == 3 || i == 6 || i == 7)
                    {
                        wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
                        tds_w_move_to_pos(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, camera, hrp, survival_wait_pos_2, 3);
                    }
                    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
                    tds_w_move_to_pos(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, camera, hrp, cf.pos, 3);
                    molten_stack = 0;
                    goto found_elevator;
                }
                vm_deallocate(mach_task_self_, (vm_address_t)title_value, title_value_length);
            }
        }
        sleep(1);
    }
found_elevator:sleep(1);
    for (;;)
    {
        register vm_offset_t p_id = rbx_get_place_id(task);
        if (p_id != tds_place_id)
        {
            goto game_started;
        }
        sleep(1);
    }
game_started:sleep(4);
    game = rbx_find_game_address(task);
    rs = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    towers_folder = rbx_instance_find_first_child(task, workspace, "Towers");
    camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    
    long player_count = 0;
    vm_address_t* player_list = rbx_instance_get_children(task, players_service, &player_count);
    local_player = player_list[0];
    free(player_list);
    
    plr_gui = rbx_instance_find_first_child(task, local_player, "PlayerGui");
    game_gui = rbx_instance_find_first_child(task, plr_gui, "GameGui");
    hotbar = rbx_instance_find_first_child(task, game_gui, "Hotbar");
    hotbar_stats_frame = rbx_instance_find_first_child(task, hotbar, "Stats");
    cash_image_label = rbx_instance_find_first_child(task, hotbar_stats_frame, "Cash");
    cash_amount_text_label = rbx_instance_find_first_child(task, cash_image_label, "Amount");
    health_frame = rbx_instance_find_first_child(task, game_gui, "Health");
    wave_text_label = rbx_instance_find_first_child(task, health_frame, "Wave");
    
    local_character = rbx_instance_find_first_child(task, workspace, "blockmincer");
    hrp = rbx_instance_find_first_child(task, local_character, "HumanoidRootPart");
    rbx_basepart_set_gravity(task, hrp, 40);
    state_folder = rbx_instance_find_first_child(task, rs, "State");
    wave_int_value = rbx_instance_find_first_child(task, state_folder, "Wave");
    voting_folder = rbx_instance_find_first_child(task, state_folder, "Voting");
    timer_folder = rbx_instance_find_first_child(task, state_folder, "Timer");
    timer_enabled = rbx_instance_find_first_child(task, timer_folder, "Enabled");
    time_int_value = rbx_instance_find_first_child(task, timer_folder, "Time");
    voting_enabled = rbx_instance_find_first_child(task, voting_folder, "Enabled");
    health_folder = rbx_instance_find_first_child(task, state_folder, "Health");
    current_health = rbx_instance_find_first_child(task, health_folder, "Current");
    max_health = rbx_instance_find_first_child(task, health_folder, "Max");
    printf("GAME STARTED, GLOBALS RESET...\n");
    
    if (cheat_mode == TDS_MOLTEN_MODE_CHEAT)
    {
        printf("VOTING...");
        wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
        tds_vote_molten(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset);
    }
    
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
    tds_w_jump_move_to_pos(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, camera, hrp, autumn_falling_set_pos, 3);
    
    sleep(1);
    
    rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
    camera_cframe.r20 = 0;
    camera_cframe.r21 = 1;
    camera_cframe.r22 = 0;
    rbx_camera_set_cframe(task, camera, &camera_cframe);
    
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 10000);
    
    inputs[0].type = 1;
    inputs[0].window_index = 0;
    inputs[0].duration = 3000000;
    inputs[0].keycode = 34;
    inputs[0].characters[0] = 'i';
    
    inputs[1].type = 2;
    inputs[1].window_index = 0;
    inputs[1].duration = 0;
    inputs[1].keycode = 34;
    inputs[1].characters[0] = 'i';
    
    inputs[2].type = 1;
    inputs[2].window_index = 0;
    inputs[2].duration = 800000;
    inputs[2].keycode = 31;
    inputs[2].characters[0] = 'o';
    
    inputs[3].type = 2;
    inputs[3].window_index = 0;
    inputs[3].duration = 0;
    inputs[3].keycode = 31;
    inputs[3].characters[0] = 'o';
    
    int c = 4;
    vm_write(task, load_address + input_queue_offset, inputs, sizeof(Input) * c);
    char f = 0;
    vm_write(task, load_address + input_queue_count_offset, &c, 4);
    vm_write(task, load_address + input_queue_finished_offset, &f, 1);
    
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 10000);
    
    rbx_tds_input_t* tds_input_stack = NULL;
    int stack_count = 0;
    if (cheat_mode == TDS_HARDCORE_MODE_CHEAT)
    {
        tds_input_stack = hardcore_input_stacks[hardcore_stack];
        stack_count = hardcore_input_stack_counts[hardcore_stack];
    }
    if (cheat_mode == TDS_MOLTEN_MODE_CHEAT)
    {
        tds_input_stack = hardcore_input_stacks[molten_stack];
        stack_count = molten_input_stack_counts[molten_stack];
    }
    
    for (;;)
    {
        if (tds_game_did_end(task, current_health, time_int_value) == true)
        {
            goto game_ended;
        }
        
        if (tds_input_stack_index < stack_count)
        {
            rbx_tds_input_t tds_input = tds_input_stack[tds_input_stack_index];
            tds_input_stack_index++;
            if (tds_input.type == RBX_TDS_INPUT_WAIT)
            {
                printf("WAITING...\n");
                if (tds_input.should_wait_until_skip_enabled == true)
                {
                    tds_wait_until_skip_enabled(task, voting_enabled, time_int_value, current_health);
                }
                else if (tds_input.should_wait_until_game_over == true)
                {
                    tds_wait_until_game_over(task, time_int_value, current_health);
                }
                else if (tds_input.should_wait_until_this_cash)
                {
                    if (tds_input.should_skip_while_waiting == true)
                    {
                        tds_wait_until_cash_while_skipping(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, cash_amount_text_label, time_int_value, current_health, voting_enabled, tds_input.cash);
                    }
                    else
                    {
                        tds_wait_until_cash(task, cash_amount_text_label, time_int_value, current_health, tds_input.cash);
                    }
                }
                else
                {
                    sleep(tds_input.wait_time);
                }
            }
            if (tds_input.type == RBX_TDS_INPUT_SKIP)
            {
                printf("SKIPPING...\n");
                wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 10000);
                tds_skip(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset);
                wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 10000);
            }
            if (tds_input.type == RBX_TDS_INPUT_PLACE)
            {
                printf("%s\n", "PLACING...");
                wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 10000);
                tds_place_tower(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, tds_input.pos, rbx_camera_get_cframe(task, camera), 70, 800, 600, tds_input.tower_number);
                wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 10000);
            }
            if (tds_input.type == RBX_TDS_INPUT_UPGRADE)
            {
                printf("%s\n", "UPGRADING...");
                wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 10000);
                tds_upgrade_tower(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, towers_folder, rbx_camera_get_cframe(task, camera), 70, 800, 600, tds_input.tower_index);
                wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 10000);
            }
        }
        usleep(tds_input_stack_execution_usleep_time);
    }
    
game_ended: printf("GAME OVER, RETURNING TO LOBBY...\n");
    tds_input_stack_execution_usleep_time = 100000;
    tds_input_stack_index = 0;
    sleep(5);
    tds_return_to_lobby(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset);
    for (;;)
    {
        register vm_offset_t p_id = rbx_get_place_id(task);
        if (p_id == tds_place_id)
        {
            sleep(5);
            goto cheat_start;
        }
        sleep(1);
    }
    
    
    
    
    
}
