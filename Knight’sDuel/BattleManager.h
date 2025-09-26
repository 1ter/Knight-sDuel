#pragma once
#include "Player.h"
#include "Enemy.h"
#include <string>

// 전투 매니저 (적 vs 플레이어)
// 턴 진행/판정/출력 관리
class BattleManager
{
public:
    // ST 소모
    static constexpr int RegenST          = 5;   // 턴 시작   +5
    static constexpr int NormalCostST     = 5;   // 일반공격  -5
    static constexpr int StrongCostST     = 25;  // 강한공격 -25
    static constexpr int DodgeCostST      = 30;  // 회피     -30
    static constexpr int ClashExtraCostST = 10;  // 무기막기 -10 

    // 디버프 확률
    static constexpr int DebuffNormalPct = 30;   // 일반공격 30%
    static constexpr int DebuffStrongPct = 100;  // 강한공격 100%

    // 출력 상태바 폭
    static constexpr int BarWidth = 16;

public:
    // 생성자 
    BattleManager(Player& InPlayer)
        : PlayerActor(InPlayer)   // 플레이어는 참조로 소유
    {
    }
    // 적 설정 (외부 Enemy에서)
    void SetEnemy(Enemy& InEnemy) { EnemyActor = &InEnemy; }

    bool IsBattleEnd() const;                       // 전투 종료 여부       
    void StartTurn();                               // 시작 턴 처리 순서
    void PlanPlayerAttack(CombatDirection InDirection, AttackKind InKind);    // 플레이어 공격 선택
    void PlanPlayerGuard(CombatDirection InDirection);                        // 플레이어 가드 선택
    void PlanPlayerDodge();                                                   // 플레이어 회피 선택획
    void ExecuteTurn();                             // 적 선택 -> ST 검증 -> 타깃 연결 -> 판정
    void PrintStatus() const;   // 로그 출력            

    // 접근자
    Player& GetPlayer() { return PlayerActor; }     // 플레이어 참조 반환
    Enemy& GetEnemy()   { return *EnemyActor; }     // 적 참조 반환 (SetEnemy)

private:
    // 동시 턴제 판정 
    void Resolve();     

    // 데미지 계산
    int DamageCompute(const Actor& InAttacker, const Actor& InDefender, AttackKind InKind) const;

    // 디버프 확률 (일반30%/강공100%)
    Debuff DebuffRoll(CombatDirection InDirection, AttackKind InKind) const;

    // 방향(가드/공격) 상,중,하 일치 여부 확인
    bool DirectionsMatch(CombatDirection InL, CombatDirection InR) const;

    // ST 부족 시 강등 규칙 적용
    void StaminaValid(Actor& InActor, ActionType& OutAct, AttackKind& OutKind) const;

    // 패링 즉시반격 (일반공격 적용, ST 0 소모)
    void ParryCounter(Actor& InGuarder, Actor& InAttacker, CombatDirection InGuardDir);

    // 적 성향에 따른 전조 대사 선택
    const std::string& PickTelegraphLine(Stance InStance) const;

private:
    // 직전 턴 출력 로그 (PrintStatus에서 사용)
    struct TurnLog
    {
        std::string Telegraph;   // [전조]
        std::string EnemyAction; // [적행동]
        std::string Judge;       // [판정]
        std::string Effects;     // [효과] 디버프
        std::string Damage;      // [피해]
        std::string Extra;       // [추가효과] 
    };

    Player& PlayerActor;          // 플레이어(참조)
    Enemy* EnemyActor = nullptr; // 적(포인터, 외부에서 할당)
    TurnLog Log;                 // 직전 턴 로그 결과
};