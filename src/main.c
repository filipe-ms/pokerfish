#include "game_config.h"
#include "scene_manager.h"
#include "game_menu.h"
#include "background.h"
#include "list.h"

// Raylib includes
#include "raylib.h"
#include "resource_dir.h"

// Function prototypes
static void LoadGameResources(void);
static void UnloadGameResources(void);

int main(void)
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

static void LoadGameResources(void)
{
	LoadBackground();
}

static void UnloadGameResources(void)
{
	UnloadBackground();
	List_FreeAll();
}