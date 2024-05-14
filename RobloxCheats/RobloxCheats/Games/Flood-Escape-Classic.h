


void gravity_cheat(task_t task)
{
    printf("\n- GENERIC CHEAT FOR GENERAL USE -\n");
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
            //rbx_print_descendants(task, my_character, 0, 1);
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
            COPY_VAR(my_hrp)
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
                
                rbx_basepart_set_velocity(task, __my_hrp, vel);
                rbx_basepart_set_gravity(task, __my_hrp, 0.0f);
            }
            else
            {
                rbx_basepart_set_gravity(task, __my_hrp, new_gravity);
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
    printf("\tcamcoords\n");
    printf("\tmousecoords\n");
    //printf("\tgameinfo\n");
    printf("\tplaceid\n");
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
                printf("PRINTING PLAYER COORDINATES...\n");
                if (my_hrp)
                {
                    vector3_t pos = rbx_basepart_get_cframe(task, my_hrp).pos;
                    printf("vector3_t pos = {\n\t.x = %f,\n\t.y = %f,\n\t.z = %f\n};\n", pos.x, pos.y, pos.z);
                }
                else
                {
                    printf("ERROR: HUMANOID ROOT PART NOT FOUND\n");
                }
            }
            if (strcmp(str, "mousecoords\xa") == 0)
            {
                printf("PRINTING MOUSE COORDINATES...\n");
                if (my_hrp)
                {
                    printf("%f, %f\n", shared_memory->MOUSE_X, shared_memory->MOUSE_Y);
                }
                else
                {
                    printf("ERROR: HUMANOID ROOT PART NOT FOUND\n");
                }
            }
            if (strcmp(str, "camcoords\xa") == 0)
            {
                printf("PRINTING CAMERA COORDINATES...\n");
                if (camera)
                {
                    rbx_cframe_t cframe = rbx_camera_get_cframe(task, camera);
                    printf("rbx_cframe_t cframe = {\n\t.r0 = %f, .r10 = %f, .r20 = %f,\n\t.r1 = %f, .r11 = %f, .r21 = %f,\n\t.r2 = %f, .r12 = %f, .r22 = %f,\n\t.pos = {.x = %f, .y = %f, .z = %f}\n};\n", cframe.r0, cframe.r10, cframe.r20, cframe.r1, cframe.r11, cframe.r21, cframe.r2, cframe.r12, cframe.r22, cframe.pos.x, cframe.pos.y, cframe.pos.z);
                }
                else
                {
                    printf("ERROR: CAMERA NOT FOUND\n");
                }
            }
            if (strcmp(str, "placeid\xa") == 0)
            {
                printf("PlaceId: %ld\n", rbx_get_placeid(task));
                long len = 0;
                char* jobid = rbx_datamodel_get_jobid(task, game, &len);
                if (jobid)
                {
                    printf("JobId: %s\n", jobid);
                    vm_deallocate(mach_task_self_, (vm_address_t)jobid, len);
                }
            }
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
}//-- MIRAGE GEAR --, 3.236792, 0.443407, 3.236792
