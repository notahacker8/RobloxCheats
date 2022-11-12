



char tds_game_did_end(task_t task,
                      vm_address_t current_health,
                      vm_address_t time_int_value)
{
    long health = rbx_intvalue_get_value(task, current_health);
    long _time = rbx_intvalue_get_value(task, time_int_value);
    if (health < 1 && _time > 99000)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int tds_get_cash(task_t task, vm_address_t cash_amount_text_label)
{
    char len = 0;
    int cash = 0;
    char* text = rbx_textlabel_get_short_text(task, cash_amount_text_label, &len);
    char* fixed_text = malloc(len);
    int _i = 0;
    for (int i = 0 ; i < len ; i++)
    {
        char c = text[i];
        if (c > 0x2f && c < 0x3a)
        {
            fixed_text[_i] = c;
            _i++;
        }
    }
    vm_deallocate(mach_task_self_, (vm_address_t)text, len);
    cash = atoi(fixed_text);
    free(fixed_text);
    return cash;
}

int tds_get_wave(task_t task, vm_address_t wave_text_label)
{
    char len = 0;
    int wave = 0;
    char* text = rbx_textlabel_get_short_text(task, wave_text_label, &len);
    wave = atoi(text + 5);
    vm_deallocate(mach_task_self_, (vm_address_t)text, len);
    return wave;
}


void tds_wait_until_game_over(task_t task,
                              vm_address_t time_int_value,
                              vm_address_t current_health)
{
    for (;;)
    {
        char game_is_over = tds_game_did_end(task, current_health, time_int_value);
        if (game_is_over == true)
        {
            return;
        }
        else
        {
            sleep(1);
        }
    }
}

void tds_wait_until_skip_enabled(task_t task,
                                 vm_address_t voting_enabled,
                                 vm_address_t time_int_value,
                                 vm_address_t current_health)
{
    for (;;)
    {
        char game_is_over = tds_game_did_end(task, current_health, time_int_value);
        char status = rbx_boolvalue_get_value(task, voting_enabled);
        if (game_is_over == true)
        {
            return;
        }
        if (status == true)
        {
            return;
        }
        else
        {
            usleep(300000);
        }
    }
}

void tds_wait_until_cash(task_t task,
                         vm_address_t cash_amount_text_label,
                         vm_address_t time_int_value,
                         vm_address_t current_health,
                         int cash_to_reach)
{
    for (;;)
    {
        int cash = tds_get_cash(task, cash_amount_text_label);
        char game_is_over = tds_game_did_end(task, current_health, time_int_value);
        if (game_is_over == true)
        {
            return;
        }
        if (cash >= cash_to_reach)
        {
            return;
        }
        else
        {
            usleep(300000);
        }
    }
}

void tds_skip(task_t task,
              vm_address_t load_address,
              vm_address_t input_queue_offset,
              vm_address_t input_queue_count_offset,
              vm_address_t input_queue_finished_offset)
{
    Input inputs[3];
    inputs[0].type = 0;
    inputs[0].duration = 0;
    inputs[0].window_index = 0;
    inputs[0].x = 362;
    inputs[0].y = 461;
    
    inputs[1].type = 3;
    inputs[1].duration = 1000;
    inputs[1].window_index = 0;
    inputs[1].x = 362;
    inputs[1].y = 461;
    
    inputs[2].type = 4;
    inputs[2].duration = 0;
    inputs[2].window_index = 0;
    inputs[2].x = 362;
    inputs[2].y = 461;
    
    int c = 3;
    vm_write(task, load_address + input_queue_offset, inputs, sizeof(Input) * c);
    char f = 0;
    vm_write(task, load_address + input_queue_count_offset, &c, 4);
    vm_write(task, load_address + input_queue_finished_offset, &f, 1);
}

void tds_wait_until_cash_while_skipping(task_t task,
                                        vm_address_t load_address,
                                        vm_address_t input_queue_offset,
                                        vm_address_t input_queue_count_offset,
                                        vm_address_t input_queue_finished_offset,
                                        vm_address_t cash_amount_text_label,
                                        vm_address_t time_int_value,
                                        vm_address_t current_health,
                                        vm_address_t voting_enabled,
                                        int cash_to_reach)
{
    for (;;)
    {
        char can_vote = rbx_boolvalue_get_value(task, voting_enabled);
        int cash = tds_get_cash(task, cash_amount_text_label);
        char game_is_over = tds_game_did_end(task, current_health, time_int_value);
        if (game_is_over == true)
        {
            return;
        }
        if (can_vote == true)
        {
            wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 10000);
            tds_skip(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset);
            wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 10000);
        }
        printf("%d\n", cash);
        if (cash >= cash_to_reach)
        {
            return;
        }
        else
        {
            usleep(300000);
        }
    }
}


