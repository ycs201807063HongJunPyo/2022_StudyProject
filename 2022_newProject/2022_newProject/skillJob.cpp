#include "skillJob.h"

SkillJob::SkillJob(){
	jobHighClass = 0;
	for (int i = 0; i <= 5; i++) {
		jobSmallClass[i] = 0;
	}
}

int SkillJob::KnightSkillSelect() {
	jobHighClass = 1;
	return jobHighClass;
}

int SkillJob::AssassinSkillSelect() {
	jobHighClass = 2;
	return jobHighClass;
}

void SkillJob::KnightSkill(int skillNumber) {
	//기사 직업이고 n번째 스킬을 눌렀다면 n번째 스킬을 버프시켜준다.
	if (jobHighClass == 1 && skillNumber == 0) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 1 && skillNumber == 1) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 1 && skillNumber == 2) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 1 && skillNumber == 3) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 1 && skillNumber == 4) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 1 && skillNumber == 5) {
		jobSmallClass[skillNumber] = 1;
	}
}



int SkillJob::GetKnightSkill(int skillNumber) {
	//해당 스킬이 찍혀있으면 1, 안찍혀있으면 0 반환
	if (jobHighClass == 1 && jobSmallClass[skillNumber] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

void SkillJob::AssassinSkill(int skillNumber) {
	//도적 직업이고 n번째 스킬을 눌렀다면 n번째 스킬을 버프시켜준다.
	if (jobHighClass == 2 && skillNumber == 0) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 2 && skillNumber == 1) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 2 && skillNumber == 2) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 2 && skillNumber == 3) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 2 && skillNumber == 4) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 2 && skillNumber == 5) {
		jobSmallClass[skillNumber] = 1;
	}
}



int SkillJob::GetAssassinSkill(int skillNumber) {
	//해당 스킬이 찍혀있으면 1, 안찍혀있으면 0 반환
	if (jobHighClass == 2 && jobSmallClass[skillNumber] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

void SkillJob::Reset() {
	jobHighClass = 0;
	for (int i = 0; i <= 5; i++) {
		jobSmallClass[i] = 0;
	}
}