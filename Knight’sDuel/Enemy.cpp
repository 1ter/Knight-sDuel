#include "Enemy.h"
#include <random>


/*
  - ����: ���� ����
       ������: �Ϲ� 60%, ���� 20%, ���� 20%
       �����: ���� 60%, �Ϲ� 20%, ���� 20%
       ������: �Ϲ� 40%, ���� 40%, ���� 20%

   - ����(�ڷ��׷���): ��ȭ�� ���� �ɸ��� ��ȭ
       ����) [Į������ ���� �̴� �Ҹ��� �鸳�ϴ�.]
       ������ ���� (��� ��� ���� Ȯ�� 30% + �Ϲݰ��� Ȯ�� 60% = ���� Ȯ�� 90%)

    �� : ���� Į�ڷ縦 ���� ������ �����δ�
         ������ ������ ���� ����� ������ �ܴ���
         Į������ ���� �̴� �Ҹ��� �鸰��
         ����� ��ƴ�� �븮�� �� �� ������ ����
    ��:  ���и� �ܴ��� ������� ��ô�� ����
         ���и� �� �տ� ���� ���� . 
         ����� ���̰� ��������� ���� �ܴ��� ���ٵ�´�
         ��ȣ���� ��Ű�� ���� �ڷ� �ü��� �����
    ���� :��밡 ȣ���� ���� �������� ���ɴ�
          ���� �ൿ�� ����� �ڼ��� ���ٵ�´�
          ������ �������� ����� �������� ���Ѻ���
          �߰����� ���߰� ȣ���� ���ٵ�´�
*/

// ���� ���� ���� ����
static int RandomRange(int Min, int Max)
{
    return Min + rand() % (Max - Min + 1);
}

void Enemy::TakeTurn()
{
    const bool telegraph = (RandomRange(1, 100) <= 30); // ���� Ȯ�� 30��

    if (telegraph)
    {
        switch (stance)
        {
        case Stance::Aggressive:
            printf("");
        case Stance::Defensive:
            printf("");
        case Stance::Balanced:
            printf("");
            break;
    }


    // ����(Stance)�� ���� �⺻ ���� Ȯ��
    int EnemyStanceRoll = RandomRange(1, 100);

    ActionType Act = ActionType::Guard;
    AttackKind Kind = AttackKind::Normal;



        switch (stance)
        {
        case Stance::Aggressive:
        {
        }
    case Stance::Aggressive:       // ������: �Ϲ� 60%, ���� 20%, ���� 20%
        if (EnemyStanceRoll <= 60)
        {
            Act = ActionType::Attack;
            Kind = AttackKind::Normal;
        }
        else if (EnemyStanceRoll <= 80)
        {
            Act = ActionType::Guard;
        }
        else
        {
            Act = ActionType::Attack;
            Kind = AttackKind::Strong;
        }
        break;

    case Stance::Defensive:       // �����: ���� 60%, �Ϲ� 20%, ���� 20%
        if (EnemyStanceRoll <= 60)
        {
            Act = ActionType::Guard;
        }
        else if (EnemyStanceRoll <= 80)
        {
            Act = ActionType::Attack;
            Kind = AttackKind::Normal;
        }
        else
        {
            Act = ActionType::Attack;
            Kind = AttackKind::Strong;
        }
        break;

    case Stance::Balanced:    // ������: �Ϲ� 40%, ���� 40%, ���� 20%
    default:
        if (EnemyStanceRoll <= 40)
        {
            Act = ActionType::Attack;
            Kind = AttackKind::Normal;
        }
        else if (EnemyStanceRoll <= 80)
        {
            Act = ActionType::Guard;
        }
        else
        {
            Act = ActionType::Attack;
            Kind = AttackKind::Strong;
        }
        break;
    }

    // ST(���¹̳�) ����
    const int AttackNormalCost = 5;
    const int AttackStrongCost = 25;
    const int DodgeCost = 30;

    if (Act == ActionType::Attack)
    {
        if (Kind == AttackKind::Strong && ST < AttackStrongCost)
        {
            // ST �����ϸ� ���� ���� �Ұ� -> �Ϲ� ���ݵ� �ȵǸ� -> ����
            if (ST >= AttackNormalCost)
            {
                Kind = AttackKind::Normal;
            }
            else
            {
                Act = ActionType::Guard;
            }
        }
        else if (Kind == AttackKind::Normal && ST < AttackNormalCost)
        {
            Act = ActionType::Guard;
        }
    }
    else if (Act == ActionType::Dodge)
    {
        if (ST < DodgeCost)
        {
            Act = ActionType::Guard;
        }
    }

    choice.Action = Act;   // ���� ���� �ϱ�

    if (Act == ActionType::Attack || Act == ActionType::Guard)
    {
        // High, Mid, Low Ȯ�� ����
        choice.Direction = static_cast<CombatDirection>(RandomRange(0, 2));
    }
    if (Act == ActionType::Attack) 
    {
        choice.Kind = Kind;
    }
    else 
    {
        choice.Kind = AttackKind::Normal;
    }    
    choice.Target = nullptr;
}



