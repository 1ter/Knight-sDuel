#pragma once
#include "Actor.h"

// �÷��̾� Ŭ����
class Player : public Actor
{
public:
    Player(const std::string& InName, int InHP, int InST, int InATK, int InDEF)
        : Actor(InName, InHP, InST, InATK, InDEF)  
    {
    }
    virtual ~Player() {}  
    virtual void ClearChoice() override { choice = {}; }  // ���� �ʱ�ȭ
    virtual void TakeTurn() override; // �Է� ��: ��� ������ �⺻��(����/�ߴ�) ����
};
