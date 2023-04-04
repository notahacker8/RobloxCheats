//
//  main.m
//  RobloxCheats
//
//  Created by me on 9/10/22.
//

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
}
rbx_rgb_t;

typedef union
{
    int i;
    float f;
}
int_float_u;

typedef union
{
    long l;
    double d;
}
long_double_u;

#import <Foundation/Foundation.h>
#include <ApplicationServices/ApplicationServices.h>
#import <Cocoa/Cocoa.h>

#include <stdio.h>
#include <stdlib.h>
#include <libproc.h>
#include <mach/mach.h>
#include <mach-o/loader.h>
#include <mach/mach_vm.h>
#include <mach/arm64/asm.h>
#include <mach/arm/thread_state.h>
#include <mach/arm/thread_status.h>
#include <mach/thread_state.h>
#include <mach/thread_status.h>
#include <mach/mach_init.h>
#include <mach/thread_policy.h>
#include <mach-o/dyld_images.h>
#include <mach/arm/_structs.h>
#include <dlfcn.h>
#include <math.h>
#include <pthread.h>
#include <pthread/sched.h>


#define __INJECTED_DYLIB_PATH__ "/Users/dimitriarmendariz/Library/Developer/Xcode/DerivedData/ESP-drqlefpymzgresdhcnxyusawdjhf/Build/Products/Debug/libESP.dylib"
#define __CPUTHROTTLE_PATH__ "/Users/dimitriarmendariz/Library/Developer/Xcode/DerivedData/cputhrottle_remake-dlbdozizqeucwzedjzkryqbozbek/Build/Products/Debug/cputhrottle_remake"
#define __COOKIE_TXT_PATH__ "/Users/dimitriarmendariz/Documents/roblox-cookie.txt"



#define PI 3.1415926535








///Check if a module is loaded into the memory of a process.
vm_address_t get_image_address(const task_t task,
                               const char* image_path)
{
    vm_address_t image_address = 0;
    kern_return_t kr;
    mach_msg_type_number_t size = 0;
    mach_msg_type_number_t data_cnt = 0;
    
    struct task_dyld_info dyld_info;
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    kr = task_info(task, TASK_DYLD_INFO, (task_info_t)&dyld_info, &count);
    struct dyld_all_image_infos* infos;
    size = sizeof(struct dyld_all_image_infos);
    vm_read(task, dyld_info.all_image_info_addr, size, (vm_address_t*)&infos, &data_cnt);
    int info_count = infos -> infoArrayCount;
    
    size = sizeof(struct dyld_image_info);
    
    for (int i = 0 ; i < info_count ; i++)
    {
        vm_address_t diia = ((vm_address_t)(infos -> infoArray)) + (i * size);
        struct dyld_image_info* i;
        vm_read(task, diia, size, (vm_address_t*)&i, &data_cnt);
        vm_address_t img_file_path_ptr = (vm_address_t)(i -> imageFilePath);
        vm_address_t img_ld_address = (vm_address_t)(i -> imageLoadAddress);
        char* img_file_path;
        kr = vm_read(task, img_file_path_ptr, PATH_MAX, (vm_address_t*)&img_file_path, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            if (strcmp(image_path, img_file_path) == 0)
            {
                image_address = img_ld_address;
            }
        }
    }
    return image_address;
}




int pids_by_name(char* name, pid_t* inout_array)
{
    pid_t pids[4096];
    int pid_index = 0;
    int count = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    int proc_count = count/sizeof(pid_t);
    for (int i = 0; i < proc_count; i++)
    {
        struct proc_bsdinfo proc;
        int st = proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0, &proc, PROC_PIDTBSDINFO_SIZE);
        if (st == PROC_PIDTBSDINFO_SIZE)
        {
            if (strcmp(name, proc.pbi_name) == 0)
            {
                inout_array[pid_index] = pids[i];
                pid_index++;
            }
        }
    }
    return pid_index;
}

vm_address_t** task_get_regions(task_t task, int region_count)
{
    kern_return_t kr;
    vm_address_t address = 0;
    vm_size_t size = 0;
    uint32_t depth = 0;
    
    vm_address_t** a = malloc(16);
    a[0] = malloc(8 * region_count);
    a[1] = malloc(8 * region_count);
    
    for (int i = 0 ; i < region_count ; i++)
    {
        struct vm_region_submap_info_64 info;
        mach_msg_type_number_t count = VM_REGION_SUBMAP_INFO_COUNT_64;
        kr = vm_region_recurse_64(task, &address, &size, &depth, (vm_region_info_64_t)&info, &count);
        if (kr != KERN_SUCCESS)
        {
            break;
        }
        if (info.is_submap){
            depth++;
        }
        else {
            a[0][i] = address; //First pointer is an array of region starts;
            a[1][i] = address + size; //Second pointer is an array of region ends;
            address += size;
        }
    }
    return a;
}

