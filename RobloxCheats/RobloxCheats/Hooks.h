
/*
 When you are running Intel Roblox on Apple Silicon, you can "replace" functions.
 */

#ifdef APPLE_ROSETTA_VERSION


typedef struct
{
    vm_address_t alloc_func_x86;
    vm_address_t alloc_func_arm;
    vm_address_t alloc_func_arm_r_ptr;
    vm_address_t alloc_func_arm_ribm_ptr;
    vm_address_t target_func_x86;
    vm_address_t target_func_arm;
    vm_address_t target_func_arm_r_ptr;
    vm_address_t target_func_arm_ribm_ptr;
    vm_address_t custom_mem;
    char initiated;
}
func_wrap_info_t;

typedef struct
{
    vm_address_t orig_func;
    float* ws_array;
    long* humanoid_whitelist;
    int whitelist_size;
    char func_enabled;
    char hook_enabled;
    char use_whitelist;
}
custom_rbx_walkspeed_wrap_custom_mem_t;

typedef struct
{
    vm_address_t orig_func;
    char* value_array;
    long* object_whitelist;
    int whitelist_size;
    char func_enabled;
    char hook_enabled;
    char use_whitelist;
}
custom_rbx_anchored_wrap_custom_mem_t;


func_wrap_info_t create_walkspeed_setter_wrap(task_t task,
                                              vm_address_t func_x86,
                                              libESPSharedMemoryObject* shared_memory)
{
    static unsigned char bytes[] = { 0x55, 0x48, 0x89, 0xE5, 0x48, 0x83, 0xEC, 0x20, 0x48, 0x89, 0x7D, 0xF8, 0xF3, 0x0F, 0x11, 0x45, 0xF4, 0x48, 0xB8, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0x48, 0x89, 0x45, 0xE8, 0x48, 0x8B, 0x45, 0xE8, 0x0F, 0xBE, 0x40, 0x1C, 0x83, 0xF8, 0x01, 0x0F, 0x85, 0x8D, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x45, 0xE8, 0x0F, 0xBE, 0x40, 0x1D, 0x83, 0xF8, 0x01, 0x75, 0x6E, 0x48, 0x8B, 0x45, 0xE8, 0x0F, 0xBE, 0x40, 0x1E, 0x83, 0xF8, 0x01, 0x75, 0x4E, 0xC7, 0x45, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x45, 0xE4, 0x48, 0x8B, 0x4D, 0xE8, 0x3B, 0x41, 0x18, 0x7D, 0x39, 0x48, 0x8B, 0x45, 0xE8, 0x48, 0x8B, 0x40, 0x10, 0x48, 0x63, 0x4D, 0xE4, 0x48, 0x8B, 0x04, 0xC8, 0x48, 0x3B, 0x45, 0xF8, 0x75, 0x16, 0x48, 0x8B, 0x45, 0xE8, 0x48, 0x8B, 0x40, 0x08, 0x48, 0x63, 0x4D, 0xE4, 0xF3, 0x0F, 0x10, 0x04, 0x88, 0xF3, 0x0F, 0x11, 0x45, 0xF4, 0xEB, 0x00, 0x8B, 0x45, 0xE4, 0x83, 0xC0, 0x01, 0x89, 0x45, 0xE4, 0xEB, 0xBB, 0xEB, 0x11, 0x48, 0x8B, 0x45, 0xE8, 0x48, 0x8B, 0x40, 0x08, 0xF3, 0x0F, 0x10, 0x00, 0xF3, 0x0F, 0x11, 0x45, 0xF4, 0xEB, 0x00, 0x48, 0x8B, 0x45, 0xE8, 0x48, 0x8B, 0x00, 0x48, 0x8B, 0x7D, 0xF8, 0xF3, 0x0F, 0x10, 0x45, 0xF4, 0xFF, 0xD0, 0x48, 0x83, 0xC4, 0x20, 0x5D, 0xC3 };
    
    /** Compile this at https://godbolt.org and convert to assembly bytes at https://defuse.ca
     
     
     typedef void functype(long, float);
     typedef struct
     {
         functype* orig_func;
         float* ws_array;
         long* humanoid_whitelist;
         int whitelist_size;
         char func_enabled;
         char hook_enabled;
         char use_whitelist;
     }
     mem_t;
     void new_set_walkspeed(long humanoid, float walkspeed)
     {
         mem_t* mem = (mem_t*)0xcafecafecafecafe;
         if (mem->func_enabled == true)
         {
             if (mem->hook_enabled == true)
             {
                 if (mem->use_whitelist == true)
                 {
                     for (int i = 0 ; i < mem->whitelist_size ; i++)
                     {
                         if (mem->humanoid_whitelist[i] == humanoid)
                         {
                             walkspeed = mem->ws_array[i];
                         }
                     }
                 }
                 else
                 {
                     walkspeed = mem->ws_array[0];
                 }
             }
             mem->orig_func(humanoid, walkspeed);
         }
     }


     */
    
    func_wrap_info_t fwi;
    fwi.initiated = true;
    fwi.target_func_x86 = func_x86;
    fwi.target_func_arm = find_rosetta_arm_func_from_x86(task, func_x86, &(fwi.target_func_arm_ribm_ptr), &(fwi.target_func_arm_r_ptr));
    fwi.custom_mem =  vm_allocate_rwx(task, NULL, sizeof(custom_rbx_walkspeed_wrap_custom_mem_t));
    memcpy(bytes + 0x11 + 2, &fwi.custom_mem, 8);
    fwi.alloc_func_x86 = vm_allocate_rwx(task, bytes, sizeof(bytes));
    while (shared_memory->FUNCTION_QUEUE_FINISHED == false) {usleep(1000); }
    shared_memory->FUNCTION_QUEUE_COUNT = 1;
    shared_memory->FUNCTION_QUEUE[0].address = fwi.alloc_func_x86;
    shared_memory->FUNCTION_QUEUE[0].type = 0;
    shared_memory->FUNCTION_QUEUE[0].async_type = FUNCTION_ASYNC_TYPE_CURRENT;
    shared_memory->FUNCTION_QUEUE[0].should_execute = true;
    shared_memory->FUNCTION_QUEUE_FINISHED = false;
    while (shared_memory->FUNCTION_QUEUE_FINISHED == false) {usleep(1000); }
    fwi.alloc_func_arm = find_rosetta_arm_func_from_x86(task, fwi.alloc_func_x86, &(fwi.alloc_func_arm_ribm_ptr), &(fwi.alloc_func_arm_r_ptr));
    
    {
        vm_write(task, fwi.alloc_func_arm_r_ptr, (vm_offset_t)&fwi.target_func_arm, 8);
        vm_write(task, fwi.alloc_func_arm_ribm_ptr, (vm_offset_t)&fwi.target_func_arm, 8);
        vm_write(task, fwi.target_func_arm_r_ptr, (vm_offset_t)&fwi.alloc_func_arm, 8);
        vm_write(task, fwi.target_func_arm_ribm_ptr, (vm_offset_t)&fwi.alloc_func_arm, 8);
    }
    
    return fwi;
}


