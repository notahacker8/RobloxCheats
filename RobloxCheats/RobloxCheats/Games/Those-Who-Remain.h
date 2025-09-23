
//base() + 0x36BCE44    st1 {v17.4s, v18.4s, v19.4s}, [x2]    51 68 00 4C

//By the truck
static vector3_t market_endless_pos1 = {
    .x = -99.582123,
    .y = 8.452641,
    .z = 17.649540
};

//In front of the entrance
static vector3_t market_endless_pos2 = {
    .x = -2.206582,
    .y = 6.200003,
    .z = -8.091423
};

static vector3_t dealership_corner_pos = {
    .x = -38.359070,
    .y = 1.549203,
    .z = -105.277214
};

static vector3_t dealership_front_pos = {
    .x = -109.018814,
    .y = 1.549203,
    .z = -93.236816
};

static vector3_t dealership_back_pos = {
    .x = -22.070091,
    .y = 1.549203,
    .z = -93.665527
};



//basically stops welds from working
void those_who_remain_aimbot_patch(task_t task, vm_address_t task_base_address, char status)
{
    static char __status = 0;
    if (status == __status)
    {
        return;
    }
    
    char orig_bytes[4] = {"\x51\x68\x00\x4C"};
    vm_address_t nop_addr = task_base_address +0x3836654;
    __status = status;
    if (status == 1)
    {
        edit_exec_memory(task, NOP, nop_addr, 4);
    }
    if (status == 0)
    {
        edit_exec_memory(task, orig_bytes, nop_addr, 4);
    }
}


//base() + 0x1DE96A4    str q0, [x0, #0xc0]    00 30 80 3D


vm_address_t* twr_stored_memory_addresses = NULL;
int twr_stored_memory_address_count = 0;
int twr_stored_memory_address_max_count = 1000000;


void twr_search_double_in_ioaccelerator(task_t task, double truncated)
{
    if (twr_stored_memory_address_count == 0)
    {
        int __twr_ma_i = 0;
        MEMORY_SEARCH_HEADER(VM_MEMORY_IOACCELERATOR)
        for (long i = 0 ; i < (size - 8) ; i += 8)
        {
            if (*((double*)(read_data + i)) == truncated)
            {
                if (__twr_ma_i < twr_stored_memory_address_max_count - 1)
                {
                    twr_stored_memory_addresses[__twr_ma_i++] = address + i;
                }
            }
        }
        MEMORY_SEARCH_FOOTER
        twr_stored_memory_address_count = __twr_ma_i;
    }
    else
    {
        COPY_BUF(twr_stored_memory_addresses)
        int __twr_ma_i = 0;
        for (int i = 0 ; i < twr_stored_memory_address_count ; i++)
        {
            if (vm_read_double_value(task, __twr_stored_memory_addresses[i]) == truncated)
            {
                if (__twr_ma_i < twr_stored_memory_address_max_count - 1)
                {
                    twr_stored_memory_addresses[__twr_ma_i++] = __twr_stored_memory_addresses[i];
                }
            }
        }
        twr_stored_memory_address_count = __twr_ma_i;
        //memcpy(twr_stored_memory_addresses, __twr_stored_memory_addresses, sizeof(twr_stored_memory_addresses));
    }
}


void those_who_remain_bot_pos(task_t task)
{
    LIBESP_DYLIB_SETUP
}


void those_who_remain_cheat(task_t task)
{
    
    EXIT_IF_TASK_TERMINATED
    twr_stored_memory_addresses = malloc(8 * twr_stored_memory_address_max_count);
    printf("- THOSE WHO REMAIN -\n");
    printf("FOR AIMBOT: SEARCH FOR BYTES: 51 68 00 4C\n");
    LIBESP_DYLIB_SETUP
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    static ESP_Color nerve_gas_esp_color = {.r = 0, .g = 0.5, .b = 1, .a = 1};
    static ESP_Color molotov_esp_color = {.r = 1, .g = 0.5, .b = 0, .a = 1};
    static ESP_Color bandages_esp_color = {.r = 0.75, .g = 0.5, .b = 0.5, .a = 1};
    static ESP_Color body_armor_esp_color = {.r = 1, .g = 0, .b = 0.75, .a = 1};
    static ESP_Color energy_drink_esp_color = {.r = 0, .g = 1, .b = 1, .a = 1};
    static ESP_Color medkit_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color barbed_wire_esp_color = {.r = 1, .g = 1, .b = 0, .a = 1};
    static ESP_Color jack_esp_color = {.r = 0.75, .g = 0, .b = 1, .a = 1};
    static ESP_Color clap_bomb_esp_color = {.r = 0.5, .g = 0.5, .b = 0.5, .a = 1};
    static ESP_Color ammo_esp_color = {.r = 0, .g = 0.75, .b = 0, .a = 1};
    static ESP_Color gas_mask_esp_color = {.r = 0.5, .g = 1, .b = 0, .a = 1};
    
    static ESP_Color target_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    //base() + 0x3667950;
    static ESP_Color jug_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color white_esp_color = {.r = 1, .g = 1, .b = 1, .a = 1};
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            ESP_Color c = jug_esp_color;
            if (c.r >= 1 && c.g <= 1)
            {
                c.b -= 0.1;
                c.g += 0.1;
            }
            if (c.g >= 1 && c.b <= 1)
            {
                c.r -= 0.1;
                c.b += 0.1;
            }
            if (c.b >= 1 && c.r <= 1)
            {
                c.g -= 0.1;
                c.r += 0.1;
            }
            jug_esp_color = c;
            usleep(50000);
        }
    });
    
    ESP_GLOBAL_VARIABLES
    
    static useconds_t esput = 1000;
    
    shared_memory -> ESP_ENABLED = esp_enabled;
    shared_memory -> ESP_COUNT = MAX_ESP_COUNT;
    shared_memory -> ESP_USLEEP_TIME = esput;
    strcpy(shared_memory->ESP_TEXT_FONT_NAME, "Arial");
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t rs = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");
    static vm_address_t entities_folder = 0 ; //rbx_instance_find_first_child(task, workspace, "Entities");
    static vm_address_t infected_folder = 0; //rbx_instance_find_first_child(task, entities_folder, "Infected");
    //rbx_print_descendants(task, rs, 0, 4);
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    rbx_print_descendants(task, local_player, 0, 0);
    vm_address_t tempstats_folder = rbx_instance_find_first_child(task, local_player, "TempStats");
    vm_address_t tempstats_spawned_bv = rbx_instance_find_first_child(task, tempstats_folder, "Spawned");
    
