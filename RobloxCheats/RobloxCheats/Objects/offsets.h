




#pragma mark - Stable (Objects) -




//These probably won't need to be updated... but just in case...


int RBX_INSTANCE_SELF_OFFSET = 0x8;
int RBX_INSTANCE_CLASS_INFO_OFFSET = 0x18;
int RBX_INSTANCE_NAME_OFFSET = 0x40;
int RBX_INSTANCE_CHILDREN_OFFSET = 0x48;
int RBX_INSTANCE_PARENT_OFFSET = 0x58;






#pragma mark - Volatile (Objects) -





/*
 Join https://www.roblox.com/games/5332389196/Hack-Tests to update these. (You will not need libESP.dylib injected)
 Call 'find_object_offsets()' to get these.
 */


int RBX_PLACEID_OFFSET = 0x40f3018;
int RBX_TEAM_BRICKCOLOR_OFFSET = 0xb4;
int RBX_MODEL_PRIMARYPART_OFFSET = 0x178;
int RBX_INTVALUE_VALUE_OFFSET = 0xb0;
int RBX_CAMERA_CFRAME_OFFSET = 0xf4;
int RBX_CAMERA_CAMERASUBJECT_OFFSET = 0x260;
int RBX_CAMERA_FIELDOFVIEW_OFFSET = 0x274;
int RBX_PLAYERS_MAXPLAYERS_OFFSET = 0x1e0;
int RBX_PLAYER_CHARACTER_OFFSET = 0xf0;
int RBX_PLAYER_TEAM_OFFSET = 0x140;
int RBX_SKY_MOONTEXTUREID_OFFSET = 0xb8;
int RBX_PLAYER_DISPLAYNAME_OFFSET = 0x1c8;
int RBX_HUMANOID_DISPLAYNAME_OFFSET = 0x278;
int RBX_DATAMODEL_JOBID_OFFSET = 0x418;
int RBX_TEXTLABEL_TEXT_OFFSET = 0x8d8;
int RBX_BASEPART_PROPERTIES_OFFSET = 0x128;
int RBX_BASEPART_PROPERTIES_CFRAME_OFFSET = 0x118;
int RBX_BASEPART_PROPERTIES_GRAVITY_OFFSET = 0x110;
int RBX_BASEPART_PROPERTIES_VELOCITY_OFFSET = 0x148;
int RBX_BASEPART_PROPERTIES_ROTVELOCITY_OFFSET = 0x154;
int RBX_BASEPART_PROPERTIES_SIZE_OFFSET = 0x270;
int RBX_LIGHTING_FOGEND_OFFSET = 0x400;
int RBX_LIGHTING_CLOCKTIME_TICK_OFFSET = 0x191;
int RBX_FRAME_VISIBLE_OFFSET = 0x5b4;
int RBX_BASEPART_PROPERTIES_CANCOLLIDE_OFFSET = 0x2cf;
int RBX_PLAYER_MOUSE_OFFSET = 0x840;









#pragma mark - Volatile (Functions) -





//Call 'find_function_offsets()' to get these.

int RBX_INSTANCE_PARENT_SETTER_METHOD_OFFSET = 0x18724e0;
int RBX_HUMANOID_WALKSPEED_SETTER_METHOD_OFFSET = 0x5b2160;
int RBX_HUMANOID_WALKSPEED_GETTER_METHOD_OFFSET = 0x965998;
int RBX_HUMANOID_JUMPPOWER_SETTER_METHOD_OFFSET = 0x5b21ac;
int RBX_TOOL_ACTIVATE_METHOD_OFFSET = 0x124f958;
int RBX_HUMANOID_HEALTH_GETTER_METHOD_OFFSET = 0x965898;
int RBX_HUMANOID_HEALTH_SETTER_METHOD_OFFSET = 0x5b2998;
int RBX_WORKSPACE_GETSERVERTIMENOW_METHOD_OFFSET = 0x1363988;
int RBX_BASEPART_ASSEMBLYLINEARVELOCITY_SETTER_METHOD_OFFSET = 0x10cb100;
int RBX_BASEPART_ASSEMBLYLINEARVELOCITY_GETTER_METHOD_OFFSET = 0x10cafe8;
int RBX_REMOTEEVENT_FIRESERVER_METHOD_OFFSET = 0x112f188;
int RBX_REMOTEFUNCTION_INVOKESERVER_METHOD_OFFSET = 0x112011c;
int RBX_MOUSE_HIT_GETTER_METHOD_OFFSET = 0x143cf68;
int RBX_BASEPART_ANCHORED_GETTER_METHOD_OFFSET = 0x1407098;
