




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
    vm_deallocate(mach_task_self_, possible_place_ids, size);
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t xaxis = rbx_instance_find_first_child(task, workspace, "XAxis");
    vm_address_t motor6d = rbx_instance_find_first_child_of_class(task, xaxis, "Motor6D");
    vm_address_t xaxis2 = rbx_instance_find_first_child(task, workspace, "XAxis2");
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
    vm_address_t hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
    vm_address_t humanoid = rbx_instance_find_first_child(task, my_character, "Humanoid");
    
    printf("//values folder: %p\n", values_folder);
    printf("//camera: %p\n", camera);
    printf("//xaxis: %p\n", xaxis);
    printf("//motor6d: %p\n", motor6d);
    printf("//xaxis2: %p\n", xaxis2);
    printf("//humanoid: %p\n", humanoid);
    printf("//hrp: %p\n", hrp);
    printf("//tool: %p\n", tool);
    printf("//text label: %p\n", text_label);
    
    
    rbx_rgb_t color = rbx_basepart_get_color(task, xaxis);
    
    size = 0x300;
    kr = vm_read(task, camera, size, &read_data, &data_cnt);
    char camera_cframe_offset_found = false;
    for (long i = 0 ; i < (size - 8); i++)
    {
        static int _v1[9];
        _v1[0] = 0x3f800000;
        _v1[4] = 0x3f800000;
        _v1[8] = 0x3f800000;
        int a = ((int*)(read_data + i))[0];
        if (a == 0x3f9c61ab)
        {
            printf("#define %s %p\n", "RBX_CAMERA_FOV_OFFSET", (void*)i);
        }
        vm_address_t b = ((vm_address_t*)(read_data + i))[0];
        if (b == humanoid)
        {
            printf("#define %s %p\n", "RBX_CAMERA_CAMERA_SUBJECT_OFFSET", (void*)i);
        }
        if (memcmp((void*)(read_data + i), &_v1, sizeof(_v1)) == 0)
        {
            float _0 = ((int*)(read_data + i - 4))[0];
            float _1 = ((int*)(read_data + i + sizeof(rbx_cframe_t) - 4 + sizeof(rbx_cframe_t)))[0];
            if (_0 == _1 && camera_cframe_offset_found == false)
            {
                camera_cframe_offset_found = true;
                printf("#define %s %p\n", "RBX_CAMERA_CFRAME_0_OFFSET", (void*)(i - (sizeof(rbx_cframe_t))));
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
            printf("#define %s %p\n", "RBX_PLAYER_TEAM_OFFSET", (void*)i);
        }
        if (a == my_character)
        {
            printf("#define %s %p\n", "RBX_PLAYER_CHARACTER_OFFSET", (void*)i);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    size = 0x200;
    kr = vm_read(task, xaxis, size, &read_data, &data_cnt);
    char part_prop_grav_offset_found = false;
    char part_prop_size_offset_found = false;
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
                if (b == 0x43480000 && part_prop_grav_offset_found == false)
                {
                    part_prop_grav_offset_found = true;
                    printf("#define %s %p\n", "RBX_PART_PROPERTIES_OFFSET", (i * 8));
                    printf("#define %s %p\n", "RBX_PART_PROPERTIES_GRAVITY_OFFSET", (x * 4));
                    printf("#define %s %p\n", "RBX_PART_PROPERTIES_CFRAME_OFFSET", (x * 4) + 8);
                }
                
                if (b == 0x40000000)
                {
                    b = ((int*)_rd)[x + 1];
                    if (b == 0x40000000)
                    {
                        b = ((int*)_rd)[x + 2];
                        if (b == 0x3f800000 && part_prop_size_offset_found == false)
                        {
                            part_prop_size_offset_found = true;
                            printf("#define %s %p\n", "RBX_PART_PROPERTIES_SIZE_OFFSET", (x * 4));
                        }
                    }
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
    char textlabel_text_offset_found = false;
    for (long i = 0 ; i < (size - 8); i++)
    {
        if ((strcmp("Amogus", read_data + i) == 0) && textlabel_text_offset_found == false)
        {
            textlabel_text_offset_found = true;
            printf("#define %s %p // keep the first\n", "RBX_TEXT_LABEL_SHORT_TEXT_OFFSET", i - 1);
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
}
