

void tds_cheat(task_t task)
{
    vm_address_t task_base_address = task_get_image_address_by_index(task, 0);
    
    static char should_search = false;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        while (!should_search)
        {
            sleep(1);
        }
        should_search = false;
        
        MEMORY_SEARCH_HEADER(VM_MEMORY_IOACCELERATOR)
        for (long i = 16 ; i < (size - 8 - 16) ; i += 8)
        {
            MEMORY_SEARCH_LOCAL_VARIABLE_FILTER_HEADER(var_type == 3)
            //if (vm_read_double_value(task, var_ptr) == 85)
            {
                //double read_double = *(double*)(read_data + i - 0x10);
                char nm[100]; bzero(nm, sizeof(nm)); int nm_l = 0;
                //char* sv[100]; bzero(sv, sizeof(sv)); long sv_l = 0;
                rbx_local_variable_get_string_value(task, var_name_ptr, nm, &nm_l);
                //printf("%s\n", nm);
                //rbx_local_variable_get_string_value(task, *((vm_address_t*)(read_data + i - 0x10)), sv, &sv_l);
                if (strcmp(nm, "Cooldown") == 0)
                {
                    // printf("%p\n", var_ptr);
                    double __a = 0.0001;
                    vm_write(task, var_ptr, &__a, 8);
                }
            }
            MEMORY_SEARCH_LOCAL_VARIABLE_FILTER_FOOTER
        }
        MEMORY_SEARCH_FOOTER
   });
    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_INPUT_SIMPLE("s", "search", {
            should_search = true;
        })
    })
    
    
}
/*
Ioaccelerator, rw-    0x11A3E7060    45.000000
Ioaccelerator, rw-    0x1292B8460    45.000000
Ioaccelerator, rw-    0x1292BDE60    45.000000
Ioaccelerator, rw-    0x1294F8E60    45.000000
Ioaccelerator, rw-    0x12961CA60    45.000000
Ioaccelerator, rw-    0x12AC79860    45.000000
Ioaccelerator, rw-    0x423D09860    45.000000
*/
