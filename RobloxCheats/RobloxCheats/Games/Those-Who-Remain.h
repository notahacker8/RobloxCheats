



void those_who_remain_cheat(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    void* handle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    printf("%p\n", handle);
    printf("%s\n", dlerror());
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
    vm_offset_t aimbot_enabled_offset = gdso(handle, s_load_address, "AIMBOT_ENABLED");
    vm_offset_t esp_enabled_offset = gdso(handle, s_load_address, "ESP_ENABLED");
    vm_offset_t window_w_offset = gdso(handle, s_load_address, "WINDOW_W");
    vm_offset_t window_h_offset = gdso(handle, s_load_address, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = gdso(handle, s_load_address, "ESP_USLEEP_TIME");
    vm_offset_t esp_box_hidden_array_offset = gdso(handle, s_load_address, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = gdso(handle, s_load_address, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_box_color_array_offset = gdso(handle, s_load_address, "ESP_BOX_COLOR_ARRAY");
    vm_offset_t esp_box_text_array_offset = gdso(handle, s_load_address, "ESP_BOX_TEXT_ARRAY");
    vm_offset_t esp_box_border_width_array_offset = gdso(handle, s_load_address, "ESP_BOX_BORDER_WIDTH_ARRAY");
    
    dlclose(handle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB__);
    
    char esp_enabled = true;
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, 1);
    
    vm_address_t esp_box_hidden_array = load_address + esp_box_hidden_array_offset;
    vm_address_t esp_box_frame_array = load_address + esp_box_frame_array_offset;
    vm_address_t esp_box_color_array = load_address + esp_box_color_array_offset;
    vm_address_t esp_box_text_array = load_address + esp_box_text_array_offset;
    vm_address_t esp_box_border_width_array = load_address + esp_box_border_width_array_offset;
    
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
    static vm_address_t* items;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            ignore_folder = rbx_instance_find_first_child(task, workspace, "Ignore");
            items_folder = rbx_instance_find_first_child(task, ignore_folder, "Items");
            if (items)
            {
                free(items);
            }
            items = rbx_instance_get_children(task, items_folder, &item_count);
            
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
            
            sleep(1);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (item_count > 0)
            {
                usleep(esput/item_count);
                int esp_index = 0;
                for (long i = 0 ; i < item_count ; i++)
                {
                    usleep(esput/item_count);
                    if (i < MAX_ESP_COUNT)
                    {
                        vm_address_t box = 0;
                        vm_address_t item = 0;
                        char item_name_len = 0;
                        char* item_name = NULL;
                        if (items)
                        {
                            item = items[i];
                        }
                        
                        if (item)
                        {
                            item_name = rbx_instance_get_name(task, item, &item_name_len);
                        }
                        
                        ESP_Color color;
                        
                        if (item_name != NULL)
                        {
                            if (strcmp("Ammo", item_name) == 0)
                            {
                                box = rbx_instance_find_first_child(task, item, "Box");
                                
                                color.r = 0.2;
                                color.g = 0.5;
                                color.b = 0.2;
                                color.a = 1;
                            }
                            if (strcmp("Energy Drink", item_name) == 0)
                            {
                                box = rbx_instance_find_first_child(task, item, "Box");
                                
                                color.r = 0;
                                color.g = 1;
                                color.b = 1;
                                color.a = 1;
                            }
                            if (strcmp("Gas Mask", item_name) == 0)
                            {
                                box = rbx_instance_find_first_child(task, item, "Box");
                                
                                color.r = 0.5;
                                color.g = 0;
                                color.b = 0.5;
                                color.a = 1;
                            }
                            if (strcmp("Barbed Wire", item_name) == 0)
                            {
                                box = rbx_instance_find_first_child(task, item, "Box");
                                
                                color.r = 0.33;
                                color.g = 0.33;
                                color.b = 0.33;
                                color.a = 1;
                            }
                            if (strcmp("Medkit", item_name) == 0)
                            {
                                box = rbx_instance_find_first_child(task, item, "Box");
                                
                                color.r = 1;
                                color.g = 0;
                                color.b = 0;
                                color.a = 1;
                            }
                            if (strcmp("Molotov", item_name) == 0)
                            {
                                box = rbx_instance_find_first_child(task, item, "Box");
                                
                                color.r = 1;
                                color.g = 0.5;
                                color.b = 0;
                                color.a = 1;
                            }
                            if (strcmp("Bandages", item_name) == 0)
                            {
                                box = rbx_instance_find_first_child(task, item, "Box");
                                
                                color.r = 1;
                                color.g = 1;
                                color.b = 1;
                                color.a = 1;
                            }
                            vm_deallocate(mach_task_self_, (vm_address_t)item_name, item_name_len);
                        }
                        if (box != 0)
                        {
                            esp_index++;
                            rbx_cframe_t box_cframe = rbx_basepart_get_cframe(task, box);
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            rbx_draw_esp_box(task, box_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, color, fov, 3, 3, 0, 0, window_w, window_h, (int)esp_index, true);
                            float border_width = 2;
                            vm_write(task, esp_box_border_width_array + (esp_index * 4), (vm_address_t)&border_width, 4);
                        }
                    }
                }
                esp_index = 0;
            }
            else
            {
                sleep(1);
            }
        }
    });
    
    
    
    
}
