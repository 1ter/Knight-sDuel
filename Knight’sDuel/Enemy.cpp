#include "Enemy.h"
#include <random>

/*
  - 성향: 랜덤 생성
	   공격형: 일반 60%, 가드 20%, 강공 20%
	   방어형: 가드 60%, 일반 20%, 강공 20%
	   균형형: 일반 40%, 가드 40%, 강공 20%

   - 전조(텔레그래프): 대화를 통한 심리전 강화
	   예시) [칼집에서 검을 뽑는 소리가 들립니다.]
	   공격형 성향 (대사 뜰시 공격 확률 30% + 일반공격 확률 60% = 공격 확률 90%)

	공 : 손이 칼자루를 향해 서서히 움직인다
		 차갑게 빛나는 눈이 상대의 약점을 겨눈다
		 칼집에서 검을 뽑는 소리가 들린다
		 상대의 빈틈을 노리듯 한 발 앞으로 디딘다
	방:  방패를 단단히 고쳐잡는 기척이 난다
		 방패를 쥔 손에 힘이 들어간다 .
		 어깨가 무겁게 움츠러들며 몸을 단단히 가다듬는다
		 심호흡을 삼키며 방패 뒤로 시선을 감춘다
	균형 :상대가 호흡을 고르며 움직임을 살핀다
		  다음 행동을 숨기듯 자세를 가다듬는다
		  차분한 눈빛으로 상대의 움직임을 지켜본다
		  발걸음을 멈추고 호흡을 가다듬는다
*/



// 간단 범위 정수 난수
static int RandomRange(int Min, int Max)
{
	return Min + rand() % (Max - Min + 1);
}

void Enemy::TakeTurn()    
{
	/////////////////////////////////될지 안될지 모름
	//전조 대사
	const bool telegraph = (RandomRange(1, 100) <= 30);  // 전조 확률 30 %

	if (telegraph)
	{
		const std::vector<std::string>* TargetLines = nullptr;

		switch (stance)
		{
		case Stance::Aggressive:

			TargetLines = &AttackLines;
			break;

		case Stance::Defensive:

			TargetLines = &DefenceLines;
			break;

		case Stance::Balanced:

			TargetLines = &BalancedLines;
			break;

		default:
			return;
		}

		if (TargetLines && !TargetLines->empty())
		{
			int MaxIndex = static_cast<int>(TargetLines->size()) - 1;
			int RandomIndex = RandomRange(0, MaxIndex);

			printf("전조: %s\n", TargetLines->at(RandomIndex).c_str()); // 대사 출력
		}
	}
	else
	{
		// 전조가 발생하지 않을 확률 70% 
		// printf("아무런 기척도 없다.\n");
	}
}

	// 성향(Stance)에 따른 기본 선택 확률
	int EnemyStanceRoll = RandomRange(1, 100);

	ActionType Act = ActionType::Guard;
	AttackKind Kind = AttackKind::Normal;

	/////////////////////////////////// 여기부터 손봐야 됨
	switch (stance)
	{
	case Stance::Aggressive:       // 공격형: 일반 60%, 가드 20%, 강공 20%

		if (EnemyStanceRoll <= 60)
		{
			Act = ActionType::Attack;
			Kind = AttackKind::Normal;
		}
		else if (EnemyStanceRoll <= 80)
		{
			Act = ActionType::Guard;
		}
		else
		{
			Act = ActionType::Attack;
			Kind = AttackKind::Strong;
		}
		break;

	case Stance::Defensive:       // 방어형: 가드 60%, 일반 20%, 강공 20%

		if (EnemyStanceRoll <= 60)
		{
			Act = ActionType::Guard;
		}
		else if (EnemyStanceRoll <= 80)
		{
			Act = ActionType::Attack;
			Kind = AttackKind::Normal;
		}
		else
		{
			Act = ActionType::Attack;
			Kind = AttackKind::Strong;
		}
		break;

	case Stance::Balanced:    // 균형형: 일반 40%, 가드 40%, 강공 20%

		if (EnemyStanceRoll <= 40)
		{
			Act = ActionType::Attack;
			Kind = AttackKind::Normal;
		}
		else if (EnemyStanceRoll <= 80)
		{
			Act = ActionType::Guard;
		}
		else
		{
			Act = ActionType::Attack;
			Kind = AttackKind::Strong;
		}
		break;

	default:
		break;
	}

	// ST(스태미나) 보정
	const int AttackNormalCost = 5;
	const int AttackStrongCost = 25;
	const int DodgeCost = 30;

	if (Act == ActionType::Attack)
	{
		if (Kind == AttackKind::Strong && ST < AttackStrongCost)
		{
			// ST 부족하면 강한 공격 불가 -> 일반 공격도 안되면 -> 가드
			if (ST >= AttackNormalCost)
			{
				Kind = AttackKind::Normal;
			}
			else
			{
				Act = ActionType::Guard;
			}
		}
		else if (Kind == AttackKind::Normal && ST < AttackNormalCost)
		{
			Act = ActionType::Guard;
		}
	}
	else if (Act == ActionType::Dodge)
	{
		if (ST < DodgeCost)
		{
			Act = ActionType::Guard;
		}
	}

	choice.Action = Act;   // 최종 선택 하기

	if (Act == ActionType::Attack || Act == ActionType::Guard)
	{
		// High, Mid, Low 확률 선택
		choice.Direction = static_cast<CombatDirection>(RandomRange(0, 2));
	}
	if (Act == ActionType::Attack)
	{
		choice.Kind = Kind;
	}
	else
	{
		choice.Kind = AttackKind::Normal;
	}

	choice.Target = nullptr;
}



