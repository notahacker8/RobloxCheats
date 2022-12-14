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






unsigned char ESP_ENABLED;
extern unsigned char ESP_ENABLED;

unsigned char AIMBOT_ENABLED;
extern unsigned char AIMBOT_ENABLED;

unsigned char ESP_ALLOCATED;
extern unsigned char ESP_ALLOCATED;


#define MAX_ESP_COUNT 400
#define MAX_INPUT_COUNT 100
#define MAX_ESP_TEXT_SIZE 50


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

float ESP_BOX_BORDER_WIDTH_ARRAY[MAX_ESP_COUNT];
extern float ESP_BOX_BORDER_WIDTH_ARRAY[MAX_ESP_COUNT];

char ESP_BOX_TEXT_ARRAY[MAX_ESP_COUNT * MAX_ESP_TEXT_SIZE];
extern char ESP_BOX_TEXT_ARRAY[MAX_ESP_COUNT * MAX_ESP_TEXT_SIZE];




useconds_t ESP_USLEEP_TIME = 100000;
extern useconds_t ESP_USLEEP_TIME;

useconds_t REFRESH_USLEEP_TIME = 5000000;
extern useconds_t REFRESH_USLEEP_TIME;

useconds_t AIMBOT_USLEEP_TIME = 300000;
extern useconds_t AIMBOT_USLEEP_TIME;

useconds_t INPUT_USLEEP_TIME = 300000;
extern useconds_t INPUT_USLEEP_TIME;





unsigned char INPUT_QUEUE_FINISHED = true;
extern unsigned char INPUT_QUEUE_FINISHED;

unsigned int INPUT_QUEUE_COUNT = 0;
extern unsigned int INPUT_QUEUE_COUNT;

Input INPUT_QUEUE[MAX_INPUT_COUNT];
extern Input INPUT_QUEUE[MAX_INPUT_COUNT];

char LEFT_MOUSE_DOWN = false;
extern char LEFT_MOUSE_DOWN;



__attribute__((constructor))
void initialize(void)
{
    
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskLeftMouseDown handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        //printf("%f, %f\n", event.locationInWindow.x, event.locationInWindow.y);
        LEFT_MOUSE_DOWN = true;
        return event;
    }];
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskLeftMouseUp handler:^NSEvent * _Nullable(NSEvent * _Nonnull event) {
        //printf("%f, %f\n", event.locationInWindow.x, event.locationInWindow.y);
        LEFT_MOUSE_DOWN = false;
        return event;
    }];
    printf("\n-> DYLIB INJECTED\n\n");
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (ESP_ALLOCATED == false && ESP_ENABLED == true)
            {
                dispatch_async(dispatch_get_main_queue(), ^
                {
                    for (int i = 0 ; i < MAX_ESP_COUNT ; i++)
                    {
                        ESP_BOX_HIDDEN_ARRAY[i] = true;
                        ESP_BOX_COLOR_ARRAY[i].r = 255.0f;
                        ESP_BOX_COLOR_ARRAY[i].a = 1.0f;
                        
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
                        ESP_BOX_ARRAY[i] = (__bridge void *)(esp_box);
                    }
                    printf("%s\n", "allocated ESP");
                    ESP_ALLOCATED = true;
                });
            }
            if (ESP_ALLOCATED == true && ESP_ENABLED == true)
            {
                dispatch_async(dispatch_get_main_queue(), ^
                {
                    for (int i = 0 ; i < MAX_ESP_COUNT ; i++)
                    {
                        ESP_BOX_HIDDEN_ARRAY[i] = true;
                    }
                    bzero(ESP_BOX_TEXT_ARRAY, sizeof(ESP_BOX_TEXT_ARRAY));
                });
            }
            if (AIMBOT_ENABLED == true || ESP_ENABLED == true || INPUT_QUEUE_FINISHED == false)
            {
                dispatch_async(dispatch_get_main_queue(), ^
                               {
                    NSWindow* w = NSApp.windows[0];
                    NSView* v = w.contentView;
                    WINDOW_W = v.frame.size.width;
                    WINDOW_H = v.frame.size.height;
                });
            }
            usleep(REFRESH_USLEEP_TIME);
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
                        
                        NSTextField* esp_box = (__bridge NSTextField*)(ESP_BOX_ARRAY[i]);
                        ESP_Frame espbf = ESP_BOX_FRAME_ARRAY[i];
                        ESP_Color espbc = ESP_BOX_COLOR_ARRAY[i];
                        
                        NSRect frame;
                        frame.origin.x = espbf.x;
                        frame.origin.y = espbf.y;
                        frame.size.width = espbf.w;
                        frame.size.height = espbf.h;
                        
                        esp_box.frame = frame;
                        esp_box.hidden = ESP_BOX_HIDDEN_ARRAY[i];
                        char* c_str = (char*)((vm_address_t)ESP_BOX_TEXT_ARRAY + (i * MAX_ESP_TEXT_SIZE));
                        NSString* objc_str = [NSString stringWithUTF8String:c_str];
                        esp_box.stringValue = objc_str;
                        
                        CGColorRef cg_color = CGColorCreateSRGB(espbc.r, espbc.g, espbc.b, espbc.a);
                        NSColor* ns_color = [NSColor colorWithCGColor:cg_color];
                        esp_box.layer.borderColor = cg_color;
                        esp_box.textColor = ns_color;
                        esp_box.layer.borderWidth = ESP_BOX_BORDER_WIDTH_ARRAY[i];
                        
                        CGColorRelease(cg_color);
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
            usleep(INPUT_USLEEP_TIME);
            if (NSApp.isActive)
            {
                //printf("queue finished: %d\n", INPUT_QUEUE_FINISHED);
                if (INPUT_QUEUE_FINISHED == false)
                {
                    for (int i = 0 ; i < INPUT_QUEUE_COUNT ; i++)
                    {
                        if (i < 100)
                        {
                            Input input = INPUT_QUEUE[i];
                            //printf("inputs[%d], input type: %d\n", i, input.type);
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
                    INPUT_QUEUE_COUNT = 0;
                    INPUT_QUEUE_FINISHED = true;
                }
            }
        }
    });
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(AIMBOT_USLEEP_TIME);
            if (NSApp.isActive)
            {
                if (AIMBOT_ENABLED)
                {
                    CGPoint point = CGPointMake(AIMBOT_X, NSApp.windows[0].contentView.frame.size.height - AIMBOT_Y + [NSApp.windows[0] titlebarHeight]);
                    moveMouseOnWindow(NSApp.windows[0], point);
                    [NSApp.windows[0] sendEvent:[NSEvent mouseEventWithType:NSEventTypeMouseMoved location:NSPointFromCGPoint(point) modifierFlags:0 timestamp:0 windowNumber:0 context:NULL eventNumber:0 clickCount:0 pressure:0]];
                }
            }
        }
    });
    
    
}

