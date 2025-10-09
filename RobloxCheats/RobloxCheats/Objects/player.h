

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(player, team, vm_address_t, RBX_PLAYER_TEAM_OFFSET);
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(player, team, vm_address_t, RBX_PLAYER_TEAM_OFFSET);

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(player, character, vm_address_t, RBX_PLAYER_CHARACTER_OFFSET);
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(player, character, vm_address_t, RBX_PLAYER_CHARACTER_OFFSET);

RBX_CREATE_BASIC_STRING_PROPERTY_GET_FUNCTION(player, displayname, RBX_PLAYER_DISPLAYNAME_OFFSET);

RBX_CREATE_BASIC_PROPERTY_GET_FUNCTION(player, last_input_timestamp, double, RBX_PLAYER_LAST_INPUT_TIMESTAMP_OFFSET);
RBX_CREATE_BASIC_PROPERTY_SET_FUNCTION(player, last_input_timestamp, double, RBX_PLAYER_LAST_INPUT_TIMESTAMP_OFFSET);


vm_address_t rbx_player_get_player_from_character(task_t task, vm_address_t character, vm_address_t players_service)
{
    vm_address_t _p = 0;
    FOR_EACH_CHILD(players_service, {
        if (character == rbx_player_get_character(task, child))
        {
            _p = child;
            i = child_count;
        }
    })
    return _p;
}
