//
//  ESP.m
//  ESP
//
//  Created by Dimitri Armendariz on 9/13/22.
//

#import "ESP.h"


const char* LIB_SIG = "DYLIB.__libESP__.SIGNATURE";
extern const char* LIB_SIG;


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

typedef struct
{
    int type;
    int keycode;
    float x;
    float y;
    char characters[8];
    int duration;
    int window_index;
}
Input;

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
 void pushESP_Text(ESP_Info* info)
 {
 __block NSTextField* tf;
 dispatch_async(dispatch_get_main_queue(), ^
 {
 tf = [[NSTextField alloc] init];
 tf.frame = NSMakeRect(0, 0, 0, 0);
 tf.lineBreakMode = NSLineBreakByClipping;
 tf.editable = NO;
 tf.hidden = YES;
 tf.selectable = NO;
 tf.alignment = NSTextAlignmentCenter;
 tf.drawsBackground = NO;
 tf.bezeled = NO;
 tf.stringValue = @"";
 [tf setFont: [NSFont fontWithName:@"Menlo Regular" size:15]];
 tf.wantsLayer = YES;
 //tf.textColor = NSColor.redColor;
 [info -> target_view addSubview:tf];
 info -> esp_box_array[info -> esp_count] = (__bridge void *)(tf);
 info -> esp_count += 1;
 });
 }
 */

unsigned char ESP_ENABLED;
extern unsigned char ESP_ENABLED;

unsigned char AIMBOT_ENABLED;
extern unsigned char AIMBOT_ENABLED;

unsigned char ESP_ALLOCATED;
extern unsigned char ESP_ALLOCATED;


#define MAX_ESP_COUNT 100


float AIMBOT_X;
extern float AIMBOT_X;

float AIMBOT_Y;
extern float AIMBOT_Y;

float WINDOW_W;
extern float WINDOW_W;

float WINDOW_H;
extern float WINDOW_H;




void* ESP_BOX_ARRAY[MAX_ESP_COUNT];

ESP_Frame ESP_BOX_FRAME_ARRAY[MAX_ESP_COUNT];
extern ESP_Frame ESP_BOX_FRAME_ARRAY[MAX_ESP_COUNT];

ESP_Color ESP_BOX_COLOR_ARRAY[MAX_ESP_COUNT];
extern ESP_Color ESP_BOX_COLOR_ARRAY[MAX_ESP_COUNT];

unsigned char ESP_BOX_HIDDEN_ARRAY[MAX_ESP_COUNT];
extern unsigned char ESP_BOX_HIDDEN_ARRAY[MAX_ESP_COUNT];




useconds_t ESP_USLEEP_TIME = 30000;
extern useconds_t ESP_USLEEP_TIME;

useconds_t AIMBOT_USLEEP_TIME = 30000;
extern useconds_t AIMBOT_USLEEP_TIME;





unsigned char INPUT_QUEUE_FINISHED = true;
extern unsigned char INPUT_QUEUE_FINISHED;

unsigned int INPUT_QUEUE_COUNT = 0;
extern unsigned int INPUT_QUEUE_COUNT;

Input INPUT_QUEUE[100];
extern Input INPUT_QUEUE[100];








