

vm_address_t rbx_player_get_team(task_t task, vm_address_t player)
{
    return vm_read_8byte_value(task, player + RBX_PLAYER_TEAM_OFFSET);
}

vm_address_t rbx_player_get_character(task_t task, vm_address_t player)
{
    return vm_read_8byte_value(task, player + RBX_PLAYER_CHARACTER_OFFSET);
}

void rbx_player_set_character(task_t task, vm_address_t player, vm_address_t new_character)
{
    vm_write(task, player + RBX_PLAYER_CHARACTER_OFFSET, (vm_offset_t)&new_character, sizeof(vm_address_t));
}

char* rbx_player_get_displayname(task_t task, vm_address_t player, long* inout_len)
{
    return vm_read_rbx_string(task, player + RBX_PLAYER_DISPLAYNAME_OFFSET, inout_len);
}

void rbx_player_set_last_input_timestamp(task_t task, vm_address_t player, double new_value)
{
    vm_write(task, player + RBX_PLAYER_LAST_INPUT_TIMESTAMP_OFFSET, (vm_offset_t)&new_value, sizeof(double));
}




vm_address_t rbx_players_get_player_by_partial_name(task_t task, vm_address_t players_service, char* partial_name)
{
    vm_address_t found_player = 0;
    long pnl = strlen(partial_name);
    long child_count = 0;
    rbx_child_t* children = rbx_instance_get_children(task, players_service, &child_count);
    if (child_count)
    {
        for (long i = 0 ; i < child_count ; i++)
        {
            vm_address_t player = children[i].child_address;
            if (player)
            {
                long player_name_len = 0;
                char* player_name = rbx_instance_get_name(task, player, &player_name_len);
                if (player_name)
                {
                    if (strncmp(player_name, partial_name, pnl) == 0)
                    {
                        found_player = player;
                        i = child_count;
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)player_name, player_name_len);
                }
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));
    }
    return found_player;
}
