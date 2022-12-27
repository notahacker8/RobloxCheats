

void jailbreak_cheat(task_t task)
{
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static vm_address_t my_char = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t target_hrp = 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            my_char = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_char, "HumanoidRootPart");
            long player_count = 0;
            rbx_child_t* player_list = rbx_instance_get_children(task, players_service, &player_count);
            for (int i = 0 ; i < player_count ; i++)
            {
                vm_address_t player = player_list[i].child_address;
                if (player != local_player)
                {
                    vm_address_t character = rbx_player_get_character(task, player);
                    vm_address_t head = rbx_instance_find_first_child(task, character, "Head");
                    long hcc = 0;
                    rbx_child_t* head_children = rbx_instance_get_children(task, head, &hcc);
                    if (hcc == 10)
                    {
                        vm_address_t ff = rbx_instance_find_first_child(task, character, "ForceField");
                        if (ff == 0)
                        {
                            vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
                            //rbx_print_descendants(task, character, 0, 2);
                            target_hrp = hrp;
                            i = (int)player_count;
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)head_children, hcc * sizeof(rbx_child_t));
                }
            }
            vm_deallocate(mach_task_self_, (vm_address_t)player_list, player_count * sizeof(rbx_child_t));
            usleep(100000);
        }
    });
    for (;;)
    {
        usleep(100);
        rbx_basepart_set_gravity(task, my_hrp, 0);
        rbx_cframe_t my_cf = rbx_basepart_get_cframe(task, my_hrp);
        //printf("%f %f %f\n", my_cf.pos.x,  my_cf.pos.y,  my_cf.pos.z);
        rbx_cframe_t cf = rbx_basepart_get_cframe(task, target_hrp);
        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
        vector3_t lv = rbx_get_cframe_look_vector(camera_cframe);
        vector3_t offset = vector3_mul_num(lv, 10);
        my_cf.pos = vector3_sub(cf.pos, offset);
        rbx_basepart_set_cframe(task, my_hrp, &cf);
    }
    
}
