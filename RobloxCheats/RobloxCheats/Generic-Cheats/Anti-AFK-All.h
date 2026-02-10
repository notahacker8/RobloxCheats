

void anti_afk_cheat(task_t task)
{
    GENERIC_CHEAT_STARTER
    ANTI_AFK
    EXIT_IF_TASK_TERMINATED
    rbx_print_descendants(task, local_player, 0, 0);
}

#pragma mark - ANTI AFK ALL -


//Anti AFK for all roblox processes
void anti_afk_all(void)
{
    static task_t tasks[100];
    static vm_address_t addys[100];
    static int task_count = 0;
    static char should_update = true;
    static char auto_update = false;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (should_update)
            {
                pid_t pids[4096];
                int pid_count = pids_by_name("RobloxPlayer", pids);
                
                int t_idx = 0;
                COPY_BUF(tasks);
                for (int i = 0 ; i < pid_count ; i++)
                {
                    task_t task;
                    if (task_for_pid(mach_task_self(), pids[i], &task) == KERN_SUCCESS)
                    {
                        vm_address_t game = rbx_find_game_address(task);
                        vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
                        vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
                        rbx_print_descendants(task, local_player, 0, 0);
                        addys[t_idx] = local_player;
                        tasks[t_idx] = task;
                        t_idx++;
                    }
                }
                task_count = t_idx;
                should_update = false;
                printf("updated tasks and addresses\n");
            }
            else
            {
                sleep(1);
            }
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            for (int i = 0 ; i < task_count ; i++)
            {
                if (rbx_instance_is_valid_child(tasks[i], addys[i]) && rbx_instance_is_a(tasks[i], addys[i], "Player"))
                {
                    rbx_player_set_last_input_timestamp(tasks[i], addys[i], 999999);
                }
                else
                {
                    addys[i] = 0;
                }
            }
            sleep(3);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (auto_update)
            {
                should_update = true;
                sleep(60 * 20);
            }
            sleep(1);
        }
    });
    
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("update", "", should_update = true;)
    })
}
