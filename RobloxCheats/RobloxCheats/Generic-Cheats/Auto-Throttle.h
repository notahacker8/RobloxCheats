
#pragma mark - AUTO THROTTLE ALL -

//Lower priority of all roblox processes if they are in background
void auto_throttle_all(char use_cputhrottle)
{
    //printf("bg auto\n");
    system("killall -9 cputhrottle_remake");
    
    static int main_pid = -1;
    
    static int roblox_pids[200];
    static int roblox_pid_count = 0;
    
    static int front_pid = -1;
    static char roblox_is_front = false;
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            @autoreleasepool {
                int new_rbx_pid_c = 0;
                ProcessSerialNumber psn = {0, kNoProcess};
                while (GetNextProcess(&psn) == noErr)
                {
                    CFStringRef procNameRef = NULL;
                    if (CopyProcessName(&psn, &procNameRef) == noErr)
                    {
                        NSString *procName = (__bridge_transfer NSString *)procNameRef;
                        if ([procName isEqualToString:@"Roblox"])
                        {
                            pid_t pid = -1;
                            GetProcessPID(&psn, &pid);
                            roblox_pids[new_rbx_pid_c] = pid;
                            new_rbx_pid_c++;
                        }
                    }
                }
                roblox_pid_count = new_rbx_pid_c;
            }
            //printf("front: %d\n", front_pid);
            //printf("roblox count: %d\n", roblox_pid_count);
            sleep(2);
        }
    });
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        for (;;)
        {
            @autoreleasepool
            {
                ProcessSerialNumber frontPSN;
                GetFrontProcess(&frontPSN);
                GetProcessPID(&frontPSN, &front_pid);
                CFStringRef procNameRef = NULL;
                char nrif = 0;
                if (CopyProcessName(&frontPSN, &procNameRef) == noErr)
                {
                    NSString *procName = (__bridge_transfer NSString *)procNameRef;
                    if ([procName isEqualToString:@"Roblox"])
                    {
                        nrif = true;
                    }
                }
                roblox_is_front = nrif;
            }
            usleep(5000);
        }
    });
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^
                   {
        
        //key: Roblox, value: cputhrottle
        NSMutableDictionary<NSNumber*, NSNumber*>* dict = [NSMutableDictionary dictionary];
        
        for (;;)
        {
            pid_t child_pids[10000]; bzero(child_pids, sizeof(child_pids));
            int cpid_count = pids_by_parent_pid(child_pids, getpid());
            //printf("%d\n", cpid_count);
            for (NSNumber* key in [dict allKeys])
            {
                //NSLog(@"key: %@, value: %@", key, dict[key]);
                if (!isProcessAlive(key.intValue))
                {
                    [dict removeObjectForKey:key];
                }
                if (dict[key])
                {
                    char e = false;
                    for (int  i = 0 ; i < cpid_count ; i++)
                    {
                        if (child_pids[i] == dict[key].intValue)
                        {
                            e = true;
                        }
                    }
                    if (!e)
                    {
                        printf("deleted a cputhrottle reference due to absence\n");
                        [dict removeObjectForKey:key];
                    }
                }
            }
            COPY_BUF(roblox_pids)
            COPY_VAR(roblox_pid_count)
            COPY_VAR(front_pid)
            COPY_VAR(roblox_is_front)
            for (int i = 0 ; i < __roblox_pid_count ; i++)
            {
                int pid = __roblox_pids[i];
                if (pid == __front_pid || (pid == main_pid && !__roblox_is_front))
                {
                    setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_MAX);
                    if (use_cputhrottle)
                    {
                        if (dict[@(pid)])
                        {
                            kill(dict[@(pid)].intValue, SIGKILL);
                            [dict removeObjectForKey:@(pid)];
                            printf("deleted cputhrottle for proc %d\n", pid);
                        }
                    }
                }
                else
                {
                    if (pid != main_pid || (pid == main_pid && __roblox_is_front && __front_pid != main_pid))
                    {
                        setpriority(PRIO_DARWIN_PROCESS, pid, PRIO_DARWIN_BG);
                        if (use_cputhrottle)
                        {
                            if (!dict[@(pid)])
                            {
                                char ps[2000];
                                bzero(ps, 2000);
                                sprintf(ps, "%d", pid);
                                char *argv[] = {__CPUTHROTTLE_PATH__, ps, "13", NULL};
                                pid_t ctpid = -1;
                                if (posix_spawn(&ctpid, __CPUTHROTTLE_PATH__, NULL, NULL, argv, NULL) == 0)
                                {
                                    dict[@(pid)] = @(ctpid);
                                    printf("started cputhrottle for proc %d\n", pid);
                                }
                            }
                        }
                    }
                }
            }
            usleep(5000);
        }
    });
    

    
    STDIN_INPUT_ENGINE({
        INSERT_STDIN_MULTIARG_INPUT("main %d",  "", {printf("set main roblox process to %d\n", main_pid);}, 1, &main_pid);
    })
    
}
