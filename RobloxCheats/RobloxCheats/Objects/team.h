

int rbx_team_get_brick_color(task_t task, vm_address_t team)
{
    return vm_read_4byte_value(task, team + RBX_TEAM_BRICKCOLOR_OFFSET);
}
