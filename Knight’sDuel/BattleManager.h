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

    void NextTurn();       // �� ó��
    void RunBattle();      // ��ü ���� ���� ����
    BattleResult GetResult() const { return Result; }

private:
    Actor* Player;
    Actor* Enemy;
    BattleResult Result;

    void PrintBattleLog() const;  // ���� ���
    void ResolveActions();        // �÷��̾�/�� �ൿ ����
};