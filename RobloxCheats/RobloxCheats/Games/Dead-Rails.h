




void dead_rails_cheat(task_t task)
{
    printf("\n- DEAD RAILS -\n");
    
    LIBESP_DYLIB_SETUP
    GENERIC_CHEAT_STARTER
    
    rbx_print_descendants(task, workspace, 0, 1);
    
    vm_address_t runtimeitems_folder = rbx_instance_find_first_child(task, workspace, "RuntimeItems");
    
    ESP_GLOBAL_VARIABLES
    ESP_Color item_esp_color = {.r = 1, .g = 0, .b = 0, .a = 0.5};
    ESP_Color valuable_esp_color = {.r = 0, .g = 1, .b = 0, .a = 0.5};
    ESP_Color gun_esp_color = {.r = 0, .g = 0, .b = 1, .a = 0.5};
    ESP_Color bond_esp_color = {.r = 1, .g = 0.8, .b = 0, .a = 0.5};
    ESP_Color consumable_esp_color = {.r = 1, .g = 0, .b = 0.75, .a = 0.5};
    
    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = 500;
    
    static char show_junk = true;
    
    BASIC_ESP_ENGINE({}, {}, 30, {});
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
        int esp_part_index = 0;
        FOR_EACH_CHILD(runtimeitems_folder, i, {
            char append = true;
            ESP_Color esp_color = item_esp_color;
            long name_l = 0;
            char* name = rbx_instance_get_name(task, child, &name_l);
            if (name)
            {
                vm_address_t bbg = rbx_instance_find_first_child_by_pointee(task, child, task_base_address, RBX_OBJECT_BillboardGui_POINTEE_OFFSET);
                
                char text[1000];
                bzero(text, sizeof(text));
                
                FOR_EACH_CHILD(bbg, i, {
                    if (vm_read_uint64_t_value(task, child) == task_base_address + RBX_OBJECT_TextLabel_POINTEE_OFFSET)
                    {
                        long str_l = 0;
                        char* str = rbx_textlabel_get_text(task, child, &str_l);
                        if (str)
                        {
                            strcat(text, str);
                            vm_deallocate(mach_task_self_, (vm_address_t)str, str_l);
                        }
                    }
                })
                if (strstr(text, "Valuable") || strstr(text, "Bounty") || strstr(text, "Holy") )
                {
                    esp_color = valuable_esp_color;
                }
                else if (strstr(text, "Bond"))
                {
                    esp_color = bond_esp_color;
                }
                else if (strstr(text, "Gun") || strstr(text, "Melee"))
                {
                    esp_color = gun_esp_color;
                }
                else if (strstr(text, "Consumable"))
                {
                    esp_color = consumable_esp_color;
                }
                else
                {
                    esp_color = item_esp_color;
                    append = show_junk;
                }
                
                if (append)
                {
                    vm_address_t prim_part = rbx_model_get_primarypart(task, child);
                    if (prim_part && rbx_instance_is_valid_child(task, prim_part))
                    {
                        APPEND_ESP_OBJ(prim_part, esp_color, name);
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)name, name_l);
            }
        })
        esp_part_count = esp_part_index;
        sleep(1);
        }
    });
    
    NOCLIP_VARIABLES
    noclip_enabled = false;
    
    BASIC_NOCLIP_PART_GETTER
    BASIC_NOCLIP_ENGINE
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("showjunk", {show_junk = true; printf("SHOWING JUNK");})
        INSERT_STDIN_INPUT_SIMPLE("hidejunk", {show_junk = false; printf("HIDING JUNK");})
    })
}