#pragma mark - ANTI-AFK -
    ANTI_AFK
    
    vm_address_t playergui = rbx_instance_find_first_child(task, local_player, "PlayerGui");
    vm_address_t main_screengui = rbx_instance_find_first_child(task, playergui, "Main");
    vm_address_t frame = rbx_instance_find_first_child(task, main_screengui, "Frame");
    vm_address_t HUD_frame = rbx_instance_find_first_child(task, frame, "HUD");
    vm_address_t fortifications_frame = rbx_instance_find_first_child(task, frame, "Fortifications");
    //rbx_print_descendants(task, workspace, 0, 1);
    /*FOR_EACH_DESCENDANT(playergui, 100000, 100, {
        long str_l = 0;
        char* str = rbx_textlabel_get_text(task, obj, &str_l);
        if (str)
        {
            //printf("%s\n", str);
            if (strstr(str, "BARBED"))
            {
                rbx_print_ancestors(task, obj, 10);
            }
            vm_deallocate(mach_task_self_, str, str_l);
        }
    })*/
    
    vm_address_t HUD_pool_tl = rbx_instance_find_first_child(task, HUD_frame, "Pool");
    
    static vm_address_t my_character = 0;
    static vm_address_t my_gun = 0;
    static vm_address_t my_gun_aimpart = 0;
    static vm_address_t my_gun_handle = 0;
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hum = 0;
    static vm_address_t drinked_bv = 0;
    
    //rbx_print_descendants(task, workspace, 0, 2);
    
    static vm_address_t ignore_folder = 0;
    static vm_address_t items_folder = 0;
    static vm_address_t pre_placement_folder = 0;
    
    static long juggernaut_hrp_count = 0;
    static vm_address_t juggernaut_hrps[100];
    static vm_address_t juggernaut_heads[100];
    static vm_address_t juggernaut_torsos[100];
    static vm_address_t juggernaut_hums[100];
    static vm_address_t juggernaut_hrp_properties_addresses[100];
    static vm_address_t juggernaut_head_properties_addresses[100];
    static vector3_t jug_cam_projections[200];
    
    static vm_address_t all_zombie_hrps[1000];
    static vm_address_t all_zombie_heads[1000];
    static vm_address_t all_zombie_torsos[1000];
    static long all_zombie_count = 0;
    
    static vm_address_t correctable_hrps[1000];
    static vm_address_t correctable_heads[1000];
    static float correctable_dists[1000]; //distance at which we project, juggernauts go first
    static long correctable_zombie_count = 0;
    static rbx_cframe_t old_camera_cframe = {};
    
    static int correctable_jug_count = 0;
    
    static float min_aimbot_default_range = 2000;
    static int aimbot_count_limit = 10;
    
    static char show_ammo = false;
    static char show_fortifications = true;
    static char show_aid = true;
    static char show_gear = true;
    static char show_throwables = true;
    
    static float esp_alpha = 0.3;
    static int esp_wait = 50;
    
    static char ammo_gun_count = 0;
    static char ammo_gun_names[100][300];
    static vm_address_t ammo_addresses[100];
    static double ammo_sets[100];
    
    static char message[200];
    
    static char aimbot_enabled = true;
    
    static long total_plr_count = 0;
    static long alive_plr_count = 0;
    
    static char list_of_dead_plrs_str[1000];
    
    static char hack_forts = false;
    static char hack_guns = false;
    static char bot_mode = false;
    static char use_rpm_hack = false;
    
    static char loopgoto_enabled = false;
    
    static char did_save_cframe = 0;
    static rbx_cframe_t saved_cframe;
    static vector3_t bot_pos;
    
    static vm_address_t rpm_addys[1000];
    static int rpm_addy_count = 0;
    
    those_who_remain_aimbot_patch(task, task_base_address, 0);
    
    
    static char fly_enabled = false;
    static float fly_speed = 40;
    static float new_gravity = 200;
    BASIC_FLY_ENGINE(BASIC_FLY_CONTROL_CODE, 1000);
    
    
    NOCLIP_VARIABLES
    noclip_enabled = false;
    BASIC_NOCLIP_PART_GETTER
    BASIC_NOCLIP_ENGINE
    
#pragma mark - loopgoto/Bot pos -
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(100);
            if (bot_mode)
            {
                esp_enabled = false;
                //noclip_enabled = true;
                esp_wait = 1000;
                rbx_basepart_set_gravity(task, my_hrp, 0);
                rbx_cframe_t cf = rbx_basepart_get_cframe(task, my_hrp);
                cf.pos = bot_pos;
                rbx_basepart_set_cframe(task, my_hrp, cf);
            }
            else
            {
               if (loopgoto_enabled && did_save_cframe)
               {
                   rbx_basepart_set_cframe(task, my_hrp, saved_cframe);
               }
            }
        }
    });
    
