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


#define KR(value) if (value != KERN_SUCCESS)\
{\
    printf("kern error: %s, line %d\n", mach_error_string(value), __LINE__);\
}

#define MSG_CHECK(value) \
        if (value != MACH_MSG_SUCCESS) { \
            printf("mach_msg error: %s (0x%x), line %d\n", mach_error_string(value), value, __LINE__); \
        } \

#include "Macros.h"


//RGB color struct
typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
}
rbx_rgb_t;



#import <Foundation/Foundation.h>
#import <ApplicationServices/ApplicationServices.h>
#import <AVFoundation/AVFoundation.h>
#import <AVFAudio/AVFAudio.h>
#import <Cocoa/Cocoa.h>
#import <Speech/Speech.h>
#import <CoreImage/CoreImage.h>
#import <Vision/Vision.h>

#include <spawn.h>
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
#include <Kernel/mach/mach_exc_server.h>
#import <sys/sysctl.h>
#import <unistd.h>


void moveMouseOnWindow(float window_x, float window_y, float window_w, float window_h, CGPoint point)
{
    @autoreleasepool {
        NSScreen* s = [NSScreen mainScreen];
        CGPoint p;
        float x = window_x;
        float y = window_y;
        //float y_o = [window titlebarHeight];
        p.y = s.frame.size.height - y - window_h;
        p.y += (window_h - point.y);
        p.x = x + (point.x);
        CGWarpMouseCursorPosition(p);
    }
    //[window sendEvent: [NSEvent mouseEventWithType:NSEventTypeMouseMoved location:point modifierFlags:0 timestamp:0 windowNumber:0 context:NULL eventNumber:0 clickCount:0 pressure:0]];
}



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
#define NOP "\x1f\x20\x03\xd5"


#include "Objects/brickcolors.h"
#include "Objects/vector3.h"
#include "Objects/cframe.h"

#include "Memory.h"
#include "ESP.h"
#include "Code-Snippets.h"
#include "Object-Pointee-Offsets.h"
#include "Offsets.h"
#include "Inputs.h"
#include "Images.h"

/*
 Change if you are NOT me.
 */

#pragma mark File Paths



char* __ROBLOXPLAYER_PATH__ = "/Applications/Roblox.app/Contents/MacOS/RobloxPlayer";
char* __LIBESP_DYLIB_PATH__ = "/Users/dimitriarmendariz/Library/Developer/Xcode/DerivedData/ESP-drqlefpymzgresdhcnxyusawdjhf/Build/Products/Debug/libESP.dylib";
//Empty folder somewhere on your computer. Will be filled with .txt files depending on how many instances of roblox you have open.
char* __ROBLOX_PIPES_FOLDER_PATH__ = "/Users/dimitriarmendariz/Documents/RobloxPipes";
char __ROBLOX_PIPE_TXT_PATH__[PATH_MAX]; //Changes by itself


//Only needed if you are going to launch a game using rbx_launch_game().
char* __COOKIE_TXT_PATH__ = "/Users/dimitriarmendariz/Documents/roblox-cookie.txt";

//Not necessary, put required for auto-injection of libESP.dylib. (If you don't run Roblox from the command line with DYLD_INSERT_LIBRARIES)
char* __MACINJECT_PATH__ = "/Users/dimitriarmendariz/Library/Developer/Xcode/DerivedData/MacInject-ccoeetojwdvjqhcawcuypbcirsha/Build/Products/Debug/MacInject";

//Something that throttles the cpu of a process to a certain percentage. Remade with correct entitlements to not require root.
//Find it here: https://github.com/rwanyoike/cputhrottle
char* __CPUTHROTTLE_PATH__ = "/Users/dimitriarmendariz/Library/Developer/Xcode/DerivedData/cputhrottle_remake-dlbdozizqeucwzedjzkryqbozbek/Build/Products/Debug/cputhrottle_remake\0";


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


