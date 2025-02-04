

//18 gems per loss
#pragma mark - HARDCORE -




/*
 
 INDEXES:
    [0 - 1] : Golden Soldier
 
 */



tds_tower_placement_t tds_hardcore_harbor_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 51.008373, .y = -0.992933, .z = 86.243256}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 47.903061, .y = -0.992998, .z = 86.300247}},    // [1] Golden
};

tds_tower_placement_t tds_hardcore_unknown_garden_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 34.029655, .y = 2.399992, .z = -7.675532}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 30.982391, .y = 2.399999, .z = -7.492532}},    // [1] Golden
};

tds_tower_placement_t tds_hardcore_marshlands_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -0.565284, .y = 4.349995, .z = 27.417507}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 1.667560, .y = 4.349996, .z = 25.407993}},    // [1] Golden
};

tds_tower_placement_t tds_hardcore_four_seasons_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 8.448233, .y = 2.100002, .z = -21.559326}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 5.720164, .y = 2.100003, .z = -22.914024}},    // [1] Golden
};

tds_tower_placement_t tds_hardcore_nether_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -10.234241, .y = 2.279205, .z = 44.424458}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -11.914979, .y = 2.279178, .z = 41.850708}},    // [1] Golden
};

tds_tower_placement_t tds_hardcore_wrecked_battlefield_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.197428, .y = 2.349978, .z = -22.570410}},    // [0] Golden
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 13.968284, .y = 2.349980, .z = -25.703827}},    // [1] Golden
};


char* tds_hardcore_map_list[] =
{
    "Harbor",
    "Unknown Garden",
    "Wrecked Battlefield",
    "Marshlands",
    "Four Seasons",
    "Nether"
};
/*
 The elements MUST be ordered the same way the map names are above.
 */
tds_tower_placement_t* tds_hardcore_map_placements_list[] =
{
    tds_hardcore_harbor_placements,
    tds_hardcore_unknown_garden_placements,
    tds_hardcore_wrecked_battlefield_placements,
    tds_hardcore_marshlands_placements,
    tds_hardcore_four_seasons_placements,
    tds_hardcore_nether_placements
};





//If you don't have golden soldier (12 gems per loss)
#pragma mark - HARDCORE (ALT1) -





tds_tower_placement_t tds_hardcore_alt1_four_seasons_placements[] =
{
(tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -19.704849, .y = 2.099998, .z = 4.838317}},
};
tds_tower_placement_t tds_hardcore_alt1_wrecked_battlefield_placements[] =
{
(tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 19.487097, .y = 2.124976, .z = -19.079676}},    // [0] Default
};
tds_tower_placement_t tds_hardcore_alt1_unknown_garden_placements[] =
{
(tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 26.869898, .y = 2.368683, .z = -21.266922}},    // [0] Barbecue
};
tds_tower_placement_t tds_hardcore_alt1_crossroads_placements[] =
{
(tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -2.672401, .y = 2.350000, .z = 2.585540}},    // [0] Barbecue
};
tds_tower_placement_t tds_hardcore_alt1_marshlands_placements[] =
{
(tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 14.008265, .y = 4.124999, .z = 11.682951}},    // [0] Default
};
tds_tower_placement_t tds_hardcore_alt1_medieval_times_placements[] =
{
(tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -15.845818, .y = 2.350010, .z = -28.559746}},    // [0] Barbecue
};
tds_tower_placement_t tds_hardcore_alt1_harbor_placements[] =
{
(tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = 59.209698, .y = -1.225037, .z = 86.177979}},    // [0] Default
};
tds_tower_placement_t tds_hardcore_alt1_nether_placements[] =
{
    (tds_tower_placement_t){.type = 1, .pos = (vector3_t){.x = -23.077606, .y = 2.053517, .z = 44.171650}},    // [0] Default
};




char* tds_hardcore_alt1_map_list[] =
{
    "Harbor",
    "Unknown Garden",
    "Wrecked Battlefield",
    "Marshlands",
    "Four Seasons",
    "Nether",
    "Crossroads",
    "Nether",
    "Medieval Times"
};
/*
 The elements MUST be ordered the same way the map names are above.
 */
tds_tower_placement_t* tds_hardcore_alt1_map_placements_list[] =
{
    tds_hardcore_alt1_harbor_placements,
    tds_hardcore_alt1_unknown_garden_placements,
    tds_hardcore_alt1_wrecked_battlefield_placements,
    tds_hardcore_alt1_marshlands_placements,
    tds_hardcore_alt1_four_seasons_placements,
    tds_hardcore_alt1_nether_placements,
    tds_hardcore_alt1_crossroads_placements,
    tds_hardcore_alt1_nether_placements,
    tds_hardcore_alt1_medieval_times_placements
};












