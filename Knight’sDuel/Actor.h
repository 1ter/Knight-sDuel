#pragma once
#include <string>
#include <vector>

// ���� Ÿ�� 
enum class ActionType
{
    None,    // �ൿ ����
    Attack,  // ����
    Guard,   // ����
    Dodge    // ȸ��
};
// ���� ���� (��/��/�ϴ�)
enum class CombatDirection
{
    High,    // ���
    Mid,     // �ߴ�
    Low      // �ϴ�
};
// ���� ����
enum class AttackKind
{
    Normal,  // �Ϲ� ����
    Strong   // ���� ����
};
// ����� Ÿ�� (���� �̻�)
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
    DebuffType Type = DebuffType::None;               // ����� ����
    CombatDirection Direction = CombatDirection::Mid; // �ʱ�ȭ �⺻�� �׳� �ߴ�����
    int Value = 0;      // ȿ����
    int Duration = 0;   // ���� �� ��

    std::string ToString() const;  // ����� ���� ���ڿ��� ��ȯ
};

// �÷��̾�, �� �θ� Ŭ����
class Actor
{
public:
    // ������: �⺻ �ɷ�ġ ����
    Actor(const std::string& InName, int InHP, int InST, int InATK, int InDEF)
        : Name(InName), HP(InHP), MaxHP(InHP), ST(InST), MaxST(InST), ATK(InATK), DEF(InDEF)
    {
        ClearChoice();              // �� ���� �� �ൿ �ʱ�ȭ
    }
    virtual ~Actor() {}             // �θ� �Ҹ���(��� ���)
    virtual void ClearChoice()      // �̹� ���� �ൿ ���� �ʱ�ȭ
    { 
        choice = { ActionType::None, CombatDirection::Mid, AttackKind::Normal, nullptr };
    }  
    virtual void TakeTurn() = 0;    // �� ����. ���� �ൿ�� ��� ������ �⺻ �ൿ(���� �ߴ�)

    // ���ҽ� ����
    void MaxVital();                // HP, ST �ִ�ġ ����
    void RegenST();                 // �� ���� ST +5
    bool CheckST(int Cost) const;   // ST Ȯ��
    void ConsumeST(int Cost);       // ST �Ҹ�

    // ���� ó��
    void TakeDamage(int Damage);                 // ���� ����
    void RefreshDebuff(const Debuff& NewDebuff); // ����� ����
    void TickDebuff();                           // �ϸ��� ����� ȿ�� ���� �� ���ӽð� ����

    // �� �Է� ����
    // �̹� �� ���� (����/����/���)
    void ChoiceAttack(CombatDirection InDirection, AttackKind InKind, Actor* InTarget)
    {
        choice.Action = ActionType::Attack;
        choice.Direction = InDirection;
        choice.Kind = InKind;
        choice.Target = InTarget;
    }
    // �̹� �� ���� (����)
    void ChoiceGuard(CombatDirection InDirection)
    {
        choice.Action = ActionType::Guard;
        choice.Direction = InDirection;
    }
    // �̹� �� ȸ��
    void ChoiceDodge()
    {
        choice.Action = ActionType::Dodge;
    }

    // ���� Ȯ��
    bool IsDead() const { return HP <= 0; }             // �������
    int GetHP() const { return HP; }                    // ���� HP
    int GetST() const { return ST; }                    // ���� ST
    int GetMaxHP() const noexcept { return MaxHP; }     // �ִ� HP
    int GetMaxST() const noexcept { return MaxST; }     // �ִ� ST
    int GetATK() const { return ATK; }                  // ���ݷ�
    int GetDEF() const { return DEF; }                  // ����
    const std::string& GetName() const { return Name; } // �̸�

    // ���� �� ���� Getter (�ൿ/����/����/���)
    ActionType      GetChoiceAction()    const { return choice.Action; }
    CombatDirection GetChoiceDirection() const { return choice.Direction; }
    AttackKind      GetChoiceKind()      const { return choice.Kind; }
    Actor*          GetChoiceTarget()    const { return choice.Target; }

protected:
    std::string Name; // �̸�
    std::vector<Debuff> ActiveDebuff; // ���� ����� ����� 

    int HP = 100;     // ü��
    int MaxHP = 100;

    int ST = 100;     // ���¹̳�
    int MaxST = 100;

    int ATK = 0;      // ����
    int DEF = 0;      // ���

    // �̹� �� ����(�ൿ/����/����/���) 
    struct Choice  
    { 
        ActionType Action;         // �ൿ (����/����/ȸ��)
        CombatDirection Direction; // ���ݡ����� ���� (��/��/��)
        AttackKind Kind;           // ���� ���� (�Ϲ�/����)
        Actor* Target;             // ��� (���� �� ���)
    };

    Choice choice;   // ���� ���� ���� ����
};

