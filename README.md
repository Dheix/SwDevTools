# Dev Tools Unlocker
## DESCRIPTION
In game developer tools unlocker for Stormworks.
> [!WARNING]
>**Only supports Stormwork version 1.9.0+.**

Use with [Ultimate-ASI-Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader)
## Installation 
### Installation with Ultimate-ASI-Loader
* Download latest Release of Ultimate-ASI-Loader, Ultimate-ASI-Loader_x64.zip
* Extract Ultimate-ASI-Loader_x64.zip (dinput8.dll) into Stromworks Directory
* In Stromworks Directory, create new folder "plugins" and extract SwDevTools.zip (SwDevTools.asi) into it
* for a Linux-based OS, set launch options to `WINEDLLOVERRIDES="dinput8=n,b" %command% `

> [!IMPORTANT]
>### Installation with Opal Shader
>If you are using [Opal Shader](https://www.nexusmods.com/stormworksbuildandrescue/mods/68) and installed Opal shader with dinput8.dll, global.ini, overload files then use this method
>* In Stormworks Directory, open global.ini and change `LoadPlugins=0` to `LoadPlugins=1` , save
>* Create new folder "plugins" and extract SwDevTools.zip (SwDevTools.asi) into it
