#pragma once
#include "Actor.h"

// 공격 방향
enum class AttackDir { High, Mid, Low };
// 공격 종류
enum class AttackKind { Normal, Strong };
// 플레이어 행동(동시 선택)
enum class PlayerAct { None, Attack, Guard, Dodge };

class Player : public Actor
{
public:
    using Actor::Actor;
    virtual ~Player() {}

    void TakeTurn() override;

    // --- 입력/선택 API (턴 시작~공개 전까지 외부에서 1회 설정) ---
    // 공격: 방향 + (일반/강공)
    void PlanAttack(AttackDir dir, AttackKind kind, Actor* target) {
        Planned.Act = PlayerAct::Attack;
        Planned.Dir = dir;
        Planned.Kind = kind;
        Planned.Target = target;
        Planned.Param = 0;
    }

    // 가드: 방향(상/중/하)
    void PlanGuard(AttackDir dir) {
        Planned.Act = PlayerAct::Guard;
        Planned.Dir = dir;
        Planned.Kind = AttackKind::Normal; // 의미 없음
        Planned.Target = nullptr;
        Planned.Param = 0;
    }

    // 회피
    void PlanDodge() {
        Planned.Act = PlayerAct::Dodge;
        Planned.Dir = AttackDir::Mid;     // 의미 없음
        Planned.Kind = AttackKind::Normal; // 의미 없음
        Planned.Target = nullptr;
        Planned.Param = 0;
    }

    void ClearPlan() { Planned = {}; }

    // (선택) 조회
    PlayerAct  GetPlannedAct()  const { return Planned.Act; }
    AttackDir  GetPlannedDir()  const { return Planned.Dir; }
    AttackKind GetPlannedKind() const { return Planned.Kind; }
    Actor* GetPlannedTarget() const { return Planned.Target; }
    int        GetPlannedParam() const { return Planned.Param; }

private:
    struct Plan 
    {
        PlayerAct  Act = PlayerAct::None;     // Attack / Guard / Dodge
        AttackDir  Dir = AttackDir::Mid;      // 상/중/하 (가드/공격에 사용)
        AttackKind Kind = AttackKind::Normal;  // 일반/강공 (공격에 사용)
        Actor* Target = nullptr;           // 공격 대상
        int        Param = 0;                 // 확장용(스킬ID 등, 지금은 미사용)
    };
    Plan Planned;
};