#pragma mark - RPM Search -
    
    //goto __rpm_search_end;
   
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        MEMORY_SEARCH_HEADER(VM_MEMORY_IOACCELERATOR)
        for (long i = 16 ; i < (size - 8 - 16) ; i += 8)
        {
            MEMORY_SEARCH_LOCAL_VARIABLE_FILTER_HEADER(var_type == 3)
            //double read_double = *(double*)(read_data + i - 0x10);
            char nm[100]; bzero(nm, sizeof(nm)); long nm_l = 0;
            //char* sv[100]; bzero(sv, sizeof(sv)); long sv_l = 0;
            rbx_local_variable_get_string_value(task, var_name_ptr, nm, &nm_l);
            //rbx_local_variable_get_string_value(task, *((vm_address_t*)(read_data + i - 0x10)), sv, &sv_l);
            if (strcmp(nm, "RPM") == 0)
            {
                rpm_addys[rpm_addy_count++] = var_ptr;
            }
            /*
            if (strstr(nm, "Slot"))
            {
                printf("%s : %f\n", nm, vm_read_double_value(task, var_ptr));
                //double d = 99999999999999;
                //vm_write(task, var_ptr, &d, 8);
                //rbx_local_variable_get_string_value(task, a, nm, &nm_l);
                // char* v[100]; bzero(v, sizeof(v)); long v_l = 0;
                //rbx_local_
                //double ___nv = 200;
                //vm_write(task, pv, &___nv, 8);
                // printf("variable: %p, %s = ???, type : %d\n", var_ptr, nm, var_type);
                //rpm_addys[rpm_addy_count++] = var_ptr;
                {
                    int cnt = 10;
                    vm_address_t var1 = var_ptr - (0x20 * cnt/2);
                    for (int _i = 0 ; _i < cnt ; _i++)
                    {
                        vm_address_t var = var1 + _i * RBX_LOCAL_VARIABLE_SIZE;
                        char* nm1[100]; bzero(nm1, sizeof(nm1)); long nm1_l = 0;
                        rbx_local_variable_get_name(task, var, nm1, &nm1_l);
                        //if (strcmp(nm1, "RPM") == 0)
                        {
                            //rpm_addys[rpm_addy_count++] = var;
                        }
                        //printf("[%p] [+%p]: %s\t\t type: %d\n", var, _i * RBX_LOCAL_VARIABLE_SIZE, nm1, rbx_local_variable_get_type(task, var));
                    }
                }
            }
             */
            MEMORY_SEARCH_LOCAL_VARIABLE_FILTER_FOOTER
        }
        MEMORY_SEARCH_FOOTER
    });
    
    __rpm_search_end: {};
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (use_rpm_hack)
            {
                for (int i = 0 ; i < rpm_addy_count ; i++)
                {
                    vm_address_t rpm_a = rpm_addys[i];
                    double v = vm_read_double_value(task, rpm_a);
                    if (correctable_jug_count)
                    {
                        v = fabs(v) * -1;
                    }
                    else
                    {
                        v = fabs(v);
                    }
                    vm_write(task, rpm_a, (vm_address_t)&v, 8);
                }
            }
            usleep(200000);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (rbx_boolvalue_get_value(task, tempstats_spawned_bv))
            {
                if (shared_memory->KEYS_DOWN['l'])
                {
                    if (did_save_cframe)
                    {
                        rbx_basepart_set_cframe(task, my_hrp, saved_cframe);
                    }
                    while (shared_memory->KEYS_DOWN['l'])
                    {
                        usleep(1000);
                    }
                }
                usleep(1000);
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
            my_character = rbx_player_get_character(task, local_player);

            if (!rbx_boolvalue_get_value(task, tempstats_spawned_bv))
            {
                //printf("RESETING ADDRESSES\n");
                bzero(ammo_addresses, sizeof(ammo_addresses));
                bzero(ammo_sets, sizeof(ammo_sets));
                for (int i = 0 ; i < 100 ; i++)
                {
                    bzero(ammo_gun_names[i], 300);
                }
                ammo_gun_count = 0;
            }
            /*else
            {
                for (int i = 0 ; i < ammo_gun_count ; i++)
                {
                    printf("Setting %s reserve @ %p to %f [%d of %d]\n", ammo_gun_names[i], ammo_addresses[i], ammo_sets[i], i + 1, ammo_gun_count);
                }
            }*/
            //rbx_print_descendants(task, my_character, 0, 1);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
            my_hum = rbx_instance_find_first_child_of_class(task, my_character, "Humanoid");
            drinked_bv = rbx_instance_find_first_child(task, my_character, "Drinked");
            //rbx_humanoid_set_walkspeed(task, my_hum, 23);
            //printf("%p: %f: \n", my_hum, rbx_humanoid_get_walkspeed(task, my_hum));
            //printf("my properties: %p\n", rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET);
            ignore_folder = rbx_instance_find_first_child(task, workspace, "Ignore");
            items_folder = rbx_instance_find_first_child(task, ignore_folder, "Items");
            //rbx_print_descendants(task, rbx_model_get_primarypart(task, bw), 0, 2);
            entities_folder = rbx_instance_find_first_child(task, workspace, "Entities");
            infected_folder = rbx_instance_find_first_child(task, entities_folder, "Infected");
            //printf("%f\n", rbx_camera_get_field_of_view(task, camera));
            sleep(3);
        }
    });
    

    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(500000);
            for (int i = 0 ; i < ammo_gun_count ; i++)
            {
                if (ammo_addresses[i])
                {
                    //printf("setting %s reserve ammo to %f: address @ %p\n", ammo_gun_names[i], ammo_sets[i], ammo_addresses[i]);
                    vm_write(task, ammo_addresses[i], (vm_address_t)&ammo_sets[i], 8);
                }
            }
        }
    });
    
    
