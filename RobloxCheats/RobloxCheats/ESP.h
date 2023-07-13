//
//  ESP.h
//  ESP
//
//  Created by me on 9/13/22.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>

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
#include <mach-o/nlist.h>
#include <math.h>
#include <tgmath.h>
#include <dlfcn.h>
#include <math.h>
#include <pthread.h>
#include <pthread/sched.h>
#include <sys/ptrace.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>




typedef struct
{
    float x;
    float y;
    float w; //width
    float h; //height
}
ESP_Frame;

typedef struct
{
    float r;
    float g;
    float b;
    float a; //alpha
}
ESP_Color;


#define MAX_ESP_COUNT 200
#define MAX_ESP_TEXT_LENGTH 200

typedef struct
{
    int type;
    int keycode;
    float x;
    float y;
    char characters[8];
    int window_index;
    char should_execute;
}
Input;

#define MAX_INPUT_COUNT 200

typedef struct
{
    int type;
    vm_address_t address;
    char arguments[8 * 6];
    char return_bytes[8];
    int async_type;
    char should_execute;
}
RemoteFunctionCall;


#define MAX_FUNCTION_COUNT 200

#define FUNCTION_ASYNC_TYPE_MAIN 1
#define FUNCTION_ASYNC_TYPE_SEPERATE 2
#define FUNCTION_ASYNC_TYPE_CURRENT 3

typedef struct
{
    float WINDOW_W;
    float WINDOW_H;

    float MOUSE_X;
    float MOUSE_Y;

    char LEFT_MOUSE_DOWN;
    unsigned char KEYS_DOWN[255];


    unsigned char ESP_ENABLED;
    unsigned char ESP_ALLOCATED;
    int ESP_COUNT;
    void* ESP_BOX_ARRAY[MAX_ESP_COUNT];
    ESP_Frame ESP_BOX_FRAME_ARRAY[MAX_ESP_COUNT];
    ESP_Color ESP_BOX_COLOR_ARRAY[MAX_ESP_COUNT];
    unsigned char ESP_BOX_HIDDEN_ARRAY[MAX_ESP_COUNT];
    float ESP_BOX_BORDER_WIDTH_ARRAY[MAX_ESP_COUNT];
    char ESP_BOX_TEXT_ARRAY[MAX_ESP_COUNT * MAX_ESP_TEXT_LENGTH];
    useconds_t ESP_USLEEP_TIME;
    useconds_t REFRESH_SLEEP_TIME;

    unsigned char INPUT_QUEUE_FINISHED;
    unsigned int INPUT_QUEUE_COUNT;
    Input INPUT_QUEUE[MAX_INPUT_COUNT];
    useconds_t INPUT_USLEEP_TIME;


    unsigned char FUNCTION_QUEUE_FINISHED;
    unsigned int FUNCTION_QUEUE_COUNT;
    RemoteFunctionCall FUNCTION_QUEUE[MAX_INPUT_COUNT];
    useconds_t FUNCTION_USLEEP_TIME;
    
    unsigned char CUSTOM_DATA[4000];
}
libESPSharedMemoryObject;


libESPSharedMemoryObject* init_shared_memory(char* file_path, char should_create_empty_data)
{
    int fd = open(file_path, O_RDWR);
    if (should_create_empty_data)
    {
        char __zero_data[sizeof(libESPSharedMemoryObject)];
        bzero(__zero_data, sizeof(__zero_data));
        write(fd, __zero_data, sizeof(libESPSharedMemoryObject));
    }
    void* address = mmap(0,sizeof(libESPSharedMemoryObject), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return address;
}

