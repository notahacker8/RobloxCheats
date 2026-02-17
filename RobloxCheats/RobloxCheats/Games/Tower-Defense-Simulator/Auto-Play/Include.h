

#define RBX_TDS_AUTOFARM_TYPE_MOLTEN_WITH_SCOUTS_DEATH_FARM 1
#define RBX_TDS_AUTOFARM_TYPE_MOLTEN_WITH_SHOTGUNNERS_DEATH_FARM 2
#define RBX_TDS_AUTOFARM_TYPE_HARDCORE_DEATH_FARM 3

#define RBX_TDS_MOLTEN_DIFFICULTY 1
#define RBX_TDS_EASY_DIFFICULTY 2

#define RBX_TDS_HARDCORE_MODE 1
#define RBX_TDS_SURVIVAL_MODE 2

#define RBX_TDS_LOBBY_PLACE_ID 3260590327
#define RBX_TDS_LOBBY_PLACE_ID_STR "3260590327"

#define RBX_TDS_EXIT_CODE_ROUND_ENDED_TO_INTERMISSION 10
#define RBX_TDS_EXIT_CODE_ROUND_ENDED_TO_MAIN_LOBBY 20
#define RBX_TDS_EXIT_CODE_GAME_TERMINATED 30


typedef struct
{
    int max_lvl;
    int lvl_costs[10]; //10 cuz i dont think a tower goes over this
}
tower_upgrade_info_t;


int sum_from_to(int *arr, int start, int end) {
    int sum = 0;
    for (int i = start; i <= end; i++)
        sum += arr[i];
    return sum;
}

typedef struct
{
    int type;
    vector3_t pos;
}
tds_tower_placement_t;

typedef struct
{
    long autofarm_type;
    tds_tower_placement_t* t_placements;
    char should_return_to_lobby;
    char should_exit_when_finished;
    char is_golden_scout;
    char auto_get_map_placements;
    void** map_dict;
    long map_count;
    char auto_set_prio;
}
tds_autoplay_options_t;

typedef struct
{
    long mode;
    long difficulty;
    char auto_set_prio;
}
tds_main_lobby_options_t;

typedef struct
{
    void** map_dict;
    long map_count;
    char** modifiers_list;
    int modif_count;
    char auto_set_prio;
    char close_game_on_failure;
}
tds_intermission_lobby_options_t;


typedef struct
{
    int level;
}
tds_tracked_tower_data_t;



#pragma mark - PLACEMENT NOTES -

/*
 
 molten with scout: 16 golden scouts (modifiers: glass, hidden, limitation, exploding, committed, nimble)
Normal scout works too
 
 molten with shotgunner: 10 shotgunners, 2 freezers
 
 hardcore: 1 crook boss, 7 trapper
 
 */

#pragma mark - MOLTEN WITH SHOTGUNNER -


tds_tower_placement_t molten_with_shotgunners_spring_fever_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -10.449526, .y = 2.372939, .z = -28.242123}},    // [0] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -7.674055, .y = 2.372939, .z = -33.588776}},    // [1] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -8.348001, .y = 2.372939, .z = -28.417494}},    // [2] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -9.712662, .y = 2.372939, .z = -33.521935}},    // [3] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -10.466976, .y = 2.372939, .z = -21.635031}},    // [4] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -9.301279, .y = 2.372939, .z = -26.579056}},    // [5] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -7.277054, .y = 2.372939, .z = -26.685715}},    // [6] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -6.109033, .y = 2.372939, .z = -28.469065}},    // [7] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -8.228934, .y = 2.372939, .z = -24.817966}},    // [8] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -6.163284, .y = 2.372939, .z = -25.003160}},    // [9] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = -5.838981, .y = 2.372939, .z = -21.702497}},    // [10] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = -2.958400, .y = 2.349997, .z = -22.634079}},    // [11] Default
};

