#include "BattleManager.h"
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

// 0~99 난수
static inline int RandRange()
{
    return rand() % 100;
}

// 내부 유틸: 상태바 출력
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

// 성향 enum -> 한글 문자열
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

// 진행 상태 (전투 종료 판단)
bool BattleManager::IsBattleEnd() const
{
    if (!EnemyActor)   // 적 미지정이면 종료
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
    Log = {};
}

// 플레이어 행동 계획(입력 반영)
void BattleManager::PlanPlayerAttack(CombatDirection InDirection, AttackKind InKind)
{
    PlayerActor.ChoiceAttack(InDirection, InKind, EnemyActor);
}
void BattleManager::PlanPlayerGuard(CombatDirection InDirection)
{
    PlayerActor.ChoiceGuard(InDirection);
}
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
    // 1) 적 선택
    EnemyActor->TakeTurn();
    // 2) 전조 로그 세팅 (이번 턴 값)
    if (EnemyActor->GetTelegraphTurn())
    {
        Log.Telegraph = PickTelegraphLine(EnemyActor->GetStance());
    }
    // 3) ST 검증/강등 (플레이어)
    {
        ActionType StaminaActionPlayer = PlayerActor.GetChoiceAction();
        AttackKind StaminaKindPlayer = PlayerActor.GetChoiceKind();
        StaminaValid(PlayerActor, StaminaActionPlayer, StaminaKindPlayer);

        // 강등/변경이 발생했다면 선택을 실제로 덮어쓰기
        if (StaminaActionPlayer != PlayerActor.GetChoiceAction() || StaminaKindPlayer != PlayerActor.GetChoiceKind())
        {
            auto Direction = PlayerActor.GetChoiceDirection();
            if (StaminaActionPlayer == ActionType::Attack)
            {
                PlayerActor.ChoiceAttack(Direction, StaminaKindPlayer, EnemyActor);
            }
            else if (StaminaActionPlayer == ActionType::Guard)
            {
                PlayerActor.ChoiceGuard(Direction);
            }
            else
            {
                PlayerActor.ChoiceDodge();
            }
        }
    }

    // 4) ST 검증/강등 (적)
    {
        ActionType StaminaActionEnemy = EnemyActor->GetChoiceAction();
        AttackKind StaminaKindEnemy = EnemyActor->GetChoiceKind();
        StaminaValid(*EnemyActor, StaminaActionEnemy, StaminaKindEnemy);
        if (StaminaActionEnemy != EnemyActor->GetChoiceAction() || StaminaKindEnemy != EnemyActor->GetChoiceKind())
        {
            auto Direction = EnemyActor->GetChoiceDirection();
            if (StaminaActionEnemy == ActionType::Attack)
            {
                EnemyActor->ChoiceAttack(Direction, StaminaKindEnemy, &PlayerActor);
            }
            else if (StaminaActionEnemy == ActionType::Guard)
            {
                EnemyActor->ChoiceGuard(Direction);
            }
            else
            {
                EnemyActor->ChoiceDodge();
            }
        }
    }
    // 5) 동시 판정
    Resolve();
}

