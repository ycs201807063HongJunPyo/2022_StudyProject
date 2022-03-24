#pragma once
class SkillJob
{
public:
	//job = 0 -> 직업 없음
	//job = 1 -> 기사
	//job = 2 -> 도적
	//job = 3 -> 기술자
	int jobHighClass;  // 대분류(직업)
	//int jobMidClass; // 중분류(직업 각인)
	int jobSmallClass[6];  //소분류(스킬)

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
