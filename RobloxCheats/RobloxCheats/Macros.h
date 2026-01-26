


#define VM_READ_FUNCTION_CREATE(data_type)\
static inline data_type vm_read_##data_type##_value(task_t task, vm_address_t address)\
{\
    data_type data;\
    vm_read_byte_array(task, address, sizeof(data), &data);\
    return data;\
}

#define RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(class_name, property_name, data_type, offset) \
static inline data_type rbx_##class_name##_get_##property_name(task_t task, vm_address_t class_name)\
{\
return vm_read_##data_type##_value(task, class_name + offset);\
}\


#define RBX_CREATE_BASIC_STRING_PROPERTY_GET_FUNCTION(class_name, property_name, offset) \
static inline char* rbx_##class_name##_get_##property_name(task_t task, vm_address_t class_name, long* inout_len)\
{\
return vm_read_rbx_string(task, class_name + offset, inout_len);\
}\


#define RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(class_name, property_name, data_type, offset) \
static inline void rbx_##class_name##_set_##property_name(task_t task, vm_address_t class_name, data_type value)\
{\
vm_write(task, class_name + offset, (vm_offset_t)&value, sizeof(value));\
}\


#define LIBESP_DYLIB_SETUP \
vm_address_t task_base_address = task_get_image_address_by_index(task, 0);\
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
while (vm_read_uint8_t_value(task, dylib_load_address + dylib_shared_memory_created_offset) == false)\
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
        rbx_player_set_last_input_timestamp(task, local_player, a);\
        sleep(5);\
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
            /*printf("%f, %f, %f\n", lv.x, lv.y, lv.z);*/\
            vector3_t rv = rbx_cframe_get_right_vector(cf);\
            bzero(&vel, sizeof(vector3_t));\
            velocity_handler\
            rbx_basepart_set_velocity(task, __my_hrp, vel);\
        }\
        else\
        {\
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
static char esp_enabled = true;\
static int esp_part_count = 0;\
static ESP_Color esp_part_colors[MAX_ESP_COUNT];\
static char esp_part_names[MAX_ESP_COUNT * MAX_ESP_TEXT_LENGTH];\
static vm_address_t esp_parts[MAX_ESP_COUNT];\
static int esp_part_flags[MAX_ESP_COUNT];\

#define BASIC_ESP_ENGINE(pre_part_loop_code, pre_draw_code, i_usleep_time, pre_hiddenset_code) \
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^\
{\
    for (;;)\
    {\
        int esp_index = 0;\
pre_part_loop_code\
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
                        float object_width = 20;\
                        float object_height = 20;\
                        char should_scale = false;\
                        float border_width = 1;\
                        float y_offset = 10;\
                        \
                        pre_draw_code\
                        \
                        char text[MAX_ESP_TEXT_LENGTH];\
                        bzero(text, MAX_ESP_TEXT_LENGTH);\
                        NSRect scaled_frame = rbx_draw_esp_box_on_3d_point(task, part_cframe.pos, camera_cframe, shared_memory, border_width, esp_color, fov, object_width, object_height, 0, 0, window_w, window_h, esp_index, should_scale);\
                        strcpy(shared_memory->ESP_BOX_TEXT_ARRAY + (esp_index * MAX_ESP_TEXT_LENGTH), text);\
                        esp_index++;\
                        rbx_draw_esp_box_on_3d_point(task, part_cframe.pos,\
                                         camera_cframe, shared_memory, 0,\
                                         esp_color, fov, 500, 40, 0, scaled_frame.size.height + y_offset, window_w, window_h, esp_index, false);\
                        char* part_name = __esp_part_names + (MAX_ESP_TEXT_LENGTH * i);\
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
            usleep(100000);\
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


#define NOCLIP_VARIABLES \
static char noclip_enabled = true;\
static vm_address_t my_character_parts[200];\
static long my_character_part_count = 0;

#define BASIC_NOCLIP_PART_GETTER \
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^\
{\
for (;;)\
{\
int __i = 0;\
FOR_EACH_DESCENDANT(task, my_character, 10000, 10, i, {\
if (rbx_instance_is_a_basepart(task, obj, task_base_address))\
{\
my_character_parts[__i++] = obj;\
}\
})\
my_character_part_count = __i;\
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
            usleep(300000);\
        }\
    }\
});


#define EXIT_IF_TASK_TERMINATED \
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^\
               {\
    for (;;)\
    {\
{pid_t p = -1; pid_for_task(task, &p);if (p == -1) {printf("task no longer exists, exiting\n");exit(0);}sleep(1);}\
    }\
});



