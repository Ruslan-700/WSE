#include "Loader.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPTSTR cmdLine, int cmdShow)
{
	Loader loader;

	loader.Process(cmdLine);
	return EXIT_SUCCESS;
}