// 직전 턴 로그 출력
void BattleManager::PrintStatus() const
{
    printf("─────────────────[Knight’s Duel]────────────────────────\n");
    printf("적 : %s [성향: %s]\n",
        EnemyActor ? EnemyActor->GetName().c_str() : "(없음)",
        EnemyActor ? ToStance(EnemyActor->GetStance()) : "-");

    if (EnemyActor)
    {
        PrintBar("HP", EnemyActor->GetHP(), EnemyActor->GetMaxHP(), BarWidth);
        PrintBar("ST", EnemyActor->GetST(), EnemyActor->GetMaxST(), BarWidth);
    }
    else
    {
        printf("적 없음\n");
    }

    printf("────────────────────────────────────────────────\n");
    printf("플레이어 : %s\n", PlayerActor.GetName().c_str());
    PrintBar("HP", PlayerActor.GetHP(), PlayerActor.GetMaxHP(), BarWidth);
    PrintBar("ST", PlayerActor.GetST(), PlayerActor.GetMaxST(), BarWidth);

    printf("────────────────────────────────────────────────────────\n");
    printf("                    [ VS ]\n");
    printf("────────────────────────────────────────────────────────\n");

    if (!Log.Telegraph.empty())
    {
        std::printf("[전조] %s\n", Log.Telegraph.c_str());
    }
    if (!Log.EnemyAction.empty())
    {
        std::printf("[적행동] %s\n", Log.EnemyAction.c_str());
    }

    if (!Log.Judge.empty())
    {
        std::printf("[판정] %s\n", Log.Judge.c_str());
    }
    if (!Log.Effects.empty())
    {
        std::printf("[효과] %s\n", Log.Effects.c_str());
    }

    if (!Log.Damage.empty())
    {
        std::printf("[피해] %s\n", Log.Damage.c_str());
    }
    if (!Log.Extra.empty())
    {
        std::printf("[추가효과] %s\n", Log.Extra.c_str());
    }
    printf("─────────────────────────────────────────────────────────────\n");
}

