#pragma once
#include "Actor.h"

// �� ����
enum class Stance
{
    Aggressive, // ������
    Defensive,  // �����
    Balanced    // ������
};

class Enemy : public Actor
{
    public:
        Enemy(const std::string& InName, int InHP, int InST, int InATK, int InDEF, Stance InStance)
            : Actor(InName, InHP, InST, InATK, InDEF), 
              stance(InStance)
        {
        }
        virtual ~Enemy() {}
        virtual void ClearChoice() override { choice = {}; }  // �ʱ�ȭ
        virtual void TakeTurn() override;                     // ���� ��� �ൿ �ڵ� ����

        // ���� Ȯ��
        Stance GetStance() const { return stance; }

    private:
        Stance stance;
};







