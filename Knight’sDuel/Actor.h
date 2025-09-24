#pragma once
#include <string>
#include <vector>

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
        ActiveDebuff.clear();
    }
    virtual ~Actor() {}  //�θ� �Ҹ���(��� ���)

    virtual void TakeTurn() = 0;

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

protected:
    std::string Name;

    int HP = 100;
    int MaxHP = 100;

    int ST = 100;
    int MaxST = 100;

    int ATK = 0;
    int DEF = 0;

    std::vector<Debuff> ActiveDebuff;
};