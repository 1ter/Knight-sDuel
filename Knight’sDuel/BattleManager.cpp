#include "BattleManager.h"
#include <cstdio>
#include <algorithm>
#include <cstdlib>

// 내부 유틸
static void PrintBar(const char* Label, int Curr, int Max, int Width)
{
    int Fill = (Max > 0) ? (Curr * Width) / Max : 0;
    if (Fill < 0) Fill = 0;
    if (Fill > Width) Fill = Width;

    std::printf("%-3s %4d / %-4d[", Label, Curr, Max);
    for (int i = 0; i < Fill; ++i) std::printf("█");
    for (int i = Fill; i < Width; ++i) std::printf("▒");
    std::printf("]\n");
}

static const char* ToStance(Stance s)
{
    switch (s)
    {
    case Stance::Aggressive: 
        return "공격형";
    case Stance::Defensive:  
        return "방어형";
    default:                 
        return "균형형";
    }
}

// 진행 상태
bool BattleManager::IsBattleEnd() const
{
    if (!EnemyActor)
    {
        return true;
    }
    return PlayerActor.IsDead() || EnemyActor->IsDead();
}

// 시작 턴 처리 순서
void BattleManager::StartTurn()
{
    // (1) 디버프 틱
    PlayerActor.TickDebuff();
    if (EnemyActor)
    {
        EnemyActor->TickDebuff();
    }
    // (2) ST +5
    PlayerActor.RegenST();
    if (EnemyActor)
    {
        EnemyActor->RegenST();
    }
    // (3) 선택 초기화
    PlayerActor.ClearChoice();
    if (EnemyActor)
    {
        EnemyActor->ClearChoice();
    }
    // 로그 초기화
    LastLog = {};
}

//플레이어: 공격 계획
void BattleManager::PlanPlayerAttack(CombatDirection InDir, AttackKind InKind)
{
    PlayerActor.ChoiceAttack(InDir, InKind, EnemyActor);
}
// 플레이어: 가드 계획
void BattleManager::PlanPlayerGuard(CombatDirection InDir)
{
    PlayerActor.ChoiceGuard(InDir);
}
// 플레이어: 회피 계획
void BattleManager::PlanPlayerDodge()
{
    PlayerActor.ChoiceDodge();
}

// 적 턴(선택)
void BattleManager::ExecuteTurn()
{
    if (!EnemyActor)
    {
        return;
    }
    // 적 선택
    EnemyActor->TakeTurn();
    // ST 검증/강등 (플레이어)
    {
        ActionType a = PlayerActor.GetChoiceAction();
        AttackKind k = PlayerActor.GetChoiceKind();
        StaminaValid(PlayerActor, a, k);
        if (a != PlayerActor.GetChoiceAction() || k != PlayerActor.GetChoiceKind())
        {
            auto dir = PlayerActor.GetChoiceDirection();
            if (a == ActionType::Attack)
            {
                PlayerActor.ChoiceAttack(dir, k, EnemyActor);
            }
            else if (a == ActionType::Guard)
            {
                PlayerActor.ChoiceGuard(dir);
            }
            else
            {
                PlayerActor.ChoiceDodge();
            }
        }
    }
    // ST 검증/강등 (적)
    {
        ActionType a = EnemyActor->GetChoiceAction();
        AttackKind k = EnemyActor->GetChoiceKind();
        StaminaValid(*EnemyActor, a, k);
        if (a != EnemyActor->GetChoiceAction() || k != EnemyActor->GetChoiceKind())
        {
            auto dir = EnemyActor->GetChoiceDirection();
            if (a == ActionType::Attack)      EnemyActor->ChoiceAttack(dir, k, &PlayerActor);
            else if (a == ActionType::Guard)  EnemyActor->ChoiceGuard(dir);
            else                               EnemyActor->ChoiceDodge();
        }
    }
    // 판정
    Resolve();
}

void BattleManager::PrintStatus() const
{
    std::printf("─────────────────[Knight’s Duel]─────────────────\n");
    std::printf("적 : %s [성향:%s]\n",
        EnemyActor ? EnemyActor->GetName().c_str() : "(없음)",
        EnemyActor ? ToStance(EnemyActor->GetStance()) : "-");

    if (EnemyActor)
    {
        PrintBar("HP", EnemyActor->GetHP(), EnemyActor->GetMaxHP(), BarWidth);
        PrintBar("ST", EnemyActor->GetST(), EnemyActor->GetMaxST(), BarWidth);
    }
    else
    {
        std::printf("적 없음\n");
    }

    std::printf("────────────────────────────────────────────────\n");
    std::printf("                    [ VS ]\n");
    std::printf("────────────────────────────────────────────────\n");

    if (!LastLog.Telegraph.empty())   std::printf("[전조] %s\n", LastLog.Telegraph.c_str());
    if (!LastLog.EnemyAction.empty()) std::printf("[적행동] %s\n", LastLog.EnemyAction.c_str());
    if (!LastLog.Judge.empty())       std::printf("[판정] %s\n", LastLog.Judge.c_str());
    if (!LastLog.Effects.empty())     std::printf("[효과] %s\n", LastLog.Effects.c_str());
    if (!LastLog.Damage.empty())      std::printf("[피해] %s\n", LastLog.Damage.c_str());
    if (!LastLog.Extra.empty())       std::printf("[추가효과] %s\n", LastLog.Extra.c_str());
}

