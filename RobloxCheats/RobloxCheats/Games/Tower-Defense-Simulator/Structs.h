


typedef struct
{
    int type;
    vector3_t pos;
    int tower_number;
    int wait_time;
    char should_wait_until_skip_enabled;
    char should_wait_until_game_over;
    char should_wait_until_this_wave;
    char should_wait_until_this_cash;
    int tower_index;
    int wave;
    int cash;
    char should_skip_while_waiting;
}
rbx_tds_input_t;

#define RBX_TDS_INPUT_WAIT 0
#define RBX_TDS_INPUT_SKIP 1
#define RBX_TDS_INPUT_PLACE 2
#define RBX_TDS_INPUT_UPGRADE 4