vm_address_t get_base_address(task_t task)
{
    vm_address_t** regions = task_get_regions(task, 1);
    vm_address_t a = regions[0][0];
    free((void*)regions);
    return a;
}

void wait_until_input_queue_finished(task_t task, vm_address_t address, int usleep_time)
{
    mach_msg_type_number_t data_cnt;
    
    for (;;)
    {
        vm_address_t read_data;
        kern_return_t kr = vm_read(task, address, 1, &read_data, &data_cnt);
        if (kr == KERN_SUCCESS)
        {
            char f = *(char*)read_data;
            vm_deallocate(mach_task_self_, read_data, 1);
            if (f == false)
            {
                usleep(usleep_time);
            }
            if (f == true)
            {
                return;
            }
        }
    }
}


unsigned long vm_read_8byte_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    long __data = 0;
    kern_return_t kr;
    kr = vm_read(task, address, 8, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        __data = *(long*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 8);
    }
    return __data;
}

unsigned int vm_read_4byte_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    int __data = 0;
    kern_return_t kr;
    kr = vm_read(task, address, sizeof(int), (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        __data = *(int*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
    }
    return __data;
}

unsigned char vm_read_1byte_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    char __data = 0;
    kern_return_t kr;
    kr = vm_read(task, address, 1, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        __data = *(char*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 1);
    }
    return __data;
}

char is_valid_pointer(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    char valid = false;
    kern_return_t kr = vm_read(task, address, 1, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        valid = true;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 1);
    }
    return valid;
}


#include "Objects/offsets.h"

#include "Objects/brickcolors.h"
#include "Objects/ESP.h"

#include "Objects/vector3.h"
#include "Objects/cframe.h"

vector3_t vm_read_vector3_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    vector3_t __data;
    bzero(&__data, sizeof(vector3_t));
    kern_return_t kr;
    kr = vm_read(task, address, sizeof(vector3_t), (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        __data = *(vector3_t*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, sizeof(vector3_t));
    }
    return __data;
}

rbx_cframe_t vm_read_rbx_cframe_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    rbx_cframe_t __data;
    bzero(&__data, sizeof(rbx_cframe_t));
    kern_return_t kr;
    kr = vm_read(task, address, sizeof(rbx_cframe_t), (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        __data = *(rbx_cframe_t*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, sizeof(rbx_cframe_t));
    }
    return __data;
}


#include "Objects/instance.h"
#include "Objects/basepart.h"
#include "Objects/camera.h"
#include "Objects/player.h"
#include "Objects/values.h"
#include "Objects/tool.h"
#include "Objects/humanoid.h"
#include "Objects/team.h"
#include "Objects/meshpart.h"
#include "Objects/lighting.h"
#include "Objects/textlabel.h"



///Get-Dylib-Symbol-Offset
vm_address_t gdso(void* dlopen_handle,
                  vm_address_t load_address,
                  char* symbol)
{
    vm_offset_t offset;
    void* address = dlsym(dlopen_handle, symbol);
    offset = ((vm_address_t)address) - load_address;
    return offset;
}



ESP_Frame rbx_draw_esp_box(task_t task,
                           vector3_t pos,
                           rbx_cframe_t camera_cframe,
                           vm_address_t esp_box_hidden_array,
                           vm_address_t esp_box_frame_array,
                           vm_address_t esp_box_color_array,
                           vm_address_t esp_box_border_width_array,
                           float border_width,
                           ESP_Color color,
                           float fov,
                           float object_width,
                           float object_height,
                           float x_offset,
                           float y_offset,
                           float window_w,
                           float window_h,
                           int index,
                           char should_scale_size)
{
    ESP_Frame frame;
    bzero(&frame, sizeof(frame));
    custom_rbx_world_to_screen_info_t wtsi;
    wtsi = rbx_world_to_screen_point(camera_cframe, fov, pos, window_w, window_h, object_width, object_height);
    char hidden = true;
    if (isnan(wtsi.x) || isnan(wtsi.y))
    {
        hidden = true;
        wtsi.x = 0;
        wtsi.y = 0;
        vm_write(task, esp_box_hidden_array + index, (vm_offset_t)&hidden, 1);
    }
    else
    {
        if (isfinite(wtsi.x) && isfinite(wtsi.y))
        {
            
            hidden = false;
            vm_write(task, esp_box_hidden_array + index, (vm_offset_t)&hidden, 1);
            
            vm_offset_t f_o = (index * sizeof(ESP_Frame));
            vm_offset_t c_o = (index * sizeof(ESP_Color));
            
            float w = object_width;
            float h = object_height;
            
            if (should_scale_size == true)
            {
                w = wtsi.width;
                h = wtsi.height;
            }
            
            frame.x = (wtsi.x - w/2) + x_offset;
            frame.y = (wtsi.y - h/2) + y_offset;
            frame.w = w;
            frame.h = h;
            
            vm_write(task, esp_box_frame_array + f_o, (vm_address_t)&frame, sizeof(ESP_Frame));
            vm_write(task, esp_box_color_array + c_o, (vm_address_t)&color, sizeof(ESP_Color));
            vm_write(task, esp_box_border_width_array + (index * sizeof(float)), (vm_address_t)&border_width, sizeof(float));
        }
    }
    return frame;
}