tds_tower_placement_t molten_with_shotgunners_toyboard_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.975733, .y = 16.088120, .z = 2.098063}},    // [0] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 15.079971, .y = 16.088120, .z = 4.149210}},    // [1] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.878760, .y = 16.088120, .z = 2.063942}},    // [2] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 13.011637, .y = 16.088120, .z = 4.066963}},    // [3] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 10.862361, .y = 16.088120, .z = 2.030276}},    // [4] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 10.924421, .y = 16.088120, .z = 4.047975}},    // [5] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 13.952244, .y = 16.088120, .z = 5.924443}},    // [6] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 11.878593, .y = 16.088120, .z = 5.843281}},    // [7] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.849299, .y = 16.088120, .z = 5.762335}},    // [8] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.861738, .y = 16.088120, .z = 7.637492}},    // [9] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 15.119858, .y = 16.088120, .z = 12.540983}},    // [10] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 15.148066, .y = 16.088120, .z = 9.441119}},    // [11] Default
};

tds_tower_placement_t molten_with_shotgunners_winter_bridges_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 19.947189, .y = 2.350005, .z = -27.846851}},    // [0] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 17.856804, .y = 2.350005, .z = -27.986788}},    // [1] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 15.827189, .y = 2.350005, .z = -27.969316}},    // [2] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 18.837292, .y = 2.350004, .z = -26.137629}},    // [3] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 16.747952, .y = 2.350004, .z = -26.076674}},    // [4] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.731144, .y = 2.350004, .z = -26.290260}},    // [5] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 17.877357, .y = 2.350004, .z = -24.373724}},    // [6] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 15.673739, .y = 2.350004, .z = -24.376247}},    // [7] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 13.650572, .y = 2.350004, .z = -24.527628}},    // [8] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.677061, .y = 2.350004, .z = -26.352442}},    // [9] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 19.305956, .y = 2.350004, .z = -22.057898}},    // [10] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 14.704304, .y = 2.350004, .z = -21.911671}},    // [11] Default
};

tds_tower_placement_t molten_with_shotgunners_lighthaos_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -2.718571, .y = 2.328601, .z = 14.026078}},    // [0] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -2.631559, .y = 2.315174, .z = 16.082590}},    // [1] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -2.713682, .y = 2.301340, .z = 18.147932}},    // [2] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -4.485553, .y = 2.318360, .z = 15.024799}},    // [3] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -4.472761, .y = 2.304322, .z = 17.150524}},    // [4] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -4.401143, .y = 2.290441, .z = 19.270971}},    // [5] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -6.237051, .y = 2.308122, .z = 16.027962}},    // [6] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -6.261215, .y = 2.294153, .z = 18.131897}},    // [7] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -3.101392, .y = 2.202222, .z = 25.061102}},    // [8] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -1.038854, .y = 2.238259, .z = 25.042173}},    // [9] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = -8.532471, .y = 2.218440, .z = 21.321833}},    // [10] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = -9.032661, .y = 2.287798, .z = 18.231104}},    // [11] Default
};

tds_tower_placement_t molten_with_shotgunners_necropolis_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 3.299171, .y = 2.362497, .z = -19.432207}},    // [0] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -3.302631, .y = 2.362497, .z = -24.543640}},    // [1] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 1.260310, .y = 2.362497, .z = -19.430815}},    // [2] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -1.130479, .y = 2.362497, .z = -24.514076}},    // [3] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -0.801431, .y = 2.362497, .z = -19.390160}},    // [4] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 2.390483, .y = 2.362497, .z = -17.637005}},    // [5] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 0.202887, .y = 2.362497, .z = -17.488859}},    // [6] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -2.200730, .y = 2.362497, .z = -26.270031}},    // [7] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 0.908314, .y = 2.350000, .z = -24.536551}},    // [8] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -0.148891, .y = 2.350000, .z = -26.282106}},    // [9] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 3.684238, .y = 2.350000, .z = -24.566563}},    // [10] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 2.338600, .y = 2.350000, .z = -27.373947}},    // [11] Default
};

void* molten_with_shotgunners_map_dict[] = {
    "Spring Fever", molten_with_shotgunners_spring_fever_placements,
    "Toyboard", molten_with_shotgunners_toyboard_placements,
    "Winter Bridges", molten_with_shotgunners_winter_bridges_placements,
    "Lighthaos", molten_with_shotgunners_lighthaos_placements,
    "Necropolis", molten_with_shotgunners_necropolis_placements,
};
long molten_with_shotgunners_map_count = 5;




