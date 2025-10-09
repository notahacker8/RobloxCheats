

/*
 
 Offsets of object properties are found by scanning for known values.
 Sometimes, we may have to compare results of multiple scans to find a common offset.
 
 */


void find_object_offsets(task_t task)
{
    //sleep(10);
    //LIBESP_DYLIB_SETUP
    //rbx_print_ancestors(task, 0x1334D52C0, 4);
    printf("\n");
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    static kern_return_t kr;
    
    vm_address_t task_base_address = task_get_image_address_by_index(task, 0);
    
    struct segment_command_64 __DATA_seg = task_get_seg(task, task_base_address, "__DATA");
    struct segment_command_64 __TEXT_seg = task_get_seg(task, task_base_address, "__TEXT");
    
    struct section_64 __TEXT__text_sect = task_get_sect(task, task_base_address, "__TEXT", "__text");
    
    printf("%p, %p\n", task_base_address+__TEXT__text_sect.offset, __TEXT__text_sect.size);
    
    vm_offset_t placeid_offset = 0;
    kr = vm_read(task, task_base_address + __DATA_seg.fileoff, __DATA_seg.vmsize, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (__DATA_seg.vmsize/8) ; i++)
        {
            unsigned long a = ((unsigned long*)read_data)[i];
            if (a == 15816797935)
            {
                placeid_offset = (i * 8) + __DATA_seg.fileoff;
            }
        }
        vm_deallocate(mach_task_self_, read_data, __DATA_seg.vmsize);
    }
    
    vm_address_t weld_patch_offset = 0;
    vm_address_t hrp_patch_offset = 0;
    kr = vm_read(task, task_base_address + __TEXT__text_sect.offset, __TEXT__text_sect.size, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        for (long i = 0 ; i < (__TEXT__text_sect.size) ; i += 4)
        {
            if (memcmp((read_data + i), "\x51\x68\x00\x4C", 4) == 0)
            {
                weld_patch_offset = i + __TEXT__text_sect.offset;
            }
            if (memcmp((read_data + i), "\x68\x86\x00\xF9\x62\x82\x8F\x3C\x61\x82\x8E\x3C\x60\x82\x8D\x3C", 16) == 0)
            {
                hrp_patch_offset = i + __TEXT__text_sect.offset;
            }
        }
        vm_deallocate(mach_task_self_, read_data, __TEXT__text_sect.size);
    }
    
    vm_address_t game = 0;
    vm_address_t intvalue = 0;
    
    {
        static mach_msg_type_number_t data_cnt;
        vm_address_t ret_addr = 0;
        kern_return_t kr = KERN_SUCCESS;
        struct vm_region_submap_info_64 info;
        vm_address_t address = 0;
        vm_size_t size = 0;
        natural_t depth = 0;
        mach_msg_type_number_t info_count = VM_REGION_SUBMAP_INFO_COUNT_64;
        while (kr == KERN_SUCCESS)
        {
            memset (&info, 0, sizeof (info));
            kr = vm_region_recurse_64 (task, &address, &size,
                                       &depth, (vm_region_recurse_info_t) &info, &info_count);
            if (info.user_tag == VM_MEMORY_IOACCELERATOR)
            {
                vm_address_t read_data;
                int err = vm_read(task, address, size, (vm_address_t*)&read_data, &data_cnt);
                if (!err)
                {
                    for (long i = 0 ; i < (size - 8)/8 ; i++)
                    {
                        if (((vm_address_t*)read_data)[i] == 42069)
                        {
                            ret_addr = address + (i * 8);
                            for (long x = 0 ; x < 0x200/8 ; x++)
                            {
                                vm_address_t a = ret_addr - (x*8);
                                if (vm_read_uint64_t_value(task, a) == a - 8)
                                {
                                    intvalue = a-8;
                                    x = 0x200/8;
                                }
                            }
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)read_data, size);
                }
            }
            address += size;
        }
        
        
        printf("pointer to 42069 int value value: %p\n", (void*)ret_addr);
        printf("intvalue address: %p\n", (void*)intvalue);
        printf("DISABLE \"GOTO\" TO ENABLE INSTANCE PROPERTY OFFSET CHECK:\t%s, line %d\n", __FILE_NAME__, __LINE__);
        
        game = rbx_instance_get_parent(task, rbx_instance_get_parent(task, rbx_instance_get_parent(task, intvalue)));
        RBX_WHAT_GAME_POINTS_TO_OFFSET = (int)(vm_read_uint64_t_value(task, game) - task_base_address);
        
       goto _instance_offset_check_end;
    _instance_offset_check: {}
        
        COPY_VAR(RBX_INSTANCE_NAME_OFFSET);
        COPY_VAR(RBX_INSTANCE_CLASS_INFO_OFFSET);
        
        for (int i = 0 ; i < 0x100 ; i++)
        {
            RBX_INSTANCE_NAME_OFFSET = i;
            RBX_INSTANCE_CLASS_INFO_OFFSET = i;
            {
                long l = 0;
                char* str = rbx_instance_get_name(task, intvalue, &l);
                if (str)
                {
                    printf("Instance.Name: [+%p] %s\n", RBX_INSTANCE_NAME_OFFSET, str);
                    vm_deallocate(mach_task_self_, (vm_address_t)str, l);
                }
            }
            {
                long l = 0;
                char* str = rbx_instance_get_class_name(task, intvalue, &l);
                if (str)
                {
                    printf("Instance.ClassName: [+%p] %s\n", RBX_INSTANCE_CLASS_INFO_OFFSET, str);
                    vm_deallocate(mach_task_self_, (vm_address_t)str, l);
                }
            }
        }
        
        RBX_INSTANCE_NAME_OFFSET = __RBX_INSTANCE_NAME_OFFSET;
        RBX_INSTANCE_CLASS_INFO_OFFSET = __RBX_INSTANCE_CLASS_INFO_OFFSET;
        
        
        COPY_VAR(RBX_INSTANCE_PARENT_OFFSET);
        
        for (int i = 0 ; i < 0x100 ; i++)
        {
            RBX_INSTANCE_PARENT_OFFSET = i;
            {
                vm_address_t p = rbx_instance_get_parent(task, intvalue);
                if (p)
                {
                    
                    long l = 0;
                    char* str = rbx_instance_get_name(task, p, &l);
                    if (str)
                    {
                        printf("Instance.Parent: [+%p] %s\n", RBX_INSTANCE_PARENT_OFFSET, str);
                        vm_deallocate(mach_task_self_, (vm_address_t)str, l);
                    }
                    
                    //printf("Instance.Parent: [+%p]", RBX_INSTANCE_PARENT_OFFSET); rbx_print_descendants(task, p, 0, 0);
                }
            }
        }
        
        RBX_INSTANCE_PARENT_OFFSET = __RBX_INSTANCE_PARENT_OFFSET;
        
        COPY_VAR(RBX_INSTANCE_CHILDREN_OFFSET);
        
        for (int i = 0 ; i < 0x100 ; i++)
        {
            RBX_INSTANCE_CHILDREN_OFFSET = i;
            printf("[%p] ", i); rbx_print_descendants(task, game, 0, 1);
        }
        
        RBX_INSTANCE_CHILDREN_OFFSET = __RBX_INSTANCE_CHILDREN_OFFSET;
        
    _instance_offset_check_end: {}
        
    }
    
    

    
    vm_size_t size = 0x1000;
    
    //game = rbx_find_game_address(task);
    printf("\n-- GAME CHECK --\n");
    rbx_print_descendants(task, game, 0, 0);
    if (!game)
    {
        printf("couldn't find game address\n");
        printf("Use BitSlicer, search for 42069 in 64 bit integers\n");
        printf("subtract the offset to IntValues from the largest address\n");
        printf("the game address is [address + parent_offset] until the parent is 0\n");
        exit(0);
    }
    printf("\n");
    //rbx_print_descendants(task, game, 0, 1);
    printf("int %s = %p;\n", "RBX_WHAT_GAME_POINTS_TO_OFFSET", (void*)(long)RBX_WHAT_GAME_POINTS_TO_OFFSET);
    printf("int %s = %p;\n", "RBX_PLACEID_OFFSET", (void*)placeid_offset);
    printf("int %s = %p;\n", "WELD_PATCH_OFFSET", (void*)weld_patch_offset);
    printf("int %s = %p;\n", "HRP_PATCH_OFFSET", (void*)hrp_patch_offset);
    
    //rbx_print_descendants(task, game, 0, 0);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t lighting_service = rbx_instance_find_first_child_of_class(task, game, "Lighting");
    vm_address_t defaultskybox = rbx_instance_find_first_child(task, lighting_service, "DefaultSkyBox");
    //rbx_print_descendants(task, lighting_service, 0, 0);
    vm_address_t replicated_storage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t initiateboom_re = rbx_instance_find_first_child(task, replicated_storage, "InitiateBoom");
    vm_address_t remotefunction_rf = rbx_instance_find_first_child(task, replicated_storage, "RemoteFunction");
    vm_address_t ofo = rbx_instance_find_first_child(task, replicated_storage, "ObjectsForOffsets");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    //rbx_print_descendants(task, replicated_storage, 0, 3);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t teams_service = rbx_instance_find_first_child_of_class(task, game, "Teams");
    vm_address_t amongus_red_team = rbx_instance_find_first_child(task, teams_service, "Amongus - red");
    
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    //rbx_print_descendants(task, local_player, 0, 10);
    
    vm_address_t seat = rbx_instance_find_first_child(task, workspace, "Seat");
    vm_address_t touchinterest = rbx_instance_find_first_child(task, seat, "TouchInterest");
    //rbx_print_descendants(task, local_player, 0, 0);
    
    long lpnl;
    char* local_player_name = rbx_instance_get_name(task, local_player, &lpnl);
    
    vm_address_t my_character = rbx_instance_find_first_child(task, workspace, local_player_name);
    //rbx_print_descendants(task, local_character, 0, 1);
    vm_address_t my_humanoid = rbx_instance_find_first_child(task, my_character, "Humanoid");
    vm_address_t my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
    vm_address_t my_head = rbx_instance_find_first_child(task, my_character, "Head");
    
    vm_address_t ofo_intvalue = rbx_instance_find_first_child(task, ofo, "IntValue");
    //rbx_print_descendants(task, ofo_intvalue, 0, 0);
    
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
    vm_address_t ofo_pp1 = rbx_instance_find_first_child(task, ofo_gui_folder, "ProximityPrompt1");
    vm_address_t ofo_pp2 = rbx_instance_find_first_child(task, ofo_gui_folder, "ProximityPrompt2");
    
    //rbx_print_descendants(task, ofo_textlabel1, 0, 0);
    
    vm_address_t ofo_part_folder = rbx_instance_find_first_child(task, ofo, "Part");
    vm_address_t ofo_part1 = rbx_instance_find_first_child(task, ofo_part_folder, "Part1");
    //rbx_print_descendants(task, ofo_part1, 0, 0);
    vm_address_t ofo_part2 = rbx_instance_find_first_child(task, ofo_part_folder, "Part2");
    vm_address_t ofo_part_cframe1_cv = rbx_instance_find_first_child(task, ofo_part_folder, "CFrame1");
    vm_address_t ofo_part_cframe2_cv = rbx_instance_find_first_child(task, ofo_part_folder, "CFrame2");
    vm_address_t ofo_part3 = rbx_instance_find_first_child(task, ofo_part_folder, "Part3");
    vm_address_t ofo_part4 = rbx_instance_find_first_child(task, ofo_part_folder, "Part4");
    
    vm_address_t ofo_module_script = rbx_instance_find_first_child(task, ofo, "ModuleScript");
    