// 내부 로직
void BattleManager::Resolve()
{
    // 현재 선택 가져오기
    ActionType      PA = PlayerActor.GetChoiceAction();
    AttackKind      PK = PlayerActor.GetChoiceKind();
    CombatDirection PD = PlayerActor.GetChoiceDirection();

    ActionType      EA = EnemyActor->GetChoiceAction();
    AttackKind      EK = EnemyActor->GetChoiceKind();
    CombatDirection ED = EnemyActor->GetChoiceDirection();

    // [적행동] 로그
    {
        const char* actStr =
            (EA == ActionType::Attack) ? ((EK == AttackKind::Strong) ? "강공" : "일반")
            : (EA == ActionType::Guard) ? "가드"
            : "회피";
        const char* dirStr =
            (ED == CombatDirection::High) ? "상단"
            : (ED == CombatDirection::Mid) ? "중단"
            : "하단";
        LastLog.EnemyAction = std::string(actStr) + "(" + dirStr + ")";
    }

    // ───────────── 조합 처리 ─────────────

    // 공격 ↔ 공격 : 무기막기
    if (PA == ActionType::Attack && EA == ActionType::Attack)
    {
        // ST 소비: 각자 자신의 공격 코스트 + 무기막기 추가 -10
        if (PK == AttackKind::Strong) { PlayerActor.ConsumeST(StrongCostST); }
        else { PlayerActor.ConsumeST(NormalCostST); }
        PlayerActor.ConsumeST(ClashExtraCostST);

        if (EK == AttackKind::Strong) { EnemyActor->ConsumeST(StrongCostST); }
        else { EnemyActor->ConsumeST(NormalCostST); }
        EnemyActor->ConsumeST(ClashExtraCostST);

        LastLog.Judge = "공격 vs 공격(무기막기) → 피해 0";
        LastLog.Damage.clear();
        LastLog.Effects.clear();
        return;
    }

    // 플레이어 공격 ↔ 적 가드
    if (PA == ActionType::Attack && EA == ActionType::Guard)
    {
        // 플레이어 공격 코스트
        if (PK == AttackKind::Strong) { PlayerActor.ConsumeST(StrongCostST); }
        else { PlayerActor.ConsumeST(NormalCostST); }

        if (DirectionsMatch(PD, ED))
        {
            // 패링 성공 → 즉시반격
            LastLog.Judge = "공격 vs 가드(일치) → 패링/즉시반격";
            ParryCounter(*EnemyActor, PlayerActor, ED);
        }
        else
        {
            // 불일치 → 100% 피해
            int dmg = DamageCompute(PlayerActor, *EnemyActor, PK);
            if (dmg > 0)
            {
                EnemyActor->TakeDamage(dmg);
            }
            LastLog.Judge = "공격 vs 가드(불일치) → 100% 피해";
            LastLog.Damage = "플레이어→적: " + std::to_string(dmg);

            // 디버프 롤
            Debuff db = DebuffRoll(PD, PK);
            if (db.Type != DebuffType::None)
            {
                EnemyActor->RefreshDebuff(db);
                LastLog.Effects = "적에게 디버프 적용";
            }
            else
            {
                LastLog.Effects.clear();
            }
        }
        return;
    }

    // 플레이어 가드 ↔ 적 공격
    if (PA == ActionType::Guard && EA == ActionType::Attack)
    {
        // 적 공격 코스트
        if (EK == AttackKind::Strong) 
        { 
            EnemyActor->ConsumeST(StrongCostST); 
        }
        else 
        { 
            EnemyActor->ConsumeST(NormalCostST); 
        }

        if (DirectionsMatch(PD, ED))
        {
            LastLog.Judge = "가드(일치) vs 공격 → 패링/즉시반격";
            ParryCounter(PlayerActor, *EnemyActor, PD);
        }
        else
        {
            int dmg = DamageCompute(*EnemyActor, PlayerActor, EK);
            if (dmg > 0)
            {
                PlayerActor.TakeDamage(dmg);
            }
            LastLog.Judge = "가드(불일치) vs 공격 → 100% 피해";
            LastLog.Damage = "적→플레이어: " + std::to_string(dmg);

            Debuff db = DebuffRoll(ED, EK);
            if (db.Type != DebuffType::None)
            {
                PlayerActor.RefreshDebuff(db);
                LastLog.Effects = "플레이어에게 디버프 적용";
            }
            else
            {
                LastLog.Effects.clear();
            }
        }
        return;
    }

    // 공격 ↔ 회피
    if (PA == ActionType::Attack && EA == ActionType::Dodge)
    {
        if (PK == AttackKind::Strong) { PlayerActor.ConsumeST(StrongCostST); }
        else { PlayerActor.ConsumeST(NormalCostST); }
        EnemyActor->ConsumeST(DodgeCostST);

        LastLog.Judge = "공격 vs 회피 → 0 피해";
        LastLog.Damage.clear();
        LastLog.Effects.clear();
        return;
    }

    // 회피 ↔ 공격
    if (PA == ActionType::Dodge && EA == ActionType::Attack)
    {
        PlayerActor.ConsumeST(DodgeCostST);
        if (EK == AttackKind::Strong) { EnemyActor->ConsumeST(StrongCostST); }
        else { EnemyActor->ConsumeST(NormalCostST); }

        LastLog.Judge = "회피 vs 공격 → 0 피해";
        LastLog.Damage.clear();
        LastLog.Effects.clear();
        return;
    }

    // 가드 ↔ 가드
    if (PA == ActionType::Guard && EA == ActionType::Guard)
    {
        LastLog.Judge = "가드 vs 가드 → 변화 없음";
        LastLog.Damage.clear();
        LastLog.Effects.clear();
        return;
    }

    // 회피 ↔ 회피
    if (PA == ActionType::Dodge && EA == ActionType::Dodge)
    {
        PlayerActor.ConsumeST(DodgeCostST);
        EnemyActor->ConsumeST(DodgeCostST);

        LastLog.Judge = "회피 vs 회피 → 변화 없음(ST -30씩)";
        LastLog.Damage.clear();
        LastLog.Effects.clear();
        return;
    }

    // 가드 ↔ 회피
    if (PA == ActionType::Guard && EA == ActionType::Dodge)
    {
        EnemyActor->ConsumeST(DodgeCostST);
        LastLog.Judge = "가드 vs 회피 → 변화 없음(적 ST -30)";
        LastLog.Damage.clear();
        LastLog.Effects.clear();
        return;
    }

    // 회피 ↔ 가드
    if (PA == ActionType::Dodge && EA == ActionType::Guard)
    {
        PlayerActor.ConsumeST(DodgeCostST);
        LastLog.Judge = "회피 vs 가드 → 변화 없음(플레이어 ST -30)";
        LastLog.Damage.clear();
        LastLog.Effects.clear();
        return;
    }
}


