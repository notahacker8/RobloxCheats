//
//  main.m
//  RobloxCheats
//
//  Created by me on 9/10/22.
//

/*
 @TODO: Notes:
 
 The rosetta version is basically useless, since Roblox has been released for native Apple Silicon.
 
 */

//RGB color struct
typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
}
rbx_rgb_t;



#import <Foundation/Foundation.h>
#include <ApplicationServices/ApplicationServices.h>
#import <AVFoundation/AVFoundation.h>
#import <AVFAudio/AVFAudio.h>
#import <Cocoa/Cocoa.h>
#import <Speech/Speech.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <libproc.h>
#include <mach/mach.h>
#include <mach-o/loader.h>
#include <mach/mach_vm.h>
#include <mach/arm64/asm.h>
#include <mach-o/dyld.h>
#include <mach/arm/thread_state.h>
#include <mach/arm/thread_status.h>
#include <mach/thread_state.h>
#include <mach/thread_status.h>
#include <mach/mach_init.h>
#include <mach/thread_policy.h>
#include <mach-o/dyld_images.h>
#include <mach/vm_region.h>
#include <mach/arm/_structs.h>
#include <mach-o/nlist.h>
#include <math.h>
#include <tgmath.h>
#include <dlfcn.h>
#include <math.h>
#include <pthread.h>
#include <pthread/sched.h>
#include <sys/ptrace.h>

#include "Jarvis/Talk.h"
#import "Jarvis/Recognition.m"

#define JARVIS

#define ARM_THREAD_STATE64 6
typedef struct
{
    __uint64_t __x[29]; /* General purpose registers x0-x28 */
    __uint64_t __fp;    /* Frame pointer x29 */
    __uint64_t __lr;    /* Link register x30 */
    __uint64_t __sp;    /* Stack pointer x31 */
    __uint64_t __pc;    /* Program counter */
    __uint32_t __cpsr;  /* Current program status register */
    __uint32_t __pad;   /* Same size for 32-bit or 64-bit clients */
}
__arm_thread_state64_t;
#define ARM_THREAD_STATE64_COUNT ((mach_msg_type_number_t) \
    (sizeof (__arm_thread_state64_t)/sizeof(uint32_t)))

#define PI 3.1415926535
#define FREEABLE //When the returned variable needs to be free()'ed
#define VM_DEALLOCABLE //When the returned variable needs to be vm_deallocate()'ed

/*
 Stuff for when we need to read a volatile variable that may change at any time because it is being written to by another thread
 */

#define COPY_BUF(var)\
typeof(var) __##var; memcpy(&__##var, &var, sizeof(var));
#define COPY_VAR(var)\
typeof(var) __##var = var;

//#define ROBLOX_INTEL_VERSION //Intel, native
//#define ROBLOX_ROSETTA_VERSION //Intel, but on apple silicon
#define ROBLOX_APPLE_VERSION //Apple silicon, native

#define __BRK_0__ "\x00\x00\x20\xd4"


#include "Objects/brickcolors.h"
#include "Objects/vector3.h"
#include "Objects/cframe.h"

#include "Memory.h"
#include "Macros.h"
#include "ESP.h"
#include "Code-Snippets.h"
#include "Offsets.h"

/*
 Change if you are NOT me.
 */


char* __ROBLOXPLAYER_PATH__ = "/Applications/Roblox.app/Contents/MacOS/RobloxPlayer";
char* __LIBESP_DYLIB_PATH__ = "/Users/dimitriarmendariz/Library/Developer/Xcode/DerivedData/ESP-drqlefpymzgresdhcnxyusawdjhf/Build/Products/Debug/libESP.dylib";
char* __COOKIE_TXT_PATH__ = "/Users/dimitriarmendariz/Documents/roblox-cookie.txt";

char* __ROBLOX_PIPES_FOLDER_PATH__ = "/Users/dimitriarmendariz/Documents/RobloxPipes";
char __ROBLOX_PIPE_TXT_PATH__[PATH_MAX];




