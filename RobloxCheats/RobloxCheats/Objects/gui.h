


RBX_CREATE_BASIC_STRING_PROPERTY_GET_FUNCTION(textlabel, text, RBX_TEXTLABEL_TEXT_OFFSET);

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(frame, visible, bool, RBX_FRAME_VISIBLE_OFFSET);
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(frame, visible, bool, RBX_FRAME_VISIBLE_OFFSET);

RBX_CREATE_BASIC_STRING_PROPERTY_GET_FUNCTION(proximityprompt, actiontext, RBX_PROXIMITYPROMPT_ACTIONTEXT_OFFSET)
RBX_CREATE_BASIC_STRING_PROPERTY_GET_FUNCTION(proximityprompt, objecttext, RBX_PROXIMITYPROMPT_OBJECTTEXT_OFFSET)


RBX_CREATE_BASIC_STRING_PROPERTY_GET_FUNCTION(imagelabel, assetid, RBX_IMAGELABEL_ASSETID_STR_OFFSET)


char rbx_textlabel_contains_text(task_t task, vm_address_t tl, char* substr)
{
    char is = false;
    long str_l = 0;
    char* str = rbx_textlabel_get_text(task, tl, &str_l);
    if (str)
    {
        if (strstr(str, substr))
        {
            is = true;
        }
        vm_deallocate(mach_task_self(), (vm_address_t)str, str_l);
    }
    return is;
}

char rbx_textlabel_equals_text(task_t task, vm_address_t tl, char* eqstr)
{
    char is = false;
    long str_l = 0;
    char* str = rbx_textlabel_get_text(task, tl, &str_l);
    if (str)
    {
        if (strcmp(str, eqstr) == 0)
        {
            is = true;
        }
        vm_deallocate(mach_task_self(), (vm_address_t)str, str_l);
    }
    return is;
}


char rbx_check_if_recieved_grey_prompt(task_t task, vm_address_t coregui)
{
    vm_address_t gui = rbx_instance_find_first_child(task, coregui, "RobloxPromptGui");
    vm_address_t po = rbx_instance_find_first_child_of_class(task, gui, "Frame");
    return (rbx_instance_find_first_child_of_class(task, po, "Frame") != 0);
}



void rbx_print_textlabel_descendants(task_t task, vm_address_t instance, int depth, int max_count, int ancestor_tree_height)
{
    FOR_EACH_DESCENDANT(task, instance, max_count, depth, {
        if (rbx_instance_is_a(task, obj, "TextLabel") || rbx_instance_is_a(task, obj, "TextButton"))
        {
            long str_l = 0;
            char* str = rbx_textlabel_get_text(task, obj, &str_l);
            if (str)
            {
                printf("\n");
                printf("text: %s\n", str);
                rbx_print_ancestors(task, obj, ancestor_tree_height);
                printf("\n");
                vm_deallocate(mach_task_self_, (vm_address_t)str, str_l);
            }
        }
    })
}
