

void assassin_cheat(task_t task)
{
    
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    
    printf("\n- ASSASSSIN -\n");
    LIBESP_DYLIB_SETUP
    
    GENERIC_CHEAT_STARTER
    
    rbx_print_descendants(task, workspace, 0, 1);
    
    static vm_address_t knifehost_folder = 0;
    static vm_address_t ghostcoins_folder = 0;
    
    vm_address_t playergui = rbx_instance_find_first_child(task, local_player, "PlayerGui");
    vm_address_t screengui = rbx_instance_find_first_child(task, playergui, "ScreenGui");
    vm_address_t ui_frame = rbx_instance_find_first_child(task, screengui, "UI");
    vm_address_t target_frame = rbx_instance_find_first_child(task, ui_frame, "Target");
    vm_address_t targettext_tl = rbx_instance_find_first_child(task, target_frame, "TargetText");
    
    ESP_GLOBAL_VARIABLES
    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = 1000;
    //rbx_print_descendants(task, ui_frame, 0, 2);
    
    static vm_address_t target_hrp = 0;
    
    static int character_count = 0;
    static vm_address_t characters[200];
    
    static char locking_on_enemy = false;
    static vm_address_t closest_hum_to_mouse = 0;
    static vm_address_t closest_hrp_to_mouse = 0;
    static vm_address_t closest_hrp_to_mouse_cframe_address = 0;
    static float closest_hrp_to_mouse_dist;
    static vm_address_t hrp_to_lock_on = 0;
    static vm_address_t hum_to_lock_on = 0;
    static vm_address_t hrp_to_lock_on_cframe_address = 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(1);
            knifehost_folder = rbx_instance_find_first_child(task, workspace, "KnifeHost");
            ghostcoins_folder = rbx_instance_find_first_child(task, workspace, "GhostCoins");
            int __character_count = 0;
            FOR_EACH_CHILD(workspace, {
                if (rbx_instance_is_a(task, child, "Model") && rbx_instance_find_first_child(task, child, "Humanoid"))
                {
                    characters[__character_count++] = child;
                }
            })
            character_count = __character_count;
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            long str_l = 0;
            char* str = rbx_textlabel_get_text(task, targettext_tl, &str_l);
            if (str)
            {
                //printf("%s\n", str);
                //vm_address_t plr = rbx_instance_find_first_child(task, players_service, str);
                //vm_address_t c=rbx_player_get_character(task, plr);
                //rbx_print_descendants(task, rbx_player_get_character(task, local_player), 0, 0);
                vm_address_t c = rbx_instance_find_last_child_of_class_and_name(task, workspace, "Model", str);
                //rbx_print_descendants(task, c, 0, 1);
                target_hrp = rbx_instance_find_first_child(task, c, "HumanoidRootPart");
                
                vm_deallocate(mach_task_self(), (vm_address_t)str, str_l);
            }
            usleep(500000);
        }
    });
    
   /* dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(1000);
            if (shared_memory->LEFT_MOUSE_DOWN)
            {
                those_who_remain_aimbot_patch(task, &st, task_base_address, true);
            }
            else
            {
                those_who_remain_aimbot_patch(task, &st, task_base_address, false);
            }
        }
    });*/
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            usleep(100 - (80 * shared_memory->LEFT_MOUSE_DOWN));
            rbx_cframe_t cf = rbx_basepart_get_cframe(task, closest_hrp_to_mouse);
            FOR_EACH_CHILD(knifehost_folder, {
                rbx_basepart_set_cframe(task, child, cf);
            })
            //rbx_print_descendants(task, target_hrp, 0, 0);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(1);
            FOR_EACH_CHILD(ghostcoins_folder, {
                rbx_basepart_set_cframe(task, child, rbx_basepart_get_cframe(task, my_hrp));
            })
        }
    });
    
    static ESP_Color default_esp_color = {.r = 0, .g = 1, .b = 0, .a = 1};
    static ESP_Color item_esp_color = {.r = 0, .g = 0, .b = 1, .a = 1};
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            sleep(2);
            //rbx_print_descendants(task, bp, 0, 1);
            int esp_part_index = 0;
            //printf("chars %d\n", character_count);
            COPY_BUF(characters)
            COPY_VAR(character_count)
            for (int i = 0 ; i < __character_count ; i++)
            {
                vm_address_t character = __characters[i];
                if (character != my_character)
                {
                    vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
                    
                    long player_name_l = 0;
                    char* player_name = rbx_instance_get_name(task, character, &player_name_l);
                    if (player_name)
                    {
                        ESP_Color esp_color;
                        esp_color.r = 1;
                        esp_color.g = 1;
                        esp_color.b = 1;
                        esp_color.a = 1;
                        
                        APPEND_ESP_OBJ(hrp, esp_color, player_name);
                        vm_deallocate(mach_task_self_, (vm_address_t)player_name, player_name_l);
                    }
                }
            }
            esp_part_count = esp_part_index;
            //printf("epc %d\n", esp_part_count);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        float window_inset = 0;
        for (;;)
        {
            float __old_dist_from_mouse = 500;
            vm_address_t __old_target_hrp = 0;
            
            COPY_VAR(character_count)
            COPY_BUF(characters)
            
            for (long i = 0 ; i < __character_count ; i++)
            {
                vm_address_t c = __characters[i];
                if (c != my_character)
                {
                    //rbx_print_descendants(task, c, 0, 0);
                    vm_address_t hrp = rbx_instance_find_first_child(task, __characters[i], "HumanoidRootPart");
                    if (hrp)
                    {
                        rbx_cframe_t hrp_cframe = rbx_basepart_get_cframe(task, hrp);
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);
                        float camera_fov = rbx_camera_get_field_of_view(task, camera);
                        float window_w = shared_memory->WINDOW_W;
                        float window_h = shared_memory->WINDOW_H;
                        custom_rbx_world_to_screen_info_t wtsi = rbx_world_to_screen_point(camera_cframe, camera_fov, hrp_cframe.pos, window_w, window_h, 1, 1);
                        if (wtsi.x > window_inset && wtsi.x < (window_w - window_inset))
                        {
                            if (wtsi.y > window_inset && wtsi.y < (window_h - window_inset))
                            {
                                
                                float mouse_x = shared_memory->MOUSE_X;
                                float mouse_y = shared_memory->MOUSE_Y;
                                
                                float dist_from_mouse = sqrtf(pow(wtsi.x - mouse_x, 2) + pow(wtsi.y - mouse_y, 2));
                                if (dist_from_mouse < __old_dist_from_mouse)
                                {
                                    __old_dist_from_mouse = dist_from_mouse;
                                    __old_target_hrp = hrp;
                                    //printf("%f\n", __old_dist_from_mouse);
                                }
                            }
                        }
                    }
                }
            }
            closest_hrp_to_mouse = __old_target_hrp;
            usleep(10000);
            //printf("%f\n", __old_dist_from_center);
        }
    });
    
    BASIC_ESP_ENGINE({
        if (part == target_hrp)
        {
            esp_color = item_esp_color;
        }
        if (part == closest_hrp_to_mouse)
        {
            esp_color = default_esp_color;
        }
    }, 100, {});
    
}
