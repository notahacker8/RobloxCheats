//
//  main.m
//  RobloxCheats
//
//  Created by Dimitri Armendariz on 9/10/22.
//

typedef struct
{
    char r;
    char g;
    char b;
}
rbx_rgb_t;

typedef union
{
    int i;
    float f;
}
int_float_u;

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

#define MAX_ESP_COUNT 400


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

void is_input_queue_finished(task_t task, vm_address_t address, char* inout_bool)
{
    static mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    kern_return_t kr = vm_read(task, address, 1, &read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        char f = *(char*)read_data;
        *inout_bool = f;
        vm_deallocate(mach_task_self_, read_data, 1);
    }
}

long vm_read_8byte_value(task_t task, vm_address_t address)
{
    if (address == 0) { return 0; }
    static mach_msg_type_number_t data_cnt;
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

int vm_read_4byte_value(task_t task, vm_address_t address)
{
    if (address == 0) { return 0; }
    static mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    int __data = 0;
    kern_return_t kr;
    kr = vm_read(task, address, 4, (vm_offset_t*)&read_data, &data_cnt);
    if (kr == KERN_SUCCESS)
    {
        __data = *(int*)read_data;
        vm_deallocate(mach_task_self_, (vm_address_t)read_data, 4);
    }
    return __data;
}

char vm_read_1byte_value(task_t task, vm_address_t address)
{
    if (address == 0) { return 0; }
    static mach_msg_type_number_t data_cnt;
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



ESP_Frame rbx_draw_esp_box(task_t task,
                           vector3_t pos,
                           rbx_cframe_t camera_cframe,
                           vm_address_t esp_box_hidden_array,
                           vm_address_t esp_box_frame_array,
                           vm_address_t esp_box_color_array,
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
        }
    }
    return frame;
}


#include "Games/Those-Who-Remain.h"
#include "Games/Flood-Escape-Classic.h"
#include "Games/Phantom-Forces.h"
#include "Games/Tower-Defense-Simulator/Structs.h"
#include "Games/Tower-Defense-Simulator/Functions.h"
#include "Games/Tower-Defense-Simulator/Input-Stacks.h"
#include "Games/Tower-Defense-Simulator/Main.h"
#include "Games/Hack-Tests.h"
#include "Games/Field-Of-Battle.h"
#include "Games/Doors.h"
#include "Games/Blox-Fruits/Main.h"
#include "Games/Arsenal.h"




int main(int argc, char** argv)
{
    kern_return_t kr;
    printf("this pid: %d\n", getpid());
    task_t task;
    pid_t pid = pid_by_name("RobloxPlayer");
    printf("roblox pid: %d\n", pid);
    kr = task_for_pid(mach_task_self_, pid, &task);
    if (kr != KERN_SUCCESS)
    {
        printf("%s%d\n", "failed to obtain task port for pid: ", pid);
    }
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
    //blox_fruits_cheat(task);
       // arsenal_cheat(task);
    //doors_test(task);
    //doors_cheat(task);
    //those_who_remain_cheat(task); // (incomplete)
    //flood_escape_classic_cheat(task);
    phantom_forces_cheat(task);
    //tower_defense_simulator_cheat(task); //(incomplete)
        //find_object_offsets(task);
    //field_of_battle_cheat(task);
        
    });
    for (;;)
    {
        sleep(1);
    }
    return 0;
}