void tds_vote_molten(task_t task,
                     vm_address_t load_address,
                     vm_address_t input_queue_offset,
                     vm_address_t input_queue_count_offset,
                     vm_address_t input_queue_finished_offset)
{
    Input inputs[3];
    inputs[0].type = 0;
    inputs[0].duration = 0;
    inputs[0].window_index = 0;
    inputs[0].x = 400;
    inputs[0].y = 270;
    
    inputs[1].type = 3;
    inputs[1].duration = 1000;
    inputs[1].window_index = 0;
    inputs[1].x = 400;
    inputs[1].y = 270;
    
    inputs[2].type = 4;
    inputs[2].duration = 0;
    inputs[2].window_index = 0;
    inputs[2].x = 400;
    inputs[2].y = 270;
    
    int c = 3;
    vm_write(task, load_address + input_queue_offset, inputs, sizeof(Input) * c);
    char f = 0;
    vm_write(task, load_address + input_queue_count_offset, &c, 4);
    vm_write(task, load_address + input_queue_finished_offset, &f, 1);
}


void tds_return_to_lobby(task_t task,
                         vm_address_t load_address,
                         vm_address_t input_queue_offset,
                         vm_address_t input_queue_count_offset,
                         vm_address_t input_queue_finished_offset)
{
    Input inputs[3];
    inputs[0].type = 0;
    inputs[0].duration = 0;
    inputs[0].window_index = 0;
    inputs[0].x = 406.695312;
    inputs[0].y = 177.250000;
    
    inputs[1].type = 3;
    inputs[1].duration = 1000;
    inputs[1].window_index = 0;
    inputs[1].x = 406.695312;
    inputs[1].y = 177.250000;
    
    inputs[2].type = 4;
    inputs[2].duration = 0;
    inputs[2].window_index = 0;
    inputs[2].x = 406.695312;
    inputs[2].y = 177.250000;
    
    int c = 3;
    vm_write(task, load_address + input_queue_offset, inputs, sizeof(Input) * c);
    char f = 0;
    vm_write(task, load_address + input_queue_count_offset, &c, 4);
    vm_write(task, load_address + input_queue_finished_offset, &f, 1);
}


void tds_place_tower(task_t task,
                     vm_address_t load_address,
                     vm_address_t input_queue_offset,
                     vm_address_t input_queue_count_offset,
                     vm_address_t input_queue_finished_offset,
                     vector3_t pos,
                     rbx_cframe_t camera_cframe,
                     float fov,
                     float window_width,
                     float window_height,
                     int tower_number)
{
    int keycode = 17 + tower_number;
    custom_rbx_world_to_screen_info_t wtsi;
    wtsi = rbx_world_to_screen_point(camera_cframe, fov, pos, window_width, window_height, 1, 1);
    printf("%f %f\n", wtsi.x, wtsi.y);
    Input inputs[5];
    
    inputs[0].type = 1;
    inputs[0].duration = 1000;
    inputs[0].keycode = keycode;
    inputs[0].characters[0] = 0x48 + tower_number;
    inputs[0].window_index = 0;
    
    inputs[1].type = 2;
    inputs[1].duration = 0;
    inputs[1].keycode = keycode;
    inputs[1].characters[0] = 0x48 + tower_number;
    inputs[1].window_index = 0;
    
    inputs[2].type = 0;
    inputs[2].duration = 400000;
    inputs[2].window_index = 0;
    inputs[2].x = wtsi.x;
    inputs[2].y = wtsi.y;
    
    inputs[3].type = 3;
    inputs[3].duration = 1000;
    inputs[3].window_index = 0;
    inputs[3].x = wtsi.x;
    inputs[3].y = wtsi.y;
    
    inputs[4].type = 4;
    inputs[4].duration = 0;
    inputs[4].window_index = 0;
    inputs[4].x = wtsi.x;
    inputs[4].y = wtsi.y;
    
    int c = 5;
    vm_write(task, load_address + input_queue_offset, inputs, sizeof(Input) * c);
    char f = 0;
    vm_write(task, load_address + input_queue_count_offset, &c, 4);
    vm_write(task, load_address + input_queue_finished_offset, &f, 1);
}