func_wrap_info_t create_walkspeed_getter_wrap(task_t task,
                                              vm_address_t func_x86,
                                              libESPSharedMemoryObject* shared_memory)
{
    static unsigned char bytes[] = { 0x55, 0x48, 0x89, 0xE5, 0x48, 0x83, 0xEC, 0x20, 0x48, 0x89, 0x7D, 0xF8, 0x48, 0xB8, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0x48, 0x89, 0x45, 0xF0, 0x48, 0x8B, 0x45, 0xF0, 0x0F, 0xBE, 0x40, 0x1C, 0x83, 0xF8, 0x01, 0x0F, 0x85, 0x8F, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x45, 0xF0, 0x0F, 0xBE, 0x40, 0x1D, 0x83, 0xF8, 0x01, 0x75, 0x6E, 0x48, 0x8B, 0x45, 0xF0, 0x0F, 0xBE, 0x40, 0x1E, 0x83, 0xF8, 0x01, 0x75, 0x4E, 0xC7, 0x45, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x45, 0xE8, 0x48, 0x8B, 0x4D, 0xF0, 0x3B, 0x41, 0x18, 0x7D, 0x39, 0x48, 0x8B, 0x45, 0xF0, 0x48, 0x8B, 0x40, 0x10, 0x48, 0x63, 0x4D, 0xE8, 0x48, 0x8B, 0x04, 0xC8, 0x48, 0x3B, 0x45, 0xF8, 0x75, 0x16, 0x48, 0x8B, 0x45, 0xF0, 0x48, 0x8B, 0x40, 0x08, 0x48, 0x63, 0x4D, 0xE8, 0xF3, 0x0F, 0x10, 0x04, 0x88, 0xF3, 0x0F, 0x11, 0x45, 0xEC, 0xEB, 0x00, 0x8B, 0x45, 0xE8, 0x83, 0xC0, 0x01, 0x89, 0x45, 0xE8, 0xEB, 0xBB, 0xEB, 0x11, 0x48, 0x8B, 0x45, 0xF0, 0x48, 0x8B, 0x40, 0x08, 0xF3, 0x0F, 0x10, 0x00, 0xF3, 0x0F, 0x11, 0x45, 0xEC, 0xEB, 0x12, 0x48, 0x8B, 0x45, 0xF0, 0x48, 0x8B, 0x00, 0x48, 0x8B, 0x7D, 0xF8, 0xFF, 0xD0, 0xF3, 0x0F, 0x11, 0x45, 0xEC, 0xEB, 0x00, 0xF3, 0x0F, 0x10, 0x45, 0xEC, 0x48, 0x83, 0xC4, 0x20, 0x5D, 0xC3 };
    
    /** Compile this at https://godbolt.org and convert to assembly bytes at https://defuse.ca
     
     typedef float functype(long);
     typedef struct
     {
         functype* orig_func;
         float* ws_array;
         long* humanoid_whitelist;
         int whitelist_size;
         char func_enabled;
         char hook_enabled;
         char use_whitelist;
     }
     mem_t;

     float new_get_walkspeed(long humanoid)
     {
         mem_t* mem = (mem_t*)0xcafecafecafecafe;
         float ret_ws;
         if (mem->func_enabled == true)
         {
             if (mem->hook_enabled == true)
             {
                 if (mem->use_whitelist == true)
                 {
                     for (int i = 0 ; i < mem->whitelist_size ; i++)
                     {
                         if (mem->humanoid_whitelist[i] == humanoid)
                         {
                             ret_ws = mem->ws_array[i];
                         }
                     }
                 }
                 else
                 {
                     ret_ws = mem->ws_array[0];
                 }
             }
             else
             {
                 ret_ws = mem->orig_func(humanoid);
             }
         }
         return ret_ws;
     }

     */
    
    func_wrap_info_t fwi;
    fwi.initiated = true;
    fwi.target_func_x86 = func_x86;
    fwi.target_func_arm = find_rosetta_arm_func_from_x86(task, func_x86, &(fwi.target_func_arm_ribm_ptr), &(fwi.target_func_arm_r_ptr));
    fwi.custom_mem =  vm_allocate_rwx(task, NULL, sizeof(custom_rbx_walkspeed_wrap_custom_mem_t));
    memcpy(bytes + 0xc + 2, &fwi.custom_mem, 8);
    fwi.alloc_func_x86 = vm_allocate_rwx(task, bytes, sizeof(bytes));
    while (shared_memory->FUNCTION_QUEUE_FINISHED == false) {usleep(1000); }
    shared_memory->FUNCTION_QUEUE_COUNT = 1;
    shared_memory->FUNCTION_QUEUE[0].address = fwi.alloc_func_x86;
    shared_memory->FUNCTION_QUEUE[0].type = 0;
    shared_memory->FUNCTION_QUEUE[0].async_type = FUNCTION_ASYNC_TYPE_CURRENT;
    shared_memory->FUNCTION_QUEUE[0].should_execute = true;
    shared_memory->FUNCTION_QUEUE_FINISHED = false;
    while (shared_memory->FUNCTION_QUEUE_FINISHED == false) {usleep(1000); }
    fwi.alloc_func_arm = find_rosetta_arm_func_from_x86(task, fwi.alloc_func_x86, &(fwi.alloc_func_arm_ribm_ptr), &(fwi.alloc_func_arm_r_ptr));
    
    {
        vm_write(task, fwi.alloc_func_arm_r_ptr, (vm_offset_t)&fwi.target_func_arm, 8);
        vm_write(task, fwi.alloc_func_arm_ribm_ptr, (vm_offset_t)&fwi.target_func_arm, 8);
        vm_write(task, fwi.target_func_arm_r_ptr, (vm_offset_t)&fwi.alloc_func_arm, 8);
        vm_write(task, fwi.target_func_arm_ribm_ptr, (vm_offset_t)&fwi.alloc_func_arm, 8);
    }
    
    
    return fwi;
}










