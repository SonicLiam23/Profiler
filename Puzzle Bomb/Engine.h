#pragma once
#include <chrono>
#include <vector>
#include <sstream>
class ObjectBase; class ManualTimer; class FrameProfiler;




/*
* Engine class should include:
* Array of objects
* spawn objects
* run "start" on objects
* Call update and render on each object
* handle collisions
*
*/
#include "SDLClasses.h"

struct Rect;
class Engine
{
private:
	bool m_Running;
#pragma region Profilers
	ManualTimer* UpdateLoopTimer;
	FrameProfiler* frameProfiler;
	std::chrono::steady_clock::time_point m_nextProfileTime;
	std::stringstream profileDataSS;
#pragma endregion
	std::vector<ObjectBase*> m_Objects;
	static Engine* s_Instance;
	Engine();

public:
	static Engine* Get();

	void Update();
	const bool IsGameRunning() const;
	void Init();
	void Start();
	void Uninit();
	void Quit();
	void AddObject(ObjectBase* obj);
	void DeleteObject(ObjectBase* obj);

	void ClickAt(Rect* pos);
	// Returns every object that is colliding with it as a vector
	std::vector<ObjectBase*> GetAllCollisionsWith(ObjectBase* Obj);
	// returns true if the 2 objects are colliding
	bool IsColliding(Rect* obj1, Rect* obj2);
	bool IsColliding(ObjectBase* obj1, Rect* obj2);
	bool IsColliding(ObjectBase* obj1, ObjectBase* obj2);

	// Gets the closest Object to the object passed in
	ObjectBase* GetClosestObject(ObjectBase* obj);
};


