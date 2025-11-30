# A basic overview of how to use

## Classes that are used by the engine and not by the developer will be in italics
## private members or functions will be in bold


## 27/11/24

### SDLClasses
Contains the necessary SDL classes, Renderer and Window that need to be referenced in many places
- GetWindow() - returns a pointer to the SDL_Window, or creates it if it doesn't exist
- GetRenderer() - returns a pointer to the SDL_Renderer, or creates it if it doesn't exist
- **s_Window** - contains a pointer to the current SDL_Window
- **s_Renderer** - contains a pointer to the current SDL_Renderer


### Player
An object I made using ObjectBase. This is an example of how to create objects

### Globals
A fully static class containing useful constants
- SCREEN_HEIGHT - Height of the window created in pixels
- SCREEN_WIDTH - Width of the window created in pixels
- MAX_OBJECTS - the maximum amount of object (and therefore textures) that can be stored

### *Image*
A class that contains the necessary data to store and show an image
- Image() - constructor that creates the image based on the path
- GetPath() - returns the image path
- GetTexture() - Returns the SDL_Texture for the image
- **m_Path** - const char* containing the path
- **m_Texture** - SDL_Texture* containing the texture

### *Renderer*
- singleton renderer that renders, stores and draws images
- Get() - Gets the instance of renderer, or creates it if it doesn't exist
- AddImage() - adds an image based on a given path, or returns it if it already exists
- DrawImage() - draws an image based on the given rect
- **Renderer()** - constructor that creates a window and renderer if they don't exist
- **s_instance** - the current instance of renderer
- **m_StoredImages** - a map of all the images, the key is the path, and data is the image class

### Engine
- **m_Running** - Bool that contains if the game is running
- **m_Objects** - vector containing each object
- Update() - runs every game tick
- IsGameRunning() - returns if the game is running
- Init() - initialises the engine, creating all variables and reserving m_Objects space given the max amount of objects
- Start() - runs the game
- Uninit() - [NOT IMPLEMENTED FULLY] will delete all variables and close the engine
- AddObject() - adds an object to the game
- DeleteObject() - [NOT IMPLEMENTED] Deletes an object to the game
- GetAllCollisionsWith() - returns a vector with all the objects currently colliding with the one passed in
- IsColliding() - returns true if both objects passed in are colliding

### InputManager
Singleton that contains all key inputs
- Get() - Gets the current instance
- Update() - updates the inputs
- GetKeyUp() - checks if a key was released this frame
- GetKeyDown() - checks if a key was pressed this frame
- GetKeyHeld() - checks if a key is being held
- **InputManager()** - constructor
- **s_Instance** - stores the current instance
- **NewScanCode** - stores the Scancode of the event that is currently being checked
- **m_PressedKeys** - stores each key and their position

### *ObjectBase*
A base class that stores everything needed for an object
- ObjectBase() - constructor
- Update() - pure virtual function that is called every frame
- Render() - renders the image of the object on screen, skips if m_img is nullptr
- GetName() - pure virtual function that returns the name
- SetImage() - sets the image for the object based on a given path
- CollisionEnabled - stores whether collision is enabled for the object
- GetPosition() - returns the SDL_Rect of the object
- **m_Img** - protected. stores an Image*
- **m_Position** - protected. stores an SDL_Rect