func_wrap_info_t create_anchored_setter_wrap(task_t task,
                                             vm_address_t func_x86,
                                             libESPSharedMemoryObject* shared_memory)
{
    static unsigned char bytes[] = { 0x55, 0x48, 0x89, 0xE5, 0x48, 0x83, 0xEC, 0x20, 0x48, 0x89, 0x7D, 0xF8, 0x48, 0x89, 0x75, 0xF0, 0x48, 0xB8, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0x48, 0x89, 0x45, 0xE8, 0x48, 0x8B, 0x45, 0xE8, 0x0F, 0xBE, 0x40, 0x1C, 0x83, 0xF8, 0x01, 0x0F, 0x85, 0x88, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x45, 0xE8, 0x0F, 0xBE, 0x40, 0x1D, 0x83, 0xF8, 0x01, 0x75, 0x6A, 0x48, 0x8B, 0x45, 0xE8, 0x0F, 0xBE, 0x40, 0x1E, 0x83, 0xF8, 0x01, 0x75, 0x4C, 0xC7, 0x45, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x45, 0xE4, 0x48, 0x8B, 0x4D, 0xE8, 0x3B, 0x41, 0x18, 0x7D, 0x37, 0x48, 0x8B, 0x45, 0xE8, 0x48, 0x8B, 0x40, 0x10, 0x48, 0x63, 0x4D, 0xE4, 0x48, 0x8B, 0x04, 0xC8, 0x48, 0x3B, 0x45, 0xF8, 0x75, 0x14, 0x48, 0x8B, 0x45, 0xE8, 0x48, 0x8B, 0x40, 0x08, 0x48, 0x63, 0x4D, 0xE4, 0x48, 0x8B, 0x04, 0xC8, 0x48, 0x89, 0x45, 0xF0, 0xEB, 0x00, 0x8B, 0x45, 0xE4, 0x83, 0xC0, 0x01, 0x89, 0x45, 0xE4, 0xEB, 0xBD, 0xEB, 0x0F, 0x48, 0x8B, 0x45, 0xE8, 0x48, 0x8B, 0x40, 0x08, 0x48, 0x8B, 0x00, 0x48, 0x89, 0x45, 0xF0, 0xEB, 0x00, 0x48, 0x8B, 0x45, 0xE8, 0x48, 0x8B, 0x00, 0x48, 0x8B, 0x7D, 0xF8, 0x48, 0x8B, 0x75, 0xF0, 0xFF, 0xD0, 0x48, 0x83, 0xC4, 0x20, 0x5D, 0xC3 };
    
    /** Compile this at https://godbolt.org and convert to assembly bytes at https://defuse.ca
     
     
     typedef void functype(long, char);
          typedef struct
          {
              functype* orig_func;
              char* value_array;
              long* object_whitelist;
              int whitelist_size;
              char func_enabled;
              char hook_enabled;
              char use_whitelist;
          }
          mem_t;
          void new_set_anchored(long part, char value)
          {
              mem_t* mem = (mem_t*)0xcafecafecafecafe;
              if (mem->func_enabled == true)
              {
                  if (mem->hook_enabled == true)
                  {
                      if (mem->use_whitelist == true)
                      {
                          for (int i = 0 ; i < mem->whitelist_size ; i++)
                          {
                              if (mem->object_whitelist[i] == part)
                              {
                                  value = mem->value_array[i];
                              }
                          }
                      }
                      else
                      {
                          value = mem->value_array[0];
                      }
                  }
                  mem->orig_func(part, value);
              }
          }



     */
    
    func_wrap_info_t fwi;
    fwi.initiated = true;
    fwi.target_func_x86 = func_x86;
    fwi.target_func_arm = find_rosetta_arm_func_from_x86(task, func_x86, &(fwi.target_func_arm_ribm_ptr), &(fwi.target_func_arm_r_ptr));
    fwi.custom_mem = vm_allocate_rwx(task, NULL, sizeof(custom_rbx_anchored_wrap_custom_mem_t));
    memcpy(bytes + 0x10 + 2, &fwi.custom_mem, 8);
    fwi.alloc_func_x86 = vm_allocate_rwx(task, bytes, sizeof(bytes));
    while (shared_memory->FUNCTION_QUEUE_FINISHED == false) {usleep(1000); }
    shared_memory->FUNCTION_QUEUE_COUNT = 1;
    shared_memory->FUNCTION_QUEUE[0].address = fwi.alloc_func_x86;
    shared_memory->FUNCTION_QUEUE[0].type = 0;
    shared_memory->FUNCTION_QUEUE[0].async_type = FUNCTION_ASYNC_TYPE_CURRENT;
    shared_memory->FUNCTION_QUEUE[0].should_execute = true;
    shared_memory->FUNCTION_QUEUE_FINISHED = false;
    while (shared_memory->FUNCTION_QUEUE_FINISHED == false) {usleep(1000); }
    fwi.alloc_func_arm = find_rosetta_arm_func_from_x86(task, fwi.alloc_func_x86, &(fwi.alloc_func_arm_ribm_ptr), &(fwi.alloc_func_arm_r_ptr));
    
    {
        vm_write(task, fwi.alloc_func_arm_r_ptr, (vm_offset_t)&fwi.target_func_arm, 8);
        vm_write(task, fwi.alloc_func_arm_ribm_ptr, (vm_offset_t)&fwi.target_func_arm, 8);
        vm_write(task, fwi.target_func_arm_r_ptr, (vm_offset_t)&fwi.alloc_func_arm, 8);
        vm_write(task, fwi.target_func_arm_ribm_ptr, (vm_offset_t)&fwi.alloc_func_arm, 8);
    }
    
    return fwi;
}

#endif
