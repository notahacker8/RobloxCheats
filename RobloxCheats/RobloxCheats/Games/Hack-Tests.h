
/*

void func(task_t task, vm_address_t instance)
{
    if (rbx_instance_is_a(task, instance, "Part") ||
        rbx_instance_is_a(task, instance, "MeshPart") ||
        rbx_instance_is_a(task, instance, "WedgePart") ||
        rbx_instance_is_a(task, instance, "CornerWedgePart") ||
        rbx_instance_is_a(task, instance, "TrussPart") ||
        rbx_instance_is_a(task, instance, "SpawnLocation") ||
        rbx_instance_is_a(task, instance, "Seat") ||
        rbx_instance_is_a(task, instance, "VehicleSeat"))
    {
        rbx_basepart_set_cancollide(task, instance, false);
    }
}
 
 */

vm_offset_t scan_bytes_offset(task_t task,
                              vm_address_t address,
                              vm_size_t scan_size,
                              char* known_bytes,
                              vm_size_t known_bytes_length,
                              int alignment)
{
    vm_offset_t offset = 0;
    char* read_data = NULL;
    mach_msg_type_number_t data_cnt;
    kern_return_t kr = vm_read(task, address, scan_size, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (scan_size - known_bytes_length) ; i++)
        {
            if (i % alignment == 0)
            {
                if (memcmp(read_data + i, known_bytes, known_bytes_length) == 0)
                {
                    offset = i;
                    i = (scan_size - known_bytes_length);
                }
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, scan_size);
    }
    return offset;
}