// 동시 턴제 판정
void BattleManager::Resolve()
{
    // 현재 선택 가져오기
    ActionType      PlayerChoiceAction = PlayerActor.GetChoiceAction();
    AttackKind      PlayerChoiceKind = PlayerActor.GetChoiceKind();
    CombatDirection PlayerChoiceDirection = PlayerActor.GetChoiceDirection();

    ActionType      EnemyChoiceAction = EnemyActor->GetChoiceAction();
    AttackKind      EnemyChoiceKind = EnemyActor->GetChoiceKind();
    CombatDirection EnemyChoiceDirection = EnemyActor->GetChoiceDirection();

    // [적행동] 문자열 로그 기록
    {
        const char* ActionStr = "";

        if (EnemyChoiceAction == ActionType::Attack)
        {
            if (EnemyChoiceKind == AttackKind::Strong)
            {
                ActionStr = "강공";
            }
            else
            {
                ActionStr = "일반";
            }
        }
        else if (EnemyChoiceAction == ActionType::Guard)
        {
            ActionStr = "가드";
        }
        else if (EnemyChoiceAction == ActionType::Dodge)
        {
            ActionStr = "회피";
        }
        else
        {
            ActionStr = "-";
        }

        const char* DirectionStr = "";

        if (EnemyChoiceDirection == CombatDirection::High)
        { 
            DirectionStr = "상단";
        }
        else if (EnemyChoiceDirection == CombatDirection::Mid)
        {
            DirectionStr = "중단";
        }
        else if (EnemyChoiceDirection == CombatDirection::Low)
        {
            DirectionStr = "하단";
        }
        else
        {
            DirectionStr = "-";
        }

        Log.EnemyAction = std::string(ActionStr) + "(" + DirectionStr + ")";
    }

    // 판정 처리
    // 공격 ↔ 공격 : 무기막기 추가로 ST -10 소모
    if (PlayerChoiceAction == ActionType::Attack && EnemyChoiceAction == ActionType::Attack)
    {
        // 플레이어 소모
        if (PlayerChoiceKind == AttackKind::Strong) 
        {
            PlayerActor.ConsumeST(StrongCostST); 
        }
        else 
        {
            PlayerActor.ConsumeST(NormalCostST); 
        }
        PlayerActor.ConsumeST(ClashExtraCostST);

        // 적 소모
        if (EnemyChoiceKind == AttackKind::Strong) 
        { 
            EnemyActor->ConsumeST(StrongCostST); 
        }
        else 
        { 
            EnemyActor->ConsumeST(NormalCostST); 
        }
        EnemyActor->ConsumeST(ClashExtraCostST);

        Log.Judge = "공격 vs 공격(무기막기) → 피해: 0, 스태미나: -10";
        Log.Damage.clear();
        Log.Effects.clear();
        return;
    }

    // 플레이어 공격 ↔ 적 가드
    if (PlayerChoiceAction == ActionType::Attack && EnemyChoiceAction == ActionType::Guard)
    {
        // 플레이어 공격 코스트
        if (PlayerChoiceKind == AttackKind::Strong) 
        {
            PlayerActor.ConsumeST(StrongCostST); 
        }
        else 
        { 
            PlayerActor.ConsumeST(NormalCostST); 
        }

        if (DirectionsMatch(PlayerChoiceDirection, EnemyChoiceDirection))
        {
            // 패링 성공 → 즉시반격
            Log.Judge = "공격 vs 가드(일치) → 패링/즉시반격";
            ParryCounter(*EnemyActor, PlayerActor, EnemyChoiceDirection);
        }
        else
        {
            // 불일치 → 100% 피해
            int Damage = DamageCompute(PlayerActor, *EnemyActor, PlayerChoiceKind);

            if (Damage > 0)
            {
                EnemyActor->TakeDamage(Damage);
            }
            Log.Judge  = "공격 vs 가드(불일치) → 100% 피해";
            Log.Damage = "플레이어→적: " + std::to_string(Damage);

            // 디버프 롤
            Debuff RolledDebuff = DebuffRoll(PlayerChoiceDirection, PlayerChoiceKind);
            if (RolledDebuff.Type != DebuffType::None)
            {
                EnemyActor->RefreshDebuff(RolledDebuff);
                Log.Effects = "적에게 디버프 적용";
                Log.Extra = RolledDebuff.ToString();
            }
            else
            {
                Log.Effects.clear();
                Log.Extra.clear();
            }
        }
        return;
    }

    // 플레이어 가드 ↔ 적 공격
    if (PlayerChoiceAction == ActionType::Guard && EnemyChoiceAction == ActionType::Attack)
    {
        // 적 공격 코스트
        if (EnemyChoiceKind == AttackKind::Strong) 
        { 
            EnemyActor->ConsumeST(StrongCostST); 
        }
        else 
        { 
            EnemyActor->ConsumeST(NormalCostST); 
        }

        if (DirectionsMatch(PlayerChoiceDirection, EnemyChoiceDirection))
        {
            Log.Judge = "가드(일치) vs 공격 → 패링/즉시반격";
            ParryCounter(PlayerActor, *EnemyActor, PlayerChoiceDirection);
        }
        else
        {
            int Damage = DamageCompute(*EnemyActor, PlayerActor, EnemyChoiceKind);

            if (Damage > 0)
            {
                PlayerActor.TakeDamage(Damage);
            }
            Log.Judge = "가드(불일치) vs 공격 → 100% 피해";
            Log.Damage = "적→플레이어: " + std::to_string(Damage);

            Debuff RolledDebuff = DebuffRoll(EnemyChoiceDirection, EnemyChoiceKind);
            if (RolledDebuff.Type != DebuffType::None)
            {
                PlayerActor.RefreshDebuff(RolledDebuff);
                Log.Effects = "플레이어에게 디버프 적용";
                Log.Extra = RolledDebuff.ToString();
            }
            else
            {
                Log.Effects.clear();
                Log.Extra.clear();
            }
        }
        return;
    }

    // 공격 ↔ 회피
    if (PlayerChoiceAction == ActionType::Attack && EnemyChoiceAction == ActionType::Dodge)
    {
        if (PlayerChoiceKind == AttackKind::Strong)
        { 
            PlayerActor.ConsumeST(StrongCostST); 
        }
        else 
        {
            PlayerActor.ConsumeST(NormalCostST); 
        }
        EnemyActor->ConsumeST(DodgeCostST);

        Log.Judge = "공격 vs 회피 → 0 피해";
        Log.Damage.clear();
        Log.Effects.clear();

        return;
    }

    // 회피 ↔ 공격
    if (PlayerChoiceAction == ActionType::Dodge && EnemyChoiceAction == ActionType::Attack)
    {
        PlayerActor.ConsumeST(DodgeCostST);

        if (EnemyChoiceKind == AttackKind::Strong) 
        {
            EnemyActor->ConsumeST(StrongCostST); 
        }
        else
        {
            EnemyActor->ConsumeST(NormalCostST); 
        }

        Log.Judge = "회피 vs 공격 → 0 피해";
        Log.Damage.clear();
        Log.Effects.clear();

        return;
    }

    // 가드 ↔ 가드
    if (PlayerChoiceAction == ActionType::Guard && EnemyChoiceAction == ActionType::Guard)
    {
        Log.Judge = "가드 vs 가드 → 변화 없음";
        Log.Damage.clear();
        Log.Effects.clear();

        return;
    }

    // 회피 ↔ 회피
    if (PlayerChoiceAction == ActionType::Dodge && EnemyChoiceAction == ActionType::Dodge)
    {
        PlayerActor.ConsumeST(DodgeCostST);
        EnemyActor->ConsumeST(DodgeCostST);

        Log.Judge = "회피 vs 회피 → 변화 없음(ST -30씩)";
        Log.Damage.clear();
        Log.Effects.clear();

        return;
    }

    // 가드 ↔ 회피
    if (PlayerChoiceAction == ActionType::Guard && EnemyChoiceAction == ActionType::Dodge)
    {
        EnemyActor->ConsumeST(DodgeCostST);
        Log.Judge = "가드 vs 회피 → 변화 없음(적 ST -30)";
        Log.Damage.clear();
        Log.Effects.clear();

        return;
    }

    // 회피 ↔ 가드
    if (PlayerChoiceAction == ActionType::Dodge && EnemyChoiceAction == ActionType::Guard)
    {
        PlayerActor.ConsumeST(DodgeCostST);
        Log.Judge = "회피 vs 가드 → 변화 없음(플레이어 ST -30)";
        Log.Damage.clear();
        Log.Effects.clear();

        return;
    }
}

