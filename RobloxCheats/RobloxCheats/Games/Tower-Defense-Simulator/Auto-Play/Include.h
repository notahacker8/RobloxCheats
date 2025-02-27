
#define RBX_TDS_ELEVATOR_TYPE_DEFAULT 7
#define RBX_TDS_ELEVATOR_TYPE_HARDCORE 8

#define RBX_TDS_MODE_TYPE_MOLTEN 1
#define RBX_TDS_MODE_TYPE_HARDCORE 2
#define RBX_TDS_MODE_TYPE_HARDCORE_ALT1 3

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
        level = atoi(c);
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
    rbx_print_descendants(task, workspace, 0, 1);
    //vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t towers_folder = rbx_instance_find_first_child(task, workspace, "Towers");
    long tower_count = 0;
    rbx_child_t* towers = rbx_instance_get_children(task, towers_folder, &tower_count);
    if (towers)
    {
        for (long i = 0 ; i < tower_count ; i++)
        {
            int type = 0;
            if (format_mode == RBX_TDS_MODE_TYPE_MOLTEN)
            {
                switch (i) {
                    case 0:
                        type = 4;
                        break;
                    case 1:
                        type = 1;
                        break;
                    case 2:
                        type = 4;
                        break;
                    case 3:
                        type = 1;
                        break;
                    case 4:
                        type = 1;
                        break;
                    case 5:
                        type = 4;
                        break;
                    case 6:
                        type = 1;
                        break;
                    case 7:
                        type = 5;
                        break;
                    case 8:
                        type = 1;
                        break;
                    case 9:
                        type = 1;
                        break;
                    case 10:
                        type = 1;
                        break;
                    case 11:
                        type = 1;
                        break;
                    case 12:
                        type = 2;
                        break;
                    case 13:
                        type = 3;
                        break;
                    default:
                        type = 5;
                        break;
                }
            }
            if (format_mode == RBX_TDS_MODE_TYPE_HARDCORE || format_mode == RBX_TDS_MODE_TYPE_HARDCORE_ALT1)
            {
                type = 1;
            }
            vm_address_t tower = towers[i].child_address;
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



#define TDS_CLICK(_x, _y) \
shared_memory->INPUT_QUEUE[0] = (Input){.type = INPUT_TYPE_MOUSE_MOVE, .should_execute = true, .x = _x, .y = _y};\
shared_memory->INPUT_QUEUE_FINISHED = false;\
usleep(100000);\
while (shared_memory->INPUT_QUEUE_FINISHED == false) {usleep(1000);}\
shared_memory->INPUT_QUEUE[1] = (Input){.type = INPUT_TYPE_LEFT_MOUSE_DOWN, .should_execute = true, .x = _x, .y = _y};\
shared_memory->INPUT_QUEUE_FINISHED = false;\
usleep(1000);\
while (shared_memory->INPUT_QUEUE_FINISHED == false) {usleep(1000);}\
shared_memory->INPUT_QUEUE[1] = (Input){.type = INPUT_TYPE_LEFT_MOUSE_UP, .should_execute = true, .x = _x, .y = _y};\
shared_memory->INPUT_QUEUE_FINISHED = false;\
usleep(1000);\
while (shared_memory->INPUT_QUEUE_FINISHED == false) {usleep(1000);}

#define TDS_CENTER_CLICK TDS_CLICK(shared_memory->WINDOW_W/2, shared_memory->WINDOW_H/2)
#define TDS_RETURN_TO_LOBBY_CLICK TDS_CLICK(shared_memory->WINDOW_W/2, 146)
#define TDS_MOLTEN_CLICK TDS_CLICK(400, 285)
#define TDS_SKIP_CLICK TDS_CLICK(425, 455)
#define TDS_EXIT_ELEVATOR_CLICK TDS_CLICK(535, 185)

#define TDS_KEYPRESS(x) \
shared_memory->INPUT_QUEUE[2] = (Input){.type = INPUT_TYPE_KEY_DOWN, .should_execute = true, .keycode = keycode_for_character(x)};\
shared_memory->INPUT_QUEUE_FINISHED = false;\
usleep(10000);\
while (shared_memory->INPUT_QUEUE_FINISHED == false) {usleep(1000);}\
shared_memory->INPUT_QUEUE[2] = (Input){.type = INPUT_TYPE_KEY_UP, .should_execute = true, .keycode = keycode_for_character(x)};\
shared_memory->INPUT_QUEUE_FINISHED = false;\
usleep(1000);\
while (shared_memory->INPUT_QUEUE_FINISHED == false) {usleep(1000);}

#define TDS_SELECT_TOWER(tds_placement) \
new_camera_cframe.pos = tds_placement.pos;\
new_camera_cframe.pos.y += 5;\
should_set_camera_cframe = true;\
usleep(10000);\
TDS_CENTER_CLICK;\
usleep(10000);\
should_set_camera_cframe = false;\

#define TDS_SELECT_TOWER_SAFE(tds_placement)\
TDS_SELECT_TOWER(tds_placement)\
while (rbx_frame_get_visible(task, roactupgrades_upgrades_frame) == false)\
{\
TDS_SELECT_TOWER(tds_placement)\
usleep(100000);\
}\


#define TDS_PLACE_TOWER(tds_placement) \
TDS_KEYPRESS('0' + tds_placement.type);\
TDS_SELECT_TOWER(tds_placement)\

#define TDS_PLACE_TOWER_SAFE(tds_placement)\
{\
long __tc = tds_get_tower_count(task, towers_folder);\
while (tds_get_tower_count(task, towers_folder) < (__tc + 1))\
{\
TDS_PLACE_TOWER(tds_placement)\
usleep(10000);\
}\
}\

#define TDS_WAIT_WHILE_COND_AND_SKIP(x, skiptime) \
while (x)\
{\
if (time < skiptime || time == skiptime)\
{\
TDS_SKIP_CLICK;\
}\
usleep(100000);\
}
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

#define TDS_UPGRADE_UNTIL_LEVEL(lvl)\
while (tds_get_selected_tower_level(task, roactupgrades_level_content_tl) < lvl)\
{\
TDS_KEYPRESS('e');\
usleep(200000);\
}


#include "Placements/Molten.h"
#include "Placements/Hardcore.h"
#include "Find-Elevator.h"
#include "Play-Round.h"


/*
 
 Will automate the process of finding an elevator, playing a round, and rejoining.
 Iterations are only counted after 'tds_play_round()'.
 
 */

/*
 
 This code works like this:
    if RobloxCheats runs with no arguments, then start a loop.
        This loop will check the place ID of the game you're in, and the current iteration.
            if the current iteration is or is greater than the max iteration count, suspend Roblox and terminate all RobloxCheats processes
            if the place ID is the lobby place ID, then run another Instance of RobloxCheats with the argument '1'
            if the place ID is NOT the lobby place ID, then run another Instance of RobloxCheats with the argument '2'
    if RobloxCheats runs with arguments, then check the first argument.
        create a loop that checks if the parent process exists, if it doesnt, exit()
        if the first argument is '1', then do 'tds_find_elevator()'
        if the first argument is '2', then do 'tds_play_round()' and increment the current iteration.
 
 */


#define RBX_TDS_REPEAT_FIND_GAME_AND_PLAY_GAME(task, elevator_type, mode_type, map_list, iterations)\
{\
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);\
    if (argc > 1)\
    {\
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{\
    for (;;)\
    {\
        char* parent_proc_name = name_by_pid(getppid());\
        if (parent_proc_name)\
        {\
            free(parent_proc_name);\
        }\
        else\
        {\
            printf("Parent process can no longer be found...\n");\
            exit(0);\
        }\
        sleep(1);\
    }\
});\
        if (atoi(argv[1]) == 1)\
        {\
            tds_find_elevator(task, elevator_type, map_list, sizeof(map_list)/sizeof(char*));\
        }\
        if (atoi(argv[1]) == 2)\
        {\
            tds_play_round(task, mode_type, true, true);\
        }\
    }\
    else\
    {\
        int current_iteration = 0;\
        int max_iterations = iterations;\
        for (;;)\
        {\
            if (current_iteration >= max_iterations)\
            {\
                task_suspend(task);\
                system("killall RobloxCheats");\
            }\
            else\
            {\
                long place_id = vm_read_uint64_t_value(task, task_base_address + RBX_PLACEID_OFFSET);\
                int __a = 0;\
                if (place_id == RBX_TDS_LOBBY_PLACE_ID)\
                {\
                    __a = 1;\
                }\
                else\
                {\
                    current_iteration++;\
                    __a = 2;\
                }\
                char cmd[strlen(argv[0]) + 5];\
                bzero(cmd, sizeof(cmd));\
                sprintf(cmd, "%s %d", argv[0], __a);\
                system(cmd);\
            }\
            sleep(1);\
        }\
    }\
}
