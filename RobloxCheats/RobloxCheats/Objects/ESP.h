
#define MAX_ESP_COUNT 400
#define MAX_INPUT_COUNT 100
#define MAX_ESP_TEXT_SIZE 50

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

