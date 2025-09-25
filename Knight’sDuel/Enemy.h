#pragma once
#include "Actor.h"
#include <vector>
#include <string>

// �� ����
enum class Stance
{
    Aggressive, // ������
    Defensive,  // �����
    Balanced    // ������
};

// ����(�ڷ��׷���) ��� ���� (��� ���� ���� ����: inline)
inline const std::vector<std::string> AttackLines =
{
    "���� Į�ڷ縦 ���� ������ �����δ�",
    "������ ������ ���� ����� ������ �ܴ���",
    "Į������ ���� �̴� �Ҹ��� �鸰��",
    "����� ��ƴ�� �븮�� �� �� ������ ����"
};

inline const std::vector<std::string> DefensiveLines =
{
    "���и� �ܴ��� ������� ��ô�� ����",
    "���и� �� �տ� ���� ����",
    "����� ���̰� ��������� ���� �ܴ��� ���ٵ�´�",
    "��ȣ���� ��Ű�� ���� �ڷ� �ü��� �����"
};

inline const std::vector<std::string> BalancedLines =
{
    "��밡 ȣ���� ���� �������� ���ɴ�",
    "���� �ൿ�� ����� �ڼ��� ���ٵ�´�",
    "������ �������� ����� �������� ���Ѻ���",
    "�߰����� ���߰� ȣ���� ���ٵ�´�"
};

// Enemy Ŭ����
class Enemy : public Actor
{
public:
    Enemy(const std::string& InName, int InHP, int InST, int InATK, int InDEF, Stance InStance)
        : Actor(InName, InHP, InST, InATK, InDEF)
        , stance(InStance)
    {
    }

    virtual ~Enemy() {}
    virtual void ClearChoice() override { choice = {}; } // ���� �ʱ�ȭ
    virtual void TakeTurn() override;                    // (����/����/����) �ൿ ���� 

    Stance GetStance() const { return stance; }          // ���� Ȯ��

private:
    // ����(�ڷ��׷���) 30% �ߵ�
    bool TelegraphRoll() const;

    // ���⺰ ���� ��� �� �� ��ȯ
    const std::string& TelegraphLinePick(Stance InStance) const;

    // ���� �⺻ ����(���� ����)
    void BaseDistribution(Stance InStance, int& OutNormal, int& OutStrong, int& OutGuard) const;

    // ������ ���� �� '��� ����' (������: 70/20/10, �����: 30/20/50, ������: 50/20/30)
    void TelegraphDistribution(Stance InStance, int& InOutNormal, int& InOutStrong, int& InOutGuard) const;

    // �ۼ�Ʈ ������ �ൿ/�������� ����
    ActionType PickAction(int NormalPct, int StrongPct, int GuardPct, AttackKind& OutKind) const;

    // High/Mid/Low ������
    CombatDirection PickDirection() const;

    // ST ���� �� �ڵ� ���� (����->�Ϲ�->����)
    void CheckStamina(ActionType& InOutAct, AttackKind& InOutKind, int InST) const;

private:
    Stance stance;
    static constexpr int TelegraphPercent = 30;    // ���� �ߵ���(30%)
};


