#pragma once
#include "Actor.h"

// 플레이어 클래스
// Actor를 상속
// 입력을 통해 턴 행동을 결정
class Player : public Actor
{
public:
    // 생성자: 이름, HP, ST, ATK, DEF 초기화
    Player(const std::string& InName, int InHP, int InST, int InATK, int InDEF)
        : Actor(InName, InHP, InST, InATK, InDEF)
    {
    }
    virtual ~Player() {}
    virtual void ClearChoice() override { choice = {}; }       // 턴 초기화 (선택 초기화)
    virtual void TakeTurn() override;                          // 플레이어 턴 처리, 입력 없을 때 기본값(가드/중단) 보정

    // 라운드 시작 시 플레이어 상태 리셋
    // HP, ST 일부 회복 (라운드 클리어 보상)
    void RoundResetPlayer(int InRecoveryHP, int InRecoveryST); 
};

