#include "Actor.h"
#include <algorithm>
#include <string>

// 리소스 관리
void Actor::MaxVital()             // HP, ST 최대/최소 제한
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

void Actor::RegenST()                 // 턴 시작시 ST +5, 최대치 제한
{
    ST += 5;
    MaxVital();
}

bool Actor::CheckST(int Cost) const   // ST 확인 
{
    return ST >= Cost;
}

void Actor::ConsumeST(int Cost)       // ST 소모
{
    ST -= Cost;

    if (ST < 0)
    {
        ST = 0;
    }
}

// 전투 처리
void Actor::TakeDamage(int Damage)     // 받은 피해 적용
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

// 디버프 재적용 시 '더 강함/더 긴 것'으로 갱신.
void Actor::RefreshDebuff(const Debuff& NewDebuff)
{
    if (NewDebuff.Type == DebuffType::None)         // 새로 적용할 디버프가 없는 경우
    {
        return;
    }

    // 같은 디버프 타입 찾기
    auto Iter = std::find_if(ActiveDebuff.begin(), ActiveDebuff.end(),
        [NewDebuff](const Debuff& D) 
        { 
            return D.Type == NewDebuff.Type; 
        }
    );

    if (Iter == ActiveDebuff.end())                 // 못 찾았을시
    {
        ActiveDebuff.push_back(NewDebuff);          // 신규 적용

        if (NewDebuff.Type == DebuffType::Stagger)  // 디버프가 경직이면 DEF 감소
        {
            DEF -= NewDebuff.Value;

            if (DEF < 0)
            {
                DEF = 0;
            }
        }
        return;
    }

    Debuff& Current = *Iter;    // 기존 존재 -> '더 강함/더 긴 것'으로 갱신 

    if (NewDebuff.Type == DebuffType::Stagger)
    {
        // 경직 재조정: 기존 패널티 되돌리고 새 패널티 적용
        int OldDefPenalty = Current.Value;
        int NewDefPenalty = std::max(Current.Value, NewDebuff.Value);
         
        if (NewDefPenalty > OldDefPenalty)
        {
            int DefPenaltyDiff = NewDefPenalty - OldDefPenalty;
            DEF = std::max(0, DEF - DefPenaltyDiff);    // 클램프, 디버프 적용 현재 방어력      
        }

        Current.Value = NewDefPenalty;
        Current.Duration = std::max(Current.Duration, NewDebuff.Duration);
        return;
    }

    // 출혈, 약화 : 틱 피해/소모량, 지속 '높은 쪽'으로 갱신
    Current.Value = std::max(Current.Value, NewDebuff.Value);
    Current.Duration = std::max(Current.Duration, NewDebuff.Duration);
}

void Actor::TickDebuff()            // 턴마다 디버프 처리
{
    if (ActiveDebuff.empty())
    {
        return;
    }

    // 각 디버프 효과 적용 (DEF 무시 고정 효과)
    for (Debuff& D : ActiveDebuff)
    {
        switch (D.Type)
        {
        case DebuffType::Bleed:     // HP -Value/턴

            HP -= D.Value;
            break;

        case DebuffType::Weakness:  // ST -Value/턴

            ST -= D.Value;
            break;

        case DebuffType::Stagger:   // DEF 감소 효과는 이미 적용되어 있음
            break;

        default:
            break;
        }

        if (D.Duration > 0)         // 디버프 있을 때 남은 턴 감소
        {
            D.Duration -= 1;       
        }
    }

    // 만료된 디버프 제거
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
            if (D.Type == DebuffType::Stagger)  // 경직은 만료 시 DEF 복구
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
            DirectionStr = "상단(내려치기)"; break;
        case CombatDirection::Mid:  
            DirectionStr = "중단(찌르기)";   break;
        case CombatDirection::Low:  
            DirectionStr = "하단(쓸어베기)"; break;
        default: 
            ;
        }

        switch (Type)
        {
        case DebuffType::Stagger:
            return DirectionStr + " → 경직 DEF -" + std::to_string(Value) + " (" + std::to_string(Duration) + "턴)";
        case DebuffType::Bleed:
            return DirectionStr + " → 출혈 HP -" + std::to_string(Value) + "/턴 (" + std::to_string(Duration) + "턴)";
        case DebuffType::Weakness:
            return DirectionStr + " → 약화 ST -" + std::to_string(Value) + "/턴 (" + std::to_string(Duration) + "턴)";
        default:
            return "";   // 빈 문자열
        }
}