#pragma mark - MOLTEN WITH SCOUT -

tds_tower_placement_t molten_with_scouts_stained_temple_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -5.988326, .y = -0.420869, .z = -21.088358}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -3.019352, .y = -0.297629, .z = -21.644505}},    // [1] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -4.024878, .y = -0.395207, .z = -18.747635}},    // [2] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -7.055871, .y = -0.458756, .z = -18.115967}},    // [3] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -1.008756, .y = -0.218506, .z = -19.379299}},    // [4] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -9.071402, .y = -0.417073, .z = -20.374645}},    // [5] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -10.027332, .y = -0.459817, .z = -17.513325}},    // [6] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -0.072303, .y = -0.170299, .z = -22.319576}},    // [7] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -13.395278, .y = -0.523858, .z = -2.604549}},    // [8] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -12.243841, .y = -0.617107, .z = 0.287416}},    // [9] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -15.354980, .y = -0.304374, .z = -0.255256}},    // [10] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -14.160305, .y = -0.418345, .z = 2.647203}},    // [11] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -11.162309, .y = -0.651272, .z = 3.273211}},    // [12] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -13.272614, .y = -0.446536, .z = 5.660399}},    // [13] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -0.174883, .y = -0.087666, .z = 7.568946}},    // [14] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -0.735953, .y = -0.023389, .z = 10.614970}},    // [15] Golden
};

tds_tower_placement_t molten_with_scouts_cataclysm_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -0.469673, .y = 2.350008, .z = 24.403967}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -3.487375, .y = 2.350008, .z = 24.337166}},    // [1] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -6.547063, .y = 2.350008, .z = 24.246323}},    // [2] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -1.984765, .y = 2.350008, .z = 21.734526}},    // [3] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 1.049109, .y = 2.350008, .z = 21.668604}},    // [4] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -5.017964, .y = 2.350008, .z = 21.577492}},    // [5] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 2.569597, .y = 2.350008, .z = 24.451412}},    // [6] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -5.117982, .y = 2.350008, .z = 26.948067}},    // [7] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -1.955861, .y = 2.350008, .z = 27.072596}},    // [8] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 1.063668, .y = 2.350008, .z = 27.138733}},    // [9] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 4.100641, .y = 2.350008, .z = 21.777504}},    // [10] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 4.119798, .y = 2.350008, .z = 27.162493}},    // [11] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -8.125936, .y = 2.350008, .z = 26.987984}},    // [12] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -8.069902, .y = 2.350008, .z = 21.623798}},    // [13] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -3.662528, .y = 2.350008, .z = 29.639462}},    // [14] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -0.486555, .y = 2.350008, .z = 29.742702}},    // [15] Golden
};

tds_tower_placement_t molten_with_scouts_forgotten_docks_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -26.698362, .y = 2.374987, .z = -18.935684}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -26.658934, .y = 2.374990, .z = -22.033594}},    // [1] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -24.040438, .y = 2.374992, .z = -20.383026}},    // [2] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -23.830105, .y = 2.374989, .z = -23.410988}},    // [3] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -26.527340, .y = 2.374988, .z = -25.058533}},    // [4] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -23.024149, .y = 2.374988, .z = -17.553268}},    // [5] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -22.891090, .y = 2.374986, .z = -11.202107}},    // [6] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -28.596050, .y = 2.374996, .z = -16.049421}},    // [7] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -28.607130, .y = 2.374987, .z = -12.999670}},    // [8] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -19.862251, .y = 2.374995, .z = -11.408348}},    // [9] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -21.322527, .y = 2.374985, .z = -8.594883}},    // [10] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -23.770550, .y = 2.374993, .z = -26.486191}},    // [11] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -16.983822, .y = 2.374984, .z = -10.360142}},    // [12] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -18.351332, .y = 2.374985, .z = -7.628070}},    // [13] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -20.963972, .y = 2.374984, .z = -5.561846}},    // [14] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -28.671425, .y = 2.374973, .z = -9.957275}},    // [15] Golden
};

