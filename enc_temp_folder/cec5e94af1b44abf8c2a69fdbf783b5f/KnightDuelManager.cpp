#include "KnightDuelManager.h"   
#include <iostream>
#include <cstdlib>
#include <ctime>


static inline int RandRange(int min, int max)
{
    return min + (std::rand() % (max - min + 1)); 
}

// �÷��̾� �Է� 
void KnightDuelManager::PlayerTurn(BattleManager& InBM)
{
    int InputAction = 0;
    std::printf("�ൿ ����: 1) ����  2) ����  3) ȸ��\n> ");
    std::cin >> InputAction;

    if (InputAction == 1) 
    {
        int InputAttack;
        std::printf("���� ����: 1) �Ϲ� ����  2) ���� ����\n> ");
        std::cin >> InputAttack;

        int InputDirection = 0;
        if (InputAttack == 1) {
            std::printf("���� ����: 1) ����ġ��(���) 2) ���(�ߴ�) 3) �����(�ϴ�)\n> ");
            std::cin >> InputDirection;
            InBM.PlanPlayerAttack(static_cast<CombatDirection>(InputDirection - 1), AttackKind::Normal);
        }
        else 
        {
            std::printf("���� ����: 1) ��Ÿ(���)  2) ����(�ߴ�)  3) ����(�ϴ�)\n> ");
            std::cin >> InputDirection;
            InBM.PlanPlayerAttack(static_cast<CombatDirection>(InputDirection - 1), AttackKind::Strong);
        }
    }
    else if (InputAction == 2) 
    {
        int InputDirection = 0;
        std::printf("���� ����: 1) ��ܸ���  2) �ߴܸ���  3) �ϴܸ���\n> ");
        std::cin >> InputDirection;
        InBM.PlanPlayerGuard(static_cast<CombatDirection>(InputDirection - 1));
    }
    else 
    {
        InBM.PlanPlayerDodge();
    }
}

// ���� ��� ���
void KnightDuelManager::PrintResult(bool IsWin)
{
    if (IsWin)
    {
        std::printf("\n �¸�! ���� ����� �����մϴ�.\n");
    }
    else
    {
        std::printf("\n �й�... ���� ����.\n");
    }
}

// ���� ���� (3����, �� ���� Ŭ���� �� ȸ��/�ʱ�ȭ)
void KnightDuelManager::RunGame()
{
    static const char* Names[3] = 
    {
        "��ǳ�� ��� �巹��ũ",
        "ȥ���� ��� ����",
        "�Ҹ��� ��� �˵帯"
    };

    for (int Round = 0; Round < 3; Round++)
    {
        // ���� ����
        Stance stance = static_cast<Stance>(rand() % 3);

        // ���� ���� (HP 100, ST 100, ATK 17~20, DEF 7~10)
        Enemy enemy(Names[Round], 100, 100, RandRange(17, 20), RandRange(7, 10), stance);

        const char* StanceChar = nullptr;
        switch (stance)
        {
        case Stance::Aggressive:
            StanceChar = "������";
            break;
        case Stance::Defensive:
            StanceChar = "�����";
            break;
        case Stance::Balanced:
        default:
            StanceChar = "������";
            break;
        }
        printf("���������������������������������������������� %d / 3������������������������������������������������\n");
        printf("\n                 %s (����: %s) \n", 
                         Round + 1, Names[Round], StanceChar);
        printf("�÷��̾� ATK:%d  DEF:%d\n", MainPlayer.GetATK(), MainPlayer.GetDEF());
        printf("��       ATK:%d  DEF:%d\n", enemy.GetATK(), enemy.GetDEF());
        printf("������������������������������������������������������������������������������������������������������������������\n");
        printf("\n\n");

        BattleManager battlemanager(MainPlayer);
        battlemanager.SetEnemy(enemy);

        // ���� ����
        while (!battlemanager.IsBattleEnd())
        {
            battlemanager.StartTurn();
            PlayerTurn(battlemanager);     // �÷��̾� �Է�
            battlemanager.ExecuteTurn();
            battlemanager.PrintStatus();
        }

        bool IsWin = !MainPlayer.IsDead();
        PrintResult(IsWin);
        if (!IsWin)
        {
            return;
        }

        // 2������� ȸ��/�ʱ�ȭ
        if (Round < 2) 
        {
            int RecoveryHP = RandRange(50, 100);
            int RecoveryST = RandRange(50, 100);
            MainPlayer.RoundResetPlayer(RecoveryHP, RecoveryST); // HP/ST ȸ��, ����� �ʱ�ȭ(���� ����)
            std::printf("���� ȸ��: HP +%d, ST +%d (����� �ʱ�ȭ)\n", RecoveryHP, RecoveryST);
        }
    }

    std::printf("\n��� ��縦 óġ! ����� è�Ǿ��Դϴ�!\n");
}
