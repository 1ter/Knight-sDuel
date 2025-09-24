#include "Player.h"

void Player::TakeTurn() 
{
    switch (Planned.Action) {
    case PlayerAction::Attack:
        if (Planned.Target) {
            // 공격 로직 (예: Planned.Target->TakeDamage(ATK);)
        }
        break;
    case PlayerAction::Guard:
        // 가드 로직
        break;
    case PlayerAction::Skill:
        if (Planned.Target) {
            // 스킬 로직 (Planned.Param == SkillId)
        }
        break;
    case PlayerAction::Item:
        if (Planned.Target) {
            // 아이템 사용 로직 (Planned.Param == ItemId)
        }
        break;
    default:
        // None: 아무 것도 안 함 (혹은 기본 행동)
        break;
    }
    ClearPlan(); // 다음 턴을 위해 계획 초기화
}