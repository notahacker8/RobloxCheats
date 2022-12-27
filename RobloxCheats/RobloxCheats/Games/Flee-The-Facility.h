
void flee_the_facility_hack(task_t task)
{
    printf("- FLEE THE FACILITY -\n");
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    rbx_print_descendants(task, workspace, 0, 1);
    printf("%p\n", rbx_instance_find_first_child_of_class_and_name(task, workspace, "Model", ""));
    
}
