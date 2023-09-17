//
//  ESP.h
//  ESP
//
//  Created by me on 9/13/22.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>
#import "ESP.h"

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










typedef void r_void_p_void_f(void);

typedef void r_void_p_long_f(long);
typedef void r_void_p_long_long_f(long, long);
typedef void r_void_p_long_long_long_f(long, long, long);
typedef void r_void_p_long_long_long_long_f(long, long, long, long);
typedef void r_void_p_long_long_long_long_long_f(long, long, long, long, long);
typedef void r_void_p_long_long_long_long_long_long_f(long, long, long, long, long, long);

typedef void r_void_p_long_float_f(long, float);



@implementation NSWindow (TitleBarHeight)
- (CGFloat) titlebarHeight
{
    return self.frame.size.height - [self contentRectForFrameRect: self.frame].size.height;
}
@end


void moveMouseOnWindow(NSWindow* window, CGPoint point)
{
    NSScreen* s = [NSScreen mainScreen];
    CGPoint p;
    float x = window.frame.origin.x;
    float y = window.frame.origin.y;
    //float y_o = [window titlebarHeight];
    p.y = s.frame.size.height - y - window.frame.size.height;
    p.y += point.y;
    p.x = x + (point.x);
    CGWarpMouseCursorPosition(p);
}



void sendKeyDown(int window_index, int keycode, char* characters)
{
    [NSApp.windows[window_index] sendEvent:[NSEvent keyEventWithType:NSEventTypeKeyDown location:CGPointZero modifierFlags:0 timestamp:0 windowNumber:0 context:NULL characters:[NSString stringWithCString:characters encoding:NSStringEncodingConversionAllowLossy] charactersIgnoringModifiers:[NSString stringWithCString:characters encoding:NSStringEncodingConversionAllowLossy] isARepeat:FALSE keyCode:keycode]];
}
void sendKeyUp(int window_index, int keycode, char* characters)
{
    [NSApp.windows[window_index] sendEvent:[NSEvent keyEventWithType:NSEventTypeKeyUp location:CGPointZero modifierFlags:0 timestamp:0 windowNumber:0 context:NULL characters:[NSString stringWithCString:characters encoding:NSStringEncodingConversionAllowLossy] charactersIgnoringModifiers:[NSString stringWithCString:characters encoding:NSStringEncodingConversionAllowLossy] isARepeat:FALSE keyCode:keycode]];
    
}
void sendLeftMouseDown(int window_index, CGPoint point)
{
    [NSApp.windows[window_index] sendEvent: [NSEvent mouseEventWithType:NSEventTypeLeftMouseDown location:point modifierFlags:0 timestamp:0 windowNumber:0 context:NULL eventNumber:0 clickCount:1 pressure:1]];
}
void sendLeftMouseUp(int window_index, CGPoint point)
{
    [NSApp.windows[window_index] sendEvent: [NSEvent mouseEventWithType:NSEventTypeLeftMouseUp location:point modifierFlags:0 timestamp:0 windowNumber:0 context:NULL eventNumber:0 clickCount:1 pressure:1]];
}






/*
float WINDOW_W;
float WINDOW_H;

float MOUSE_X;
float MOUSE_Y;

char LEFT_MOUSE_DOWN = false;
unsigned char KEYS_DOWN[255];


unsigned char ESP_ENABLED = false;
unsigned char ESP_ALLOCATED = false;
int ESP_COUNT = 0;
void* ESP_BOX_ARRAY[MAX_ESP_COUNT];
ESP_Frame ESP_BOX_FRAME_ARRAY[MAX_ESP_COUNT];
ESP_Color ESP_BOX_COLOR_ARRAY[MAX_ESP_COUNT];
unsigned char ESP_BOX_HIDDEN_ARRAY[MAX_ESP_COUNT];
float ESP_BOX_BORDER_WIDTH_ARRAY[MAX_ESP_COUNT];
char ESP_BOX_TEXT_ARRAY[MAX_ESP_COUNT * MAX_ESP_TEXT_LENGTH];
useconds_t ESP_USLEEP_TIME = 100000;
useconds_t REFRESH_SLEEP_TIME = 5;






unsigned char INPUT_QUEUE_FINISHED = true;
unsigned int INPUT_QUEUE_COUNT = 0;
Input INPUT_QUEUE[MAX_INPUT_COUNT];
useconds_t INPUT_USLEEP_TIME = 300000;


unsigned char FUNCTION_QUEUE_FINISHED = true;
unsigned int FUNCTION_QUEUE_COUNT = 0;
RemoteFunctionCall FUNCTION_QUEUE[MAX_INPUT_COUNT];
useconds_t FUNCTION_USLEEP_TIME = 300000;

char SHOULD_NOP = false;
vm_address_t NOP_ADDRESS = 0x0;
*/

