
#define RBX_LIGHTING_CLOCKTIME_HOUR_TICK_COUNT 14062500


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

char* rbx_sky_get_moontextureid(task_t task, vm_address_t address, long* inout_len)
{
    return vm_read_rbx_string(task, address + RBX_SKY_MOONTEXTUREID_OFFSET, inout_len);
}

unsigned int rbx_lighting_get_clocktime_tick(task_t task, vm_address_t address)
{
    return vm_read_4byte_value(task, address + RBX_LIGHTING_CLOCKTIME_TICK_OFFSET);
}
