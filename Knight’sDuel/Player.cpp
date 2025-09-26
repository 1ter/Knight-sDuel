#include "Player.h"

// �Է��� ������ �⺻��(����/�ߴ�)���� ����
void Player::TakeTurn()
{
    if (choice.Action == ActionType::None)
    {
        choice.Action = ActionType::Guard;
        choice.Direction = CombatDirection::Mid;
    }
}
 
// ���� �ʱ�ȭ
void Player::RoundResetPlayer(int InRecoveryHP, int InRecoveryST)
{
    // 1) ����� ���� ���� ���� ����
    if (!ActiveDebuff.empty())
    {
        for (const Debuff& D : ActiveDebuff)
        {
            if (D.Type == DebuffType::Stagger)
            {
                // �������� �𿴴� DEF ����
                // RefreshDebuff()���� DEF -= NewDebuff.Value�� ����
                DEF += D.Value;  
            }
        }
        ActiveDebuff.clear();  // ����� �ʱ�ȭ
    }
    // 2) HP, ST ȸ�� (���� ����)
    HP += InRecoveryHP;
    ST += InRecoveryST;

    MaxVital();     // 3) HP/ST �ִ�ġ Ŭ����
    ClearChoice();  // 4) �� ���� �ʱ�ȭ
}