FREEABLE char* read_file(char* path, long* inout_length)
{
    char* bytes = NULL;
    struct stat buf;
    if (stat(path, &buf) == 0)
    {
        long file_size = buf.st_size;
        if (inout_length)
        {
            *inout_length = file_size;
        }
        FILE* fp = fopen(path, "r");
        if (fp)
        {
            bytes = malloc(file_size + 1);
            bzero(bytes, file_size + 1);
            fread(bytes, file_size, 1, fp);
            fclose(fp);
        }
        else
        {
            fprintf(stderr, "failed to open file: '%s'\n", path);
        }
    }
    else
    {
        fprintf(stderr, "failed to get info about file: '%s'\n", path);
    }
    return bytes;
}

void write_to_file(char* path, char* data, long data_length)
{
    FILE* fp = fopen(path, "w");
    if (fp)
    {
        fwrite(data, data_length, 1, fp);
        fclose(fp);
    }
    else
    {
        fprintf(stderr, "failed to open file: '%s'\n", path);
    }
}


///File Get-Dylib-Symbol-Offset
vm_offset_t file_gdso(char* file_path, char* symbol_name)
{
    vm_offset_t offset = 0;
    
    char* file_data = read_file(file_path, NULL);
    if (file_data)
    {
        struct mach_header_64* header = (struct mach_header_64*)file_data;
        char* current_cmd = file_data + sizeof(struct mach_header_64);
        for (int i = 0 ; i < (header -> ncmds) ; i++)
        {
            struct load_command* ld_cmd = (struct load_command*)current_cmd;
            if (ld_cmd -> cmd == LC_SYMTAB)
            {
                struct symtab_command* st_cmd = (struct symtab_command*)ld_cmd;
                for (int _i = 0 ; _i < st_cmd -> nsyms ; _i++)
                {
                    struct nlist_64* nl64 = (struct nlist_64*)(file_data + st_cmd -> symoff + (_i * sizeof(struct nlist_64)));
                    //printf("%s\n", file_data + st_cmd -> stroff + nl64 ->n_un.n_strx );
                    if (strcmp(file_data + st_cmd -> stroff + nl64 ->n_un.n_strx + 1, symbol_name) == 0)
                    {
                        offset = nl64 -> n_value;
                    }
                }
            }
            current_cmd += ld_cmd -> cmdsize;
        }
        free(file_data);
    }
    return offset;
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
                if (inout_array)
                {
                    inout_array[pid_index] = pids[i];
                }
                pid_index++;
            }
        }
    }
    return pid_index;
}

FREEABLE char* name_by_pid(int pid)
{
    char* name = NULL;
    pid_t pids[4096];
    int count = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    int proc_count = count/sizeof(pid_t);
    for (int i = 0; i < proc_count; i++)
    {
        struct proc_bsdinfo proc;
        int st = proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0, &proc, PROC_PIDTBSDINFO_SIZE);
        if (st == PROC_PIDTBSDINFO_SIZE)
        {
            if (pids[i] == pid)
            {
                name = malloc(32);
                memcpy(name, proc.pbi_name, 32);
            }
        }
    }
    return name;
}

