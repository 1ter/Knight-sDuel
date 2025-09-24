#include "Player.h"

void Player::TakeTurn() 
{
    switch (Planned.Action) {
    case PlayerAction::Attack:
        if (Planned.Target) {
            // ���� ���� (��: Planned.Target->TakeDamage(ATK);)
        }
        break;
    case PlayerAction::Guard:
        // ���� ����
        break;
    case PlayerAction::Skill:
        if (Planned.Target) {
            // ��ų ���� (Planned.Param == SkillId)
        }
        break;
    case PlayerAction::Item:
        if (Planned.Target) {
            // ������ ��� ���� (Planned.Param == ItemId)
        }
        break;
    default:
        // None: �ƹ� �͵� �� �� (Ȥ�� �⺻ �ൿ)
        break;
    }
    ClearPlan(); // ���� ���� ���� ��ȹ �ʱ�ȭ
}