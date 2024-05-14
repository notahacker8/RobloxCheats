# RobloxCheats
Cheating tool for MacOS Roblox, mainly for the upcoming native version of roblox<br>
This is actually the 2nd version of the cheat. The original is actually from my [libHack.dylib](https://github.com/notahacker8/libHack) project.<br>
This project was rebuilt to be more flexible with updates and crash the application less.<br>

## Notes
 - <b>libESP.dylib</b> controls all in-app inputs, ESP, and function calls
  - <b>RobloxCheats</b> is the actual executable to run after libESP.dylib is injected into Roblox
 - Run <b>find_object_offsets()</b> and <b>find_function_offsets()</b> in https://www.roblox.com/games/15816797935/Hacker-Testing to update the offsets. (You will not need to inject libESP.dylib for this)
 - Run <b>patch_roblox_file()</b> before opening roblox to allow for flying and breakpoints.
 - Compile libESP.dylib for the architecture of Roblox
 - Change the file paths at the top of main.m for <b>YOUR</b> machine.

## Images

![Arsenal](https://github.com/notahacker8/RobloxCheats/blob/main/RobloxCheats-SampleImages/Arsenal.png)


https://github.com/notahacker8/RobloxCheats/assets/77363964/eb363a19-4ced-4be2-894a-def50e1c608a


![Blox-Fruits](https://github.com/notahacker8/RobloxCheats/blob/main/RobloxCheats-SampleImages/Blox-Fruits.png)


https://github.com/notahacker8/RobloxCheats/assets/77363964/d6579f8f-89bf-47d2-81c3-8a7ee9b64234





