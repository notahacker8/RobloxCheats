# RobloxCheats
Cheating tool for Roblox (macOS M1)<br>
This is actually the 2nd version of the cheat. The original is actually from my [<b>libHack.dylib</b>](https://github.com/notahacker8/libHack) project.<br>
This project was rebuilt to be more flexible with updates and crash the application less.<br>
I will also try to add as many comments as I can in the future to explain my code as much as possible. (or if somehow forget how all of this works)

## Notes
 - <b>libESP.dylib</b> controls all in-app inputs and UI
 - Change the <b>_</b><b>_INJECTED_DYLIB_</b><b>_</b> macro to the actual path of libESP.dylib
 - <b>_RobloxCheats</b> is the actual executable to run after libESP.dylib is injected into Roblox
 - Edit the code to your liking. The current cheat is set to Phantom Forces, change it if you feel like it.
 - Run <b>find_object_offsets()</b> in https://www.roblox.com/games/5332389196/Hack-Tests to update the offsets in the code. (You will not need to inject libESP.dylib for this)
 - Compile both for x86_64. On Apple Silicon Macs, choose to build for Rosetta.<br>
 ![Build](https://github.com/notahacker8/RobloxCheats/blob/main/Build.png)<br>
 - Also, root is not required! Yay! You can finally run a cheat without having to give root access! (The cheat runs with debugging privileges)

## Usage (Terminal)
DYLD_INSERT_LIBRARIES=(path to libESP.dylib) (path to RobloxPlayer) <br> <br>
See [<b>robloxShell</b>](https://github.com/notahacker8/RobloxCheats/blob/main/robloxShell) for how I did it<br>

<b>Have Fun!</b>

## Phantom Forces ESP
![Phantom Forces ESP](https://github.com/notahacker8/RobloxCheatApp/blob/main/Phantom-Forces-ESP.png)

## Those Who Remain Item ESP
![Those Who Remain Item ESP](https://github.com/notahacker8/RobloxCheatApp/blob/main/Those-Who-Remain-Item-ESP.png)
