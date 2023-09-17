


void gravity_cheat(task_t task)
{
    printf("- GRAVITY CHEAT FOR GENERAL USE -\n");
    LIBESP_DYLIB_SETUP
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players, "Player");
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    
    static char fly_enabled = false;
    static float fly_speed = 50;
    
    
    static float new_gravity = 30;
    
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            my_character = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            //rbx_print_descendants(task, my_character, 0, 1);
            //rbx_basepart_set_gravity(task, my_hrp, new_gravity);
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(500);
            if (fly_enabled)
            {
                vector3_t vel;
                rbx_cframe_t cf = rbx_camera_get_cframe(task, camera);
                vector3_t lv = rbx_cframe_get_look_vector(cf);
                vector3_t rv = rbx_cframe_get_right_vector(cf);
                
                bzero(&vel, sizeof(vector3_t));
                
                unsigned char* keys_down = shared_memory->KEYS_DOWN;
                
                if (keys_down['w']) { vel = vector3_add(vector3_mul_num(lv, 1), vel); }
                if (keys_down['a']) { vel = vector3_add(vector3_mul_num(rv, -1), vel); }
                if (keys_down['s']) { vel = vector3_add(vector3_mul_num(lv, -1), vel); }
                if (keys_down['d']) { vel = vector3_add(vector3_mul_num(rv, 1), vel); }
                
                float magnitude = vector3_magnitude(vel);
                if (!isnan(magnitude) && magnitude > 0.0f)
                {
                    vel = vector3_div_num(vel,  magnitude);
                }
                vel = vector3_mul_num(vel, fly_speed);
                
                rbx_basepart_set_velocity(task, my_hrp, vel);
                rbx_basepart_set_gravity(task, my_hrp, 0.0f);
            }
            else
            {
                rbx_basepart_set_gravity(task, my_hrp, new_gravity);
            }
        }
    });
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    
    printf("List of stdin commands:\n");
    printf("\tfly\n");
    printf("\tflyspeed [(float)]\n");
    printf("\tnofly\n");
    printf("\tgrav [(float)]\n");
    printf("\tcoords\n");
    printf("\tquit\n");
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            char str[100];
            bzero(str, 100);
            fgets(str, 100, stdin);
            if (strcmp(str, "fly\xa") == 0)
            {
                rbx_smooth_fly(task, true);
                fly_enabled = true;
                printf("ENABLED FLY\n");
            }
            if (strncmp(str, "flyspeed ", strlen("flyspeed ")) == 0)
            {
                sscanf(str, "flyspeed %f", &fly_speed);
                printf("SET FLY SPEED TO %f\n", fly_speed);
            }
            if (strcmp(str, "nofly\xa") == 0)
            {
                rbx_smooth_fly(task, false);
                fly_enabled = false;
                printf("DISABLED FLY\n");
            }
            if (strncmp(str, "grav ", strlen("grav ")) == 0)
            {
                sscanf(str, "grav %f", &new_gravity);
                printf("GRAVITY SET TO %f\n", new_gravity);
            }
            if (strcmp(str, "coords\xa") == 0)
            {
                printf("PRINTING COORDINATES...\n");
                if (my_hrp)
                {
                    vector3_t pos = rbx_basepart_get_cframe(task, my_hrp).pos;
                    printf("\tx: %f\n\ty: %f\n\tz: %f\n", pos.x, pos.y, pos.z);
                }
                else
                {
                    printf("ERROR: HUMANOID ROOT PART NOT FOUND\n");
                }
            }
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
}
