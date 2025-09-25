#pragma once
#include "Actor.h"

// �÷��̾� Ŭ����
class Player : public Actor
{
public:
    Player(const std::string& InName, int InHP, int InST, int InATK, int InDEF)
        : Actor(InName, InHP, InST, InATK, InDEF)  
    {
    }
    virtual ~Player() {}  
    virtual void ClearChoice() override { choice = {}; }  // ���� �ʱ�ȭ
    virtual void TakeTurn() override; // �Է� ��: ��� ������ �⺻��(����/�ߴ�) ����

    // �ൿ ��ȹ 
    // ����
    void ChoiceAttack(CombatDirection InDirection, AttackKind InKind, Actor* InTarget)
    {
        choice.Action = ActionType::Attack;
        choice.Direction = InDirection;
        choice.Kind = InKind;
        choice.Target = InTarget;
    }
    // ����
    void ChoiceGuard(CombatDirection InDirection)
    {
        choice.Action = ActionType::Guard;
        choice.Direction = InDirection;
    }
    // ȸ��
    void ChoiceDodge()
    {
        choice.Action = ActionType::Dodge;
    }
};
