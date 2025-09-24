#pragma once
#include "Actor.h"

// 적 행동(동시 선택)
enum class EnemyAct { None, Attack, Guard, Dodge };
// 공격 방향/종류는 Player와 동일 타입 재사용
enum class EnemyPersona { Aggressive, Defensive, Balanced };
enum class TelegraphType { None, AggressiveCue, DefensiveCue, BalancedCue };

class Enemy : public Actor
{
//public:
//    using Actor::Actor;
//    virtual ~Enemy() {}
//
//    void TakeTurn() override;  // AI가 성향/전조/스태미너로 행동 결정
//
//    // 전투 매니저가 세팅/조회
//    void SetPersona(EnemyPersona p) { Persona = p; }
//    void SetTelegraph(TelegraphType t) { Telegraph = t; }
//    void SetTarget(Actor* t) { Target = t; }
//
//    EnemyPersona  GetPersona()   const { return Persona; }
//    TelegraphType GetTelegraph() const { return Telegraph; }
//    Actor* GetTarget()    const { return Target; }
//
//    // (선택) 기록/디버깅용
//    EnemyAct   GetLastAct()  const { return Last.Act; }
//    AttackDir  GetLastDir()  const { return Last.Dir; }
//    AttackKind GetLastKind() const { return Last.Kind; }
//
//private:
//    EnemyPersona  Persona = EnemyPersona::Balanced;
//    TelegraphType Telegraph = TelegraphType::None;
//    Actor* Target = nullptr;
//
//    struct Chosen {
//        EnemyAct   Act = EnemyAct::None;     // Attack / Guard / Dodge
//        AttackDir  Dir = AttackDir::Mid;     // 상/중/하
//        AttackKind Kind = AttackKind::Normal; // 일반/강공
//    };
//    Chosen Last; // 직전 턴 기록
};