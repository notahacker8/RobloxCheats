


void stop_it_slender_cheat(task_t task)
{
    LIBESP_DYLIB_SETUP
    
    GENERIC_CHEAT_STARTER
    //rbx_print_descendants(task, teams_service, 0, 1);
    static  vm_address_t slender_team = 0;
    
    static vm_address_t map_model = 0;
    static vm_address_t gens_model = 0;
    
    //rbx_print_ancestors(task, 0x2ce5da580, 10);
    
    /*FOR_EACH_DESCENDANT(task, game, 100000, 10, i, {
        if (vm_read_uint64_t_value(task, obj) == task_base_address + RBX_OBJECT_ProximityPrompt_POINTEE_OFFSET)
        {
            rbx_print_ancestors(task, obj, 5);
        }
    })*/
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            map_model = rbx_instance_find_first_child(task, workspace, "MAP");
            gens_model = rbx_instance_find_first_child(task, map_model, "GENS");
            //rbx_print_descendants(task, gens_model, 0, 3);
            sleep(2);
        }
    });
    //vm_address_t pages_model = rbx_instance_find_first_child(task, map_model, "Pages");
    ESP_GLOBAL_VARIABLES
    
    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = 1000;
    
    shared_memory->INPUT_USLEEP_TIME = 10000;
    shared_memory->INPUT_COUNT = 6;
    
    printf("'m' to spam space\n");
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(500);
            if (shared_memory->KEYS_DOWN['m'])
            {
                send_keypress_yielding(shared_memory, 0, keycode_for_character(' '), " ", 200);
            }
        }
    });
    
    static ESP_Color page_esp_color = {.r = 0, .g = 1, .b = 0, .a = 0.5};
    static vm_address_t pages[100];
    static long page_count = 0;
    
    static ESP_Color generator_esp_color = {.r = 1, .g = 0, .b = 0.5, .a = 0.5};
    static vm_address_t generators[100];
    static long generator_count = 0;
    
    static ESP_Color slender_esp_color = {.r = 1, .g = 0, .b = 0, .a = 0.5};
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            slender_team = rbx_instance_find_first_child(task, teams_service, "Slender");
            
            int page_idx = 0;
            FOR_EACH_DESCENDANT(task, map_model, 100000, 3, i, {
                //if (rbx_instance_is_a(task, obj, "ClickDetector"))
                if (vm_read_uint64_t_value(task, obj) == task_base_address + RBX_OBJECT_ClickDetector_POINTEE_OFFSET)
                {
                    vm_address_t obj_p = rbx_instance_get_parent(task, obj);
                    if (rbx_instance_is_named_partially(task, obj_p, "Page"))
                    {
                        //rbx_print_ancestors(task, obj_p, 3);
                        pages[page_idx++] = obj_p;
                    }
                }
            })
            page_count = page_idx;
            
            int gen_idx = 0;
            FOR_EACH_CHILD(gens_model, i, {
                vm_address_t genbase = 0;
                FOR_EACH_CHILD(child, x, {
                    if (rbx_instance_is_a_basepart(task, child, task_base_address))
                    {
                        genbase = child;
                        x = child_count;
                    }
                })
                generators[gen_idx++] = genbase;
            })
            generator_count = gen_idx;
            
            sleep(3);
            
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            int esp_part_index = 0;
            for (int i = 0 ; i < page_count ; i++)
            {
                vm_address_t page = pages[i];
                //rbx_print_descendants(task, page, 0, 0);
                APPEND_ESP_OBJ(page, page_esp_color, "Page");
            }
            for (int i = 0 ; i < generator_count ; i++)
            {
                vm_address_t gen = generators[i];
                //rbx_print_descendants(task, page, 0, 0);
                APPEND_ESP_OBJ(gen, generator_esp_color, "Generator");
            }
            FOR_EACH_CHILD(players_service, i, {
                vm_address_t character = rbx_player_get_character(task, child);
                //rbx_print_descendants(task, character, 0, 1);
                vm_address_t team = rbx_player_get_team(task, child);
                vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
                if (team == slender_team)
                {
                    APPEND_ESP_OBJ(hrp, slender_esp_color, "SLENDER")
                }
            })
            
            esp_part_count = esp_part_index;
            sleep(1);
        }
    });
    
    
    BASIC_ESP_ENGINE({}, {
        should_scale = true;
        object_width  =2;
        object_height = 2;
    }, 100, {})
    
            
}
