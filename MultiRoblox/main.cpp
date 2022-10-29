/*
 *	Obviously, we need this, or how are we going to interact with the operating
 *	system?
 */
#include <Windows.h>
/*
 *	It's also obvious that we need this, since `iostream` gives us the `std::cout`
 *	and the `std::cin` objects.
 */
#include <iostream>
//#include <SDKDDKVer.h>

/*
 *	This is the color library, LICENSE does not apply to this!
 *	You can find the color library here: https://github.com/aafulei/color-console
 */
#include "color.h"

int main()
{
	CreateMutex(0, 1, L"ROBLOX_singletonMutex");
	std::cout << dye::aqua("MultiRoblox ") << dye::green("v0.0.1") << "\n" << dye::white("MultiRoblox is the best open-source program that allows you to run multiple ROBLOX instances!") << "\n" << dye::grey("Press `Ctrl + C` or close this window to exit. If exited, all ROBLOX instances except one will be killed.") << "\n";
	for (;;) { /* Nothing to do here... */ }
	return 0;
}