#include "Cursor.h"
#include "Engine.h"
#include "Rect.h"
#include <vector>
void Cursor::DoClick(Rect* position)
{
	Engine::Get()->ClickAt(position);
}