void rbx_launch_game(long place_id, char* cookie_file_path)
{
    static int buffer_size = 99999;
    char cmd[buffer_size];
    char* auth_ticket_cmd = "echo $(curl -i 'https://auth.roblox.com/v1/authentication-ticket/' -X 'POST' -H 'authority: auth.roblox.com' -H 'accept: */*' -H 'accept-language: en-US,en;q=0.8' -H 'content-length: 0' -H 'content-type: application/json' -H 'cookie: %s' -H 'origin: https://www.roblox.com' -H 'referer: https://www.roblox.com/' -H 'sec-ch-ua: \"Brave\";v=\"111\", \"Not(A:Brand\";v=\"8\", \"Chromium\";v=\"111\"' -H 'sec-ch-ua-mobile: ?0' -H 'sec-ch-ua-platform: \"macOS\"' -H 'sec-fetch-dest: empty' -H 'sec-fetch-mode: cors' -H 'sec-fetch-site: same-site' -H 'sec-gpc: 1' -H 'user-agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36' -H 'x-csrf-token: %s')";
    char* place_launch_cmd = "DYLD_INSERT_LIBRARIES=%s /Applications/Roblox.app/Contents/MacOS/RobloxPlayer -ticket %s -launchtime 1679869627256 -scriptURL \"https://assetgame.roblox.com/game/PlaceLauncher.ashx?request=RequestGame&browserTrackerId=142472346421&placeId=%ld&isPlayTogetherGame=false&joinAttemptId=0f907d75-ec16-4d3b-8d80-3f02d0cafc4b&joinAttemptOrigin=PlayButton\" -browserTrackerId 142472346421 -rloc en_us -gloc en_us -launchExp InApp";
    
    char cookie[buffer_size];
    struct stat file_stat;
    stat(cookie_file_path, &file_stat);
    FILE* fp = fopen(cookie_file_path, "r");
    fgets(cookie, (int)file_stat.st_size, fp);
    fclose(fp);
    
    sprintf(cmd, auth_ticket_cmd, cookie, "");
    
    printf("%s\n", cmd);
    
    char* csrf_token = NULL;
    char* auth_ticket = NULL;
    
    FILE* pipe = NULL;
    char response[buffer_size];
    
    char* csrf_token_start = NULL;
    while (!csrf_token_start)
    {
        pipe = popen(cmd, "r");
        fgets(response, buffer_size, pipe);
        pclose(pipe);
        csrf_token_start = strstr(response, "x-csrf-token: ");
    }
    
    if (csrf_token_start)
    {
        csrf_token_start += strlen("x-csrf-token: ");
        char* csrf_token_end = strchr(csrf_token_start, 0xd);
        long csrf_token_length = (csrf_token_end - csrf_token_start);
        csrf_token = malloc(csrf_token_length);
        memcpy(csrf_token, csrf_token_start, csrf_token_length);
        
        printf("CSRF-TOKEN: [%s]\n", csrf_token);
        bzero(cmd, sizeof(cmd));
        bzero(response, sizeof(response));
        sprintf(cmd, auth_ticket_cmd, cookie, csrf_token);
        
        char* auth_ticket_start = NULL;
        
        while (!auth_ticket_start)
        {
            pipe = popen(cmd, "r");
            fgets(response, buffer_size, pipe);
            pclose(pipe);
            auth_ticket_start = strstr(response, "rbx-authentication-ticket: ");
        }
        if (auth_ticket_start)
        {
            auth_ticket_start += strlen("rbx-authentication-ticket: ");
            char* auth_ticket_end = strchr(auth_ticket_start, 0xd);
            long auth_ticket_length = (auth_ticket_end - auth_ticket_start);
            auth_ticket = malloc(auth_ticket_length);
            memcpy(auth_ticket, auth_ticket_start, auth_ticket_length);
            printf("RBX-AUTH-TICKET: [%s]\n", auth_ticket);
            bzero(cmd, sizeof(cmd));
            sprintf(cmd, place_launch_cmd, __INJECTED_DYLIB_PATH__, auth_ticket, place_id);
            free(csrf_token);
            free(auth_ticket);
            system(cmd);
        }
    }
}