int BattleManager::DamageCompute(const Actor& InAttacker, const Actor& InDefender, AttackKind InKind) const
{
    int atk = InAttacker.GetATK();
    int def = InDefender.GetDEF();
    int raw = (InKind == AttackKind::Strong) ? (atk * 2 - def) : (atk - def);
    return std::max(0, raw);
}

Debuff BattleManager::DebuffRoll(CombatDirection InDir, AttackKind InKind) const
{
    Debuff out{};
    bool trigger = (InKind == AttackKind::Strong);

    if (!trigger)
    {
        int roll = std::rand() % 100;  // 일반 30%
        trigger = (roll < DebuffNormalPct);
    }
    if (!trigger) return out;

    switch (InDir)
    {
    case CombatDirection::High: // 경직
        out.Type = DebuffType::Stagger;
        out.Value = (InKind == AttackKind::Strong) ? 5 : 3;
        out.Duration = 2;
        break;
    case CombatDirection::Mid:  // 출혈
        out.Type = DebuffType::Bleed;
        out.Value = (InKind == AttackKind::Strong) ? 8 : 5;
        out.Duration = (InKind == AttackKind::Strong) ? 3 : 2;
        break;
    case CombatDirection::Low:  // 약화
        out.Type = DebuffType::Weakness;
        out.Value = (InKind == AttackKind::Strong) ? 10 : 5;
        out.Duration = 2;
        break;
    }
    return out;
}

bool BattleManager::DirectionsMatch(CombatDirection InL, CombatDirection InR) const
{
    return InL == InR;
}

void BattleManager::StaminaValid(Actor& InActor, ActionType& OutAct, AttackKind& OutKind) const
{
    if (OutAct == ActionType::Attack)
    {
        if (OutKind == AttackKind::Strong)
        {
            if (!InActor.CheckST(StrongCostST))
            {
                if (InActor.CheckST(NormalCostST)) OutKind = AttackKind::Normal;
                else                               OutAct = ActionType::Guard;
            }
        }
        else
        {
            if (!InActor.CheckST(NormalCostST))
            {
                OutAct = ActionType::Guard;
            }
        }
        return;
    }
    if (OutAct == ActionType::Dodge)
    {
        if (!InActor.CheckST(DodgeCostST))
        {
            OutAct = ActionType::Guard;
        }
        return;
    }
    // Guard: 비용 없음
}

void BattleManager::ParryCounter(Actor& InGuarder, Actor& InAttacker, CombatDirection InGuardDir)
{
    // 즉시반격: 일반공격 1회(ST 0)
    int dmg = DamageCompute(InGuarder, InAttacker, AttackKind::Normal);
    if (dmg > 0) InAttacker.TakeDamage(dmg);

    Debuff db = DebuffRoll(InGuardDir, AttackKind::Normal);
    if (db.Type != DebuffType::None)
    {
        InAttacker.RefreshDebuff(db);
        LastLog.Effects = "즉시반격 디버프 적용";
    }

    LastLog.Damage = "가드 측 즉시반격 → " + std::to_string(dmg);
}