task_t get_roblox_task_port(int* out_pid)
{
    char* file_path_format = "%s/roblox-pipe-%d.txt";
    
    task_t task;
    static pid_t pid_array[4096];
    memset(pid_array, -1, sizeof(pid_array));
    int pid_count = pids_by_name("RobloxPlayer", pid_array);
    pid_t pid = -1;
    int pid_idx = 0;
    if (pid_count > 1)
    {
        printf("Multiple RobloxPlayer PID's found.\n");
        printf("Type index to continue.\n");
        for (int i = 0 ; i < pid_count ; i++)
        {
            printf("\t[%d]: %d\n", i, pid_array[i]);
        }
        scanf("%d", &pid_idx);
    }
    else
    {
        pid_idx = 0;
    }
    
    pid = pid_array[pid_idx];
    *out_pid = pid;
    
    kern_return_t kr = task_for_pid(mach_task_self_, pid, &task);
    if (kr != KERN_SUCCESS)
    {
        printf("%s%d\n", "failed to obtain task port for pid: ", pid);
        printf("%s\n", mach_error_string(kr));
        printf("%s\n", "(Do you have SIP disabled?)");
    }
    else
    {
        bzero(__ROBLOX_PIPE_TXT_PATH__, PATH_MAX);
        sprintf(__ROBLOX_PIPE_TXT_PATH__, file_path_format, __ROBLOX_PIPES_FOLDER_PATH__, pid);
        struct stat buf;
        if (stat(__ROBLOX_PIPE_TXT_PATH__, &buf) != 0)
        {
            FILE* fp = fopen(__ROBLOX_PIPE_TXT_PATH__, "a");
            if (fp)
            {
                fclose(fp);
                printf("successfully created file: %s\n", __ROBLOX_PIPE_TXT_PATH__);
            }
            else
            {
                printf("error creating file: %s\n", __ROBLOX_PIPE_TXT_PATH__);
            }
        }
        else
        {
            printf("using existing file: %s\n", __ROBLOX_PIPE_TXT_PATH__);
        }
    }
    
    NSArray* dircontents = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:[NSString stringWithCString:__ROBLOX_PIPES_FOLDER_PATH__] error:NULL];
    [dircontents enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        NSString *filename = (NSString *)obj;
        int extracted_pid = -1;
        if (sscanf(filename.UTF8String, "roblox-pipe-%d.txt", &extracted_pid))
        {
            char is_valid = false;
            for (int i = 0 ; i < pid_count ; i++)
            {
                if (pid_array[i] == extracted_pid)
                {
                    is_valid = true;
                }
            }
            if (!is_valid)
            {
                printf("DELETING FILE: %s/%s\n", __ROBLOX_PIPES_FOLDER_PATH__, filename.UTF8String);
                char cmd[PATH_MAX];
                bzero(cmd, PATH_MAX);
                sprintf(cmd, "rm %s/%s", __ROBLOX_PIPES_FOLDER_PATH__, filename.UTF8String);
                system(cmd);
            }
        }
    }];
    
    return task;
}


int keycode_for_character(char c)
{
    static int keycodes[255];
    static char has_been_called = false;
    if (!has_been_called)
    {
        has_been_called = true;
        keycodes['a'] = 0;
        keycodes['b'] = 11;
        keycodes['c'] = 8;
        keycodes['d'] = 2;
        keycodes['e'] = 14;
        keycodes['f'] = 3;
        keycodes['g'] = 5;
        keycodes['h'] = 4;
        keycodes['i'] = 34;
        keycodes['j'] = 38;
        keycodes['k'] = 40;
        keycodes['l'] = 37;
        keycodes['m'] = 46;
        keycodes['n'] = 45;
        keycodes['o'] = 31;
        keycodes['p'] = 35;
        keycodes['q'] = 12;
        keycodes['r'] = 15;
        keycodes['s'] = 1;
        keycodes['t'] = 17;
        keycodes['u'] = 32;
        keycodes['v'] = 9;
        keycodes['w'] = 13;
        keycodes['x'] = 7;
        keycodes['y'] = 16;
        keycodes['z'] = 6;
        keycodes['0'] = 29;
        keycodes['1'] = 18;
        keycodes['2'] = 19;
        keycodes['3'] = 20;
        keycodes['4'] = 21;
        keycodes['5'] = 23;
        keycodes['6'] = 22;
        keycodes['7'] = 26;
        keycodes['8'] = 28;
        keycodes['9'] = 25;
    }
    return keycodes[tolower(c)];
}