int isProcessAlive(pid_t pid) {
    if (kill(pid, 0) == 0) {
        return 1; // alive
    }
    if (errno == EPERM) {
        return 1; // alive but no permission
    }
    return 0; // not alive
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

int pids_by_parent_pid(pid_t* inout_array, int parent_pid)
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
            if (proc.pbi_ppid == parent_pid)
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

task_t get_roblox_task_port(int* out_pid, int default_index, char use_default_idx)
{
    char* file_path_format = "%s/roblox-pipe-%d.txt";
    
    task_t task = 0;
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
        if (use_default_idx)
        {
            pid_idx = default_index;
        }
        else
        {
            scanf("%d", &pid_idx);
        }
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
        keycodes['\t'] = 48;
        keycodes[' '] = 49;
        keycodes['='] = 24;
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
#include "Objects/model.h"

#include "Objects/<<<ROOT>>>.h"
#include "Objects/functions.h"
#include "Objects/local_variable.h"



NSRect rbx_draw_esp_box_on_3d_point(task_t task,
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
    NSRect frame;
    bzero(&frame, sizeof(frame));
    custom_rbx_world_to_screen_info_t wtsi;
    wtsi = rbx_world_to_screen_point(camera_cframe, fov, pos, window_w, window_h, object_width, object_height);
    if (!isnormal(wtsi.x) || !isnormal(wtsi.y))
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
        
        frame.origin.x = (wtsi.x - w/2) + x_offset;
        frame.origin.y = (wtsi.y - h/2) + y_offset;
        frame.size.width = w;
        frame.size.height = h;
        
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
    //printf("%s\n", cookie);
    static NSMutableURLRequest* request1;
    request1 = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:@"https://auth.roblox.com/v1/client-assertion/"]];
    [request1 setHTTPMethod:@"GET"];
    [request1 setValue:@"https://www.roblox.com" forHTTPHeaderField:@"referer"];
    [request1 setValue:[NSString stringWithCString:cookie encoding:NSNonLossyASCIIStringEncoding] forHTTPHeaderField:@"cookie"];
    [request1 setTimeoutInterval:10000];
    
    static NSMutableURLRequest* request2;
    request2 = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:@"https://auth.roblox.com/v1/authentication-ticket/"]];
    [request2 setHTTPMethod:@"POST"];
    [request2 setValue:@"https://www.roblox.com" forHTTPHeaderField:@"referer"];
    [request2 setValue:[NSString stringWithCString:cookie encoding:NSNonLossyASCIIStringEncoding] forHTTPHeaderField:@"cookie"];
    [request2 setTimeoutInterval:10000];
    
    static NSString* rbx_client_assertion = NULL;
    static NSString* rbx_auth_ticket = NULL;
    
    static char request1_finished = false;
    static char request2_finished = false;
    
    printf(" > LAUNCHING GAME\n");
    printf("Make sure you have your full cookie in cookie.txt!!!\n");
    
    while (!rbx_client_assertion)
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
                rbx_client_assertion = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
                NSLog(@"RBX CLIENT ASSERTION: %@", rbx_client_assertion);
                [request2 setHTTPBody:data];
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
        request1_finished = false;
    }
    
    
    while (!rbx_auth_ticket)
    {
        request2_finished = false;
        //NSLog(@"%@", [request1 allHTTPHeaderFields]);
        NSURLSessionDataTask *data_task = [NSURLSession.sharedSession dataTaskWithRequest:request2
                                                                        completionHandler:^(NSData *data,
                                                                                            NSURLResponse *response,
                                                                                            NSError *error)
       {
            NSHTTPURLResponse *http_response = (NSHTTPURLResponse *)response;
            if(http_response.statusCode == 200)
            {
                rbx_auth_ticket = [http_response valueForHTTPHeaderField:@"rbx-authentication-ticket"];
                NSLog(@"RBX AUTH TICKET: %@", rbx_auth_ticket);
            }
            else
            {
                NSString* xcsrf_token = [http_response valueForHTTPHeaderField:@"x-csrf-token"];
                [request2 setValue:xcsrf_token forHTTPHeaderField:@"x-csrf-token"];
            }
            request2_finished = true;
        }];
        
        [data_task resume];
        sleep(1);
        while (!request2_finished)
        {
            usleep(1000);
        }
        [data_task suspend];
        request2_finished = false;
    }
    
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
        sprintf(cmd, place_launch_cmd, __LIBESP_DYLIB_PATH__, __ROBLOXPLAYER_PATH__, rbx_auth_ticket.UTF8String, placeid);
    }
    printf("%s\n", cmd);
    system(cmd);
    
    rbx_client_assertion = NULL;
    rbx_auth_ticket = NULL;
}



