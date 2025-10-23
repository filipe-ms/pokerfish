#include "game_config.h"
#include "scene_manager.h"
#include "game_menu.h"

// Raylib includes
#include "raylib.h"
#include "resource_dir.h"

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(game_window.width, game_window.height, "Hello Raylib");
	SearchAndSetResourceDir("resources");

	ChangeScene(game_menu);
	
	while (!GameShouldQuit()) SceneLoop();

	CloseWindow();
	return 0;
}
