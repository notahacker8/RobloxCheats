



rbx_tds_input_t hardcore_input_stacks[1][100];
int hardcore_input_stack_counts[1];

rbx_tds_input_t molten_input_stacks[1][100];
int molten_input_stack_counts[1];

int tds_input_stack_index = 0;
useconds_t tds_input_stack_execution_usleep_time = 100000;




void init_tds_input_stacks(void)
{
    
    
#pragma mark -
#pragma mark Hardcore (Autumn Falling)
    
    /*
     -32.195030, 3.270883, 6.498524 (Ace Pilot)
     Military Bases:
     -38.946480, 3.004749, 29.055859
     -18.169022, 3.004660, -16.471029
     -43.971558, 3.004388, -18.334816
     -29.812855, 3.002073, 1.961421
     -33.782475, 3.002073, 1.793431
     */
    
    
    int i = 0;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].should_wait_until_this_cash = true;
    hardcore_input_stacks[0][i].should_skip_while_waiting = true;
    hardcore_input_stacks[0][i].cash = 750;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].wait_time = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_PLACE;
    hardcore_input_stacks[0][i].pos = vector3_init(-32.195030, 3.270883, 6.498524);
    hardcore_input_stacks[0][i].tower_number = 2;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].should_wait_until_this_cash = true;
    hardcore_input_stacks[0][i].should_skip_while_waiting = true;
    hardcore_input_stacks[0][i].cash = 375;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].wait_time = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_UPGRADE;
    hardcore_input_stacks[0][i].tower_index = 0;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].should_wait_until_this_cash = true;
    hardcore_input_stacks[0][i].should_skip_while_waiting = true;
    hardcore_input_stacks[0][i].cash = 525;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].wait_time = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_UPGRADE;
    hardcore_input_stacks[0][i].tower_index = 0;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].should_wait_until_this_cash = true;
    hardcore_input_stacks[0][i].should_skip_while_waiting = true;
    hardcore_input_stacks[0][i].cash = 1500;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].wait_time = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_PLACE;
    hardcore_input_stacks[0][i].pos = vector3_init(-38.946480, 3.004749, 29.055859);
    hardcore_input_stacks[0][i].tower_number = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].wait_time = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_UPGRADE;
    hardcore_input_stacks[0][i].tower_index = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].wait_time = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_UPGRADE;
    hardcore_input_stacks[0][i].tower_index = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].wait_time = 1;
    i++;
    
    
    // 2
    
    
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].should_wait_until_this_cash = true;
    hardcore_input_stacks[0][i].should_skip_while_waiting = true;
    hardcore_input_stacks[0][i].cash = 1500;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].wait_time = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_PLACE;
    hardcore_input_stacks[0][i].pos = vector3_init(-18.169022, 3.004660, -16.471029);
    hardcore_input_stacks[0][i].tower_number = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].wait_time = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_UPGRADE;
    hardcore_input_stacks[0][i].tower_index = 2;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_WAIT;
    hardcore_input_stacks[0][i].wait_time = 1;
    i++;
    
    hardcore_input_stacks[0][i].type = RBX_TDS_INPUT_UPGRADE;
    hardcore_input_stacks[0][i].tower_index = 2;
    i++;
    
    hardcore_input_stack_counts[0] = i + 1;
    
#pragma mark -
#pragma mark Molten (Autumn Falling)
    
    i = 0;
    molten_input_stacks[0][i].type = RBX_TDS_INPUT_PLACE;
    
    molten_input_stack_counts[0] = i;
    
    
}
