


void assassin_hack(task_t task)
{
    
    static mach_msg_type_number_t data_cnt;
    
    void* handle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
    vm_offset_t left_mouse_down_offset = gdso(handle, s_load_address, "LEFT_MOUSE_DOWN");
    
    dlclose(handle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB__);
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t replicated_storage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    vm_address_t client_round_in_progress_bool_value = rbx_instance_find_first_child(task, replicated_storage, "ClientRoundInProgress");
    
    
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t player_gui = rbx_instance_find_first_child(task, local_player, "PlayerGui");
    vm_address_t screen_gui = rbx_instance_find_first_child(task, player_gui, "ScreenGui");
    vm_address_t ui = rbx_instance_find_first_child(task, screen_gui, "UI");
    vm_address_t target_frame = rbx_instance_find_first_child(task, ui, "Target");
    vm_address_t target_text = rbx_instance_find_first_child(task, target_frame, "TargetText");
    vm_address_t target_shadow_text = rbx_instance_find_first_child(task, target_text, "ShadowText");
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            vm_address_t read_data;
            char round_in_progress = rbx_boolvalue_get_value(task, client_round_in_progress_bool_value);
            kern_return_t kr = vm_read(task, load_address + left_mouse_down_offset, 1, (vm_offset_t*)&read_data, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                char lmd = *(char*)read_data;
                vm_deallocate(mach_task_self_, read_data, 1);
                if (round_in_progress == true && lmd == true)
                {
                    char len;
                    char* target_name = rbx_textlabel_get_short_text(task, target_shadow_text, &len);
                    if (target_name)
                    {
                        vm_address_t target_character = rbx_instance_find_first_child(task, workspace, target_name);
                        vm_address_t target_hrp = rbx_instance_find_first_child(task, target_character, "Torso");
                        vm_address_t my_character = rbx_player_get_character(task, local_player);
                        vm_address_t my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
                        if (target_hrp && my_hrp)
                        {
                            rbx_cframe_t target_cframe = rbx_basepart_get_cframe(task, target_hrp);
                            vector3_t lv = rbx_get_cframe_look_vector(target_cframe);
                            float dist = 1;
                            target_cframe.pos.x -= lv.x * dist;
                            target_cframe.pos.y -= lv.y * dist;
                            target_cframe.pos.y -= 1;
                            target_cframe.pos.z -= lv.z * dist;
                            rbx_basepart_set_gravity(task, my_hrp, 0);
                            rbx_basepart_set_cframe(task, my_hrp, &target_cframe);
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)target_name, len);
                    }
                }
            }
            usleep(100);
        }
    });
}