// 데미지 계산
int BattleManager::DamageCompute(const Actor& InAttacker, const Actor& InDefender, AttackKind InKind) const
{
    int Atk = InAttacker.GetATK();
    int Def = InDefender.GetDEF();
    int Raw = 0;

    if (InKind == AttackKind::Strong)
    {
        Raw = (Atk * 2 - Def);
    }
    else
    {
        Raw = (Atk - Def);
    }

    return std::max(0, Raw);
}

// 디버프 롤 (일반공격 디버프 30%, 강한일격 디버프 100%)
Debuff BattleManager::DebuffRoll(CombatDirection InDirection, AttackKind InKind) const
{
    Debuff DebuffResult{};   // 기본값(없음)
    bool trigger = (InKind == AttackKind::Strong);  // 강공은 무조건 발생(100%)

    // 일반공격이면 30% 확률 체크
    if (!trigger)
    {
        int Roll = RandRange();
        trigger = (Roll < DebuffNormalPct);  // DebuffNormalPct = 30
    }
    if (!trigger)
    {
        return DebuffResult;
    }

    // 방향(상단/중단/하단)별 디버프 
    switch (InDirection)
    {
    case CombatDirection::High: // 경직
        DebuffResult.Type = DebuffType::Stagger;
        DebuffResult.Value = (InKind == AttackKind::Strong) ? 5 : 3;
        DebuffResult.Duration = 2;
        break;
    case CombatDirection::Mid:  // 출혈
        DebuffResult.Type = DebuffType::Bleed;
        DebuffResult.Value = (InKind == AttackKind::Strong) ? 8 : 5;
        DebuffResult.Duration = (InKind == AttackKind::Strong) ? 3 : 2;
        break;
    case CombatDirection::Low:  // 약화
        DebuffResult.Type = DebuffType::Weakness;
        DebuffResult.Value = (InKind == AttackKind::Strong) ? 10 : 5;
        DebuffResult.Duration = 2;
        break;
    }

    return DebuffResult;
}

