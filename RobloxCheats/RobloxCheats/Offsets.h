


#pragma mark - Volatile (Objects) -





/*
 Join https://www.roblox.com/games/5332389196/Hack-Tests to update these. (You will not need libESP.dylib injected)
 Call 'find_object_offsets()' to get these.
 */

#pragma mark auto


int RBX_WHAT_GAME_POINTS_TO_OFFSET = 0x509a2f8;
int RBX_PLACEID_OFFSET = 0x66acb08;
int RBX_TEAM_BRICKCOLOR_OFFSET = 0xc8;
int RBX_MODEL_PRIMARYPART_OFFSET = 0x1f8;
int RBX_INTVALUE_VALUE_OFFSET = 0xc8;
int RBX_STRINGVALUE_VALUE_OFFSET = 0xc8;
int RBX_CFRAMEVALUE_VALUE_OFFSET = 0xc8;
int RBX_CAMERA_CFRAME_OFFSET = 0xe8;
int RBX_CAMERA_CAMERASUBJECT_OFFSET = 0xd8;
int RBX_CAMERA_FIELDOFVIEW_OFFSET = 0x150;
int RBX_PLAYERS_MAXPLAYERS_OFFSET = 0x118;
int RBX_PLAYER_CHARACTER_OFFSET = 0x298;
int RBX_PLAYER_TEAM_OFFSET = 0x1c8;
int RBX_HUMANOID_DISPLAYNAME_OFFSET = 0xc8;
int RBX_SKY_MOONTEXTUREID_OFFSET = 0xd0;
int RBX_PLAYER_DISPLAYNAME_OFFSET = 0xf8;
int RBX_DATAMODEL_JOBID_OFFSET = 0x130;
int RBX_TEXTLABEL_TEXT_OFFSET = 0xba0;
int RBX_BASEPART_PROPERTIES_OFFSET = 0x150;
int RBX_BASEPART_PROPERTIES_CFRAME_OFFSET = 0x11c;
int RBX_BASEPART_PROPERTIES_GRAVITY_OFFSET = 0x114;
int RBX_BASEPART_PROPERTIES_RECIEVEAGE_OFFSET = 0x118;
int RBX_BASEPART_PROPERTIES_VELOCITY_OFFSET = 0x14c;
int RBX_BASEPART_PROPERTIES_ROTVELOCITY_OFFSET = 0x158;
int RBX_BASEPART_PROPERTIES_SIZE_OFFSET = 0x2b0;
int RBX_LIGHTING_FOGEND_OFFSET = 0x4a0;
int RBX_LIGHTING_CLOCKTIME_TICK_OFFSET = 0x1a9;
int RBX_FRAME_VISIBLE_OFFSET = 0x411;
int RBX_BASEPART_PROPERTIES_CANCOLLIDE_OFFSET = 0x313;
int RBX_PROXIMITYPROMPT_ACTIONTEXT_OFFSET = 0xc8;
int RBX_PROXIMITYPROMPT_OBJECTTEXT_OFFSET = 0xe0;
int RBX_PROXIMITYPROMPT_MAXACTIVATIONDISTANCE_OFFSET = 0x120;



#pragma mark manual

int RBX_INSTANCE_SELF_OFFSET = 0x8;
int RBX_INSTANCE_CLASS_INFO_OFFSET = 0x18;
int RBX_INSTANCE_PARENT_OFFSET = 0x48;
int RBX_INSTANCE_NAME_OFFSET = 0x68;
int RBX_INSTANCE_CHILDREN_OFFSET = 0x70;

int RBX_PLAYER_LAST_INPUT_TIMESTAMP_OFFSET = 0xa60;
int RBX_HUMANOID_HEALTH_OFFSET = 0x17c;
int RBX_HUMANOID_HIPHEIGHT_OFFSET = 0x188;
int RBX_HUMANOID_WALKSPEED_OFFSET = 0x1bc;
int RBX_HUMANOID_SEATPART_OFFSET = 0x110 - 8;

//Find what set_parent jumps to, then find the function that it calls
int RBX_SET_PARENT_HOOK_OFFSET = 0x13C68D4;
int RBX_BYTES_TO_NOP_FOR_FLY_OFFSET = 0x1382CC0;

/*
 base() + 0x119C660    stur q2, [x8, #0xf4]    02 41 8F 3C  <-----
 base() + 0x119C664    add x10, x8, #0xc4    0A 11 03 91
 base() + 0x119C668    ldr x9, [x9, #0x40]    29 21 40 F9
 base() + 0x119C66C    str x9, [x10, #0x40]    49 21 00 F9
 base() + 0x119C670    stur q1, [x8, #0xc4]    01 41 8C 3C
 */





#pragma mark - Volatile (Functions) -





//Call 'find_function_offsets()' to get these.


int RBX_TOOL_ACTIVATE_METHOD_OFFSET = 0x1c95538;
int RBX_HUMANOID_HEALTH_GETTER_METHOD_OFFSET = 0xec96b0;
int RBX_HUMANOID_HEALTH_SETTER_METHOD_OFFSET = 0xec9c4c;
int RBX_HUMANOID_WALKSPEED_GETTER_METHOD_OFFSET = 0xec9898;
int RBX_HUMANOID_WALKSPEED_SETTER_METHOD_OFFSET = 0xec98a0;
int RBX_INSTANCE_SET_PARENT_METHOD_OFFSET = 0x13df978;
int RBX_HUMANOID_RIGTYPE_GETTER_METHOD_OFFSET = 0xeca3fc;


