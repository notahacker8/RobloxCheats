


void field_of_battle_cheat(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    void* handle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    printf("%p\n", handle);
    printf("%s\n", dlerror());
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
    vm_offset_t esp_enabled_offset = gdso(handle, s_load_address, "ESP_ENABLED");
    vm_offset_t max_esp_count_offset = gdso(handle, s_load_address, "MAX_ESP_COUNT");
    vm_offset_t window_w_offset = gdso(handle, s_load_address, "WINDOW_W");
    vm_offset_t window_h_offset = gdso(handle, s_load_address, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = gdso(handle, s_load_address, "ESP_USLEEP_TIME");
    vm_offset_t esp_box_hidden_array_offset = gdso(handle, s_load_address, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = gdso(handle, s_load_address, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_box_color_array_offset = gdso(handle, s_load_address, "ESP_BOX_COLOR_ARRAY");
    
    dlclose(handle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB__);
    
    char esp_enabled = true;
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, 1);
    
    vm_address_t esp_box_hidden_array = load_address + esp_box_hidden_array_offset;
    vm_address_t esp_box_frame_array = load_address + esp_box_frame_array_offset;
    vm_address_t esp_box_color_array = load_address + esp_box_color_array_offset;
    
    static float window_w;
    static float window_h;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t projectiles_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Projectiles");
    
    static char* gem_dictionary[60];
    gem_dictionary[4] = "Benitoite";
    gem_dictionary[5] = "Tanzanite";
    gem_dictionary[6] = "Alexandrite";
    gem_dictionary[7] = "Diamond";
    gem_dictionary[8] = "Sapphire";
    gem_dictionary[9] = "Emerald";
    gem_dictionary[10] = "Ruby";
    gem_dictionary[11] = "Lapis Lazuli";
    gem_dictionary[12] = "Topaz";
    gem_dictionary[13] = "Garnet";
    gem_dictionary[14] = "Aquamarine";
    gem_dictionary[15] = "Spinel";
    gem_dictionary[16] = "Amber";
    gem_dictionary[17] = "Titanite";
    gem_dictionary[18] = "Tourmaline";
    gem_dictionary[19] = "Kunzite";
    gem_dictionary[20] = "Amethyst";
    gem_dictionary[21] = "Citrine";
    gem_dictionary[22] = "Peridot";
    gem_dictionary[24] = "Onyx";
    gem_dictionary[25] = "Turquoise";
    gem_dictionary[27] = "Feldspar";
    gem_dictionary[28] = "Jade";
    gem_dictionary[29] = "Nephrite";
    gem_dictionary[30] = "Olivine";
    gem_dictionary[31] = "Copal";
    gem_dictionary[32] = "Mithril";
    gem_dictionary[42] = "Titan Core";
    
    //rbx_print_descendants(task, unbreakable_folder, 0);
    //rbx_print_children_profiles(task, unbreakable_folder);
    
    /*
    long unbreakable_count = 0;
    vm_address_t* unbreakables = rbx_instance_get_children(task, unbreakable_folder, &unbreakable_count);
    for (long i = 0 ; i < unbreakable_count ; i++)
    {
        vm_address_t unbreakable = unbreakables[i];
        char* u_name;
        char u_name_len;
        if (unbreakable)
        {
            if (strcmp(rbx_instance_get_name(task, unbreakable, &u_name_len), "Ore") == 0)
            {
                long child_count = 0;
                vm_address_t* children = rbx_instance_get_children(task, unbreakable, &child_count);
                for (long x = 0 ; x < child_count ; x++)
                {
                    vm_address_t child = children[x];
                    if (child)
                    {
                        char* c_name;
                        char c_name_len;
                        if (strcmp(rbx_instance_get_name(task, child, &c_name_len), "Gem") == 0)
                        {
                            vm_address_t gem = child;
                            vm_address_t hitby = rbx_instance_find_first_child(task, gem, "HitBy");
                            vm_address_t gemtype = rbx_instance_find_first_child(task, gem, "GemType");
                            long gtv = rbx_intvalue_get_value(task, gemtype);
                            if (gem_dictionary[gtv] == NULL)
                            {
                                if (gtv != 0)
                                {
                                    printf("unlisted gem type: %ld\n", gtv);
                                }
                            }
                            if (hitby)
                            {
                                vm_address_t hitby_value = rbx_intvalue_get_value(task, hitby);
                                char pnl;
                                char* player_name = rbx_instance_get_name(task, hitby_value, &pnl);
                                if (player_name)
                                {
                                    if (strcmp(player_name, "blockmincer") == 0)
                                    {
                                        printf("GemType: %ld (%s)\n", gtv, gem_dictionary[gtv]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
     */
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            //printf("GEMS: \n");
            long projectile_count = 0;
            vm_address_t* projectiles = rbx_instance_get_children(task, projectiles_folder, &projectile_count);
            if (projectiles)
            {
                for (long i = 0 ; i < projectile_count ; i++)
                {
                    vm_address_t projectile = projectiles[i];
                    vm_address_t gem_type = rbx_instance_find_first_child(task, projectile, "GemType");
                    if (gem_type)
                    {
                        long gv = rbx_intvalue_get_value(task, gem_type);
                        //printf("%ld\n", rbx_intvalue_get_value(task, gem_type));
                        if (gv == 42)
                        {
                            printf("<b>LEGENDARY FOUND</b><br>\n");
                        }
                        if (gv > 31 && gv < 37)
                        {
                            printf("<b>LEGENDARY FOUND</b><br>\n");
                        }
                        rbx_rgb_t gem_color = rbx_basepart_get_color(task, projectile);
                        printf("<b style=\"color:#%02hhx%02hhx%02hhx\">%ld (%s)</b></br>\n", gem_color.r, gem_color.g, gem_color.b, gv, gem_dictionary[gv]);
                    }
                }
                free(projectiles);
            }
            //printf("\n");
            sleep(2);
        }
    });
    
    /*
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            //printf("GEMS: \n");
            long projectile_count = 0;
            vm_address_t* projectiles = rbx_instance_get_children(task, projectiles_folder, &projectile_count);
            if (projectiles)
            {
                for (long i = 0 ; i < projectile_count ; i++)
                {
                    vm_address_t projectile = projectiles[i];
                    vm_address_t gem_type = rbx_instance_find_first_child(task, projectile, "GemType");
                    if (gem_type)
                    {
                        long gv = rbx_intvalue_get_value(task, gem_type);
                        vm_address_t legendary_gem = 0;
                        if (gv == 42)
                        {
                            legendary_gem = projectile;
                        }
                        if (gv > 31 && gv < 37)
                        {
                            legendary_gem = projectile;
                        }
                        if (legendary_gem)
                        {
                            printf("<b>%s</b><br>\n", "LEGENDARY_FOUND");
                            rbx_cframe_t gem_cframe = rbx_basepart_get_cframe(task, legendary_gem);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            custom_rbx_world_to_screen_info_t wtsi;
                            
                            kern_return_t kr;
                            kr = vm_read(task, load_address + window_w_offset, 4, &read_data, &data_cnt);
                            if (kr == KERN_SUCCESS)
                            {
                                window_w = *(float*)read_data;
                                vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
                            }
                            
                            vm_read(task, load_address + window_h_offset, 4, &read_data, &data_cnt);
                            if (kr == KERN_SUCCESS)
                            {
                                window_h = *(float*)read_data;
                                vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
                            }
                            
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            wtsi = rbx_world_to_screen_point(camera_cframe, fov, gem_cframe.pos, window_w, window_h);
                            char hidden = true;
                            if (isnan(wtsi.x) || isnan(wtsi.y))
                            {
                                hidden = true;
                                wtsi.x = 0;
                                wtsi.y = 0;
                                vm_write(task, esp_box_hidden_array, (vm_offset_t)&hidden, 1);
                            }
                            else
                            {
                                if (isfinite(wtsi.x) && isfinite(wtsi.y))
                                {
                                    hidden = false;
                                    vm_write(task, esp_box_hidden_array, (vm_offset_t)&hidden, 1);
                                    
                                    ESP_Frame frame;
                                    frame.x = wtsi.x - 10;
                                    frame.y = wtsi.y - 10;
                                    frame.w = 20;
                                    frame.h = 20;
                                    
                                    ESP_Color color;
                                    color.r = 0;
                                    color.g = 1;
                                    color.b = 0;
                                    color.a = 1;
                                    
                                    vm_write(task, esp_box_frame_array, (vm_address_t)&frame, sizeof(ESP_Frame));
                                    vm_write(task, esp_box_color_array, (vm_address_t)&color, sizeof(ESP_Color));
                                }
                            }
                        }
                    }
                }
                free(projectiles);
            }
            usleep(1000);
        }
    });
    */
    
}