#define GENERIC_CHEAT_STARTER \
vm_address_t game = rbx_find_game_address(task);\
vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");\
vm_address_t coregui = rbx_instance_find_first_child_of_class(task, game, "CoreGui");\
vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");\
vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");\
vm_address_t teams_service = rbx_instance_find_first_child_of_class(task, game, "Teams");\
vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");\
vm_address_t backpack = rbx_instance_find_first_child_of_class(task, local_player, "Backpack");\
vm_address_t replicated_storage = rbx_instance_find_first_child_of_class(task, game, "ReplicatedStorage");\
vm_address_t replicated_first = rbx_instance_find_first_child_of_class(task, game, "ReplicatedFirst");\
vm_address_t lighting_service = rbx_instance_find_first_child_of_class(task, game, "Lighting");\
static vm_address_t my_character = 0;\
static vm_address_t my_hrp = 0;\
static vm_address_t my_hum = 0;\
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^\
{\
    for (;;)\
    {\
        my_character = rbx_player_get_character(task, local_player);\
        my_hrp = rbx_instance_find_first_child(task, my_character, "HumanoidRootPart");\
        my_hum = rbx_instance_find_first_child_of_class(task, my_character, "Humanoid");\
        sleep(1);\
    }\
});



#define STDIN_INPUT_ENGINE(code) \
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^\
               {\
char b = 0;\
char pr[10000];\
bzero(pr, sizeof(pr));\
char str[10000];\
bzero(str, sizeof(str));\
strcat(pr, "List of commands:\n");\
    for (;;)\
    {\
        code\
        if (!b)\
        {\
            b = 1;\
            printf("%s\n", pr);\
        }\
        fgets(str, 100, stdin);\
    }\
});\

#define INSERT_STDIN_INPUT_SIMPLE(cmd, code) \
strcat(pr, "\t"cmd"\n");\
if (strcmp(str, cmd"\xa") == 0)\
{\
    code\
}\


#define INSERT_STDIN_MULTIARG_INPUT(cmd_n_arg_format, code, argcount, ...) \
strcat(pr, "\t"cmd_n_arg_format"\n"); \
if (sscanf(str, cmd_n_arg_format, __VA_ARGS__) == argcount)\
{\
    code\
}\




#define MEMORY_SEARCH_HEADER(USER_TAG) \
{\
static mach_msg_type_number_t data_cnt;\
kern_return_t kr = KERN_SUCCESS;\
struct vm_region_submap_info_64 info;\
vm_address_t address = 0;\
vm_size_t size = 0;\
natural_t depth = 0;\
mach_msg_type_number_t info_count = VM_REGION_SUBMAP_INFO_COUNT_64;\
while (kr == KERN_SUCCESS)\
{\
    memset (&info, 0, sizeof (info));\
    kr = vm_region_recurse_64 (task, &address, &size,\
                               &depth, (vm_region_recurse_info_t) &info, &info_count);\
    if (info.user_tag == USER_TAG)\
    {\
        vm_address_t read_data;\
        int err = vm_read(task, address, size, (vm_address_t*)&read_data, &data_cnt);\
        if (!err)\
        {
        
        
        
#define MEMORY_SEARCH_FOOTER  vm_deallocate(mach_task_self_, (vm_address_t)read_data, size);}}\
address += size;}}

#define MEMORY_SEARCH_LOCAL_VARIABLE_FILTER_HEADER(var_type_filter)\
vm_address_t var_name_ptr = *((vm_address_t*)(read_data + i));\
if (var_name_ptr > task_base_address)\
{\
    {\
        vm_address_t var_ptr = address + i - 0x10;\
        uint32_t var_type = *((uint32_t*)(read_data + i - 0x10 + 12));\
        if (var_type_filter)\
        {\
            uint16_t a_sig = vm_read_uint16_t_value(task, var_name_ptr + 6);\
            if (a_sig == 0xffff)\
            {\
                
                
#define MEMORY_SEARCH_LOCAL_VARIABLE_FILTER_FOOTER }}}}



#define POST_NOTIF(title, body)\
strcpy(shared_memory->NOTIFICATION_TITLE_TEXT, title);\
strcpy(shared_memory->NOTIFICATION_BODY_TEXT, body);\
shared_memory->SHOULD_DISPLAY_NOTIFICATION = true;\



#define WAIT_WHILE_COND(x) \
while (x)\
{\
usleep(50000);\
}


#define DO_WHILE_COND_WITH_TIMEOUT(dothis, cond, timeout_sec) ({                              \
    double _timeout = (timeout_sec);                                  \
    struct timespec _start, _now;                                     \
    clock_gettime(CLOCK_MONOTONIC, &_start);                          \
    bool _timed_out = false;                                          \
    while (cond) {                                                    \
        clock_gettime(CLOCK_MONOTONIC, &_now);                        \
        double _elapsed = (_now.tv_sec - _start.tv_sec) +             \
                          (_now.tv_nsec - _start.tv_nsec) / 1e9;      \
        if (_elapsed >= _timeout) {                                   \
            _timed_out = true;                                        \
            break;                                                    \
        }                                                             \
dothis;                                                               \
        usleep(50000);                                                \
    }                                                                 \
    _timed_out; /* returns true if timeout occurred */                 \
})

#define WAIT_WHILE_COND_WITH_TIMEOUT(cond, timeout_sec)\
DO_WHILE_COND_WITH_TIMEOUT({}, cond, timeout_sec)
