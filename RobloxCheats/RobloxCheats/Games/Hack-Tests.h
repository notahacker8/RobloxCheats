

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

void find_object_offsets(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    static kern_return_t kr;
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    struct segment_command_64 __DATA_seg;
    task_mach_segment_info(task, task_base_address, "__DATA", &__DATA_seg);
    
    vm_offset_t placeid_offset = 0;
    for (long addr = task_base_address + __DATA_seg.fileoff ;
         addr < task_base_address + __DATA_seg.fileoff + __DATA_seg.vmsize; addr++)
    {
        if ((addr % 8) == 0)
        {
            long a = vm_read_8byte_value(task, addr);
            if (a == 5332389196)
            {
                placeid_offset = (addr - task_base_address);
            }
        }
    }
    
    printf("int %s = %p;\n", "RBX_PLACEID_OFFSET", (void*)placeid_offset);
    
    vm_size_t size = 0x1000;
    
    vm_address_t game = rbx_find_game_address(task);
    //rbx_print_descendants(task, game, 0, 10);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t localdata_folder = rbx_instance_find_first_child(task, workspace, "LocalData");
    vm_address_t displayname_folder = rbx_instance_find_first_child(task, localdata_folder, "DisplayName");
    vm_address_t jobid_folder = rbx_instance_find_first_child(task, localdata_folder, "JobId");
    
    long display_name_len = 0;
    char* display_name = rbx_instance_get_name(task, rbx_instance_find_first_child_of_class(task, displayname_folder, "Folder"), &display_name_len);
    
    long job_id_len = 0;
    char* job_id = rbx_instance_get_name(task, rbx_instance_find_first_child_of_class(task, jobid_folder, "Folder"), &job_id_len);
    
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
    vm_address_t screengui = rbx_instance_find_first_child(task, plr_gui, "ScreenGui");
    vm_address_t textlabel = rbx_instance_find_first_child(task, screengui, "TextLabel");
    vm_address_t screengui2 = rbx_instance_find_first_child(task, plr_gui, "ScreenGui2");
    vm_address_t screengui2_frame = rbx_instance_find_first_child(task, screengui2, "Frame");
    
    long my_plr_name_len;
    char* my_plr_name = rbx_instance_get_name(task, my_plr, &my_plr_name_len);
    vm_address_t my_character = rbx_instance_find_first_child(task, workspace, my_plr_name);
    vm_address_t backpack = rbx_instance_find_first_child(task, my_plr, "Backpack");
    vm_address_t tool = rbx_instance_find_first_child(task, backpack, "ClassicSword");
    vm_address_t handle = rbx_instance_find_first_child(task, tool, "Handle");
    vm_address_t hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
    vm_address_t head = rbx_instance_find_first_child(task, my_character, "Head");
    vm_address_t humanoid = rbx_instance_find_first_child(task, my_character, "Humanoid");
    
    
    rbx_print_descendants(task, workspace, 0, 0);
    
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
    
    vm_offset_t datamodel_jobid_offset = 0;
    
    for (int i = 0 ; i < size ; i++)
    {
        if (i % 8 == 0)
        {
            if (!datamodel_jobid_offset)
            {
                vm_address_t a = vm_read_8byte_value(task, game + i);
                if (is_valid_pointer(task, a))
                {
                    char* b = NULL;
                    vm_read(task, a, RBX_JOBID_LENGTH, (vm_address_t*)&b, &data_cnt);
                    if (b)
                    {
                        if (strncmp(job_id, b, RBX_JOBID_LENGTH) == 0)
                        {
                            datamodel_jobid_offset = i;
                            printf("int %s = %p;\n", "RBX_DATAMODEL_JOBID_OFFSET", (void*)datamodel_jobid_offset);
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)b, RBX_JOBID_LENGTH);
                    }
                }
            }
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
                        if (truncf(b) == 200.0f)
                        {
                            if (truncf(c) == -250.0f && truncf(d) == 30.0f && truncf(e) == 733.0f)
                            {
                                //printf("xaxis: [instance + %p] + %p: %f, %f %f %f\n", i, _i, b, c, d, e);
                                vm_address_t a0 = vm_read_8byte_value(task, zaxis + i);
                                float b0 = ((int_float_u)(int)vm_read_4byte_value(task, a0 + _i)).f;
                                if (truncf(b0) == 200.0f)
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
    
    
    vm_offset_t player_displayname_offset = 0;
    vm_offset_t textlabel_text_offset = 0;
    vm_offset_t humanoid_displayname_offset = 0;
    
    for (int i = 0 ; i < size; i++)
    {
        if (!player_displayname_offset)
        {
            long a_l = 0;
            char* a = vm_read_rbx_string(task, my_plr + i, &a_l);
            if (a)
            {
                if (strcmp(a, display_name) == 0)
                {
                    player_displayname_offset = i;
                    printf("int %s = %p;\n", "RBX_PLAYER_DISPLAYNAME_OFFSET", (void*)(player_displayname_offset));
                }
                vm_deallocate(mach_task_self_, (vm_address_t)a, a_l);
            }
        }
        if (!textlabel_text_offset)
        {
            long a_l = 0;
            char* a = vm_read_rbx_string(task, textlabel + i, &a_l);
            if (a)
            {
                if (strcmp(a, "Amogus") == 0)
                {
                    textlabel_text_offset = i;
                    printf("int %s = %p;\n", "RBX_TEXTLABEL_TEXT_OFFSET", (void*)(textlabel_text_offset));
                }
                vm_deallocate(mach_task_self_, (vm_address_t)a, a_l);
            }
        }
        if (!humanoid_displayname_offset)
        {
            long a_l = 0;
            char* a = vm_read_rbx_string(task, sitting_noob_humanoid + i, &a_l);
            if (a)
            {
                if (strcmp(a, "Sitting Noob") == 0)
                {
                    humanoid_displayname_offset = i;
                    printf("int %s = %p;\n", "RBX_HUMANOID_DISPLAYNAME_OFFSET", (void*)(humanoid_displayname_offset));
                }
                vm_deallocate(mach_task_self_, (vm_address_t)a, a_l);
            }
        }
    }
    
    
    
    vm_offset_t humanoid_move_direction_offset = 0;
    vm_offset_t humanoid_encrypted_health_offset = 0;
    vm_offset_t humanoid_encrypted_maxhealth_offset = 0;
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
                    printf("int %s = %p;\n", "RBX_HUMANOID_MOVEDIRECTION_OFFSET", (void*)humanoid_move_direction_offset);
                }
            }
            if (!humanoid_encrypted_health_offset)
            {
                float a = vm_read_rbx_encrypted_float(task, noob_humanoid + i);
                if (truncf(a) == 55.0f)
                {
                    humanoid_encrypted_health_offset = i;
                    printf("int %s = %p;\n", "RBX_HUMANOID_ENCRYPTED_HEALTH_OFFSET", (void*)humanoid_encrypted_health_offset);
                }
            }
            if (!humanoid_encrypted_maxhealth_offset)
            {
                float a = vm_read_rbx_encrypted_float(task, noob_humanoid + i);
                if (truncf(a) == 100.0f)
                {
                    humanoid_encrypted_maxhealth_offset = i;
                    printf("int %s = %p;\n", "RBX_HUMANOID_ENCRYPTED_MAXHEALTH_OFFSET", (void*)humanoid_encrypted_maxhealth_offset);
                }
            }
        }
    }
    
    vm_offset_t tool_canbedropped_offset = 0;
    vm_offset_t frame_visible_offset = 0;
    for (int i = 0 ; i < size; i++)
    {
        if (!tool_canbedropped_offset)
        {
            char a = vm_read_1byte_value(task, starterpack_classicsword + i);
            char b = vm_read_1byte_value(task, starterpack_classicslinghsot + i);
            if (a == true && b == true)
            {
                char c = vm_read_1byte_value(task, starterpack_rocketlauncher + i);
                if (c == false)
                {
                    tool_canbedropped_offset = i;
                    printf("int %s = %p;\n", "RBX_TOOL_CANBEDROPPED_OFFSET", (void*)tool_canbedropped_offset);
                }
            }
        }
        if (!frame_visible_offset)
        {
            char a = vm_read_1byte_value(task, textlabel + i);
            if (a == true)
            {
                char b = vm_read_1byte_value(task, screengui2_frame + i);
                if (b == false)
                {
                    frame_visible_offset = i;
                    printf("int %s = %p;\n", "RBX_FRAME_VISIBLE_OFFSET", (void*)frame_visible_offset);
                }
            }
        }
    }
    
    
    vm_offset_t players_respawn_time_offset = 0;
    vm_offset_t players_maxplayers_offset = 0;
    vm_offset_t lighting_fogend_offset = 0;
    vm_offset_t lighting_brightness_offset = 0;
    vm_offset_t lighting_ambient_offset = 0;
    for (int i = 0 ; i < size ; i++)
    {
        if (i % 4 == 0)
        {
            if (!players_maxplayers_offset)
            {
                int a = vm_read_4byte_value(task, players_service + i);
                int b = vm_read_4byte_value(task, players_service + i + 4);
                if (a == 10 && a == b)
                {
                    players_maxplayers_offset = i;
                    printf("int %s = %p;\n", "RBX_PLAYERS_MAXPLAYERS_OFFSET", (void*)players_maxplayers_offset);
                }
            }
            if (!players_respawn_time_offset)
            {
                float a = ((int_float_u)(int)vm_read_4byte_value(task, players_service + i)).f;
                if (truncf(a) == 5.0f)
                {
                    players_respawn_time_offset = i;
                    printf("int %s = %p;\n", "RBX_PLAYERS_RESPAWN_TIME_OFFSET", (void*)players_respawn_time_offset);
                }
            }
            if (!lighting_fogend_offset)
            {
                float a = ((int_float_u)(int)vm_read_4byte_value(task, lighting + i)).f;
                if (truncf(a) == 420.0f)
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
    
    static vm_address_t character = 0;
    static vm_address_t hrp = 0;
    
    static char is_w_pressed = false;
    static char is_a_pressed = false;
    static char is_s_pressed = false;
    static char is_d_pressed = false;
    
    static float speed = 100;
    
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






void find_function_offsets(task_t task)
{
    vm_address_t root_table = rbx_find_root_table(task);
    //printf("<<<ROOT>>>: %p\n", (void*)root_table);
    
    printf("int %s = %p;\n", "RBX_INSTANCE_PARENT_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Instance", "Parent", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_HUMANOID_WALKSPEED_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "WalkSpeed", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_HUMANOID_JUMPPOWER_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "JumpPower", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_TOOL_ACTIVATE_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Tool", "Activate", RBX_METHOD_TYPE_NEITHER));
    
    //Use this to see how the some floats are encrypted. (The health value is encrypted in memory)
    printf("int %s = %p;\n", "RBX_HUMANOID_HEALTH_GETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "Health", RBX_METHOD_TYPE_GETTER));
    
    printf("int %s = %p;\n", "RBX_WORKSPACE_GETSERVERTIMENOW_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Workspace", "GetServerTimeNow", RBX_METHOD_TYPE_NEITHER));
}
