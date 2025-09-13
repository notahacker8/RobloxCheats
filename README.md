# RobloxCheats
Cheating tool for MacOS Roblox, mainly for the upcoming native version of roblox<br>
This is actually the 2nd version of the cheat. The original is actually from my [libHack.dylib](https://github.com/notahacker8/libHack) project.<br>
This project was rebuilt to be more flexible with updates and crash the application less.<br>

## Notes
 - <b>libESP.dylib</b> controls all in-app inputs, ESP, and function calls
  - <b>RobloxCheats</b> is the actual executable to run after libESP.dylib is injected into Roblox
 - Run <b>find_object_offsets()</b> and <b>find_function_offsets()</b> in https://www.roblox.com/games/15816797935/Hacker-Testing to update the offsets. (You will not need to inject libESP.dylib for this)
 - Run <b>patch_roblox_file()</b> before opening roblox to allow for flying and breakpoints. (Not needed as of now)
 - Compile libESP.dylib for the architecture of Roblox. (You can check this by executing the command: file /path/to/RobloxPlayer)
 - Change the file paths at the top of main.m for <b>YOUR</b> machine.
 - It is recommended to use <b>DYLD_INSERT_LIBRARIES</b> inside a shell script to run roblox with libESP.dylib

## Steps for setting up

- Download both folders, they're Xcode projects.
- Compile libESP.dylib for the correct architecture.
- Change the file paths at the top of RobloxCheats/main.m.
- Find the object offsets by going to the game mentioned in the notes section.
- Run your cheat function in the main() function. (e.g. generic_cheat())


## Images




https://github.com/user-attachments/assets/e5af78ba-7182-4784-b838-902b90e68fac

<img width="1440" alt="Blox-Fruits" src="https://github.com/user-attachments/assets/8bd2e10c-1927-4832-9db7-fbe3c2314779" />

https://github.com/notahacker8/RobloxCheats/assets/77363964/d6579f8f-89bf-47d2-81c3-8a7ee9b64234

https://github.com/user-attachments/assets/e2531c41-8829-4f38-b5de-bb2c90236920




