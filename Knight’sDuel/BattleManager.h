#pragma once
#pragma once
#include "Actor.h"

// 전투 결과
enum class BattleResult
{
    None,
    PlayerWin,  // 플레이어 승
    EnemyWin,   // 적 승
    Draw        // 무승부
};

// 전투 관리 클래스
class BattleManager
{
public:
    BattleManager(Actor* InPlayer, Actor* InEnemy)
        : Player(InPlayer), Enemy(InEnemy), Result(BattleResult::None)
    {
    }

    void NextTurn();       // 턴 처리
    void RunBattle();      // 전체 전투 루프 실행
    BattleResult GetResult() const { return Result; }

private:
    Actor* Player;
    Actor* Enemy;
    BattleResult Result;

    void PrintBattleLog() const;  // 상태 출력
    void ResolveActions();        // 플레이어/적 행동 판정
};