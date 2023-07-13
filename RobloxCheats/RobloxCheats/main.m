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

#define PI 3.1415926535
#define FREEABLE
#define VM_DEALLOCABLE



//#define ROBLOX_INTEL_VERSION //Intel, native
//#define ROBLOX_ROSETTA_VERSION //Intel, but on apple silicon
#define ROBLOX_APPLE_VERSION



#include "Memory.h"
#include "Macros.h"
#include "ESP.h"
#include "Requests.h"

/*
 Change it if you are NOT me.
 */

char* __ROBLOXPLAYER_PATH__ = "/Applications/Roblox.app/Contents/MacOS/RobloxPlayer";
char* __LIBESP_DYLIB_PATH__ = "/Users/dimitriarmendariz/Library/Developer/Xcode/DerivedData/ESP-drqlefpymzgresdhcnxyusawdjhf/Build/Products/Debug/libESP.dylib";
char* __COOKIE_TXT_PATH__ = "/Users/dimitriarmendariz/Documents/roblox-cookie.txt";
char* __OFFSETS_TXT_PATH__ = "/Users/dimitriarmendariz/Documents/roblox-offsets.txt";
char* __ROBLOX_PIPE_TXT_PATH__ = "/Users/dimitriarmendariz/Documents/roblox-pipe.txt";


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
                inout_array[pid_index] = pids[i];
                pid_index++;
            }
        }
    }
    return pid_index;
}


task_t get_roblox_task_port(void)
{
    task_t task;
    pid_t pid_array[4096];
    memset(pid_array, -1, sizeof(pid_array));
    pids_by_name("RobloxPlayer", pid_array);
    
    pid_t pid = pid_array[0]; //The most recent RobloxPlayer process.
    kern_return_t kr = task_for_pid(mach_task_self_, pid, &task);
    if (kr != KERN_SUCCESS)
    {
        printf("%s%d\n", "failed to obtain task port for pid: ", pid);
        printf("%s\n", mach_error_string(kr));
        printf("%s\n", "(Do you have SIP disabled?)");
    }
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

#if defined ROBLOX_APPLE_VERSION || ROBLOX_INTEL_VERSION

//ONLY RUN WHEN ROBLOX UPDATES
void patch_roblox_file(void)
{
    long file_bytes_length = 0;
    char* file_bytes = read_file(__ROBLOXPLAYER_PATH__, &file_bytes_length);
    if (file_bytes)
    {
        //Instructions of the function that check if we manipulate the __TEXT segment.
        //Not needed for the Rosetta version of Roblox
#ifdef ROBLOX_APPLE_VERSION
        char RET[4] = "\xc0\x03\x5f\xd6";
        char bytes_to_find[8] = "\x83\x06\xC1\x3C\x60\x94\xA1\x4E";
#endif
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
                }
                i = 0;
            }
        }
        free(file_bytes);
    }
}

#endif


#include "Objects/offsets.h"
#include "Objects/brickcolors.h"
#include "Objects/vector3.h"
#include "Objects/cframe.h"



vector3_t vm_read_vector3_value(task_t task, vm_address_t address)
{
    mach_msg_type_number_t data_cnt;
    vm_address_t read_data;
    vector3_t __data;
    bzero(&__data, sizeof(vector3_t));
    kern_return_t kr = vm_read(task, address, sizeof(vector3_t), (vm_offset_t*)&read_data, &data_cnt);
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
    kern_return_t kr = vm_read(task, address, sizeof(rbx_cframe_t), (vm_offset_t*)&read_data, &data_cnt);
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
    system(cmd);
}



#include "Hooks.h"


#include "Games/Flood-Escape-Classic.h"
#include "Games/Phantom-Forces.h"
#include "Games/Hack-Tests.h"
//#include "Games/Field-Of-Battle/Gem-Collect.h"
//#include "Games/Field-Of-Battle/Auto-Farm.h"
//#include "Games/Doors.h"
//#include "Games/Arsenal.h"
#include "Games/Blox-Fruits/Cheat.h"
#include "Games/Blox-Fruits/Auto-Farm.h"
//#include "Games/Jailbreak.h"
//#include "Games/Those-Who-Remain.h"





#define TESTING   //When you are in Xcode. Comment this macro out when building the final product.





int main(int argc, char** argv)
{
    patch_roblox_file();
    task_t task = get_roblox_task_port();
    blox_fruits_cheat(task);
    //phantom_forces_cheat(task);
    //vm_address_t tba = task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__);
    //seg_sect_address(task, tba, "__TEXT", "__text");
    //vm_address_t root_table =  task_get_image_address_by_path(task, __ROBLOXPLAYER_PATH__) + 0x39C94B0; //rbx_find_root_table(task);
    //printf("<<<ROOT>>>: %p\n", (void*)root_table);
    //vm_address_t a = rbx_root_table_lookup(task, root_table, "Name");
    //printf("%p\n", a);
    //vm_address_t game = rbx_find_game_address(task);
    //rbx_print_descendants(task, game, 0, 10);
    //flood_escape_classic_cheat();
    //blox_fruits_cheat(task);
    //phantom_forces_cheat(task);
    //blox_fruits_auto_farm(task, "Forest Pirate [Lv. 1825]", "Deep Forest Quest Giver", -9, '1', false, false, false, false, false, false);
    //blox_fruits_auto_farm(task, "Desert Bandit [Lv. 60]", "Desert Adventurer", -2, '1', true, false, false, false, false, false);
    //blox_fruits_auto_farm(task, "Cursed Captain [Lv. 1325] [Raid Boss]", "Desert Adventurer", -10, '1', false, false, false, false, false, false);
    //blox_fruits_auto_farm("Mythological Pirate [Lv. 1850]", "", -10, '1');
    //find_function_offsets(task);
    //find_object_offsets(task);
    //phantom_forces_cheat(task);
    //Don't let the process exit if we do use cheats, since many of the cheats run in a background thread.
    for (;;)
    {
        sleep(10);
    }
    return 0;
}

