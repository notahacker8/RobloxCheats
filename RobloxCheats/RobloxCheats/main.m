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
#include <mach/arm/_structs.h>
#include <dlfcn.h>
#include <math.h>
#include <pthread.h>
#include <pthread/sched.h>

#include "Memory.h"
#include "ESP.h"
#include "Requests.h"


#define PI 3.1415926535

/*
 This is for testing, change it if you are NOT me.
 */

char* __ROBLOXPLAYER_PATH__ = "/Applications/Roblox.app/Contents/MacOS/RobloxPlayer";
char* __LIBESP_DYLIB_PATH__ = "/Users/dimitriarmendariz/Library/Developer/Xcode/DerivedData/ESP-drqlefpymzgresdhcnxyusawdjhf/Build/Products/Debug/libESP.dylib";
char* __COOKIE_TXT_PATH__ = "/Users/dimitriarmendariz/Documents/roblox-cookie.txt";
char* __OFFSETS_TXT_PATH__ = "/Users/dimitriarmendariz/Documents/roblox-offsets.txt";
char* __CPUTHROTTLE_PATH__ = "/Users/dimitriarmendariz/Library/Developer/Xcode/DerivedData/cputhrottle_remake-dlbdozizqeucwzedjzkryqbozbek/Build/Products/Debug/cputhrottle_remake";




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

void wait_until_queue_finished(task_t task, vm_address_t address, int usleep_time)
{
    for (;;)
    {
        if (vm_read_1byte_value(task, address) == 1)
        {
            break;
        }
        else
        {
            usleep(usleep_time);
        }
    }
    return;
}

void send_simple_keypress(task_t task,
                          vm_address_t load_address,
                          vm_address_t input_queue_offset,
                          vm_address_t input_queue_count_offset,
                          vm_address_t input_queue_finished_offset,
                          int keycode,
                          useconds_t duration)
{
    static char f = 0;
    static int input_count = 2;
    Input inputs[input_count];
    inputs[0] = (Input){.type = 1, .duration = duration, .keycode = keycode, .window_index = 0};
    inputs[1] = (Input){.type = 2, .duration = 0, .keycode = keycode, .window_index = 0};
    
    vm_write(task, load_address + input_queue_offset, (vm_address_t)inputs, (int)sizeof(inputs));
    vm_write(task, load_address + input_queue_count_offset, (vm_address_t)&input_count, sizeof(int));
    vm_write(task, load_address + input_queue_finished_offset, (vm_address_t)&f, sizeof(char));
    
    wait_until_queue_finished(task, load_address + input_queue_finished_offset, 500);
}

//This one sends a character along with the Input. We can't use this for buttons like 'Tab' or 'Delete'
void send_character_keypress(task_t task,
                             vm_address_t load_address,
                             vm_address_t input_queue_offset,
                             vm_address_t input_queue_count_offset,
                             vm_address_t input_queue_finished_offset,
                             char character,
                             useconds_t duration)
{
    static char f = 0;
    static int input_count = 2;
    int keycode = keycode_for_character(character);
    Input inputs[input_count];
    inputs[0] = (Input){.type = 1, .duration = duration, .keycode = keycode, .window_index = 0, .characters[0] = character};
    inputs[1] = (Input){.type = 2, .duration = 0, .keycode = keycode, .window_index = 0, .characters[0] = character};
    
    vm_write(task, load_address + input_queue_offset, (vm_address_t)inputs, (int)sizeof(inputs));
    vm_write(task, load_address + input_queue_count_offset, (vm_address_t)&input_count, sizeof(int));
    vm_write(task, load_address + input_queue_finished_offset, (vm_address_t)&f, sizeof(char));
    
    wait_until_queue_finished(task, load_address + input_queue_finished_offset, 500);
}

