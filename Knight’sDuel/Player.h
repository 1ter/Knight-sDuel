#pragma once
#include "Actor.h"

// ���� ����
enum class AttackDir { High, Mid, Low };
// ���� ����
enum class AttackKind { Normal, Strong };
// �÷��̾� �ൿ(���� ����)
enum class PlayerAct { None, Attack, Guard, Dodge };

class Player : public Actor
{
public:
    using Actor::Actor;
    virtual ~Player() {}

    void TakeTurn() override;

    // --- �Է�/���� API (�� ����~���� ������ �ܺο��� 1ȸ ����) ---
    // ����: ���� + (�Ϲ�/����)
    void PlanAttack(AttackDir dir, AttackKind kind, Actor* target) {
        Planned.Act = PlayerAct::Attack;
        Planned.Dir = dir;
        Planned.Kind = kind;
        Planned.Target = target;
        Planned.Param = 0;
    }

    // ����: ����(��/��/��)
    void PlanGuard(AttackDir dir) {
        Planned.Act = PlayerAct::Guard;
        Planned.Dir = dir;
        Planned.Kind = AttackKind::Normal; // �ǹ� ����
        Planned.Target = nullptr;
        Planned.Param = 0;
    }

    // ȸ��
    void PlanDodge() {
        Planned.Act = PlayerAct::Dodge;
        Planned.Dir = AttackDir::Mid;     // �ǹ� ����
        Planned.Kind = AttackKind::Normal; // �ǹ� ����
        Planned.Target = nullptr;
        Planned.Param = 0;
    }

    void ClearPlan() { Planned = {}; }

    // (����) ��ȸ
    PlayerAct  GetPlannedAct()  const { return Planned.Act; }
    AttackDir  GetPlannedDir()  const { return Planned.Dir; }
    AttackKind GetPlannedKind() const { return Planned.Kind; }
    Actor* GetPlannedTarget() const { return Planned.Target; }
    int        GetPlannedParam() const { return Planned.Param; }

private:
    struct Plan 
    {
        PlayerAct  Act = PlayerAct::None;     // Attack / Guard / Dodge
        AttackDir  Dir = AttackDir::Mid;      // ��/��/�� (����/���ݿ� ���)
        AttackKind Kind = AttackKind::Normal;  // �Ϲ�/���� (���ݿ� ���)
        Actor* Target = nullptr;           // ���� ���
        int        Param = 0;                 // Ȯ���(��ųID ��, ������ �̻��)
    };
    Plan Planned;
};

