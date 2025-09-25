#pragma once
#include "Actor.h"

// 적 성향
enum class Stance
{
    Aggressive, // 공격형
    Defensive,  // 방어형
    Balanced    // 균형형
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
        virtual void ClearChoice() override { choice = {}; }  // 초기화
        virtual void TakeTurn() override;                     // 성향 기반 행동 자동 선택

        // 상태 확인
        Stance GetStance() const { return stance; }

    private:
        Stance stance;
};