pthread_mutex_t __text_edit_lock;

void edit_exec_memory(task_t task, char* bytes, vm_address_t addr, int len)
{
    pthread_mutex_lock(&__text_edit_lock);
    task_suspend(task);
    vm_protect(task, addr, len, FALSE, VM_PROT_READ|VM_PROT_WRITE|VM_PROT_COPY);
    vm_write(task, addr, (vm_address_t)bytes, len);
    vm_protect(task, addr, len, FALSE, VM_PROT_READ|VM_PROT_EXECUTE);
    task_resume(task);
    pthread_mutex_unlock(&__text_edit_lock);
}

void rbx_smooth_fly_patch(task_t task, vm_address_t task_base_address, char status)
{
    //return; //sh dont work :(
    static char __status = 0;
    if (status == __status)
    {
        return;
    }

    char orig_bytes[4] = {"\x02\x81\x88\x3c"};
    
    vm_address_t nop_addr = task_base_address + FLY_PATCH_OFFSET;
    __status = status;
    if (status == 1)
    {
        edit_exec_memory(task, NOP, nop_addr, 4);
    }
    if (status == 0)
    {
        edit_exec_memory(task, orig_bytes, nop_addr, 4);
    }
}

//basically stops welds from working
void weld_patch(task_t task, vm_address_t task_base_address, char status)
{
    static char __status = 0;
    if (status == __status)
    {
        return;
    }
    
    char orig_bytes[4] = {"\x51\x68\x00\x4C"};
    vm_address_t nop_addr = task_base_address + WELD_PATCH_OFFSET;
    __status = status;
    if (status == 1)
    {
        edit_exec_memory(task, NOP, nop_addr, 4);
    }
    if (status == 0)
    {
        edit_exec_memory(task, orig_bytes, nop_addr, 4);
    }
}

void hrp_patch(task_t task, vm_address_t task_base_address, char status)
{
    static char __status = 0;
    if (status == __status)
    {
        return;
    }
    
    /*
     base() + 0x1EA6924    str x8, [x19, #0x108]    68 86 00 F9
     base() + 0x1EA6928    stur q2, [x19, #0xf8]    62 82 8F 3C
     base() + 0x1EA692C    stur q1, [x19, #0xe8]    61 82 8E 3C
     base() + 0x1EA6930    stur q0, [x19, #0xd8]    60 82 8D 3C
     */
    
    char orig_bytes[16] = {"\x68\x86\x00\xF9\x62\x82\x8F\x3C\x61\x82\x8E\x3C\x60\x82\x8D\x3C"};
    vm_address_t nop_addr = task_base_address + HRP_PATCH_OFFSET;
    
    __status = status;
    if (status == 1)
    {
        edit_exec_memory(task, NOP, nop_addr, 4);
        edit_exec_memory(task, NOP, nop_addr + 4, 4);
        edit_exec_memory(task, NOP, nop_addr + 8, 4);
        edit_exec_memory(task, NOP, nop_addr + 12, 4);
    }
    if (status == 0)
    {
        edit_exec_memory(task, orig_bytes, nop_addr, 16);
    }
}




#include "Generic.h"
#include "Games/Hack-Tests.h"
#include "Games/Arsenal.h"
#include "Games/Tower-Defense-Simulator/Auto-Play/Include.h"
#include "Games/Tower-Defense-Simulator/Cheats/Include.h"
#include "Games/Blox-Fruits/Auto-Farm.h"
#include "Games/Blox-Fruits/Cheat.h"
#include "Games/Those-Who-Remain.h"
#include "Games/Field-Of-Battle/Auto-Farm.h"
#include "Games/Field-Of-Battle/Gem-Collect.h"
#include "Games/Field-Of-Battle/General-Killer.h"
#include "Games/Dead-Rails.h"
#include "Games/Stop-It-Slender.h"
#include "Games/Phantom-Forces.h"
#include "Games/Assassin.h"
#include "Games/Hitman-Card-Game.h"
#include "Games/Lumber-Tycoon-2.h"

