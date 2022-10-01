


void those_who_remain_cheat(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    void* handle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
    vm_offset_t aimbot_enabled_offset = gdso(handle, s_load_address, "AIMBOT_ENABLED");
    vm_offset_t esp_enabled_offset = gdso(handle, s_load_address, "ESP_ENABLED");
    vm_offset_t max_esp_count_offset = gdso(handle, s_load_address, "MAX_ESP_COUNT");
    vm_offset_t aimbot_x_offset = gdso(handle, s_load_address, "AIMBOT_X");
    vm_offset_t aimbot_y_offset = gdso(handle, s_load_address, "AIMBOT_Y");
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
    static vm_address_t ignore_folder = 0;
    static vm_address_t items_folder = 0;
    useconds_t esput = 1000;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    static long item_count = 0;
    static vm_address_t items[100];
    
    static ESP_Frame frame;
    static ESP_Color color;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            ignore_folder = rbx_instance_find_first_child(task, workspace, "Ignore");
            items_folder = rbx_instance_find_first_child(task, ignore_folder, "Items");
            rbx_instance_get_children_v2(task, items_folder, items, &item_count, 100);
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
    for (;;)
    {
            for (long i = 0 ; i < item_count ; i++)
            {
                if (i < 100)
                {
                    usleep(esput/item_count);
                    vm_address_t item = items[i];
                    vm_address_t box = 0;
                    
                    char item_name_len = 0;
                    char* item_name = NULL;
                    
                    if (item)
                    {
                        item_name = rbx_instance_get_name(task, item, &item_name_len);
                    }
                    
                    if (item_name != NULL)
                    {
                        if (strcmp("Energy Drink", item_name) == 0)
                        {
                            box = rbx_instance_find_first_child(task, item, "Box");
                            
                            color.r = 0;
                            color.g = 0.5;
                            color.b = 0.5;
                            color.a = 1;
                        }
                        if (strcmp("Gas Mask", item_name) == 0)
                        {
                            box = rbx_instance_find_first_child(task, item, "Box");
                            
                            color.r = 0;
                            color.g = 0;
                            color.b = 0;
                            color.a = 1;
                        }
                        if (strcmp("Barbed Wire", item_name) == 0)
                        {
                            box = rbx_instance_find_first_child(task, item, "Box");
                            
                            color.r = 0.35;
                            color.g = 0.35;
                            color.b = 0.3;
                            color.a = 1;
                        }
                    }
                    if (box != 0)
                    {
                        rbx_cframe_t box_cframe = rbx_basepart_get_cframe(task, box);
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        custom_rbx_world_to_screen_info_t wtsi;
                        
                        vm_read(task, load_address + window_w_offset, 4, &read_data, &data_cnt);
                        window_w = *(float*)read_data;
                        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
                        
                        vm_read(task, load_address + window_h_offset, 4, &read_data, &data_cnt);
                        window_h = *(float*)read_data;
                        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
                        
                        float fov = rbx_camera_get_field_of_view(task, camera);
                        
                        wtsi = rbx_world_to_screen_point(camera_cframe, fov, box_cframe.pos, window_w, window_h);
                        
                        static char hidden = true;
                        static float max_mag = 300;
                        static float min_mag = 10;
                        static float max_resize = 30;
                        float resize_factor = (max_resize/(max_mag - min_mag));
                        
                        static float resize = 0;
                        static float mag = 0;
                        
                        if (isnan(wtsi.x) || isnan(wtsi.y))
                        {
                            hidden = true;
                            wtsi.x = 0;
                            wtsi.y = 0;
                            vm_write(task, esp_box_hidden_array + i, (vm_offset_t)&hidden, 1);
                        }
                        else
                        {
                            hidden = false;
                            vm_write(task, esp_box_hidden_array + i, (vm_offset_t)&hidden, 1);
                            
                            vm_offset_t f_o = (i * sizeof(ESP_Frame));
                            vm_offset_t c_o = (i * sizeof(ESP_Color));
                            mag = wtsi.magnitude;
                            if (wtsi.magnitude > max_mag)
                            {
                                mag = max_mag;
                            }
                            if (wtsi.magnitude < min_mag)
                            {
                                mag = min_mag;
                            }
                           // printf("%f\n", mag);
                            resize = (((max_mag - min_mag) - mag) * resize_factor);
                            
                            
                            frame.x = wtsi.x - (4 + resize/2);
                            frame.y = wtsi.y -  (4 + resize/2);
                            frame.w = 8 + resize;
                            frame.h = 8 + resize;
                            
                            vm_write(task, esp_box_frame_array + f_o, (vm_address_t)&frame, sizeof(ESP_Frame));
                            vm_write(task, esp_box_color_array + c_o, (vm_address_t)&color, sizeof(ESP_Color));
                        }
                        
                        box = 0;
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)item_name, item_name_len);
                }
                else
                {
                    usleep(999999);
                }
            }
    }
    });
}