/*
 Disables the checks the game has that prevent us from editing the executable memory.
 ONLY RUN WHEN ROBLOX UPDATES

 */
void patch_roblox_file(void)
{
#ifdef ROBLOX_APPLE_VERSION
    long file_bytes_length = 0;
    char* file_bytes = read_file(__ROBLOXPLAYER_PATH__, &file_bytes_length);
    if (file_bytes)
    {
        //Instructions of the function that check if we manipulate the __TEXT segment.
        //Use BitSlicer, check read accesses to a random address of the __TEXT segment (eg. base address)
        //Not needed for the Rosetta version of Roblox
        char RET[4] = {"\xc0\x03\x5f\xd6"};
        char bytes_to_find[8] = {"\x84\x16\xc1\x6c\x80\x94\xa2\x0e"};
        /*
         base() + 0x2B2B788    ldp d4, d5, [x20], #0x10    84 16 C1 6C
         base() + 0x2B2B78C    mla v0.2s, v4.2s, v2.2s    80 94 A2 0E
         */
        char* index1 = memmem(file_bytes, file_bytes_length, bytes_to_find, sizeof(bytes_to_find));
        for (long i = (long)(index1 - file_bytes) ; i-- > 0 ; )
        {
            if (memcmp(file_bytes + i, RET, sizeof(RET)) == 0)
            {
                if (memcmp(file_bytes + i - 4, RET, sizeof(RET)) == 0)
                {
                    printf("%s %s\n", "file already patched at", __ROBLOXPLAYER_PATH__);
                }
                else
                {
                    memcpy(file_bytes + i + 4, RET, sizeof(RET));
                    write_to_file(__ROBLOXPLAYER_PATH__, file_bytes, file_bytes_length);
                    printf("%s %s\n", "successfully patched file at", __ROBLOXPLAYER_PATH__);
                }
                i = 0;
            }
        }
        free(file_bytes);
    }
#endif
}



#include "Objects/instance.h"
#include "Objects/basepart.h"
#include "Objects/camera.h"
#include "Objects/player.h"
#include "Objects/values.h"
#include "Objects/tool.h"
#include "Objects/humanoid.h"
#include "Objects/team.h"
#include "Objects/lighting.h"
#include "Objects/gui.h"
#include "Objects/datamodel.h"

#include "Objects/<<<ROOT>>>.h"
#include "Objects/functions.h"








ESP_Frame rbx_draw_esp_box_on_3d_point(task_t task,
                                       vector3_t pos,
                                       rbx_cframe_t camera_cframe,
                                       libESPSharedMemoryObject* shared_memory,
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
    if (isnan(wtsi.x) || isnan(wtsi.y) || !isfinite(wtsi.x) || !isfinite(wtsi.y))
    {
        wtsi.x = 0;
        wtsi.y = 0;
        shared_memory->ESP_BOX_HIDDEN_ARRAY[index] = true;
    }
    else
    {
        shared_memory->ESP_BOX_HIDDEN_ARRAY[index] = false;
        
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
        
        shared_memory->ESP_BOX_FRAME_ARRAY[index] = frame;
        shared_memory->ESP_BOX_COLOR_ARRAY[index] = color;
        shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[index] = border_width;
    }
    return frame;
}

