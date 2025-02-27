

char* rbx_textlabel_get_text(task_t task, vm_address_t textlabel, long* inout_len)
{
    return vm_read_rbx_string(task, textlabel + RBX_TEXTLABEL_TEXT_OFFSET, inout_len);
}


RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(frame, visible, bool, RBX_FRAME_VISIBLE_OFFSET);
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(frame, visible, bool, RBX_FRAME_VISIBLE_OFFSET);