#pragma mark - Ammo search -
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (my_gun)
            {
                if (shared_memory->KEYS_DOWN['p'])
                {
                    COPY_VAR(my_gun)
                    long gun_name_l = 0;
                    char* gun_name = rbx_instance_get_name(task, __my_gun, &gun_name_l);
                    if (gun_name)
                    {
                        char is_listed = false;
                        for (int i = 0 ; i < ammo_gun_count ; i++)
                        {
                            if (strcmp(gun_name, ammo_gun_names[i]) == 0)
                            {
                                is_listed = true;
                            }
                        }
                        if (!is_listed)
                        {
                            double current_pool = -1 ;
                            
                            long str_l = 0;
                            char* str = rbx_textlabel_get_text(task, HUD_pool_tl, &str_l);
                            if (str)
                            {
                                current_pool = atof(str);
                                vm_deallocate(mach_task_self_, (vm_address_t)str, str_l);
                            }
                            if (current_pool)
                            {
                                printf("scanning for %f\n", current_pool);
                                if (twr_stored_memory_address_count == 0 || twr_stored_memory_address_count > 1)
                                {
                                    strcpy(message, "SCANNING MEMORY... DO NOT RELOAD YOUR WEAPON");
                                    twr_search_double_in_ioaccelerator(task, current_pool);
                                    COPY_BUF(twr_stored_memory_addresses)
                                    int __twr_ma_i = 0;
                                    for (int i = 0 ; i < twr_stored_memory_address_count ; i++)
                                    {
                                        vm_address_t a = __twr_stored_memory_addresses[i];
                                        if (vm_read_double_value(task, a) == current_pool)
                                        {
                                            vm_address_t b = a - RBX_LOCAL_VARIABLE_SIZE;
                                            
                                            char a_name[100]; int a_name_l = 0; bzero(a_name, sizeof(a_name));
                                            rbx_local_variable_get_name(task, a, a_name, &a_name_l);
                                            if (strcmp(a_name, "Pool") == 0)
                                            {
                                                char b_name[100]; int b_name_l = 0; bzero(b_name, sizeof(b_name));
                                                rbx_local_variable_get_name(task, b, b_name, &b_name_l);
                                                if (strcmp(b_name, "Name") == 0)
                                                {
                                                    printf("my gun: %s\n", gun_name);
                                                    char b_val[100]; int b_val_l = 0; bzero(b_val, sizeof(b_val));
                                                    rbx_local_variable_get_string_value(task, vm_read_vm_address_t_value(task, b), b_val, &b_val_l);
                                                    if (strcmp(gun_name, b_val) == 0)
                                                    {
                                                        twr_stored_memory_addresses[__twr_ma_i++] = a;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    twr_stored_memory_address_count = __twr_ma_i;
                                    sprintf(message, "[%d addresses] DONE, NOW RELOAD", twr_stored_memory_address_count);
                                }
                                if (twr_stored_memory_address_count == 1)
                                {
                                    ammo_addresses[ammo_gun_count] = twr_stored_memory_addresses[0];
                                    sprintf(message, "CAPPED RESERVE AMMO FOR %s AT %f", gun_name, current_pool);;
                                    ammo_sets[ammo_gun_count] = current_pool;
                                    bzero(ammo_gun_names[ammo_gun_count], 300);
                                    strcpy(ammo_gun_names[ammo_gun_count], gun_name);
                                    ammo_gun_count++;
                                    twr_stored_memory_address_count = 0;
                                    sleep(1);
                                    strcpy(message, "");
                                }
                            }
                        }
                        
                        vm_deallocate(mach_task_self_, (vm_address_t)gun_name, gun_name_l);
                    }
                    
                }
                usleep(1000);
            }
            else
            {
                sleep(1);
            }
        }
    });
    
    
    double __nan = NAN;
    double __neg = -3000;
    double __inf = INFINITY;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (hack_forts)
            {
                strcpy(message, "SEARCHING MEMORY... DON'T FORTIFY...");
                MEMORY_SEARCH_HEADER(VM_MEMORY_IOACCELERATOR)
                for (long i = 16 ; i < (size - 8 - 16) ; i += 8)
                {
                    MEMORY_SEARCH_LOCAL_VARIABLE_FILTER_HEADER(var_type == 3)
                    
                    double read_double = *((double*)(read_data + i - 0x10));
                    //double read_double = vm_read_double_value(task, var_ptr);
                    if (trunc(read_double) == read_double)
                    {
                        char nm[100]; bzero(nm, sizeof(nm)); int nm_l = 0;
                        rbx_local_variable_get_string_value(task, var_name_ptr, nm, &nm_l);
                        if (hack_forts)
                        {
                            if (read_double == 2 || read_double == 1)
                            {
                                if (strcmp(nm, "Count") == 0)
                                {
                                    printf("COUNT: %p : %f\n", var_ptr, read_double);
                                    vm_write(task, var_ptr, (vm_address_t)&__nan, 8);
                                }
                            }
                            if (read_double == 2)
                            {
                                if (strcmp(nm, "Swings") == 0)
                                {
                                    printf("SWINGS: %p : %f\n", var_ptr, read_double);
                                    vm_write(task, var_ptr, (vm_address_t)&__nan, 8);
                                }
                            }
                        }
                    }
                    
                    
                    MEMORY_SEARCH_LOCAL_VARIABLE_FILTER_FOOTER
                }
                MEMORY_SEARCH_FOOTER
                //twr_search_double_in_ioaccelerator(task, read_wire_count);
                strcpy(message, "");
                hack_forts = false;
            }
            else
            {
                sleep(1);
            }
        }
    });
    
#pragma mark - My Gun -

    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            vm_address_t __my_gun = 0;
            vm_address_t __my_gun_handle = 0;
            vm_address_t __my_gun_aimpart = 0;
            FOR_EACH_CHILD(my_character, {
                //rbx_print_descendants(task, child, 0, 1);
                vm_address_t ____my_gun_aimpart = rbx_instance_find_first_child(task, child, "AimPart");
                vm_address_t ____my_gun_pos = rbx_instance_find_first_child(task, child, "Pos");
                if (____my_gun_aimpart || ____my_gun_pos)
                {
                    __my_gun_aimpart = ____my_gun_aimpart;
                    __my_gun = child;
                    __my_gun_handle = rbx_instance_find_first_child(task, child, "Handle");
                }
                
            })
            my_gun = __my_gun;
            my_gun_handle = __my_gun_handle;
            my_gun_aimpart = __my_gun_aimpart;
            //rbx_print_descendants(task, my_gun, 0, 0);
            sleep(1);
        }
    });
    
    
