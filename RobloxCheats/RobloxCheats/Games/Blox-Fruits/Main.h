

void blox_fruits_cheat(task_t task)
{
    
    printf("- BLOX FRUITS -\n");
    
    void* handle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB__);
    
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
    
    useconds_t esput = 1000;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players, "Player");
    
    static vm_address_t fruits[20];
    static vm_address_t fruit_parents[20];
    static ESP_Color blox_fruit_esp_color;
    blox_fruit_esp_color.r = 0;
    blox_fruit_esp_color.g = 1;
    blox_fruit_esp_color.b = 1;
    blox_fruit_esp_color.a = 1;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            vm_address_t read_data;
            mach_msg_type_number_t data_cnt;
            kern_return_t kr;
            kr = vm_read(task, load_address + window_w_offset, 4, &read_data, &data_cnt);
            if (kr == KERN_SUCCESS)
            {
                window_w = *(float*)read_data;
                vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
            }
            
            kr = vm_read(task, load_address + window_h_offset, 4, &read_data, &data_cnt);
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
            printf(" -> SEARCHING FOR FRUITS\n");
            long workspace_child_count = 0;
            rbx_child_t* workspace_children = rbx_instance_get_children(task, workspace, &workspace_child_count);
            int fruit_index = 0;
            if (workspace_children)
            {
                for (int i = 0 ; i < workspace_child_count ; i++)
                {
                    vm_address_t workspace_child = workspace_children[i].child_address;
                    vm_address_t fruit_parent = workspace_child;
                    vm_address_t fruit = rbx_instance_find_first_child(task, workspace_child, "Fruit");
                    vm_address_t handle = rbx_instance_find_first_child(task, workspace_child, "Handle");
                    if (fruit)
                    {
                        fruits[fruit_index] = handle;
                        fruit_parents[fruit_index] = fruit_parent;
                        fruit_index++;
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)workspace_children, workspace_child_count * sizeof(rbx_child_t));
                if (fruit_index < 21)
                {
                    bzero((void*)fruits + (fruit_index * 8), (20 - fruit_index) * 8);
                    bzero((void*)fruit_parents + (fruit_index * 8), (20 - fruit_index) * 8);
                }
            }
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            int esp_index = 0;
            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
            float fov = rbx_camera_get_field_of_view(task, camera);
            vm_address_t character = rbx_player_get_character(task, local_player);
            vm_address_t my_hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
            float new_gravity = 20;
            rbx_basepart_set_gravity(task, my_hrp, new_gravity);
            rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
            for (int i = 0 ; i < 20 ; i++)
            {
                vm_address_t fruit = fruits[i];
                if (fruit)
                {
                    rbx_cframe_t fruit_cframe = rbx_basepart_get_cframe(task, fruit);
                    float border_width = 2;
                    ESP_Frame frame = rbx_draw_esp_box(task, fruit_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, blox_fruit_esp_color, fov, 20, 20, 0, 0, window_w, window_h, (esp_index * 2), false);
                    vm_write(task, esp_box_border_width_array + (esp_index * 2 * 4), (vm_address_t)&border_width, 4);
                    
                    int __espi = (esp_index * 2) + 1;
                    border_width = 0;
                    rbx_draw_esp_box(task, fruit_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, blox_fruit_esp_color, fov, 300, 40, 0, 20 + frame.h, window_w, window_h, __espi, false);
                    vm_write(task, esp_box_border_width_array + (__espi * 4), (vm_address_t)&border_width, 4);
                    char dist_str[50];
                    char fnl = 0;
                    char* fruit_name = rbx_instance_get_name(task, fruit_parents[i], &fnl);
                    sprintf(dist_str, "[%s] %.0f", fruit_name, vector3_dist_dif(my_hrp_cframe.pos, fruit_cframe.pos));
                    vm_write(task, esp_box_text_array + (__espi * 50), (vm_address_t)dist_str, 50);
                    vm_deallocate(mach_task_self_, (vm_address_t)fruit_name, fnl);
                    printf("%s\n", dist_str);
                    
                    //STOP THE CHEAT IF A FRUIT IS FOUND THAT YOU PLAN ON PICKING UP!!!!!!!!!!!!!!!!!!!!
                    //rbx_print_children_profiles(task, fruits_parents[i]);
                    //rbx_camera_set_camera_subject(task, camera, character);
                    rbx_camera_set_camera_subject(task, camera, fruit);
                    sleep(1);
                    esp_index++;
                }
            }
            usleep(esput);
        }
    });
    
    

}