//Sends a click, but the coordinates don't really matter. It's usually where your mouse already is.
void send_left_click(task_t task,
                     vm_address_t load_address,
                     vm_address_t input_queue_offset,
                     vm_address_t input_queue_count_offset,
                     vm_address_t input_queue_finished_offset,
                     NSPoint point,
                     useconds_t duration)
{
    static char f = 0;
    static int input_count = 2;
    Input inputs[input_count];
    inputs[0] = (Input){.type = 3, .duration = duration, .window_index = 0, .x = point.x, .y = point.y};
    inputs[1] = (Input){.type = 4, .duration = 0, .window_index = 0, .x = point.x, .y = point.y};
    
    vm_write(task, load_address + input_queue_offset, (vm_address_t)inputs, (int)sizeof(inputs));
    vm_write(task, load_address + input_queue_count_offset, (vm_address_t)&input_count, sizeof(int));
    vm_write(task, load_address + input_queue_finished_offset, (vm_address_t)&f, sizeof(char));
    
    wait_until_queue_finished(task, load_address + input_queue_finished_offset, 500);
}

//Moves your mouse for you.
void send_mouse_move(task_t task,
                     vm_address_t load_address,
                     vm_address_t input_queue_offset,
                     vm_address_t input_queue_count_offset,
                     vm_address_t input_queue_finished_offset,
                     NSPoint point)
{
    static char f = 0;
    static int input_count = 1;
    Input inputs[input_count];
    inputs[0] = (Input){.type = 0, .duration = 0, .window_index = 0, .x = point.x, .y = point.y};
    
    vm_write(task, load_address + input_queue_offset, (vm_address_t)inputs, (int)sizeof(inputs));
    vm_write(task, load_address + input_queue_count_offset, (vm_address_t)&input_count, sizeof(int));
    vm_write(task, load_address + input_queue_finished_offset, (vm_address_t)&f, sizeof(char));
    
    wait_until_queue_finished(task, load_address + input_queue_finished_offset, 500);
}



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

#include "Objects/<<<ROOT>>>.h"
#include "Objects/functions.h"



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

//Launch a roblox game from RobloxCheats using your cookie and desired PlaceId. ServerId/JobId is optional.
void rbx_launch_game(char* cookie_path,
                     char* placeid,
                     char* serverid)
{
    struct stat buf;
    if (stat(cookie_path, &buf) != 0)
    {
        fprintf(stderr, "failed to get info about file: '%s'\n", cookie_path);
        return;
    };
    
    char* cookie = malloc(buf.st_size);
    FILE* fp = fopen(__COOKIE_TXT_PATH__, "r");
    if (!fp)
    {
        fprintf(stderr, "failed to get read data from file: %s\n", cookie_path);
    }
    fread(cookie, buf.st_size, 1, fp);
    fclose(fp);
    
    static NSString* rbx_auth_ticket;
    rbx_auth_ticket = NULL;
    
    static NSMutableURLRequest* request1;
    request1 = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:@"https://auth.roblox.com/v1/authentication-ticket/"]];
    [request1 setHTTPMethod:@"POST"];
    [request1 setValue:@"0" forHTTPHeaderField:@"Content-Length"];
    [request1 setValue:@"https://www.roblox.com" forHTTPHeaderField:@"referer"];
    [request1 setValue:[NSString stringWithCString:cookie encoding:NSNonLossyASCIIStringEncoding] forHTTPHeaderField:@"cookie"];
    
    static char request1_finished = false;
    printf("STARTING\n");
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
            }
            else
            {
                NSString* xcsrf_token = [http_response valueForHTTPHeaderField:@"x-csrf-token"];
                [request1 setValue:xcsrf_token forHTTPHeaderField:@"x-csrf-token"];
            }
            request1_finished = true;
        }];
        [data_task resume];
        while (!request1_finished)
        {
            usleep(1000);
        }
        
    }
    
    request1_finished = false;
    
    free(cookie);
    
    char* place_launch_cmd = "DYLD_INSERT_LIBRARIES=%s %s -ticket %s -scriptURL \"https://assetgame.roblox.com/game/PlaceLauncher.ashx?request=RequestGame&placeId=%s%s\"";
    char cmd[1000];
    char __gameid_param[100];
    bzero(__gameid_param, 100);
    if (serverid)
    {
        sprintf(__gameid_param, "gameId=%s", serverid);
    }
    sprintf(cmd, place_launch_cmd, __LIBESP_DYLIB_PATH__,__ROBLOXPLAYER_PATH__, rbx_auth_ticket.UTF8String, placeid, __gameid_param);
    system(cmd);
}


