#pragma once
#include "Actor.h"
#include <vector>
#include <string>

// �� ����
enum class Stance
{
	Aggressive, // ������
	Defensive,  // �����
	Balanced    // ������
};

inline const std::vector<std::string> AttackLines =
{
	"���� Į�ڷ縦 ���� ������ �����δ�",
	"������ ������ ���� ����� ������ �ܴ���",
	"Į������ ���� �̴� �Ҹ��� �鸰��",
	"����� ��ƴ�� �븮�� �� �� ������ ����"
};
inline const std::vector<std::string> DefenceLines =
{
	"���и� �ܴ��� ������� ��ô�� ����",
	"���и� �� �տ� ���� ����",
	"����� ���̰� ��������� ���� �ܴ��� ���ٵ�´�",
	"��ȣ���� ��Ű�� ���� �ڷ� �ü��� �����"
};
inline const std::vector<std::string> BalancedLines =
{
	"�밡 ȣ���� ���� �������� ���ɴ�",
	"���� �ൿ�� ����� �ڼ��� ���ٵ�´�",
	"������ �������� ����� �������� ���Ѻ���",
	"�߰����� ���߰� ȣ���� ���ٵ�´�"
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







