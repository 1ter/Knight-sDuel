#include "Actor.h"
#include <algorithm>
#include <string>

// ���ҽ� ����
void Actor::MaxVital()             // HP, ST �ִ�/�ּ� ����
{
    if (HP < 0)
    {
        HP = 0;
    }
    if (ST < 0)
    {
        ST = 0;
    }
    if (HP > MaxHP)
    {
        HP = MaxHP;
    }
    if (ST > MaxST)
    {
        ST = MaxST;
    }
}

void Actor::RegenST()                 // �� ���۽� ST +5, �ִ�ġ ����
{
    ST += 5;
    MaxVital();
}

bool Actor::CheckST(int Cost) const   // ST Ȯ�� 
{
    return ST >= Cost;
}

void Actor::ConsumeST(int Cost)       // ST �Ҹ�
{
    ST -= Cost;

    if (ST < 0)
    {
        ST = 0;
    }
}

// ���� ó��
void Actor::TakeDamage(int Damage)     // ���� ���� ����
{
    if (Damage <= 0)
    {
        return;
    }

    HP -= Damage;

    if (HP < 0)
    {
        HP = 0;
    }
}

// ����� ������ �� '�� ����/�� �� ��'���� ����.
void Actor::RefreshDebuff(const Debuff& NewDebuff)
{
    if (NewDebuff.Type == DebuffType::None)         // ���� ������ ������� ���� ���
    {
        return;
    }

    // ���� ����� Ÿ�� ã��
    auto Iter = std::find_if(ActiveDebuff.begin(), ActiveDebuff.end(),
        [NewDebuff](const Debuff& D) 
        { 
            return D.Type == NewDebuff.Type; 
        }
    );

    if (Iter == ActiveDebuff.end())                 // �� ã������
    {
        ActiveDebuff.push_back(NewDebuff);          // �ű� ����

        if (NewDebuff.Type == DebuffType::Stagger)  // ������� �����̸� DEF ����
        {
            DEF -= NewDebuff.Value;

            if (DEF < 0)
            {
                DEF = 0;
            }
        }
        return;
    }

    Debuff& Current = *Iter;    // ���� ���� -> '�� ����/�� �� ��'���� ���� 

    if (NewDebuff.Type == DebuffType::Stagger)
    {
        // ���� ������: ���� �г�Ƽ �ǵ����� �� �г�Ƽ ����
        int OldDefPenalty = Current.Value;
        int NewDefPenalty = std::max(Current.Value, NewDebuff.Value);
         
        if (NewDefPenalty > OldDefPenalty)
        {
            int DefPenaltyDiff = NewDefPenalty - OldDefPenalty;
            DEF = std::max(0, DEF - DefPenaltyDiff);    // Ŭ����, ����� ���� ���� ����      
        }

        Current.Value = NewDefPenalty;
        Current.Duration = std::max(Current.Duration, NewDebuff.Duration);
        return;
    }

    // ����, ��ȭ : ƽ ����/�Ҹ�, ���� '���� ��'���� ����
    Current.Value = std::max(Current.Value, NewDebuff.Value);
    Current.Duration = std::max(Current.Duration, NewDebuff.Duration);
}

void Actor::TickDebuff()            // �ϸ��� ����� ó��
{
    if (ActiveDebuff.empty())
    {
        return;
    }

    // �� ����� ȿ�� ���� (DEF ���� ���� ȿ��)
    for (Debuff& D : ActiveDebuff)
    {
        switch (D.Type)
        {
        case DebuffType::Bleed:     // HP -Value/��

            HP -= D.Value;
            break;

        case DebuffType::Weakness:  // ST -Value/��

            ST -= D.Value;
            break;

        case DebuffType::Stagger:   // DEF ���� ȿ���� �̹� ����Ǿ� ����
            break;

        default:
            break;
        }

        if (D.Duration > 0)         // ����� ���� �� ���� �� ����
        {
            D.Duration -= 1;       
        }
    }

    // ����� ����� ����
    std::vector<Debuff> RemainDebuff;
    RemainDebuff.reserve(ActiveDebuff.size());

    for (const Debuff& D : ActiveDebuff)
    {
        if (D.Duration > 0)
        {
            RemainDebuff.push_back(D);
        }
        else
        {
            if (D.Type == DebuffType::Stagger)  // ������ ���� �� DEF ����
            {
                DEF += D.Value;       
            }
        }
    }

    ActiveDebuff.clear();
    for (const Debuff& D : RemainDebuff) 
    {
        ActiveDebuff.push_back(D);
    }

    MaxVital();  
}

std::string Debuff::ToString() const
{
        std::string DirectionStr;
        switch (Direction)
        {
        case CombatDirection::High: 
            DirectionStr = "���(����ġ��)"; break;
        case CombatDirection::Mid:  
            DirectionStr = "�ߴ�(���)";   break;
        case CombatDirection::Low:  
            DirectionStr = "�ϴ�(�����)"; break;
        default: 
            ;
        }

        switch (Type)
        {
        case DebuffType::Stagger:
            return DirectionStr + " �� ���� DEF -" + std::to_string(Value) + " (" + std::to_string(Duration) + "��)";
        case DebuffType::Bleed:
            return DirectionStr + " �� ���� HP -" + std::to_string(Value) + "/�� (" + std::to_string(Duration) + "��)";
        case DebuffType::Weakness:
            return DirectionStr + " �� ��ȭ ST -" + std::to_string(Value) + "/�� (" + std::to_string(Duration) + "��)";
        default:
            return "";   // �� ���ڿ�
        }
}

