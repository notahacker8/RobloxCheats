



#define RBX_FOB_ALL_LEGENDARY_FILTER if (gv > 31)
#define RBX_FOB_BAD_LEGENDARY_FILTER if (gv == 35 || gv == 36) //Spirit Shard and Dragon Bone go brrrrrr..
#define RBX_FOB_GOOD_LEGENDARY_FILTER if (gv > 31 && gv != 35 && gv != 36)


void field_of_battle_collect_legendary_gem(void)
{
    task_t task = get_roblox_task_port();
    printf("- FIELD OF BATTLE (LEGENDARY GEM COLLECT) -\n");

    vm_address_t game = rbx_find_game_address(task);
    vm_address_t workspace = rbx_instance_find_first_child_of_class(task, game, "Workspace");
    vm_address_t unbreakable_folder = rbx_instance_find_first_child(task, workspace, "Unbreakable");
    vm_address_t projectiles_folder = rbx_instance_find_first_child(task, unbreakable_folder, "Projectiles");
    
    vm_address_t players_service = rbx_instance_find_first_child_of_class(task, game, "Players");
    vm_address_t local_player = rbx_instance_find_first_child_of_class(task, players_service, "Player");
    
    static char gem_found = false;
    static vm_address_t gem = 0;
    static vector3_t new_hrp_pos = {.x = -2050, .y = 758, .z = -1940}; //Middle of lobby, up where the gems fall.
    
    
    
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
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            if (gem_found == true)
            {
                //Gem disappears when we collect
                vm_address_t gem_parent = rbx_instance_get_parent(task, gem);
                if (gem_parent)
                {
                    rbx_cframe_t gem_cf = rbx_basepart_get_cframe(task, gem);
                    new_hrp_pos = gem_cf.pos;
                }
                else
                {
                    new_hrp_pos.y = -999999.0f;  //Teleport to a place where nobody will see us, after the gem is collected
                }
            }
            usleep(10);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
    {
        for (;;)
        {
            
            vm_address_t character = rbx_player_get_character(task, local_player);
            vm_address_t hrp = rbx_instance_find_first_child(task, character, "HumanoidRootPart");
            rbx_basepart_set_gravity(task, hrp, 0);
            
            rbx_cframe_t cf = rbx_basepart_get_cframe(task, hrp);
            cf.pos = new_hrp_pos;
            rbx_basepart_set_cframe(task, hrp, cf);
            
            long projectile_count = 0;
            rbx_child_t* projectiles = rbx_instance_get_children(task, projectiles_folder, &projectile_count);
            if (projectiles)
            {
                
                if (gem_found == false)
                {
                    printf(" > SCANNING GEMS...\n");
                }
                
                for (long i = 0 ; i < projectile_count ; i++)
                {
                    vm_address_t projectile = projectiles[i].child_address;
                    vm_address_t gem_type = rbx_instance_find_first_child(task, projectile, "GemType");
                    if (gem_type)
                    {
                        long gv = rbx_intvalue_get_value(task, gem_type);
                        RBX_FOB_ALL_LEGENDARY_FILTER
                        {
                            printf(" > GEM FOUND!!! (GemType: %ld) (Name: %s)\n", gv, gem_dictionary[gv]);
                            if (gem_found == false)
                            {
                                gem = projectile;
                            }
                            gem_found = true;
                        }
                    }
                }
                
                vm_deallocate(mach_task_self_, (vm_address_t)projectiles, projectile_count * sizeof(rbx_child_t));
            }
            usleep(20000);
        }
    });
   
    
}

