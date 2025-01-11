

char* rbx_textlabel_get_text(task_t task, vm_address_t textlabel, long* inout_len)
{
    return vm_read_rbx_string(task, textlabel + RBX_TEXTLABEL_TEXT_OFFSET, inout_len);
}

char rbx_frame_get_visible(task_t task, vm_address_t frame)
{
    return vm_read_uint8_t_value(task, frame + RBX_FRAME_VISIBLE_OFFSET);
}


