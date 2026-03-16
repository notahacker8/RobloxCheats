

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>
#include "ESP.h"


NSWindow* create_external_overlay_window(libESPSharedMemoryObject* shared_memory)
{
    
    NSRect screenFrame = [NSScreen mainScreen].frame;
    NSWindow *overlayWindow = [[NSWindow alloc] initWithContentRect:screenFrame
                                                          styleMask:NSWindowStyleMaskBorderless
                                                            backing:NSBackingStoreBuffered
                                                              defer:NO];
    overlayWindow.backgroundColor = [NSColor clearColor];
    overlayWindow.opaque = NO;
    overlayWindow.level = NSStatusWindowLevel + 1; // above normal windows
    overlayWindow.ignoresMouseEvents = YES;         // passes clicks through
    overlayWindow.hasShadow = NO;
    overlayWindow.collectionBehavior = NSWindowCollectionBehaviorCanJoinAllSpaces |
                                       NSWindowCollectionBehaviorFullScreenAuxiliary;

    overlayWindow.contentView.wantsLayer = YES;
    overlayWindow.contentView.layer.frame = overlayWindow.contentView.bounds;
    overlayWindow.contentView.layer.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;

    [overlayWindow makeKeyAndOrderFront:nil];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            CGFloat x = shared_memory->WINDOW_X;
            CGFloat y = shared_memory->WINDOW_Y;
            CGFloat width = shared_memory->WINDOW_W;
            CGFloat height = shared_memory->WINDOW_H;
            NSRect rect = NSMakeRect(x, y, width, height);
            //NSLog(@"%@", roblox_window);
            dispatch_async(dispatch_get_main_queue(), ^{
                [overlayWindow setFrame:rect display:true];
            });
            usleep(500000);
        }
    });
    return overlayWindow;
}


void EXTERNAL_ESP_ENGINE(libESPSharedMemoryObject* shared_memory)
{
    static CATextLayer* EXTERNAL_ESP_BOX_ARRAY[MAX_ESP_COUNT];
    static NSWindow* external_overlay_window = NULL;
    
    external_overlay_window = create_external_overlay_window(shared_memory);
    for (int i = 0; i < MAX_ESP_COUNT; i++)
    {
        CATextLayer *text = [CATextLayer layer];
        text.string = @"";
        text.frame = NSMakeRect(0, 0, 100, 100);
        
        text.actions = @{
            @"position": [NSNull null],
            @"bounds": [NSNull null],
            @"frame": [NSNull null],
            @"opacity": [NSNull null],
            @"backgroundColor": [NSNull null]
        };
        
        text.alignmentMode = kCAAlignmentCenter;
        text.contentsScale = NSScreen.mainScreen.backingScaleFactor;
        text.hidden = YES;
        text.cornerRadius = 2;
        NSFont *font = [NSFont fontWithName: [NSString stringWithUTF8String:shared_memory->ESP_TEXT_FONT_NAME] size:shared_memory->ESP_TEXT_FONT_SIZE];

        text.font = (__bridge CFTypeRef)font;
        text.fontSize = shared_memory->ESP_TEXT_FONT_SIZE;
        text.drawsAsynchronously = YES;

        [external_overlay_window.contentView.layer addSublayer:text];

        EXTERNAL_ESP_BOX_ARRAY[i] = (text);
    }
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            
            dispatch_async(dispatch_get_main_queue(), ^
                           {
                @autoreleasepool {
                    for (int i = 0 ; i < MAX_ESP_COUNT ; i++)
                    {
                        CATextLayer *esp_box = EXTERNAL_ESP_BOX_ARRAY[i];
                        
                        ESP_Color espbc = shared_memory->ESP_BOX_COLOR_ARRAY[i];
                        char* c_str = (char*)((vm_address_t)shared_memory->ESP_BOX_TEXT_ARRAY + (i * MAX_ESP_TEXT_LENGTH));
                        NSString* objc_str = [NSString stringWithUTF8String:c_str];
                        esp_box.string = objc_str;
                        
                        CGColorRef cg_color = CGColorCreateSRGB(espbc.r, espbc.g, espbc.b, espbc.a);
                        
                        esp_box.borderColor = cg_color;
                        esp_box.foregroundColor = cg_color;
                        esp_box.borderWidth = shared_memory->ESP_BOX_BORDER_WIDTH_ARRAY[i];
                        
                        NSRect espbf = shared_memory->ESP_BOX_FRAME_ARRAY[i];
                        if (!shared_memory->APP_IS_ACTIVE ||
                            fabs(espbf.size.width) > fabs(shared_memory->WINDOW_W) ||
                            fabs(espbf.size.height) > fabs(shared_memory->WINDOW_H) ||
                            fabs(espbf.origin.x) > fabs(shared_memory->WINDOW_W * 2) ||
                            fabs(espbf.origin.y) > fabs(shared_memory->WINDOW_H * 2))
                        {
                            esp_box.frame = NSMakeRect(0, 0, 100, 100);
                            esp_box.hidden = true;
                        }
                        else
                        {
                            esp_box.frame = espbf;
                            esp_box.hidden = shared_memory->ESP_BOX_HIDDEN_ARRAY[i];
                        }
                        
                        
                        CGColorRelease(cg_color);
                    }
                }
            });
            usleep(shared_memory->ESP_USLEEP_TIME);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            @autoreleasepool
            {
                
                dispatch_async(dispatch_get_main_queue(), ^
                               {
                    @autoreleasepool {
                        for (int i = 0 ; i < MAX_ESP_COUNT ; i++)
                        {
                            shared_memory->ESP_BOX_HIDDEN_ARRAY[i] = true;
                        }
                        bzero(shared_memory->ESP_BOX_TEXT_ARRAY, sizeof(shared_memory->ESP_BOX_TEXT_ARRAY));
                    }
                });
                
            }
            sleep(shared_memory->REFRESH_SLEEP_TIME);
        }
    });
    
}

int main(int argc, const char * argv[]) {
    if (argc < 4)
    {
        printf("args format: [roblox-pipe.txt path] [cheats pid] [roblox pid]\n");
        exit(0);
    }
    char* fp = argv[1];
    pid_t cheats_pid = atoi(argv[2]);
    pid_t rbx_pid = atoi(argv[3]);
    libESPSharedMemoryObject* shared_memory = init_shared_memory(fp, false);
    EXTERNAL_ESP_ENGINE(shared_memory);
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        for (;;)
        {
            if (!(kill(cheats_pid, 0) == 0) || !(kill(rbx_pid, 0) == 0))
            {
                printf("cheats exited, exiting as well\n");
                exit(0);
            }
            sleep(1);
        }
    });
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
    }
    return NSApplicationMain(argc, argv);
}
