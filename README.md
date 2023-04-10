# RobloxCheats
Cheating tool for Roblox (macOS M1)<br>
This is actually the 2nd version of the cheat. The original is actually from my [libHack.dylib](https://github.com/notahacker8/libHack) project.<br>
This project was rebuilt to be more flexible with updates and crash the application less.<br>

<b>Download:</b> [<b>RobloxCheatsFolder.zip</b>](https://github.com/notahacker8/RobloxCheats/blob/main/RobloxCheatsFolder.zip)

## Notes
 - <b>libESP.dylib</b> controls all in-app inputs, ESP, and function calls
  - <b>RobloxCheats</b> is the actual executable to run after libESP.dylib is injected into Roblox
 - Run <b>find_object_offsets()</b> and <b>find_function_offsets()</b> in https://www.roblox.com/games/5332389196/Hack-Tests to update the offsets.(You will not need to inject libESP.dylib for this)
 - Compile both for x86_64/Rosetta.
 - ***root*** is not required for the cheat to work. However, SIP will have to be disabled for the cheat to work now.

## Usage (Terminal)
***To run a new Roblox process with libESP.dylib injected:***
```
cd /path/to/RobloxCheatsFolder/
./RobloxShell
```
***To run RobloxCheats:***
```
cd /path/to/RobloxCheatsFolder/
./RobloxCheats [parameters]
```

## Images

![Arsenal](https://github.com/notahacker8/RobloxCheats/blob/main/RobloxCheats-SampleImages/Arsenal.png)
![Doors](https://github.com/notahacker8/RobloxCheats/blob/main/RobloxCheats-SampleImages/Doors.png)
![Phantom-Forces](https://github.com/notahacker8/RobloxCheats/blob/main/RobloxCheats-SampleImages/Phantom-Forces.png)
![Jailbreak](https://github.com/notahacker8/RobloxCheats/blob/main/RobloxCheats-SampleImages/Jailbreak.png)