tds_tower_placement_t molten_with_scouts_dusty_bridges_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 20.182301, .y = -32.759140, .z = -345.677368}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 20.115532, .y = -32.788406, .z = -348.711426}},    // [1] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 17.501019, .y = -32.761940, .z = -347.091858}},    // [2] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 22.780188, .y = -32.788723, .z = -347.329102}},    // [3] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 17.404865, .y = -32.788250, .z = -350.132385}},    // [4] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 19.997240, .y = -32.879021, .z = -351.717590}},    // [5] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 22.750854, .y = -32.822746, .z = -350.453735}},    // [6] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.717268, .y = -32.764050, .z = -348.444366}},    // [7] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.806379, .y = -32.848114, .z = -345.344299}},    // [8] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.529522, .y = -32.883629, .z = -351.468445}},    // [9] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 17.153664, .y = -32.988300, .z = -353.119934}},    // [10] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 25.477161, .y = -32.910236, .z = -348.778412}},    // [11] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 28.483999, .y = -33.164875, .z = -348.302460}},    // [12] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 27.149553, .y = -33.281586, .z = -343.230255}},    // [13] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 24.032131, .y = -33.582855, .z = -342.611633}},    // [14] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.015755, .y = -33.030296, .z = -347.084351}},    // [15] Golden
};

tds_tower_placement_t molten_with_scouts_summer_castle_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 7.146088, .y = 2.316005, .z = 23.751472}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 7.225718, .y = 2.318508, .z = 26.781590}},    // [1] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 7.340322, .y = 2.321045, .z = 29.796024}},    // [2] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.909203, .y = 2.323258, .z = 28.160332}},    // [3] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 10.016806, .y = 2.321033, .z = 25.156727}},    // [4] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 10.148346, .y = 2.325963, .z = 31.171326}},    // [5] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.714622, .y = 2.328226, .z = 29.602087}},    // [6] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.733200, .y = 2.325872, .z = 26.591797}},    // [7] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 15.420872, .y = 2.330667, .z = 28.019035}},    // [8] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 15.324036, .y = 2.328115, .z = 24.955511}},    // [9] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 15.335945, .y = 2.333049, .z = 31.180714}},    // [10] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.711311, .y = 2.330860, .z = 32.942627}},    // [11] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.733223, .y = 2.318212, .z = 22.075848}},    // [12] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 18.100109, .y = 2.333074, .z = 26.437277}},    // [13] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 7.592140, .y = 2.323817, .z = 32.869408}},    // [14] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 6.957363, .y = 2.313320, .z = 20.678400}},    // [15] Golden
};

tds_tower_placement_t molten_with_scouts_crossroads_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.223600, .y = 2.349998, .z = 10.052916}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 6.208572, .y = 2.349998, .z = 10.107122}},    // [1] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 7.710122, .y = 2.349999, .z = 7.455783}},    // [2] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 4.613606, .y = 2.349999, .z = 7.386536}},    // [3] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 7.859448, .y = 2.349998, .z = 12.735926}},    // [4] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 10.717433, .y = 2.349999, .z = 7.429662}},    // [5] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.186213, .y = 2.349999, .z = 4.748565}},    // [6] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 6.137532, .y = 2.349999, .z = 4.674458}},    // [7] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 3.101558, .y = 2.349999, .z = 4.772588}},    // [8] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 10.978402, .y = 2.349998, .z = 12.535565}},    // [9] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.493093, .y = 2.349998, .z = 9.945759}},    // [10] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 5.233804, .y = 2.349998, .z = 14.262314}},    // [11] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 8.079960, .y = 2.349998, .z = 15.826757}},    // [12] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.261675, .y = 2.349999, .z = 4.557654}},    // [13] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 13.930889, .y = 2.349999, .z = 7.069809}},    // [14] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.843813, .y = 2.349998, .z = 11.918501}},    // [15] Golden
};