#include "Games/Those-Who-Remain.h"
#include "Games/Flood-Escape-Classic.h"
#include "Games/Phantom-Forces.h"
#include "Games/Hack-Tests.h"
#include "Games/Field-Of-Battle.h"
#include "Games/Doors.h"
#include "Games/Arsenal.h"
#include "Games/Blox-Fruits.h"
#include "Games/Jailbreak.h"
#include "Games/Emergency-Response.h"


int main(int argc, char** argv)
{
    kern_return_t kr;
    printf("RobloxCheats pid: %d\n", getpid());
    task_t task;
    pid_t pid_array[4096];
    int process_count = pids_by_name("RobloxPlayer", pid_array);
    pid_t pid = pid_array[0]; //The most recent RobloxPlayer process.
    printf("RobloxPlayer pid: %d\n", pid);
    printf("If using in Xcode, make sure both libESP.dylib and RobloxCheats are building for Intel\n");
    kr = task_for_pid(mach_task_self_, pid, &task);
    if (kr != KERN_SUCCESS)
    {
        printf("%s%d\n", "failed to obtain task port for pid: ", pid);
        printf("%s\n", mach_error_string(kr));
    }

    //We have to run the Blox Fruits Chest Farm from the Command Line, since whenever we launch Roblox from this process, Roblox becomes a child process.
    if (argc == 2)
    {
        if (strcmp(argv[1], "blox_fruits_chest_farm") == 0)
        {
            if (process_count > 0)
            {
                blox_fruits_chest_collect(task);
                sleep(10);
                exit(0);
            }
            else
            {
                dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                {
                    sleep(20);
                    exit(0);
                });
                rbx_launch_game(2753915549, __COOKIE_TXT_PATH__); //blox fruits
            }
        }
    }
    
    
    //rbx_launch_game(292439477); //PF

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        //printf("%ld\n", blox_fruits_get_pure_enemy_name_length("Shanda [Lv. 475]", strlen("Shanda [Lv. 475]")));
        /*vector3_t pos = {.x = -65, .y = 130, .z = -25};
        vm_address_t game = rbx_find_game_address(task);
        vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
        vm_address_t players = rbx_instance_find_first_child_of_class(task, game, "Players");
        vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
        vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players, "Player");
        vm_address_t map_model = rbx_instance_find_first_child(task, workspace, "Map");
        vm_address_t enemies_folder = rbx_instance_find_first_child(task, workspace, "Enemies");
        vm_address_t npcs_folder = rbx_instance_find_first_child(task, workspace, "NPCs");
        vm_address_t playergui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
        vm_address_t maingui = rbx_instance_find_first_child(task, playergui, "Main");
        vm_address_t questframe = rbx_instance_find_first_child(task, maingui, "Quest");
        vm_address_t questframe_container = rbx_instance_find_first_child(task, questframe, "Container");
        vm_address_t questtitle_frame = rbx_instance_find_first_child(task, questframe_container, "QuestTitle");
        vm_address_t quest_title_textlabel = rbx_instance_find_first_child(task, questtitle_frame, "Title");
        long quest_string_length = 0;
        char* quest_string = rbx_textlabel_get_text(task, quest_title_textlabel, &quest_string_length);
        printf("%s\n", quest_string);
        rbx_print_descendants(task, blox_fruits_find_quest_giver(task, npcs_folder), 0, 0);
        rbx_print_descendants(task, blox_fruits_find_first_alive_enemy(task, enemies_folder, quest_string, quest_string_length), 0, 0);*/
        //rbx_print_descendants(task, questtitle_frame, 0, 2);
        //printf("%s\n", rbx_textlabel_get_text(task, quest_title_textlabel, malloc(8)));
        //blox_fruits_find_first_alive_enemy(task, enemies_folder, "", 1);
        //printf("%p\n", printf);
        //fly_test(task);
        //blox_fruits_find_first_alive_enemy_of_lowest_level(task, enemies_folder, "");
        //rbx_instance_for_each_descendant(task, workspace, bf_func);
        
        /*
         [Model] Bandit Quest Giver (0x7f812dc95a90) (child count: 23)
         */
        //rbx_print_descendants(task, blox_fruits_find_quest_giver(task, npcs_folder), 0, 0);
        //printf("%p\n", rbx_find_game_address(task));
        //blox_fruits_chest_collect(task);
        //blox_fruits_auto_farm(task);
        //arsenal_cheat(task);
        doors_cheat(task);
        //flood_escape_classic_cheat(task);
        //phantom_forces_cheat(task);
        //find_object_offsets(task, "nezukoaooo64");
        //fly_test(task);
        //field_of_battle_collect_legendary_gem(task);
        //field_of_battle_auto_farm(task);
        //jailbreak_cheat(task);
        //emergency_response_cheat(task);
    });
    
    for (;;)
    {
        sleep(5);
    }
    
    return 0;
}
