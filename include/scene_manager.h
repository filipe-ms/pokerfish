// scene_manager.h

#pragma once

#include "raylib.h"

typedef struct Scene {
    void (*load_scene)();
    void (*unload_scene)();
    void (*update_scene)();
    void (*draw_scene)();
} Scene;

void InitSceneManager(void);
void ChangeScene(Scene new_scene);
void SceneLoop(void);
void EndSceneManager(void);
bool GameShouldQuit(void);