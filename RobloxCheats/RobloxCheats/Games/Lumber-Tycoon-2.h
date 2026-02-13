void lt2_cheat(task_t task)
{
    LIBESP_DYLIB_SETUP
    GENERIC_CHEAT_STARTER
    ESP_GLOBAL_VARIABLES
    
    shared_memory->ESP_COUNT = MAX_ESP_COUNT;
    shared_memory->ESP_ENABLED = true;
    shared_memory->ESP_USLEEP_TIME = 1000;
    
    vector3_t pos = {
        .x = 329.388855,
        .y = -0.589523,
        .z = 79.106377
    };

    rbx_cframe_t cf = DEFAULT_CFRAME;
    cf.pos = pos;
    
    ESP_Color color = {.r = 1, .g = 0, .b = 0, .a = 1};
    
    vm_address_t logmodels_folder = rbx_instance_find_first_child(task, workspace, "LogModels");
    
    static vm_address_t logs[10000];
    static vm_address_t log_cframe_addresses[10000];
    static int log_count = 0;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            COPY_BUF(logs)
            COPY_BUF(log_cframe_addresses)
            int log_index = 0;
            FOR_EACH_DESCENDANT(task, logmodels_folder, 100000, 100, i, {
                if (rbx_instance_is_a_basepart(task, obj, task_base_address))
                {
                    __logs[log_index] = obj;
                    __log_cframe_addresses[log_index] = rbx_basepart_get_properties_address(task, obj) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
                    log_index++;
                }
            })
            memcpy(log_cframe_addresses, __log_cframe_addresses, sizeof(log_cframe_addresses));
            memcpy(logs, __logs, sizeof(logs));
            log_count = log_index;
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            int esp_part_index = 0;
            COPY_BUF(logs)
            COPY_VAR(log_count)
            for (int i = 0 ; i < __log_count ; i++)
            {
                vm_address_t log = __logs[i];
                APPEND_ESP_OBJ(log, color, "Log");
            }
            esp_part_count = esp_part_index;
            sleep(1);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            COPY_BUF(logs)
            COPY_BUF(log_cframe_addresses)
            COPY_VAR(log_count)
            for (int i = 0 ; i < __log_count ; i++)
            {
                vm_write(task, __log_cframe_addresses[i], (vm_address_t)&cf, sizeof(cf));
            }
            usleep(100);
        }
    });
    
    static char fly_enabled = true;
    PRESS_ONCE_KEYBIND_CREATE('m', {
        fly_enabled = !fly_enabled;
        rbx_smooth_fly_patch(task, task_base_address, fly_enabled);
    }, true, "fly")
    
    BASIC_ESP_ENGINE({}, {}, 100, {})
    
    static float fly_speed = 300;
    BASIC_FLY_ENGINE(BASIC_FLY_CONTROL_CODE, 300)
    
}

