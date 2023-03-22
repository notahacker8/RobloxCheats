
#define MAX_ESP_COUNT 200
#define MAX_INPUT_COUNT 100
#define MAX_FUNCTION_COUNT 100
#define MAX_ESP_TEXT_LENGTH 50

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

typedef struct
{
    int type;
    vm_address_t address;
    char arguments[8 * 6];
    char return_bytes[8];
    int finished;
}
RemoteFunctionCall;


