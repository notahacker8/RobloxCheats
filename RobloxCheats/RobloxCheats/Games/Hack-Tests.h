

/*
 
 Offsets of object properties are found by scanning for known values.
 Sometimes, we may have to compare results of multiple scans to find a common offset.
 
 */


void find_object_offsets(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    static kern_return_t kr;
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    struct segment_command_64 __DATA_seg = task_get_seg(task, task_base_address, "__DATA");
    
    vm_offset_t placeid_offset = 0;
    
    kr = vm_read(task, task_base_address + __DATA_seg.fileoff, __DATA_seg.vmsize, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (__DATA_seg.vmsize/8) ; i++)
        {
            unsigned long a = ((unsigned long*)read_data)[i];
            if (a == 5332389196)
            {
                placeid_offset = (i * 8) + __DATA_seg.fileoff;
            }
        }
        vm_deallocate(mach_task_self_, read_data, __DATA_seg.vmsize);
    }
    
    printf("int %s = %p;\n", "RBX_PLACEID_OFFSET", (void*)placeid_offset);
    
    vm_size_t size = 0x1000;
    
    vm_address_t game = rbx_find_game_address(task);
    //rbx_print_descendants(task, game, 0, 0);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t lighting_service = rbx_instance_find_first_child_of_class(task, game, "Lighting");
    vm_address_t defaultskybox = rbx_instance_find_first_child(task, lighting_service, "DefaultSkyBox");
    //rbx_print_descendants(task, game, 0, 1);
    vm_address_t replicated_storage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t ofo = rbx_instance_find_first_child(task, replicated_storage, "ObjectsForOffsets");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t teams_service = rbx_instance_find_first_child_of_class(task, game, "Teams");
    vm_address_t amongus_red_team = rbx_instance_find_first_child(task, teams_service, "Amongus - red");
    
    long plr_count = 0;
    rbx_child_t* players_list = rbx_instance_get_children(task, players_service, &plr_count);
    vm_address_t local_player = players_list[0].child_address;
    //rbx_print_descendants(task, local_player, 0, 0);
    
    long lpnl;
    char* local_player_name = rbx_instance_get_name(task, local_player, &lpnl);
    
    vm_address_t local_character = rbx_instance_find_first_child(task, workspace, local_player_name);
    //rbx_print_descendants(task, local_character, 0, 1);
    vm_address_t local_humanoid = rbx_instance_find_first_child(task, local_character, "Humanoid");
    
    vm_address_t ofo_intvalue = rbx_instance_find_first_child(task, ofo, "IntValue");
    
    vm_address_t ofo_camera_folder = rbx_instance_find_first_child(task, ofo, "Camera");
    vm_address_t ofo_camera_cframevalue = rbx_instance_find_first_child(task, ofo_camera_folder, "CFrameValue");
    
    vm_address_t ofo_player_folder = rbx_instance_find_first_child(task, ofo, "Player");
    vm_address_t ofo_player_team_ov = rbx_instance_find_first_child(task, ofo_player_folder, "Team");
    vm_address_t ofo_player_displayname_sv = rbx_instance_find_first_child(task, ofo_player_folder, "DisplayName");
    
    vm_address_t ofo_game_folder = rbx_instance_find_first_child(task, ofo, "Game");
    vm_address_t ofo_game_jobid_sv = rbx_instance_find_first_child(task, ofo_game_folder, "JobId");
    
    vm_address_t ofo_humanoid_folder = rbx_instance_find_first_child(task, ofo, "Humanoid");
    vm_address_t ofo_humanoid1 = rbx_instance_find_first_child(task, ofo_humanoid_folder, "Humanoid1");
    
    vm_address_t ofo_gui_folder = rbx_instance_find_first_child(task, ofo, "Gui");
    vm_address_t ofo_textlabel1 = rbx_instance_find_first_child(task, ofo_gui_folder, "TextLabel1");
    vm_address_t ofo_textlabel2 = rbx_instance_find_first_child(task, ofo_gui_folder, "TextLabel2");
    vm_address_t ofo_frame1 = rbx_instance_find_first_child(task, ofo_gui_folder, "Frame1");
    vm_address_t ofo_frame2 = rbx_instance_find_first_child(task, ofo_gui_folder, "Frame2");
    
    vm_address_t ofo_part_folder = rbx_instance_find_first_child(task, ofo, "Part");
    vm_address_t ofo_part1 = rbx_instance_find_first_child(task, ofo_part_folder, "Part1");
    vm_address_t ofo_part2 = rbx_instance_find_first_child(task, ofo_part_folder, "Part2");
    vm_address_t ofo_part_cframe1_cv = rbx_instance_find_first_child(task, ofo_part_folder, "CFrame1");
    vm_address_t ofo_part_cframe2_cv = rbx_instance_find_first_child(task, ofo_part_folder, "CFrame2");
    vm_address_t ofo_part3 = rbx_instance_find_first_child(task, ofo_part_folder, "Part3");
    vm_address_t ofo_part4 = rbx_instance_find_first_child(task, ofo_part_folder, "Part4");
    
    char known_bytes[100];
    
    ///(Check "/Objects/brickcolors.h")
    ((int*)known_bytes)[0] = 1004; //Really red
    vm_offset_t team_brickcolor_offset = scan_bytes_offset(task, amongus_red_team, size, known_bytes, 4, 4);
    printf("int %s = %p;\n", "RBX_TEAM_BRICKCOLOR_OFFSET", (void*)team_brickcolor_offset);
    bzero(known_bytes, sizeof(known_bytes));
    
    bzero(known_bytes, sizeof(known_bytes));
    *(vm_address_t*)known_bytes = rbx_instance_find_first_child(task, local_character, "HumanoidRootPart");
    vm_offset_t model_primarypart_offset = scan_bytes_offset(task, local_character, size, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_MODEL_PRIMARYPART_OFFSET", (void*)(model_primarypart_offset));
    
    bzero(known_bytes, sizeof(known_bytes));
    *(int*)known_bytes = 42069;
    vm_offset_t intvalue_value_offset = scan_bytes_offset(task, ofo_intvalue, size, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_INTVALUE_VALUE_OFFSET", (void*)(intvalue_value_offset));
    
    bzero(known_bytes, sizeof(known_bytes));
    *(rbx_cframe_t*)known_bytes = vm_read_rbx_cframe_value(task, ofo_camera_cframevalue + intvalue_value_offset);
    vm_offset_t camera_cframe_offset = scan_bytes_offset(task, camera, size, known_bytes, sizeof(rbx_cframe_t), 4);
    printf("int %s = %p;\n", "RBX_CAMERA_CFRAME_OFFSET", (void*)(camera_cframe_offset));
    
    bzero(known_bytes, sizeof(known_bytes));
    *(vm_address_t*)known_bytes = local_humanoid;
    vm_offset_t camera_camerasubject_offset = scan_bytes_offset(task, camera, size, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_CAMERA_CAMERASUBJECT_OFFSET", (void*)(camera_camerasubject_offset));
    
    kr = vm_read(task, camera, size, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (size - 8); i++)
        {
            float a = ((float*)(read_data + i))[0];
            if (truncf(a * 1000) == truncf((70/(180/PI)) * 1000))
            {
                printf("int %s = %p;\n", "RBX_CAMERA_FIELDOFVIEW_OFFSET", (void*)i);
            }
        }
        vm_deallocate(mach_task_self_, read_data, data_cnt);
    }
    
    bzero(known_bytes, sizeof(known_bytes));
    ((int*)known_bytes)[0] = 10;
    ((int*)known_bytes)[1] = 10;
    vm_offset_t players_maxplayers_offset = scan_bytes_offset(task, players_service, size, known_bytes, 8, 4);
    printf("int %s = %p;\n", "RBX_PLAYERS_MAXPLAYERS_OFFSET", (void*)(players_maxplayers_offset));
    
    bzero(known_bytes, sizeof(known_bytes));
    *(vm_address_t*)known_bytes = local_character;
    vm_offset_t player_character_offset = scan_bytes_offset(task, local_player, size, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_PLAYER_CHARACTER_OFFSET", (void*)(player_character_offset));
    
    bzero(known_bytes, sizeof(known_bytes));
    *(vm_address_t*)known_bytes = vm_read_8byte_value(task, ofo_player_team_ov + intvalue_value_offset);
    vm_offset_t player_team_offset = scan_bytes_offset(task, local_player, size, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_PLAYER_TEAM_OFFSET", (void*)(player_team_offset));
    
    long klpdl = 0;
    char* known_local_player_displayname = vm_read_rbx_string(task, ofo_player_displayname_sv + intvalue_value_offset, &klpdl);
    
    long kjil = 0;
    char* known_jobid = vm_read_rbx_string(task, ofo_game_jobid_sv + intvalue_value_offset, &kjil);
    
    for (long i = 0 ; i < (size - 8); i++)
    {
        long a_l = 0;
        char* a = vm_read_rbx_string(task, local_player + i, &a_l);
        if (a)
        {
            if (strcmp(a, known_local_player_displayname) == 0)
            {
                printf("int %s = %p;\n", "RBX_PLAYER_DISPLAYNAME_OFFSET", (void*)(i));
            }
            vm_deallocate(mach_task_self_, (vm_address_t)a, a_l);
        }
        
        long b_l = 0;
        char* b = vm_read_rbx_string(task, game + i, &b_l);
        if (b)
        {
            if (strcmp(b, known_jobid) == 0)
            {
                printf("int %s = %p;\n", "RBX_DATAMODEL_JOBID_OFFSET", (void*)(i));
            }
            vm_deallocate(mach_task_self_, (vm_address_t)b, b_l);
        }
        
        long c_l = 0;
        char* c = vm_read_rbx_string(task, ofo_humanoid1 + i, &c_l);
        if (c)
        {
            if (strcmp(c, "FirstName") == 0)
            {
                printf("int %s = %p;\n", "RBX_HUMANOID_DISPLAYNAME_OFFSET", (void*)(i));
            }
            vm_deallocate(mach_task_self_, (vm_address_t)c, c_l);
        }
        
        long d_l = 0;
        char* d = vm_read_rbx_string(task, ofo_textlabel2 + i, &d_l);
        if (d)
        {
            if (strcmp(d, "ReallyLongSampleText_1234567890_abcdefghijklmnopqrstuvwxyz") == 0)
            {
                static char found = false;
                if (!found)
                {
                    found = true;
                    printf("int %s = %p;\n", "RBX_TEXTLABEL_TEXT_OFFSET", (void*)(i));
                }
            }
            vm_deallocate(mach_task_self_, (vm_address_t)d, d_l);
        }
        long f_l = 0;
        char* f = vm_read_rbx_string(task, defaultskybox + i, &f_l);
        if (f)
        {
            if (strcmp(f, "rbxasset://sky/moon.jpg") == 0)
            {
                static char found = false;
                if (!found)
                {
                    found = true;
                    printf("int %s = %p;\n", "RBX_SKY_MOONTEXTUREID_OFFSET", (void*)(i));
                }
            }
            vm_deallocate(mach_task_self_, (vm_address_t)f, f_l);
        }
    }
    
    vm_offset_t basepart_properties_offset = 0;
    
    kr = vm_read(task, ofo_part1, size, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (size - 8)/8; i++)
        {
            vm_address_t a = ((vm_address_t*)read_data)[i];
            if (is_valid_pointer(task, a) && rbx_instance_get_self(task, a) != a)
            {
                
                rbx_cframe_t known_cframe1 = vm_read_rbx_cframe_value(task, ofo_part_cframe1_cv + intvalue_value_offset);
                vm_offset_t basepart_properties_cframe_offset = scan_bytes_offset(task, a, size, (char*)&known_cframe1, sizeof(rbx_cframe_t), 4);
                if (basepart_properties_cframe_offset)
                {
                    rbx_cframe_t known_cframe2 = vm_read_rbx_cframe_value(task, ofo_part_cframe2_cv + intvalue_value_offset);
                    vm_address_t pp2 = vm_read_8byte_value(task, ofo_part2 + (i * 8));
                    rbx_cframe_t read_cframe2 = vm_read_rbx_cframe_value(task, pp2 + basepart_properties_cframe_offset);
                    if (memcmp(&read_cframe2, &known_cframe2, sizeof(rbx_cframe_t)) == 0)
                    {
                        basepart_properties_offset = (i * 8);
                        printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_OFFSET", (void*)(i * 8));
                        printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_CFRAME_OFFSET", (void*)(basepart_properties_cframe_offset));
                        printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_GRAVITY_OFFSET", (void*)(basepart_properties_cframe_offset - 0x8));
                        printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_VELOCITY_OFFSET", (void*)(basepart_properties_cframe_offset + sizeof(rbx_cframe_t)));
                        printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_ROTVELOCITY_OFFSET", (void*)(basepart_properties_cframe_offset + sizeof(rbx_cframe_t) + sizeof(vector3_t)));
                        i = (size - 8)/8;
                    }
                }
            }
        }
        vm_deallocate(mach_task_self_, read_data, data_cnt);
    }
    
    bzero(known_bytes, sizeof(known_bytes));
    *(vector3_t*)known_bytes = (vector3_t){.x = 8, .y = 1, .z = 2};
    vm_offset_t basepart_properties_size_offset = scan_bytes_offset(task, vm_read_8byte_value(task, ofo_part1 + basepart_properties_offset), size, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_SIZE_OFFSET", (void*)(basepart_properties_size_offset));
    
    
    kr = vm_read(task, lighting_service, size, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (size - 16); i++)
        {
            float a = *((float*)(read_data + i));
            float b = *((float*)(read_data + i + 4));
            float c = *((float*)(read_data + i + 8));
            float d = *((float*)(read_data + i + 12));
            if (truncf(a) == 420.0f)
            {
                printf("int %s = %p;\n", "RBX_LIGHTING_FOGEND_OFFSET", (void*)(i * 4));
            }
            if (truncf(a) == 2 &&
                truncf(b * 100) == truncf(((255.f/255.f) * 100)) &&
                truncf(c * 100) == truncf(((0.f/255) * 100)) &&
                truncf(d * 100) == truncf(((127.f/255.f) * 100)))
            {
                printf("int %s = %p;\n", "RBX_LIGHTING_BRIGHTNESS_OFFSET", (void*)(i * 4));
                printf("int %s = %p;\n", "RBX_LIGHTING_AMBIENT_OFFSET", (void*)((i + 1) * 4));
            }
            int e = *(int*)(read_data + i);
            if (e == RBX_LIGHTING_CLOCKTIME_HOUR_TICK_COUNT)
            {
                printf("int %s = %p;\n", "RBX_LIGHTING_CLOCKTIME_TICK_OFFSET", (void*)(i));
            }
        }
        vm_deallocate(mach_task_self_, read_data, data_cnt);
    }
    
    kr = vm_read(task, ofo_frame1, size, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < size; i++)
        {
            char a = ((char*)read_data)[i];
            if (a == true)
            {
                if (vm_read_1byte_value(task, ofo_frame2 + i) == false)
                {
                    if (vm_read_1byte_value(task, ofo_textlabel1 + i) == true)
                    {
                        if (vm_read_1byte_value(task, ofo_textlabel2 + i) == true)
                        {
                            printf("int %s = %p;\n", "RBX_FRAME_VISIBLE_OFFSET", (void*)(i));
                            i = size;
                        }
                    }
                }
            }
        }
        vm_deallocate(mach_task_self_, read_data, data_cnt);
    }
    
    {
        vm_address_t zap = rbx_instance_find_first_child(task, workspace, "ZAxis");
        vm_address_t xap = rbx_instance_find_first_child(task, workspace, "XAxis");
        vm_address_t wpp = rbx_instance_find_first_child(task, workspace, "WorldPoint");
        vm_address_t zap_pp = vm_read_8byte_value(task, zap + basepart_properties_offset);
        vm_address_t xap_pp = vm_read_8byte_value(task, xap + basepart_properties_offset);
        vm_address_t wpp_pp = vm_read_8byte_value(task, wpp + basepart_properties_offset);
        
        kr = vm_read(task, xap_pp, size, &read_data, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            for (long i = 0 ; i < size; i++)
            {
                char a = ((char*)read_data)[i];
                if (a == true)
                {
                    if (vm_read_1byte_value(task, zap_pp + i) == false)
                    {
                        if (vm_read_1byte_value(task, wpp_pp + i) == true)
                        {
                            printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_CANCOLLIDE_OFFSET", (void*)(i));
                            i = size;
                        }
                    }
                }
            }
            vm_deallocate(mach_task_self_, read_data, data_cnt);
        }
    }
    
    kr = vm_read(task, local_player, size, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (size - 8)/8; i++)
        {
            vm_address_t a = ((vm_address_t*)read_data)[i];
            if (is_valid_pointer(task, a) && rbx_instance_get_self(task, a) == a)
            {
                if (rbx_instance_is_a(task, a, "PlayerMouse"))
                {
                    printf("int %s = %p;\n", "RBX_PLAYER_MOUSE_OFFSET", (void*)(i * 8));
                    i = size;
                }
            }
        }
        vm_deallocate(mach_task_self_, read_data, data_cnt);
    }
    //printf("%p\n", ofo_textlabel1);
}







