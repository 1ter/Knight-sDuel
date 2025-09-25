#include "BattleManager.h"
#include <cstdio>  

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

void BattleManager::NextTurn()
{
    // 1) 디버프 Tick
    Player->TickDebuff();
    Enemy->TickDebuff();

    // 2) ST +5 자연회복
    Player->RegenST();
    Enemy->RegenST();

    // 3) 행동 선택
    Player->TakeTurn();  // (플레이어 입력 기반, 나중에 구현)
    Enemy->TakeTurn();   // (적 AI 기반, 나중에 구현)

    // 4) 판정 수행
    ResolveActions();

    // 5) 상태 출력
    PrintStatus();

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

void BattleManager::PrintStatus() const
{
    printf("\n[상태 출력]\n");
    printf("%s: HP=%d/%d, ST=%d/%d, ATK=%d, DEF=%d\n",
        Player->GetName().c_str(), Player->GetHP(), Player->GetMaxHP(),
        Player->GetST(), Player->GetMaxST(), Player->GetATK(), Player->GetDEF());

    printf("%s: HP=%d/%d, ST=%d/%d, ATK=%d, DEF=%d\n",
        Enemy->GetName().c_str(), Enemy->GetHP(), Enemy->GetMaxHP(),
        Enemy->GetST(), Enemy->GetMaxST(), Enemy->GetATK(), Enemy->GetDEF());
}

void BattleManager::ResolveActions()
{
    // ⚠️ TODO: 전투 규칙 구현
    // 공격 vs 공격, 공격 vs 가드, 회피 등 규칙은 여기서 작성
    // 현재는 임시로 "플레이어가 적을 단순 공격"만 처리

    int Damage = Player->GetATK() - Enemy->GetDEF();
    if (Damage < 0) Damage = 0;

    Enemy->TakeDamage(Damage);

    printf("%s 가 %s 를 공격! 피해 %d\n",
        Player->GetName().c_str(), Enemy->GetName().c_str(), Damage);
}