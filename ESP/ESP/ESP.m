//
//  ESP.h
//  ESP
//
//  Created by me on 9/13/22.
//



#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>
#import <objc/runtime.h>
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

void print_subviews(NSView* view, int tab_index)
{
    NSString* str = [NSString stringWithFormat:@"%@", view];
    for (int i = 0 ; i < tab_index ; i++)
    {
        printf("\t");
    }
    printf("%s\n", [str cStringUsingEncoding:NSASCIIStringEncoding]);
    NSArray* subviews = [view subviews];
    for (int i = 0 ; i < subviews.count ; i++)
    {
        for (int x = 0 ; x < tab_index + 1 ; x++)
        {
            printf("\t");
        }
        print_subviews(subviews[i], tab_index + 1);
    }
}


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
    //[window sendEvent: [NSEvent mouseEventWithType:NSEventTypeMouseMoved location:point modifierFlags:0 timestamp:0 windowNumber:0 context:NULL eventNumber:0 clickCount:0 pressure:0]];
}

void printSelectorsOfClass(Class cls) {
    unsigned int methodCount;
    Method *methods = class_copyMethodList(cls, &methodCount);
    
    for (unsigned int i = 0; i < methodCount; i++) {
        SEL selector = method_getName(methods[i]);
        const char *selectorName = sel_getName(selector);
        NSLog(@"%@", [NSString stringWithUTF8String:selectorName]);
    }
    
    free(methods);
}


void sendKeyDown(NSWindow* window, int keycode, char* characters)
{
    [window makeKeyAndOrderFront:nil];
    [window sendEvent:[NSEvent keyEventWithType:NSEventTypeKeyDown location:CGPointZero modifierFlags:0 timestamp:0 windowNumber:0 context:NULL characters:[NSString stringWithCString:characters encoding:NSStringEncodingConversionAllowLossy] charactersIgnoringModifiers:[NSString stringWithCString:characters encoding:NSStringEncodingConversionAllowLossy] isARepeat:FALSE keyCode:keycode]];
}
void sendKeyUp(NSWindow* window, int keycode, char* characters)
{
    [window makeKeyAndOrderFront:nil];
    [window sendEvent:[NSEvent keyEventWithType:NSEventTypeKeyUp location:CGPointZero modifierFlags:0 timestamp:0 windowNumber:0 context:NULL characters:[NSString stringWithCString:characters encoding:NSStringEncodingConversionAllowLossy] charactersIgnoringModifiers:[NSString stringWithCString:characters encoding:NSStringEncodingConversionAllowLossy] isARepeat:FALSE keyCode:keycode]];
    
}
void sendLeftMouseDown(NSWindow* window, CGPoint point)
{
    [window makeKeyAndOrderFront:nil];
    [window sendEvent: [NSEvent mouseEventWithType:NSEventTypeLeftMouseDown location:point modifierFlags:0 timestamp:0 windowNumber:0 context:NULL eventNumber:0 clickCount:1 pressure:1]];
}
void sendLeftMouseUp(NSWindow* window, CGPoint point)
{
    [window makeKeyAndOrderFront:nil];
    [window sendEvent: [NSEvent mouseEventWithType:NSEventTypeLeftMouseUp location:point modifierFlags:0 timestamp:0 windowNumber:0 context:NULL eventNumber:0 clickCount:1 pressure:1]];
    //CGEventRef mouseDownEvent = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, point, kCGMouseButtonLeft);
    //CGEventPost(kCGEventSourceStateHIDSystemState, mouseDownEvent);
    //CFRelease(mouseDownEvent);
}



char SHARED_FILE_PATH[PATH_MAX];
char SHOULD_CREATE_SHARED_MEMORY = false;
char SHARED_MEMORY_CREATED = false;
libESPSharedMemoryObject* SHARED_MEMORY;



