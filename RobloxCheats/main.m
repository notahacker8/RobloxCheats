//
//  main.m
//  RobloxCheats
//
//  Created by Dimitri Armendariz on 9/10/22.
//

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
#include <mach-o/dyld_images.h>
#include <mach/arm/_structs.h>
#include <dlfcn.h>
#include <pthread.h>

#define __INJECTED_DYLIB__ "/Users/dimitriarmendariz/Library/Developer/Xcode/DerivedData/ESP-drqlefpymzgresdhcnxyusawdjhf/Build/Products/Debug/libESP.dylib"


#define PI 3.1415926535
#define _KR_     if (kr != KERN_SUCCESS) { return NULL;}


#define ARM_THREAD_STATE64        6

#define ARM_THREAD_STATE_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_thread_state_t)/sizeof(uint32_t)))
#define ARM_THREAD_STATE32_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_thread_state32_t)/sizeof(uint32_t)))
#define ARM_THREAD_STATE64_COUNT ((mach_msg_type_number_t) \
   (sizeof (arm_thread_state64_t)/sizeof(uint32_t)))

///Check if a module is loaded into the memory of a process.
vm_address_t get_image_address(const task_t task,
                               const char* image_path)
{
    vm_address_t image_address = 0;
    kern_return_t kr;
    mach_msg_type_number_t size = 0;
    static mach_msg_type_number_t data_cnt = 0;
    
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
        vm_read(task, img_file_path_ptr, PATH_MAX, (vm_address_t*)&img_file_path, &data_cnt);
        if (strcmp(image_path, img_file_path) == 0)
        {
            image_address = img_ld_address;
        }
    }
    return image_address;
}




pid_t pid_by_name(char* name)
{
    pid_t pids[4096];
    int retpid = -1;
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
                retpid = pids[i];
            }
        }
    }
    return retpid;
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
            //do stuff
            a[0][i] = address;
            a[1][i] = address + size;
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
    static mach_msg_type_number_t data_cnt;
    
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



#include "Objects/offsets.h"
#include "Objects/vector3.h"
#include "Objects/ESP.h"
#include "Objects/cframe.h"
#include "Objects/instance.h"
#include "Objects/basepart.h"
#include "Objects/camera.h"
#include "Objects/player.h"
#include "Objects/values.h"
#include "Objects/textlabel.h"



///Get Dylib Symbol Offset
vm_address_t gdso(void* dlopen_handle,
                  vm_address_t load_address,
                  char* symbol)
{
    vm_offset_t offset;
    void* address = dlsym(dlopen_handle, symbol);
    offset = ((vm_address_t)address) - load_address;
    return offset;
}




#include "Games/Those-Who-Remain.h"
#include "Games/Flood-Escape-Classic.h"
#include "Games/Phantom-Forces.h"
#include "Games/Tower-Defense-Simulator/Structs.h"
#include "Games/Tower-Defense-Simulator/Functions.h"
#include "Games/Tower-Defense-Simulator/Input-Stacks.h"
#include "Games/Tower-Defense-Simulator/Main.h"
#include "Games/Hack-Tests.h"




