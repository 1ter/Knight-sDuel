// Enemy.cpp
#include "Enemy.h"
#include <cstdio>
#include <cstdlib> // rand
#include <ctime>   // time (��򰡿��� srand(...) 1���� ȣ��)

static int RandomRange(int min, int max)
{
    return min + (std::rand() % (max - min + 1));
}

void Enemy::TakeTurn()
{
    // 30% ����(�ڷ��׷���) �ߵ�
    const bool telegraph = (RandomRange(1, 100) <= 30);

    if (telegraph)
    {
        switch (stance)
        {
        case Stance::Aggressive:
            std::printf("[����] Į������ ���� �̴� �Ҹ��� �鸳�ϴ١�\n");
            break;
        case Stance::Defensive:
            std::printf("[����] ���и� �ܴ��� ������� ��ô�� ���ϴ١�\n");
            break;
        case Stance::Balanced:
            std::printf("[����] ��밡 ȣ���� ���� �������� ���̴ϴ١�\n");
            break;
        }
    }

    // ���⺰ �⺻ Ȯ���� �ൿ ���� (1~100)
    int r = RandomRange(1, 100);
    ActionType act = ActionType::Guard;
    AttackKind kind = AttackKind::Normal;

    switch (stance)
    {
    case Stance::Aggressive: // �Ϲ� 60, ���� 20, ���� 20 (+������ ���� +30)
        if (telegraph && r <= 30) { act = ActionType::Attack; kind = AttackKind::Normal; }
        else if (r <= 60) { act = ActionType::Attack; kind = AttackKind::Normal; }
        else if (r <= 80) { act = ActionType::Guard; }
        else { act = ActionType::Attack; kind = AttackKind::Strong; }
        break;

    case Stance::Defensive: // ���� 60, �Ϲ� 20, ���� 20 (+������ ���� +30)
        if (telegraph && r <= 30) { act = ActionType::Guard; }
        else if (r <= 60) { act = ActionType::Guard; }
        else if (r <= 80) { act = ActionType::Attack; kind = AttackKind::Normal; }
        else { act = ActionType::Attack; kind = AttackKind::Strong; }
        break;

    case Stance::Balanced: // �Ϲ� 40, ���� 40, ���� 20 (+������ �Ϲ�/���� ���� +15)
    default:
        if (telegraph && r <= 15) { act = ActionType::Attack; kind = AttackKind::Normal; }
        else if (telegraph && r <= 30) { act = ActionType::Guard; }
        else if (r <= 40) { act = ActionType::Attack; kind = AttackKind::Normal; }
        else if (r <= 80) { act = ActionType::Guard; }
        else { act = ActionType::Attack; kind = AttackKind::Strong; }
        break;
    }

    // ST ��� ���� (���� ����)
    const int COST_ATTACK_N = 5;
    const int COST_ATTACK_S = 25;
    const int COST_DODGE = 30;

    if (act == ActionType::Attack)
    {
        if (kind == AttackKind::Strong && ST < COST_ATTACK_S)
        {
            if (ST >= COST_ATTACK_N) kind = AttackKind::Normal; // ���� �� �Ϲݷ� �ٿ�׷��̵�
            else                     act = ActionType::Guard;  // �� �� �Ұ� �� ����
        }
        else if (kind == AttackKind::Normal && ST < COST_ATTACK_N)
        {
            act = ActionType::Guard;
        }
    }
    else if (act == ActionType::Dodge)
    {
        if (ST < COST_DODGE) act = ActionType::Guard;
    }

    // ���� ���� ����
    choice.Action = act;

    if (act == ActionType::Attack || act == ActionType::Guard)
        choice.Direction = static_cast<CombatDirection>(RandomRange(0, 2)); // High/Mid/Low
    else
        choice.Direction = CombatDirection::Mid; // ȸ�Ǵ� �ǹ� ���� �� �⺻��

    choice.Kind = (act == ActionType::Attack ? kind : AttackKind::Normal);
    choice.Target = nullptr; // ��� �����ʹ� ��Ʋ �Ŵ������� �ؼ�
}
