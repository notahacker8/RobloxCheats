





void doors_cheat(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    printf("- DOORS (ESP) -\n");
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;) { pid_t a[4096]; if (pids_by_name("RobloxPlayer", a) == 0) { exit(0); } sleep(1); }
    });
    
    void* dlhandle = dlopen(__INJECTED_DYLIB_PATH__, RTLD_NOW);
    
    vm_address_t s_load_address = get_image_address(mach_task_self_, __INJECTED_DYLIB_PATH__);
    
    vm_offset_t esp_count_offset = gdso(dlhandle, s_load_address, "ESP_COUNT");
    vm_offset_t esp_enabled_offset = gdso(dlhandle, s_load_address, "ESP_ENABLED");
    vm_offset_t window_w_offset = gdso(dlhandle, s_load_address, "WINDOW_W");
    vm_offset_t window_h_offset = gdso(dlhandle, s_load_address, "WINDOW_H");
    vm_offset_t esp_usleep_time_offset = gdso(dlhandle, s_load_address, "ESP_USLEEP_TIME");
    
    vm_offset_t esp_box_hidden_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_HIDDEN_ARRAY");
    vm_offset_t esp_box_frame_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_FRAME_ARRAY");
    vm_offset_t esp_box_color_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_COLOR_ARRAY");
    vm_offset_t esp_box_text_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_TEXT_ARRAY");
    vm_offset_t esp_box_border_width_array_offset = gdso(dlhandle, s_load_address, "ESP_BOX_BORDER_WIDTH_ARRAY");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  get_image_address(task, __INJECTED_DYLIB_PATH__);
    
    char esp_enabled = true;
    vm_write(task, load_address + esp_enabled_offset, (vm_offset_t)&esp_enabled, 1);
    int esp_count = MAX_ESP_COUNT;
    vm_write(task, load_address + esp_count_offset, (vm_offset_t)&esp_count, 4);
    
    vm_address_t esp_box_hidden_array = load_address + esp_box_hidden_array_offset;
    vm_address_t esp_box_frame_array = load_address + esp_box_frame_array_offset;
    vm_address_t esp_box_color_array = load_address + esp_box_color_array_offset;
    vm_address_t esp_box_text_array = load_address + esp_box_text_array_offset;
    vm_address_t esp_box_border_width_array = load_address + esp_box_border_width_array_offset;
    
    
    useconds_t esput = 1000;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));
    
    
    static float window_w;
    static float window_h;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t lighting = rbx_instance_find_first_child_of_class(task, game, "Lighting");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    
    static ESP_Color door_esp_color = {.r = 0, .g = 0, .b = 1, .a = 1};
    static ESP_Color hint_book_esp_color = {.r = 0, .g = 1, .b = 1, .a = 1};
    static ESP_Color key_esp_color = {.r = 1, .g = 1, .b = 0,.a = 1};
    static ESP_Color lever_esp_color = {.r = 1, .g = 0.5, .b = 0, .a = 1};
    static ESP_Color monster_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color figure_esp_color = {.r = 1, .g = 0, .b = 0.8, .a = 1};
    static ESP_Color electrical_key_esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    
    static vector3_t new_aimbient = {.x = 1, .y = 1, .z = 1}; //Colors, but ok...
    
    static float camera_field_of_view = 70.f;
    
    static int esp_object_count = 0;
    static vm_address_t esp_parts[MAX_ESP_COUNT];
    static ESP_Color esp_colors[MAX_ESP_COUNT];
    
    static char current_event[100];
    static char* current_event_format = "";
    static char current_event_exists = false;
    
    static vm_address_t monster_part = 0;
    static float distance_from_monster = 0.0f;
    
    static useconds_t object_search_usleep_time = 100000;

    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (current_event_exists)
            {
                if (monster_part)
                {
                    rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                    rbx_cframe_t part_cframe = rbx_basepart_get_cframe(task, monster_part);
                    distance_from_monster = vector3_dist_dif(camera_cframe.pos, part_cframe.pos);
                    sprintf(current_event, current_event_format, distance_from_monster);
                }
            }
            usleep(10000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            camera_field_of_view = rbx_camera_get_field_of_view(task, camera);
            window_w = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_w_offset))).f;
            window_h = ((int_float_u)((int)vm_read_4byte_value(task, load_address + window_h_offset))).f;
            
            int esp_object_index = 0;
            
            vm_address_t rushmoving = rbx_instance_find_first_child(task, workspace, "RushMoving");
            vm_address_t ambushmoving = rbx_instance_find_first_child(task, workspace, "AmbushMoving");
            vm_address_t seekmoving = rbx_instance_find_first_child(task, workspace, "SeekMoving");

            if (rushmoving)
            {
                //rbx_print_descendants(task, rushmoving, 0, 1);
                monster_part = rbx_instance_find_first_child_of_class(task, rushmoving, "Part");
                current_event_format = "RUSH INCOMING - [%.0f]";
                current_event_exists = true;
            }
            if (ambushmoving)
            {
                monster_part = rbx_instance_find_first_child_of_class(task, ambushmoving, "Part");
                current_event_format = "AMBUSH INCOMING - [%.0f]";
                current_event_exists = true;
            }
            if (!rushmoving && !ambushmoving)
            {
                distance_from_monster = 0.0f;
                current_event_exists = false;
                monster_part = 0;
            }
            if (monster_part)
            {
                if (distance_from_monster < 500)
                {
                    esp_parts[esp_object_index] = monster_part;
                    esp_colors[esp_object_index] = monster_esp_color;
                    esp_object_index++;
                }
            }
            
            vm_address_t current_rooms_folder = rbx_instance_find_first_child(task, workspace, "CurrentRooms");
            vm_address_t final_room = rbx_instance_find_first_child(task, current_rooms_folder, "100");
            if (current_rooms_folder)
            {
                long current_rooms_count;
                rbx_child_t* current_rooms = rbx_instance_get_children(task, current_rooms_folder, &current_rooms_count);
                if (current_rooms)
                {
                    if (current_rooms_count > 1)
                    {
                        vm_address_t current_room = current_rooms[current_rooms_count - 2].child_address;
                        if (final_room)
                        {
                            current_room = final_room;
                        }
                        if (current_room)
                        {
                            //rbx_print_descendants(task, current_room, 0, 1);
                            vm_address_t door_model = rbx_instance_find_first_child(task, current_room, "Door");
                            if (door_model)
                            {
                                vm_address_t door = rbx_instance_find_first_child(task, door_model, "Door");
                                esp_parts[esp_object_index] = door;
                                esp_colors[esp_object_index] = door_esp_color;
                                esp_object_index++;
                            }
                            if (final_room)
                            {
                                long current_room_child_count = 0;
                                rbx_child_t* current_room_children = rbx_instance_get_children(task, current_room, &current_room_child_count);
                                if (current_room_children)
                                {
                                    for (long i = 0 ; i < current_room_child_count ; i++)
                                    {
                                        vm_address_t child = current_room_children[i].child_address;
                                        if (child)
                                        {
                                            unsigned char cnl = 0;
                                            char* child_name = rbx_instance_get_name(task, child, &cnl);
                                            if (child_name)
                                            {
                                                if (strcmp(child_name, "LiveBreakerPolePickup") == 0)
                                                {
                                                    vm_address_t base = rbx_instance_find_first_child(task, child, "Base");
                                                    esp_parts[esp_object_index] = base;
                                                    esp_colors[esp_object_index] = hint_book_esp_color;
                                                    esp_object_index++;
                                                }
                                                vm_deallocate(mach_task_self_, (vm_address_t)child_name, cnl);
                                            }
                                        }
                                        usleep(50);
                                    }
                                    vm_deallocate(mach_task_self_, (vm_address_t)current_room_children, current_room_child_count * sizeof(rbx_child_t));
                                }
                            }
                            
                            if (!seekmoving)
                            {
                                object_search_usleep_time = 100000;
                                
                                vm_address_t figure_setup_folder = rbx_instance_find_first_child(task, current_room, "FigureSetup");
                                vm_address_t figure_model = rbx_instance_find_first_child(task, figure_setup_folder, "FigureRagdoll");
                                if (figure_model)
                                {
                                    vm_address_t figure_part = rbx_instance_find_first_child(task, figure_model, "Root");
                                    esp_parts[esp_object_index] = figure_part;
                                    esp_colors[esp_object_index] = figure_esp_color;
                                    esp_object_index++;
                                }
                                
                                vm_address_t current_room_assets_folder = rbx_instance_find_first_child(task, current_room, "Assets");
                                vm_address_t lever_model = rbx_instance_find_first_child(task, current_room_assets_folder, "LeverForGate");
                                if (lever_model)
                                {
                                    vm_address_t lever = rbx_instance_find_first_child(task, lever_model, "Main");
                                    esp_parts[esp_object_index] = lever;
                                    esp_colors[esp_object_index] = lever_esp_color;
                                    esp_object_index++;
                                }
                                
                                vm_address_t key_obtain = rbx_instance_find_first_child(task, current_room_assets_folder, "KeyObtain");
                                if (key_obtain)
                                {
                                    vm_address_t key = rbx_instance_find_first_child(task, key_obtain, "Hitbox");
                                    esp_parts[esp_object_index] = key;
                                    esp_colors[esp_object_index] = key_esp_color;
                                    esp_object_index++;
                                }
                                
                                vm_address_t electrical_key_obtain = rbx_instance_find_first_child(task, current_room_assets_folder, "ElectricalKeyObtain");
                                if (electrical_key_obtain)
                                {
                                    vm_address_t electrical_key = rbx_instance_find_first_child(task, electrical_key_obtain, "Hitbox");
                                    esp_parts[esp_object_index] = electrical_key;
                                    esp_colors[esp_object_index] = electrical_key_esp_color;
                                    esp_object_index++;
                                }
                                
                                long current_room_asset_count = 0;
                                rbx_child_t* current_room_assets = rbx_instance_get_children(task, current_room_assets_folder, &current_room_asset_count);
                                if (current_room_assets)
                                {
                                    for (int i = 0 ; i < current_room_asset_count ; i++)
                                    {
                                        vm_address_t asset = current_room_assets[i].child_address;
                                        vm_address_t livehintbook = rbx_instance_find_first_child(task, asset, "LiveHintBook");
                                        vm_address_t livehintbookbase = rbx_instance_find_first_child(task, livehintbook, "Base");
                                        if (livehintbookbase)
                                        {
                                            esp_parts[esp_object_index] = livehintbookbase;
                                            esp_colors[esp_object_index] = hint_book_esp_color;
                                            esp_object_index++;
                                        }
                                        
                                        long asset_child_count = 0;
                                        rbx_child_t* asset_children = rbx_instance_get_children(task, asset, &asset_child_count);
                                        if (asset_children)
                                        {
                                            for (int _i = 0 ; _i < asset_child_count ; _i++)
                                            {
                                                vm_address_t asset_child = asset_children[_i].child_address;
                                                unsigned char nl = 0;
                                                char* name = rbx_instance_get_name(task, asset_child, &nl);
                                                if (name)
                                                {
                                                    vm_address_t container = 0;
                                                    if (strcmp(name, "DrawerContainer") == 0)
                                                    {
                                                        container = asset_child;
                                                    }
                                                    if (strcmp(name, "RolltopContainer") == 0)
                                                    {
                                                        container = asset_child;
                                                    }
                                                    vm_deallocate(mach_task_self_, (vm_address_t)name, nl);
                                                    
                                                    if (container)
                                                    {
                                                        key_obtain = rbx_instance_find_first_child(task, container, "KeyObtain");
                                                        if (key_obtain)
                                                        {
                                                            vm_address_t key = rbx_instance_find_first_child(task, key_obtain, "Hitbox");
                                                            esp_parts[esp_object_index] = key;
                                                            esp_colors[esp_object_index] = key_esp_color;
                                                            esp_object_index++;
                                                        }
                                                    }
                                                }
                                                usleep(50);
                                            }
                                            vm_deallocate(mach_task_self_, (vm_address_t)asset_children, asset_child_count * sizeof(rbx_child_t));
                                        }
                                        usleep(50);
                                    }
                                    vm_deallocate(mach_task_self_, (vm_address_t)current_room_assets, current_room_asset_count * sizeof(rbx_child_t));
                                }
                            }
                            else
                            {
                                object_search_usleep_time = 10000;
                            }
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)current_rooms, current_rooms_count * sizeof(rbx_child_t));
                }
            }
            esp_object_count = esp_object_index;
            bzero((char*)esp_parts + (esp_object_count * 8), sizeof(esp_parts) - (esp_object_count * 8));
            usleep(object_search_usleep_time);
        }
    });
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            int esp_index = 0;
            if (esp_object_count > 0 && esp_object_count < MAX_ESP_COUNT)
            {
                for (int i = 0 ; i < esp_object_count ; i++)
                {
                    vm_address_t part = esp_parts[i];
                    if (part)
                    {
                        vm_address_t parent = rbx_instance_get_parent(task, part);
                        vm_address_t instance_self = rbx_instance_get_self(task, part);
                        if (parent && instance_self == part)
                        {
                            //rbx_print_descendants(task, part, 0, 0);
                            ESP_Color color = esp_colors[i];
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            rbx_cframe_t part_cframe = rbx_basepart_get_cframe(task, part);
                            vector3_t part_size = rbx_basepart_get_size(task, part);
                            
                            rbx_draw_esp_box(task, part_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, 2, color, camera_field_of_view, sqrtf(part_size.x * part_size.x + part_size.z * part_size.z), part_size.y, 0, 0, window_w, window_h, esp_index, true);
                            char text[MAX_ESP_TEXT_LENGTH];
                            bzero(text, MAX_ESP_TEXT_LENGTH);
                            vm_write(task, esp_box_text_array + (esp_index * MAX_ESP_TEXT_LENGTH), (vm_address_t)text, MAX_ESP_TEXT_LENGTH);
                            esp_index++;
                        }
                    }
                    usleep(esput/esp_object_count);
                }
            }
            else
            {
                usleep(esput);
            }
            if (current_event_exists)
            {
                //Draw the message
                if (distance_from_monster < 500) //The monster is very far away when it's a false positive
                {
                    ESP_Frame frame = (ESP_Frame){.x = 0, . y = window_h - 200, .w = window_w, .h = 40};
                    char hidden = false;
                    float border_width = 0;
                    vm_write(task, esp_box_hidden_array + esp_index, (vm_address_t)&hidden, 1);
                    vm_write(task, esp_box_text_array + (esp_index * MAX_ESP_TEXT_LENGTH), (vm_address_t)current_event, MAX_ESP_TEXT_LENGTH);
                    vm_write(task, esp_box_frame_array + (esp_index * sizeof(ESP_Frame)), (vm_address_t)&frame, sizeof(ESP_Frame));
                    vm_write(task, esp_box_color_array + (esp_index * sizeof(ESP_Color)), (vm_address_t)&monster_esp_color, sizeof(ESP_Color));
                    vm_write(task, esp_box_border_width_array + (esp_index * 4), (vm_address_t)&border_width, 4);
                    esp_index++;
                }
            }
            char hiddens[MAX_ESP_COUNT - esp_index];
            memset(hiddens, 1, sizeof(hiddens));
            vm_write(task, esp_box_hidden_array + esp_index, (vm_offset_t)&hiddens, (int)sizeof(hiddens));
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            vm_write(task, lighting + RBX_LIGHTING_AMBIENT_OFFSET, (vm_offset_t)&new_aimbient, sizeof(vector3_t));
            //vm_write(task, lighting + RBX_LIGHTING_BRIGHTNESS_OFFSET, (vm_offset_t)&new_brightness, 4);
            usleep(50);
        }
    });

}
