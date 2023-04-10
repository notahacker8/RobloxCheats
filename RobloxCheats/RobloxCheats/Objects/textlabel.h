

char* rbx_textlabel_get_text(task_t task, vm_address_t textlabel, long* inout_len)
{
    mach_msg_type_number_t data_cnt;
    char* text = NULL;
    unsigned char short_length = vm_read_1byte_value(task, textlabel + RBX_TEXTLABEL_SHORT_TEXT_OFFSET);
    if (short_length > 44)
    {
        long long_length = vm_read_8byte_value(task, textlabel + RBX_TEXTLABEL_SHORT_TEXT_OFFSET + 8);
        vm_address_t str_ptr = vm_read_8byte_value(task, textlabel + RBX_TEXTLABEL_SHORT_TEXT_OFFSET + 16);
        if (is_valid_pointer(task, str_ptr))
        {
            *inout_len = long_length;
            vm_read(task, str_ptr, long_length, (vm_offset_t*)&text, &data_cnt);
        }
    }
    else
    {
        *inout_len = (long)short_length;
        vm_read(task, textlabel + RBX_TEXTLABEL_SHORT_TEXT_OFFSET + 1, (short_length/2), (vm_offset_t*)&text, &data_cnt);
    }
    return text;
}



