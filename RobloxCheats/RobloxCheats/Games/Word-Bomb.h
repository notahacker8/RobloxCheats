

vm_address_t word_bomb_get_infoframe(task_t task, vm_address_t playergui)
{
    vm_address_t gameui_screengui = rbx_instance_find_first_child(task, playergui, "GameUI");
    
    vm_address_t container_frame = rbx_instance_find_first_child(task, gameui_screengui, "Container");
    vm_address_t gamespace_frame = rbx_instance_find_first_child(task, container_frame, "GameSpace");
    vm_address_t defaultui_frame = rbx_instance_find_first_child(task, gamespace_frame, "DefaultUI");
    vm_address_t gamecontainer_frame = rbx_instance_find_first_child(task, defaultui_frame, "GameContainer");
    vm_address_t desktopcontainer_frame = rbx_instance_find_first_child(task, gamecontainer_frame, "DesktopContainer");
    vm_address_t infoframeconatiner = rbx_instance_find_first_child(task, desktopcontainer_frame, "InfoFrameContainer");
    vm_address_t infoframe = rbx_instance_find_first_child(task, infoframeconatiner, "InfoFrame");
    return infoframe;
}

char word_bomb_check_if_my_turn(task_t task, vm_address_t infoframe_title)
{
    char is_my_turn = false;
    long title_text_length = 0;
    char* title_text = rbx_textlabel_get_text(task, infoframe_title, &title_text_length);
    if (title_text)
    {
        if (strcmp(title_text, "Quick! Type an English word containing:") == 0)
        {
            //printf("PLAYER's TURN\n");
            is_my_turn = true;
        }
        vm_deallocate(mach_task_self_, (vm_address_t)title_text, title_text_length);
    }
    return is_my_turn;
}



