





void doors_cheat(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    printf("- DOORS (ESP) -\n");
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
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    
    static ESP_Color door_esp_color = {.r = 0, .g = 0, .b = 1, .a = 1};
    static ESP_Color hint_book_esp_color = {.r = 0, .g = 1, .b = 1, .a = 1};
    static ESP_Color key_esp_color = {.r = 1, .g = 1, .b = 0,.a = 1};
    static ESP_Color lever_esp_color = {.r = 1, .g = 0.5, .b = 0, .a = 1};
    static ESP_Color monster_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color snare_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    static ESP_Color figure_esp_color = {.r = 1, .g = 0, .b = 0.8, .a = 1};
    static ESP_Color electrical_key_esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    
    static float camera_field_of_view;
    
    static int esp_object_count = 0;
    static vm_address_t esp_parts[MAX_ESP_COUNT];
    static ESP_Color esp_colors[MAX_ESP_COUNT];
    
    static char current_event[100];
    static char* current_event_format = "";
    static char current_event_exists = false;
    
    static vm_address_t monster_part = 0;
    static float distance_from_monster = 0.0f;
    
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
            window_w = ((int_float_u)(vm_read_4byte_value(task, load_address + window_w_offset))).f;
            window_h = ((int_float_u)(vm_read_4byte_value(task, load_address + window_h_offset))).f;
            int esp_object_index = 0;
            vm_address_t rushmoving = rbx_instance_find_first_child(task, workspace, "RushMoving");
            vm_address_t ambushmoving = rbx_instance_find_first_child(task, workspace, "AmbushMoving");
            if (rushmoving)
            {
                monster_part = rbx_instance_find_first_child_of_class(task, rushmoving, "Part");
                esp_parts[esp_object_index] = monster_part;
                esp_colors[esp_object_index] = monster_esp_color;
                esp_object_index++;
                current_event_format = "RUSH INCOMING - [%.0f]";
                current_event_exists = true;
            }
            if (ambushmoving)
            {
                monster_part = rbx_instance_find_first_child_of_class(task, ambushmoving, "Part");
                esp_parts[esp_object_index] = monster_part;
                esp_colors[esp_object_index] = monster_esp_color;
                esp_object_index++;
                current_event_format = "AMBUSH INCOMING - [%.0f]";
                current_event_exists = true;
            }
            if (!rushmoving && !ambushmoving)
            {
                current_event_exists = false;
                monster_part = 0;
            }
            vm_address_t current_rooms_folder = rbx_instance_find_first_child(task, workspace, "CurrentRooms");
            if (current_rooms_folder)
            {
                long current_rooms_count;
                rbx_child_t* current_rooms = rbx_instance_get_children(task, current_rooms_folder, &current_rooms_count);
                if (current_rooms)
                {
                    vm_address_t current_room = current_rooms[current_rooms_count - 2].child_address;
                    //rbx_print_descendants(task, current_room, 0, 0);
                    if (current_room)
                    {
                        vm_address_t door_model = rbx_instance_find_first_child(task, current_room, "Door");
                        if (door_model)
                        {
                            vm_address_t door = rbx_instance_find_first_child(task, door_model, "Door");
                            esp_parts[esp_object_index] = door;
                            esp_colors[esp_object_index] = door_esp_color;
                            esp_object_index++;
                        }
                        
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
                            vm_address_t electrical_key = rbx_instance_find_first_child(task, key_obtain, "Hitbox");
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
                                char asset_name_len = 0;
                                char* asset_name = rbx_instance_get_name(task, asset, &asset_name_len);
                                if (asset_name)
                                {
                                    if (strcmp(asset_name, "Snare") == 0)
                                    {
                                        vm_address_t snare_hitbox = rbx_instance_find_first_child(task, asset, "Hitbox");
                                        esp_parts[esp_object_index] = snare_hitbox;
                                        esp_colors[esp_object_index] = snare_esp_color;
                                        esp_object_index++;
                                    }
                                    vm_deallocate(mach_task_self_, (vm_address_t)asset_name, asset_name_len);
                                }
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
                                        char nl = 0;
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
                    vm_deallocate(mach_task_self_, (vm_address_t)current_rooms, current_rooms_count * sizeof(rbx_child_t));
                }
            }
            esp_object_count = esp_object_index;
            usleep(100000);
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
                    vm_address_t parent = rbx_instance_get_parent(task, part);
                    vm_address_t instance_self = vm_read_8byte_value(task, part + 8);
                    if (parent && instance_self)
                    {
                        //rbx_print_descendants(task, part, 0, 0);
                        ESP_Color color = esp_colors[i];
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        rbx_cframe_t part_cframe = rbx_basepart_get_cframe(task, part);
                        vector3_t part_size = rbx_basepart_get_size(task, part);
                        
                        rbx_draw_esp_box(task, part_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, 2, color, camera_field_of_view, sqrtf(part_size.x * part_size.x + part_size.z * part_size.z), part_size.y, 0, 0, window_w, window_h, esp_index, true);
                        char text[MAX_ESP_TEXT_LENGTH];
                        vm_write(task, esp_box_text_array + (esp_index * MAX_ESP_TEXT_LENGTH), (vm_address_t)text, MAX_ESP_TEXT_LENGTH);
                        esp_index++;
                        //printf("%f %f %f\n", part_size.x, part_size.y, part_size.z);
                    }
                    usleep(esput/esp_object_count);
                }
                //printf("%d, %d\n", esp_object_count, esp_index);
            }
            else
            {
                usleep(esput);
            }
            if (current_event_exists)
            {
                //Draw the message
                if (distance_from_monster < 999) //The monster is very far away when it's a false positive
                {
                    char hidden = false;
                    ESP_Frame frame;
                    frame.x = 0;
                    frame.y = window_h - 200;
                    frame.w = window_w;
                    frame.h = 40;
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

}



void doors_test(task_t task)
{
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t current_rooms_folder = rbx_instance_find_first_child(task, workspace, "CurrentRooms");
    //rbx_print_children_profiles(task, current_room);
    for (;;)
    {
        long current_rooms_count;
        vm_address_t* current_rooms = rbx_instance_get_children(task, current_rooms_folder, &current_rooms_count);
        vm_address_t current_room = current_rooms[current_rooms_count - 2];
        vm_address_t current_room_assets_folder = rbx_instance_find_first_child(task, current_room, "Assets");
        //rbx_print_descendants(task, current_room_assets_folder, 0);
        //rbx_print_children_profiles(task, workspace);
        free(current_rooms);
        sleep(1);
    }
    //rbx_print_descendants(task, 0x7fce40c58ae0, 0);
    //rbx_print_children_profiles(task, 0x7fce708e61e0); //Table
    //rbx_print_descendants(task, 0x7fce708e61e0, 0);
    //rbx_print_descendants(task, current_room_assets_folder, 0); //Assets
    /*
    rbx_print_children_profiles(task, 0x7fce56b15e60); //Rolltop_Desk
    rbx_print_children_profiles(task, 0x7fce42610340); //RolltopContainer
    rbx_print_children_profiles(task, 0x7fce420fabe0); //Vitamins -> Main [Part]
     */
    
    /*
     Rolltop_Desk -> RolltopContainer -> Vitamins -> [Part] Main
     Table -> DrawerContainer -> Lighter -> [Part] Main
     Dresser -> DrawerContainer -> Lighter -> [Part] Main
     */
    
    //0x7fce42610340
    /*
     [Model] Dresser (0x7fce740c9690) (child count: 5)
     [Model] Dresser (0x7fce53308d00) (child count: 5)
     [Model] Dresser (0x7fce752fe500) (child count: 5)
     [Model] Dresser (0x7fce40db1680) (child count: 5)
     [Model] Dresser (0x7fce54a3ae30) (child count: 5)
     [Model] Dresser (0x7fce7523bed0) (child count: 5)
     */
}