//Launch a roblox game from RobloxCheats using your cookie and desired PlaceId. ServerId/JobId is optional.
//If serverid is NULL, a random server will be selected (like normal)
typedef struct
{
    
}
rbx_launch_game_options_t;
void rbx_launch_game(char* cookie_txt_path,
                     char* placeid,
                     char* serverid)
{
    char* cookie = read_file(cookie_txt_path, NULL);
    printf("%s\n", cookie);
    static NSMutableURLRequest* request1;
    request1 = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:@"https://auth.roblox.com/v1/authentication-ticket/"]];
    [request1 setHTTPMethod:@"POST"];
    [request1 setValue:@"0" forHTTPHeaderField:@"Content-Length"];
    [request1 setValue:@"https://www.roblox.com" forHTTPHeaderField:@"referer"];
    [request1 setValue:[NSString stringWithCString:cookie encoding:NSNonLossyASCIIStringEncoding] forHTTPHeaderField:@"cookie"];
    [request1 setTimeoutInterval:10000];
    
    static NSString* rbx_auth_ticket;
    rbx_auth_ticket = NULL;
    
    static char request1_finished = false;
    printf(" > LAUNCHING GAME\n");
    printf("Make sure you have your full cookie in cookie.txt!!!\n");
    
    while (!rbx_auth_ticket)
    {
        request1_finished = false;
        //NSLog(@"%@", [request1 allHTTPHeaderFields]);
        NSURLSessionDataTask *data_task = [NSURLSession.sharedSession dataTaskWithRequest:request1
                                                                        completionHandler:^(NSData *data,
                                                                                            NSURLResponse *response,
                                                                                            NSError *error)
       {
            NSHTTPURLResponse *http_response = (NSHTTPURLResponse *)response;
            if(http_response.statusCode == 200)
            {
                rbx_auth_ticket = [http_response valueForHTTPHeaderField:@"rbx-authentication-ticket"];
                NSLog(@"AUTH TICKET: %@", rbx_auth_ticket);
            }
            else
            {
                NSString* xcsrf_token = [http_response valueForHTTPHeaderField:@"x-csrf-token"];
                NSLog(@"X-CSRF TOKEN: %@", xcsrf_token);
                [request1 setValue:xcsrf_token forHTTPHeaderField:@"x-csrf-token"];
            }
            request1_finished = true;
        }];
        [data_task resume];
        sleep(1);
        while (!request1_finished)
        {
            usleep(1000);
        }
        [data_task suspend];
    }
    
    request1_finished = false;
    
    free(cookie);
    
    char cmd[10000];
    if (serverid)
    {
        char* place_launch_cmd = "DYLD_INSERT_LIBRARIES=%s %s -ticket %s -scriptURL \"https://assetgame.roblox.com/game/PlaceLauncher.ashx?request=RequestGameJob&placeId=%s&gameId=%s\"";
        sprintf(cmd, place_launch_cmd, __LIBESP_DYLIB_PATH__,__ROBLOXPLAYER_PATH__, rbx_auth_ticket.UTF8String, placeid, serverid);
    }
    else
    {
        char* place_launch_cmd = "DYLD_INSERT_LIBRARIES=%s %s -ticket %s -scriptURL \"https://assetgame.roblox.com/game/PlaceLauncher.ashx?request=RequestGame&placeId=%s\"";
        sprintf(cmd, place_launch_cmd, __LIBESP_DYLIB_PATH__,__ROBLOXPLAYER_PATH__, rbx_auth_ticket.UTF8String, placeid);
    }
    printf("%s\n", cmd);
    system(cmd);
}