///DOES NOT WORK IN XCODE DEBUGGER ENVIRONMENT
void RUNTIME_INJECTION(int pid)
{
    char cmd[2000];
    bzero(cmd, 2000);
    sprintf(cmd, "%s %s %d", __MACINJECT_PATH__, __LIBESP_DYLIB_PATH__, pid);
    printf("%s\n", cmd);
    //system(cmd);
}


#define THOSE_WHO_REMAIN_PLACEID 488667523
#define THOSE_WHO_REMAIN_PLACE_ID_STR "488667523"
#define TDS_PLACE_ID 3260590327

#define FOB_PRO_PLACE_ID 213519718
#define FOB_ELITE_PLACE_ID 213518645
#define FOB_BEGINNER_PLACE_ID 147536429

#include <stdio.h>
#include <mach/mach.h>

//For testing
void ttst(task_t task) {
    thread_act_array_t thread_list;
    mach_msg_type_number_t thread_count;
    kern_return_t kr;

    kr = task_threads(task, &thread_list, &thread_count);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "task_threads failed: %s\n", mach_error_string(kr));
        return;
    }

    for (mach_msg_type_number_t i = 0; i < thread_count; i++) {
        thread_extended_info_data_t extinfo;
        mach_msg_type_number_t count = THREAD_EXTENDED_INFO_COUNT;

        kr = thread_info(thread_list[i],
                         THREAD_EXTENDED_INFO,
                         (thread_info_t)&extinfo,
                         &count);
        if (kr == KERN_SUCCESS) {
            printf("%s, cpu: %d\n", extinfo.pth_name[0] ? extinfo.pth_name : "(unnamed)", extinfo.pth_cpu_usage);
            if (extinfo.pth_name[0])
            {
                //KR(thread_suspend(thread_list[i]))
            }
            else
            {
                KR(thread_suspend(thread_list[i]))
            }
        }
    }

    // cleanup
    for (mach_msg_type_number_t i = 0; i < thread_count; i++) {
        mach_port_deallocate(mach_task_self(), thread_list[i]);
    }
    vm_deallocate(mach_task_self(),
                  (vm_address_t)thread_list,
                  thread_count * sizeof(thread_act_t));
}



int main(int argc, char** argv)
{
    srand(time(NULL));
    pthread_mutex_init(&__text_edit_lock, NULL);
    
    pid_t pid = -1; task_t task = get_roblox_task_port(&pid, 0, false);
    
    //find_object_offsets(task);
    //find_object_pointee_offsets(task);
    
    //hitman_card_game_cheat(task);
    //bg_prio_auto_all(false);
    //assassin_cheat(task);
    //rbx_print_textlabel_descendants(task, game, 1000, 10000000, 7);
    //rbx_print_game_info(task);
    //lt2_cheat(task);
    //rbx_find_game_address(task);
    //field_of_battle_general_killer(task);
    //field_of_battle_end_round(task);
    //rbx_print_textlabel_descendants(task, rbx_find_game_address(task), 1000, 100000, 1000);
    //tds_collect(task);
    //generic_cheat(task);
    //tds_autofarm_operation(argc, argv);
    //tds_hardcore_death_farm_operation(argc, argv);
    //find_object_offsets(task);
    //rbx_print_game_info(task);
    //generic_cheat(task);
    //dead_rails_cheat(task);
    //tds_hardcore_death_farm_operation(argc, argv);
    //tds_easy_gold_scout_win_farm_operation(argc, argv);
    //those_who_remain_cheat(task);
    //blox_fruits_cheat(task);
    //assassin_cheat(task);
    //stop_it_slender_cheat(task);
    //arsenal_cheat(task);
    //phantom_forces_cheat(task);
    //blox_fruits_auto_farm(task, autofarm_info);
    //find_object_offsets(task);
    //those_who_remain_cheat(task);
    //blox_fruits_auto_farm(task, chrono_farm);
    //generic_cheat(task);
    //bg_prio_auto_all(true);
    //anti_afk_all();
    //tds_cheat(task);
    //those_who_remain_cheat(task);
    
    //rbx_print_descendants(task, rbx_find_game_address(task), 0, 4);

    //Don't let the process exit if we do use cheats, since many of the cheats run in a background thread.
    
    for (;;)
    {
        sleep(10);
    }
    return 0;
}
