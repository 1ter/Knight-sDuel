#pragma once
#include <string>
#include <vector>

// ���� Ÿ�� 
enum class ActionType
{
    None,
    Attack,  // ����
    Guard,   // ����
    Dodge    // ȸ��
};

enum class CombatDirection
{
    High,    // ���
    Mid,     // �ߴ�
    Low      // �ϴ�
};

enum class AttackKind
{
    Normal,  // �Ϲ� ����
    Strong   // ���� ����
};


// ����� Ÿ��
enum class DebuffType
{
    None,
    Stagger,   // [����] DEF ����
    Bleed,     // [����] HP ����
    Weakness   // [��ȭ] ST ����
};

// ����� ����ü
struct Debuff
{
    DebuffType Type = DebuffType::None;
    int Value = 0;      // ȿ����
    int Duration = 0;   // ���� �� ��

};

class Actor
{
public:
    Actor(const std::string& InName, int InHP, int InST, int InATK, int InDEF)
        : Name(InName), HP(InHP), MaxHP(InHP), ST(InST), MaxST(InST), ATK(InATK), DEF(InDEF)
    {
        ClearChoice();      
    }
    virtual ~Actor() {}             //�θ� �Ҹ���(��� ���)
    virtual void ClearChoice() { choice = {}; }  // �̹� ���� �ൿ ������ �ʱ� ���·�
    virtual void TakeTurn() = 0;    // �� ���. ���� �ൿ�� ��� ������ �⺻ �ൿ(���� �ߴ�)

    // ���ҽ� ����
    void MaxVital();                // HP, ST �ִ�ġ ����
    void RegenST();                 // �� ���� ST +5
    bool CheckST(int Cost) const;   // ST Ȯ��
    void ConsumeST(int Cost);       // ST �Ҹ�

    // ���� ó��
    void TakeDamage(int Damage);                 // ���� ����
    void RefreshDebuff(const Debuff& NewDebuff); // ����� ����
    void TickDebuff();                           // �ϸ��� ����� ó��

    // ���� Ȯ��
    bool IsDead() const { return HP <= 0; }
    int GetHP() const { return HP; }
    int GetST() const { return ST; }
    int GetMaxHP() const noexcept { return MaxHP; }
    int GetMaxST() const noexcept { return MaxST; }
    int GetATK() const { return ATK; }
    int GetDEF() const { return DEF; }
    const std::string& GetName() const { return Name; }

    // ���� �� ���� Getter (�ൿ/����/����/���)
    ActionType      GetChoiceAction()    const { return choice.Action; }
    CombatDirection GetChoiceDirection() const { return choice.Direction; }
    AttackKind      GetChoiceKind()      const { return choice.Kind; }
    Actor*          GetChoiceTarget()    const { return choice.Target; }

protected:
    std::string Name; // �̸�
    std::vector<Debuff> ActiveDebuff; // ���� ����� 

    int HP = 100;     // ü��
    int MaxHP = 100;

    int ST = 100;     // ���¹̳�
    int MaxST = 100;

    int ATK = 0;      // ����
    int DEF = 0;      // ���

    struct Choice  // (�ൿ/����/����/���) 
    {
        ActionType Action = ActionType::None;
        CombatDirection Direction = CombatDirection::Mid;
        AttackKind Kind = AttackKind::Normal;
        Actor* Target = nullptr;
    };

    Choice choice;
};