///Enables smooth flight by stopping our player's velocity from being edited.
///You will need to run patch_roblox_file() for this to work.
void rbx_smooth_fly(task_t task, char status)
{
    vm_address_t task_base_address = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    struct section_64 __TEXT__text_sect = task_get_sect(task, task_base_address, "__TEXT", "__text");
    //Bytes for Velocity write address of humanoid root parts. This will affect NPC's and such.
    //00 81 8E 3C 02 81 8F 3C 09 21 03 91 (apple)
    //03 68 F6 3C 03 3C 80 3D 02 38 80 3D (rosetta)
    /*
    base() + 0xDCBAE0    stur q0, [x8, #0xe4]    00 41 8E 3C
    base() + 0xDCBAE4    stur q2, [x8, #0xf4]    02 41 8F 3C
    base() + 0xDCBAE8    add x10, x8, #0xc4    0A 11 03 91
     */
#ifdef ROBLOX_ROSETTA_VERSION
    char fly_bytes[12] = {"\x03\x68\xF6\x3C\x03\x3C\x80\x3D\x02\x38\x80\x3D"};
#endif
#ifdef ROBLOX_APPLE_VERSION
    char fly_bytes[12] = {"\x00\x41\x8E\x3C\x02\x41\x8F\x3C\x0A\x11\x03\x91"};
    
#endif
    char orig_bytes[4];
    memcpy(orig_bytes, fly_bytes + 4, 4);
    char NOP[4] = {"\x1f\x20\x03\xd5"};
    
    if (status == 1)
    {
#ifdef ROBLOX_APPLE_VERSION
                memcpy(fly_bytes + 4, orig_bytes, 4);
                vm_address_t nop_addr = task_base_address + __TEXT__text_sect.offset + scan_bytes_offset(task,task_base_address + __TEXT__text_sect.offset, __TEXT__text_sect.size, fly_bytes, 12, 4) + 4;
        //printf("%p\n", nop_addr);
                task_suspend(task);
                vm_protect(task, nop_addr, 4, FALSE, VM_PROT_READ|VM_PROT_WRITE|VM_PROT_COPY);
                vm_write(task, nop_addr, (vm_address_t)NOP, 4);
                vm_protect(task, nop_addr, 4, FALSE, VM_PROT_READ|VM_PROT_EXECUTE);
                task_resume(task);
#endif
#ifdef ROBLOX_ROSETTA_VERSION
                memcpy(fly_bytes + 4, orig_bytes, 4);
                vm_address_t nop_addr = find_bytes_in_rosetta(task, fly_bytes, 12) + 4;
                //printf("%p\n", nop_addr);
                task_suspend(task);
                vm_protect(task, nop_addr, 4, FALSE, VM_PROT_READ|VM_PROT_WRITE|VM_PROT_COPY);
                vm_write(task, nop_addr, (vm_address_t)NOP, 4);
                vm_protect(task, nop_addr, 4, FALSE, VM_PROT_ALL|VM_PROT_COPY);
                task_resume(task);
#endif
    }
    
    if (status == 0)
    {
#ifdef ROBLOX_APPLE_VERSION
                memcpy(fly_bytes + 4, NOP, 4);
                vm_address_t nop_addr = task_base_address + __TEXT__text_sect.offset + scan_bytes_offset(task,task_base_address + __TEXT__text_sect.offset, __TEXT__text_sect.size, fly_bytes, 12, 4) + 4;
        //printf("%p\n", nop_addr);
                task_suspend(task);
                vm_protect(task, nop_addr, 4, FALSE, VM_PROT_READ|VM_PROT_WRITE|VM_PROT_COPY);
                vm_write(task, nop_addr, (vm_address_t)orig_bytes, 4);
                vm_protect(task, nop_addr, 4, FALSE, VM_PROT_READ|VM_PROT_EXECUTE);
                task_resume(task);
#endif
#ifdef ROBLOX_ROSETTA_VERSION
                memcpy(fly_bytes + 4, NOP, 4);
                vm_address_t nop_addr = find_bytes_in_rosetta(task, fly_bytes, 12) + 4;
                //printf("%p\n", nop_addr);
                task_suspend(task);
                vm_protect(task, nop_addr, 4, FALSE, VM_PROT_READ|VM_PROT_WRITE|VM_PROT_COPY);
                vm_write(task, nop_addr, (vm_address_t)orig_bytes, 4);
                vm_protect(task, nop_addr, 4, FALSE, VM_PROT_ALL|VM_PROT_COPY);
                task_resume(task);
#endif
    }
}

#define YIELD_INPUT(input_index)\
while (shared_memory->INPUT_QUEUE[input_index].should_execute == true) {usleep(500);}

#define SEND_MOUSEMOVE_PRECHECKING(input_index, _x, _y)\
{\
if (shared_memory->INPUT_QUEUE[input_index].should_execute == false)\
{\
shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_MOUSE_MOVE, .should_execute = true, .x = _x, .y = _y};\
}\
}

