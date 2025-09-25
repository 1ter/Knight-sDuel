#pragma once
#include "Actor.h"
#include <vector>
#include <string>

// 적 성향
enum class Stance
{
    Aggressive, // 공격형
    Defensive,  // 방어형
    Balanced    // 균형형
};

// 전조(텔레그래프) 대사 모음 (헤더 다중 포함 안전: inline)
inline const std::vector<std::string> AttackLines =
{
    "손이 칼자루를 향해 서서히 움직인다",
    "차갑게 빛나는 눈이 상대의 약점을 겨눈다",
    "칼집에서 검을 뽑는 소리가 들린다",
    "상대의 빈틈을 노리듯 한 발 앞으로 디딘다"
};

inline const std::vector<std::string> DefensiveLines =
{
    "방패를 단단히 고쳐잡는 기척이 난다",
    "방패를 쥔 손에 힘이 들어간다",
    "어깨가 무겁게 움츠러들며 몸을 단단히 가다듬는다",
    "심호흡을 삼키며 방패 뒤로 시선을 감춘다"
};

inline const std::vector<std::string> BalancedLines =
{
    "상대가 호흡을 고르며 움직임을 살핀다",
    "다음 행동을 숨기듯 자세를 가다듬는다",
    "차분한 눈빛으로 상대의 움직임을 지켜본다",
    "발걸음을 멈추고 호흡을 가다듬는다"
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

    Stance GetStance() const { return stance; }          // 상태 확인

private:
    // 전조(텔레그래프) 30% 발동
    bool TelegraphRoll() const;

    // 성향별 전조 대사 한 줄 반환
    const std::string& TelegraphLinePick(Stance InStance) const;

    // 성향 기본 분포(전조 없음)
    void BaseDistribution(Stance InStance, int& OutNormal, int& OutStrong, int& OutGuard) const;

    // 전조가 떴을 때 '명시 분포' (공격형: 70/20/10, 방어형: 30/20/50, 균형형: 50/20/30)
    void TelegraphDistribution(Stance InStance, int& InOutNormal, int& InOutStrong, int& InOutGuard) const;

    // 퍼센트 분포로 행동/공격종류 결정
    ActionType PickAction(int NormalPct, int StrongPct, int GuardPct, AttackKind& OutKind) const;

    // High/Mid/Low 무작위
    CombatDirection PickDirection() const;

    // ST 부족 시 자동 강등 (강공->일반->가드)
    void CheckStamina(ActionType& InOutAct, AttackKind& InOutKind, int InST) const;

private:
    Stance stance;
    static constexpr int TelegraphPercent = 30;    // 전조 발동률(30%)
};


