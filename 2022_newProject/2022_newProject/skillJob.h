#pragma once
class SkillJob
{
public:
	//job = 0 -> 직업 없음
	//job = 1 -> 기사
	int jobHighClass;  // 대분류(직업)
	//int jobMidClass; // 중분류(직업 각인)
	int jobSmallClass[6];  //소분류(스킬)

public:
	SkillJob();

	int KnightSkillSelect();
	void Reset();

	void KnightSkill(int skillNumber);
	int GetKnightSkill(int skillNumber);
};
