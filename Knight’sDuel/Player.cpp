#include "Player.h"

// 입력이 없으면 기본값(가드/중단)으로 보정
void Player::TakeTurn()
{
    if (choice.Action == ActionType::None)
    {
        choice.Action = ActionType::Guard;
        choice.Direction = CombatDirection::Mid;
    }
}
 
// 라운드 초기화
void Player::RoundResetPlayer(int InRecoveryHP, int InRecoveryST)
{
    // 1) 디버프 제거 전에 경직 복구
    if (!ActiveDebuff.empty())
    {
        for (const Debuff& D : ActiveDebuff)
        {
            if (D.Type == DebuffType::Stagger)
            {
                // 경직으로 깎였던 DEF 복구
                // RefreshDebuff()에서 DEF -= NewDebuff.Value로 깎였음
                DEF += D.Value;  
            }
        }
        ActiveDebuff.clear();  // 디버프 초기화
    }
    // 2) HP, ST 회복 (라운드 보상)
    HP += InRecoveryHP;
    ST += InRecoveryST;

    MaxVital();     // 3) HP/ST 최대치 클램프
    ClearChoice();  // 4) 턴 선택 초기화
}

