#pragma once
#include "Actor.h"
#include <vector>
#include <string>

// 적 성향
enum class Stance
{
	Aggressive, // 공격형
	Defensive,  // 방어형
	Balanced    // 균형형
};

inline const std::vector<std::string> AttackLines =
{
	"손이 칼자루를 향해 서서히 움직인다",
	"차갑게 빛나는 눈이 상대의 약점을 겨눈다",
	"칼집에서 검을 뽑는 소리가 들린다",
	"상대의 빈틈을 노리듯 한 발 앞으로 디딘다"
};
inline const std::vector<std::string> DefenceLines =
{
	"방패를 단단히 고쳐잡는 기척이 난다",
	"방패를 쥔 손에 힘이 들어간다",
	"어깨가 무겁게 움츠러들며 몸을 단단히 가다듬는다",
	"심호흡을 삼키며 방패 뒤로 시선을 감춘다"
};
inline const std::vector<std::string> BalancedLines =
{
	"대가 호흡을 고르며 움직임을 살핀다",
	"다음 행동을 숨기듯 자세를 가다듬는다",
	"차분한 눈빛으로 상대의 움직임을 지켜본다",
	"발걸음을 멈추고 호흡을 가다듬는다"
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







