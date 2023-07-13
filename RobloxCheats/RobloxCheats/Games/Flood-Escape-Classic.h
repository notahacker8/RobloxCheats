


void flood_escape_classic_cheat(void)
{
    task_t task = get_roblox_task_port();
    printf("- FLOOD ESCAPE (GRAVITY) -\n");
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players, "Player");
    
    for (;;)
    {
        vm_address_t character = rbx_player_get_character(task, local_player);
        vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
        float new_gravity = 30;
        rbx_basepart_set_gravity(task, hrp, new_gravity);
        usleep(500000);
    }
    
}