#pragma mark - TARGET FIND -
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (aimbot_enabled && my_gun && !bot_mode)
            {
                COPY_BUF(all_zombie_hrps)
                COPY_BUF(all_zombie_heads)
                COPY_BUF(all_zombie_torsos)
                COPY_VAR(all_zombie_count)
                
                COPY_VAR(juggernaut_hrp_count)
                COPY_BUF(juggernaut_hrps)
                int cj_idx = 0;
                int c_idx = 0;
                int j_idx = 0;
                
                char my_gun_name[200];
                {
                    long nl = 0;
                    char* n = rbx_instance_get_name(task, my_gun, &nl);
                    if (n)
                    {
                        strcpy(my_gun_name, n);
                        vm_deallocate(mach_task_self_, n, nl);
                    }
                }
                
                char is_flamethrower = (strstr(my_gun_name, "Flame") > 0);
                
                for (int i = 0 ; i < __all_zombie_count ; i++)
                {
                    vm_address_t z_hrp = __all_zombie_hrps[i];
                    vm_address_t z_head = __all_zombie_heads[i];
                    vm_address_t z_torso = __all_zombie_torsos[i]; //rbx_instance_find_first_child(task, rbx_instance_get_parent(task, z_hrp), "Torso");
                    char is_jug = false;
                    for (long x = 0 ; x < __juggernaut_hrp_count ; x++)
                    {
                        if (__juggernaut_hrps[x] == z_hrp)
                        {
                            is_jug = true;
                            x = __juggernaut_hrp_count;
                        }
                    }
                    rbx_cframe_t z_hrp_cf = rbx_basepart_get_cframe(task, z_hrp);
                    rbx_cframe_t cam_cf = rbx_camera_get_cframe(task, camera);
                    float dist = vector3_dist_dif(cam_cf.pos, z_hrp_cf.pos);
                    if (is_jug)
                    {
                        vector3_t cam_look_vector = rbx_cframe_get_look_vector(cam_cf);
                        vector3_t cam_projected_pos = vector3_add(cam_cf.pos, vector3_mul_num(cam_look_vector, dist));
                        vector3_t jug_look_vector = rbx_cframe_get_look_vector(z_hrp_cf);
                        vector3_t jug_projected_pos = vector3_add(cam_cf.pos, vector3_mul_num(jug_look_vector, dist));
                        jug_cam_projections[j_idx++] = jug_projected_pos;
                        float delta_dist = vector3_dist_dif(jug_projected_pos, cam_projected_pos);
                        if (delta_dist < (sqrtf(1.9) * dist + (10000 * is_flamethrower)))
                        {
                            correctable_hrps[c_idx] = z_hrp;
                            if (is_flamethrower)
                            {
                                correctable_heads[c_idx] = z_torso;
                                correctable_dists[c_idx] = 5;
                            }
                            else
                            {
                                correctable_heads[c_idx] = z_head;
                                correctable_dists[c_idx] = 3;
                            }
                            c_idx++;
                            cj_idx++;
                        }
                    }
                    else
                    {
                        if (dist < 2000 && c_idx < aimbot_count_limit)
                        {
                            //if (dist < 2000)
                            if (dist > min_aimbot_default_range || dist < 10)
                            {
                                correctable_hrps[c_idx] = z_hrp;
                                if (is_flamethrower)
                                {
                                    correctable_heads[c_idx] = z_torso;
                                    correctable_dists[c_idx] = 6;
                                }
                                else
                                {
                                    correctable_heads[c_idx] = z_head;
                                    correctable_dists[c_idx] = 4;
                                }
                                c_idx++;
                            }
                            else if (dist < (200 - (120 * (is_flamethrower))))
                            {
                                vector3_t cam_look_vector = rbx_cframe_get_look_vector(cam_cf);
                                vector3_t cam_projected_pos = vector3_add(cam_cf.pos, vector3_mul_num(cam_look_vector, dist));
                                float delta_dist = vector3_dist_dif(z_hrp_cf.pos, cam_projected_pos);
                                if (delta_dist < (sqrt(3 * dist)))
                                {
                                    correctable_hrps[c_idx] = z_hrp;
                                    if (is_flamethrower)
                                    {
                                        correctable_heads[c_idx] = z_torso;
                                        correctable_dists[c_idx] = 6;
                                    }
                                    else
                                    {
                                        correctable_heads[c_idx] = z_head;
                                        correctable_dists[c_idx] = 4;
                                    }
                                    c_idx++;
                                }
                            }
                        }
                    }
                }
                correctable_zombie_count = c_idx;
                correctable_jug_count = cj_idx;
                usleep(100000);
            }
            
            else
            {
                correctable_zombie_count = 0;
                correctable_jug_count = 0;
                sleep(1);
            }
        }
    });
    
#pragma mark - HEAD POS -
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (aimbot_enabled && my_gun && !bot_mode)
            {
                COPY_VAR(correctable_zombie_count)
                if (__correctable_zombie_count > 0)
                {
                    if (shared_memory->LEFT_MOUSE_DOWN || shared_memory->KEYS_DOWN['t'])
                    {
                        COPY_BUF(correctable_hrps)
                        COPY_BUF(correctable_heads)
                        COPY_BUF(correctable_dists)
                        
                        //COPY_VAR(juggernaut_hrp_count)
                        //COPY_BUF(juggernaut_hrps)
                        those_who_remain_aimbot_patch(task, task_base_address, true);
                        for (long i = 0 ; i < __correctable_zombie_count ; i++)
                        {
                            vm_address_t cz_hrp = __correctable_hrps[i];
                            if (rbx_instance_is_valid_child(task, cz_hrp))
                            {
                                float __D = __correctable_dists[i];
                                float __O = 0.0f + ((float)rand() / RAND_MAX) * 0.3f;
                                vm_address_t cz_head = __correctable_heads[i];
                                if (rbx_instance_is_valid_child(task, cz_head))
                                {
                                    rbx_cframe_t new_zchead_cf = old_camera_cframe;
                                    new_zchead_cf.pos = vector3_add(old_camera_cframe.pos, vector3_mul_num(rbx_cframe_get_look_vector(old_camera_cframe), __D + __O));
                                    rbx_basepart_set_cframe(task, cz_head, new_zchead_cf);
                                }
                            }
                        }
                        usleep(100);
                    }
                    else
                    {
                        old_camera_cframe = rbx_camera_get_cframe(task, camera);;
                        those_who_remain_aimbot_patch(task, task_base_address, false);
                        usleep(20);
                    }
                    
                }
                else
                {
                    those_who_remain_aimbot_patch(task, task_base_address, false);
                    usleep(10000);
                }
            }
            else
            {
                those_who_remain_aimbot_patch(task, task_base_address, false);
                sleep(1);
            }
        }
    });
    
    
