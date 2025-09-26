#include "Enemy.h"
#include <cstdlib>

// Ȯ�� 
static inline int RandRange() // 0~99
{
    return rand() % 100;
}
static inline int Rand3()     // ���� (High/Mid/Low)
{ 
    return rand() % 3; 
}

// ����(�ڷ��׷���) 30% �ߵ�
bool Enemy::TelegraphRoll() const
{
    return RandRange() < TelegraphPercent;  // 30%
}

// ���� �⺻ ����(���� ����)
void Enemy::BaseDistribution(Stance InStance, int& OutNormal, int& OutStrong, int& OutGuard) const
{
    switch (InStance)
    {
    case Stance::Aggressive: // �Ϲ� 60%, ���� 20%, ���� 20%
        OutNormal = 60; OutStrong = 20; OutGuard = 20;
        break;
    case Stance::Defensive:  // �Ϲ� 20%, ���� 20%, ���� 60%
        OutNormal = 20; OutStrong = 20; OutGuard = 60;
        break;
    case Stance::Balanced:   // �Ϲ� 40%, ���� 20%, ���� 40%
    default:
        OutNormal = 40; OutStrong = 20; OutGuard = 40;
        break;
    }
}

// ������ ���� �� '��� ����'
void Enemy::TelegraphDistribution(Stance InStance, int& OutNormal, int& OutStrong, int& OutGuard) const
{
    switch (InStance)
    {
    case Stance::Aggressive: // �Ϲ� 70%, ���� 20%, ���� 10%
        OutNormal = 70; OutStrong = 20; OutGuard = 10;
        break;
    case Stance::Defensive:  // �Ϲ� 30%, ���� 20%, ���� 50%
        OutNormal = 30; OutStrong = 20; OutGuard = 50;
        break;
    case Stance::Balanced:   // �Ϲ� 50%, ���� 20%, ���� 30%
    default:
        OutNormal = 50; OutStrong = 20; OutGuard = 30;
        break;
    }
}

// �ۼ�Ʈ ������ (�ൿ/��������) ����
ActionType Enemy::PickAction(int InNormalPct, int InStrongPct, int InGuardPct, AttackKind& OutKind) const
{
    int ActionRoll = RandRange();       // 0~99

    if (ActionRoll < InNormalPct)      
    {
        OutKind = AttackKind::Normal;   // �Ϲ� ����
        return ActionType::Attack;
    }
    if (ActionRoll < InNormalPct + InStrongPct)
    {
        OutKind = AttackKind::Strong;   // ���� ����
        return ActionType::Attack;
    }

    return ActionType::Guard;           // ���� 
}

// ����,���� (High/Mid/Low) ������
CombatDirection Enemy::PickDirection() const
{
    switch (Rand3())
    {
    case 0: 
        return CombatDirection::High;
    case 1: 
        return CombatDirection::Mid;
    default: 
        return CombatDirection::Low;
    }
}

//ST ���� �� �ڵ� ����(����->�Ϲ�->����), ����� ��� ����
void Enemy::CheckStamina(ActionType& OutAct, AttackKind& OutKind, int InST) const
{
    if (OutAct == ActionType::Attack)
    {
        if (OutKind == AttackKind::Strong)
        {
            if (InST < 25)                         // ���� ST �����ҽ�
            {
                if (InST >= 5)
                {
                    OutKind = AttackKind::Normal;  // ���� -> �Ϲ�
                }
                else
                {
                    OutAct = ActionType::Guard;    // ST ������ ����
                }
            }
        }
        else // �Ϲ� ����
        {
            if (InST < 5)
            {
                OutAct = ActionType::Guard;        // ST ������ ����
            }
        }

        return;
    }

    if (OutAct == ActionType::Dodge)
    {
        if (InST < 30) // ȸ�� ST �����ҽ� ����
        {
            OutAct = ActionType::Guard;
        }
        return;
    }
}

// �� (����/����/����) �ൿ ���� 
void Enemy::TakeTurn()
{
    // �⺻ ���� �ʱ�ȭ ClearChoice()
    // ���⼭ (����/����/����) ������� choice ����
    int NormalPct = 0;
    int StrongPct = 0;
    int GuardPct = 0;

    // ���� �߻�
    IsTelegraphTurn = false;           //���� �� �� �ʱ�ȭ
    bool Telegraph = TelegraphRoll();
    IsTelegraphTurn = Telegraph;       // ���� �߻� ���� ����

    if (Telegraph) // ���� ����(��ð�) ���� , ���� ���
    {
        TelegraphDistribution(stance, NormalPct, StrongPct, GuardPct);
    }
    else           // �⺻ ����
    {
        BaseDistribution(stance, NormalPct, StrongPct, GuardPct); 
    }

    AttackKind SelectedKind{};                              // ���� ���� ���� ���� (PickAction�� ���� ä��)
    ActionType SelectedAction = PickAction(NormalPct, StrongPct, GuardPct, SelectedKind); // (����/����) ����
    CombatDirection SelectedDirection = PickDirection();   // ���� ���� (����/����)
    CheckStamina(SelectedAction, SelectedKind, ST);        // ���¹̳� �˻� �� �ڵ� ����

    // ���� ���� ����
    choice.Action = SelectedAction;              // ����/����/ȸ��
    choice.Direction = SelectedDirection;        // ��/��/��

    if (SelectedAction == ActionType::Attack)    // ������ ���� (�Ϲ�/����) ���� 
    {
        choice.Kind = SelectedKind;         
    }
}
