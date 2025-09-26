#pragma once
#include "Player.h"
#include "Enemy.h"
#include <string>

// ���� �Ŵ��� (�� vs �÷��̾�)
// �� ����/����/��� ����
class BattleManager
{
public:
    // ST �Ҹ�
    static constexpr int RegenST          = 5;   // �� ����   +5
    static constexpr int NormalCostST     = 5;   // �Ϲݰ���  -5
    static constexpr int StrongCostST     = 25;  // ���Ѱ��� -25
    static constexpr int DodgeCostST      = 30;  // ȸ��     -30
    static constexpr int ClashExtraCostST = 10;  // ���⸷�� -10 

    // ����� Ȯ��
    static constexpr int DebuffNormalPct = 30;   // �Ϲݰ��� 30%
    static constexpr int DebuffStrongPct = 100;  // ���Ѱ��� 100%

    // ��� ���¹� ��
    static constexpr int BarWidth = 16;

public:
    // ������ 
    BattleManager(Player& InPlayer)
        : PlayerActor(InPlayer)   // �÷��̾�� ������ ����
    {
    }
    // �� ���� (�ܺ� Enemy����)
    void SetEnemy(Enemy& InEnemy) { EnemyActor = &InEnemy; }

    bool IsBattleEnd() const;                       // ���� ���� ����       
    void StartTurn();                               // ���� �� ó�� ����
    void PlanPlayerAttack(CombatDirection InDirection, AttackKind InKind);    // �÷��̾� ���� ����
    void PlanPlayerGuard(CombatDirection InDirection);                        // �÷��̾� ���� ����
    void PlanPlayerDodge();                                                   // �÷��̾� ȸ�� ����ȹ
    void ExecuteTurn();                             // �� ���� -> ST ���� -> Ÿ�� ���� -> ����
    void PrintStatus() const;   // �α� ���            

    // ������
    Player& GetPlayer() { return PlayerActor; }     // �÷��̾� ���� ��ȯ
    Enemy& GetEnemy()   { return *EnemyActor; }     // �� ���� ��ȯ (SetEnemy)

private:
    // ���� ���� ���� 
    void Resolve();     

    // ������ ���
    int DamageCompute(const Actor& InAttacker, const Actor& InDefender, AttackKind InKind) const;

    // ����� Ȯ�� (�Ϲ�30%/����100%)
    Debuff DebuffRoll(CombatDirection InDirection, AttackKind InKind) const;

    // ����(����/����) ��,��,�� ��ġ ���� Ȯ��
    bool DirectionsMatch(CombatDirection InL, CombatDirection InR) const;

    // ST ���� �� ���� ��Ģ ����
    void StaminaValid(Actor& InActor, ActionType& OutAct, AttackKind& OutKind) const;

    // �и� ��ùݰ� (�Ϲݰ��� ����, ST 0 �Ҹ�)
    void ParryCounter(Actor& InGuarder, Actor& InAttacker, CombatDirection InGuardDir);

    // �� ���⿡ ���� ���� ��� ����
    const std::string& PickTelegraphLine(Stance InStance) const;

private:
    // ���� �� ��� �α� (PrintStatus���� ���)
    struct TurnLog
    {
        std::string Telegraph;   // [����]
        std::string EnemyAction; // [���ൿ]
        std::string Judge;       // [����]
        std::string Effects;     // [ȿ��] �����
        std::string Damage;      // [����]
        std::string Extra;       // [�߰�ȿ��] 
    };

    Player& PlayerActor;          // �÷��̾�(����)
    Enemy* EnemyActor = nullptr; // ��(������, �ܺο��� �Ҵ�)
    TurnLog Log;                 // ���� �� �α� ���
};