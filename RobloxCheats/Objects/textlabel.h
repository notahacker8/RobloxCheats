


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