tds_tower_placement_t molten_with_scouts_sugar_rush_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -22.287657, .y = 14.549652, .z = -58.568287}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -25.465534, .y = 14.562976, .z = -58.615677}},    // [1] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -19.218388, .y = 14.558797, .z = -58.617355}},    // [2] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -23.878580, .y = 14.572752, .z = -55.974876}},    // [3] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -20.850365, .y = 14.561967, .z = -55.895309}},    // [4] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -26.919237, .y = 14.584408, .z = -55.907070}},    // [5] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -17.821442, .y = 14.574450, .z = -55.907391}},    // [6] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -25.261232, .y = 14.583554, .z = -53.272125}},    // [7] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -22.232635, .y = 14.574257, .z = -53.194199}},    // [8] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -19.167763, .y = 14.567250, .z = -53.170769}},    // [9] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -28.483240, .y = 14.575188, .z = -58.590721}},    // [10] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -28.294621, .y = 14.592940, .z = -53.118912}},    // [11] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -26.492043, .y = 14.588220, .z = -50.533287}},    // [12] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -23.448086, .y = 14.580844, .z = -50.413845}},    // [13] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -20.408453, .y = 14.574487, .z = -50.413982}},    // [14] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -17.395592, .y = 14.581108, .z = -50.688721}},    // [15] Golden
};

tds_tower_placement_t molten_with_scouts_toyboard_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.211677, .y = 16.088120, .z = 11.005280}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.183187, .y = 16.088120, .z = 7.904672}},    // [1] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 11.886947, .y = 16.088120, .z = 9.458585}},    // [2] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 11.888878, .y = 16.088120, .z = 12.506937}},    // [3] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 11.837754, .y = 16.088120, .z = 6.318763}},    // [4] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.558176, .y = 16.088120, .z = 11.041348}},    // [5] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.545599, .y = 16.088120, .z = 7.763099}},    // [6] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.395268, .y = 16.088120, .z = 14.282475}},    // [7] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 9.187503, .y = 16.088120, .z = 4.706577}},    // [8] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.399332, .y = 16.088120, .z = 4.666532}},    // [9] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.624596, .y = 16.088120, .z = 14.125759}},    // [10] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 12.044209, .y = 16.088120, .z = 15.722612}},    // [11] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 3.975580, .y = 16.088120, .z = 6.489492}},    // [12] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 0.872389, .y = 16.088120, .z = 6.464625}},    // [13] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 2.484878, .y = 16.088120, .z = 3.804413}},    // [14] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -0.514470, .y = 16.088120, .z = 3.708802}},    // [15] Golden
};


void* molten_with_scouts_map_dict[] = {
    "Stained Temple",molten_with_scouts_stained_temple_placements,
    "Cataclysm", molten_with_scouts_cataclysm_placements,
    "Forgotten Docks", molten_with_scouts_forgotten_docks_placements,
    "Dusty Bridges", molten_with_scouts_dusty_bridges_placements,
    "Summer Castle", molten_with_scouts_summer_castle_placements,
    "Crossroads", molten_with_scouts_crossroads_placements,
    "Sugar Rush", molten_with_scouts_sugar_rush_placements,
    "Toyboard", molten_with_scouts_toyboard_placements,
};
long molten_with_scouts_map_count = 8;


#pragma mark - HARDCORE -

tds_tower_placement_t hardcore_crossroads_placements[] =
{
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 9.731409, .y = 2.349998, .z = 9.880926}},    // [0] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 2.623988, .y = 2.350000, .z = -2.661462}},    // [1] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -2.620718, .y = 2.350008, .z = -2.652845}},    // [2] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 2.662422, .y = 2.349999, .z = 2.606677}},    // [3] Default
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -2.575851, .y = 2.349999, .z = 2.765388}},    // [4] Default
    (tds_tower_placement_t){.type = 2, .pos = (vector3_t){.x = 8.570588, .y = 2.349999, .z = 7.059342}},    // [5] Default
};

void* hardcore_map_dict[] = {
    "Crossroads", hardcore_crossroads_placements,
};
long hardcore_map_count = 1;


char tds_check_if_tower_loaded(task_t task, vm_address_t towers_folder, long idx)
{
    char __loaded = false;
    long tc = 0;
    rbx_child_t* t = rbx_instance_get_children(task, towers_folder, &tc);
    if (t)
    {
        if (tc > idx)
        {
            /*
             It seems that while towers are still loading, they dont have this child...
             */
            if (rbx_instance_find_first_child(task, t[idx].child_address, "Display"))
            {
                __loaded = true;
            }
        }
        vm_deallocate(mach_task_self_, (vm_address_t)t, tc * sizeof(rbx_child_t));
    }
    return __loaded;
}


