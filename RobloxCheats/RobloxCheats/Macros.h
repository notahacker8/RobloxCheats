

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
while (vm_read_1byte_value(task, dylib_load_address + dylib_shared_memory_created_offset) == false)\
{\
    usleep(100000); /* Wait for the shared memory to be created */ \
}\
libESPSharedMemoryObject* shared_memory = init_shared_memory(__ROBLOX_PIPE_TXT_PATH__, false);

