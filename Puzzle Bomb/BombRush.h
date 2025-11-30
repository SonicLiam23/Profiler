#pragma once
class Background;

struct BombRush
{
    static void Start();

    static void RestartGame();

    static void EndGame();

    static Background* bg;
};