//This is for when we have a cheat running as a background thread, and want the RobloxCheats process to terminate if the RobloxPlayer process terminates, so the cheats aren't left hanging around.
void rbx_run_robloxcheats_self_termination_thread(void)
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;) { sleep(1); pid_t a[4096]; if (pids_by_name("RobloxPlayer", a) == 0) { exit(0); } }
    });
}

//Update the offsets in memory using the offsets saved in the offsets.txt file. NOT to be confused with what find_object_offsets() and find_function_offsets() do!
void update_all_offsets(char* offsets_file_path)
{
    struct stat buf;
    if (stat(offsets_file_path, &buf) == 0)
    {
        long file_size = buf.st_size;
        char* bytes = malloc(file_size);
        FILE* fp = fopen(offsets_file_path, "r");
        if (fp)
        {
            fread(bytes, file_size, 1, fp);
            //printf("%s\n", bytes);
            char* __start1 = strstr(bytes, "int ");
            while (__start1)
            {
                __start1 = strstr(__start1, "int ");
                if (__start1)
                {
                    char* __end1 = strstr(__start1, " = ");
                    if (__end1)
                    {
                        char* __offset_name = __start1 + strlen("int ");
                        char offset_name[(__end1 - __offset_name) + 1];
                        bzero(offset_name, sizeof(offset_name));
                        memcpy(offset_name, __offset_name, (__end1 - __offset_name));
                        
                        int* data_ptr = dlsym(RTLD_DEFAULT, offset_name);
                        
                        if (data_ptr)
                        {
                            char* __start2 = __end1 + strlen(" = ");
                            char* __end2 = strstr(__start2, ";");
                            if (__end2)
                            {
                                char hexstr[(__end2 - __start2) + 1];
                                bzero(hexstr, sizeof(hexstr));
                                memcpy(hexstr, __start2, (__end2 - __start2));
                                //printf("%p\n", (int)strtol(hexstr, NULL, 0));
                                int new_value = (int)strtol(hexstr, NULL, 0);
                                //printf("Updating '%s' to %p\n", offset_name, (void*)(long)new_value);
                                *data_ptr = new_value;
                            }
                        }
                        else
                        {
                            fprintf(stderr, "failed to locate symbol '%s' in this process\n", offset_name);
                        }
                    }
                    __start1 = __end1;
                }
            }
        }
        else
        {
            fprintf(stderr, "failed to open file: '%s'\n", offsets_file_path);
        }
        fclose(fp);
        free(bytes);
    }
    else
    {
        fprintf(stderr, "failed to get info about file: '%s'\n", offsets_file_path);
    }
}


#include "Games/Flood-Escape-Classic.h"
#include "Games/Phantom-Forces.h"
#include "Games/Hack-Tests.h"
#include "Games/Field-Of-Battle.h"
#include "Games/Doors.h"
#include "Games/Arsenal.h"
#include "Games/Blox-Fruits.h"
#include "Games/Jailbreak.h"
#include "Games/Emergency-Response.h"
#include "Games/Word-Bomb.h"


//#define TESTING   //When you are in Xcode