#define SEND_MOUSEMOVE_YIELDING(input_index, _x, _y)\
{\
YIELD_INPUT(input_index)\
shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_MOUSE_MOVE, .should_execute = true, .x = _x, .y = _y};\
YIELD_INPUT(input_index)\
}

#define SEND_LEFTMOUSEDOWN(input_index)\
{\
shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_LEFT_MOUSE_DOWN, .should_execute = true, .x = shared_memory->MOUSE_X, .y = shared_memory->MOUSE_Y};\
}

#define SEND_LEFTMOUSEUP(input_index)\
{\
shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_LEFT_MOUSE_UP, .should_execute = true, .x = shared_memory->MOUSE_X, .y = shared_memory->MOUSE_Y};\
}


#define SEND_KEYDOWN(input_index, key)\
shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_KEY_DOWN, .should_execute = true, .keycode = keycode_for_character(key)};\

#define SEND_KEYUP(input_index, key)\
shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_KEY_UP, .should_execute = true, .keycode = keycode_for_character(key)};\


#define SEND_LEFTCLICK_YIELDING(input_index, duration) \
{\
YIELD_INPUT(input_index)\
SEND_LEFTMOUSEDOWN(input_index)\
usleep(duration);\
YIELD_INPUT(input_index)\
SEND_LEFTMOUSEUP(input_index)\
YIELD_INPUT(input_index)\
}

#define SEND_LEFTCLICK_PRECHECKING(input_index, duration) \
{\
if (shared_memory->INPUT_QUEUE[input_index].should_execute == false)\
{\
SEND_LEFTMOUSEDOWN(input_index)\
usleep(duration);\
YIELD_INPUT(input_index)\
SEND_LEFTMOUSEUP(input_index)\
}\
}

#define SEND_KEYPRESS_YIELDING(input_index, key, duration) \
{\
YIELD_INPUT(input_index)\
SEND_KEYDOWN(input_index, key)\
usleep(duration);\
YIELD_INPUT(input_index)\
SEND_KEYUP(input_index, key)\
YIELD_INPUT(input_index)\
}

#define SEND_KEYPRESS_PRECHECKING(input_index, key, duration) \
{\
if (shared_memory->INPUT_QUEUE[input_index].should_execute == false)\
{\
SEND_KEYDOWN(input_index, key)\
usleep(duration);\
YIELD_INPUT(input_index)\
SEND_KEYUP(input_index, key)\
}\
}





#include "Hooks.h"


#include "Games/Flood-Escape-Classic.h"
#include "Games/Phantom-Forces.h"
#include "Games/Hack-Tests.h"
#include "Games/Field-Of-Battle/Auto-Farm.h"
#include "Games/Field-Of-Battle/Gem-Collect.h"
#include "Games/Field-Of-Battle/General-Killer.h"
#include "Games/Arsenal.h"
#include "Games/Blox-Fruits/Cheat.h"
#include "Games/Blox-Fruits/Auto-Farm.h"
#include "Games/Those-Who-Remain.h"
//#include "Games/Tower-Defense-Simulator/Auto-Play/Include.h"



int main(int argc, char** argv)
{
    //sleep(100000);
    patch_roblox_file();
    int pid = -1;
    task_t task = get_roblox_task_port(&pid);
    
    //find_object_offsets(task);
    //find_function_offsets(task);
    //generic_cheat(task);
    //blox_fruits_cheat(task);
    //phantom_forces_cheat(task);

    //Jarvis won't work without this :'(. Thanks ChatGPT for letting me know.
    [[NSRunLoop currentRunLoop] run];
    
    //Don't let the process exit if we do use cheats, since many of the cheats run in a background thread.
    for (;;)
    {
        sleep(10000);
    }
    return 0;
}

