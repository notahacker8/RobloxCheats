

#define RBX_TDS_MODE_TYPE_MOLTEN_TOYBOARD_DEATH_FARM 1
#define RBX_TDS_MODE_TYPE_HARDCORE_CROSSROADS_DEATH_FARM 2

#define RBX_TDS_LOBBY_PLACE_ID 3260590327

typedef struct
{
    int type;
    vector3_t pos;
}
tds_tower_placement_t;

char tds_check_if_tower_loaded(task_t task, vm_address_t towers_folder, long idx)
{
    char __loaded = false;
    long tc = 0;
    rbx_child_t* t = rbx_instance_get_children(task, towers_folder, &tc);
    if (t)
    {
        if (tc > idx)
        {
            /*
             It seems that while towers are still loading, they dont have this child...
             */
            if (rbx_instance_find_first_child(task, t[idx].child_address, "Display"))
            {
                __loaded = true;
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)t, tc * sizeof(rbx_child_t));
    }
    return __loaded;
}


long tds_get_selected_tower_level(task_t task, vm_address_t level_textlabel)
{
    long level = 0;
    
    long __l = 0;
    char* c = rbx_textlabel_get_text(task, level_textlabel, &__l);
    if (c)
    {
        level = atoi(c + 7); //Level: %d [+7 until integer]
        vm_deallocate(mach_task_self_, (vm_address_t)c, __l);
    }
    
    return level;
}

long tds_get_tower_count(task_t task, vm_address_t towers_folder)
{
    long tc = 0;
    rbx_child_t* t = rbx_instance_get_children(task, towers_folder, &tc);
    if (t)
    {
        vm_deallocate(mach_task_self_, (vm_address_t)t, tc * sizeof(rbx_child_t));
    }
    return tc;
}



void tds_print_placements(task_t task, int format_mode)
{
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    //vm_address_t rs = rbx_instance_find_first_child(task, game, "ReplicatedStorage");
    
    //rbx_print_descendants(task, rs, 0, 3);
    //vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t towers_folder = rbx_instance_find_first_child(task, workspace, "Towers");
    long tower_count = 0;
    rbx_child_t* towers = rbx_instance_get_children(task, towers_folder, &tower_count);
    if (towers)
    {
        for (long i = 0 ; i < tower_count ; i++)
        {
            int type = 1;
            vm_address_t tower = towers[i].child_address;
            //rbx_print_descendants(task, tower, 0, 1);
            vm_address_t tower_hrp = rbx_instance_find_first_child(task, tower, "HumanoidRootPart");
            rbx_cframe_t thrpcf = rbx_basepart_get_cframe(task, tower_hrp);
            printf("(tds_tower_placement_t){.type = %d, .pos = (vector3_t){.x = %f, .y = %f, .z = %f}},\t", type, thrpcf.pos.x, thrpcf.pos.y, thrpcf.pos.z);
            //printf("\t[%ld]  (%f, %f, %f) \t\t", i, thrpcf.pos.x, thrpcf.pos.y, thrpcf.pos.z);
            long len = 0;
            char* name = rbx_instance_get_name(task, tower, &len);
            printf("// [%ld] %s\n", i, name);
            vm_deallocate(mach_task_self_, (vm_address_t)name, len);
            //rbx_print_descendants(task, tower, 0, 0);
        }
        vm_deallocate(mach_task_self_, (vm_address_t)towers, tower_count * sizeof(rbx_child_t));
    }
}

void U_TDS_CLICK(libESPSharedMemoryObject* shared_memory, float _x, float _y)
{
    send_mousemove_yielding(shared_memory, 0, _x, _y);
    send_leftclick_yielding(shared_memory, 1, _x, _y, 100000);
}

void U_TDS_KEYPRESS(libESPSharedMemoryObject* shared_memory, char key)
{
    send_keypress_yielding(shared_memory, 2, keycode_for_character(key), &key, 200000);
}

#define TDS_MOUSEMOVE(_x, _y) U_TDS_MOUSEMOVE(shared_memory, _x, _y)
#define TDS_CLICK(_x, _y) U_TDS_CLICK(shared_memory, _x, _y)
#define TDS_KEYPRESS(key) U_TDS_KEYPRESS(shared_memory, key)

#define TDS_SKIP_CLICK TDS_CLICK(440, 510)
#define TDS_CENTER_CLICK TDS_CLICK(shared_memory->WINDOW_W/2, shared_memory->WINDOW_H/2)


#define TDS_SELECT_TOWER(tds_placement) \
new_camera_cframe.pos = tds_placement.pos;\
new_camera_cframe.pos.y += 5;\
should_set_camera_cframe = true;\
send_mousemove_yielding(shared_memory, 0, shared_memory->WINDOW_W/2, shared_memory->WINDOW_H/2);\
sleep(1);\
TDS_CENTER_CLICK;\
usleep(30000);\
should_set_camera_cframe = false;\


#define TDS_DESELECT_TOWER() \
new_camera_cframe.pos.x = -99999;\
new_camera_cframe.pos.y = -99999;\
new_camera_cframe.pos.z = -99999;\
should_set_camera_cframe = true;\
send_mousemove_yielding(shared_memory, 0, shared_memory->WINDOW_W/2, shared_memory->WINDOW_H/2);\
sleep(1);\
TDS_CENTER_CLICK;\
usleep(30000);\
should_set_camera_cframe = false;\


#define TDS_PLACE_TOWER(tds_placement) \
TDS_KEYPRESS('0' + tds_placement.type);\
TDS_SELECT_TOWER(tds_placement)\


#define TDS_WAIT_WHILE_COND(x) \
while (x)\
{\
usleep(100000);\
}

#define TDS_WAIT_FOR_TOWER_LOAD(idx) \
while (tds_check_if_tower_loaded(task, towers_folder, idx) == false)\
{\
usleep(300000);\
}

#define TDS_UPGRADE_UNTIL_LEVEL(lvl, tl)\
while (tds_get_selected_tower_level(task, tl) < lvl)\
{\
TDS_KEYPRESS('e');\
usleep(200000);\
}



long TDS_UPGRADE_TOWER(task_t task, libESPSharedMemoryObject* shared_memory, vm_address_t reactupgrades_gui, int times)
{
    vm_address_t frame = rbx_instance_find_first_child(task, reactupgrades_gui, "upgrades");
    if (!frame)
    {
        return -1;
    }
    vm_address_t leveltext_tl = rbx_instance_find_first_descendant(task, reactupgrades_gui, "levelText", "TextLabel", 1000, 100000);
    long lvl = tds_get_selected_tower_level(task, leveltext_tl);
    for (int i = 0 ; i < times ; i++)
    {
        TDS_KEYPRESS('e');
    }
    sleep(1);
    lvl = tds_get_selected_tower_level(task, leveltext_tl);
    return lvl;
}

#warning UPDATE THIS IF NEEDED
char tds_get_can_skip(task_t task, vm_address_t vote_prompt_tl)
{
    return (vm_read_uint8_t_value(task, vote_prompt_tl + 0x131) == 1);
}


#include "Play-Round.h"