char SHARED_FILE_PATH[PATH_MAX];
char SHOULD_CREATE_SHARED_MEMORY = false;
char SHARED_MEMORY_CREATED = false;
libESPSharedMemoryObject* SHARED_MEMORY;



void EXECUTE_FUNCTION(int i)
{
    RemoteFunctionCall func = SHARED_MEMORY->FUNCTION_QUEUE[i];
    if (func.should_execute == true)
    {
        if (func.type == 0)
        {
            ((r_void_p_void_f*)func.address)();
        }
        if (func.type == 1)
        {
            long arg1 = ((long*)func.arguments)[0];
            ((r_void_p_long_f*)func.address)(arg1);
        }
        if (func.type == 2)
        {
            long arg1 = ((long*)func.arguments)[0];
            long arg2 = ((long*)func.arguments)[1];
            ((r_void_p_long_long_f*)func.address)(arg1, arg2);
        }
        if (func.type == 3)
        {
            long arg1 = ((long*)func.arguments)[0];
            long arg2 = ((long*)func.arguments)[1];
            long arg3 = ((long*)func.arguments)[2];
            ((r_void_p_long_long_long_f*)func.address)(arg1, arg2, arg3);
        }
        if (func.type == 4)
        {
            long arg1 = ((long*)func.arguments)[0];
            long arg2 = ((long*)func.arguments)[1];
            long arg3 = ((long*)func.arguments)[2];
            long arg4 = ((long*)func.arguments)[3];
            ((r_void_p_long_long_long_long_f*)func.address)(arg1, arg2, arg3, arg4);
        }
        if (func.type == 5)
        {
            long arg1 = ((long*)func.arguments)[0];
            long arg2 = ((long*)func.arguments)[1];
            long arg3 = ((long*)func.arguments)[2];
            long arg4 = ((long*)func.arguments)[3];
            long arg5 = ((long*)func.arguments)[4];
            ((r_void_p_long_long_long_long_long_f*)func.address)(arg1, arg2, arg3, arg4, arg5);
        }
        if (func.type == 6)
        {
            long arg1 = ((long*)func.arguments)[0];
            long arg2 = ((long*)func.arguments)[1];
            long arg3 = ((long*)func.arguments)[2];
            long arg4 = ((long*)func.arguments)[3];
            long arg5 = ((long*)func.arguments)[4];
            long arg6 = ((long*)func.arguments)[5];
            ((r_void_p_long_long_long_long_long_long_f*)func.address)(arg1, arg2, arg3, arg4, arg5, arg6);
        }
        if (func.type == 7)
        {
            long arg1 = ((long*)func.arguments)[0];
            float arg2 = ((float*)func.arguments + 8)[0];
            ((r_void_p_long_float_f*)func.address)(arg1, arg2);
        }
        SHARED_MEMORY->FUNCTION_QUEUE[i].should_execute = false;
    }
}

/*
void SIGTRAP_HANDLER(int signum, siginfo_t* signal_info, void* context)
{
    ucontext_t *ucontext = (ucontext_t *)context;
    ucontext->uc_mcontext->__ss.__rip += 4;
}
*/