int main(int argc, char** argv)
{
    
#ifndef TESTING
    static char current_directory[PATH_MAX];
    bzero(current_directory, sizeof(current_directory));
    getcwd(current_directory, PATH_MAX);
    
    __CPUTHROTTLE_PATH__ = malloc(PATH_MAX);
    __LIBESP_DYLIB_PATH__ = malloc(PATH_MAX);
    __OFFSETS_TXT_PATH__ = malloc(PATH_MAX);
    __COOKIE_TXT_PATH__ = malloc(PATH_MAX);
    
    sprintf(__LIBESP_DYLIB_PATH__, "%s%s", current_directory, "/libESP.dylib");
    sprintf(__CPUTHROTTLE_PATH__, "%s%s", current_directory, "/Tools/cputhrottle_remake");
    sprintf(__OFFSETS_TXT_PATH__, "%s%s", current_directory, "/Data/offsets.txt");
    sprintf(__COOKIE_TXT_PATH__, "%s%s", current_directory, "/Data/cookie.txt");
    
#endif
    
    kern_return_t kr;
    printf("RobloxCheats pid: %d\n", getpid());
    task_t task;
    pid_t pid_array[4096];
    memset(pid_array, -1, sizeof(pid_array));
    int process_count = pids_by_name("RobloxPlayer", pid_array);
    
    pid_t pid = pid_array[0]; //The most recent RobloxPlayer process.
    printf("RobloxPlayer pid: %d\n", pid);
    printf("\nIf using in Xcode, make sure both libESP.dylib and RobloxCheats are building for Intel\n\n");
    kr = task_for_pid(mach_task_self_, pid, &task);
    if (kr != KERN_SUCCESS)
    {
        printf("%s%d\n", "failed to obtain task port for pid: ", pid);
        printf("%s\n", mach_error_string(kr));
        printf("%s\n", "(Do you have SIP disabled?)");
    }

    //We have to run the Blox Fruits Chest Farm from the Command Line, since whenever we launch Roblox from this process, Roblox becomes a child process, and the main thread of RobloxCheats kinda stops doing its own thing.
    if (argc > 1)
    {
        update_all_offsets(__OFFSETS_TXT_PATH__);
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
                rbx_launch_game(__COOKIE_TXT_PATH__, "2753915549", "");
            }
        }
        if (strcmp(argv[1], "phantomforces") == 0)
        {
            rbx_run_robloxcheats_self_termination_thread();
            phantom_forces_cheat(task);
        }
        if (strcmp(argv[1], "doors") == 0)
        {
            rbx_run_robloxcheats_self_termination_thread();
            doors_cheat(task);
        }
        if (strcmp(argv[1], "arsenal") == 0)
        {
            rbx_run_robloxcheats_self_termination_thread();
            arsenal_cheat(task);
        }
        if (strcmp(argv[1], "jailbreak") == 0)
        {
            rbx_run_robloxcheats_self_termination_thread();
            jailbreak_cheat(task);
        }
        if (strcmp(argv[1], "find_function_offsets") == 0)
        {
            find_function_offsets(task);
            exit(0);
        }
        if (strcmp(argv[1], "find_object_offsets") == 0)
        {
            if (argc > 2)
            {
                find_object_offsets(task, argv[2]);
            }
            else
            {
                fprintf(stderr, "error: lacking argument '%s'\n", "display_name");
            }
            exit(0);
        }
    }
    else
    {
        printf("\nPlease provide an argument\n");
        printf("List of arguments:\n");
        printf("\tfind_object_offsets [display_name]\n");
        printf("\tfind_function_offsets\n");
        printf("\tblox_fruits_chest_farm\n");
        printf("\tdoors\n");
        printf("\tphantomforces\n");
        printf("\tjailbreak\n");
        printf("\tarsenal\n");
        printf("ex.: ./RobloxCheats doors\n\n");
#ifndef TESTING
        exit(0);
#endif
    }
    
    
    //jailbreak_cheat(task);
    
    
    //Don't let the process exit if we do use cheats, since many of the cheats run in a background thread.
    for (;;)
    {
        sleep(10);
    }
    
    return 0;
}