void tds_upgrade_tower(task_t task,
                       vm_address_t load_address,
                       vm_address_t input_queue_offset,
                       vm_address_t input_queue_count_offset,
                       vm_address_t input_queue_finished_offset,
                       vm_address_t towers_folder,
                       rbx_cframe_t camera_cframe,
                       float fov,
                       float window_width,
                       float window_height,
                       int tower_index)
{
    long tower_count = 0;
    vm_address_t* towers = rbx_instance_get_children(task, towers_folder, &tower_count);
    if (tower_index > tower_count)
    {
        return;
    }
    if (towers == NULL)
    {
        return;
    }
    vm_address_t tower = towers[tower_index];
    free(towers);
    vm_address_t tower_hrp = rbx_instance_find_first_child(task, tower, "HumanoidRootPart");
    rbx_cframe_t thrpcf = rbx_basepart_get_cframe(task, tower_hrp);
    custom_rbx_world_to_screen_info_t wtsi;
    wtsi = rbx_world_to_screen_point(camera_cframe, fov, thrpcf.pos, window_width, window_height, 1, 1);
    printf("%f %f\n", wtsi.x, wtsi.y);
    Input inputs[6];
    float _x = wtsi.x + 60;
    float _y = wtsi.y + 47;
    
    inputs[0].type = 0;
    inputs[0].duration = 400000;
    inputs[0].window_index = 0;
    inputs[0].x = wtsi.x;
    inputs[0].y = wtsi.y;
    
    inputs[1].type = 3;
    inputs[1].duration = 1000;
    inputs[1].window_index = 0;
    inputs[1].x = wtsi.x;
    inputs[1].y = wtsi.y;
    
    inputs[2].type = 4;
    inputs[2].duration = 0;
    inputs[2].window_index = 0;
    inputs[2].x = wtsi.x;
    inputs[2].y = wtsi.y;
    
    inputs[3].type = 0;
    inputs[3].duration = 400000;
    inputs[3].window_index = 0;
    inputs[3].x = _x;
    inputs[3].y = _y;

    inputs[4].type = 3;
    inputs[4].duration = 1000;
    inputs[4].window_index = 0;
    inputs[4].x = _x;
    inputs[4].y = _y;
    
    inputs[5].type = 4;
    inputs[5].duration = 0;
    inputs[5].window_index = 0;
    inputs[5].x = _x;
    inputs[5].y = _y;
    
    int c = 6;
    vm_write(task, load_address + input_queue_offset, inputs, sizeof(Input) * c);
    char f = 0;
    vm_write(task, load_address + input_queue_count_offset, &c, 4);
    vm_write(task, load_address + input_queue_finished_offset, &f, 1);
}

