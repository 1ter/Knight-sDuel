#pragma once
#pragma once
#include "Actor.h"

// ���� ���
enum class BattleResult
{
    None,
    PlayerWin,  // �÷��̾� ��
    EnemyWin,   // �� ��
    Draw        // ���º�
};

// ���� ���� Ŭ����
class BattleManager
{
public:
    BattleManager(Actor* InPlayer, Actor* InEnemy)
        : Player(InPlayer), Enemy(InEnemy), Result(BattleResult::None)
    {
    }

    void RunBattle();      // ��ü ���� ���� ����
    void NextTurn();       // �� ó��
    BattleResult GetResult() const { return Result; }

private:
    Actor* Player;
    Actor* Enemy;
    BattleResult Result;

    void PrintStatus() const;  // ���� ���
    void ResolveActions();     // �÷��̾�/�� �ൿ ����
};