#pragma mark - MODULE SCRIPT -
    
    /*
     
     DATA TYPES ENUM:
     1: BOOL
     
     3: DOUBLE
     5: STRING
     6: DICTIONARY : [Value + 0x20] + 0x20 = First element, ARRAY:
     7: FUNCTION
     8: CFRAME/RAY
     
     
     
     
     -- AlmondTypes
     -- Generic all-datatypes example with Almond prefix and PascalCase

     local m = {}
     m.AlmondString = "TestString"
     m.AlmondNumber = 123.456
     m.AlmondBool = true
     m.AlmondArray = {"Item1", "Item2", "Item3"}
     m.AlmondTuple = {3.14, workspace, "TupleString1"}
     m.AlmondDict = {AlmondKey1 = "Value1", AlmondKey2 = 42, AlmondKey3 = false}
     m.AlmondVector3 = Vector3.new(10, 5, -3)
     m.AlmondCFrame = CFrame.new(1, 2, 3) * CFrame.Angles(0, math.rad(45), 0)
     m.AlmondColor3 = Color3.fromRGB(255, 180, 60)
     m.AlmondBrickColor = BrickColor.new("Bright yellow")
     m.AlmondUDim2 = UDim2.new(0.5, 10, 0.5, -10)
     m.AlmondInstance = workspace:FindFirstChild("Baseplate")
     m.AlmondEnum = Enum.HumanoidStateType.Seated
     m.AlmondNumberRange = NumberRange.new(5, 15)
     m.AlmondRect = Rect.new(0, 0, 100, 200)
     m.AlmondRay = Ray.new(Vector3.new(0, 5, 0), Vector3.new(0, -1, 0))

     m.AlmondFunction = function()
         print("AlmondFunction executed")
     end

     function m:AlmondMethodExample(amount)
         self.AlmondNumber -= amount
         print("AlmondNumber is now:", self.AlmondNumber)
         if self.AlmondNumber <= 0 then
             self.AlmondFunction()
         end
     end

     return m

     */
    
    /*
     local m = require(game.ReplicatedStorage.ObjectsForOffsets.ModuleScript)
     */
    
    
    char known_bytes[100];
    
    ///(Check "/Objects/brickcolors.h")
    ((int*)known_bytes)[0] = 1004; //Really red
    vm_offset_t team_brickcolor_offset = scan_bytes_offset(task, amongus_red_team, size, known_bytes, 4, 4);
    printf("int %s = %p;\n", "RBX_TEAM_BRICKCOLOR_OFFSET", (void*)team_brickcolor_offset);
    bzero(known_bytes, sizeof(known_bytes));
    
    bzero(known_bytes, sizeof(known_bytes));
    *(vm_address_t*)known_bytes = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
    vm_offset_t model_primarypart_offset = scan_bytes_offset(task, my_character, size, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_MODEL_PRIMARYPART_OFFSET", (void*)(model_primarypart_offset));
    
    bzero(known_bytes, sizeof(known_bytes));
    *(int*)known_bytes = 42069;
    vm_offset_t intvalue_value_offset = scan_bytes_offset(task, ofo_intvalue, size, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_INTVALUE_VALUE_OFFSET", (void*)(intvalue_value_offset));
    
    vm_offset_t stringvalue_value_offset = intvalue_value_offset;
    vm_offset_t cframevalue_value_offset = intvalue_value_offset;
    
    printf("int %s = %p;\n", "RBX_STRINGVALUE_VALUE_OFFSET", (void*)(stringvalue_value_offset));
    printf("int %s = %p;\n", "RBX_CFRAMEVALUE_VALUE_OFFSET", (void*)(cframevalue_value_offset));
    
    {
        rbx_cframe_t a = vm_read_rbx_cframe_t_value(task, ofo_camera_cframevalue + cframevalue_value_offset);
        vector3_t lv = rbx_cframe_get_look_vector(a);
        printf("//CFrame confirmation: {lv:(%f, %f, %f)}\n", lv.x, lv.y, lv.z);
    }
    
    /*
     @TODO: ^ THESE TWO CAN CHANGE ^
     */
    
    /*bzero(known_bytes, sizeof(known_bytes));
    *(rbx_cframe_t*)known_bytes = vm_read_rbx_cframe_t_value(task, ofo_camera_cframevalue + cframevalue_value_offset);
    vm_offset_t camera_cframe_offset = scan_bytes_offset(task, camera, size, known_bytes, sizeof(rbx_cframe_t), 4);
    printf("int %s = %p;\n", "RBX_CAMERA_CFRAME_OFFSET", (void*)(camera_cframe_offset));*/
    
    {
        rbx_cframe_t known_cframe = vm_read_rbx_cframe_t_value(task, ofo_camera_cframevalue + cframevalue_value_offset);
        vector3_t known_pos = known_cframe.pos;
        kr = vm_read(task, camera, size, &read_data, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            for (long i = 0 ; i < (size - 12); i++)
            {
                float a = ((float*)(read_data + i))[0];
                float b = ((float*)(read_data + i))[1];
                float c = ((float*)(read_data + i))[2];
               // printf("%p: %f, %f, %f | %f, %f, %f\n", i, a, b, c, known_pos.x, known_pos.y, known_pos.z);
                if ((truncf(a) == truncf(known_pos.x)) && (truncf(b) == truncf(known_pos.y)) &&(truncf(c) == truncf(known_pos.z)))
                {
                    printf("int %s = %p;\n", "RBX_CAMERA_CFRAME_OFFSET", (void*)(i - 36));
                    i = (size - 12);
                }
            }
            vm_deallocate(mach_task_self_, read_data, data_cnt);
        }
    }
    
    bzero(known_bytes, sizeof(known_bytes));
    *(vm_address_t*)known_bytes = my_humanoid;
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
    ((int*)known_bytes)[0] = 50;
    ((int*)known_bytes)[1] = 50;
    vm_offset_t players_maxplayers_offset = scan_bytes_offset(task, players_service, size, known_bytes, 8, 4);
    printf("int %s = %p;\n", "RBX_PLAYERS_MAXPLAYERS_OFFSET", (void*)(players_maxplayers_offset));
    
    bzero(known_bytes, sizeof(known_bytes));
    *(vm_address_t*)known_bytes = my_character;
    vm_offset_t player_character_offset = scan_bytes_offset(task, local_player, size, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_PLAYER_CHARACTER_OFFSET", (void*)(player_character_offset));
    
    bzero(known_bytes, sizeof(known_bytes));
    *(vm_address_t*)known_bytes = vm_read_uint64_t_value(task, ofo_player_team_ov + intvalue_value_offset);
    vm_offset_t player_team_offset = scan_bytes_offset(task, local_player, size, known_bytes, 8, 8);
    printf("int %s = %p;\n", "RBX_PLAYER_TEAM_OFFSET", (void*)(player_team_offset));
    
    long klpdl = 0;
    char* known_local_player_displayname = vm_read_rbx_string(task, ofo_player_displayname_sv + stringvalue_value_offset, &klpdl);
    
    long kjil = 0;
    char* known_jobid = vm_read_rbx_string(task, ofo_game_jobid_sv + stringvalue_value_offset, &kjil);
    
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
                
                rbx_cframe_t known_cframe1 = vm_read_rbx_cframe_t_value(task, ofo_part_cframe1_cv + cframevalue_value_offset);
                vm_offset_t basepart_properties_cframe_offset = scan_bytes_offset(task, a, size, (char*)&known_cframe1, sizeof(rbx_cframe_t), 4);
                if (basepart_properties_cframe_offset)
                {
                    rbx_cframe_t known_cframe2 = vm_read_rbx_cframe_t_value(task, ofo_part_cframe2_cv + cframevalue_value_offset);
                    vm_address_t pp2 = vm_read_uint64_t_value(task, ofo_part2 + (i * 8));
                    rbx_cframe_t read_cframe2 = vm_read_rbx_cframe_t_value(task, pp2 + basepart_properties_cframe_offset);
                    if (memcmp(&read_cframe2, &known_cframe2, sizeof(rbx_cframe_t)) == 0)
                    {
                        basepart_properties_offset = (i * 8);
                        printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_OFFSET", (void*)(i * 8));
                        printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_CFRAME_OFFSET", (void*)(basepart_properties_cframe_offset));
                        printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_GRAVITY_OFFSET", (void*)(basepart_properties_cframe_offset - 0x8));
                        printf("int %s = %p;\n", "RBX_BASEPART_PROPERTIES_RECIEVEAGE_OFFSET", (void*)(basepart_properties_cframe_offset - 0x4));
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
    vm_offset_t basepart_properties_size_offset = scan_bytes_offset(task, vm_read_uint64_t_value(task, ofo_part1 + basepart_properties_offset), size, known_bytes, 4, 4);
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
                if (vm_read_uint8_t_value(task, ofo_frame2 + i) == false)
                {
                    if (vm_read_uint8_t_value(task, ofo_textlabel1 + i) == true)
                    {
                        if (vm_read_uint8_t_value(task, ofo_textlabel2 + i) == true)
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
        vm_address_t zap_pp = vm_read_uint64_t_value(task, zap + basepart_properties_offset);
        vm_address_t xap_pp = vm_read_uint64_t_value(task, xap + basepart_properties_offset);
        vm_address_t wpp_pp = vm_read_uint64_t_value(task, wpp + basepart_properties_offset);
        //printf("WP: %p\n", wpp_pp);
        
        /*
         They use bit flags now
         */
        
        kr = vm_read(task, xap_pp, size, &read_data, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            for (long i = 0 ; i < size; i++)
            {
                char a = ((char*)read_data)[i];
                if (a == 0x3a)
                {
                    if (vm_read_uint8_t_value(task, zap_pp + i)  == 0x32)
                    {
                        if (vm_read_uint8_t_value(task, wpp_pp + i) == 0x3a)
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
    
    for (long i = 0 ; i < size; i++)
    {
        long as_l = 0;
        char * as = vm_read_rbx_string(task, ofo_pp1 + i, &as_l);
        if (as)
        {
           // printf("%s\n", as);
            if (strcmp(as, "Interact1") == 0)
            {
                printf("int %s = %p;\n", "RBX_PROXIMITYPROMPT_ACTIONTEXT_OFFSET", (void*)(i));
            }
            if (strcmp(as, "Object1") == 0)
            {
                printf("int %s = %p;\n", "RBX_PROXIMITYPROMPT_OBJECTTEXT_OFFSET", (void*)(i));
            }
            vm_deallocate(mach_task_self_, (vm_address_t)as, as_l);
        }
        float b = vm_read_float_value(task, ofo_pp1 + i);
        if (truncf(b) == 10)
        {
            printf("int %s = %p;\n", "RBX_PROXIMITYPROMPT_MAXACTIVATIONDISTANCE_OFFSET", (void*)(i));
        }
    }
    vm_deallocate(mach_task_self_, read_data, data_cnt);
    
    
    
    
    printf("\n");
    rbx_print_descendants(task, game, 0, 0);
    rbx_print_descendants(task, workspace, 0, 0);
    rbx_print_descendants(task, ofo_module_script, 0, 0);
    rbx_print_descendants(task, local_player, 0, 0);
    rbx_print_descendants(task, my_humanoid, 0, 0);
    rbx_print_descendants(task, my_character, 0, 0);
    rbx_print_descendants(task, my_hrp, 0, 0);
    rbx_print_descendants(task, my_head, 0, 0);
    /*vm_address_t t = rbx_instance_find_first_child_of_class(task, my_character, "Tool");
    rbx_print_descendants(task, t, 0, 0);*/
    
    //
    
    /*
     */
    goto __lv_end;
    
    MEMORY_SEARCH_HEADER(VM_MEMORY_IOACCELERATOR)
    for (long i = 0 ; i < (size - 8) ; i += 8)
    {
        vm_address_t a = *((vm_address_t*)(read_data + i));
        if (a > task_base_address)
        {
            uint32_t b = *((uint32_t*)(read_data + i + 4));
            if (b == 1)
            {
                vm_address_t pv = address + i - 0x10;
                uint32_t type = *((uint32_t*)(read_data + i - 0x10 + 12));
                //uint32_t type_sig = *((uint32_t*)(read_data + i - 0x10 + 8));
                if (type > 0 && type < 13)
                {
                    uint16_t a_sig = vm_read_uint16_t_value(task, a + 6);
                    if (a_sig == 0xffff)
                    {
                        char nm[100]; bzero(nm, sizeof(nm)); long nm_l = 0;
                        rbx_local_variable_get_string_value(task, a, nm, &nm_l);
                        if (strstr(nm, "Almond"))
                        {
                            rbx_print_local_variable(task, pv, 0, NULL);
                            //printf("variable: %p, name: %s, type : %d\n", pv, nm, type);
                        }
                    }
                }
            }
        }
    }
    MEMORY_SEARCH_FOOTER
        
    __lv_end:{};
    
    
    //goto __loop_end;

    for (;;)
    {
        printf("\n-- CHECKS --\n");
        printf("My health: %f\n", rbx_humanoid_get_health(task, my_humanoid));
        printf("hrp velocity addr: %p\n", rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_VELOCITY_OFFSET);
        vm_address_t my_seatpart = rbx_humanoid_get_seatpart(task, my_humanoid);
        if (my_seatpart)
        {
            printf("My seatpart: ");
            rbx_print_descendants(task, rbx_humanoid_get_seatpart(task, my_humanoid), 0, 0);
        }
        printf("Player last input timestamp: %f\n", rbx_player_get_last_input_timestamp(task, local_player));
        printf("what game points to %p\n", vm_read_uint64_t_value(task, game));
        kr = vm_read(task, local_player, 0x1000, &read_data, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            for (int i = 0 ; i < 0x1000/8 ; i++)
            {
                double a = ((double*)read_data)[i];
                if (99999 > a && a > 10)
                {
                    //printf("last_input_timestamp: [PLAYER + %p] = %f\n", i * 8, a);
                }
            }
            vm_deallocate(mach_task_self_, read_data, 0x1000);
        }
        kr = vm_read(task, my_humanoid, 0x1000, &read_data, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            for (int i = 0 ; i < 0x1000/4 ; i++)
            {
                float a = ((float*)read_data)[i];
                if (99999 > a && a > 10)
                {
                    //printf("[HUMANOID + %p] = %f\n", i * 4, a);
                }
            }
            vm_deallocate(mach_task_self_, read_data, 0x1000);
        }
        sleep(1);
    }
    
    __loop_end: {};
    
    //sleep(1000);
    
    /*vm_address_t t = rbx_instance_find_first_child_of_class(task, my_character, "Tool");
    
    shared_memory->FUNCTION_USLEEP_TIME = 100000;
    shared_memory->FUNCTION_COUNT = 10;

    shared_memory->FUNCTION_QUEUE[2].address = task_base_address + 0x01c0cfb4;
    shared_memory->FUNCTION_QUEUE[2].type = 2;
    shared_memory->FUNCTION_QUEUE[2].async_type = FUNCTION_ASYNC_TYPE_MAIN;
    
    vm_address_t s = vm_allocate_rwx(task, NULL, 100);
    char args[48];
    bzero(args, sizeof(args));
    memcpy(args, &initiateboom_re, 8);
    memcpy(args + 8, &s, 8);
    
    memcpy(shared_memory->FUNCTION_QUEUE[2].arguments, args, 48);
    //printf("%p(%p, %p)\n", task_base_address + RBX_INSTANCE_SET_PARENT_METHOD_OFFSET, t, workspace);
    shared_memory->FUNCTION_QUEUE[2].should_execute = true;
    
    while (shared_memory->FUNCTION_QUEUE[2].should_execute)
    {
        usleep(1000);
    }
    printf("return: %p\n", *(long*)shared_memory->FUNCTION_QUEUE[2].return_bytes);*/
    
   // printf("%p, %p\n", t, workspace);
}







void find_function_offsets(task_t task)
{
    vm_address_t root_table =  rbx_find_root_table(task); //rbx_find_root_table(task);
    printf("<<<ROOT>>>: %p\n", (void*)root_table);
    
    printf("int %s = %p;\n", "RBX_TOOL_ACTIVATE_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Tool", "Activate", RBX_METHOD_TYPE_NEITHER));
    printf("int %s = %p;\n", "RBX_HUMANOID_HEALTH_GETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "Health", RBX_METHOD_TYPE_GETTER));
    printf("int %s = %p;\n", "RBX_HUMANOID_HEALTH_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "Health", RBX_METHOD_TYPE_SETTER));
    printf("int %s = %p;\n", "RBX_HUMANOID_WALKSPEED_GETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "WalkSpeed", RBX_METHOD_TYPE_GETTER));
    printf("int %s = %p;\n", "RBX_HUMANOID_WALKSPEED_SETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "WalkSpeed", RBX_METHOD_TYPE_SETTER));
    printf("int %s = %p;\n", "RBX_INSTANCE_SET_PARENT_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Instance", "Parent", RBX_METHOD_TYPE_SETTER));
    printf("int %s = %p;\n", "RBX_HUMANOID_RIGTYPE_GETTER_METHOD_OFFSET", (void*)rbx_find_method_offset(task, root_table, "Humanoid", "RigType", RBX_METHOD_TYPE_GETTER));
}

