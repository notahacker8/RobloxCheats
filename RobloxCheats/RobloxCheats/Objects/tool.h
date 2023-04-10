

void rbx_tool_set_canbedropped(task_t task, vm_address_t tool, char new_value)
{
    vm_write(task, tool + RBX_TOOL_CANBEDROPPED_OFFSET, (vm_address_t)&new_value, sizeof(char));
}