long tds_get_selected_tower_level(task_t task, vm_address_t level_textlabel)
{
    long level = 0;
    
    long __l = 0;
    char* c = rbx_textlabel_get_text(task, level_textlabel, &__l);
    if (c)
    {
        level = atoi(c + 7); //Level: %d [+7 until integer]
        vm_deallocate(mach_task_self_, (vm_address_t)c, __l);
    }
    
    return level;
}

long tds_get_tower_count(task_t task, vm_address_t towers_folder)
{
    long tc = 0;
    rbx_child_t* t = rbx_instance_get_children(task, towers_folder, &tc);
    if (t)
    {
        vm_deallocate(mach_task_self_, (vm_address_t)t, tc * sizeof(rbx_child_t));
    }
    return tc;
}



void tds_print_placements(task_t task, int format_mode)
{
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    //vm_address_t rs = rbx_instance_find_first_child(task, game, "ReplicatedStorage");
    
    //rbx_print_descendants(task, rs, 0, 3);
    //vm_address_t camera = rbx_instance_find_first_child_of_class(task, workspace, "Camera");
    vm_address_t towers_folder = rbx_instance_find_first_child(task, workspace, "Towers");
    long tower_count = 0;
    rbx_child_t* towers = rbx_instance_get_children(task, towers_folder, &tower_count);
    if (towers)
    {
        printf("tds_tower_placement_t placements[] = \n{\n");
        for (long i = 0 ; i < tower_count ; i++)
        {
            int type = 1;
            if (format_mode == RBX_TDS_AUTOFARM_TYPE_MOLTEN_WITH_SHOTGUNNERS_DEATH_FARM)
            {
                if (i > 9 && i < 12)
                {
                    type = 2;
                }
            }
            vm_address_t tower = towers[i].child_address;
            //rbx_print_descendants(task, tower, 0, 2);
            vm_address_t tower_hrp = rbx_instance_find_first_child_partial(task, tower, "RootPart");
            rbx_cframe_t thrpcf = rbx_basepart_get_cframe(task, tower_hrp);
            printf("\t(tds_tower_placement_t){.type = %d, .pos = (vector3_t){.x = %f, .y = %f, .z = %f}},\t", type, thrpcf.pos.x, thrpcf.pos.y, thrpcf.pos.z);
            //printf("\t[%ld]  (%f, %f, %f) \t\t", i, thrpcf.pos.x, thrpcf.pos.y, thrpcf.pos.z);
            long len = 0;
            char* name = rbx_instance_get_name(task, tower, &len);
            printf("// [%ld] %s\n", i, name);
            vm_deallocate(mach_task_self_, (vm_address_t)name, len);
            //rbx_print_descendants(task, tower, 0, 0);
        }
        vm_deallocate(mach_task_self_, (vm_address_t)towers, tower_count * sizeof(rbx_child_t));
        printf("};\n");
    }
}

void U_TDS_CLICK(libESPSharedMemoryObject* shared_memory, float _x, float _y)
{
    send_mousemove_yielding(shared_memory, 0, _x, _y);
    send_leftclick_yielding(shared_memory, 1, _x, _y, 10000);
}

void U_TDS_KEYPRESS(libESPSharedMemoryObject* shared_memory, char key)
{
    send_keypress_yielding(shared_memory, 2, keycode_for_character(key), &key, 100000);
}

char TDS_CHECK_PREPLACE_TOWER(task_t task, vm_address_t camera, tds_tower_placement_t placement)
{
    char is = false;
    vm_address_t tower = rbx_instance_find_last_child_of_class(task, camera, "Model");
    vm_address_t tower_hrp = rbx_instance_find_first_child_partial(task, tower, "RootPart");
    rbx_cframe_t thrp_cf = rbx_basepart_get_cframe(task, tower_hrp);
    float mag = vector3_dist_dif(thrp_cf.pos, placement.pos);
    if (mag < 0.1)
    {
        //printf("preplace in range\n");
        is = true;
    }
    return is;
}

