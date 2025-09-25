#include "Player.h"

// 아직 입력 없음
void Player::TakeTurn()
{
    if (choice.Action == ActionType::None)   // 기본값(가드/중단)
    {
        choice.Action = ActionType::Guard;
        choice.Direction = CombatDirection::Mid;
        //choice.Kind = AttackKind::Normal;
        //choice.Target = nullptr;            
    }
}
