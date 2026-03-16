
vm_address_t hitman_card_get_icon(task_t task, vm_address_t card)
{
    vm_address_t face_sgui = rbx_instance_find_first_child(task, card, "Face");
    vm_address_t il = rbx_instance_find_first_child(task, face_sgui, "Icon");
    return il;
}

NSString* hitman_get_card_name_by_icon(task_t task, vm_address_t icon_imagelabel, NSDictionary* card_icon_dict)
{
    NSString* name = @"";
    long len = 0;
    char* rbxassetidstr = rbx_imagelabel_get_assetid(task, icon_imagelabel, &len);
    if (rbxassetidstr)
    {
        name = [[card_icon_dict allKeysForObject:[NSString stringWithUTF8String:rbxassetidstr]] firstObject];
        vm_deallocate(mach_task_self(), (vm_address_t)rbxassetidstr, len);
    }
    return name;
}


///NOT FINISHED/DOESNT WORK
void hitman_card_game_cheat(task_t task)
{
    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t matchsetup_folder = rbx_instance_find_first_child(task, workspace, "MatchSetup");
    vm_address_t hand_folder = rbx_instance_find_first_child(task, matchsetup_folder, "Hand");
    vm_address_t cardpile_folder = rbx_instance_find_first_child(task, matchsetup_folder, "CardPile");
    
    //rbx_print_descendants(task, hand_folder, 0, 4);
    //rbx_print_descendants(task, cardpile_folder, 0, 4);
    rbx_print_descendants(task, matchsetup_folder, 0, 2);
    
    static NSMutableDictionary* card_icon_dict = NULL;
    card_icon_dict = [@{
        @"Angel": @"rbxassetid://117775321700268",
        @"Attack": @"rbxassetid://130911272290977",
        @"Bottom": @"rbxassetid://15978915623",
        @"Cancel": @"rbxassetid://125407668036769",
        @"Chain": @"rbxassetid://137224513077672",
        @"CloneHand": @"rbxassetid://88504747433505",
        @"FakeShuffle": @"rbxassetid://111670539303500",
        @"Future": @"rbxassetid://15978831522",
        @"Hitman": @"rbxassetid://83529624038561",
        @"Inferno": @"rbxassetid://125770155576078",
        @"Mirror": @"rbxassetid://135463541009935",
        @"Moon": @"rbxassetid://134898202623951",
        @"Pirate": @"rbxassetid://114041193982628",
        @"QuickShuffle": @"rbxassetid://124403665915532",
        @"Reverse": @"rbxassetid://104138641868462",
        @"Rewind": @"rbxassetid://70768890627366",
        @"Search": @"rbxassetid://85710504559070",
        @"Shield": @"rbxassetid://128463336928261",
        @"Skip": @"rbxassetid://15979178306",
        @"Slash": @"rbxassetid://111276799685809",
        @"Steal": @"rbxassetid://139364503023431",
        @"SuperAttack": @"rbxassetid://115479958598328"
    } mutableCopy];
    
    static NSMutableDictionary* ranked_total_cards = NULL;
    ranked_total_cards = [@{
        @"Angel": @4,
        @"Attack": @4,
        @"Bottom": @2,
        @"Cancel": @3,
        @"Chain": @4,
        @"Future": @3,
        @"Hitman": @1,
        @"Inferno": @3,
        @"Mirror": @5,
        @"Pirate": @1,
        @"QuickShuffle": @3,
        @"Reverse": @4,
        @"Rewind": @2,
        @"Shield": @2,
        @"Skip": @4,
        @"Steal": @2,
        @"Search": @3,
    } mutableCopy];
    
    static NSMutableDictionary* ranked_cards_left = NULL;
    ranked_cards_left = [ranked_total_cards mutableCopy];
    
    static char cardpile_did_change = false;
    
    static char placed_cards_stack_str[10000];
    bzero(placed_cards_stack_str, sizeof(placed_cards_stack_str));
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        rbx_child_t* children = NULL;
        long child_count = 0;
        for (;;)
        {
            long new_child_count = 0;
            rbx_child_t* new_children = rbx_instance_get_children(task, cardpile_folder, &new_child_count);
            if (new_child_count != child_count)
            {
                cardpile_did_change = true;
                //printf("changed children\n");
            }
            else if (new_children && children && !(memcmp(new_children, children, child_count * sizeof(rbx_child_t)) == 0))
            {
                //printf("changed children\n");
                cardpile_did_change = true;
            }
            if (children)
            {
                vm_deallocate(mach_task_self(), (vm_address_t)children, child_count * sizeof(rbx_child_t));
            }
            children = new_children;
            child_count = new_child_count;
            usleep(1000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            while (!cardpile_did_change) {usleep(1000);}
            cardpile_did_change = false;
            vm_address_t top_card = rbx_instance_find_last_child_of_class(task, cardpile_folder, "MeshPart");
            vm_address_t il = hitman_card_get_icon(task, top_card);
            NSString* name = hitman_get_card_name_by_icon(task, il, card_icon_dict);
        }
    });

    /*
    
    for (;;)
    {
        FOR_EACH_CHILD(hand_folder, {
            long name_l = 0;
            char* name = rbx_instance_get_name(task, child, &name_l);
            if (name)
            {
                vm_address_t face_sgui = rbx_instance_find_first_child(task, child, "Face");
                vm_address_t il = rbx_instance_find_first_child(task, face_sgui, "Icon");
                long len = 0;
                char* assetid_str = rbx_imagelabel_get_assetid(task, il, &len);
                if (assetid_str)
                {
                    card_icon_dict[[[NSString alloc] initWithUTF8String:name]] = [[NSString alloc] initWithUTF8String:assetid_str];
                    vm_deallocate(mach_task_self(), (vm_address_t)name, len);
                }
                vm_deallocate(mach_task_self(), (vm_address_t)name, name_l);
            }
        })
        NSLog(@"%@", card_icon_dict);
        sleep(1);
    }
     */
}


/*
 static custom_rbx_instance_mutant_data_t new_md[10000];
 static custom_rbx_instance_mutant_data_t old_md[10000];
 FOR_EACH_DESCENDANT(task, workspace, 10000, 10, {
     old_md[i].instance = obj;
     old_md[i].parent = rbx_instance_get_parent(task, obj);
     old_md[i].child_count = rbx_instance_get_child_count(task, obj);
 })
 
 for (;;)
 {
     for (int i = 0; i < 10000 ; i++)
     {
         vm_address_t instance = old_md[i].instance;
         
         
         vm_address_t new_p = rbx_instance_get_parent(task, instance);
         vm_address_t new_cc = rbx_instance_get_child_count(task, instance);
         if (new_p != old_md[i].parent)
         {
             printf("parent changed, old parent: ");
             rbx_print_descendants(task, old_md[i].parent, 0, 0);
             rbx_print_descendants(task, instance, 0, 0);
             printf("\n");
         }
         if (new_cc != old_md[i].child_count)
         {
             printf("child count changed: %ld -> %ld\n", old_md[i].child_count, new_cc);
             rbx_print_descendants(task, instance, 0, 0);
             printf("\n");
         }
     }
     sleep(1);
 }
 */
