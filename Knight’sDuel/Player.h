#pragma once
#include "Actor.h"

// �÷��̾� Ŭ����
// Actor�� ���
// �Է��� ���� �� �ൿ�� ����
class Player : public Actor
{
public:
    // ������: �̸�, HP, ST, ATK, DEF �ʱ�ȭ
    Player(const std::string& InName, int InHP, int InST, int InATK, int InDEF)
        : Actor(InName, InHP, InST, InATK, InDEF)
    {
    }
    virtual ~Player() {}
    virtual void ClearChoice() override { choice = {}; }       // �� �ʱ�ȭ (���� �ʱ�ȭ)
    virtual void TakeTurn() override;                          // �÷��̾� �� ó��, �Է� ���� �� �⺻��(����/�ߴ�) ����

    // ���� ���� �� �÷��̾� ���� ����
    // HP, ST �Ϻ� ȸ�� (���� Ŭ���� ����)
    void RoundResetPlayer(int InRecoveryHP, int InRecoveryST); 
};

