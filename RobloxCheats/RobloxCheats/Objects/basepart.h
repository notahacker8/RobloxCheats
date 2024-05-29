


vm_address_t rbx_basepart_get_properties_address(task_t task, vm_address_t basepart)
{
    return vm_read_uint64_value(task, basepart + RBX_BASEPART_PROPERTIES_OFFSET);
}




float rbx_basepart_get_gravity(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_float_value(task, p_a + RBX_BASEPART_PROPERTIES_GRAVITY_OFFSET);
}

void rbx_basepart_set_gravity(task_t task, vm_address_t basepart, float new_value)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_BASEPART_PROPERTIES_GRAVITY_OFFSET, (vm_address_t)&new_value, sizeof(float));
}




rbx_cframe_t rbx_basepart_get_cframe(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_rbx_cframe_value(task, p_a + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET);
}

void rbx_basepart_set_cframe(task_t task, vm_address_t basepart, rbx_cframe_t cf)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET, (vm_address_t)&cf, sizeof(rbx_cframe_t));
    return;
}




vector3_t rbx_basepart_get_velocity(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_vector3_value(task, p_a + RBX_BASEPART_PROPERTIES_VELOCITY_OFFSET);
}

void rbx_basepart_set_velocity(task_t task, vm_address_t basepart, vector3_t new_velocity)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_BASEPART_PROPERTIES_VELOCITY_OFFSET, (vm_address_t)&new_velocity, sizeof(vector3_t));
    //vm_write(task, p_a + RBX_BASEPART_PROPERTIES_VELOCITY_OFFSET + 0x48, (vm_address_t)&_n, sizeof(vector3_t));
}




vector3_t rbx_basepart_get_rotvelocity(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_vector3_value(task, p_a + RBX_BASEPART_PROPERTIES_ROTVELOCITY_OFFSET);
}

void rbx_basepart_set_rotvelocity(task_t task, vm_address_t basepart, vector3_t new_velocity)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_BASEPART_PROPERTIES_ROTVELOCITY_OFFSET, (vm_address_t)&new_velocity, sizeof(vector3_t));
}




vector3_t rbx_basepart_get_size(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_vector3_value(task, p_a + RBX_BASEPART_PROPERTIES_SIZE_OFFSET);
}

void rbx_basepart_set_size(task_t task, vm_address_t basepart, vector3_t new_size)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_BASEPART_PROPERTIES_SIZE_OFFSET, (vm_address_t)&new_size, sizeof(vector3_t));
}


void rbx_basepart_set_cancollide(task_t task, vm_address_t basepart, char value)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    vm_write(task, p_a + RBX_BASEPART_PROPERTIES_CANCOLLIDE_OFFSET, (vm_address_t)&value, 1);
}

char rbx_basepart_get_cancollide(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_uint8_value(task,  p_a + RBX_BASEPART_PROPERTIES_CANCOLLIDE_OFFSET);
}


int rbx_basepart_get_recieveage(task_t task, vm_address_t basepart)
{
    vm_address_t p_a = rbx_basepart_get_properties_address(task, basepart);
    return vm_read_uint32_value(task,  p_a + RBX_BASEPART_PROPERTIES_RECIEVEAGE_OFFSET);
}