__attribute__((constructor))
void initialize(void)
{
    printf("%p\n", &AIMBOT_ENABLED);
    printf("hello from libESP.dylib\n");
    

    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskLeftMouseDown handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        printf("%f, %f\n", event.locationInWindow.x, event.locationInWindow.y);
        return event;
    }];
        
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (ESP_ALLOCATED == false && ESP_ENABLED == true)
            {
                for (int i = 0 ; i < MAX_ESP_COUNT ; i++)
                {
                    ESP_BOX_HIDDEN_ARRAY[i] = true;
                    ESP_BOX_COLOR_ARRAY[i].r = 255.0f;
                    ESP_BOX_COLOR_ARRAY[i].a = 1.0f;
                    
                    NSView* esp_box;
                    esp_box = [[NSView alloc] init];
                    esp_box.wantsLayer = YES;
                    esp_box.layer.opacity = 1;
                    esp_box.hidden = YES;
                    esp_box.layer.cornerRadius = 3;
                    esp_box.layer.borderWidth = 2;
                    [NSApp.windows[0].contentView.subviews[0] addSubview:esp_box];
                    ESP_BOX_ARRAY[i] = (__bridge void *)(esp_box);
                }
                printf("%s\n", "allocated ESP");
                ESP_ALLOCATED = true;
            }
            if (ESP_ALLOCATED == true && ESP_ENABLED == true)
            {
                dispatch_async(dispatch_get_main_queue(), ^
                               {
                    for (int i = 0 ; i < MAX_ESP_COUNT ; i++)
                    {
                        ESP_BOX_HIDDEN_ARRAY[i] = true;
                    }
                });
            }
            if (AIMBOT_ENABLED == true || ESP_ENABLED == true)
            {
                dispatch_async(dispatch_get_main_queue(), ^
                               {
                    NSWindow* w = NSApp.windows[0];
                    NSView* v = w.contentView;
                    WINDOW_W = v.frame.size.width;
                    WINDOW_H = v.frame.size.height;
                });
            }
            sleep(1);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (ESP_ALLOCATED == true && ESP_ENABLED == true)
            {
                dispatch_async(dispatch_get_main_queue(), ^
                {
                    for (int i = 0 ; i < MAX_ESP_COUNT ; i++)
                    {
                        NSView* esp_box = (__bridge NSView*)(ESP_BOX_ARRAY[i]);
                        ESP_Frame espbf = ESP_BOX_FRAME_ARRAY[i];
                        ESP_Color espbc = ESP_BOX_COLOR_ARRAY[i];
                        
                        esp_box.frame = NSMakeRect(espbf.x, espbf.y, espbf.w, espbf.h);
                        esp_box.hidden = ESP_BOX_HIDDEN_ARRAY[i];
                        esp_box.layer.borderColor = CGColorCreateSRGB(espbc.r, espbc.g, espbc.b, espbc.a);
                    }
                });
            }
            usleep(ESP_USLEEP_TIME);
        }
    });
    
    
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(1000);
            if (INPUT_QUEUE_FINISHED == false)
            {
                for (int i = 0 ; i < INPUT_QUEUE_COUNT ; i++)
                {
                    if (NSApp.isActive)
                    {
                        if (i < 100)
                        {
                            Input input = INPUT_QUEUE[i];
                            if (input.type == 0)
                            {
                                CGPoint point = CGPointMake(input.x, NSApp.windows[input.window_index].contentView.frame.size.height - input.y + [NSApp.windows[input.window_index] titlebarHeight]);
                                moveMouseOnWindow(NSApp.windows[input.window_index], point);
                                [NSApp.windows[input.window_index] sendEvent:[NSEvent mouseEventWithType:NSEventTypeMouseMoved location:NSPointFromCGPoint(point) modifierFlags:0 timestamp:0 windowNumber:0 context:NULL eventNumber:0 clickCount:0 pressure:0]];
                            }
                            if (input.type == 1)
                            {
                                sendKeyDown(input.window_index, input.keycode, input.characters);
                            }
                            if (input.type == 2)
                            {
                                sendKeyUp(input.window_index, input.keycode, input.characters);
                            }
                            if (input.type == 3)
                            {
                                CGPoint point = CGPointMake(input.x, NSApp.windows[input.window_index].contentView.frame.size.height - input.y + [NSApp.windows[input.window_index] titlebarHeight]);
                                sendLeftMouseDown(input.window_index, point);
                            }
                            if (input.type == 4)
                            {
                                CGPoint point = CGPointMake(input.x, NSApp.windows[input.window_index].contentView.frame.size.height - input.y + [NSApp.windows[input.window_index] titlebarHeight]);
                                sendLeftMouseUp(input.window_index, point);
                            }
                            usleep(input.duration);
                        }
                    }
                }
                INPUT_QUEUE_COUNT = 0;
                INPUT_QUEUE_FINISHED = true;
            }
        }
    });
    
    
    
    
    
    
}

