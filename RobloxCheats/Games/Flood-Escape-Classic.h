


void flood_escape_class_cheat(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t players = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players, "Player");
    char player_name_len;
    char* player_name = rbx_instance_get_name(task, local_player, &player_name_len);
    
    for (;;)
    {
        vm_address_t character = rbx_instance_find_first_child(task, workspace, player_name);
        vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
        float new_gravity = 90;
        rbx_basepart_set_gravity(task, hrp, new_gravity);
        sleep(1);
    }
}
