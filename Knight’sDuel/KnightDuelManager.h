#pragma once
#include "Player.h"
#include "Enemy.h"
#include "BattleManager.h"
#include <cstdlib>

// 게임 진행 담당
// BattleManager를 사용해 전투 수행
class KnightDuelManager
{
    static inline int RandRange(int min, int max)
    {
        return min + (rand() % (max - min + 1));

    }
 
public:
    KnightDuelManager()   // 플레이어 초기화 (HP,ST,ATK,DEF)
        : MainPlayer("플레이어", 100, 100,RandRange(17, 20), RandRange(7, 10))
    {

    }   
    void RunGame();      // 게임 실행

private:
    void PlayerTurn(BattleManager& BM);   // 플레이어 입력 -> 행동 계획
    void PrintResult(bool bWin);          // 전투 결과 출력

private:
    Player MainPlayer;  // 플레이어 캐릭터
};
