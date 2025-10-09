
#define RBX_LIGHTING_CLOCKTIME_HOUR_TICK_COUNT 14062500

/*
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
*/

RBX_CREATE_BASIC_STRING_PROPERTY_GET_FUNCTION(sky, moontextureid, RBX_SKY_MOONTEXTUREID_OFFSET)
RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(lighting, clocktime_tick, uint32_t, RBX_LIGHTING_CLOCKTIME_TICK_OFFSET)
