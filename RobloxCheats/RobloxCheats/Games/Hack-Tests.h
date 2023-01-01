




void find_object_offsets(task_t task)
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
    
    static int ids_found;
    for (long i = 0 ; i < (size/8) ; i++)
    {
        long _id = possible_place_ids[i];
        if (_id == 5332389196)
        {
            ids_found++;
            if (ids_found == 3) //Third one is stable (For some reason)
            {
                printf("#define %s %p\n", "RBX_PLACE_ID_OFFSET", (void*)(((i * 8) + data_start) - get_base_address(task)));
            }
        }
    }
    vm_deallocate(mach_task_self_, (vm_address_t)possible_place_ids, size);
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t xaxis = rbx_instance_find_first_child(task, workspace, "XAxis");
    vm_address_t zaxis = rbx_instance_find_first_child(task, workspace, "ZAxis");
    vm_address_t values_folder = rbx_instance_find_first_child(task, workspace, "Values");
    vm_address_t short_string_value = rbx_instance_find_first_child(task, values_folder, "ShortStringValue");
    vm_address_t bool_value = rbx_instance_find_first_child(task, values_folder, "BoolValue");
    vm_address_t int_value = rbx_instance_find_first_child(task, values_folder, "IntValue");
    vm_address_t cframe_value = rbx_instance_find_first_child(task, values_folder, "CFrameValue");
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
    vm_address_t head = rbx_instance_find_first_child(task, my_character, "Head");
    vm_address_t humanoid = rbx_instance_find_first_child(task, my_character, "Humanoid");
    
    rbx_print_descendants(task, my_plr, 0, 0);
    rbx_print_descendants(task, my_team, 0, 0);
    rbx_print_descendants(task, workspace, 0, 2);
    //printf("%s\n", rbx_player_get_display_name(task, my_plr, malloc(1)));
    
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
    
    size = 0x400;
    kr = vm_read(task, xaxis, size, &read_data, &data_cnt);
    char part_prop_grav_offset_found = false;
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
                if (part_prop_grav_offset_found == false)
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
                            float grav = ((float*)_rd)[grav_offset/4];
                            if (truncf(grav) == 200) //gravity is 200
                            {
                                vm_address_t zaxispropaddr = vm_read_8byte_value(task, zaxis + propoffset);
                                int_float_u zaxisval_u;
                                zaxisval_u.i = vm_read_4byte_value(task, zaxispropaddr + grav_offset);
                                if (truncf(zaxisval_u.f) == 200) //check if the offsets work for another part (ZAxis part)
                                {
                                    part_prop_grav_offset_found = true;
                                    part_prop_offset = propoffset;
                                    part_cframe_offset = cf_offset;
                                    part_gravity_offset = grav_offset;
                                    printf("#define %s %p\n", "RBX_PART_PROPERTIES_OFFSET", (void*)propoffset);
                                    printf("#define %s %p\n", "RBX_PART_PROPERTIES_GRAVITY_OFFSET", (void*)grav_offset);
                                    printf("#define %s %p\n", "RBX_PART_PROPERTIES_CFRAME_OFFSET", (void*)cf_offset);
                                }
                            }
                        }
                    }
                }
            }
            vm_deallocate(mach_task_self_, _rd, size);
        }
        vm_deallocate(mach_task_self_, read_data, size);
    
    size = 0x400;
    char part_prop_size_offset_found = false;
    vm_address_t xaxis_properties_address = vm_read_8byte_value(task, xaxis + part_prop_offset);
    kr = vm_read(task, xaxis_properties_address, size, &read_data, &data_cnt);
    for (long i = 0 ; i < (size/4) - 8 ; i++)
    {
        if (part_prop_size_offset_found == false)
        {
            float a = ((float*)read_data)[i];
            float b = ((float*)read_data)[i + 1];
            float c = ((float*)read_data)[i + 2];
            if (truncf(a) == 8 && truncf(b) == 1 && truncf(c) == 2) //The x,y,z size for the XAxis part
            {
                printf("#define %s %p\n", "RBX_PART_PROPERTIES_SIZE_OFFSET", (void*)(i * 4));
                part_prop_size_offset_found = true;
            }
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
            printf("#define %s %p\n", "RBX_STRING_VALUE_SHORT_VALUE_OFFSET", (void*)(i - 1)); //first actual byte is the number of characters * 2 (i dont know why)
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
    
}
