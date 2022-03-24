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

int SkillJob::TechnicianSkillSelect() {
	jobHighClass = 3;
	return jobHighClass;
}

void SkillJob::KnightSkill(int skillNumber) {
	//��� �����̰� n��° ��ų�� �����ٸ� n��° ��ų�� ���������ش�.
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

void SkillJob::AssassinSkill(int skillNumber) {
	//���� �����̰� n��° ��ų�� �����ٸ� n��° ��ų�� ���������ش�.
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

void SkillJob::TechnicianSkill(int skillNumber) {
	//����� �����̰� n��° ��ų�� �����ٸ� n��° ��ų�� ���������ش�.
	if (jobHighClass == 3 && skillNumber == 0) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 3 && skillNumber == 1) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 3 && skillNumber == 2) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 3 && skillNumber == 3) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 3 && skillNumber == 4) {
		jobSmallClass[skillNumber] = 1;
	}
	else if (jobHighClass == 3 && skillNumber == 5) {
		jobSmallClass[skillNumber] = 1;
	}
}

int SkillJob::GetPlayerSelectSkill(int skillNumber) {
	//�ش� ��ų�� ���������� 1, ������������ 0 ��ȯ
	if (jobHighClass == 1 && jobSmallClass[skillNumber] == 1) {
		return 1;
	}
	else if (jobHighClass == 2 && jobSmallClass[skillNumber] == 1) {
		return 1;
	}
	else if (jobHighClass == 3 && jobSmallClass[skillNumber] == 1) {
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