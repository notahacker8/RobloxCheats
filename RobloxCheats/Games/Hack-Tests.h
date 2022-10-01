

void hack_tests(task_t task)
{
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
    
    dlclose(handle);
    printf("%p\n", esp_enabled_offset);
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB__);
    
    char esp_enabled = true;
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, 1);
    //char enabled = true;
    //vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&enabled, 1);
     
    static char _name[50];
    printf("%p\n", &_name);
    
    float window_w;
    float window_h;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t x_axis = rbx_instance_find_first_child(task, workspace, "XAxis");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    
    useconds_t esput = 1000000;
    vm_write(task, load_address + aimbot_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    //rbx_print_children_profiles(task, workspace);

    //vm_address_t ref_player = rbx_instance_find_first_child(task, ignore_folder, "RefPlayer");
    for (;;)
    {
        usleep(esput);
        //vm_address_t _char = rbx_instance_find_first_child(task, workspace, _name);
        //vm_address_t x_axis = rbx_instance_find_first_child(task, _char, "Head");
        rbx_cframe_t xaxis_cframe = rbx_basepart_get_cframe(task, x_axis);
        //printf("%f %f %f\n", xaxis_cframe.pos.x, xaxis_cframe.pos.y, xaxis_cframe.pos.z);
        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
        custom_rbx_world_to_screen_info_t wtsi;
        
        vm_read(task, load_address + window_w_offset, 4, &read_data, &data_cnt);
        window_w = *(float*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
        
        vm_read(task, load_address + window_h_offset, 4, &read_data, &data_cnt);
        window_h = *(float*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
        
        float fov = rbx_camera_get_field_of_view(task, camera);
        
        wtsi = rbx_world_to_screen_point(camera_cframe, fov, xaxis_cframe.pos, window_w, window_h);

        char enabled = true;
        
        if (isnan(wtsi.x) || isnan(wtsi.y))
        {
            enabled = false;
            wtsi.x = 0;
            wtsi.y = 0;
            vm_write(task, load_address + aimbot_enabled_offset, (vm_offset_t)&enabled, 1);
        }
        else
        {
            if (wtsi.y > 0 && wtsi.y < window_h)
            {
                if (wtsi.x > 0 && wtsi.y < window_w)
                {
                    vm_write(task, load_address + aimbot_x_offset, (vm_address_t)&wtsi.x, 4);
                    wtsi.y = window_h - wtsi.y;
                    vm_write(task, load_address + aimbot_y_offset, (vm_address_t)&wtsi.y, 4);
                    vm_write(task, load_address + aimbot_enabled_offset, (vm_offset_t)&enabled, 1);
                }
                else
                {
                    enabled = false;
                    vm_write(task, load_address + aimbot_enabled_offset, (vm_offset_t)&enabled, 1);
                }
            } else
            {
                enabled = false;
                vm_write(task, load_address + aimbot_enabled_offset, (vm_offset_t)&enabled, 1);
            }
        }
    }
}


void input_test(task_t task)
{
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
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t x_axis = rbx_instance_find_first_child(task, workspace, "XAxis");
    vm_address_t rs = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t values_folder = rbx_instance_find_first_child(task, workspace, "Values");
    vm_address_t string_value = rbx_instance_find_first_child(task, values_folder, "StringValue");
    vm_address_t bool_value = rbx_instance_find_first_child(task, values_folder, "BoolValue");
    vm_address_t int_value = rbx_instance_find_first_child(task, values_folder, "IntValue");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t teams_service = rbx_instance_find_first_child_of_class(task, game, "Teams");
    vm_address_t my_team = rbx_instance_find_first_child(task, teams_service, "Amongus - red");
    long plr_count = 0;
    vm_address_t* plrs = rbx_instance_get_children(task, players_service, &plr_count);
    vm_address_t my_plr = plrs[0];
    char my_plr_name_len;
    char* my_plr_name = rbx_instance_get_name(task, my_plr, &my_plr_name_len);
    vm_address_t my_character = rbx_instance_find_first_child(task, workspace, my_plr_name);
    vm_address_t hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
    rbx_cframe_t cf = rbx_basepart_get_cframe(task, hrp);
    
    printf("%p\n", load_address + input_queue_finished_offset);
    
    /*Input inputs[2];
    
    inputs[0].type = 3;
    inputs[0].duration = 1000;
    inputs[0].y = 100;
    inputs[0].x = 300;
    
    inputs[1].type = 4;
    inputs[1].duration = 0;
    inputs[1].y = 100;
    inputs[1].x = 300;*/
    /*
    Input inputs[2];
    inputs[0].type = 1;
    inputs[0].duration = 1000;
    inputs[0].keycode = 49;
    inputs[0].characters[0] = ' ';
    inputs[0].window_index = 0;
    
    inputs[1].type = 1;
    inputs[1].duration = 0;
    inputs[1].keycode = 13;
    inputs[1].characters[0] = 'w';
    inputs[1].window_index = 0;

    
    printf("%d\n", vm_write(task, load_address + input_queue_offset, inputs, sizeof(inputs)));
    int c = 2;
    char f = 0;
    printf("%d\n", vm_write(task, load_address + input_queue_count_offset, &c, 4));
    printf("%d\n", vm_write(task, load_address + input_queue_finished_offset, &f, 1));
    
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 100);
    //sleep(5);
    
    inputs[0].type = 2;
    inputs[0].duration = 1000;
    inputs[0].keycode = 49;
    inputs[0].characters[0] = ' ';
    inputs[0].window_index = 0;
    
    inputs[1].type = 2;
    inputs[1].duration = 0;
    inputs[1].keycode = 13;
    inputs[1].characters[0] = 'w';
    inputs[1].window_index = 0;
    
    
    printf("%d\n", vm_write(task, load_address + input_queue_offset, inputs, sizeof(inputs)));
    printf("%d\n", vm_write(task, load_address + input_queue_count_offset, &c, 4));
    printf("%d\n", vm_write(task, load_address + input_queue_finished_offset, &f, 1));*/
    
    
}



void find_object_offsets(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    static kern_return_t kr;
    
    vm_address_t** regions = task_get_regions(task, 3);
    vm_address_t data_start = regions[0][1];
    vm_address_t data_end = regions[1][2];
    vm_address_t* possible_place_ids;
    vm_size_t size = (data_end - data_start);
    vm_read(task, data_start, size, (vm_address_t*)&possible_place_ids, &data_cnt);
    free(regions);
    static int ids_found;
    for (long i = 0 ; i < (size/8) ; i++)
    {
        long _id = possible_place_ids[i];
        if (_id == 5332389196)
        {
            ids_found++;
            if (ids_found == 3)
            {
                printf("#define %s %p\n", "RBX_PLACE_ID_OFFSET", ((i * 8) + data_start) - get_base_address(task));
            }
        }
    }
    vm_deallocate(task, possible_place_ids, size);
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t rs = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t values_folder = rbx_instance_find_first_child(task, workspace, "Values");
    vm_address_t short_string_value = rbx_instance_find_first_child(task, values_folder, "ShortStringValue");
    vm_address_t bool_value = rbx_instance_find_first_child(task, values_folder, "BoolValue");
    vm_address_t int_value = rbx_instance_find_first_child(task, values_folder, "IntValue");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t teams_service = rbx_instance_find_first_child_of_class(task, game, "Teams");
    vm_address_t my_team = rbx_instance_find_first_child(task, teams_service, "Amongus - red");
    long plr_count = 0;
    vm_address_t* plrs = rbx_instance_get_children(task, players_service, &plr_count);
    vm_address_t my_plr = plrs[0];
    vm_address_t plr_gui = rbx_instance_find_first_child(task, my_plr, "PlayerGui");
    vm_address_t screen_gui = rbx_instance_find_first_child(task, plr_gui, "ScreenGui");
    vm_address_t text_label = rbx_instance_find_first_child(task, screen_gui, "TextLabel");
    vm_address_t game_gui = rbx_instance_find_first_child(task, plr_gui, "GameGui");
    vm_address_t hotbar = rbx_instance_find_first_child(task, game_gui, "Hotbar");
    vm_address_t hotbar_stats_frame = rbx_instance_find_first_child(task, hotbar, "Stats");
    vm_address_t cash_image_label = rbx_instance_find_first_child(task, hotbar_stats_frame, "Cash");
    vm_address_t cash_amount_text_label = rbx_instance_find_first_child(task, cash_image_label, "Amount");
    vm_address_t health_frame = rbx_instance_find_first_child(task, game_gui, "Health");
    vm_address_t wave_text_label = rbx_instance_find_first_child(task, health_frame, "Wave");
    char my_plr_name_len;
    char* my_plr_name = rbx_instance_get_name(task, my_plr, &my_plr_name_len);
    vm_address_t my_character = rbx_instance_find_first_child(task, workspace, my_plr_name);
    vm_address_t tool = rbx_instance_find_first_child_of_class(task, my_character, "Tool");
    vm_address_t hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
    vm_address_t humanoid = rbx_instance_find_first_child(task, my_character, "Humanoid");
    rbx_cframe_t cf = rbx_basepart_get_cframe(task, hrp);
    cf.pos.y = 1000;
    //rbx_basepart_set_cframe(task, hrp, &cf);
    vm_deallocate(mach_task_self_, (vm_address_t)my_plr_name, my_plr_name_len);
    free(plrs);
    
    size = 0x300;
    kr = vm_read(task, camera, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8); i++)
    {
        static int _v1[9];
        _v1[0] = 0x3f800000;
        _v1[4] = 0x3f800000;
        _v1[8] = 0x3f800000;
        int a = ((int*)(read_data + i))[0];
        if (a == 0x3f9c61ab)
        {
            printf("#define %s %p\n", "RBX_CAMERA_FOV_OFFSET", i);
        }
        vm_address_t b = ((vm_address_t*)(read_data + i))[0];
        if (b == humanoid)
        {
            printf("#define %s %p\n", "RBX_CAMERA_CAMERA_SUBJECT_OFFSET", i);
        }
        if (memcmp(read_data + i, &_v1, sizeof(_v1)) == 0)
        {
            float _0 = ((int*)(read_data + i - 4))[0];
            float _1 = ((int*)(read_data + i + sizeof(rbx_cframe_t) - 4 + sizeof(rbx_cframe_t)))[0];
            if (_0 == _1)
            {
                printf("#define %s %p // keep the first \n", "RBX_CAMERA_CFRAME_0_OFFSET", i - (sizeof(rbx_cframe_t)));
            }
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x300;
    kr = vm_read(task, my_plr, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8) ; i++)
    {
        vm_address_t a = ((vm_address_t*)(read_data + i))[0];
        if (a == my_team)
        {
            printf("#define %s %p\n", "RBX_PLAYER_TEAM_OFFSET", i);
        }
        if (a == my_character)
        {
            printf("#define %s %p\n", "RBX_PLAYER_CHARACTER_OFFSET", i);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x300;
    kr = vm_read(task, hrp, size, &read_data, &data_cnt);
    for (long i = 0 ; i < size/8 ; i++)
    {
        vm_address_t a = ((vm_address_t*)read_data)[i];
        vm_address_t _rd;
        kr = vm_read(task, a, size, &_rd, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            for (long x = 0 ; x < size/4 ; x++)
            {
                int b = ((int*)_rd)[x];
                if (b == 0x43480000)
                {
                    printf("#define %s %p // keep the first\n", "RBX_PART_PROPERTIES_OFFSET", (i * 8));
                    printf("#define %s %p // keep the first\n", "RBX_PART_PROPERTIES_GRAVITY_OFFSET", (x * 4));
                    printf("#define %s %p // keep the first\n", "RBX_PART_PROPERTIES_CFRAME_OFFSET", (x * 4) + 8);
                }
                
                if (b == 0x40000000)
                {
                    b = ((int*)_rd)[x + 1];
                    if (b == 0x40000000)
                    {
                        b = ((int*)_rd)[x + 2];
                        if (b == 0x3f800000)
                        {
                            printf("#define %s %p  // keep the first\n", "RBX_PART_PROPERTIES_SIZE_OFFSET", (x * 4));
                        }
                    }
                }
                if (b == 194)
                {
                    printf("MSG\n");
                }
            }
        }
        vm_deallocate(mach_task_self_, _rd, size);
    }
    vm_deallocate(mach_task_self_, read_data, size);
    
    size = 0x300;
    kr = vm_read(task, short_string_value, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8); i++)
    {
        if (strcmp("Short string", read_data + i) == 0)
        {
            printf("#define %s %p\n", "RBX_STRING_VALUE_SHORT_VALUE_OFFSET", i - 1);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x300;
    kr = vm_read(task, int_value, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8); i++)
    {
        long a = *((long*)(read_data + i));
        if (a == 42069)
        {
            printf("#define %s %p\n", "RBX_INT_VALUE_VALUE_OFFSET", i);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x1000;
    kr = vm_read(task, text_label, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8); i++)
    {
        if (strcmp("Amogus", read_data + i) == 0)
        {
            printf("#define %s %p // keep the first\n", "RBX_TEXT_LABEL_SHORT_TEXT_OFFSET", i - 1);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    

        /*
        vm_address_t cash = rbx_instance_find_first_child(task, my_plr, "Cash");
        vm_address_t towers_folder = rbx_instance_find_first_child(task, workspace, "Towers");
        vm_address_t state_folder = rbx_instance_find_first_child(task, rs, "State");
        vm_address_t wave = rbx_instance_find_first_child(task, state_folder, "Wave");
        vm_address_t voting_folder = rbx_instance_find_first_child(task, state_folder, "Voting");
        vm_address_t health_folder = rbx_instance_find_first_child(task, state_folder, "Health");
        vm_address_t current_health = rbx_instance_find_first_child(task, health_folder, "Current");
        vm_address_t max_health = rbx_instance_find_first_child(task, health_folder, "Max");
        vm_address_t timer_folder = rbx_instance_find_first_child(task, state_folder, "Timer");
        vm_address_t timer_enabled = rbx_instance_find_first_child(task, timer_folder, "Enabled");
        vm_address_t time = rbx_instance_find_first_child(task, timer_folder, "Time");
        vm_address_t voting_enabled = rbx_instance_find_first_child(task, voting_folder, "Enabled");*/
    //printf("%p\n", wave_text_label);
        //printf("%d\n", tds_get_cash(task, cash_amount_text_label));
       // printf("%d\n", tds_get_wave(task, wave_text_label));
        //rbx_print_children_profiles(task, health_frame);
    /*
    vm_address_t elevators_folder = rbx_instance_find_first_child(task, workspace, "Elevators");
    long elevator_count = 0;
    vm_address_t* elevators = rbx_instance_get_children(task, elevators_folder, &elevator_count);
    for (int i = 0 ; i < elevator_count ; i++)
    {
        vm_address_t ha = elevators[i];
        vm_address_t state = rbx_instance_find_first_child(task, ha, "State");
        vm_address_t map_folder = rbx_instance_find_first_child(task, state, "Map");
        vm_address_t title = rbx_instance_find_first_child(task, map_folder, "Title");
        char title_value_length = 0;
        char* title_value = rbx_stringvalue_get_short_value(task, title, &title_value_length);
        printf("%s\n", title_value);
        vm_deallocate(mach_task_self_, (vm_address_t)title_value, title_value_length);
    }
    //rbx_cframe_t cf; //163.278320 4.140847 95.030930
    for (;;)
    {
    cf = rbx_basepart_get_cframe(task, hrp);
    printf("%f %f %f\n", cf.pos.x, cf.pos.y, cf.pos.z);
        usleep(100000);
    }*/
        /*
         [ImageLabel] Content (0x7f7c3ac9d400) (child count: 4)
         */
        /*
         [ScreenGui] GameGui (0x7f7c7abad600) (child count: 20)
             [UIScale] UIScale (0x60000b232400) (child count: 0)
             [Frame] Menu (0x7f7c7aba0600) (child count: 2)
             [Frame] Health (0x7f7c5b7fc600) (child count: 5)
             [Frame] Keybinds (0x7f7c5b6dd400) (child count: 3)
             [Frame] MobileButtons (0x7f7c7ac13400) (child count: 4)
             [ScreenGui] HoverGui (0x7f7c7af2be00) (child count: 4)
             [Frame] WIP (0x7f7c7f6df200) (child count: 1)
             [Folder] Upgrade (0x60000b21d100) (child count: 2)
             [Frame] Waves (0x7f7c3ac9ca00) (child count: 2)
             [Frame] Voting (0x7f7c3aca6200) (child count: 2)
             [Frame] Background (0x7f7c5b6e9000) (child count: 1)
             [Frame] Backdrop (0x7f7c5b6eac00) (child count: 1)
             [Frame] Dialog (0x7f7c5b6a2400) (child count: 4)
             [Frame] Prompt (0x7f7c5b6a6c00) (child count: 1)
             [Frame] TopBar (0x7f7c3ad37a00) (child count: 4)
             [Frame] Difficulties (0x7f7c5b578800) (child count: 2)
             [Frame] Hotbar (0x7f7c3ad3bc00) (child count: 4)
             [Frame] Results (0x7f7c396f1600) (child count: 1)
             [Frame] Objectives (0x7f7c5b5b5e00) (child count: 2)
             [Frame] Modifiers (0x7f7c7aa19a00) (child count: 2)
         */
        
        //vm_address_t wave = rbx_instance_find_first_child(task, my_plr, "Wave");
        //printf("%ld\n", rbx_intvalue_get_value(task, cash));
        //printf("%p\n", hrp);
        /*
    long tc = 0;
        vm_address_t* t = rbx_instance_get_children(task, towers_folder, &tc);
    for (int i = 0 ; i < tc ; i++)
    {
        cf = rbx_basepart_get_cframe(task, rbx_instance_find_first_child(task, t[i], "HumanoidRootPart"));
        char len = 0;
        char* name = rbx_instance_get_name(task, t[i], &len);
        printf("%s (%p), %f %f %f\n", name, t[i], cf.pos.x, cf.pos.y, cf.pos.z);
        vm_deallocate(mach_task_self_, name, len);
    }*/
        
        /*
        rbx_print_children_profiles(task, 0x7fb340f2a5c0);
        
        rbx_cframe_t cf = rbx_basepart_get_cframe(task,0x7fb340f90500);
        custom_rbx_world_to_screen_info_t wtsi;
        wtsi = rbx_world_to_screen_point(rbx_camera_get_cframe(task, camera), 70, cf.pos, 800, 600);
        printf("%f %f %f\n", cf.pos.x, cf.pos.y, cf.pos.z);
        printf("%f %f\n", wtsi.x, wtsi.y);
        printf("voting enabled%p\n", voting_enabled);
        printf("wave %p\n", wave);
        printf("time %p\n", time);*/

    //0x178 (3280220280) (-265 to -267)
}

/*
TDS NOTES:
 [ScreenGui] GameGui (0x7fb9c93d1800) (child count: 20)
 [ScreenGui] SharedGui (0x7fb9bf2f4400) (child count: 9)
    Cash: local_player["Cash"] (IntValue)
    Map: rs["State"]["Map"] (StringValue)
    Wave: rs["State"]["Wave"] (IntValue)
    VotingEnabled: rs["State"]["Voting"]["Enabled"] (BoolValue)
    TimerEnabled: rs["State"]["Timer"]["Enabled"] (BoolValue)
    Time: rs["State"]["Timer"]["Time"] (IntValue)
 
 [Folder] Timer (0x60000f198700) (child count: 3)
 [Folder] Voting (0x60000f198800) (child count: 2)
 */

/*
 [Workspace] Workspace (0x7f9077e19000) (child count: 52)
     [Camera] Camera (0x7f9081455ea0) (child count: 0)
     [StringValue] Type (0x7f90a2b64cf0) (child count: 0)
     [Folder] Towers (0x60000c077000) (child count: 0)
     [Folder] Trash (0x60000c077100) (child count: 1)
     [Folder] Items (0x60000c077200) (child count: 0)
     [Folder] Replicate (0x60000c077300) (child count: 0)
     [StringValue] Music (0x7f90a2b64e20) (child count: 0)
     [BoolValue] Triumph (0x7f90a2b64f50) (child count: 0)
     [Terrain] Terrain (0x7f907721dfe0) (child count: 0)
     [Part] Plate (0x7f907721f2d0) (child count: 1)
     [Model] Map (0x7f9074a5de40) (child count: 8)
     [Model] blockmincer (0x7f9075ddd700) (child count: 25)
     [Folder] NPCs (0x60001418e500) (child count: 0)
     [Folder] ClientUnits (0x60001418e600) (child count: 0)
     [Folder] PathArrows (0x60001418e700) (child count: 0)
     [Folder] Folder (0x60000c032900) (child count: 8)
     [Model] PathArrow (0x7f9097f2a260) (child count: 1)
     [Model] PathArrow (0x7f9094ebe5d0) (child count: 1)
     [Model] PathArrow (0x7f9074eb92f0) (child count: 1)
     [Model] PathArrow (0x7f90d28fe530) (child count: 1)
     [Model] PathArrow (0x7f90d28fdcd0) (child count: 1)
     [Model] PathArrow (0x7f90a1f9c5a0) (child count: 1)
     [Model] PathArrow (0x7f90a28da750) (child count: 1)
     [Model] PathArrow (0x7f90a28c63d0) (child count: 1)
     [Model] PathArrow (0x7f90a28baba0) (child count: 1)
     [Model] PathArrow (0x7f9076638370) (child count: 1)
     [Model] PathArrow (0x7f9095877640) (child count: 1)
     [Model] PathArrow (0x7f9095f206b0) (child count: 1)
     [Model] PathArrow (0x7f90d28f8100) (child count: 1)
     [Model] PathArrow (0x7f90d28f78a0) (child count: 1)
     [Model] PathArrow (0x7f90d28f7040) (child count: 1)
     [Model] PathArrow (0x7f9095ec9520) (child count: 1)
     [Model] PathArrow (0x7f9095e81de0) (child count: 1)
     [Model] PathArrow (0x7f9097c097e0) (child count: 1)
     [Model] PathArrow (0x7f90a28e23c0) (child count: 1)
     [Model] PathArrow (0x7f90a28e1b60) (child count: 1)
     [Model] PathArrow (0x7f90a28e1300) (child count: 1)
     [Model] PathArrow (0x7f90a2e7c130) (child count: 1)
     [Model] PathArrow (0x7f90773c0fd0) (child count: 1)
     [Model] PathArrow (0x7f90d1fd8130) (child count: 1)
     [Model] PathArrow (0x7f90d1f73e50) (child count: 1)
     [Model] PathArrow (0x7f90d1f62170) (child count: 1)
     [Model] PathArrow (0x7f90d1f61770) (child count: 1)
     [Model] PathArrow (0x7f90a28dca90) (child count: 1)
     [Model] PathArrow (0x7f90a1fc3cc0) (child count: 1)
     [Model] PathArrow (0x7f90a1fc46c0) (child count: 1)
     [Model] PathArrow (0x7f9093007a40) (child count: 1)
     [Model] PathArrow (0x7f90930082a0) (child count: 1)
     [Model] PathArrow (0x7f9093008b00) (child count: 1)
     [Model] PathArrow (0x7f908224fe00) (child count: 1)
     [Model] PathArrow (0x7f90a2e7ab20) (child count: 1)
     [Model] PathArrow (0x7f9094d6d690) (child count: 1)
 [ReplicatedStorage] ReplicatedStorage (0x6000171b3f00) (child count: 11)
     [Folder] State (0x60000f19fe00) (child count: 15)
     [Folder] Resources (0x60000f198a00) (child count: 7)
     [Folder] Client (0x60000f199400) (child count: 3)
     [Folder] Assets (0x60000f19bf00) (child count: 12)
     [Folder] Animators (0x600017014400) (child count: 4)
     [Folder] Upgrades (0x600017014900) (child count: 45)
     [RemoteEvent] RemoteEvent (0x7fb9bfce1170) (child count: 0)
     [RemoteFunction] RemoteFunction (0x7fb9bfce2120) (child count: 0)
     [Folder] StateReplicators (0x600017014a00) (child count: 5)
     [Folder] Modifiers (0x600017014c00) (child count: 0)
     [Folder]  (0x600017015400) (child count: 14)
 */