void find_object_offsets(task_t task, char* display_name)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    static kern_return_t kr;
    
    vm_size_t size = 0x1000;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    
    vm_address_t starterpack = rbx_instance_find_first_child_of_class(task, game, "StarterPack");
    vm_address_t starterpack_classicsword = rbx_instance_find_first_child(task, starterpack, "ClassicSword");
    vm_address_t starterpack_classicslinghsot = rbx_instance_find_first_child(task, starterpack, "ClassicSlingshot");
    vm_address_t starterpack_rocketlauncher = rbx_instance_find_first_child(task, starterpack, "RocketLauncher");
    
    vm_address_t lighting = rbx_instance_find_first_child_of_class(task, game, "Lighting");
    vm_address_t spawnlocation = rbx_instance_find_first_child_of_class(task, workspace, "SpawnLocation");
    vm_address_t noob = rbx_instance_find_first_child(task, workspace, "Noob");
    vm_address_t noob_humanoid = rbx_instance_find_first_child(task, noob, "Humanoid");
    vm_address_t sitting_noob = rbx_instance_find_first_child(task, workspace, "Sitting Noob");
    vm_address_t sitting_noob_humanoid = rbx_instance_find_first_child(task, sitting_noob, "Humanoid");
    vm_address_t xaxis = rbx_instance_find_first_child(task, workspace, "XAxis");
    vm_address_t zaxis = rbx_instance_find_first_child(task, workspace, "ZAxis");
    vm_address_t meshpart1 = rbx_instance_find_first_child(task, workspace, "MeshPart1");
    vm_address_t meshpart2 = rbx_instance_find_first_child(task, workspace, "MeshPart2");
    
    vm_address_t values_folder = rbx_instance_find_first_child(task, workspace, "Values");
    vm_address_t short_string_value = rbx_instance_find_first_child(task, values_folder, "ShortStringValue");
    vm_address_t bool_value = rbx_instance_find_first_child(task, values_folder, "BoolValue");
    vm_address_t int_value = rbx_instance_find_first_child(task, values_folder, "IntValue");
    vm_address_t cframe_value = rbx_instance_find_first_child(task, values_folder, "CFrameValue");
    
    vm_address_t other_values_folder = rbx_instance_find_first_child(task, values_folder, "OtherValues");
    vm_address_t playeraccountage_value = rbx_instance_find_first_child(task, other_values_folder, "PlayerAccountAge");
    vm_address_t noobmovedirectionx_value = rbx_instance_find_first_child(task, other_values_folder, "NoobMoveDirectionX");
    vm_address_t noobmovedirectiony_value = rbx_instance_find_first_child(task, other_values_folder, "NoobMoveDirectionY");
    vm_address_t noobmovedirectionz_value = rbx_instance_find_first_child(task, other_values_folder, "NoobMoveDirectionZ");
    
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t teams_service = rbx_instance_find_first_child_of_class(task, game, "Teams");
    vm_address_t my_team = rbx_instance_find_first_child(task, teams_service, "Amongus - red");
    
    long plr_count = 0;
    rbx_child_t* plrs = rbx_instance_get_children(task, players_service, &plr_count);
    vm_address_t my_plr = plrs[0].child_address;
    
    vm_address_t plr_gui = rbx_instance_find_first_child(task, my_plr, "PlayerGui");
    vm_address_t screen_gui = rbx_instance_find_first_child(task, plr_gui, "ScreenGui");
    vm_address_t text_label = rbx_instance_find_first_child(task, screen_gui, "TextLabel");
    unsigned char my_plr_name_len;
    char* my_plr_name = rbx_instance_get_name(task, my_plr, &my_plr_name_len);
    vm_address_t my_character = rbx_instance_find_first_child(task, workspace, my_plr_name);
    vm_address_t backpack = rbx_instance_find_first_child(task, my_plr, "Backpack");
    vm_address_t tool = rbx_instance_find_first_child(task, backpack, "ClassicSword");
    vm_address_t handle = rbx_instance_find_first_child(task, tool, "Handle");
    vm_address_t hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
    vm_address_t head = rbx_instance_find_first_child(task, my_character, "Head");
    vm_address_t humanoid = rbx_instance_find_first_child(task, my_character, "Humanoid");
    
    /*for (;;)
    {
        rbx_basepart_set_rotvelocity(task, hrp, (vector3_t){.x = 1000, .y = 0, .z = 0});
    }*/
    
    
    rbx_print_descendants(task, my_plr, 0, 5);
    
    static char known_bytes[4096];
    
    ((long*)known_bytes)[0] = 42069;
    vm_offset_t intvalue_value_offset = scan_bytes_offset(task, int_value, 0x1000, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_INTVALUE_VALUE_OFFSET", (void*)intvalue_value_offset);
    bzero(known_bytes, sizeof(known_bytes));
    
    ((long*)known_bytes)[0] = head;
    vm_offset_t model_primarypart_offset = scan_bytes_offset(task, my_character, 0x1000, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_MODEL_PRIMARYPART_OFFSET", (void*)model_primarypart_offset);
    bzero(known_bytes, sizeof(known_bytes));
    
    ///(Check "/Objects/brickcolors.h")
    ((int*)known_bytes)[0] = 1004; //Really red
    vm_offset_t team_brickcolor_offset = scan_bytes_offset(task, my_team, 0x1000, known_bytes, 4, 4);
    printf("int %s = %p;\n", "RBX_TEAM_BRICKCOLOR_OFFSET", (void*)team_brickcolor_offset);
    bzero(known_bytes, sizeof(known_bytes));
    
    long account_age = vm_read_8byte_value(task, playeraccountage_value + intvalue_value_offset);
    
    ((int*)known_bytes)[0] = (int)account_age;
    vm_offset_t player_accountage_offset = scan_bytes_offset(task, my_plr, 0x1000, known_bytes, 4, 4);
    printf("int %s = %p;\n", "RBX_PLAYER_ACCOUNTAGE_OFFSET", (void*)player_accountage_offset);
    bzero(known_bytes, sizeof(known_bytes));
    
    ((long*)known_bytes)[0] = my_team;
    vm_offset_t player_team_offset = scan_bytes_offset(task, my_plr, 0x1000, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_PLAYER_TEAM_OFFSET", (void*)player_team_offset);
    bzero(known_bytes, sizeof(known_bytes));
    
    ((long*)known_bytes)[0] = my_character;
    vm_offset_t player_character_offset = scan_bytes_offset(task, my_plr, 0x1000, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_PLAYER_CHARACTER_OFFSET", (void*)player_character_offset);
    bzero(known_bytes, sizeof(known_bytes));
    
    ((long*)known_bytes)[0] = spawnlocation;
    vm_offset_t player_respawnlocation_offset = scan_bytes_offset(task, my_plr, 0x1000, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_PLAYER_RESPAWNLOCATION_OFFSET", (void*)player_respawnlocation_offset);
    bzero(known_bytes, sizeof(known_bytes));
    
    vm_offset_t player_last_input_timestamp_offset = player_accountage_offset - 4 - 8;
    printf("int %s = %p;\n", "RBX_PLAYER_LAST_INPUT_TIMESTAMP_OFFSET", (void*)player_last_input_timestamp_offset);
    
    vm_offset_t part_properties_offset = 0;
    vm_offset_t part_properties_gravity_offset = 0;
    vm_offset_t part_properties_cframe_offset = 0;
    vm_offset_t part_properties_velocity_offset = 0;
    vm_offset_t part_properties_rotvelocity_offset = 0;
    
    for (int i = 0 ; i < size ; i++)
    {
        if (i % 8 == 0)
        {
            if (!part_properties_offset)
            {
                vm_address_t a = vm_read_8byte_value(task, xaxis + i);
                if (is_valid_pointer(task, a) && rbx_instance_get_self(task, a) != a)
                {
                    for (int _i = 0 ; _i < size ; _i++)
                    {
                        float b = ((int_float_u)(int)vm_read_4byte_value(task, a + _i)).f;
                        float c = ((int_float_u)(int)vm_read_4byte_value(task, a + _i + 8 + sizeof(rbx_cframe_t) - sizeof(vector3_t))).f;
                        float d = ((int_float_u)(int)vm_read_4byte_value(task, a + _i + 8 + sizeof(rbx_cframe_t) - sizeof(vector3_t) + 4)).f;
                        float e = ((int_float_u)(int)vm_read_4byte_value(task, a + _i + 8 + sizeof(rbx_cframe_t) - sizeof(vector3_t) + 8)).f;
                        if (truncf(b) == 200)
                        {
                            if (truncf(c) == -250 && truncf(d) == 30 && truncf(e) == 733)
                            {
                                //printf("xaxis: [instance + %p] + %p: %f, %f %f %f\n", i, _i, b, c, d, e);
                                vm_address_t a0 = vm_read_8byte_value(task, zaxis + i);
                                float b0 = ((int_float_u)(int)vm_read_4byte_value(task, a0 + _i)).f;
                                if (truncf(b0) == 200)
                                {
                                    part_properties_offset = i;
                                    part_properties_gravity_offset = _i;
                                    part_properties_cframe_offset = _i + 8;
                                    part_properties_velocity_offset = part_properties_cframe_offset + sizeof(rbx_cframe_t);
                                    part_properties_rotvelocity_offset = part_properties_velocity_offset + sizeof(vector3_t);
                                    printf("int %s = %p;\n", "RBX_PART_PROPERTIES_OFFSET", (void*)part_properties_offset);
                                    printf("int %s = %p;\n", "RBX_PART_PROPERTIES_GRAVITY_OFFSET", (void*)part_properties_gravity_offset);
                                    printf("int %s = %p;\n", "RBX_PART_PROPERTIES_CFRAME_OFFSET", (void*)part_properties_cframe_offset);
                                    printf("int %s = %p;\n", "RBX_PART_PROPERTIES_VELOCITY_OFFSET", (void*)part_properties_velocity_offset);
                                    printf("int %s = %p;\n", "RBX_PART_PROPERTIES_ROTVELOCITY_OFFSET", (void*)part_properties_rotvelocity_offset);
                                    //printf("zaxis: [instance + %p] + %p\n", i, _i);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    
    vm_offset_t part_properties_cancollide_offset = 0;
    vm_offset_t part_properties_size_offset = 0;
    vm_offset_t part_rgb_color_offset = 0;
    
    for (int i = 0 ; i < size; i++)
    {
        if (i % 1 == 0)
        {
            if (!part_properties_cancollide_offset)
            {
                vm_address_t a = vm_read_8byte_value(task, xaxis + part_properties_offset);
                vm_address_t b = vm_read_8byte_value(task, zaxis + part_properties_offset);
                vm_address_t c = vm_read_8byte_value(task, spawnlocation + part_properties_offset);
                char d = vm_read_1byte_value(task, a + i);
                char e = vm_read_1byte_value(task, b + i);
                char f = vm_read_1byte_value(task, c + i);
                if (d == true && e == false && f == true)
                {
                    part_properties_cancollide_offset = i;
                    printf("int %s = %p;\n", "RBX_PART_PROPERTIES_CANCOLLIDE_OFFSET", (void*)part_properties_cancollide_offset);
                }
            }
            if (!part_rgb_color_offset)
            {
                rbx_rgb_t xaxis_rgb_color = {.r = 0xff, .g = 0, .b = 0}; //255, 0, 0
                rbx_rgb_t zaxis_rgb_color = {.r = 0, .g = 0, .b = 0xff}; //0, 0, 255
                rbx_rgb_t a;
                a.r = vm_read_1byte_value(task, xaxis + i);
                a.g = vm_read_1byte_value(task, xaxis + i + 1);
                a.b = vm_read_1byte_value(task, xaxis + i + 2);
                if (memcmp(&a, &xaxis_rgb_color, sizeof(rbx_rgb_t)) == 0)
                {
                    rbx_rgb_t b;
                    b.r = vm_read_1byte_value(task, zaxis + i);
                    b.g = vm_read_1byte_value(task, zaxis + i + 1);
                    b.b = vm_read_1byte_value(task, zaxis + i + 2);
                    if (memcmp(&b, &zaxis_rgb_color, sizeof(rbx_rgb_t)) == 0)
                    {
                        part_rgb_color_offset = i;
                        printf("int %s = %p;\n", "RBX_PART_RGB_COLOR_OFFSET", (void*)part_rgb_color_offset);
                    }
                }
            }
        }
        if (i % 4 == 0)
        {
            if (!part_properties_size_offset)
            {
                vm_address_t a = vm_read_8byte_value(task, xaxis + part_properties_offset);
                float b = ((int_float_u)(int)vm_read_4byte_value(task, a + i)).f;
                float c = ((int_float_u)(int)vm_read_4byte_value(task, a + i + 4)).f;
                float d = ((int_float_u)(int)vm_read_4byte_value(task, a + i + 8)).f;
                if (truncf(b) == 8 && truncf(c) == 1 && truncf(d) == 2) //The x,y,z size for the XAxis part
                {
                    part_properties_size_offset = i;
                    printf("int %s = %p;\n", "RBX_PART_PROPERTIES_SIZE_OFFSET", (void*)part_properties_size_offset);
                }
            }
        }
    }
    
    
    kr = vm_read(task, meshpart1, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8) ; i++)
    {
        vm_address_t asset_id_str_ptr = vm_read_8byte_value(task, meshpart1 + i);
        char* asset_id_str = NULL;
        kr = vm_read(task, asset_id_str_ptr, RBX_MESHPART_ASSET_ID_STRING_LENGTH, (vm_offset_t*)&asset_id_str, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            if (strcmp(asset_id_str, "rbxassetid://11661592844") == 0)
            {
                printf("int %s = %p;\n", "RBX_MESHPART_MESHID_OFFSET", (void*)i);
            }
            if (strcmp(asset_id_str, "rbxassetid://11661592868") == 0)
            {
                printf("int %s = %p;\n", "RBX_MESHPART_TEXTUREID_OFFSET", (void*)i);
            }
            vm_deallocate(mach_task_self_, (vm_offset_t)asset_id_str, RBX_MESHPART_ASSET_ID_STRING_LENGTH);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    

    
    
    
    char camera_cframe_offset_found = false;
    rbx_cframe_t camera_cframe = vm_read_rbx_cframe_value(task, cframe_value + intvalue_value_offset);
    
    kr = vm_read(task, camera, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8); i++)
    {
        float a = ((float*)(read_data + i))[0];
        if (truncf(a * 1000) == truncf((70/(180/PI)) * 1000))
        {
            printf("int %s = %p;\n", "RBX_CAMERA_FOV_OFFSET", (void*)i);
        }
        vm_address_t b = ((vm_address_t*)(read_data + i))[0];
        if (b == humanoid)
        {
            printf("int %s = %p;\n", "RBX_CAMERA_CAMERA_SUBJECT_OFFSET", (void*)i);
        }
        if (camera_cframe_offset_found == false)
        {
            if (memcmp((rbx_cframe_t*)(read_data + i), &camera_cframe, sizeof(rbx_cframe_t)) == 0)
            {
                
                camera_cframe_offset_found = true;
                printf("int %s = %p;\n", "RBX_CAMERA_CFRAME_0_OFFSET", (void*)(i));
            }
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    char display_name_len = strlen(display_name);
    char name_bytes[display_name_len + 1]; //First byte in memory is the string length * 2
    name_bytes[0] = display_name_len * 2;
    memcpy(name_bytes + 1, display_name, display_name_len);

    
    kr = vm_read(task, my_plr, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - (display_name_len + 1)) ; i++)
    {
        if (memcmp((char*)(read_data + i), name_bytes, sizeof(name_bytes)) == 0)
        {
            printf("int %s = %p;\n", "RBX_PLAYER_DISPLAY_NAME_OFFSET", (void*)i);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    vm_offset_t humanoid_move_direction_offset = 0;
    for (int i = 0 ; i < size; i++)
    {
        if (i % 4 == 0)
        {
            if (!humanoid_move_direction_offset)
            {
                double movedirx = ((long_double_u)(long)vm_read_8byte_value(task, noobmovedirectionx_value + intvalue_value_offset)).d;
                double movediry = ((long_double_u)(long)vm_read_8byte_value(task, noobmovedirectiony_value + intvalue_value_offset)).d;
                double movedirz = ((long_double_u)(long)vm_read_8byte_value(task, noobmovedirectionz_value + intvalue_value_offset)).d;
                float a = ((int_float_u)(int)vm_read_4byte_value(task, noob_humanoid + i)).f;
                float b = ((int_float_u)(int)vm_read_4byte_value(task, noob_humanoid + i + 4)).f;
                float c = ((int_float_u)(int)vm_read_4byte_value(task, noob_humanoid + i + 8)).f;
                if (truncf(a * 1000) == truncf(movedirx * 1000) &&
                    truncf(b * 1000) == truncf(movediry * 1000) &&
                    truncf(c * 1000) == truncf(movedirz * 1000))
                {
                    humanoid_move_direction_offset = i;
                    printf("int %s = %p;\n", "RBX_HUMANOID_MOVE_DIRECTION_OFFSET", (void*)humanoid_move_direction_offset);
                }
            }
        }
    }
    
    vm_offset_t tool_can_be_dropped_offset = 0;
    for (int i = 0 ; i < size; i++)
    {
        if (!tool_can_be_dropped_offset)
        {
            char a = vm_read_1byte_value(task, starterpack_classicsword + i);
            char b = vm_read_1byte_value(task, starterpack_classicslinghsot + i);
            if (a == true && b == true)
            {
                char c = vm_read_1byte_value(task, starterpack_rocketlauncher + i);
                if (c == false)
                {
                    tool_can_be_dropped_offset = i;
                    printf("int %s = %p;\n", "RBX_TOOL_CANBEDROPPED_OFFSET", (void*)tool_can_be_dropped_offset);
                }
            }
        }
    }
    
    
    vm_offset_t text_label_short_text_offset = 0;
    kr = vm_read(task, text_label, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8); i++)
    {
        if (!text_label_short_text_offset)
        {
            if ((strcmp("Amogus", (char*)(read_data + i)) == 0))
            {
                text_label_short_text_offset = (i - 1); //First byte is the string length * 2;
                printf("int %s = %p;\n", "RBX_TEXTLABEL_SHORT_TEXT_OFFSET", (void*)text_label_short_text_offset);
            }
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    
    vm_offset_t players_respawn_time_offset = 0;
    vm_offset_t players_max_players_offset = 0;
    vm_offset_t lighting_fogend_offset = 0;
    vm_offset_t lighting_brightness_offset = 0;
    vm_offset_t lighting_ambient_offset = 0;
    for (int i = 0 ; i < size ; i++)
    {
        if (i % 4 == 0)
        {
            if (!players_max_players_offset)
            {
                int a = vm_read_4byte_value(task, players_service + i);
                int b = vm_read_4byte_value(task, players_service + i + 4);
                if (a == 10 && a == b)
                {
                    players_max_players_offset = i;
                    printf("int %s = %p;\n", "RBX_PLAYERS_MAX_PLAYER_OFFSET", (void*)players_max_players_offset);
                }
            }
            if (!players_respawn_time_offset)
            {
                float a = ((int_float_u)(int)vm_read_4byte_value(task, players_service + i)).f;
                if (truncf(a) == 5)
                {
                    players_respawn_time_offset = i;
                    printf("int %s = %p;\n", "RBX_PLAYERS_RESPAWN_TIME_OFFSET", (void*)players_respawn_time_offset);
                }
            }
            if (!lighting_fogend_offset)
            {
                float a = ((int_float_u)(int)vm_read_4byte_value(task, lighting + i)).f;
                if (truncf(a) == 420)
                {
                    lighting_fogend_offset = i;
                    printf("int %s = %p;\n", "RBX_LIGHTING_FOGEND_OFFSET", (void*)lighting_fogend_offset);
                }
            }
            if (!lighting_brightness_offset)
            {
                float a = ((int_float_u)(int)vm_read_4byte_value(task, lighting + i)).f;
                float b = ((int_float_u)(int)vm_read_4byte_value(task, lighting + i + 4)).f;
                float c = ((int_float_u)(int)vm_read_4byte_value(task, lighting + i + 8)).f;
                float d = ((int_float_u)(int)vm_read_4byte_value(task, lighting + i + 12)).f;
                if (truncf(a) == 2 &&
                    truncf(b * 100) == truncf(((255.f/255.f) * 100)) &&
                    truncf(c * 100) == truncf(((0.f/255) * 100)) &&
                    truncf(d * 100) == truncf(((127.f/255.f) * 100)))
                {
                    lighting_brightness_offset = i;
                    lighting_ambient_offset = i + 4;
                    printf("int %s = %p;\n", "RBX_LIGHTING_BRIGHTNESS_OFFSET", (void*)lighting_brightness_offset);
                    printf("int %s = %p;\n", "RBX_LIGHTING_AMBIENT_OFFSET", (void*)lighting_ambient_offset);
                }
            }
        }
    }
    
}






void fly_test(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    void* dlhandle = dlopen(__LIBESP_DYLIB_PATH__, RTLD_NOW);
    
    vm_address_t s_load_address = task_get_image_address_by_path(mach_task_self_, __LIBESP_DYLIB_PATH__);
    
    vm_offset_t keys_down_offset = gdso(dlhandle, s_load_address, "KEYS_DOWN");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static vm_address_t character = 0; rbx_player_get_character(task, local_player);
    static vm_address_t hrp = 0; rbx_instance_find_first_child(task, character, "HumanoidRootPart");
    
    static char is_w_pressed = false;
    static char is_a_pressed = false;
    static char is_s_pressed = false;
    static char is_d_pressed = false;
    
    static float speed = 16;
    
    static vector3_t vel = {.x = 0, .y = 0, .z = 0};
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            character = rbx_player_get_character(task, local_player);
            hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(1000);
            
            is_w_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 'w');
            is_a_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 'a');
            is_s_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 's');
            is_d_pressed = vm_read_1byte_value(task, load_address + keys_down_offset + 'd');
            
            rbx_cframe_t cf = rbx_camera_get_cframe(task, camera);
            rbx_basepart_set_gravity(task, hrp, 0.0f);
            vector3_t lv = rbx_cframe_get_look_vector(cf);
            vector3_t rv = rbx_cframe_get_right_vector(cf);
            
            bzero(&vel, sizeof(vector3_t));
            
            if (is_w_pressed)
            {
                vel = vector3_add(vector3_mul_num(lv, 1), vel);
            }
            if (is_a_pressed)
            {
                vel = vector3_add(vector3_mul_num(rv, -1), vel);
            }
            if (is_s_pressed)
            {
                vel = vector3_add(vector3_mul_num(lv, -1), vel);
            }
            if (is_d_pressed)
            {
                vel = vector3_add(vector3_mul_num(rv, 1), vel);
            }
            float magnitude = vector3_magnitude(vel);
            if (!isnan(magnitude) && magnitude > 0.0f)
            {
                vel = vector3_div_num(vel,  magnitude);
            }
            vel = vector3_mul_num(vel, speed);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(100);
            rbx_basepart_set_velocity(task, hrp, vel);
        }
    });
    
}








void func_call_test(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    void* dlhandle = dlopen(__LIBESP_DYLIB_PATH__, RTLD_NOW);
    
    vm_address_t s_load_address = task_get_image_address_by_path(mach_task_self_, __LIBESP_DYLIB_PATH__);
    
    vm_offset_t function_queue_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE");
    vm_offset_t function_queue_count_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE_COUNT");
    vm_offset_t function_queue_finished_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE_FINISHED");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t terrain = rbx_instance_find_first_child_of_class(task, workspace, "Terrain");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t rs = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    
    vm_address_t re = rbx_instance_find_first_child(task, rs, "InitiateBoom");
    vm_address_t rf = rbx_instance_find_first_child(task, rs, "RemoteFunction");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t character = rbx_player_get_character(task, local_player);
    vm_address_t humanoid = rbx_instance_find_first_child_of_class(task, character, "Humanoid");
    vm_address_t tool = rbx_instance_find_first_child_of_class(task, character, "Tool");
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    vm_address_t humanoid_set_walkspeed = task_base_address + RBX_HUMANOID_WALKSPEED_SETTER_METHOD_OFFSET;
    //vm_address_t instance_set_parent = task_base_address + RBX_INSTANCE_PARENT_SETTER_METHOD_OFFSET;
    vm_address_t re_fs = task_base_address + 0x107e788;
    vm_address_t rf_is = task_base_address + 0x107de10;
    vm_address_t camera_w2s = task_base_address + 0xd0dee4;
    vm_address_t terrain_clear = task_base_address + 0xfbb2e6;
    
    vm_address_t mem2;
    //char* s = "\x52kicked by libESP.dylib!!!!";
    vm_allocate(task, &mem2, 1000, VM_FLAGS_ANYWHERE);
    //vm_write(task, mem2, (vm_address_t)s, 26);
    
    vm_address_t mem;
    vm_allocate(task, &mem, 1000, VM_FLAGS_ANYWHERE);
    
    printf("rf: %p\n", rf);
    
    //printf("%p\n", mem);
    
    RemoteFunctionCall remote_call;
    memset(&remote_call, 0, sizeof(remote_call));
    remote_call.type = 1;
    remote_call.address = terrain_clear;
    ((long*)remote_call.arguments)[0] = terrain;
    ((long*)remote_call.arguments)[1] = mem;
    ((long*)remote_call.arguments)[2] = mem2;
    remote_call.finished = 0;
    
    int c = 1;
    char f = 0;
    
    
    vm_write(task, load_address + function_queue_offset, &remote_call, sizeof(RemoteFunctionCall));
    vm_write(task, load_address + function_queue_count_offset, &c, 4);
    vm_write(task, load_address + function_queue_finished_offset, &f, 1);

    printf("CALLED\n");
    
    
    sleep(1);
    
    vm_read(task, load_address + function_queue_offset, sizeof(remote_call), &remote_call, &data_cnt);
    printf("%p\n", *(long*)(remote_call.return_bytes));
    
    
     /*
    for (;;)
    {
        usleep(1000);
        
        vm_write(task, load_address + function_queue_offset, &remote_call, sizeof(RemoteFunctionCall));
        vm_write(task, load_address + function_queue_count_offset, &c, 4);
        vm_write(task, load_address + function_queue_finished_offset, &f, 1);
    }*/
    
    /*
    vm_address_t classdesc = vm_read_8byte_value(task, humanoid + 0x18);
    vm_address_t list_start = vm_read_8byte_value(task, classdesc + 0x30);
    vm_address_t list_end = vm_read_8byte_value(task, classdesc + 0x38);
    printf("%p, %p\n", list_start, list_end);
    for (long i = 0 ; i < (list_end - list_start)/8 ; i++)
    {
        vm_address_t rbx_class_method_info = vm_read_8byte_value(task, list_start + (i * 8));
        vm_address_t cmin = vm_read_8byte_value(task, rbx_class_method_info + 8);
        char* str = NULL;
        int data_cnt = 0;
        vm_read(task, cmin, 100, &str, &data_cnt);
        printf("%p, %s\n", rbx_class_method_info, str + 1);
    }*/
}



void fob_tool_all_test(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    void* dlhandle = dlopen(__LIBESP_DYLIB_PATH__, RTLD_NOW);
    
    vm_address_t s_load_address = task_get_image_address_by_path(mach_task_self_, __LIBESP_DYLIB_PATH__);
    
    vm_offset_t function_queue_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE");
    vm_offset_t function_queue_count_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE_COUNT");
    vm_offset_t function_queue_finished_offset = gdso(dlhandle, s_load_address, "FUNCTION_QUEUE_FINISHED");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t backpack = rbx_instance_find_first_child_of_class(task, local_player, "Backpack");
    vm_address_t character = rbx_player_get_character(task, local_player);
    vm_address_t humanoid = rbx_instance_find_first_child_of_class(task, character, "Humanoid");
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    vm_address_t humanoid_set_walkspeed = task_base_address + RBX_HUMANOID_WALKSPEED_SETTER_METHOD_OFFSET;
    //vm_address_t instance_set_parent = task_base_address + RBX_INSTANCE_PARENT_SETTER_METHOD_OFFSET;
    vm_address_t tool_activate = task_base_address + RBX_TOOL_ACTIVATE_METHOD_OFFSET;
    vm_address_t humanoid_equiptool = task_base_address + 0x6fcae0;

    vm_address_t str;
    //char* s = "\x52kicked by libESP.dylib!!!!";
    vm_allocate(task, &str, 100, VM_FLAGS_ANYWHERE);
    
    for (;;)
    {

        wait_until_queue_finished(task, load_address + function_queue_finished_offset, 1000);
        
        /*long backpack_cc = 0;
        rbx_child_t* backpack_children = rbx_instance_get_children(task, backpack, &backpack_cc);
        if (backpack_children)
        {
            for (long i = 0 ; i < backpack_cc ; i++)
            {*/
        vm_address_t tool = rbx_instance_find_first_child_of_class(task, backpack, "Tool");
        for (;;)
        {
                if (tool && rbx_instance_is_a(task, tool, "Tool"))
                {
                    RemoteFunctionCall remote_call;
                    remote_call.type = 2;
                    remote_call.address = humanoid_equiptool;
                    ((long*)remote_call.arguments)[0] = humanoid;
                    ((long*)remote_call.arguments)[1] = tool;
                    remote_call.finished = 0;
                    int c = 1;
                    char f = 0;
                    vm_write(task, load_address + function_queue_offset, &remote_call, sizeof(RemoteFunctionCall));
                    vm_write(task, load_address + function_queue_count_offset, &c, 4);
                    vm_write(task, load_address + function_queue_finished_offset, &f, 1);
                    wait_until_queue_finished(task, load_address + function_queue_finished_offset, 1000);
                }
        }
       /*     }
            vm_deallocate(mach_task_self_, backpack_children, backpack_cc * sizeof(rbx_child_t));
        }*/
        
        wait_until_queue_finished(task, load_address + function_queue_finished_offset, 1000);
        
        long character_cc = 0;
        rbx_child_t* character_children = rbx_instance_get_children(task, character, &character_cc);
        if (character_children)
        {
            for (long i = 0 ; i < character_cc ; i++)
            {
                vm_address_t obj = character_children[i].child_address;
                if (obj && rbx_instance_is_a(task, obj, "Tool"))
                {
                    RemoteFunctionCall remote_call;
                    remote_call.type = 1;
                    remote_call.address = tool_activate;
                    ((long*)remote_call.arguments)[0] = obj;
                    remote_call.finished = 0;
                    int c = 1;
                    char f = 0;
                    vm_write(task, load_address + function_queue_offset, &remote_call, sizeof(RemoteFunctionCall));
                    vm_write(task, load_address + function_queue_count_offset, &c, 4);
                    vm_write(task, load_address + function_queue_finished_offset, &f, 1);
                    wait_until_queue_finished(task, load_address + function_queue_finished_offset, 1000);
                }
            }
            vm_deallocate(mach_task_self_, character_children, character_cc * sizeof(rbx_child_t));
        }
        usleep(100000);
    }
    
    
     
    for (;;)
    {
        usleep(1000);
    }
    
    /*
    vm_address_t classdesc = vm_read_8byte_value(task, humanoid + 0x18);
    vm_address_t list_start = vm_read_8byte_value(task, classdesc + 0x30);
    vm_address_t list_end = vm_read_8byte_value(task, classdesc + 0x38);
    printf("%p, %p\n", list_start, list_end);
    for (long i = 0 ; i < (list_end - list_start)/8 ; i++)
    {
        vm_address_t rbx_class_method_info = vm_read_8byte_value(task, list_start + (i * 8));
        vm_address_t cmin = vm_read_8byte_value(task, rbx_class_method_info + 8);
        char* str = NULL;
        int data_cnt = 0;
        vm_read(task, cmin, 100, &str, &data_cnt);
        printf("%p, %s\n", rbx_class_method_info, str + 1);
    }*/
}


void find_function_offsets(task_t task)
{
    vm_address_t root_table = rbx_find_root_table(task);
    printf("<<<ROOT>>>: %p\n", (void*)root_table);
    
    printf("int %s = %p;\n", "RBX_INSTANCE_PARENT_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Instance", "Parent", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_HUMANOID_WALKSPEED_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "WalkSpeed", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_HUMANOID_JUMPPOWER_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "JumpPower", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_TOOL_ACTIVATE_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Tool", "Activate", RBX_METHOD_TYPE_NEITHER));
}
