// Enemy.cpp
#include "Enemy.h"
#include <cstdio>
#include <cstdlib> // rand
#include <ctime>   // time (어딘가에서 srand(...) 1번만 호출)

static int RandomRange(int min, int max)
{
    return min + (std::rand() % (max - min + 1));
}

void Enemy::TakeTurn()
{
    // 30% 전조(텔레그래프) 발동
    const bool telegraph = (RandomRange(1, 100) <= 30);

    if (telegraph)
    {
        switch (stance)
        {
        case Stance::Aggressive:
            std::printf("[전조] 칼집에서 검을 뽑는 소리가 들립니다…\n");
            break;
        case Stance::Defensive:
            std::printf("[전조] 방패를 단단히 고쳐잡는 기척이 납니다…\n");
            break;
        case Stance::Balanced:
            std::printf("[전조] 상대가 호흡을 고르며 움직임을 살핍니다…\n");
            break;
        }
    }

    // 성향별 기본 확률로 행동 선택 (1~100)
    int r = RandomRange(1, 100);
    ActionType act = ActionType::Guard;
    AttackKind kind = AttackKind::Normal;

    switch (stance)
    {
    case Stance::Aggressive: // 일반 60, 가드 20, 강공 20 (+전조면 공격 +30)
        if (telegraph && r <= 30) { act = ActionType::Attack; kind = AttackKind::Normal; }
        else if (r <= 60) { act = ActionType::Attack; kind = AttackKind::Normal; }
        else if (r <= 80) { act = ActionType::Guard; }
        else { act = ActionType::Attack; kind = AttackKind::Strong; }
        break;

    case Stance::Defensive: // 가드 60, 일반 20, 강공 20 (+전조면 가드 +30)
        if (telegraph && r <= 30) { act = ActionType::Guard; }
        else if (r <= 60) { act = ActionType::Guard; }
        else if (r <= 80) { act = ActionType::Attack; kind = AttackKind::Normal; }
        else { act = ActionType::Attack; kind = AttackKind::Strong; }
        break;

    case Stance::Balanced: // 일반 40, 가드 40, 강공 20 (+전조면 일반/가드 각각 +15)
    default:
        if (telegraph && r <= 15) { act = ActionType::Attack; kind = AttackKind::Normal; }
        else if (telegraph && r <= 30) { act = ActionType::Guard; }
        else if (r <= 40) { act = ActionType::Attack; kind = AttackKind::Normal; }
        else if (r <= 80) { act = ActionType::Guard; }
        else { act = ActionType::Attack; kind = AttackKind::Strong; }
        break;
    }

    // ST 비용 보정 (문제 스펙)
    const int COST_ATTACK_N = 5;
    const int COST_ATTACK_S = 25;
    const int COST_DODGE = 30;

    if (act == ActionType::Attack)
    {
        if (kind == AttackKind::Strong && ST < COST_ATTACK_S)
        {
            if (ST >= COST_ATTACK_N) kind = AttackKind::Normal; // 강공 → 일반로 다운그레이드
            else                     act = ActionType::Guard;  // 둘 다 불가 → 가드
        }
        else if (kind == AttackKind::Normal && ST < COST_ATTACK_N)
        {
            act = ActionType::Guard;
        }
    }
    else if (act == ActionType::Dodge)
    {
        if (ST < COST_DODGE) act = ActionType::Guard;
    }

    // 최종 선택 세팅
    choice.Action = act;

    if (act == ActionType::Attack || act == ActionType::Guard)
        choice.Direction = static_cast<CombatDirection>(RandomRange(0, 2)); // High/Mid/Low
    else
        choice.Direction = CombatDirection::Mid; // 회피는 의미 없음 → 기본값

    choice.Kind = (act == ActionType::Attack ? kind : AttackKind::Normal);
    choice.Target = nullptr; // 대상 포인터는 배틀 매니저에서 해석
}