__attribute__((constructor))
void initialize(void)
{
    /*{
        struct sigaction action;
        memset(&action, 0, sizeof(action));
        sigemptyset(&action.sa_mask);
        action.sa_flags |= SA_SIGINFO;
        action.sa_sigaction = &SIGTRAP_HANDLER;
        sigaction(SIGBUS, &action, NULL);
    }*/
    
    
    //Create unshared memory to give the functions something to write to.
    //This will be deallocated once real shared memory is created
    SHARED_MEMORY = malloc(sizeof(libESPSharedMemoryObject));
    bzero(SHARED_MEMORY, sizeof(libESPSharedMemoryObject));
    
    SHARED_MEMORY->LEFT_MOUSE_DOWN = false;

    SHARED_MEMORY->ESP_ENABLED = false;
    SHARED_MEMORY->ESP_ALLOCATED = false;
    SHARED_MEMORY->ESP_COUNT = 0;
    SHARED_MEMORY->ESP_USLEEP_TIME = 100000;
    SHARED_MEMORY->REFRESH_SLEEP_TIME = 5;

    SHARED_MEMORY->INPUT_QUEUE_FINISHED = true;
    SHARED_MEMORY->INPUT_QUEUE_COUNT = 0;
    SHARED_MEMORY->INPUT_USLEEP_TIME = 300000;

    SHARED_MEMORY->FUNCTION_QUEUE_FINISHED = true;
    SHARED_MEMORY->FUNCTION_QUEUE_COUNT = 0;
    SHARED_MEMORY->FUNCTION_USLEEP_TIME = 300000;
    
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskMouseMoved handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        SHARED_MEMORY->MOUSE_X = event.locationInWindow.x;
        SHARED_MEMORY->MOUSE_Y = event.locationInWindow.y;
        return event;
    }];
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskLeftMouseDown handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        SHARED_MEMORY->LEFT_MOUSE_DOWN = true;
        return event;
    }];
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskLeftMouseUp handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        SHARED_MEMORY->LEFT_MOUSE_DOWN = false;
        return event;
    }];
    
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        char key_ascii = [event.characters cStringUsingEncoding:NSNonLossyASCIIStringEncoding][0];
        ((char*)(SHARED_MEMORY->KEYS_DOWN + key_ascii))[0] = true;
        return event;
    }];
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyUp handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        char key_ascii = [event.characters cStringUsingEncoding:NSNonLossyASCIIStringEncoding][0];
        ((char*)(SHARED_MEMORY->KEYS_DOWN + key_ascii))[0] = false;
        return event;
    }];
    
    
    printf("\n-> DYLIB LOADED\n\n");
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (SHARED_MEMORY->ESP_ALLOCATED == false && SHARED_MEMORY->ESP_ENABLED == true)
            {
                dispatch_async(dispatch_get_main_queue(), ^
                {
                    for (int i = 0 ; i < MAX_ESP_COUNT ; i++)
                    {
                        SHARED_MEMORY->ESP_BOX_HIDDEN_ARRAY[i] = true;
                        SHARED_MEMORY->ESP_BOX_COLOR_ARRAY[i].r = 255.0f;
                        SHARED_MEMORY->ESP_BOX_COLOR_ARRAY[i].a = 1.0f;
                        
                        NSTextField* esp_box;
                        esp_box = [[NSTextField alloc] init];
                        esp_box.drawsBackground = NO;
                        esp_box.editable = NO;
                        esp_box.selectable = NO;
                        esp_box.hidden = YES;
                        esp_box.alignment = NSTextAlignmentCenter;
                        esp_box.wantsLayer = YES;
                        esp_box.layer.cornerRadius = 3;
                        esp_box.bezeled = NO;
                        [esp_box setFont: [NSFont fontWithName:@"Menlo Regular" size:15]];
                        [NSApp.windows[0].contentView.subviews[0] addSubview:esp_box];
                        SHARED_MEMORY->ESP_BOX_ARRAY[i] = (__bridge void *)(esp_box);
                    }
                    printf("%s\n", "allocated ESP");
                    SHARED_MEMORY->ESP_ALLOCATED = true;
                });
            }
            if (NSApp)
            {
                if (NSApp.mainWindow)
                {
                    dispatch_async(dispatch_get_main_queue(), ^
                                   {
                        NSWindow* w = NSApp.windows[0];
                        NSView* v = w.contentView;
                        SHARED_MEMORY->WINDOW_W = v.frame.size.width;
                        SHARED_MEMORY->WINDOW_H = v.frame.size.height;
                    });
                }
            }
            if (SHARED_MEMORY_CREATED == false && SHOULD_CREATE_SHARED_MEMORY == true)
            {
                //Initiate the *Actually* shared memory, copy any stored data, and deallocate the old memory.
                void* om = SHARED_MEMORY;
                SHARED_MEMORY = init_shared_memory(SHARED_FILE_PATH, true);
                memcpy(SHARED_MEMORY, om, sizeof(libESPSharedMemoryObject));
                free(om);
                SHARED_MEMORY_CREATED = true;
            }
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (SHARED_MEMORY->ESP_ALLOCATED == true && SHARED_MEMORY->ESP_ENABLED == true)
            {
                dispatch_async(dispatch_get_main_queue(), ^
                               {
                    for (int i = 0 ; i < MAX_ESP_COUNT ; i++)
                    {
                        SHARED_MEMORY->ESP_BOX_HIDDEN_ARRAY[i] = true;
                    }
                    bzero(SHARED_MEMORY->ESP_BOX_TEXT_ARRAY, sizeof(SHARED_MEMORY->ESP_BOX_TEXT_ARRAY));
                });
            }
            sleep(SHARED_MEMORY->REFRESH_SLEEP_TIME);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (SHARED_MEMORY->ESP_ALLOCATED == true && SHARED_MEMORY->ESP_ENABLED == true)
            {
                dispatch_async(dispatch_get_main_queue(), ^
                {
                    for (int i = 0 ; i < SHARED_MEMORY->ESP_COUNT ; i++)
                    {
                        if (i < MAX_ESP_COUNT)
                        {
                            NSTextField* esp_box = (__bridge NSTextField*)(SHARED_MEMORY->ESP_BOX_ARRAY[i]);
                            ESP_Frame espbf = SHARED_MEMORY->ESP_BOX_FRAME_ARRAY[i];
                            ESP_Color espbc = SHARED_MEMORY->ESP_BOX_COLOR_ARRAY[i];
                            
                            NSRect frame;
                            frame.origin.x = espbf.x;
                            frame.origin.y = espbf.y;
                            frame.size.width = espbf.w;
                            frame.size.height = espbf.h;
                            
                            esp_box.frame = frame;
                            esp_box.hidden = SHARED_MEMORY->ESP_BOX_HIDDEN_ARRAY[i];
                            char* c_str = (char*)((vm_address_t)SHARED_MEMORY->ESP_BOX_TEXT_ARRAY + (i * MAX_ESP_TEXT_LENGTH));
                            NSString* objc_str = [NSString stringWithUTF8String:c_str];
                            esp_box.stringValue = objc_str;
                            
                            CGColorRef cg_color = CGColorCreateSRGB(espbc.r, espbc.g, espbc.b, espbc.a);
                            NSColor* ns_color = [NSColor colorWithCGColor:cg_color];
                            esp_box.layer.borderColor = cg_color;
                            esp_box.textColor = ns_color;
                            esp_box.layer.borderWidth = SHARED_MEMORY->ESP_BOX_BORDER_WIDTH_ARRAY[i];
                            
                            CGColorRelease(cg_color);
                        }
                    }
                });
            }
            usleep(SHARED_MEMORY->ESP_USLEEP_TIME);
        }
    });
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(SHARED_MEMORY->INPUT_USLEEP_TIME);
            if (NSApp.isActive)
            {
                if (SHARED_MEMORY->INPUT_QUEUE_FINISHED == false)
                {
                    for (int i = 0 ; i < SHARED_MEMORY->INPUT_QUEUE_COUNT ; i++)
                    {
                        if (i < MAX_INPUT_COUNT)
                        {
                            Input input = SHARED_MEMORY->INPUT_QUEUE[i];
                            if (input.should_execute == true)
                            {
                                if (input.type == INPUT_TYPE_MOUSE_MOVE)
                                {
                                    CGPoint point = CGPointMake(input.x, NSApp.windows[input.window_index].contentView.frame.size.height - input.y + [NSApp.windows[input.window_index] titlebarHeight]);
                                    moveMouseOnWindow(NSApp.windows[input.window_index], point);
                                    [NSApp.windows[input.window_index] sendEvent:[NSEvent mouseEventWithType:NSEventTypeMouseMoved location:NSPointFromCGPoint(point) modifierFlags:0 timestamp:0 windowNumber:0 context:NULL eventNumber:0 clickCount:0 pressure:0]];
                                }
                                if (input.type == INPUT_TYPE_KEY_DOWN)
                                {
                                    sendKeyDown(input.window_index, input.keycode, input.characters);
                                }
                                if (input.type == INPUT_TYPE_KEY_UP)
                                {
                                    sendKeyUp(input.window_index, input.keycode, input.characters);
                                }
                                if (input.type == INPUT_TYPE_LEFT_MOUSE_DOWN)
                                {
                                    CGPoint point = CGPointMake(input.x, NSApp.windows[input.window_index].contentView.frame.size.height - input.y + [NSApp.windows[input.window_index] titlebarHeight]);
                                    sendLeftMouseDown(input.window_index, point);
                                }
                                if (input.type == INPUT_TYPE_LEFT_MOUSE_UP)
                                {
                                    CGPoint point = CGPointMake(input.x, NSApp.windows[input.window_index].contentView.frame.size.height - input.y + [NSApp.windows[input.window_index] titlebarHeight]);
                                    sendLeftMouseUp(input.window_index, point);
                                }
                                SHARED_MEMORY->INPUT_QUEUE[i].should_execute = false;
                            }
                        }
                    }
                    SHARED_MEMORY->INPUT_QUEUE_FINISHED = true;
                }
            }
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(SHARED_MEMORY->FUNCTION_USLEEP_TIME);
            if (SHARED_MEMORY->FUNCTION_QUEUE_FINISHED == false)
            {
                for (int i = 0 ; i < SHARED_MEMORY->FUNCTION_QUEUE_COUNT ; i++)
                {
                    if (i < MAX_FUNCTION_COUNT)
                    {
                        char at = SHARED_MEMORY->FUNCTION_QUEUE[i].async_type;
                        if (at == FUNCTION_ASYNC_TYPE_MAIN)
                        {
                            dispatch_async(dispatch_get_main_queue(), ^{
                                EXECUTE_FUNCTION(i);
                            });
                        }
                        if (at == FUNCTION_ASYNC_TYPE_SEPERATE)
                        {
                            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                            {
                                EXECUTE_FUNCTION(i);
                            });
                        }
                        if (at == FUNCTION_ASYNC_TYPE_CURRENT)
                        {
                            EXECUTE_FUNCTION(i);
                        }
                    }
                }
                SHARED_MEMORY->FUNCTION_QUEUE_FINISHED = true;
            }
        }
    });
    
    
    
}

