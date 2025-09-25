#pragma once
#include "Actor.h"

// 플레이어 클래스
class Player : public Actor
{
public:
    Player(const std::string& InName, int InHP, int InST, int InATK, int InDEF)
        : Actor(InName, InHP, InST, InATK, InDEF)  
    {
    }
    virtual ~Player() {}  
    virtual void ClearChoice() override { choice = {}; }  // 선택 초기화
    virtual void TakeTurn() override; // 입력 전: 비어 있으면 기본값(가드/중단) 보정
};