#pragma mark - Get Infected -
     
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            int z_idx = 0;
            int jug_idx = 0;
            FOR_EACH_CHILD(infected_folder, {
                long name_l = 0;
                char* name = rbx_instance_get_name(task, child, &name_l);
                if (name)
                {
                    vm_address_t hum = rbx_instance_find_first_child_of_class(task, child, "Humanoid");
                    vm_address_t hrp = rbx_instance_find_first_child(task, child, "HumanoidRootPart");
                    vm_address_t head = rbx_instance_find_first_child(task, child, "Head");
                    vm_address_t torso = rbx_instance_find_first_child(task, child, "Torso");
                    //rbx_basepart_set_cancollide(task, head, false);
                    
                    all_zombie_hrps[z_idx] = hrp;
                    all_zombie_heads[z_idx] = head;
                    all_zombie_torsos[z_idx] = torso;
                    z_idx++;
                    
                    if (strcmp(name, "Juggernaut") == 0)
                    {
                        vm_address_t hrp_properties_address = rbx_basepart_get_properties_address(task, hrp);
                        juggernaut_hums[jug_idx] = hum;
                        juggernaut_heads[jug_idx] = head;
                        juggernaut_torsos[jug_idx] = torso;
                        juggernaut_hrps[jug_idx] = hrp;
                        juggernaut_hrp_properties_addresses[jug_idx] = hrp_properties_address;
                        jug_idx++;
                    }
                    else
                    {
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)name, name_l);
                }
            })
            juggernaut_hrp_count = jug_idx;
            all_zombie_count = z_idx;
            sleep(2);
        }
    });
    
    
    /*dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            for (int i = 0 ; i < juggernaut_hrp_count ; i++)
            {
                vm_address_t z_hrp_cf_addr = juggernaut_hrp_properties_addresses[i] + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                rbx_cframe_t new_cf = rbx_camera_get_cframe(task, camera);
                rbx_cframe_t z_hrp_cf = vm_read_rbx_cframe_t_value(task, z_hrp_cf_addr);
                new_cf.pos = z_hrp_cf.pos;
                vm_write(task, z_hrp_cf_addr, &new_cf, sizeof(rbx_cframe_t));
            }
            usleep(10);
        }
    });*/
    
    
#pragma mark - OBJECT FIND -
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (esp_enabled)
            {
                int esp_part_index = 0;
                long cc = 0;
                rbx_child_t* children = rbx_instance_get_children(task, items_folder, &cc);
                if (children)
                {
                    for (long i = 0 ; i < cc ; i++)
                    {
                        vm_address_t child = children[i].child_address;
                        
                        long name_len;
                        char* name = rbx_instance_get_name(task, child, &name_len);
                        if (name)
                        {
                            char __is__ = false;
                            ESP_Color __color__ = {.a = 0};
                            if (show_ammo)
                            {
                                if (strcmp(name, "Ammo") == 0)
                                {
                                    __is__ = true;
                                    __color__ = ammo_esp_color;
                                }
                            }
                            if (show_aid)
                            {
                                if (strcmp(name, "Medkit") == 0)
                                {
                                    __is__ = true;
                                    __color__ = medkit_esp_color;
                                }
                                if (strcmp(name, "Energy Drink") == 0)
                                {
                                    __is__ = true;
                                    __color__ = energy_drink_esp_color;
                                }
                                if (strcmp(name, "Bandages") == 0)
                                {
                                    __is__ = true;
                                    __color__ = bandages_esp_color;
                                }
                            }
                            if (show_gear)
                            {
                                if (strcmp(name, "Gas Mask") == 0)
                                {
                                    __is__ = true;
                                    __color__ = gas_mask_esp_color;
                                }
                                if (strcmp(name, "Body Armor") == 0)
                                {
                                    __is__ = true;
                                    __color__ = body_armor_esp_color;
                                }
                            }
                            if (show_throwables)
                            {
                                if (strcmp(name, "Nerve Gas") == 0)
                                {
                                    __is__ = true;
                                    __color__ = nerve_gas_esp_color;
                                }
                                if (strcmp(name, "Molotov") == 0)
                                {
                                    __is__ = true;
                                    __color__ = molotov_esp_color;
                                }
                                if (strcmp(name, "Frag") == 0)
                                {
                                    __is__ = true;
                                    __color__ = ammo_esp_color;
                                }
                            }
                            if (show_fortifications)
                            {
                                if (strcmp(name, "Jack") == 0)
                                {
                                    __is__ = true;
                                    __color__ = jack_esp_color;
                                }
                                if (strcmp(name, "Clap Bomb") == 0)
                                {
                                    __is__ = true;
                                    __color__ = clap_bomb_esp_color;
                                }
                                if (strcmp(name, "Barbed Wire") == 0)
                                {
                                    __is__ = true;
                                    __color__ = barbed_wire_esp_color;
                                }
                            }
                            if (__is__)
                            {
                                __color__.a = esp_alpha;
                                vm_address_t box = rbx_model_get_primarypart(task, child);
                                APPEND_ESP_OBJ(box, __color__, name);
                            }
                            vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)children, cc * sizeof(rbx_child_t));
                }
                for (long i = 0 ; i < juggernaut_hrp_count ; i++)
                {
                    //rbx_print_descendants(task, juggernaut_hrps[i], 0, 0);
                    char txt[100];
                    bzero(txt, 100);
                    sprintf(txt, "JUGGERNAUT (HP: %.0f)", rbx_humanoid_get_health(task, juggernaut_hums[i]));
                    APPEND_ESP_OBJ(juggernaut_hrps[i], target_esp_color, txt)
                }
                long __tpc = 0;
                long __apc = rbx_boolvalue_get_value(task, tempstats_spawned_bv);
                COPY_BUF(list_of_dead_plrs_str)
                bzero(__list_of_dead_plrs_str, sizeof(__list_of_dead_plrs_str));
                FOR_EACH_CHILD(players_service, {
                    __tpc++;
                    vm_address_t ts = rbx_instance_find_first_child(task, child, "TempStats");
                    vm_address_t ts_bv = rbx_instance_find_first_child(task, ts, "Spawned");
                    if (rbx_boolvalue_get_value(task, ts_bv))
                    {
                        if (child != local_player)
                        {
                            __apc++;
                            vm_address_t c = rbx_player_get_character(task, child);
                            vm_address_t hrp = rbx_instance_find_first_child(task, c, "HumanoidRootPart");
                            long name_l = 0;
                            char* name = rbx_instance_get_name(task, child, &name_l);
                            if (name)
                            {
                                char txt[100];
                                bzero(txt, 100);
                                sprintf(txt, "%s", name);
                                APPEND_ESP_OBJ(hrp, white_esp_color, txt)
                                vm_deallocate(mach_task_self_, (vm_address_t)name, name_l);
                            }
                        }
                    }
                    else
                    {
                        long pnl = 0;
                        char* pn = rbx_instance_get_name(task, child, &pnl);
                        if (pn)
                        {
                            sprintf(__list_of_dead_plrs_str, "%s%s\n", __list_of_dead_plrs_str, pn);
                            vm_deallocate(mach_task_self_, (vm_address_t)pn, pnl);
                        }
                    }
                    
                })
                memcpy(list_of_dead_plrs_str, __list_of_dead_plrs_str, sizeof(list_of_dead_plrs_str));
                alive_plr_count = __apc;
                total_plr_count = __tpc;
                //APPEND_ESP_OBJ(closest_z_hrp, target_esp_color, "TARGET")
                esp_part_count = esp_part_index;
                usleep(500000);
                //NSLog(@"ITEM CHECK");
            }
            else
            {
                esp_part_count = 0;
                sleep(2);
            }
        }
    });
    
    
    
    
