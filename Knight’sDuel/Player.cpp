#include "Player.h"

// ���� �Է� ����
void Player::TakeTurn()
{
    if (choice.Action == ActionType::None)   // �⺻��(����/�ߴ�)
    {
        choice.Action = ActionType::Guard;
        choice.Direction = CombatDirection::Mid;
        //choice.Kind = AttackKind::Normal;
        //choice.Target = nullptr;            
    }
}
