#pragma once
#include "Actor.h"

// �� �ൿ(���� ����)
enum class EnemyAct { None, Attack, Guard, Dodge };
// ���� ����/������ Player�� ���� Ÿ�� ����
enum class EnemyPersona { Aggressive, Defensive, Balanced };
enum class TelegraphType { None, AggressiveCue, DefensiveCue, BalancedCue };

class Enemy : public Actor
{
//public:
//    using Actor::Actor;
//    virtual ~Enemy() {}
//
//    void TakeTurn() override;  // AI�� ����/����/���¹̳ʷ� �ൿ ����
//
//    // ���� �Ŵ����� ����/��ȸ
//    void SetPersona(EnemyPersona p) { Persona = p; }
//    void SetTelegraph(TelegraphType t) { Telegraph = t; }
//    void SetTarget(Actor* t) { Target = t; }
//
//    EnemyPersona  GetPersona()   const { return Persona; }
//    TelegraphType GetTelegraph() const { return Telegraph; }
//    Actor* GetTarget()    const { return Target; }
//
//    // (����) ���/������
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
//        AttackDir  Dir = AttackDir::Mid;     // ��/��/��
//        AttackKind Kind = AttackKind::Normal; // �Ϲ�/����
//    };
//    Chosen Last; // ���� �� ���
};