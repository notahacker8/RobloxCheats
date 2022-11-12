


char* rbx_textlabel_get_short_text(task_t task, vm_address_t textlabel, char* inout_len)
{
    static mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    char length;
    char* text;
    kern_return_t kr;
    kr = vm_read(task, textlabel + RBX_TEXT_LABEL_SHORT_TEXT_OFFSET, 1, (vm_offset_t*)&read_data, &data_cnt);
    _KR_
    length = *(char*)read_data;
    vm_deallocate(mach_task_self_, read_data, 1);
    
    kr = vm_read(task, textlabel + RBX_TEXT_LABEL_SHORT_TEXT_OFFSET + 1, length/2, (vm_offset_t*)&text, &data_cnt);
    _KR_
    *inout_len = length/2;

    return text;
}

char* rbx_textlabel_get_long_text(task_t task, vm_address_t textlabel, char* inout_len)
{
    static mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    char length;
    char* text;
    kern_return_t kr;
    kr = vm_read(task, textlabel + 0xa48, 8, (vm_offset_t*)&read_data, &data_cnt);
    _KR_
    vm_address_t str_ptr = *(vm_address_t*)read_data;
    vm_deallocate(mach_task_self_, read_data, 8);
    kr = vm_read(task, str_ptr, 1, (vm_offset_t*)&read_data, &data_cnt);
    _KR_
    length = *(char*)read_data;
    vm_deallocate(mach_task_self_, read_data, 1);
    
    kr = vm_read(task, str_ptr, length/2, (vm_offset_t*)&text, &data_cnt);
    _KR_
    *inout_len = length/2;

    return text;
}


char* rbx_textlabel_get_text(task_t task, vm_address_t textlabel, long* inout_len)
{
    static mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    
    vm_address_t long_text_ptr;
    vm_address_t _text;
    char length;
    char* text;
    
    kern_return_t kr;
    kr = vm_read(task, textlabel + RBX_TEXT_LABEL_SHORT_TEXT_OFFSET + 0x10, 8, (vm_offset_t*)&read_data, &data_cnt);
    _KR_
    long_text_ptr = *(vm_address_t*)read_data;
    vm_deallocate(mach_task_self_, read_data, 8);
    if (long_text_ptr == 0)
    {
        //printf("short text\n");
        _text = textlabel + RBX_TEXT_LABEL_SHORT_TEXT_OFFSET + 1;
        kr = vm_read(task, textlabel + RBX_TEXT_LABEL_SHORT_TEXT_OFFSET, 1, (vm_offset_t*)&read_data, &data_cnt);
        _KR_;
        length = *(char*)read_data;
        vm_deallocate(mach_task_self_, read_data, 1);
        *inout_len = (long)length;
        kr = vm_read(task, _text, length, (vm_offset_t*)&text, &data_cnt);
    }
    else
    {
        //printf("long text\n");
        _text = long_text_ptr;
        
        kr = vm_read(task, textlabel + RBX_TEXT_LABEL_SHORT_TEXT_OFFSET + 8, 8, (vm_offset_t*)&read_data, &data_cnt);
        _KR_;
        length = *(char*)read_data;
        *inout_len = (long)length;
        vm_deallocate(mach_task_self_, read_data, 8);
        kr = vm_read(task, _text, length, (vm_offset_t*)&text, &data_cnt);
    }
    
    return text;
}
