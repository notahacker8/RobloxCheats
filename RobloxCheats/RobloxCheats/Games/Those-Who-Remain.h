
void those_who_remain_tests(task_t task)
{
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    vm_address_t my_character = rbx_player_get_character(task, local_player);
    vm_address_t my_weapon = rbx_instance_find_first_child(task, camera, "MG 42");
    vm_address_t pp = vm_read_uint64_t_value(task, my_weapon + RBX_MODEL_PRIMARYPART_OFFSET);
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
    
    ESP_GLOBAL_VARIABLES
    
    static char esp_enabled = true;
    static useconds_t esput = 1000;
    
    shared_memory -> ESP_ENABLED = esp_enabled;
    shared_memory -> ESP_COUNT = MAX_ESP_COUNT;
    shared_memory -> ESP_USLEEP_TIME = esput;
    strcpy(shared_memory->ESP_TEXT_FONT_NAME, "Arial");
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static vm_address_t my_character = 0;
    static vm_address_t my_hrp = 0;
    
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
            my_character = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");
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
                            APPEND_ESP_OBJ(box, __color__, name);
                        }
                        vm_deallocate(mach_task_self_, (vm_address_t)name, name_len);
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)children, cc * sizeof(rbx_child_t));
            }
            esp_part_count = esp_part_index;
            usleep(100000);
        }
    });

    
    BASIC_ESP_ENGINE({}, 30, {})
    
    
    
    
    
    
#pragma mark - stdin Inputs -
    
    
    
    
    
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("quit", printf("QUITTING...\n"); exit(0);)
        {
            INSERT_STDIN_MULTIARG_INPUT("setalpha %f", {printf("SET ESP ALPHA TO %f\n", esp_alpha);}, 1, &esp_alpha);
        }
    })
   
}
