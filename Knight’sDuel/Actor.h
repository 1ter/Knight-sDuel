#pragma once
#include <string>
#include <vector>

// 전투 타입 
enum class ActionType
{
    None,    // 행동 없음
    Attack,  // 공격
    Guard,   // 가드
    Dodge    // 회피
};
// 전투 방향 (상/중/하단)
enum class CombatDirection
{
    High,    // 상단
    Mid,     // 중단
    Low      // 하단
};
// 공격 종류
enum class AttackKind
{
    Normal,  // 일반 공격
    Strong   // 강한 공격
};
// 디버프 타입 (상태 이상)
enum class DebuffType
{
    None,
    Stagger,   // [경직] DEF 감소
    Bleed,     // [출혈] HP 감소
    Weakness   // [약화] ST 감소
};

// 디버프 구조체
struct Debuff
{
    DebuffType Type = DebuffType::None;               // 디버프 종류
    CombatDirection Direction = CombatDirection::Mid; // 초기화 기본값 그냥 중단으로
    int Value = 0;      // 효과량
    int Duration = 0;   // 남은 턴 수

    std::string ToString() const;  // 디버프 상태 문자열로 변환
};

// 플레이어, 적 부모 클래스
class Actor
{
public:
    // 생성자: 기본 능력치 설정
    Actor(const std::string& InName, int InHP, int InST, int InATK, int InDEF)
        : Name(InName), HP(InHP), MaxHP(InHP), ST(InST), MaxST(InST), ATK(InATK), DEF(InDEF)
    {
        ClearChoice();              // 턴 시작 시 행동 초기화
    }
    virtual ~Actor() {}             // 부모 소멸자(상속 대비)
    virtual void ClearChoice()      // 이번 턴의 행동 선택 초기화
    { 
        choice = { ActionType::None, CombatDirection::Mid, AttackKind::Normal, nullptr };
    }  
    virtual void TakeTurn() = 0;    // 턴 진행. 만약 행동이 비어 있으면 기본 행동(가드 중단)

    // 리소스 관리
    void MaxVital();                // HP, ST 최대치 제한
    void RegenST();                 // 턴 시작 ST +5
    bool CheckST(int Cost) const;   // ST 확인
    void ConsumeST(int Cost);       // ST 소모

    // 전투 처리
    void TakeDamage(int Damage);                 // 피해 적용
    void RefreshDebuff(const Debuff& NewDebuff); // 디버프 갱신
    void TickDebuff();                           // 턴마다 디버프 효과 적용 및 지속시간 감소

    // 턴 입력 선택
    // 이번 턴 공격 (방향/종류/대상)
    void ChoiceAttack(CombatDirection InDirection, AttackKind InKind, Actor* InTarget)
    {
        choice.Action = ActionType::Attack;
        choice.Direction = InDirection;
        choice.Kind = InKind;
        choice.Target = InTarget;
    }
    // 이번 턴 가드 (방향)
    void ChoiceGuard(CombatDirection InDirection)
    {
        choice.Action = ActionType::Guard;
        choice.Direction = InDirection;
    }
    // 이번 턴 회피
    void ChoiceDodge()
    {
        choice.Action = ActionType::Dodge;
    }

    // 상태 확인
    bool IsDead() const { return HP <= 0; }             // 사망여부
    int GetHP() const { return HP; }                    // 현재 HP
    int GetST() const { return ST; }                    // 현재 ST
    int GetMaxHP() const noexcept { return MaxHP; }     // 최대 HP
    int GetMaxST() const noexcept { return MaxST; }     // 최대 ST
    int GetATK() const { return ATK; }                  // 공격력
    int GetDEF() const { return DEF; }                  // 방어력
    const std::string& GetName() const { return Name; } // 이름

    // 현재 턴 선택 Getter (행동/방향/강도/대상)
    ActionType      GetChoiceAction()    const { return choice.Action; }
    CombatDirection GetChoiceDirection() const { return choice.Direction; }
    AttackKind      GetChoiceKind()      const { return choice.Kind; }
    Actor*          GetChoiceTarget()    const { return choice.Target; }

protected:
    std::string Name; // 이름
    std::vector<Debuff> ActiveDebuff; // 현재 적용된 디버프 

    int HP = 100;     // 체력
    int MaxHP = 100;

    int ST = 100;     // 스태미나
    int MaxST = 100;

    int ATK = 0;      // 공격
    int DEF = 0;      // 방어

    // 이번 턴 선택(행동/방향/공격/대상) 
    struct Choice  
    { 
        ActionType Action;         // 행동 (공격/가드/회피)
        CombatDirection Direction; // 공격·가드 방향 (상/중/하)
        AttackKind Kind;           // 공격 종류 (일반/강공)
        Actor* Target;             // 대상 (공격 시 상대)
    };

    Choice choice;   // 현재 턴의 선택 저장
};