// 방향 일치 여부 (패링 조건)
bool BattleManager::DirectionsMatch(CombatDirection PlayerDirection, CombatDirection EnemyDirection) const
{
    return PlayerDirection == EnemyDirection;
}

// ST 검증/강등 (강공 -> 일반 -> 가드, 회피 -> 가드)
void BattleManager::StaminaValid(Actor& InActor, ActionType& OutAction, AttackKind& OutKind) const
{
    if (OutAction == ActionType::Attack)
    {
        if (OutKind == AttackKind::Strong)
        {
            if (!InActor.CheckST(StrongCostST))
            {
                if (InActor.CheckST(NormalCostST))
                {
                    OutKind = AttackKind::Normal;
                }
                else
                {
                    OutAction = ActionType::Guard;
                }
            }
        }
        else
        {
            if (!InActor.CheckST(NormalCostST))
            {
                OutAction = ActionType::Guard;
            }
        }
        return;
    }
    if (OutAction == ActionType::Dodge)
    {
        if (!InActor.CheckST(DodgeCostST))
        {
            OutAction = ActionType::Guard;
        }
        return;
    }
    // 가드 ST 비용 없음
}

// 패링 즉시반격 (가드 성공 시 즉시 '일반공격' 1회 적용, 반격은 ST 소모 없음, 반격도 디버프 롤 적용)
void BattleManager::ParryCounter(Actor& InGuarder, Actor& InAttacker, CombatDirection InGuardDirection)
{
    // 즉시반격 (일반공격 ST 소모 없음)
    int Damage = DamageCompute(InGuarder, InAttacker, AttackKind::Normal);

    if (Damage > 0)
    {
        InAttacker.TakeDamage(Damage);
    }

    Debuff RolledDebuff = DebuffRoll(InGuardDirection, AttackKind::Normal);
    if (RolledDebuff.Type != DebuffType::None)
    {
        InAttacker.RefreshDebuff(RolledDebuff);
        Log.Effects = "즉시반격 디버프 적용";
        Log.Extra = RolledDebuff.ToString();
    }

    Log.Damage = "가드 측 즉시반격 → " + std::to_string(Damage);
}

// 성향별 전조 대사 한 줄 골라서 반환
const std::string& BattleManager::PickTelegraphLine(Stance InStance) const
{
    // 전조(텔레그래프) 대사 모음 
    static const std::vector<std::string> AttackLines =
    {
        "손이 칼자루를 향해 서서히 움직인다",
        "차갑게 빛나는 눈이 상대의 약점을 겨눈다",
        "칼집에서 검을 뽑는 소리가 들린다",
        "상대의 빈틈을 노리듯 한 발 앞으로 디딘다"
    };

    static const std::vector<std::string> DefensiveLines =
    {
        "방패를 단단히 고쳐잡는 기척이 난다",
        "방패를 쥔 손에 힘이 들어간다",
        "어깨가 무겁게 움츠러들며 몸을 단단히 가다듬는다",
        "심호흡을 삼키며 방패 뒤로 시선을 감춘다"
    };

    static const std::vector<std::string> BalancedLines =
    {
        "상대가 호흡을 고르며 움직임을 살핀다",
        "다음 행동을 숨기듯 자세를 가다듬는다",
        "차분한 눈빛으로 상대의 움직임을 지켜본다",
        "발걸음을 멈추고 호흡을 가다듬는다"
    };

    switch (InStance)
    {
    case Stance::Aggressive:
        return AttackLines[RandRange() % AttackLines.size()];
    case Stance::Defensive:
        return DefensiveLines[RandRange() % DefensiveLines.size()];
    default:
        return BalancedLines[RandRange() % BalancedLines.size()];
    }
}
