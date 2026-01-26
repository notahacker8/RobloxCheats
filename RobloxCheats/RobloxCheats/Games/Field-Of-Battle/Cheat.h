

void field_of_battle_cheat(task_t task)
{
    printf("- FIELD OF BATTLE -\n");
    LIBESP_DYLIB_SETUP
    GENERIC_CHEAT_STARTER
    
    ESP_GLOBAL_VARIABLES
    
    static ESP_Color chalice_esp_color = {.r = 1, .g = 0, .b = 0, .a = 1};
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            ESP_Color c = chalice_esp_color;
            if (c.r >= 1 && c.g <= 1)
            {
                c.b -= 0.1;
                c.g += 0.1;
            }
            if (c.g >= 1 && c.b <= 1)
            {
                c.r -= 0.1;
                c.b += 0.1;
            }
            if (c.b >= 1 && c.r <= 1)
            {
                c.g -= 0.1;
                c.r += 0.1;
            }
            chalice_esp_color = c;
            usleep(100000);
        }
    });
    
    
    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = 100;
    strcpy(shared_memory->ESP_TEXT_FONT_NAME, "Menlo");
    
    BASIC_ESP_ENGINE({}, {}, 100, {})

    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t projectiles_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Projectiles");
    
    
    //Gem name given its GemType number.
    static char* gem_dictionary[100];
    gem_dictionary[1] = "Red Diamond";
    gem_dictionary[2] = "Grandidierite";
    gem_dictionary[3] = "Poudretteite";
    gem_dictionary[4] = "Benitoite";
    gem_dictionary[5] = "Tanzanite";
    gem_dictionary[6] = "Alexandrite";
    gem_dictionary[7] = "Diamond";
    gem_dictionary[8] = "Sapphire";
    gem_dictionary[9] = "Emerald";
    gem_dictionary[10] = "Ruby";
    gem_dictionary[11] = "Lapis Lazuli";
    gem_dictionary[12] = "Topaz";
    gem_dictionary[13] = "Garnet";
    gem_dictionary[14] = "Aquamarine";
    gem_dictionary[15] = "Spinel";
    gem_dictionary[16] = "Amber";
    gem_dictionary[17] = "Titanite";
    gem_dictionary[18] = "Tourmaline";
    gem_dictionary[19] = "Kunzite";
    gem_dictionary[20] = "Amethyst";
    gem_dictionary[21] = "Citrine";
    gem_dictionary[22] = "Peridot";
    gem_dictionary[24] = "Onyx";
    gem_dictionary[25] = "Turquoise";
    gem_dictionary[27] = "Feldspar";
    gem_dictionary[28] = "Jade";
    gem_dictionary[29] = "Nephrite";
    gem_dictionary[30] = "Olivine";
    gem_dictionary[31] = "Copal";
    gem_dictionary[32] = "Mithril";
    gem_dictionary[33] = "Demonite";
    gem_dictionary[34] = "Fury Stone";
    gem_dictionary[35] = "Dragon Bone";
    gem_dictionary[36] = "Spirit Shard";
    gem_dictionary[42] = "Titan Core";
    gem_dictionary[51] = "Legendary Shard";
    
    static vm_address_t gems[1000];
    static long gem_types[1000];
    static int gem_count = 0;
    
    static vm_address_t gem_scan_instance = 0;
    
    static int gem_scan_sleep_time = 3;
    
    static char demon_mode = false;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            
            int gem_index = 0;
            
            gem_scan_start: {
                gem_scan_instance = demon_mode ? projectiles_folder : (gem_scan_instance == unbreakable_folder) ? projectiles_folder : unbreakable_folder;
                
                FOR_EACH_DESCENDANT(task, gem_scan_instance, 100000, 5, i, {
                //rbx_print_descendants(task, obj, 0, 0);
                //vm_address_t gemtype_iv = rbx_instance_find_first_child_of_class_and_name(task, obj, "IntValue", "GemType");
                vm_address_t gemtype_iv = rbx_instance_find_first_child_of_pointee_and_name(task, task_base_address, obj, RBX_OBJECT_IntValue_POINTEE_OFFSET, "GemType");
                if (gemtype_iv)
                {
                    vm_address_t gem = obj;
                    gem_types[gem_index] = rbx_intvalue_get_value(task, gemtype_iv);
                    gems[gem_index] = gem;
                    gem_index++;
                    
                }
            })
                
            };
            if (gem_scan_instance == projectiles_folder && !demon_mode)
            {
               goto gem_scan_start;
            }
            
            gem_count = gem_index;
            
            sleep(gem_scan_sleep_time);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            int esp_part_index = 0;
            
            COPY_BUF(gems)
            COPY_BUF(gem_types)
            COPY_VAR(gem_count)
            for (int i = 0 ; i < __gem_count ; i++)
            {
                vm_address_t gem = __gems[i];
                long gtype = __gem_types[i];
                if ((!demon_mode && gem_dictionary[gtype]) || (demon_mode && (gtype < 5 || gtype > 31) && gem_dictionary[gtype]))
                {
                    rbx_rgb_t c = rbx_basepart_get_color(task, gem);
                    ESP_Color ec = {.r = c.r, .g = c.g, .b = c.b, .a = 0.5};
                    if (gtype > 31 && gtype != 51)
                    {
                        ec = chalice_esp_color;
                    }
                    APPEND_ESP_OBJ(gem, ec, gem_dictionary[gtype]);
                }
            }
            
            esp_part_count = esp_part_index;
            sleep(1);
        }
    });
    
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("demonmode", {
            demon_mode = true;
            gem_scan_sleep_time = 1;
            printf("ENABLED DEMON MODE\n");
        });
        INSERT_STDIN_INPUT_SIMPLE("nodemonmode", {
            demon_mode = false;
            gem_scan_sleep_time = 3;
            printf("DISABLED DEMON MODE\n");
        });
    })
    
}
