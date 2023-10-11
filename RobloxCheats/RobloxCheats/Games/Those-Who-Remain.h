
void those_who_remain_tests(task_t task)
{
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t my_character = rbx_player_get_character(task, local_player);
    vm_address_t my_weapon = rbx_instance_find_first_child(task, camera, "MG 42");
    vm_address_t pp = vm_read_8byte_value(task, my_weapon + RBX_MODEL_PRIMARYPART_OFFSET);
    //vm_address_t pp = rbx_instance_find_first_child(task, my_weapon, "Pos");
    rbx_print_descendants(task, my_weapon, 0, 1);
    for (;;)
    {
        rbx_cframe_t cf = rbx_basepart_get_cframe(task, pp);
        cf.r20 = 0;
        cf.r21 = 1;
        cf.r22 = 0;
        rbx_basepart_set_cframe(task, pp, cf);
    }
}



void those_who_remain_cheat(task_t task)
{
    printf("- THOSE WHO REMAIN -\n");
    LIBESP_DYLIB_SETUP
    
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
    
    static useconds_t esput = 2000;
    
    shared_memory -> ESP_ENABLED = true;
    shared_memory -> ESP_COUNT = MAX_ESP_COUNT;
    shared_memory -> ESP_USLEEP_TIME = esput;
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    //vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    //vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    //rbx_print_descendants(task, game, 0, 2);
    
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
                            rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                            rbx_cframe_t part_cframe = rbx_basepart_get_cframe(task, part);
                            float dist = vector3_dist_dif(part_cframe.pos, camera_cframe.pos);
                            float fov = rbx_camera_get_field_of_view(task, camera);
                            ESP_Color esp_color = esp_part_colors[i];
                            float window_w = shared_memory->WINDOW_W;
                            float window_h = shared_memory->WINDOW_H;
                            ESP_Frame scaled_frame = rbx_draw_esp_box_on_3d_point(task, part_cframe.pos, camera_cframe, shared_memory, 2, esp_color, fov, 20, 20, 0, 0, window_w, window_h, esp_index, false);
                            esp_index++;
                            rbx_draw_esp_box_on_3d_point(task, part_cframe.pos,
                                             camera_cframe, shared_memory, 0,
                                             esp_color, fov, 500, 40, 0, scaled_frame.h + 10, window_w, window_h, esp_index, false);
                            char* part_name = esp_part_names + (MAX_ESP_TEXT_LENGTH * i);
                            char text[MAX_ESP_TEXT_LENGTH];
                            bzero(text, MAX_ESP_TEXT_LENGTH);
                            sprintf(text, "%s [%.0f]", part_name, dist);
                            //vm_write(task, dylib_esp_box_text_array + (esp_index * MAX_ESP_TEXT_LENGTH), (vm_address_t)text, MAX_ESP_TEXT_LENGTH);
                            strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);
                            esp_index++;
                        }
                    }
                    usleep(esput/__esp_part_count);
                }
                memset(shared_memory->ESP_BOX_HIDDEN_ARRAY + esp_index, 1, MAX_ESP_COUNT - esp_index);
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
    printf("\tesput [(int) esput]\n");
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
            if (strncmp(str, "esput ", strlen("esput ")) == 0)
            {
                sscanf(str, "esput %d", &esput);
                printf("SET ESP USLEEP TIME TO %d\n", esput);
            }
            if (strcmp(str, "quit\xa") == 0)
            {
                printf("QUITTING...\n");
                exit(0);
            }
        }
    });
   
}
