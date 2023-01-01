


void doors_cheat(task_t task)
{
    static mach_msg_type_number_t data_cnt;
    printf("- DOORS -\n");
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
    
    
    useconds_t esput = 1000;
    vm_write(task, load_address + esp_usleep_time_offset, (vm_address_t)&esput, sizeof(useconds_t));

    static float window_w;
    static float window_h;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t my_character = rbx_player_get_character(task, local_player);
    vm_address_t my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
    
    static char current_event[100];
    static char* current_event_format = "";
    
    static vm_address_t current_room = 0;
    static vm_address_t door = 0;
    static vm_address_t hint_books[8];
    static vm_address_t key = 0;
    static vm_address_t monster_part = 0;
    static vm_address_t lever = 0;
    
    static ESP_Color door_esp_color;
    door_esp_color.r = 0;
    door_esp_color.g = 0;
    door_esp_color.b = 1;
    door_esp_color.a = 1;
    
    static ESP_Color hint_book_esp_color;
    hint_book_esp_color.r = 0;
    hint_book_esp_color.g = 1;
    hint_book_esp_color.b = 1;
    hint_book_esp_color.a = 1;
    
    static ESP_Color key_esp_color;
    key_esp_color.r = 1;
    key_esp_color.g = 1;
    key_esp_color.b = 0;
    key_esp_color.a = 1;
    
    static ESP_Color lever_esp_color;
    lever_esp_color.r = 1;
    lever_esp_color.g = 0.5;
    lever_esp_color.b = 0;
    lever_esp_color.a = 1;
    
    static ESP_Color monster_esp_color;
    monster_esp_color.r = 1;
    monster_esp_color.g = 0;
    monster_esp_color.b = 0;
    monster_esp_color.a = 1;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            
            vm_address_t rushmoving = rbx_instance_find_first_child(task, workspace, "RushMoving");
            vm_address_t ambushmoving = rbx_instance_find_first_child(task, workspace, "AmbushMoving");
            //rbx_print_children_profiles(task, workspace);
            
            char door_exists = false;
            char monster_exists = false;
            char key_exists = false;
            char lever_exists = false;
            
            if (rushmoving)
            {
                monster_part = rbx_instance_find_first_child_of_class(task, rushmoving, "Part");
                current_event_format = "RUSH INCOMING - [%.0f]";
                monster_exists = true;
            }
            if (ambushmoving)
            {
                monster_part = rbx_instance_find_first_child_of_class(task, ambushmoving, "Part");
                current_event_format = "AMBUSH INCOMING - [%.0f]";
                monster_exists = true;
            }
            
            vm_address_t current_rooms_folder = rbx_instance_find_first_child(task, workspace, "CurrentRooms");
            if (current_rooms_folder)
            {
                long current_rooms_count;
                rbx_child_t* current_rooms = rbx_instance_get_children(task, current_rooms_folder, &current_rooms_count);
                if (current_rooms)
                {
                    current_room = current_rooms[current_rooms_count - 2].child_address;
                    if (current_room)
                    {
                        vm_address_t door_model = rbx_instance_find_first_child(task, current_room, "Door");
                        if (door_model)
                        {
                            door = rbx_instance_find_first_child(task, door_model, "Door");
                            door_exists = true;
                        }
                        vm_address_t current_room_assets_folder = rbx_instance_find_first_child(task, current_room, "Assets");
                        vm_address_t lever_model = rbx_instance_find_first_child(task, current_room_assets_folder, "LeverForGate");
                        if (lever_model)
                        {
                            lever = rbx_instance_find_first_child(task, lever_model, "Main");
                            lever_exists = true;
                        }
                        vm_address_t key_obtain = rbx_instance_find_first_child(task, current_room_assets_folder, "KeyObtain");
                        if (key_obtain)
                        {
                            key_exists = true;
                            key = rbx_instance_find_first_child(task, key_obtain, "Hitbox");
                        }
                        
                        long current_room_asset_count = 0;
                        rbx_child_t* current_room_assets = rbx_instance_get_children(task, current_room_assets_folder, &current_room_asset_count);
                        if (current_room_assets)
                        {
                            int hint_book_index = 0;
                            for (int i = 0 ; i < current_room_asset_count ; i++)
                            {
                                vm_address_t asset = current_room_assets[i].child_address;
                                vm_address_t hintbook = rbx_instance_find_first_child(task, asset, "HintBook");
                                vm_address_t livehintbook = rbx_instance_find_first_child(task, asset, "LiveHintBook");
                                if (hintbook)
                                {
                                    if (livehintbook)
                                    {
                                        hint_books[hint_book_index] = hintbook;
                                    }
                                    else //this is to avoid "dead" esp, or when the esp is outlining an object we don't need
                                    {
                                        hint_books[hint_book_index] = 0;
                                    }
                                    hint_book_index++;
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
                                        vm_address_t container = 0;
                                        if (strcmp(name, "DrawerContainer") == 0)
                                        {
                                            container = asset_child;
                                        }
                                        if (strcmp(name, "RolltopContainer") == 0)
                                        {
                                            container = asset_child;
                                        }
                                        if (container)
                                        {
                                            key_obtain = rbx_instance_find_first_child(task, container, "KeyObtain");
                                            if (key_obtain)
                                            {
                                                key_exists = true;
                                                key = rbx_instance_find_first_child(task, key_obtain, "Hitbox");
                                            }
                                        }
                                        vm_deallocate(mach_task_self_, (vm_address_t)name, nl);
                                        usleep(100);
                                    }
                                    vm_deallocate(mach_task_self_, (vm_address_t)asset_children, asset_child_count * sizeof(rbx_child_t));
                                }
                                usleep(100);
                            }
                            vm_deallocate(mach_task_self_, (vm_address_t)current_room_assets, current_room_asset_count * sizeof(rbx_child_t));
                        }
                    }
                    vm_deallocate(mach_task_self_, (vm_address_t)current_rooms, current_rooms_count * sizeof(rbx_child_t));
                }
            }
            
            if (door_exists == false)
            {
                door = 0;
            }
            if (key_exists == false)
            {
                key = 0;
            }
            if (lever_exists == false)
            {
                lever = 0;
            }
            if (monster_exists == false)
            {
                monster_part = 0;
                current_event_format = "";
                bzero(current_event, sizeof(current_event));
            }
            
            kern_return_t kr;
            vm_address_t read_data;
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

            usleep(1000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
            float fov = rbx_camera_get_field_of_view(task, camera);
            float border_width = 2;
            
            int esp_index = 0;
            if (door)
            {
                vm_address_t parent = rbx_instance_get_parent(task, door);
                if (parent)
                {
                    camera_cframe = rbx_camera_get_cframe(task, camera);
                    rbx_cframe_t door_cframe = rbx_basepart_get_cframe(task, door);
                    rbx_draw_esp_box(task, door_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, border_width, door_esp_color, fov, 5, 7.75, 0, 0, window_w, window_h, esp_index, true);
                    esp_index++;
                }
            }
            
            for (int i = 0 ; i < 8 ; i++)
            {
                vm_address_t hint_book = hint_books[i];
                if (hint_book)
                {
                    vm_address_t parent = rbx_instance_get_parent(task, hint_book);
                    if (parent)
                    {
                        camera_cframe = rbx_camera_get_cframe(task, camera);
                        rbx_cframe_t hint_book_cframe = rbx_basepart_get_cframe(task, hint_book);
                        rbx_draw_esp_box(task, hint_book_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, border_width, hint_book_esp_color, fov, 0.75, 2, 0, 0, window_w, window_h, esp_index, true);
                        esp_index++;
                    }
                }
            }
            
            if (key)
            {
                vm_address_t parent = rbx_instance_get_parent(task, key);
                if (parent)
                {
                    camera_cframe = rbx_camera_get_cframe(task, camera);
                    rbx_cframe_t key_cframe = rbx_basepart_get_cframe(task, key);
                    rbx_draw_esp_box(task, key_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, border_width, key_esp_color, fov, 2, 2, 0, 0, window_w, window_h, esp_index, true);
                    esp_index++;
                }
            }
            
            if (lever)
            {
                vm_address_t parent = rbx_instance_get_parent(task, lever);
                if (parent)
                {
                    camera_cframe = rbx_camera_get_cframe(task, camera);
                    rbx_cframe_t lever_cframe = rbx_basepart_get_cframe(task, lever);
                    rbx_draw_esp_box(task, lever_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, border_width,lever_esp_color, fov, 1.5, 2, 0, 0, window_w, window_h, esp_index, true);
                    esp_index++;
                }
            }
            
            char hidden = false;
            ESP_Frame frame;
            frame.x = 0;
            frame.y = window_h - 200;
            frame.w = window_w;
            frame.h = 40;
            border_width = 0;
            vm_write(task, esp_box_hidden_array + esp_index, (vm_address_t)&hidden, 1);
            vm_write(task, esp_box_text_array + (esp_index * MAX_ESP_TEXT_SIZE), (vm_address_t)current_event, MAX_ESP_TEXT_SIZE);
            vm_write(task, esp_box_frame_array + (esp_index * sizeof(ESP_Frame)), (vm_address_t)&frame, sizeof(ESP_Frame));
            vm_write(task, esp_box_color_array + (esp_index * sizeof(ESP_Color)), (vm_address_t)&monster_esp_color, sizeof(ESP_Color));
            vm_write(task, esp_box_border_width_array + (esp_index * 4), (vm_address_t)&border_width, 4);
            esp_index++;
            border_width = 2;
            
            if (monster_part)
            {
                vm_address_t parent = rbx_instance_get_parent(task, monster_part);
                if (parent)
                {
                    camera_cframe = rbx_camera_get_cframe(task, camera);
                    rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);
                    rbx_cframe_t monster_cframe = rbx_basepart_get_cframe(task, monster_part);
                    rbx_draw_esp_box(task, monster_cframe.pos, camera_cframe, esp_box_hidden_array, esp_box_frame_array, esp_box_color_array, esp_box_border_width_array, border_width, monster_esp_color, fov, 5, 5, 0, 0, window_w, window_h, esp_index, true);
                    float dist = vector3_dist_dif(my_hrp_cframe.pos, monster_cframe.pos);
                    sprintf(current_event, current_event_format, dist);
                    esp_index++;
                }
            }
            
            char hiddens[MAX_ESP_COUNT - esp_index];
            memset(hiddens, 1, sizeof(hiddens));
            vm_write(task, esp_box_hidden_array + esp_index, (vm_address_t)hiddens, (int)sizeof(hiddens));
            
            //printf(current_event);
            usleep(esput);
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
