#pragma once
#include <string>
#include <vector>

// 전투 타입 
enum class ActionType
{
    None,
    Attack,  // 공격
    Guard,   // 가드
    Dodge    // 회피
};

enum class CombatDirection
{
    High,    // 상단
    Mid,     // 중단
    Low      // 하단
};

enum class AttackKind
{
    Normal,  // 일반 공격
    Strong   // 강한 공격
};


// 디버프 타입
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
    DebuffType Type = DebuffType::None;
    int Value = 0;      // 효과량
    int Duration = 0;   // 남은 턴 수

};

class Actor
{
public:
    Actor(const std::string& InName, int InHP, int InST, int InATK, int InDEF)
        : Name(InName), HP(InHP), MaxHP(InHP), ST(InST), MaxST(InST), ATK(InATK), DEF(InDEF)
    {
        ClearChoice();      
    }
    virtual ~Actor() {}             //부모 소멸자(상속 대비)
    virtual void ClearChoice() { choice = {}; }  // 이번 턴의 행동 선택을 초기 상태로
    virtual void TakeTurn() = 0;    // 턴 명시. 만약 행동이 비어 있으면 기본 행동(가드 중단)

    // 리소스 관리
    void MaxVital();                // HP, ST 최대치 제한
    void RegenST();                 // 턴 시작 ST +5
    bool CheckST(int Cost) const;   // ST 확인
    void ConsumeST(int Cost);       // ST 소모

    // 전투 처리
    void TakeDamage(int Damage);                 // 피해 적용
    void RefreshDebuff(const Debuff& NewDebuff); // 디버프 갱신
    void TickDebuff();                           // 턴마다 디버프 처리

    // 상태 확인
    bool IsDead() const { return HP <= 0; }
    int GetHP() const { return HP; }
    int GetST() const { return ST; }
    int GetMaxHP() const noexcept { return MaxHP; }
    int GetMaxST() const noexcept { return MaxST; }
    int GetATK() const { return ATK; }
    int GetDEF() const { return DEF; }
    const std::string& GetName() const { return Name; }

    // 현재 턴 선택 Getter (행동/방향/강도/대상)
    ActionType      GetChoiceAction()    const { return choice.Action; }
    CombatDirection GetChoiceDirection() const { return choice.Direction; }
    AttackKind      GetChoiceKind()      const { return choice.Kind; }
    Actor*          GetChoiceTarget()    const { return choice.Target; }

protected:
    std::string Name; // 이름
    std::vector<Debuff> ActiveDebuff; // 현재 디버프 

    int HP = 100;     // 체력
    int MaxHP = 100;

    int ST = 100;     // 스태미나
    int MaxST = 100;

    int ATK = 0;      // 공격
    int DEF = 0;      // 방어

    struct Choice  // (행동/방향/강도/대상) 
    {
        ActionType Action = ActionType::None;
        CombatDirection Direction = CombatDirection::Mid;
        AttackKind Kind = AttackKind::Normal;
        Actor* Target = nullptr;
    };

    Choice choice;
};