void word_bomb_cheat(task_t task)
{
    
    srand((int)time(NULL));
    
    void* dlhandle = dlopen(__LIBESP_DYLIB_PATH__, RTLD_NOW);
    if (!dlhandle)
    {
        printf("%s\n", dlerror());
    }
    
    vm_address_t s_load_address = task_get_image_address_by_path(mach_task_self_, __LIBESP_DYLIB_PATH__);
    
    vm_offset_t input_usleep_time_offset = gdso(dlhandle, s_load_address, "INPUT_USLEEP_TIME");
    vm_offset_t input_queue_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE");
    vm_offset_t input_queue_count_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE_COUNT");
    vm_offset_t input_queue_finished_offset = gdso(dlhandle, s_load_address, "INPUT_QUEUE_FINISHED");
    
    dlclose(dlhandle);
    
    vm_address_t load_address =  task_get_image_address_by_path(task, __LIBESP_DYLIB_PATH__);
    if (!load_address)
    {
        printf("Couldn't find libESP.dylib in task %d\n", task);
        exit(0);
    }
    
    useconds_t iut = 1000;
    vm_write(task, load_address + input_usleep_time_offset, (vm_address_t)&iut, sizeof(useconds_t));
    
    static int max_word_length = 7;
    
    
    static vm_size_t english_size = 0;
    static char* english_bytes = NULL;
    
    NSMutableURLRequest* request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:@"https://www.mit.edu/~ecprice/wordlist.10000"]];
    //NSMutableURLRequest* request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:@"https://raw.githubusercontent.com/dwyl/english-words/master/words.txt"]];
    [request setHTTPMethod:@"GET"];
    
    NSURLSession *session = [NSURLSession sharedSession];
    
    NSURLSessionDataTask *data_task = [session dataTaskWithRequest:request completionHandler:^(NSData *data, NSURLResponse *response, NSError *error)
    {
      NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *)response;
      if(httpResponse.statusCode == 200)
      {
          english_size = data.length;
          english_bytes = malloc(english_size);
          memcpy(english_bytes, (char*)data.bytes, english_size);
      }
      else
      {
        NSLog(@"Error sending request to %@", request.URL);
      }
    }];
    [data_task resume];
    
    while (!(data_task.state == NSURLSessionTaskStateCompleted))
    {
        usleep(10000);
    }
    sleep(1);
    printf("REQUEST FOR ENGLISH WORDS COMPLETED\n");
    
    printf("%ld\n", english_size);
    
    for (int i = 0 ; i < english_size ; i++)
    {
        english_bytes[i] = toupper(english_bytes[i]);
    }
    
    
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    vm_address_t playergui = rbx_instance_find_first_child_of_class(task, local_player, "PlayerGui");
    
    char** found_words = malloc(900000 * sizeof(char*));
    int* found_word_lengths = malloc(900000 * sizeof(int));
    
    for (;;)
    {
        int found_word_count = 0;
        vm_address_t infoframe = word_bomb_get_infoframe(task, playergui);
        
        vm_address_t infoframe_title = rbx_instance_find_first_child(task, infoframe, "Title");
        vm_address_t infoframe_textframe = rbx_instance_find_first_child(task, infoframe, "TextFrame");
        
        int letter_count = 0;
        char letters[100];
        char is_my_turn = word_bomb_check_if_my_turn(task, infoframe_title);
        
        if (is_my_turn)
        {
            long iftf_cc = 0;
            rbx_child_t* iftf_children = rbx_instance_get_children(task, infoframe_textframe, &iftf_cc);
            if (iftf_children)
            {
                for (long i = 0 ; i < iftf_cc ; i++)
                {
                    vm_address_t child = iftf_children[i].child_address;
                    if (child)
                    {
                        vm_address_t letter_imglabel = rbx_instance_find_first_child(task, child, "Letter");
                        vm_address_t textlabel = rbx_instance_find_first_child_of_class(task, letter_imglabel, "TextLabel");
                        if (textlabel)
                        {
                            //rbx_print_descendants(task, letter_imglabel, 0, 1);
                            long length = 0;
                            char* text = rbx_textlabel_get_text(task, textlabel, &length);
                            if (text)
                            {
                                letters[letter_count++] = text[0];
                                vm_deallocate(mach_task_self_, (vm_address_t)text, length);
                            }
                        }
                    }
                }
                vm_deallocate(mach_task_self_, (vm_address_t)iftf_children, iftf_cc * sizeof(rbx_child_t));
            }
        }
        
        if (letter_count > 2)
        {
            printf("FINDING WORDS...\n");
            found_word_count = 0;
            char* real_letters = letters + 2;
            int real_letter_count = letter_count - 2;
            char* search = english_bytes;
            while (strstr(search, "\n"))
            {
                char* nlc = strstr(search, "\n");
                //char* end = nlc - real_letter_count;
                long word_length = nlc - search;
                if (word_length < (max_word_length + 1))
                {
                    //printf("%d\n", word_length);
                    for (char* test = search ; test < nlc ; test++)
                    {
                        if (memcmp(test, real_letters, real_letter_count) == 0)
                        {
                            char is_only_alpha = true;
                            for (int i = 0 ; i < word_length ; i++)
                            {
                                if (isalpha(search[i]) == false)
                                {
                                    is_only_alpha = false;
                                    i = (int)word_length;
                                }
                            }
                            if (is_only_alpha)
                            {
                                //printf("%.*s\n", word_length, search);
                                found_word_lengths[found_word_count] = (int)word_length;
                                found_words[found_word_count] = search;
                                found_word_count++;
                            }
                        }
                    }
                }
                search = nlc + 1;
            }
            if (found_word_count > 0)
            {
                printf("TYPING: ");
                int word_index = rand() % found_word_count;
                char* selected_word = found_words[word_index];
                
                char __word[found_word_lengths[word_index] + 1];
                memset(__word, 0, sizeof(__word));
                for (int i = 0 ; i < found_word_lengths[word_index] ; i++)
                {
                    __word[i] = selected_word[i];
                }
                for (int i = 0 ; i < found_word_lengths[word_index] ; i++)
                {
                    printf("%c", __word[i]);
                    send_character_keypress(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, __word[i], 10000);
                }
                
                printf("\n");
                send_simple_keypress(task, load_address, input_queue_offset, input_queue_count_offset, input_queue_finished_offset, (36), 5000);
            }
            
        }
        
        usleep(100000);
    }
    
    //rbx_print_descendants(task, infoframe, 0, 4);
    for (;;)
    {
        //printf("%s\n", rbx_textlabel_get_text(task, infoframe_title, malloc(8)));
        
        sleep(1);
    }
}