void EXECUTE_FUNCTION(int i)
{
    RemoteFunctionCall func = SHARED_MEMORY->FUNCTION_QUEUE[i];
    char* args = func.arguments;
    if (func.should_execute == true)
    {
        if (func.type == 0)
        {
            printf("%p()\n", func.address);
            ((r_void_p_void_f*)func.address)();
        }
        if (func.type == 1)
        {
            long arg1 = *((long*)(args));
            printf("%p(%p)\n", func.address, arg1);
            ((r_void_p_long_f*)func.address)(arg1);
        }
        if (func.type == 2)
        {
            long arg1 = *((long*)(args));
            long arg2 = *((long*)(args + 8));
            printf("%p(%p, %p)\n", func.address, arg1, arg2);
            ((r_void_p_long_long_f*)func.address)(arg1, arg2);
        }
        if (func.type == 4)
        {
            long arg1 = *((long*)(args));
            long arg2 = *((long*)(args + 8));
            long arg3 = *((long*)(args + 16));
            long arg4 = *((long*)(args + 24));
            printf("%p(%p, %p, %p, %p)\n", func.address, arg1, arg2, arg3, arg4);
            ((r_void_p_long_long_long_long_f*)func.address)(arg1, arg2, arg3, arg4);
        }
        if (func.type == 7)
        {
            long arg1 = *((long*)(args));
            float arg2 = *((float*)(args + 8));
            printf("%p(%p, %f)\n", func.address, arg1, arg2);
            ((r_void_p_long_float_f*)func.address)(arg1, arg2);
        }
        func.should_execute = false;
        SHARED_MEMORY->FUNCTION_QUEUE[i] = func;
    }
}



void SIGTRAP_HANDLER(int signum, siginfo_t* signal_info, void* context)
{
    ucontext_t *ucontext = (ucontext_t *)context;
    _STRUCT_MCONTEXT64 mctx = *(ucontext->uc_mcontext);
    for (int i = 0 ; i < (sizeof(SHARED_MEMORY->BREAKPOINTS)/sizeof(ESP_BKPT)) ; i++)
    {
        if (SHARED_MEMORY->BREAKPOINTS[i].trip_address == mctx.__ss.__pc)
        {
            if (SHARED_MEMORY->BREAKPOINTS[i].hit == false)
            {
                SHARED_MEMORY->BREAKPOINTS[i].hit = true;
                memcpy(&SHARED_MEMORY->BREAKPOINTS[i].MCTX, &mctx, sizeof(mctx));
            }
            if (SHARED_MEMORY->BREAKPOINTS[i].hit == true)
            {
                memcpy(ucontext->uc_mcontext, &SHARED_MEMORY->BREAKPOINTS[i].MCTX, sizeof(mctx));
            }
            i = (sizeof(SHARED_MEMORY->BREAKPOINTS)/sizeof(ESP_BKPT));
        }
    }
}



