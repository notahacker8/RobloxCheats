# RobloxCheats
Cheating tool for MacOS Roblox, mainly for the upcoming native version of roblox<br>
This is actually the 2nd version of the cheat. The original is actually from my [libHack.dylib](https://github.com/notahacker8/libHack) project.<br>
This project was rebuilt to be more flexible with updates and crash the application less.<br>

## Notes
 - <b>libESP.dylib</b> controls all in-app inputs, ESP, and function calls
  - <b>RobloxCheats</b> is the actual executable to run after libESP.dylib is injected into Roblox
 - Run <b>find_object_offsets()</b> in https://www.roblox.com/games/15816797935/Hacker-Testing to update the offsets.
 - Run <b>patch_roblox_file()</b> before opening roblox to allow for flying and breakpoints. (Not needed as of now)
 - Compile libESP.dylib for the architecture of Roblox. (You can check this by executing the command: file /path/to/RobloxPlayer)
 - Change the file paths at the top of main.m for <b>YOUR</b> machine.
 - It is recommended to use <b>DYLD_INSERT_LIBRARIES</b> inside a shell script to run roblox with libESP.dylib
 - [Misc/lasso-tool.html](https://github.com/notahacker8/RobloxCheats/blob/main/RobloxCheats/RobloxCheats/Misc/lasso-tool.html) is used to find the coordinates of image crops. For retina display, captured images of the window will be twice the size of the actual window.
 - Dylib tools appear at the top menu bar as "Tools" in the injected Roblox app.

## Steps for setting up

- Download both folders, they're Xcode projects.
- Compile libESP.dylib for the correct architecture.
- Change the file paths at the top of RobloxCheats/main.m.
- Find the object offsets by going to the game mentioned in the notes section. Some will have to be manually found.
- Run your cheat function in the main() function. (e.g. generic_cheat())


## Images


<img width="1440" height="900" alt="Screenshot 2025-09-14 at 2 19 35 AM" src="https://github.com/user-attachments/assets/ab144dbc-b6d6-44a8-97ae-dde7a4ddeaac" />


<img width="1440" alt="Blox-Fruits" src="https://github.com/user-attachments/assets/8bd2e10c-1927-4832-9db7-fbe3c2314779" />

https://github.com/user-attachments/assets/bdddfabc-4387-4294-8a38-db2c322bd4df

https://github.com/user-attachments/assets/831cf822-319b-4ea2-af0f-e5e00e6b8af3


https://github.com/user-attachments/assets/3c343ca5-448c-43dc-9a67-f371c029cb76



<img width="1440" height="900" alt="Screenshot 2025-09-12 at 7 25 59 PM" src="https://github.com/user-attachments/assets/2a705eab-06f2-42af-962a-0b36a20e368a" />

<img width="820" height="744" alt="Screenshot 2025-09-12 at 7 13 27 PM copy" src="https://github.com/user-attachments/assets/49561f16-7e81-431f-ac7f-0c3765eb2078" />

<img width="1470" height="956" alt="Screenshot 2026-01-15 at 8 20 42 AM" src="https://github.com/user-attachments/assets/58641464-3b84-4945-b361-698a50b8e7f1" />



