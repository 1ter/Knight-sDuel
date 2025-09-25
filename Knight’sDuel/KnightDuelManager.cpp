#include "KnightDuelManager.h"
#include <cstdlib>

int GameManager::RandRange(int min, int max) const
{
    if (max < min) { int t = min; min = max; max = t; }
    return min + (std::rand() % (max - min + 1));
}

Stance GameManager::RandomStance() const
{
    int r = std::rand() % 3;
    if (r == 0) { return Stance::Aggressive; }
    if (r == 1) { return Stance::Defensive; }
    return Stance::Balanced;
}

void GameManager::DestroyEnemy()
{
    if (EnemyActor != nullptr)
    {
        delete EnemyActor;
        EnemyActor = nullptr;
    }
}

Enemy* GameManager::CreateAldric()
{
    const char* name = "불멸의 기사 알드릭";
    int hp = RandRange(80, 100);
    int st = 100;
    int atk = RandRange(17, 20);
    int def = RandRange(7, 10);
    Stance stance = RandomStance();
    return new Enemy(name, hp, st, atk, def, stance);
}

void GameManager::Reset()
{
    DestroyEnemy();
}

bool GameManager::SpawnBoss()
{
    if (EnemyActor != nullptr)
    {
        return false; // 이미 스폰됨
    }
    EnemyActor = CreateAldric();
    Battle.SetEnemy(*EnemyActor);
    Battle.StartTurn(); // 전투 시작 턴처리(디버프 틱/회복/초기화)
    return true;
}