#define TDS_CLICK(_x, _y) U_TDS_CLICK(shared_memory, _x, _y);
#define TDS_KEYPRESS(key) U_TDS_KEYPRESS(shared_memory, key);

#define TDS_READY_CLICK TDS_CLICK(450, 470)
#define TDS_RESTART_CLICK TDS_CLICK(324, 102)
#define TDS_SKIP_CLICK TDS_CLICK(440, 510)
#define TDS_RETURN_TO_LOBBY_CLICK TDS_CLICK(474, 106)

#define TDS_LOBBY_PLAY_GAME_CLICK TDS_CLICK(400, 132)
#define TDS_LOBBY_SURVIVAL_CLICK TDS_CLICK(400, 292)
#define TDS_LOBBY_HARDCORE_CLICK TDS_CLICK(150, 180)
#define TDS_LOBBY_MOLTEN_CLICK TDS_CLICK(934/2, 292)
#define TDS_LOBBY_EASY_CLICK TDS_CLICK(106/2, 292)
#define TDS_LOBBY_SOLO_CLICK TDS_CLICK(298, 330)

#define TDS_LOBBY_PLAYTIME_REWARD_CLAIM_OPEN_CLICK TDS_CLICK(500, 1125/2)

#define TDS_LOBBY_PLAYTIME_REWARD_1_CLAIM_CLICK TDS_CLICK(660/2, 535/2)
#define TDS_LOBBY_PLAYTIME_REWARD_2_CLAIM_CLICK TDS_CLICK(800/2, 535/2)
#define TDS_LOBBY_PLAYTIME_REWARD_3_CLAIM_CLICK TDS_CLICK(950/2, 535/2)
#define TDS_LOBBY_PLAYTIME_REWARD_4_CLAIM_CLICK TDS_CLICK(660/2, 345/2)
#define TDS_LOBBY_PLAYTIME_REWARD_5_CLAIM_CLICK TDS_CLICK(800/2, 345/2)
#define TDS_LOBBY_PLAYTIME_REWARD_6_CLAIM_CLICK TDS_CLICK(950/2, 345/2)

#define TDS_LOBBY_PLAYTIME_REWARD_YELLOW_CLAIM_BUTTON_CLICK TDS_CLICK(400, 105)

#define TDS_INTERMISSION_READY_CLICK TDS_CLICK(400, 100)
#define TDS_INTERMISSION_VETO_CLICK TDS_CLICK(557, 100)
#define TDS_INTERMISSION_MODIFIERS_CLICK TDS_CLICK(60, 60)
#define TDS_INTERMISSION_MODIFIERS_SEARCH_CLICK TDS_CLICK(602/2, 794/2)
#define TDS_INTERMISSION_MODIFIERS_TOP_CENTER_MODIFIER_CLICK TDS_CLICK(793/2, 702/2)
#define TDS_INTERMISSION_VOTE_FOR_MODIFIERS_CLICK TDS_CLICK(937/2, 230/2)

#define TDS_CENTER_CLICK TDS_CLICK(shared_memory->WINDOW_W/2, shared_memory->WINDOW_H/2)


#define TDS_SELECT_TOWER(tds_placement) \
img_verif_tower_selected = false;\
new_camera_cframe.pos = tds_placement.pos;\
new_camera_cframe.pos.y += 2;\
should_set_camera_cframe = true;\
send_mousemove_yielding(shared_memory, 0, shared_memory->WINDOW_W/2, shared_memory->WINDOW_H/2);\
usleep(200000);\
TDS_CENTER_CLICK;\
usleep(30000);\
TDS_WAIT_WHILE_COND_WITH_TIMEOUT(!(img_verif_tower_selected), 10);\
should_set_camera_cframe = false;\


#define TDS_DESELECT_TOWER() \
new_camera_cframe.pos.x = -99999;\
new_camera_cframe.pos.y = -99999;\
new_camera_cframe.pos.z = -99999;\
should_set_camera_cframe = true;\
send_mousemove_yielding(shared_memory, 0, shared_memory->WINDOW_W/2, shared_memory->WINDOW_H/2);\
usleep(200000);\
TDS_CENTER_CLICK;\
usleep(30000);\
should_set_camera_cframe = false;\
img_verif_tower_selected = false;\