void tds_w_move_to_pos(task_t task,
                       vm_address_t load_address,
                       vm_address_t input_queue_offset,
                       vm_address_t input_queue_count_offset,
                       vm_address_t input_queue_finished_offset,
                       vm_address_t camera,
                       vm_address_t my_hrp,
                       vector3_t pos,
                       float inaccuracy)
{
    Input inputs[1];
    static int input_count = 1;
    inputs[0].type = 1;
    inputs[0].duration = 0;
    inputs[0].keycode = 13;
    inputs[0].characters[0] = 'w';
    inputs[0].window_index = 0;

    
    vm_write(task, load_address + input_queue_offset, inputs, sizeof(inputs));
    char f = 0;
    vm_write(task, load_address + input_queue_count_offset, &input_count, 4);
    vm_write(task, load_address + input_queue_finished_offset, &f, 1);
    
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
    
    for (;;)
    {
        rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
        float i = inaccuracy;
        vector3_t lv = vector3_unit(pos, camera_cframe.pos);
        camera_cframe.r20 = -lv.x;
        camera_cframe.r21 = -lv.y;
        camera_cframe.r22 = -lv.z;
        rbx_camera_set_cframe(task, camera, &camera_cframe);
        if (hrp_cframe.pos.x > (pos.x - i) && hrp_cframe.pos.x < (pos.x + i))
        {
            if (hrp_cframe.pos.z > (pos.z - i) && hrp_cframe.pos.z < (pos.z + i))
            {
                printf(" > LOCATION REACHED\n");
                break;
            }
        }
        usleep(10000);
    }
    
    inputs[0].type = 2;
    inputs[0].duration = 0;
    inputs[0].keycode = 13;
    inputs[0].characters[0] = 'w';
    inputs[0].window_index = 0;

    
    vm_write(task, load_address + input_queue_offset, inputs, sizeof(inputs));
    vm_write(task, load_address + input_queue_count_offset, &input_count, 4);
    vm_write(task, load_address + input_queue_finished_offset, &f, 1);
    
}


void tds_w_jump_move_to_pos(task_t task,
                            vm_address_t load_address,
                            vm_address_t input_queue_offset,
                            vm_address_t input_queue_count_offset,
                            vm_address_t input_queue_finished_offset,
                            vm_address_t camera,
                            vm_address_t my_hrp,
                            vector3_t pos,
                            float inaccuracy)
{
    Input inputs[2];
    static int input_count = 2;
    inputs[0].type = 1;
    inputs[0].duration = 0;
    inputs[0].keycode = 13;
    inputs[0].characters[0] = 'w';
    inputs[0].window_index = 0;
    
    inputs[1].type = 1;
    inputs[1].duration = 0;
    inputs[1].keycode = 49;
    inputs[1].characters[0] = ' ';
    inputs[1].window_index = 0;

    
    vm_write(task, load_address + input_queue_offset, inputs, sizeof(inputs));
    char f = 0;
    vm_write(task, load_address + input_queue_count_offset, &input_count, 4);
    vm_write(task, load_address + input_queue_finished_offset, &f, 1);
    
    wait_until_input_queue_finished(task, load_address + input_queue_finished_offset, 1000);
    
    for (;;)
    {
        rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
        float i = inaccuracy;
        vector3_t lv = vector3_unit(pos, camera_cframe.pos);
        camera_cframe.r20 = -lv.x;
        camera_cframe.r21 = -lv.y;
        camera_cframe.r22 = -lv.z;
        rbx_camera_set_cframe(task, camera, &camera_cframe);
        if (hrp_cframe.pos.x > (pos.x - i) && hrp_cframe.pos.x < (pos.x + i))
        {
            if (hrp_cframe.pos.z > (pos.z - i) && hrp_cframe.pos.z < (pos.z + i))
            {
                printf(" > LOCATION REACHED\n");
                break;
            }
        }
        usleep(1000);
    }
    
    inputs[0].type = 2;
    inputs[0].duration = 0;
    inputs[0].keycode = 13;
    inputs[0].characters[0] = 'w';
    inputs[0].window_index = 0;
    
    inputs[1].type = 2;
    inputs[1].duration = 0;
    inputs[1].keycode = 49;
    inputs[1].characters[0] = ' ';
    inputs[1].window_index = 0;

    
    vm_write(task, load_address + input_queue_offset, inputs, sizeof(inputs));
    vm_write(task, load_address + input_queue_count_offset, &input_count, 4);
    vm_write(task, load_address + input_queue_finished_offset, &f, 1);
    
}
