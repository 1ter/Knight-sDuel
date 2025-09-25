#include "BattleManager.h"
#include "Actor.h"
#include "Player.h"
#include <cstdio>  

// 턴 처리
void BattleManager::NextTurn()
{
    Player->ClearChoice();  // 턴 시작마다 초기화

    // 1) 디버프 Tick
    Player->TickDebuff();
    Enemy->TickDebuff();

    // 2) ST +5 자연회복
    Player->RegenST();
    Enemy->RegenST();

    // 3) 행동 선택
    Player->TakeTurn();  // (플레이어 입력 기반, 나중에 구현)
    Enemy->TakeTurn();   // (적 AI 기반, 나중에 구현)

    // 4) 전투 판정
    ResolveActions();

    // 5) 상태 출력
    PrintBattleLog();

    // 6) 승패 판정
    if (Player->IsDead() && Enemy->IsDead())
    {
        Result = BattleResult::Draw;
    }
    else if (Enemy->IsDead())
    {
        Result = BattleResult::PlayerWin;
    }
    else if (Player->IsDead())
    {
        Result = BattleResult::EnemyWin;
    }
}

// 전투 결과
void BattleManager::RunBattle()
{
    printf("=== 전투 시작 ===\n");

    while (Result == BattleResult::None)
    {
        NextTurn();
    }

    switch (Result)
    {
    case BattleResult::PlayerWin:
        printf("플레이어 승리!\n");
        break;
    case BattleResult::EnemyWin:
        printf("적 승리!\n");
        break;
    case BattleResult::Draw:
        printf("무승부!\n");
        break;
    default:
        break;
    }
}

// 플레이어/적 상태 출력 내부 
void BattleManager::PrintBattleLog() const
{

    /*printf("\n[상태 출력]\n");
    printf("%s: HP=%d/%d, ST=%d/%d, ATK=%d, DEF=%d\n",
        Player->GetName().c_str(), Player->GetHP(), Player->GetMaxHP(),
        Player->GetST(), Player->GetMaxST(), Player->GetATK(), Player->GetDEF());

    printf("%s: HP=%d/%d, ST=%d/%d, ATK=%d, DEF=%d\n",
        Enemy->GetName().c_str(), Enemy->GetHP(), Enemy->GetMaxHP(),
        Enemy->GetST(), Enemy->GetMaxST(), Enemy->GetATK(), Enemy->GetDEF());*/
}

// 전투 규칙 구현
void BattleManager::ResolveActions()
{
    // 공격 대 방어일때 이런식으로 아직 미완
    int Damage = Player->GetATK() - Enemy->GetDEF();
    if (Damage < 0)
    {
        Damage = 0;
    }
    Enemy->TakeDamage(Damage);
    printf("%s 가 %s 를 공격! 피해 %d\n",
        Player->GetName().c_str(), Enemy->GetName().c_str(), Damage);
    

}