__attribute__((constructor))
void initialize(void)
{
    {
        struct sigaction action;
        memset(&action, 0, sizeof(action));
        sigemptyset(&action.sa_mask);
        action.sa_flags = SA_SIGINFO;
        action.sa_sigaction = &SIGTRAP_HANDLER;
        sigaction(SIGTRAP, &action, NULL);
    }
    
    //Create unshared memory to give the functions something to write to.
    //This will be deallocated once real shared memory is created
    SHARED_MEMORY = malloc(sizeof(libESPSharedMemoryObject));
    bzero(SHARED_MEMORY, sizeof(libESPSharedMemoryObject));
    
    SHARED_MEMORY->ESP_COUNT = 0;
    SHARED_MEMORY->ESP_USLEEP_TIME = 1000000;
    SHARED_MEMORY->REFRESH_SLEEP_TIME = 5;
    SHARED_MEMORY->ESP_TEXT_FONT_SIZE = 12;

    SHARED_MEMORY->INPUT_COUNT = 0;
    SHARED_MEMORY->INPUT_USLEEP_TIME = 1000000;

    SHARED_MEMORY->FUNCTION_COUNT = 0;
    SHARED_MEMORY->FUNCTION_USLEEP_TIME = 1000000;
    
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskMouseMoved handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        SHARED_MEMORY->MOUSE_X = event.locationInWindow.x;
        SHARED_MEMORY->MOUSE_Y = event.locationInWindow.y;
        //printf("mouse move: %f, %f\n", SHARED_MEMORY->MOUSE_X, SHARED_MEMORY->MOUSE_Y);
        return event;
    }];
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskLeftMouseDown handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        SHARED_MEMORY->LEFT_MOUSE_DOWN = true;
        //printf("mouse move: %f, %f\n", SHARED_MEMORY->MOUSE_X, SHARED_MEMORY->MOUSE_Y);
        return event;
    }];
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskLeftMouseUp handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        SHARED_MEMORY->LEFT_MOUSE_DOWN = false;
        return event;
    }];
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskRightMouseDown handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        SHARED_MEMORY->RIGHT_MOUSE_DOWN = true;
        //printf("mouse move: %f, %f\n", SHARED_MEMORY->MOUSE_X, SHARED_MEMORY->MOUSE_Y);
        return event;
    }];
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskRightMouseUp handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        SHARED_MEMORY->RIGHT_MOUSE_DOWN = false;
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
            if (NSApp)
            {
                dispatch_async(dispatch_get_main_queue(), ^{
                    NSWindow* w = NSApp.mainWindow;
                    if (w)
                    {
                        //sprintf(SHARED_MEMORY->NEW_MAIN_WINDOW_TITLE, "%s - [%d]", NSApp.menu.title, getpid());
                        w.title = [NSString stringWithFormat:@"%@ - [%d]", NSApp.mainMenu.itemArray[0].title, getpid()];
                        [NSApp.dockTile setBadgeLabel:[NSString stringWithFormat:@"%s", SHARED_MEMORY->DOCKTILE_BADGELABEL_TEXT]];
                    }
                });
                SHARED_MEMORY->APP_IS_ACTIVE = NSApp.isActive;
            }
            sleep(1);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (SHARED_MEMORY->ESP_ALLOCATED == false && SHARED_MEMORY->ESP_ENABLED == true)
            {
                NSView* rbx_ogre_view = NSApp.windows[0].contentView.subviews[0];
                //NSLog(@"ogre size: %f, %f", rbx_ogre_view.frame.size.width, rbx_ogre_view.frame.size.height);
                NSView* esp_v = [[NSView alloc] init];
                esp_v.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
                esp_v.frame = rbx_ogre_view.frame;
                //NSLog(@"v size: %f, %f", esp_v.frame.size.width, esp_v.frame.size.height);
                [rbx_ogre_view addSubview:esp_v];
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
                        esp_box.enabled = NO;
                        [esp_box setFont: [NSFont fontWithName:[NSString stringWithUTF8String:SHARED_MEMORY->ESP_TEXT_FONT_NAME] size:SHARED_MEMORY->ESP_TEXT_FONT_SIZE]];
                        [esp_v addSubview:esp_box];
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
                        
                        SHARED_MEMORY->WINDOW_X = w.frame.origin.x;
                        SHARED_MEMORY->WINDOW_Y = w.frame.origin.y;
                        //print_subviews(v.subviews[0], 0);
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
                            
                            ESP_Color espbc = SHARED_MEMORY->ESP_BOX_COLOR_ARRAY[i];
                            char* c_str = (char*)((vm_address_t)SHARED_MEMORY->ESP_BOX_TEXT_ARRAY + (i * MAX_ESP_TEXT_LENGTH));
                            NSString* objc_str = [NSString stringWithUTF8String:c_str];
                            esp_box.stringValue = objc_str;
                            
                            CGColorRef cg_color = CGColorCreateSRGB(espbc.r, espbc.g, espbc.b, espbc.a);
                            NSColor* ns_color = [NSColor colorWithCGColor:cg_color];
                            
                            //CGColorRef cg_color2 = CGColorCreateSRGB(1, 1, 1, espbc.a/2);
                            esp_box.layer.borderColor = cg_color;
                            //esp_box.layer.backgroundColor = cg_color2;
                            esp_box.textColor = ns_color;
                            
                            NSRect espbf = SHARED_MEMORY->ESP_BOX_FRAME_ARRAY[i];
                            esp_box.frame = espbf;
                            esp_box.hidden = SHARED_MEMORY->ESP_BOX_HIDDEN_ARRAY[i];
                            esp_box.layer.borderWidth = SHARED_MEMORY->ESP_BOX_BORDER_WIDTH_ARRAY[i];
                            
                            CGColorRelease(cg_color);
                            //CGColorRelease(cg_color2);
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
            if (SHARED_MEMORY->ESP_ALLOCATED == true && SHARED_MEMORY->ESP_ENABLED == true)
            {
                dispatch_async(dispatch_get_main_queue(), ^
                {
                    for (int i = 0 ; i < SHARED_MEMORY->ESP_COUNT ; i++)
                    {
                        if (i < MAX_ESP_COUNT)
                        {
                            NSTextField* esp_box = (__bridge NSTextField*)(SHARED_MEMORY->ESP_BOX_ARRAY[i]);
                            [esp_box setFont: [NSFont fontWithName:[NSString stringWithUTF8String:SHARED_MEMORY->ESP_TEXT_FONT_NAME] size:SHARED_MEMORY->ESP_TEXT_FONT_SIZE]];
                                                        
                        }
                    }
                });
            }
            sleep(1);
        }
    });

    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(SHARED_MEMORY->INPUT_USLEEP_TIME);
            if (NSApp.isActive)
            {
                for (int i = 0 ; i <  SHARED_MEMORY->INPUT_COUNT ; i++)
                {
                    if (i < MAX_INPUT_COUNT)
                    {
                        Input input = SHARED_MEMORY->INPUT_QUEUE[i];
                        if (input.should_execute == true)
                        {
                            if (input.type == INPUT_TYPE_MOUSE_MOVE)
                            {
                                CGPoint point = CGPointMake(input.x, NSApp.mainWindow.contentView.frame.size.height - input.y + [NSApp.mainWindow titlebarHeight]);
                                moveMouseOnWindow(NSApp.mainWindow, point);
                                NSEvent *mouseEvent = [NSEvent mouseEventWithType:NSEventTypeMouseMoved location:NSPointFromCGPoint(point) modifierFlags:0 timestamp:0 windowNumber:0 context:NULL eventNumber:0 clickCount:0 pressure:0];
                                
                                [NSApp.mainWindow sendEvent:mouseEvent];
                                //printSelectorsOfClass([NSWindow class]);
                                //[NSApp.mainWindow performSelector:@selector(_handleMouseMoved:) withObject:mouseEvent];
                            }
                            if (input.type == INPUT_TYPE_KEY_DOWN)
                            {
                                sendKeyDown(NSApp.mainWindow, input.keycode, input.characters);
                            }
                            if (input.type == INPUT_TYPE_KEY_UP)
                            {
                                sendKeyUp(NSApp.mainWindow, input.keycode, input.characters);
                            }
                            if (input.type == INPUT_TYPE_LEFT_MOUSE_DOWN)
                            {
                                CGPoint point = CGPointMake(input.x, NSApp.mainWindow.contentView.frame.size.height - input.y + [NSApp.mainWindow titlebarHeight]);
                                sendLeftMouseDown(NSApp.mainWindow, point);
                            }
                            if (input.type == INPUT_TYPE_LEFT_MOUSE_UP)
                            {
                                CGPoint point = CGPointMake(input.x, NSApp.mainWindow.contentView.frame.size.height - input.y + [NSApp.mainWindow titlebarHeight]);
                                sendLeftMouseUp(NSApp.mainWindow, point);
                            }
                            SHARED_MEMORY->INPUT_QUEUE[i].should_execute = false;
                        }
                    }
                }
            }
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(SHARED_MEMORY->FUNCTION_USLEEP_TIME);
            for (int i = 0 ; i < SHARED_MEMORY->FUNCTION_COUNT ; i++)
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
        }
    });
    
    strcpy(SHARED_MEMORY->DOCKTILE_BADGELABEL_TEXT, ".dylib");
    
}
