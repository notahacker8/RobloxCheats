

void field_of_battle_general_killer(task_t task)
{
    LIBESP_DYLIB_SETUP
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t characters_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Characters");
    
    static vm_address_t my_character = 0;
    
    static vm_address_t general_torso = 0;
    static vm_address_t general_torso_cframe_address = 0;
    static vm_address_t handle = 0;
    static vm_address_t handle_cframe_address = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    
    static vector3_t new_hrp_pos;
    
    vm_address_t orc_char_folder = rbx_instance_find_first_child(task, characters_folder, "Orc");
    vm_address_t human_char_folder = rbx_instance_find_first_child(task, characters_folder, "Human");
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            my_character = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            rbx_basepart_set_gravity(task, my_hrp, 30);
            my_hrp_cframe_address = rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            vm_address_t my_char_folder = rbx_instance_get_parent(task, my_character);
            vm_address_t general = 0;
            if (my_char_folder == human_char_folder)
            {
                general = rbx_instance_find_first_child(task, orc_char_folder, "Orc General");
            }
            if (my_char_folder == orc_char_folder)
            {
                general = rbx_instance_find_first_child(task, human_char_folder, "Human General");
            }
            vm_address_t tool = rbx_instance_find_first_child_of_class(task, my_character, "Tool");
            handle = rbx_instance_find_first_child(task, tool, "Handle");
            handle_cframe_address = rbx_basepart_get_properties_address(task, handle) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            general_torso = rbx_instance_find_first_child(task, general, "Torso");
            general_torso_cframe_address = rbx_basepart_get_properties_address(task, general_torso) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            rbx_camera_set_camera_subject(task, camera, general);
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            new_hrp_pos = vector3_add((vector3_t){.x = 0, .y = 40, .z = 0}, rbx_basepart_get_cframe(task, general_torso).pos);
            usleep(10000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(10);
            if (general_torso && handle && shared_memory->LEFT_MOUSE_DOWN == true)
            {
                for (int i = 0 ; i < 10000 ; i++)
                {
                vm_copy(task, general_torso_cframe_address, sizeof(rbx_cframe_t), handle_cframe_address);
                }
            }
            if (my_hrp)
            {
                //vm_write(task, my_hrp_cframe_address + 0x24, (vm_address_t)&new_hrp_pos, 12);
            }
        }
    });
    
    
}
