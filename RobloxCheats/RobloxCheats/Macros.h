

#define LIBESP_DYLIB_SETUP \
vm_address_t dylib_load_address = task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);\
if (!dylib_load_address)\
{\
    printf("Couldn't find libESP.dylib in task %d\n", task);\
    exit(0);\
}\
vm_offset_t dylib_shared_file_path_offset = file_gdso(__LIBESP_DYLIB_PATH__, "SHARED_FILE_PATH");\
vm_offset_t dylib_should_create_shared_memory_offset = file_gdso(__LIBESP_DYLIB_PATH__, "SHOULD_CREATE_SHARED_MEMORY");\
vm_offset_t dylib_shared_memory_created_offset = file_gdso(__LIBESP_DYLIB_PATH__, "SHARED_MEMORY_CREATED");\
{\
char a = true;\
vm_write(task, dylib_load_address + dylib_shared_file_path_offset, (vm_offset_t)__ROBLOX_PIPE_TXT_PATH__, (int)strlen(__ROBLOX_PIPE_TXT_PATH__));\
vm_write(task, dylib_load_address + dylib_should_create_shared_memory_offset, (vm_offset_t)&a, 1);\
}\
while (vm_read_uint8_value(task, dylib_load_address + dylib_shared_memory_created_offset) == false)\
{\
    usleep(100000); /* Wait for the shared memory to be created */ \
}\
libESPSharedMemoryObject* shared_memory = init_shared_memory(__ROBLOX_PIPE_TXT_PATH__, false);


#define ANTI_AFK \
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^\
{\
    for (;;)\
    {\
        double a = 9999999;\
        sleep(1);\
        rbx_player_set_last_input_timestamp(task, local_player, a);\
        sleep(1);\
    }\
});



#define BASIC_FLY_ENGINE(velocity_handler, usleep_time) \
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^\
{\
    for (;;)\
    {\
        usleep(usleep_time);\
        COPY_VAR(my_hrp)\
        if (fly_enabled)\
        {\
            vector3_t vel;\
            rbx_cframe_t cf = rbx_camera_get_cframe(task, camera);\
            vector3_t lv = rbx_cframe_get_look_vector(cf);\
            vector3_t rv = rbx_cframe_get_right_vector(cf);\
            bzero(&vel, sizeof(vector3_t));\
            velocity_handler\
            rbx_basepart_set_velocity(task, __my_hrp, vel);\
            rbx_basepart_set_gravity(task, __my_hrp, 0.0f);\
        }\
        else\
        {\
            rbx_basepart_set_gravity(task, __my_hrp, new_gravity);\
        }\
    }\
});

#define BASIC_FLY_CONTROL_CODE \
unsigned char* keys_down = shared_memory->KEYS_DOWN;\
if (keys_down['w']) { vel = vector3_add(vector3_mul_num(lv, 1), vel); }\
if (keys_down['a']) { vel = vector3_add(vector3_mul_num(rv, -1), vel); }\
if (keys_down['s']) { vel = vector3_add(vector3_mul_num(lv, -1), vel); }\
if (keys_down['d']) { vel = vector3_add(vector3_mul_num(rv, 1), vel); }\
float magnitude = vector3_magnitude(vel);\
if (!isnan(magnitude) && magnitude > 0.0f)\
{\
    vel = vector3_div_num(vel,  magnitude);\
}\
vel = vector3_mul_num(vel, fly_speed);


#define ESP_GLOBAL_VARIABLES \
static int esp_part_count = 0;\
static ESP_Color esp_part_colors[MAX_ESP_COUNT];\
static char esp_part_names[MAX_ESP_COUNT * MAX_ESP_TEXT_LENGTH];\
static vm_address_t esp_parts[MAX_ESP_COUNT];\

