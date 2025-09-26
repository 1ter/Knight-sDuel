#include "Enemy.h"
#include <cstdlib>

// 확률 
static inline int RandRange() // 0~99
{
    return rand() % 100;
}
static inline int Rand3()     // 방향 (High/Mid/Low)
{ 
    return rand() % 3; 
}

// 전조(텔레그래프) 30% 발동
bool Enemy::TelegraphRoll() const
{
    return RandRange() < TelegraphPercent;  // 30%
}

// 성향 기본 분포(전조 없음)
void Enemy::BaseDistribution(Stance InStance, int& OutNormal, int& OutStrong, int& OutGuard) const
{
    switch (InStance)
    {
    case Stance::Aggressive: // 일반 60%, 강공 20%, 가드 20%
        OutNormal = 60; OutStrong = 20; OutGuard = 20;
        break;
    case Stance::Defensive:  // 일반 20%, 강공 20%, 가드 60%
        OutNormal = 20; OutStrong = 20; OutGuard = 60;
        break;
    case Stance::Balanced:   // 일반 40%, 강공 20%, 가드 40%
    default:
        OutNormal = 40; OutStrong = 20; OutGuard = 40;
        break;
    }
}

// 전조가 떴을 때 '명시 분포'
void Enemy::TelegraphDistribution(Stance InStance, int& OutNormal, int& OutStrong, int& OutGuard) const
{
    switch (InStance)
    {
    case Stance::Aggressive: // 일반 70%, 강공 20%, 가드 10%
        OutNormal = 70; OutStrong = 20; OutGuard = 10;
        break;
    case Stance::Defensive:  // 일반 30%, 강공 20%, 가드 50%
        OutNormal = 30; OutStrong = 20; OutGuard = 50;
        break;
    case Stance::Balanced:   // 일반 50%, 강공 20%, 가드 30%
    default:
        OutNormal = 50; OutStrong = 20; OutGuard = 30;
        break;
    }
}

// 퍼센트 분포로 (행동/공격종류) 결정
ActionType Enemy::PickAction(int InNormalPct, int InStrongPct, int InGuardPct, AttackKind& OutKind) const
{
    int ActionRoll = RandRange();       // 0~99

    if (ActionRoll < InNormalPct)      
    {
        OutKind = AttackKind::Normal;   // 일반 공격
        return ActionType::Attack;
    }
    if (ActionRoll < InNormalPct + InStrongPct)
    {
        OutKind = AttackKind::Strong;   // 강한 공격
        return ActionType::Attack;
    }

    return ActionType::Guard;           // 가드 
}

// 공격,가드 (High/Mid/Low) 무작위
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

//ST 부족 시 자동 강등(강공->일반->가드), 가드는 비용 없음
void Enemy::CheckStamina(ActionType& OutAct, AttackKind& OutKind, int InST) const
{
    if (OutAct == ActionType::Attack)
    {
        if (OutKind == AttackKind::Strong)
        {
            if (InST < 25)                         // 강공 ST 부족할시
            {
                if (InST >= 5)
                {
                    OutKind = AttackKind::Normal;  // 강공 -> 일반
                }
                else
                {
                    OutAct = ActionType::Guard;    // ST 없으면 가드
                }
            }
        }
        else // 일반 공격
        {
            if (InST < 5)
            {
                OutAct = ActionType::Guard;        // ST 없으면 가드
            }
        }

        return;
    }

    if (OutAct == ActionType::Dodge)
    {
        if (InST < 30) // 회피 ST 부족할시 가드
        {
            OutAct = ActionType::Guard;
        }
        return;
    }
}

// 적 (성향/전조/분포) 행동 선택 
void Enemy::TakeTurn()
{
    // 기본 선택 초기화 ClearChoice()
    // 여기서 (성향/전조/분포) 기반으로 choice 설정
    int NormalPct = 0;
    int StrongPct = 0;
    int GuardPct = 0;

    // 전조 발생
    IsTelegraphTurn = false;           //이전 턴 값 초기화
    bool Telegraph = TelegraphRoll();
    IsTelegraphTurn = Telegraph;       // 전조 발생 여부 저장

    if (Telegraph) // 전조 분포(명시값) 적용 , 전조 대사
    {
        TelegraphDistribution(stance, NormalPct, StrongPct, GuardPct);
    }
    else           // 기본 분포
    {
        BaseDistribution(stance, NormalPct, StrongPct, GuardPct); 
    }

    AttackKind SelectedKind{};                              // 공격 종류 담을 변수 (PickAction이 값을 채움)
    ActionType SelectedAction = PickAction(NormalPct, StrongPct, GuardPct, SelectedKind); // (공격/가드) 결정
    CombatDirection SelectedDirection = PickDirection();   // 방향 선택 (공격/가드)
    CheckStamina(SelectedAction, SelectedKind, ST);        // 스태미너 검사 및 자동 강등

    // 최종 선택 적용
    choice.Action = SelectedAction;              // 공격/가드/회피
    choice.Direction = SelectedDirection;        // 상/중/하

    if (SelectedAction == ActionType::Attack)    // 공격일 때만 (일반/강공) 적용 
    {
        choice.Kind = SelectedKind;         
    }
}
