#pragma once
class SkillJob
{
public:
	//job = 0 -> ���� ����
	//job = 1 -> ���
	int jobHighClass;  // ��з�(����)
	//int jobMidClass; // �ߺз�(���� ����)
	int jobSmallClass[6];  //�Һз�(��ų)

public:
	SkillJob();

	int KnightSkillSelect();
	void Reset();

	void KnightSkill(int skillNumber);
	int GetKnightSkill(int skillNumber);
};
