# MultiRoblox
## What is MultiRoblox?
MultiRoblox is an open-source program that allows you to open multiple instances of ROBLOX!
## How do I get MultiRoblox?
Simply download from here: https://github.com/Dashbloxx/MultiRoblox/releases
## Where can I recieve support for MultiRoblox?
Recieve support for MultiRoblox here: [https://discord.gg/U28WHhRHUS](https://discord.gg/QBY5ybZUDQ)

## How do I use it?
1. Start MultiRoblox **before** ROBLOX. If ROBLOX is already open, MultiRoblox offers to close it for you.
2. Open as many ROBLOX windows as you want and keep MultiRoblox open (you can minimize it).
3. Tick **Start with Windows** if you want MultiRoblox to always start before ROBLOX.

## How does it work?
ROBLOX allows only one client by creating a mutex named `ROBLOX_singletonMutex`. MultiRoblox creates and holds that mutex first, so every ROBLOX client thinks it is the only one.
