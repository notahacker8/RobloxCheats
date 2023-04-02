


void func(task_t task, vm_address_t instance)
{
    if (rbx_instance_is_a(task, instance, "Part") ||
        rbx_instance_is_a(task, instance, "MeshPart") ||
        rbx_instance_is_a(task, instance, "WedgePart"))
    {
        rbx_basepart_set_cancollide(task, instance, false);
        //rbx_basepart_set_size(task, instance, (vector3_t){.x = -10, .y = -10, .z = -10});
    }
}

void find_object_offsets(task_t task, char* display_name)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    static kern_return_t kr;
    
    vm_address_t** regions = task_get_regions(task, 3);
    vm_address_t data_start = regions[0][1]; //Start of __DATA section
    vm_address_t data_end = regions[1][2]; //End of __DATA section
    vm_address_t* possible_place_ids;
    vm_size_t size = (data_end - data_start);
    vm_read(task, data_start, size, (vm_address_t*)&possible_place_ids, &data_cnt);
    free(regions);
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
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
    vm_address_t other_values_folder = rbx_instance_find_first_child(task, values_folder, "OtherValues");
    vm_address_t short_string_value = rbx_instance_find_first_child(task, values_folder, "ShortStringValue");
    vm_address_t bool_value = rbx_instance_find_first_child(task, values_folder, "BoolValue");
    vm_address_t int_value = rbx_instance_find_first_child(task, values_folder, "IntValue");
    vm_address_t cframe_value = rbx_instance_find_first_child(task, values_folder, "CFrameValue");
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
    char my_plr_name_len;
    char* my_plr_name = rbx_instance_get_name(task, my_plr, &my_plr_name_len);
    vm_address_t my_character = rbx_instance_find_first_child(task, workspace, my_plr_name);
    vm_address_t backpack = rbx_instance_find_first_child(task, my_plr, "Backpack");
    vm_address_t tool = rbx_instance_find_first_child(task, backpack, "ClassicSword");
    vm_address_t handle = rbx_instance_find_first_child(task, tool, "Handle");
    vm_address_t hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
    vm_address_t head = rbx_instance_find_first_child(task, my_character, "Head");
    vm_address_t humanoid = rbx_instance_find_first_child(task, my_character, "Humanoid");
    
    /*
    rbx_print_descendants(task, my_plr, 0, 3);
    rbx_print_descendants(task, workspace, 0, 1);
    rbx_print_descendants(task, game, 0, 0);*/
    //rbx_print_descendants(task, workspace, 0, 2);*/
    //printf("%s\n", rbx_player_get_display_name(task, my_plr, malloc(1)));
    
    printf("localplr: %p\n", my_plr);
    printf("handle: %p\n", handle);
    printf("tool: %p\n", tool);
    printf("xaxis: %p\n", xaxis);
    /*
     0x11045D22C    ldr w22, [x7, #0x330]       F6 30 43 B9
     0x11045D230    cmp w22, #5                 DF 16 00 71
     */
    printf("hrp: %p\n", hrp);
    printf("hum: %p\n", humanoid);
    printf("nmdx: %p\n", noobmovedirectionx_value);
    
    rbx_print_descendants(task, workspace, 0, 2);
    rbx_rgb_t xaxis_rgb_color = {.r = 0xff, .g = 0, .b = 0}; //255, 0, 0
    rbx_rgb_t zaxis_rgb_color = {.r = 0, .g = 0, .b = 0xff}; //0, 0, 255
    
    size = 0x400;
    kr = vm_read(task, xaxis, size, &read_data, &data_cnt);
    long part_prop_offset = 0;
    long part_cframe_offset = 0;
    long part_gravity_offset = 0;
    for (long i = 0 ; i < size/8 ; i++)
    {
        vm_address_t a = ((vm_address_t*)read_data)[i];
        vm_address_t _rd;
        kr = vm_read(task, a, size, &_rd, &data_cnt);
        if (kr == KERN_SUCCESS && a != xaxis)
        {
            for (long x = 0 ; x < size/4 ; x++)
            {
                float b = ((float*)_rd)[x];
                float c = ((float*)_rd)[x + 1];
                float d = ((float*)_rd)[x + 2];
                //-269 for ZAxis
                if (truncf(b) == -250 && truncf(c) == 30 && truncf(d) == 733) //x,y,z coordinates of the XAxis part
                {
                    long propoffset = (i * 8);
                    long cf_offset = (x * 4) + sizeof(vector3_t) - sizeof(rbx_cframe_t);
                    long grav_offset = cf_offset - 8;
                    float grav = *((float*)(_rd + grav_offset));
                    if (truncf(grav) == 200) //gravity is 200
                    {
                        vm_address_t zaxispropaddr = vm_read_8byte_value(task, zaxis + propoffset);
                        int_float_u zaxisval_u;
                        zaxisval_u.i = vm_read_4byte_value(task, zaxispropaddr + grav_offset);
                        if (truncf(zaxisval_u.f) == 200) //check if the offsets work for another part (ZAxis part)
                        {
                            part_prop_offset = propoffset;
                            part_cframe_offset = cf_offset;
                            part_gravity_offset = grav_offset;
                            printf("#define %s %p\n", "RBX_PART_PROPERTIES_OFFSET", (void*)propoffset);
                            printf("#define %s %p\n", "RBX_PART_PROPERTIES_GRAVITY_OFFSET", (void*)grav_offset);
                            printf("#define %s %p\n", "RBX_PART_PROPERTIES_CFRAME_OFFSET", (void*)cf_offset);
                            x = size/4;
                            i = size/8;
                        }
                    }
                }
            }
        }
        vm_deallocate(mach_task_self_, _rd, size);
    }
    vm_deallocate(mach_task_self_, read_data, size);
    
    vm_address_t xaxispropaddr = vm_read_8byte_value(task, xaxis + part_prop_offset);
    vm_address_t zaxispropaddr = vm_read_8byte_value(task, zaxis + part_prop_offset);
    
    size = 0x400;
    kr = vm_read(task, xaxispropaddr, size, &read_data, &data_cnt);
    for (long i = 0 ; i < size; i++)
    {
        char a = ((char*)read_data)[i];
        if (a == true && vm_read_1byte_value(task, zaxispropaddr + i) == false)
        {
            printf("#define %s %p\n", "RBX_PART_PROPERTIES_CANCOLLIDE_OFFSET", (void*)i);
            i = size;
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x400;
    kr = vm_read(task, xaxis, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - sizeof(rbx_rgb_t)) ; i++)
    {
        rbx_rgb_t a = *((rbx_rgb_t*)(read_data + i));
        if (memcmp(&a, &xaxis_rgb_color, sizeof(rbx_rgb_t)) == 0)
        {
            int b = vm_read_4byte_value(task, zaxis + i);
            if (memcmp(&b, &zaxis_rgb_color, sizeof(rbx_rgb_t)) == 0)
            {
                printf("#define %s %p\n", "RBX_PART_RGB_COLOR_OFFSET", (void*)i);
                i = (size - sizeof(rbx_rgb_t));
            }
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x400;
    kr = vm_read(task, xaxispropaddr, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size/4) - 8 ; i++)
    {
        float a = ((float*)read_data)[i];
        float b = ((float*)read_data)[i + 1];
        float c = ((float*)read_data)[i + 2];
        if (truncf(a) == 8 && truncf(b) == 1 && truncf(c) == 2) //The x,y,z size for the XAxis part
        {
            printf("#define %s %p\n", "RBX_PART_PROPERTIES_SIZE_OFFSET", (void*)(i * 4));
            i = (size/4);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    
    size = 0x400;
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
                printf("#define %s %p\n", "RBX_MESHPART_MESHID_OFFSET", (void*)i);
            }
            if (strcmp(asset_id_str, "rbxassetid://11661592868") == 0)
            {
                printf("#define %s %p\n", "RBX_MESHPART_TEXTUREID_OFFSET", (void*)i);
            }
            vm_deallocate(mach_task_self_, (vm_offset_t)asset_id_str, RBX_MESHPART_ASSET_ID_STRING_LENGTH);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    
    
    size = 0x300;
    kr = vm_read(task, my_character, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8); i++)
    {
        vm_address_t a = *((vm_address_t*)(read_data + i));
        if (a == head)
        {
            printf("#define %s %p\n", "RBX_MODEL_PRIMARYPART_OFFSET", (void*)i);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x300;
    kr = vm_read(task, short_string_value, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8); i++)
    {
        if (strcmp("Short string", (char*)(read_data + i)) == 0)
        {
            printf("#define %s %p\n", "RBX_STRING_VALUE_SHORT_VALUE_OFFSET", (void*)(i - 1));
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x300;
    long int_value_value_offset = 0;
    kr = vm_read(task, int_value, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8); i++)
    {
        long a = *((long*)(read_data + i));
        if (a == 42069)
        {
            int_value_value_offset = i;
            printf("#define %s %p\n", "RBX_INT_VALUE_VALUE_OFFSET", (void*)i);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x300;
    long team_brickcolor_offset = 0;
    kr = vm_read(task, my_team, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 4); i++)
    {
        int a = *((int*)(read_data + i));
        ///(Check "/Objects/brickcolors.h")
        if (a == 1004) //Really red
        {
            team_brickcolor_offset = i;
            printf("#define %s %p\n", "RBX_TEAM_BRICKCOLOR_OFFSET", (void*)i);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    
    char camera_cframe_offset_found = false;
    rbx_cframe_t camera_cframe;
    vm_read(task, cframe_value + int_value_value_offset, sizeof(rbx_cframe_t), (vm_offset_t*)&read_data, &data_cnt);
    camera_cframe = *(rbx_cframe_t*)read_data;
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x300;
    kr = vm_read(task, camera, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size - 8); i++)
    {
        int a = ((int*)(read_data + i))[0];
        if (a == 0x3f9c61ab) //(70/(180/PI)), as a float
        {
            printf("#define %s %p\n", "RBX_CAMERA_FOV_OFFSET", (void*)i);
        }
        vm_address_t b = ((vm_address_t*)(read_data + i))[0];
        if (b == humanoid)
        {
            printf("#define %s %p\n", "RBX_CAMERA_CAMERA_SUBJECT_OFFSET", (void*)i);
        }
        if (camera_cframe_offset_found == false)
        {
            if (memcmp((rbx_cframe_t*)(read_data + i), &camera_cframe, sizeof(rbx_cframe_t)) == 0)
            {
                
                camera_cframe_offset_found = true;
                printf("#define %s %p\n", "RBX_CAMERA_CFRAME_0_OFFSET", (void*)(i));
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
            printf("#define %s %p\n", "RBX_PLAYER_DISPLAY_NAME_OFFSET", (void*)i);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    int known_account_age = vm_read_4byte_value(task, playeraccountage_value + int_value_value_offset);
    
    
    size = 0x1000;
    kr = vm_read(task, my_plr, size, &read_data, &data_cnt);
    for (int i = 0 ; i < (size - 8); i++)
    {
        int a = *((int*)(read_data + i));
        if (a == known_account_age)
        {
            double b = *((double*)(read_data + i - 0xc));
            if (b > 0.0f && b < 99999999.0f)
            {
                printf("#define %s %p\n", "RBX_PLAYER_ACCOUNT_AGE_OFFSET", (void*)(i));
                printf("#define %s %p\n", "RBX_PLAYER_LAST_INPUT_TIMESTAMP_OFFSET", (void*)(i - 0xc));
            }
        }
        
        vm_address_t c = ((vm_address_t*)(read_data + i))[0];
        if (c == my_team)
        {
            printf("#define %s %p\n", "RBX_PLAYER_TEAM_OFFSET", (void*)i);
        }
        if (c == my_character)
        {
            printf("#define %s %p\n", "RBX_PLAYER_CHARACTER_OFFSET", (void*)i);
        }
        if (c == spawnlocation)
        {
            printf("#define %s %p\n", "RBX_PLAYER_RESPAWNLOCATION_OFFSET", (void*)i);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x1000;
    kr = vm_read(task, noob_humanoid, size, &read_data, &data_cnt);
    float movedirx = (float)rbx_numbervalue_get_value(task, noobmovedirectionx_value);
    float movediry = (float)rbx_numbervalue_get_value(task, noobmovedirectiony_value);
    float movedirz = (float)rbx_numbervalue_get_value(task, noobmovedirectionz_value);
    for (int i = 0 ; i < (size - 12); i++)
    {
        float a = *((float*)(read_data + i));
        if (truncf(a * 100.0f) == truncf(movedirx * 100.0f))
        {
            float b = *((float*)(read_data + i + 4));
            if (truncf(b * 100.0f) == truncf(movediry * 100.0f))
            {
                float c = *((float*)(read_data + i + 8));
                if (truncf(c * 100.0f) == truncf(movedirz * 100.0f))
                {
                    printf("#define %s %p\n", "RBX_HUMANOID_MOVE_DIRECTION_OFFSET", (void*)i);
                }
            }
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x400;
    kr = vm_read(task, sitting_noob_humanoid, size, &read_data, &data_cnt);
    for (long i = 0 ; i < size; i++)
    {
        char a = ((char*)read_data)[i];
        if (a == true && vm_read_1byte_value(task, humanoid + i) == false && vm_read_1byte_value(task, noob_humanoid) == false)
        {
            printf("#define %s %p\n", "RBX_HUMANOID_STTING_OFFSET", (void*)i);
            //i = size;
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x1000;
    kr = vm_read(task, text_label, size, &read_data, &data_cnt);
    char textlabel_text_offset_found = false;
    for (long i = 0 ; i < (size - 8); i++)
    {
        if ((strcmp("Amogus", (char*)(read_data + i)) == 0) && textlabel_text_offset_found == false)
        {
            textlabel_text_offset_found = true;
            printf("#define %s %p\n", "RBX_TEXT_LABEL_SHORT_TEXT_OFFSET", (void*)(i - 1));
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    
    //long state = 0;
    for (;;)
    {
        char b = 1;
        vm_write(task, humanoid + 0x184, &b, 1);
        rbx_instance_for_each_descendant(task, my_character, func);
        sleep(1);
    }
}






void fly_test(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    void* dlhandle = dlopen(__INJECTED_DYLIB_PATH__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB_PATH__);
    
    vm_offset_t keys_down_offset = gdso(dlhandle, s_load_address, "KEYS_DOWN");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB_PATH__);
    
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
    
    static float speed = 200;
    
    static vector3_t vel = {.x = 0, .y = 0, .z = 0};
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            character = rbx_player_get_character(task, local_player);
            hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
            rbx_instance_for_each_descendant(task, workspace, func);
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
