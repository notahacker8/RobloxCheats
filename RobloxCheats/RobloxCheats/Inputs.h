

void yield_input(libESPSharedMemoryObject* shared_memory, int input_index)
{
    while (shared_memory->INPUT_QUEUE[input_index].should_execute == true) {usleep(500);}
}

void mousemove(libESPSharedMemoryObject* shared_memory, NSPoint p)
{
    if (shared_memory->APP_IS_ACTIVE)
    {
        shared_memory->MOUSE_X = p.x; shared_memory->MOUSE_Y = p.y;
        moveMouseOnWindow(shared_memory->WINDOW_X, shared_memory->WINDOW_Y, shared_memory->WINDOW_W, shared_memory->WINDOW_H, p);
    }
}

void send_mousemove_prechecking(libESPSharedMemoryObject* shared_memory, int input_index, float x, float y)
{
    if (shared_memory->INPUT_QUEUE[input_index].should_execute == false)
    {
        shared_memory->MOUSE_X = x; shared_memory->MOUSE_Y = y;
        shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_MOUSE_MOVE, .should_execute = true, .x = x, .y = y};
    }
}

void send_mousemove_yielding(libESPSharedMemoryObject* shared_memory, int input_index, float x, float y)
{
    yield_input(shared_memory, input_index);
    shared_memory->MOUSE_X = x; shared_memory->MOUSE_Y = y;
    shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_MOUSE_MOVE, .should_execute = true, .x = x, .y = y};
    yield_input(shared_memory, input_index);
}

void send_leftmousedown(libESPSharedMemoryObject* shared_memory, int input_index, float x, float y)
{
    shared_memory->MOUSE_X = x; shared_memory->MOUSE_Y = y;
    shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_LEFT_MOUSE_DOWN, .should_execute = true, .x = x, .y = y};
}

void send_leftmouseup(libESPSharedMemoryObject* shared_memory, int input_index, float x, float y)
{
    shared_memory->MOUSE_X = x; shared_memory->MOUSE_Y = y;
    shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_LEFT_MOUSE_UP, .should_execute = true, .x = x, .y = y};
}

void send_keydown(libESPSharedMemoryObject* shared_memory, int input_index, char keycode, char* characters)
{
    strcpy(shared_memory->INPUT_QUEUE[input_index].characters, characters);
    shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_KEY_DOWN, .should_execute = true, .keycode = keycode};
}

void send_keyup(libESPSharedMemoryObject* shared_memory, int input_index, char keycode, char* characters)
{
    strcpy(shared_memory->INPUT_QUEUE[input_index].characters, characters);
    shared_memory->INPUT_QUEUE[input_index] = (Input){.type = INPUT_TYPE_KEY_UP, .should_execute = true, .keycode = keycode};
}


void send_leftclick_yielding(libESPSharedMemoryObject* shared_memory, int input_index, float x, float y, int duration)
{
    yield_input(shared_memory, input_index);
    send_leftmousedown(shared_memory, input_index, x, y);
    usleep(duration);
    yield_input(shared_memory, input_index);
    send_leftmouseup(shared_memory, input_index, x, y);
    yield_input(shared_memory, input_index);
}


void send_leftclick_prechecking(libESPSharedMemoryObject* shared_memory, int input_index, float x, float y, int duration)
{
    if (shared_memory->INPUT_QUEUE[input_index].should_execute == false)
    {
        send_leftmousedown(shared_memory, input_index, x, y);
        usleep(duration);
        yield_input(shared_memory, input_index);
        send_leftmouseup(shared_memory, input_index, x, y);
    }
}



void send_keypress_yielding(libESPSharedMemoryObject* shared_memory, int input_index, int keycode, char* characters, int duration)
{
    yield_input(shared_memory, input_index);
    send_keydown(shared_memory, input_index, keycode, characters);
    usleep(duration);
    yield_input(shared_memory, input_index);
    send_keyup(shared_memory, input_index, keycode, characters);
    yield_input(shared_memory, input_index);
}


void send_keypress_prechecking(libESPSharedMemoryObject* shared_memory, int input_index, int keycode, char* characters, int duration)
{
    if (shared_memory->INPUT_QUEUE[input_index].should_execute == false)
    {
        send_keydown(shared_memory, input_index, keycode, characters);
        usleep(duration);
        yield_input(shared_memory, input_index);
        send_keyup(shared_memory, input_index, keycode, characters);
    }
}
