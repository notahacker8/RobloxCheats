


void flood_escape_classic_cheat(task_t task)
{
    printf("- FLOOD ESCAPE -\n");
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;) { pid_t a[4096]; if (pids_by_name("RobloxPlayer", a) == 0) { exit(0); } sleep(1); }
    });
    
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
