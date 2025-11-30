#include "BombRush.h"
#include "Engine.h"
#include <windows.h>
#include "SamplingProfiler.h"
#include "ManagedMemory.h"

//#include "SDL_ttf.h"


int main(int argc, char* argv[])
{
	
	
	

	Engine* e = Engine::Get();

	BombRush::Start();

	e->Start();

	SamplingProfiler sp(GetCurrentThreadId(), 1);
	sp.Start();
	while (e->IsGameRunning())
	{
		e->Update();
	}

	sp.End();

	e->Quit();

	return 0;

}