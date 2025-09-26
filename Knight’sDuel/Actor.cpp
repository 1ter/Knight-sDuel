#include "Actor.h"
#include <algorithm>
#include <string>

// HP, ST 최대/최소 제한
void Actor::MaxVital()             
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
// 턴 시작 시 ST +5 (최대치 제한 포함)
void Actor::RegenST()                 
{
    ST += 5;
    MaxVital();
}
// ST가 행동에 필요한 Cost 이상인지 확인
bool Actor::CheckST(int Cost) const   
{
    return ST >= Cost;
}
// ST가 행동에 필요한 Cost 이상인지 확인
void Actor::ConsumeST(int Cost)      
{
    ST -= Cost;

    if (ST < 0)
    {
        ST = 0;
    }
}

// 받은 피해 적용
void Actor::TakeDamage(int Damage)    
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

// 새 디버프 재적용,갱신 ('더 강함/더 긴 것')
void Actor::RefreshDebuff(const Debuff& NewDebuff)
{
    if (NewDebuff.Type == DebuffType::None)         // 새로 적용할 디버프가 없는 경우
    {
        return;
    }

    // 동일 타입 디버프 찾기
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

    Debuff& Current = *Iter;    // 기존 디버프가 있을 때 → 갱신 

    if (NewDebuff.Type == DebuffType::Stagger)
    {
        // 경직은 DEF 감소치가 더 큰 쪽을 우선
        int OldDefPenalty = Current.Value;
        int NewDefPenalty = std::max(Current.Value, NewDebuff.Value);
         
        if (NewDefPenalty > OldDefPenalty)
        {
            int DefPenaltyDiff = NewDefPenalty - OldDefPenalty;
            DEF = std::max(0, DEF - DefPenaltyDiff);    // 디버프 적용 현재 방어력      
        }
        // 값/지속 턴 갱신
        Current.Value = NewDefPenalty; 
        Current.Duration = std::max(Current.Duration, NewDebuff.Duration);
        return;
    }
    // 출혈, 약화 : 틱 피해/소모량, 지속 '높은 쪽'으로 갱신
    Current.Value = std::max(Current.Value, NewDebuff.Value);
    Current.Duration = std::max(Current.Duration, NewDebuff.Duration);
}

// 매 턴마다 디버프 효과 적용 및 남은 턴 차감
void Actor::TickDebuff()           
{
    if (ActiveDebuff.empty())
    {
        return;
    }

    // 1) 각 디버프 효과 적용
    for (Debuff& D : ActiveDebuff)
    {
        switch (D.Type)
        {
        case DebuffType::Bleed:     // HP (-Value/턴)

            HP -= D.Value;
            break;

        case DebuffType::Weakness:  // ST (-Value/턴)

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
        if (D.Duration > 0)        // 아직 남아있음 → 유지
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
    // 초기화 하고 갱신
    ActiveDebuff.clear();
    for (const Debuff& D : RemainDebuff) 
    {
        ActiveDebuff.push_back(D);
    }
    // HP, ST 클램프
    MaxVital();   
}

// 디버프 정보를 사람이 읽을 수 있는 문자열로 변환 (로그/출력용)
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
            return "";   // 빈 문자열, 디버프 없음
        }
}