#pragma mark - ESP -

    
    
    
    BASIC_ESP_ENGINE({
        char is_jug = false;
        for (long x = 0 ; x < juggernaut_hrp_count ; x++)
        {
            if (part == juggernaut_hrps[x])
            {
                x = juggernaut_hrp_count;
                is_jug = true;
            }
        }
        should_scale = true;
        object_width = 2;
        object_height = 2;
        if (is_jug)
        {
            esp_color = jug_esp_color;
        }
    }, esp_wait, {
        if (juggernaut_hrp_count)
        {
            //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 240, .w = shared_memory->WINDOW_W, .h = 40};
            NSRect frame = {.origin.x = shared_memory->WINDOW_H};
            frame.origin.x = 0;
            frame.origin.y = shared_memory->WINDOW_H - 240;
            frame.size.width = shared_memory->WINDOW_W;
            frame.size.height = 40;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            char text[MAX_ESP_TEXT_LENGTH];
            bzero(text, MAX_ESP_TEXT_LENGTH);
            sprintf(text, "%s%ld", "JUGGERNAUTS: ", juggernaut_hrp_count);
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = target_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
            for (int i = 0; i < juggernaut_hrp_count ; i++)
            {
                rbx_cframe_t cam_cf = rbx_camera_get_cframe(task, camera);
                float fov = rbx_camera_get_field_of_view(task, camera);
                rbx_draw_esp_box_on_3d_point(task, jug_cam_projections[i], cam_cf, shared_memory, 10, gas_mask_esp_color, fov, 60, 60, 0, 0, shared_memory->WINDOW_W, shared_memory->WINDOW_H, esp_index, false);
                char text[MAX_ESP_TEXT_LENGTH];
                bzero(text, MAX_ESP_TEXT_LENGTH);
                sprintf(text, "\n%.0f", rbx_humanoid_get_health(task, juggernaut_hums[i]));
                strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
                esp_index++;
            }
        }
       if (correctable_zombie_count)
        {
            //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 240, .w = shared_memory->WINDOW_W, .h = 40};
            NSRect frame = {.origin.x = shared_memory->WINDOW_H};
            frame.origin.x = 0;
            frame.origin.y = shared_memory->WINDOW_H - 260;
            frame.size.width = shared_memory->WINDOW_W;
            frame.size.height = 40;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            char text[MAX_ESP_TEXT_LENGTH];
            bzero(text, MAX_ESP_TEXT_LENGTH);
            sprintf(text, "%s%ld", "AIMBOT TARGETS: ", correctable_zombie_count);
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = energy_drink_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
        if (correctable_jug_count)
         {
             //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 240, .w = shared_memory->WINDOW_W, .h = 40};
             NSRect frame = {.origin.x = shared_memory->WINDOW_H};
             frame.origin.x = 0;
             frame.origin.y = shared_memory->WINDOW_H - 280;
             frame.size.width = shared_memory->WINDOW_W;
             frame.size.height = 40;
             shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
             char text[MAX_ESP_TEXT_LENGTH];
             bzero(text, MAX_ESP_TEXT_LENGTH);
             sprintf(text, "%s%d", "JUG SNIPER: ", correctable_jug_count);
             strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
             shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
             shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = gas_mask_esp_color;
             shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
             esp_index++;
         }
        //if (correctable_jug_count)
         {
             //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 240, .w = shared_memory->WINDOW_W, .h = 40};
             NSRect frame = {.origin.x = shared_memory->WINDOW_H};
             frame.origin.x = 0;
             frame.origin.y = shared_memory->WINDOW_H - 340;
             frame.size.width = shared_memory->WINDOW_W;
             frame.size.height = 40;
             shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
             char text[MAX_ESP_TEXT_LENGTH];
             bzero(text, MAX_ESP_TEXT_LENGTH);
             sprintf(text, "%s", message);
             strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
             shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
             shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = target_esp_color;
             shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
             esp_index++;
         }
        {
            //ESP_Frame frame = (ESP_Frame){.x = 0, .y = shared_memory->WINDOW_H - 240, .w = shared_memory->WINDOW_W, .h = 40};
            NSRect frame = {.origin.x = shared_memory->WINDOW_H};
            frame.origin.x = shared_memory->WINDOW_W - 220;
            frame.origin.y = shared_memory->WINDOW_H - 480;
            frame.size.width = 220;
            frame.size.height = 200;
            shared_memory->ESP_BOX_HIDDEN_ARRAY[esp_index] = false;
            char text[MAX_ESP_TEXT_LENGTH];
            bzero(text, MAX_ESP_TEXT_LENGTH);
            sprintf(text, "( %ld / %ld )   ALIVE\n\nDEAD:\n\n%s", alive_plr_count, total_plr_count, list_of_dead_plrs_str);
            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
            shared_memory->ESP_BOX_FRAME_ARRAY[esp_index] = frame;
            shared_memory->ESP_BOX_COLOR_ARRAY[esp_index] = medkit_esp_color;
            shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[esp_index] = 0;
            esp_index++;
        }
       
    } )
    
    
    static int old_count_limit = 0;
    

    
    
#pragma mark - stdin Inputs -
    
    
    
    
    
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("quit", printf("QUITTING...\n"); exit(0);)
        INSERT_STDIN_INPUT_SIMPLE("rageaim", min_aimbot_default_range = 0; printf(">> Rage Aim Enabled: Targeting all enemies regardless of distance\n"); )
        INSERT_STDIN_INPUT_SIMPLE("legitaim",min_aimbot_default_range = 2000; printf(">> Legit Aim Enabled: Targeting within realistic range only\n");)
        INSERT_STDIN_INPUT_SIMPLE("aimbot",aimbot_enabled = true; printf("ENABLED AIMBOT\n");)
        INSERT_STDIN_INPUT_SIMPLE("noaimbot",aimbot_enabled = false; printf("DISABLED AIMBOT\n");)
        INSERT_STDIN_INPUT_SIMPLE("fly", printf("ENABLED FLY\n"); fly_enabled = true;)
        INSERT_STDIN_INPUT_SIMPLE("nofly", printf("DISABLED FLY\n"); fly_enabled = false;)
        INSERT_STDIN_INPUT_SIMPLE("clip", printf("DISABLED NOCLIP\n"); noclip_enabled = false;)
        INSERT_STDIN_INPUT_SIMPLE("noclip", printf("ENABLED NOCLIP\n"); noclip_enabled = true;)
        INSERT_STDIN_INPUT_SIMPLE("loopgoto",loopgoto_enabled = true; printf("ENABLED LOOPGOTO\n");)
        INSERT_STDIN_INPUT_SIMPLE("noloopgoto",loopgoto_enabled = false; printf("DISABLED LOOPGOTO\n");)
        INSERT_STDIN_INPUT_SIMPLE("hackforts",hack_forts = true; printf("HACKING FORTIFICATIONS\n");)
        INSERT_STDIN_INPUT_SIMPLE("hackguns",hack_guns = true; printf("HACKING GUNS\n");)
        INSERT_STDIN_INPUT_SIMPLE("hackboth",hack_guns = true;hack_forts = true; printf("HACKING GUNS AND FORTS\n");)
        INSERT_STDIN_INPUT_SIMPLE("rpmhack",use_rpm_hack=true; printf("USING RPM HACK\n");)
        INSERT_STDIN_INPUT_SIMPLE("norpmhack",use_rpm_hack=false; printf("DISABLING RPM HACK\n");)
        INSERT_STDIN_INPUT_SIMPLE("savecf",if (my_hrp) {did_save_cframe = true; saved_cframe = rbx_basepart_get_cframe(task, my_hrp); printf("SAVED CFRAME\n'l' to goto.\n");vector3_t pos = saved_cframe.pos;
            printf("vector3_t pos = {\n\t.x = %f,\n\t.y = %f,\n\t.z = %f\n};\n", pos.x, pos.y, pos.z);})
        INSERT_STDIN_INPUT_SIMPLE("ft",
                                  if (my_hrp) {
            rbx_cframe_t cam_cf = rbx_camera_get_cframe(task, camera);
            rbx_cframe_t hrp_cf = rbx_basepart_get_cframe(task, my_hrp);
            hrp_cf.pos = vector3_add(hrp_cf.pos, vector3_mul_num(rbx_cframe_get_look_vector(cam_cf), 20));
            rbx_basepart_set_cframe(task, my_hrp, hrp_cf);
        })
        INSERT_STDIN_INPUT_SIMPLE("dealershipbot1", bot_mode = true; bot_pos = dealership_front_pos;printf("DEALERSHIP BOT 1\n");)
        INSERT_STDIN_INPUT_SIMPLE("dealershipbot2", bot_mode = true; bot_pos = dealership_back_pos;printf("DEALERSHIP BOT 2\n");)
        INSERT_STDIN_INPUT_SIMPLE("unbot",
                                  printf("SWITCHED AWAY FROM BOT MODE\n");
                                  bot_mode = false; esp_wait = 50; noclip_enabled = false; esp_enabled = true;)
        {
            INSERT_STDIN_MULTIARG_INPUT("setalpha %f", {printf("SET ESP ALPHA TO %f\n", esp_alpha);}, 1, &esp_alpha);
            INSERT_STDIN_MULTIARG_INPUT("espwait %d", {printf("SET ESP WAIT TO %d\n", esp_wait);}, 1, &esp_wait);
            INSERT_STDIN_MULTIARG_INPUT("aimbotcountlimit %d", {printf("SET AIMBOT COUNT LIMIT TO %d\n", aimbot_count_limit);}, 1, &aimbot_count_limit);
            INSERT_STDIN_MULTIARG_INPUT("prim %lf", {printf("SET AMMO[0] (PRIMARY) RESERVE TO %lf\n", ammo_sets[0]);}, 1, &ammo_sets[0]);
            INSERT_STDIN_MULTIARG_INPUT("sec %lf", {printf("SET AMMO[1] (SECONDARY) RESERVE TO %lf\n", ammo_sets[1]);}, 1, &ammo_sets[1]);
            INSERT_STDIN_MULTIARG_INPUT("flyspeed %f", {printf("SET FLY SPEED TO %f\n", fly_speed);}, 1, &fly_speed);
        }
    })
    
    
    
   
}
