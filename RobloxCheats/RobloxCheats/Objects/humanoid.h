

char* rbx_humanoid_get_displayname(task_t task, vm_address_t humanoid, long* inout_len)
{
    return vm_read_rbx_string(task, humanoid + RBX_HUMANOID_DISPLAYNAME_OFFSET, inout_len);
}


RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(humanoid, health, float, RBX_HUMANOID_HEALTH_OFFSET)
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(humanoid, health, float, RBX_HUMANOID_HEALTH_OFFSET)

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(humanoid, walkspeed, float, RBX_HUMANOID_WALKSPEED_OFFSET)
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(humanoid, walkspeed, float, RBX_HUMANOID_WALKSPEED_OFFSET)

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(humanoid, seatpart, vm_address_t, RBX_HUMANOID_SEATPART_OFFSET)
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(humanoid, seatpart, vm_address_t, RBX_HUMANOID_SEATPART_OFFSET)
