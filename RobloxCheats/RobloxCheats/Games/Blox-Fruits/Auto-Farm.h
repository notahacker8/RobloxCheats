

vm_address_t blox_fruits_find_first_alive_enemy_by_name(task_t task,
                                                        vm_address_t enemies_folder,
                                                        char* name,
                                                        char is_partial_name)
{
    vm_address_t __enemy = 0;
    long cc = 0;
    rbx_child_t* children = rbx_instance_get_children(task, enemies_folder, &cc);
    if (children)
    {
        for (long i = 0 ; i < cc ; i++)
        {
            vm_address_t enemy = children[i].child_address;
            long enl = 0;
            char* en = rbx_instance_get_name(task, enemy, &enl);
            if (en)
            {
                char is__ = false;
                if (is_partial_name)
                {
                    if (strstr(en, name))
                    {
                        is__ = true;
                    }
                }
                else
                {
                    if (strcmp(en, name) == 0)
                    {
                        is__ = true;
                    }
                }
                if (is__)
                {
                    vm_address_t head = rbx_instance_find_first_child(task, enemy, "Head");
                    vm_address_t obj = rbx_instance_find_first_child_of_class(task, head, "ParticleEmitter");
                    if (obj == 0)
                    {
                        __enemy = enemy;
                        i = cc;
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)en, enl);
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, cc * sizeof(rbx_child_t));
    }
    return __enemy;
}



void blox_fruits_auto_farm(task_t task,
                           char* enemy_name,
                           char* quest_giver_name,
                           float y_offset,
                           char tool_key,
                           char should_do_quest,
                           char auto_click,
                           char is_partial_name,
                           char use_z,
                           char use_x,
                           char use_c,
                           char use_v,
                           char use_f)
{
    printf("- BLOX FRUITS (AUTO FARM) -\n");
    
    LIBESP_DYLIB_SETUP
    
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t player_gui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
    vm_address_t main_screengui = rbx_instance_find_first_child(task, player_gui, "Main");
    vm_address_t quest_frame = rbx_instance_find_first_child(task, main_screengui, "Quest");
    rbx_print_descendants(task, quest_frame, 0, 0);
    //vm_address_t badgeservice = rbx_instance_find_first_child_of_class(task, game, "BadgeService");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    //vm_address_t lighting_service = rbx_instance_find_first_child_of_class(task, game, "Lighting");
    //vm_address_t replicatedstorage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t npcs_folder = rbx_instance_find_first_child(task, workspace, "NPCs");
    vm_address_t enemies_folder = rbx_instance_find_first_child(task, workspace, "Enemies");
    rbx_print_descendants(task, enemies_folder, 0, 1);
    
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    static vm_address_t target_enemy = 0;
    static vm_address_t target_enemy_hrp = 0;
    static vm_address_t quest_giver = 0;
    static vm_address_t quest_giver_part = 0;
    static vector3_t new_pos;
    
    static char should_write_cframe = false;
    static char is_too_far = false;
    static float max_teleport_range = 80;
    
    shared_memory->INPUT_QUEUE_COUNT = 7;
    shared_memory->INPUT_USLEEP_TIME = 100000;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            my_character = rbx_player_get_character(task, local_player);
            vm_address_t tool = rbx_instance_find_first_child_of_class(task, my_character, "Tool");
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            target_enemy = blox_fruits_find_first_alive_enemy_by_name(task, enemies_folder, enemy_name, is_partial_name);
            rbx_print_descendants(task, npcs_folder, 0, 1);
            quest_giver = rbx_instance_find_first_child(task, npcs_folder, quest_giver_name);
            quest_giver_part = rbx_instance_find_first_child(task, quest_giver, "Head");
            //target_enemy = blox_fruits_find_first_alive_enemy_in_range(task, enemies_folder, my_hrp, 150);
            target_enemy_hrp = rbx_instance_find_first_child(task, target_enemy, "HumanoidRootPart");
            if (target_enemy_hrp)
            {
                float sz = 30;
                rbx_basepart_set_size(task, target_enemy_hrp, (vector3_t){.x = sz,. y = sz, .z = sz});
            }
            my_hrp_cframe_address = rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            
            if (!tool)
            {
                shared_memory->INPUT_QUEUE[6] = (Input){.type = INPUT_TYPE_KEY_DOWN, .should_execute = true, .keycode = keycode_for_character(tool_key)};
                shared_memory->INPUT_QUEUE_FINISHED = false;
                while (shared_memory->INPUT_QUEUE_FINISHED == false) {usleep(10000);}
                shared_memory->INPUT_QUEUE[6] = (Input){.type = INPUT_TYPE_KEY_UP, .should_execute = true, .keycode = keycode_for_character(tool_key)};
                shared_memory->INPUT_QUEUE_FINISHED = false;
                while (shared_memory->INPUT_QUEUE_FINISHED == false) {usleep(10000);}
            }
            
            while (shared_memory->INPUT_QUEUE_FINISHED == false) {usleep(10000);}
            
            if (auto_click == true)
            {
                shared_memory->INPUT_QUEUE[0] = (Input){.type = INPUT_TYPE_LEFT_MOUSE_DOWN, .should_execute = true, .x = shared_memory->MOUSE_X, .y = shared_memory->MOUSE_Y};
            }
            
            if (use_z) { shared_memory->INPUT_QUEUE[1] = (Input){.type = INPUT_TYPE_KEY_DOWN, .should_execute = true, .keycode = keycode_for_character('z')}; }
            if (use_x) { shared_memory->INPUT_QUEUE[2] = (Input){.type = INPUT_TYPE_KEY_DOWN, .should_execute = true, .keycode = keycode_for_character('x')}; }
            if (use_c) { shared_memory->INPUT_QUEUE[3] = (Input){.type = INPUT_TYPE_KEY_DOWN, .should_execute = true, .keycode = keycode_for_character('c')}; }
            if (use_v) { shared_memory->INPUT_QUEUE[4] = (Input){.type = INPUT_TYPE_KEY_DOWN, .should_execute = true, .keycode = keycode_for_character('v')}; }
            if (use_f) { shared_memory->INPUT_QUEUE[5] = (Input){.type = INPUT_TYPE_KEY_DOWN, .should_execute = true, .keycode = keycode_for_character('f')}; }
            shared_memory->INPUT_QUEUE_FINISHED = false;
            
            usleep(10000);
            
            while (shared_memory->INPUT_QUEUE_FINISHED == false) {usleep(1000);}
            if (auto_click == true)
            {
            shared_memory->INPUT_QUEUE[0] = (Input){.type = INPUT_TYPE_LEFT_MOUSE_UP, .should_execute = true, .x = shared_memory->MOUSE_X, .y = shared_memory->MOUSE_Y};
            }
            
            if (use_z) { shared_memory->INPUT_QUEUE[1] = (Input){.type = INPUT_TYPE_KEY_UP, .should_execute = true, .keycode = keycode_for_character('z')}; }
            if (use_x) { shared_memory->INPUT_QUEUE[2] = (Input){.type = INPUT_TYPE_KEY_UP, .should_execute = true, .keycode = keycode_for_character('x')}; }
            if (use_c) { shared_memory->INPUT_QUEUE[3] = (Input){.type = INPUT_TYPE_KEY_UP, .should_execute = true, .keycode = keycode_for_character('c')}; }
            if (use_v) { shared_memory->INPUT_QUEUE[4] = (Input){.type = INPUT_TYPE_KEY_UP, .should_execute = true, .keycode = keycode_for_character('v')}; }
            if (use_f) { shared_memory->INPUT_QUEUE[5] = (Input){.type = INPUT_TYPE_KEY_UP, .should_execute = true, .keycode = keycode_for_character('f')}; }
            
            shared_memory->INPUT_QUEUE_FINISHED = false;
            while (shared_memory->INPUT_QUEUE_FINISHED == false) {usleep(10000);}
            
            usleep(100000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            char __swcf = false;
            rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
            if (should_do_quest)
            {
                if (rbx_frame_get_visible(task, quest_frame) == true)
                {
                    //rbx_print_descendants(task, target_enemy_hrp, 0, 0);
                    if (target_enemy_hrp)
                    {
                        rbx_cframe_t target_hrp_cframe = rbx_basepart_get_cframe(task, target_enemy_hrp);
                        vector3_t __new_pos = vector3_add(target_hrp_cframe.pos, (vector3_t){.x = 0, .y = y_offset,.z = 0});
                        if (vector3_dist_dif(__new_pos, my_hrp_cframe.pos) > max_teleport_range)
                        {
                            __new_pos = vector3_add(my_hrp_cframe.pos, vector3_mul_num(vector3_unit(target_hrp_cframe.pos, my_hrp_cframe.pos), max_teleport_range));
                        }
                        new_pos = __new_pos;
                        __swcf = true;
                    }
                }
                else
                {
                    rbx_print_descendants(task, quest_giver_part, 0, 0);
                    if (quest_giver_part)
                    {
                        rbx_cframe_t target_cframe = rbx_basepart_get_cframe(task, quest_giver_part);
                        vector3_t __new_pos = target_cframe.pos;
                        if (vector3_dist_dif(__new_pos, my_hrp_cframe.pos) > max_teleport_range)
                        {
                            __new_pos = vector3_add(my_hrp_cframe.pos, vector3_mul_num(vector3_unit(target_cframe.pos, my_hrp_cframe.pos), max_teleport_range));
                        }
                        new_pos = __new_pos;
                        __swcf = true;
                    }
                }
            }
            else
            {
                if (target_enemy_hrp)
                {
                    if (rbx_instance_get_self(task, target_enemy_hrp) == target_enemy_hrp &&
                        rbx_instance_get_parent(task, target_enemy_hrp) != 0)
                    {
                        rbx_cframe_t target_hrp_cframe = rbx_basepart_get_cframe(task, target_enemy_hrp);
                        vector3_t __new_pos = vector3_add(target_hrp_cframe.pos, (vector3_t){.x = 0, .y = y_offset,.z = 0});
                        if (vector3_dist_dif(__new_pos, my_hrp_cframe.pos) > max_teleport_range)
                        {
                            __new_pos = vector3_add(my_hrp_cframe.pos, vector3_mul_num(vector3_unit(target_hrp_cframe.pos, my_hrp_cframe.pos), max_teleport_range));
                            is_too_far = true;
                        }
                        new_pos = __new_pos;
                        __swcf = true;
                    }
                }
            }
            should_write_cframe = __swcf;
            if (is_too_far)
            {
                is_too_far = false;
                usleep(300000);
            }
            usleep(2000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (should_write_cframe)
            {
                vm_write(task, my_hrp_cframe_address + 0x24, (vm_offset_t)&new_pos, 12);
                usleep(8);
            }
            else
            {
                usleep(1000);
            }
        }
    });
    
    
}
