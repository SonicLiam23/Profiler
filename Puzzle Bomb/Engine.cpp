#include "Engine.h"

#include "Log.h"
#include "Timers.h"
#include "FrameProfiler.h"
#include <chrono>
#include "Globals.h"
#include "Renderer.h"
#include "InputManager.h"
#include "ObjectBase.h"
#include <cmath>
#include "Rect.h"
#include "SDL_ttf.h"
#include "SDL.h"

constexpr float profileTimeIntervalSeconds = 3;
constexpr float fpsTarget = 60.0f;

Engine* Engine::s_Instance = nullptr;
Engine* Engine::Get()
{
	return (s_Instance != nullptr) ? s_Instance : s_Instance = new Engine();
}

Engine::Engine() : m_Running(false)
{

}

// i dont like how "complex" this has become, but make it work before you make it pretty, right :D
void Engine::Update()
{
	std::cout << "\033[H";
	std::chrono::steady_clock::time_point frameStart = UpdateLoopTimer->Start();

	
	SDL_RenderClear(SDLClasses::GetRenderer());

	ManualTimer loopTimer("Update Objects");
	// call update on each object
	loopTimer.Start();
	for (ObjectBase*& obj : m_Objects)
	{
		obj->Update();
	}
	loopTimer.Stop();

	ManualTimer renderTimer("Render Objects");
	renderTimer.Start();
	for (ObjectBase*& obj : m_Objects)
	{
		obj->Render();
	}
	SDL_RenderPresent(SDLClasses::GetRenderer());
	renderTimer.Stop();


	if (frameStart > m_nextProfileTime)
	{
		m_nextProfileTime = std::chrono::time_point_cast<std::chrono::milliseconds>(frameStart + std::chrono::duration<float>(3.4f));
		profileDataSS.str("");
		profileDataSS << "Avg FPS: " << frameProfiler->GetAverageFPS() << "\n";
		profileDataSS << "Harmonic Mean FPS: " << frameProfiler->GetHarmonicMeanFPS() << "\n";
		profileDataSS << "Low 1% FPS: " << frameProfiler->GetAvgLowOnePercent() << "\n";
		profileDataSS << "Min FPS: " << frameProfiler->GetMinFPS() << "\n";
		profileDataSS << "Total Allocations" << ManagedMemory::allocations << "\n";
	}
	LOGTEXTC(profileDataSS.str());

	// deltatime is in milliseconds
	float deltaTime;
	UpdateLoopTimer->Stop(&deltaTime, false);
	frameProfiler->AddFrame(deltaTime, frameStart);



	const float targetMs = 1000.0f / fpsTarget;
	if (deltaTime < targetMs)
		SDL_Delay(targetMs - deltaTime);

	InputManager::Get()->Update();
}

const bool Engine::IsGameRunning() const
{
	return m_Running;
}

void Engine::Init()
{
	if (m_Running)
	{
		return;
	}

	UpdateLoopTimer = new ManualTimer("Engine Update");
	frameProfiler = new FrameProfiler(profileTimeIntervalSeconds);
	m_nextProfileTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() + std::chrono::duration<float>(3.4f));
		


	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	m_Objects.reserve(Globals::ESTIMATED_AMT_OF_OBJECTS);

	// creates classes if they dont already exist
	Renderer::Get();
	InputManager::Get();
}

// Run this after added all objects
void Engine::Start()
{
	m_Running = true;
}

void Engine::Uninit()
{
	m_Running = false;

	LOGTEXTM(profileDataSS.str());

	for (int i = 0; !m_Objects.empty(); ++i)
	{

			delete m_Objects.back();
			m_Objects.pop_back();		
	}

	profileDataSS.str("");
	profileDataSS << "Allocations after Uninit: " << ManagedMemory::allocations;
	LOGTEXTM(profileDataSS.str());

	SDLClasses::DeleteAll();
	delete UpdateLoopTimer;
	delete frameProfiler;
}

void Engine::Quit()
{
	SDL_Quit();
}

void Engine::AddObject(ObjectBase* obj)
{

	for (size_t i = 0; i < m_Objects.size(); ++i)
	{
		ObjectBase* objToFind = m_Objects[i];
		if (obj == objToFind)
		{
			return;
		}

	}


	m_Objects.push_back(obj);


}

void Engine::DeleteObject(ObjectBase* obj)
{
	for (int i = 0; i < m_Objects.size(); ++i)
	{
		if (obj == m_Objects[i])
		{
			m_Objects.erase(m_Objects.begin() + i);
			delete obj;
			break;
		}
	}
	
}

void Engine::ClickAt(Rect* pos)
{
	for (ObjectBase* CurrObj : m_Objects)
	{
		if (CurrObj->CollisionEnabled)
		{
			if (IsColliding(CurrObj, pos))
			{
				CurrObj->OnClick();
			}
		}
	}
}

std::vector<ObjectBase*> Engine::GetAllCollisionsWith(ObjectBase* Obj)
{
	std::vector<ObjectBase*> ReturnVector;
	for (ObjectBase* CurrObj : m_Objects)
	{
		if (CurrObj->CollisionEnabled)
		{
			if (IsColliding(Obj, CurrObj))
			{
				ReturnVector.push_back(CurrObj);
			}
		}
	}
	return ReturnVector;
}

bool Engine::IsColliding(Rect* obj1, Rect* obj2)
{
	SDL_Rect rect1 = obj1->TryParse<SDL_Rect>();
	SDL_Rect rect2 = obj2->TryParse<SDL_Rect>();
	return SDL_HasIntersection(&rect1, &rect2);
}

bool Engine::IsColliding(ObjectBase* obj1, Rect* obj2)
{
	return obj1->CollisionEnabled && IsColliding(obj1->GetPosition(), obj2);
}

bool Engine::IsColliding(ObjectBase* obj1, ObjectBase* obj2)
{
	return obj1->CollisionEnabled && obj2->CollisionEnabled && IsColliding(obj1->GetPosition(), obj2->GetPosition());
}


ObjectBase* Engine::GetClosestObject(ObjectBase* obj)
{
	float closestDistance = INT_MAX;
	ObjectBase* closestObject = nullptr;
	for (ObjectBase* currObj : m_Objects)
	{
		if (obj != currObj)
		{
			int dX;
			int dY;
			dX = obj->rect.x - currObj->rect.x;
			dY = obj->rect.y - currObj->rect.y;

			float hyp;
			hyp = (dX * dX) + (dY * dY);
			hyp = std::sqrt(hyp);

			if (hyp < closestDistance)
			{
				closestObject = currObj;
			}
		}
	}

	return closestObject;
}

