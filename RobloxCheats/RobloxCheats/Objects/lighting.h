


void rbx_lighting_set_brightness(task_t task, vm_address_t address, float value)
{
    vm_write(task, address + RBX_LIGHTING_BRIGHTNESS_OFFSET, (vm_offset_t)&value, 4);
}

void rbx_lighting_set_fogend(task_t task, vm_address_t address, float value)
{
    vm_write(task, address + RBX_LIGHTING_FOGEND_OFFSET, (vm_offset_t)&value, 4);
}

void rbx_lighting_set_ambient(task_t task, vm_address_t address, vector3_t value)
{
    vm_write(task, address + RBX_LIGHTING_AMBIENT_OFFSET, (vm_offset_t)&value, sizeof(vector3_t));
}
