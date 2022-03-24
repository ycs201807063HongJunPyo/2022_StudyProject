#pragma once
class SkillJob
{
public:
	//job = 0 -> ���� ����
	//job = 1 -> ���
	//job = 2 -> ����
	//job = 3 -> �����
	int jobHighClass;  // ��з�(����)
	//int jobMidClass; // �ߺз�(���� ����)
	int jobSmallClass[6];  //�Һз�(��ų)

public:
	SkillJob();

	int KnightSkillSelect();
	int AssassinSkillSelect();
	int TechnicianSkillSelect();

	void KnightSkill(int skillNumber);
	void AssassinSkill(int skillNumber);
	void TechnicianSkill(int skillNumber);
	int GetPlayerSelectSkill(int skillNumber);

	void Reset();
};