void find_function_offsets(task_t task)
{
    vm_address_t root_table =  rbx_find_root_table(task); //rbx_find_root_table(task);
    printf("<<<ROOT>>>: %p\n", (void*)root_table);
    
    printf("int %s = %p;\n", "RBX_INSTANCE_PARENT_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Instance", "Parent", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_HUMANOID_WALKSPEED_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "WalkSpeed", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_HUMANOID_WALKSPEED_GETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "WalkSpeed", RBX_METHOD_TYPE_GETTER));
    
    printf("int %s = %p;\n", "RBX_HUMANOID_JUMPPOWER_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "JumpPower", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_TOOL_ACTIVATE_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Tool", "Activate", RBX_METHOD_TYPE_NEITHER));
    
    printf("int %s = %p;\n", "RBX_HUMANOID_HEALTH_GETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "Health", RBX_METHOD_TYPE_GETTER));
    
    printf("int %s = %p;\n", "RBX_HUMANOID_HEALTH_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "Health", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_WORKSPACE_GETSERVERTIMENOW_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Workspace", "GetServerTimeNow", RBX_METHOD_TYPE_NEITHER));
    
    printf("int %s = %p;\n", "RBX_BASEPART_ASSEMBLYLINEARVELOCITY_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "BasePart", "AssemblyLinearVelocity", RBX_METHOD_TYPE_SETTER));
    
    printf("int %s = %p;\n", "RBX_BASEPART_ASSEMBLYLINEARVELOCITY_GETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "BasePart", "AssemblyLinearVelocity", RBX_METHOD_TYPE_GETTER));
    
    printf("int %s = %p;\n", "RBX_REMOTEEVENT_FIRESERVER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "RemoteEvent", "FireServer", RBX_METHOD_TYPE_NEITHER));
    
    printf("int %s = %p;\n", "RBX_REMOTEFUNCTION_INVOKESERVER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "RemoteFunction", "InvokeServer", RBX_METHOD_TYPE_NEITHER));
    
    printf("int %s = %p;\n", "RBX_MOUSE_HIT_GETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Mouse", "Hit", RBX_METHOD_TYPE_GETTER));
    
    printf("int %s = %p;\n", "RBX_BASEPART_ANCHORED_GETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "BasePart", "Anchored", RBX_METHOD_TYPE_GETTER));
}
