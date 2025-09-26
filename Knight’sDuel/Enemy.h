#pragma once
#include "Actor.h"
#include <vector>
#include <string>

// 적 성향
//  Aggressive: 공격 성향 (공격 확률 ↑)
//  Defensive : 방어 성향 (가드 확률 ↑)
//  Balanced  : 균형 성향 (공/수 균형)
enum class Stance
{
    Aggressive, // 공격형
    Defensive,  // 방어형
    Balanced    // 균형형
};

// Enemy 클래스
class Enemy : public Actor
{
public:
    Enemy(const std::string& InName, int InHP, int InST, int InATK, int InDEF, Stance InStance)
        : Actor(InName, InHP, InST, InATK, InDEF)
        , stance(InStance)
    {
    }

    virtual ~Enemy() {}
    virtual void ClearChoice() override { choice = {}; } // 선택 초기화
    virtual void TakeTurn() override;                    // (성향/전조/분포) 행동 선택 

    // 현재 성향 반환
    Stance GetStance() const { return stance; }          
    // 전조 발생 여부 (대사는 배틀매니저에서)
    bool GetTelegraphTurn() const { return IsTelegraphTurn; }

private:
    // 전조(텔레그래프) 30% 발동
    bool TelegraphRoll() const;

    // 성향 기본 분포(전조 없음)
    void BaseDistribution(Stance InStance, int& OutNormal, int& OutStrong, int& OutGuard) const;

    // 전조가 떴을 때 '명시 분포' (공격형: 70/20/10, 방어형: 30/20/50, 균형형: 50/20/30)
    void TelegraphDistribution(Stance InStance, int& InOutNormal, int& InOutStrong, int& InOutGuard) const;

    // 퍼센트 분포로 행동/공격종류 결정
    ActionType PickAction(int NormalPct, int StrongPct, int GuardPct, AttackKind& OutKind) const;

    // 무작위 방향 선택 High/Mid/Low 
    CombatDirection PickDirection() const;

    // ST 부족 시 자동 강등 (강공->일반->가드)
    void CheckStamina(ActionType& InOutAct, AttackKind& InOutKind, int InST) const;

private:
    Stance stance;                                 // 적 성향
    static constexpr int TelegraphPercent = 30;    // 전조 발동률(30%)
    bool IsTelegraphTurn = false;                  // 이번 턴 전조 발생 여부
};