#define TDS_WAIT_WHILE_COND(x) \
while (x)\
{\
usleep(50000);\
}


#define TDS_DO_WHILE_COND_WITH_TIMEOUT(dothis, cond, timeout_sec) ({                              \
    double _timeout = (timeout_sec);                                  \
    struct timespec _start, _now;                                     \
    clock_gettime(CLOCK_MONOTONIC, &_start);                          \
    bool _timed_out = false;                                          \
    while (cond) {                                                    \
        clock_gettime(CLOCK_MONOTONIC, &_now);                        \
        double _elapsed = (_now.tv_sec - _start.tv_sec) +             \
                          (_now.tv_nsec - _start.tv_nsec) / 1e9;      \
        if (_elapsed >= _timeout) {                                   \
            _timed_out = true;                                        \
            break;                                                    \
        }                                                             \
dothis;                                                               \
        usleep(50000);                                                \
    }                                                                 \
    _timed_out; /* returns true if timeout occurred */                 \
})

#define TDS_WAIT_WHILE_COND_WITH_TIMEOUT(cond, timeout_sec)\
TDS_DO_WHILE_COND_WITH_TIMEOUT({}, cond, timeout_sec)


#define TDS_PLACE_TOWER(tds_placement) \
img_verif_tower_selected = false;\
TDS_KEYPRESS('0' + tds_placement.type);\
new_camera_cframe.pos = tds_placement.pos;\
new_camera_cframe.pos.y += 0;\
should_set_camera_cframe = true;\
send_mousemove_yielding(shared_memory, 0, shared_memory->WINDOW_W/2, shared_memory->WINDOW_H/2);\
usleep(200000);\
TDS_WAIT_WHILE_COND_WITH_TIMEOUT(!(TDS_CHECK_PREPLACE_TOWER(task, camera, tds_placement)), 10);\
sleep(1);\
TDS_CENTER_CLICK;\
usleep(300000);\
TDS_WAIT_WHILE_COND_WITH_TIMEOUT(!(tds_get_tower_count(task, towers_folder) > current_tower_count), 10);\
should_set_camera_cframe = false;\
TDS_WAIT_WHILE_COND_WITH_TIMEOUT(!(img_verif_tower_selected), 10);\




#define TDS_WAIT_FOR_TOWER_LOAD(idx) \
while (tds_check_if_tower_loaded(task, towers_folder, idx) == false)\
{\
usleep(300000);\
}


long TDS_UPGRADE_TOWER(task_t task, libESPSharedMemoryObject* shared_memory, vm_address_t reactupgrades_gui, int times)
{
    vm_address_t frame = rbx_instance_find_first_child(task, reactupgrades_gui, "upgrades");
    if (!frame)
    {
        return -1;
    }
    vm_address_t leveltext_tl = rbx_instance_find_first_descendant(task, reactupgrades_gui, "levelText", "TextLabel", 1000, 100000);
    long lvl = tds_get_selected_tower_level(task, leveltext_tl);
    for (int i = 0 ; i < times ; i++)
    {
        TDS_KEYPRESS('e');
    }
    sleep(1);
    lvl = tds_get_selected_tower_level(task, leveltext_tl);
    return lvl;
}

long TDS_UPGRADE_TOWER_UNTIL_LEVEL(task_t task, libESPSharedMemoryObject* shared_memory, vm_address_t reactupgrades_gui, int desired_lvl)
{
    vm_address_t frame = rbx_instance_find_first_child(task, reactupgrades_gui, "upgrades");
    if (!frame)
    {
        return -1;
    }
    vm_address_t leveltext_tl = rbx_instance_find_first_descendant(task, reactupgrades_gui, "levelText", "TextLabel", 1000, 100000);
    TDS_DO_WHILE_COND_WITH_TIMEOUT(TDS_KEYPRESS('e'); sleep(1);, tds_get_selected_tower_level(task, leveltext_tl) < desired_lvl, 10);
    sleep(1);
    return tds_get_selected_tower_level(task, leveltext_tl);
}

#include "Play-Round.h"
#include "Intermission.h"
#include "Join-Game.h"
#include "Operations.h"

