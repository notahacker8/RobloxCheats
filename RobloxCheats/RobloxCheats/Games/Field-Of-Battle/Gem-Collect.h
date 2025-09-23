



#define FOB_ALL_LEGENDARY_FILTER if (gv > 31)
#define FOB_BAD_LEGENDARY_FILTER if (gv == 35 || gv == 36) //Spirit Shard and Dragon Bone go brrrrrr..
#define FOB_GOOD_LEGENDARY_FILTER if (gv > 31 && gv != 35 && gv != 36)
#define FOB_ALL_COMPLETE_LEGENDARY_FILTER if (gv > 31 && gv != 51) //Legendary shard lol.


void field_of_battle_collect_legendary_gem(task_t task)
{
    LIBESP_DYLIB_SETUP
    printf("- FIELD OF BATTLE (LEGENDARY GEM COLLECT) -\n");

    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t projectiles_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Projectiles");
    //rbx_print_descendants(task, workspace, 0, 4);
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    //static vector3_t new_hrp_pos = {.x = -2050, .y = 758, .z = -1940}; //Middle of lobby, up where the gems fall.

    static vm_address_t gem = 0;
    static vector3_t gem_pos = {};
    
    static vm_address_t my_hrp = 0;
    static vm_address_t my_hrp_cframe_address = 0;
    
    //Gem name given its GemType number.
    static char* gem_dictionary[100];
    gem_dictionary[1] = "Red Diamond";
    gem_dictionary[2] = "Grandidierite";
    gem_dictionary[3] = "Poudretteite";
    gem_dictionary[4] = "Benitoite";
    gem_dictionary[5] = "Tanzanite";
    gem_dictionary[6] = "Alexandrite";
    gem_dictionary[7] = "Diamond";
    gem_dictionary[8] = "Sapphire";
    gem_dictionary[9] = "Emerald";
    gem_dictionary[10] = "Ruby";
    gem_dictionary[11] = "Lapis Lazuli";
    gem_dictionary[12] = "Topaz";
    gem_dictionary[13] = "Garnet";
    gem_dictionary[14] = "Aquamarine";
    gem_dictionary[15] = "Spinel";
    gem_dictionary[16] = "Amber";
    gem_dictionary[17] = "Titanite";
    gem_dictionary[18] = "Tourmaline";
    gem_dictionary[19] = "Kunzite";
    gem_dictionary[20] = "Amethyst";
    gem_dictionary[21] = "Citrine";
    gem_dictionary[22] = "Peridot";
    gem_dictionary[24] = "Onyx";
    gem_dictionary[25] = "Turquoise";
    gem_dictionary[27] = "Feldspar";
    gem_dictionary[28] = "Jade";
    gem_dictionary[29] = "Nephrite";
    gem_dictionary[30] = "Olivine";
    gem_dictionary[31] = "Copal";
    gem_dictionary[32] = "Mithril";
    gem_dictionary[33] = "Demonite";
    gem_dictionary[34] = "Fury Stone";
    gem_dictionary[35] = "Dragon Bone";
    gem_dictionary[36] = "Spirit Shard";
    gem_dictionary[42] = "Titan Core";
    gem_dictionary[51] = "Legendary Shard";
    
    static char should_write_cframe = true;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            if (shared_memory->KEYS_DOWN['f'])
            {
                rbx_smooth_fly(task, true);
            }
            else
            {
                rbx_smooth_fly(task, false);
            }
            usleep(100000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            vm_address_t character = rbx_player_get_character(task, local_player);
            my_hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
            my_hrp_cframe_address = rbx_basepart_get_properties_address(task, my_hrp) + RBX_BASEPART_PROPERTIES_CFRAME_OFFSET;
            rbx_basepart_set_gravity(task, my_hrp, 30);
            sleep(1);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (gem)
            {
                //Gem disappears when we collect
                vm_address_t gem_parent = rbx_instance_get_parent(task, gem);
                if (gem_parent)
                {
                    rbx_cframe_t gem_cf = rbx_basepart_get_cframe(task, gem);
                    gem_pos = gem_cf.pos;
                }
                else
                {
                    //new_hrp_pos.y = -999999.0f;  //Teleport to a place where nobody will see us, after the gem is collected
                }
            }
            usleep(10000);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            usleep(1);
            if (should_write_cframe)
            {
                if (my_hrp && gem)
                {
                    vm_write(task, my_hrp_cframe_address + 0x24, (vm_address_t)&gem_pos.x, 4);
                    vm_write(task, my_hrp_cframe_address + 0x24 + 8, (vm_address_t)&gem_pos.z, 4);
                }
            }
            else
            {
                usleep(100);
            }
        }
    });
    
    /*
     FAST!!!!
     */
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            vm_address_t __gem = 0;
            long projectile_count = 0;
            rbx_child_t* projectiles = rbx_instance_get_children(task, projectiles_folder, &projectile_count);
            if (projectiles)
            {
                
                for (long i = 0 ; i < projectile_count ; i++)
                {
                    vm_address_t projectile = projectiles[i].child_address;
                    vm_address_t gem_type = rbx_instance_find_first_child(task, projectile, "GemType");
                    if (gem_type)
                    {
                        long gv = rbx_intvalue_get_value(task, gem_type);
                        FOB_ALL_COMPLETE_LEGENDARY_FILTER
                        {
                            printf(" > GEM FOUND!!! (GemType: %ld) (Name: %s)\n", gv, gem_dictionary[gv]);
                            __gem = projectile;
                        }
                    }
                }
                
                vm_deallocate(mach_task_self_, (vm_address_t)projectiles, projectile_count * sizeof(rbx_child_t));
            }
            gem = __gem;
            usleep(2000);
        }
    });
   
    
}



#undef FOB_ALL_LEGENDARY_FILTER
#undef FOB_BAD_LEGENDARY_FILTER
#undef FOB_GOOD_LEGENDARY_FILTER
#undef FOB_ALL_COMPLETE_LEGENDARY_FILTER