int fmain(int argc, const char* argv[]) {
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    
    task_t task;
    pid_t pid = pid_by_name("RobloxPlayer");
    task_for_pid(mach_task_self_, pid, &task);
    void* handle = dlopen(__INJECTED_DYLIB__, RTLD_NOW);
    
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    /*
    vm_address_t character = rbx_instance_find_first_child(task, workspace, "blockmincer");
    vm_address_t humanoid = rbx_instance_find_first_child(task, character, "Humanoid");
    printf("%p %p\n", camera, humanoid);
     */
    
    vm_address_t ignore_folder = rbx_instance_find_first_child(task, workspace, "Ignore");
    vm_address_t ref_player = rbx_instance_find_first_child(task, ignore_folder, "RefPlayer");
    
    
    vm_address_t players_folder = rbx_instance_find_first_child(task, workspace, "Players");
    vm_address_t blue_team_folder = rbx_instance_find_first_child(task, players_folder, "Bright blue");
    vm_address_t orange_team_folder = rbx_instance_find_first_child(task, players_folder, "Bright orange");
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t plr = rbx_instance_get_children(task, players_service, malloc(8))[0];
    printf("%p\n", plr);
    rbx_print_children_profiles(task, 0x7fc530475d50);
    /*
    long cc;
    vm_address_t* chars = rbx_instance_get_children(task, blue_team_folder, &cc);
    for (long i = 0 ; i < cc ; i++)
    {
        rbx_print_children_profiles(task, chars[i]);
    }
     */
    //rbx_print_children_profiles(task, 0x7fb69911d890);
    //rbx_cframe_t cf = rbx_basepart_get_cframe(task, hrp);
    //cf.pos.y += 3.0f;
    //rbx_basepart_set_cframe(task, hrp, &cf);
    
    //printf("%f\n", rbx_basepart_get_gravity(task, hrp));
    
    //rbx_camera_set_field_of_view(task, camera, 90);
   // printf("%f %f\n", wtsi.x, wtsi.y);
    //printf("%f %f %f\n", xc.pos.x, xc.pos.y, xc.pos.z);
    
    
    

    
    //0x388 (speed)
    //bzero(&cf.pos, sizeof(vector3_t));
    //printf("%f, %f, %f\n", cf.pos.x, cf.pos.y, cf.pos.z);
    //cf.pos.x = 0;
    //cf.pos.y = 1000;
    //cf.pos.z = 0;
    //rbx_basepart_set_cframe(task, hrp, &cf);
    //rbx_instance_t* workspace = rbx_find_first_child(task, game, "Workspace");
    /*
    rbx_instance_t* ignore_folder = rbx_find_first_child(task, workspace, "Ignore");
    rbx_instance_t* items_folder = rbx_find_first_child(task, ignore_folder, "Items");
    for (;;)
    {
    rbx_instance_t* item = rbx_find_first_child(task, items_folder, "Barbed Wire");
    rbx_instance_t* box = rbx_find_first_child(task, item, "Box");
    rbx_instance_t* character = rbx_find_first_child(task, workspace, "TowerOfHell_IsStale");
    rbx_instance_t* hrp = rbx_find_first_child(task, character, "HumanoidRootPart");
     
    //rbx_print_children_profiles(task, hrp);
    
    if (box && hrp)
    {
    rbx_base_part_t* box_;
    vm_read(task, (vm_address_t)box -> instance_self, sizeof(rbx_base_part_t), (vm_address_t*)&box_, &data_cnt);
    rbx_base_part_properties_t* box_properties;
    vm_read(task, (vm_address_t)(box_ -> properties), sizeof(rbx_base_part_properties_t), (vm_address_t*)&box_properties, &data_cnt);
    
    
    rbx_base_part_t* hrp_;
    vm_read(task, (vm_address_t)hrp -> instance_self, sizeof(rbx_base_part_t), (vm_address_t*)&hrp_, &data_cnt);
    rbx_base_part_properties_t* hrp_properties;
    vm_read(task, (vm_address_t)(hrp_ -> properties), sizeof(rbx_base_part_properties_t), (vm_address_t*)&hrp_properties, &data_cnt);
    float v = 0.0f;
    //vm_write(task, (vm_address_t)hrp_ -> properties + ((void*)&(hrp_properties -> gravity) - (void*)hrp_properties), (vm_address_t)&v, 4);
    vector3_t old_pos = hrp_properties -> cframe0.pos;
    vector3_t new_pos = box_properties -> cframe0.pos;
    vm_write(task, (vm_address_t)hrp_ -> properties + ((void*)&(hrp_properties -> cframe0.pos) - (void*)hrp_properties), (vm_address_t)&new_pos, 12);
    sleep(2);
    vm_write(task, (vm_address_t)hrp_ -> properties + ((void*)&(hrp_properties -> cframe0.pos) - (void*)hrp_properties), (vm_address_t)&old_pos, 12);
    }
    sleep(10);
    }
    return 0;
     
    
    rbx_instance_t* i = rbx_get_instance(task, 0x7facceb79e50);
    rbx_print_children_profiles(task, i);*/
    return 0;
}



/*
thread_act_array_t threads;
mach_msg_type_number_t thread_count;
task_threads(task, &threads, &thread_count);
for (int i = 0 ; i < thread_count ; i++)
{
    thread_act_t thread = threads[i];
    arm_thread_state64_t regs;
    kern_return_t kr;
    bzero(&regs, sizeof(regs));
    mach_msg_type_number_t sc = ARM_THREAD_STATE64_COUNT;e
    mach_msg_type_number_t state = ARM_THREAD_STATE64;
    kr = thread_get_state(thread, state, (thread_state_t)(&regs), &sc);
    printf("%d, %p\n", kr, regs.__pc);
}
*/

/*
 
 
 CLICK ON TOWER: 441.832031, 334.234375
 UPGRADE :501.562500, 381.363281
 [CHANGE]: 60, 47
 
 CLICK ON TOWER: 347.226562, 321.566406
 CHANGE TARGET: 252.082031, 369.636719
 [CHANGE]: -95, 48
 
 CLICK ON TOWER: 433.097656, 336.851562
 CLOSE POP-UP: 550.199219, 524.800781
 [CHANGE]: 117, 188
 
 CLICK ON LOBBY RETURN: 406.695312, 177.250000
 
 */



int main(int argc, char** argv)
{
    static mach_msg_type_number_t data_cnt;
    static vm_address_t read_data;
    printf("this pid: %d\n", getpid());
    task_t task;
    pid_t pid = pid_by_name("RobloxPlayer");
    task_for_pid(mach_task_self_, pid, &task);
    /*
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t towers_folder = rbx_instance_find_first_child(task, workspace, "Towers");
    long tower_count;
    vm_address_t* towers = rbx_instance_get_children(task, towers_folder, &tower_count);
    for (int i = 0 ; i < tower_count ; i++)
    {
        vm_address_t tower = towers[i];
        vm_address_t tower_hrp = rbx_instance_find_first_child(task, tower, "HumanoidRootPart");
        rbx_cframe_t cf = rbx_basepart_get_cframe(task, tower_hrp);
        printf("%f, %f, %f\n", cf.pos.x, cf.pos.y, cf.pos.z);
        vm_address_t upgrades_folder = rbx_instance_find_first_child(task, tower, "Upgrades");
        //rbx_print_children_profiles(task, tower);
        //rbx_print_children_profiles(task, 0x7fb0ed3d8480);
    }
     */
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
    //those_who_remain_cheat(task);
    //flood_escape_class_cheat(task);
    phantom_forces_cheat(task);
    //tower_defense_simulator_cheat(task); //(incomplete)
    //find_object_offsets(task);
    });
    sleep(1000000);
    return 0;
}
