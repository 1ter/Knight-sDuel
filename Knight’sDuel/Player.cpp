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
                DEF += D.Value;  // RefreshDebuff()���� DEF -= NewDebuff.Value�� ����
            }
        }
        ActiveDebuff.clear();
    }
    // ȸ��
    HP += InRecoveryHP;
    ST += InRecoveryST;
    MaxVital();     // Ŭ����
    ClearChoice();  // �� ���� �ʱ�ȭ
}