#define BASIC_ESP_ENGINE(pre_draw_code, i_usleep_time, pre_hiddenset_code) \
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^\
{\
    for (;;)\
    {\
        int esp_index = 0;\
\
        COPY_BUF(esp_parts)\
        COPY_BUF(esp_part_colors)\
        COPY_BUF(esp_part_names)\
\
        COPY_VAR(esp_part_count)\
        \
        if (__esp_part_count > 0 && __esp_part_count < MAX_ESP_COUNT && esp_enabled)\
        {\
            for (int i = 0 ; i < __esp_part_count ; i++)\
            {\
                vm_address_t part = __esp_parts[i];\
                if (part)\
                {\
                    if (rbx_instance_is_valid_child(task, part))\
                    {\
                        rbx_cframe_t my_hrp_cframe = rbx_basepart_get_cframe(task, my_hrp);\
                        rbx_cframe_t camera_cframe = rbx_camera_get_cframe(task, camera);\
                        rbx_cframe_t part_cframe = rbx_basepart_get_cframe(task, part);\
                        float dist = vector3_dist_dif(part_cframe.pos, my_hrp_cframe.pos);\
                        float fov = rbx_camera_get_field_of_view(task, camera);\
                        ESP_Color esp_color = __esp_part_colors[i];\
                        float window_w = shared_memory->WINDOW_W;\
                        float window_h = shared_memory->WINDOW_H;\
                        \
                        pre_draw_code\
                        \
                        ESP_Frame scaled_frame = rbx_draw_esp_box_on_3d_point(task, part_cframe.pos, camera_cframe, shared_memory, 1, esp_color, fov, 20, 20, 0, 0, window_w, window_h, esp_index, false);\
                        esp_index++;\
                        rbx_draw_esp_box_on_3d_point(task, part_cframe.pos,\
                                         camera_cframe, shared_memory, 0,\
                                         esp_color, fov, 500, 40, 0, scaled_frame.h + 10, window_w, window_h, esp_index, false);\
                        char* part_name = __esp_part_names + (MAX_ESP_TEXT_LENGTH * i);\
                        char text[MAX_ESP_TEXT_LENGTH];\
                        bzero(text, MAX_ESP_TEXT_LENGTH);\
                        sprintf(text, "%s [%.0f]", part_name, dist);\
                        strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);\
                        esp_index++;\
                    }\
                }\
                usleep(i_usleep_time);\
            }\
        }\
        else\
        {\
            usleep(esput);\
        }\
        \
        pre_hiddenset_code\
        \
        memset(shared_memory->ESP_BOX_HIDDEN_ARRAY + esp_index, 1, MAX_ESP_COUNT - esp_index);\
    }\
});

#define APPEND_ESP_OBJ(part, esp_color, string)\
if (part) {\
esp_part_colors[esp_part_index] = esp_color;\
esp_parts[esp_part_index] = part;\
bzero(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), MAX_ESP_TEXT_LENGTH);\
memcpy(esp_part_names + (esp_part_index * MAX_ESP_TEXT_LENGTH), string, strlen(string));\
esp_part_index++;\
}


#define BASIC_NOCLIP_PART_GETTER \
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^\
               {\
    for (;;)\
    {\
        my_character = rbx_player_get_character(task, local_player);\
        my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");\
        {\
            long child_count = 0;\
            rbx_child_t* children = rbx_instance_get_children(task, my_character, &child_count);\
            if (children)\
            {\
                int __i = 0;\
                for (long i = 0 ; i < child_count ; i++)\
                {\
                    vm_address_t object = children[i].child_address;\
                    if (rbx_instance_is_a(task, object, "Accessory"))\
                    {\
                        vm_address_t h = rbx_instance_find_first_child_of_class(task, object, "Part");\
                        if (__i < 200)\
                        {\
                            my_character_parts[__i++] = h;\
                        }\
                    }\
                    if (rbx_instance_is_a(task, object, "Part") ||\
                        rbx_instance_is_a(task, object, "MeshPart"))\
                    {\
                        if (__i < 200)\
                        {\
                            my_character_parts[__i++] = object;\
                        }\
                    }\
                }\
                my_character_part_count = __i;\
                vm_deallocate(mach_task_self_, (vm_address_t)children, child_count * sizeof(rbx_child_t));\
            }\
        }\
        sleep(1);\
    }\
});


#define BASIC_NOCLIP_ENGINE \
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^\
               {\
    for (;;)\
    {\
        if (noclip_enabled)\
        {\
            COPY_VAR(my_character_part_count)\
            COPY_BUF(my_character_parts)\
            for (long i = 0 ; i < __my_character_part_count ; i++)\
            {\
                if (rbx_instance_is_valid_child(task, __my_character_parts[i]))\
                {\
                    rbx_basepart_set_cancollide(task, __my_character_parts[i], false);\
                }\
            }\
            usleep(10000);\
        }\
        else\
        {\
            sleep(1);\
        }\
    }\
});
