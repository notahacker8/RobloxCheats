# RobloxCheats
Cheating tool for Roblox (macOS M1)<br>
This is actually the 2nd version of the cheat. The original is actually from my [<b>libHack.dylib</b>](https://github.com/notahacker8/libHack) project.<br>
This project was rebuilt to be more flexible with updates and crash the application less.<br>

## Notes
 - <b>libESP.dylib</b> controls all in-app inputs and UI
  - <b>_RobloxCheats</b> is the actual executable to run after libESP.dylib is injected into Roblox
 - Change the <b>_ _ INJECTED_DYLIB _ _</b> macro to the actual path of libESP.dylib
 - Run <b>find_object_offsets()</b> in https://www.roblox.com/games/5332389196/Hack-Tests to update the offsets in the code. (You will not need to inject libESP.dylib for this)
 - Compile both for x86_64.
 - root is not required for the cheat to work. However, SIP will have to be disabled for the cheat to work now.

## Usage (Terminal)
DYLD_INSERT_LIBRARIES=(path to libESP.dylib) (path to RobloxPlayer) <br> <br>
See [<b>robloxShell</b>](https://github.com/notahacker8/RobloxCheats/blob/main/robloxShell) for an example<br>

## Arsenal ESP + Aimbot
![Arsenal ESP + Aimbot](https://github.com/notahacker8/RobloxCheats/blob/main/RobloxCheats-Images/Arsenal-Aimbot-ESP.png)

## Phantom Forces ESP
![Phantom Forces ESP](https://github.com/notahacker8/RobloxCheatApp/blob/main/Phantom-Forces-ESP.png)

## Those Who Remain Item ESP
![Those Who Remain Item ESP](https://github.com/notahacker8/RobloxCheatApp/blob/main/Those-Who-Remain-Item-ESP.png)

## Doors ESP
![Doors ESP](https://github.com/notahacker8/RobloxCheatApp/blob/main/Doors-ESP.png)
![Doors ESP](https://github.com/notahacker8/RobloxCheatApp/blob/main/Doors-ESP-2.png)
