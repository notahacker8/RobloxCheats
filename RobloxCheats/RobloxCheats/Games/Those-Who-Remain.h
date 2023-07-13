


void those_who_remain_cheat(void)
{
    static mach_msg_type_number_t data_cnt;
    task_t task = get_roblox_task_port();
    printf("- THOSE WHO REMAIN (ESP) -\n");
    
    vm_address_t load_address = task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    if (!load_address)
    {
        printf("Couldn't find libESP.dylib in task %d\n", task);
        exit(0);
    }
    
    vm_offset_t esp_count_offset = file_gdso(__LIBESP_DYLIB_PATH__, "ESP_COUNT");
    vm_offset_t esp_enabled_offset = file_gdso(__LIBESP_DYLIB_PATH__, "ESP_ENABLED");
    vm_offset_t window_w_offset = file_gdso(__LIBESP_DYLIB_PATH__, "WINDOW_W");
    vm_offset_t window_h_offset = file_gdso(__LIBESP_DYLIB_PATH__, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = file_gdso(__LIBESP_DYLIB_PATH__, "ESP_USLEEP_TIME");
    
    vm_offset_t esp_box_hidden_array_offset = file_gdso(__LIBESP_DYLIB_PATH__, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = file_gdso(__LIBESP_DYLIB_PATH__, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_box_color_array_offset = file_gdso(__LIBESP_DYLIB_PATH__, "ESP_BOX_COLOR_ARRAY");
    vm_offset_t esp_box_text_array_offset = file_gdso(__LIBESP_DYLIB_PATH__, "ESP_BOX_TEXT_ARRAY");
    vm_offset_t esp_box_border_width_array_offset = file_gdso(__LIBESP_DYLIB_PATH__, "ESP_BOX_BORDER_WIDTH_ARRAY");
    
    vm_offset_t udp_sockaddr_in_offset = file_gdso(__LIBESP_DYLIB_PATH__, "UDP_SOCKADDR_IN");
    
    struct sockaddr_in __udp_sock_addr_in;
    bzero(&__udp_sock_addr_in, sizeof(__udp_sock_addr_in));
    
    {
        struct sockaddr_in* _saddr;
        if (vm_read(task, load_address + udp_sockaddr_in_offset, sizeof(struct sockaddr_in), (vm_address_t*)&_saddr, &data_cnt) == KERN_SUCCESS)
        {
            __udp_sock_addr_in = *_saddr;
            vm_deallocate(mach_task_self_, (vm_address_t)_saddr, sizeof(struct sockaddr_in));
        }
    }
    
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
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
    
    static int esp_part_count = 0;
    static ESP_Color esp_part_colors[MAX_ESP_COUNT];
    static char esp_part_names[MAX_ESP_COUNT * MAX_ESP_TEXT_LENGTH];
    static vm_address_t esp_parts[MAX_ESP_COUNT];
    
    static ESP_Color __new_esp_part_colors[MAX_ESP_COUNT];
    static char __new_esp_part_names[MAX_ESP_COUNT * MAX_ESP_TEXT_LENGTH];
    static vm_address_t __new_esp_parts[MAX_ESP_COUNT];
    
    static char esp_enabled = true;
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, (int)sizeof(char));
    int esp_count = MAX_ESP_COUNT;
    vm_write(task, load_address + esp_count_offset, (vm_offset_t)&esp_count, (int)sizeof(int));
    
    vm_address_t esp_box_text_array = load_address + esp_box_text_array_offset;
    vm_address_t esp_box_hidden_array = load_address + esp_box_hidden_array_offset;
    vm_address_t esp_box_frame_array = load_address + esp_box_frame_array_offset;
    vm_address_t esp_box_color_array = load_address + esp_box_color_array_offset;
    vm_address_t esp_box_border_width_array = load_address + esp_box_border_width_array_offset;
    
    static useconds_t esput = 2000;
    //vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    static float window_w;
    static float window_h;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    //vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    //vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    //rbx_print_descendants(task, workspace, 0, 3);
    
    static vm_address_t ignore_folder = 0;
    static vm_address_t items_folder = 0;
    
    static char show_ammo = true;
    static char show_fortifications = true;
    static char show_aid = true;
    static char show_gear = true;
    static char show_throwables = true;
    
    static float esp_alpha = 0.5;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            ignore_folder = rbx_instance_find_first_child(task, workspace, "Ignore");
            items_folder = rbx_instance_find_first_child(task, ignore_folder, "Items");
            //rbx_print_descendants(task, my_character, 0, 2);
            window_w = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_w_offset))).f;
            window_h = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_h_offset))).f;
            
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
                            vm_address_t box = rbx_instance_find_first_child(task, child, "Box");
                            __new_esp_parts[esp_part_index] = box;
                            __new_esp_part_colors[esp_part_index] = __color__;
                            bzero(__new_esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);
                            memcpy(__new_esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), name, name_len);
                            esp_part_index++;
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                    }
                    usleep(1000);
                }
                vm_deallocate(mach_task_self_, (vm_address_t)children, cc * sizeof(rbx_child_t));
            }
            esp_part_count = esp_part_index;
            memcpy(esp_parts, __new_esp_parts, sizeof(esp_parts));
            memcpy(esp_part_colors, __new_esp_part_colors, sizeof(esp_part_colors));
            memcpy(esp_part_names, __new_esp_part_names, sizeof(esp_part_names));
            usleep(100000);
        }
    });

    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            int esp_index = 0;
            int __esp_part_count = esp_part_count;
            if (__esp_part_count > 0 && __esp_part_count < MAX_ESP_COUNT)
            {
                for (int i = 0 ; i < __esp_part_count ; i++)
                {
                    vm_address_t part = esp_parts[i];
                    if (part)
                    {
                        vm_address_t parent = rbx_instance_get_parent(task, part);
                        vm_address_t instance_self = rbx_instance_get_self(task, part);
                        //rbx_print_descendants(task, instance_self, 0, 0);
                        if (parent && instance_self == part)
                        {
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            ESP_Color esp_color = esp_part_colors[i];
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            rbx_cframe_t part_cframe = rbx_basepart_get_cframe(task, part);
                            ESP_Frame scaled_frame = rbx_draw_esp_box(task, part_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, 2, esp_color, fov, 2, 2, 0, 0, window_w, window_h, esp_index, true);
                            esp_index++;
                            rbx_draw_esp_box(task, part_cframe.pos,
                                             camera_cframe, esp_box_hidden_array,
                                             esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, 0,
                                             esp_color, fov, 200, 40, 0, scaled_frame.h + 10, window_w, window_h, esp_index, false);
                            char* part_name = esp_part_names + (MAX_ESP_TEXT_LENGTH * i);
                            char text[MAX_ESP_TEXT_LENGTH];
                            bzero(text, MAX_ESP_TEXT_LENGTH);
                            sprintf(text, "%s [%.0f]", part_name, vector3_dist_dif(part_cframe.pos, camera_cframe.pos));
                            vm_write(task, esp_box_text_array + (esp_index * MAX_ESP_TEXT_LENGTH), (vm_address_t)text, MAX_ESP_TEXT_LENGTH);
                            esp_index++;
                        }
                    }
                    usleep(esput/__esp_part_count);
                }

                char hiddens[MAX_ESP_COUNT - esp_index];
                memset(hiddens, 1, sizeof(hiddens));
                vm_write(task, esp_box_hidden_array + esp_index, (vm_offset_t)&hiddens, (int)sizeof(hiddens));
            }
            else
            {
                usleep(esput);
            }
        }
    });
    
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    
    
    
    
    printf("List of stdin commands:\n");
    printf("\tsetalpha [(float) esp_color_alpha]\n");
    printf("\tquit\n");
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            char str[100];
            bzero(str, 100);
            fgets(str, 100, stdin);
            if (strncmp(str, "setalpha ", strlen("setalpha ")) == 0)
            {
                sscanf(str, "setalpha %f", &esp_alpha);
                printf("SET ESP ALPHA TO %f\n", esp_alpha);
            }
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
   
}
