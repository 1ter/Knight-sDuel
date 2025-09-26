#include "KnightDuelManager.h"   
#include <iostream>
#include <cstdlib>
#include <ctime>


static inline int RandRange(int min, int max)
{
    return min + (std::rand() % (max - min + 1)); 
}

// 플레이어 입력 
void KnightDuelManager::PlayerTurn(BattleManager& InBM)
{
    int InputAction = 0;
    std::printf("행동 선택: 1) 공격  2) 가드  3) 회피\n> ");
    std::cin >> InputAction;

    if (InputAction == 1) 
    {
        int InputAttack;
        std::printf("공격 종류: 1) 일반 공격  2) 강한 공격\n> ");
        std::cin >> InputAttack;

        int InputDirection = 0;
        if (InputAttack == 1) {
            std::printf("방향 선택: 1) 내려치기(상단) 2) 찌르기(중단) 3) 쓸어베기(하단)\n> ");
            std::cin >> InputDirection;
            InBM.PlanPlayerAttack(static_cast<CombatDirection>(InputDirection - 1), AttackKind::Normal);
        }
        else 
        {
            std::printf("방향 선택: 1) 강타(상단)  2) 관통(중단)  3) 절단(하단)\n> ");
            std::cin >> InputDirection;
            InBM.PlanPlayerAttack(static_cast<CombatDirection>(InputDirection - 1), AttackKind::Strong);
        }
    }
    else if (InputAction == 2) 
    {
        int InputDirection = 0;
        std::printf("가드 방향: 1) 상단막기  2) 중단막기  3) 하단막기\n> ");
        std::cin >> InputDirection;
        InBM.PlanPlayerGuard(static_cast<CombatDirection>(InputDirection - 1));
    }
    else 
    {
        InBM.PlanPlayerDodge();
    }
}

// 전투 결과 출력
void KnightDuelManager::PrintResult(bool IsWin)
{
    if (IsWin)
    {
        std::printf("\n 승리! 다음 라운드로 진입합니다.\n");
    }
    else
    {
        std::printf("\n 패배... 게임 종료.\n");
    }
}

// 게임 실행 (3라운드, 각 라운드 클리어 시 회복/초기화)
void KnightDuelManager::RunGame()
{
    static const char* Names[3] = 
    {
        "폭풍의 기사 드레이크",
        "혼돈의 기사 로웰",
        "불멸의 기사 알드릭"
    };

    for (int Round = 0; Round < 3; Round++)
    {
        // 성향 랜덤
        Stance stance = static_cast<Stance>(rand() % 3);

        // 스탯 랜덤 (HP 100, ST 100, ATK 17~20, DEF 7~10)
        Enemy enemy(Names[Round], 100, 100, RandRange(17, 20), RandRange(7, 10), stance);

        const char* StanceChar = nullptr;
        switch (stance)
        {
        case Stance::Aggressive:
            StanceChar = "공격형";
            break;
        case Stance::Defensive:
            StanceChar = "방어형";
            break;
        case Stance::Balanced:
        default:
            StanceChar = "균형형";
            break;
        }
        printf("─────────────────────라운드 %d / 3────────────────────────\n");
        printf("\n                 %s (성향: %s) \n", 
                         Round + 1, Names[Round], StanceChar);
        printf("플레이어 ATK:%d  DEF:%d\n", MainPlayer.GetATK(), MainPlayer.GetDEF());
        printf("적       ATK:%d  DEF:%d\n", enemy.GetATK(), enemy.GetDEF());
        printf("─────────────────────────────────────────────────────────\n");
        printf("\n\n");

        BattleManager battlemanager(MainPlayer);
        battlemanager.SetEnemy(enemy);

        // 전투 루프
        while (!battlemanager.IsBattleEnd())
        {
            battlemanager.StartTurn();
            PlayerTurn(battlemanager);     // 플레이어 입력
            battlemanager.ExecuteTurn();
            battlemanager.PrintStatus();
        }

        bool IsWin = !MainPlayer.IsDead();
        PrintResult(IsWin);
        if (!IsWin)
        {
            return;
        }

        // 2라운드까지 회복/초기화
        if (Round < 2) 
        {
            int RecoveryHP = RandRange(50, 100);
            int RecoveryST = RandRange(50, 100);
            MainPlayer.RoundResetPlayer(RecoveryHP, RecoveryST); // HP/ST 회복, 디버프 초기화(경직 복구)
            std::printf("라운드 회복: HP +%d, ST +%d (디버프 초기화)\n", RecoveryHP, RecoveryST);
        }
    }

    std::printf("\n모든 기사를 처치! 당신은 챔피언입니다!\n");
}
