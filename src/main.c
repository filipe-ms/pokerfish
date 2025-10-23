#include "game_config.h"
#include "scene_manager.h"
#include "game_menu.h"
#include "background.h"

// Raylib includes
#include "raylib.h"
#include "resource_dir.h"

// Function prototypes
void LoadGameResources(void);
void UnloadGameResources(void);

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(game_window.width, game_window.height, "Hello Raylib");
	SearchAndSetResourceDir("resources");

	LoadGameResources();

	ChangeScene(game_menu);
	
	while (!GameShouldQuit()) SceneLoop();

	UnloadGameResources();

	CloseWindow();
	return 0;
}

void LoadGameResources(void)
{
	LoadBackground();
}

void UnloadGameResources(void)
{
	UnloadBackground();
}