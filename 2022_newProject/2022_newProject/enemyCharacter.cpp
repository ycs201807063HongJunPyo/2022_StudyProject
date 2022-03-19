#include "enemyCharacter.h"

EnemyCharacter::EnemyCharacter() {
	maxHealth = 25;
	currentHealth = maxHealth;
	damage = 3;
	defence = 2;
	fastAttack = 6;
}

int EnemyCharacter::DefaultAttack(int number, int enemyDamage) {
	int sumDamage = number + enemyDamage;
	return sumDamage;
}

int EnemyCharacter::ImHit(int attackDamage) {
	int current = currentHealth;
	if (attackDamage >= defence) {
		current = currentHealth - (attackDamage - defence);
	}
	return current;
}

void EnemyCharacter::EnemyUnit(int unitNumber, int rank) {
	int unit = unitNumber % 5;
	//´Ü´ÜÇÑ À¯´Ö
	if (unit == 1) {
		maxHealth = rank * 25;
		currentHealth = maxHealth;
		damage = rank + 2;
		defence = (ceil(rank * 1.5)) + 0;
		fastAttack = rank + 5;
	}
	//¼Ó°ø À¯´Ö
	else if (unit == 2) {
		maxHealth = rank * 10;
		currentHealth = maxHealth;
		damage = rank * 2;
		defence = rank + 1;
		fastAttack = (ceil(rank*1.8)) + 14;
	}
	//¹ë·±½ºÇü À¯´Ö
	else if (unit == 3) {
		maxHealth = rank * 15;
		currentHealth = maxHealth;
		damage = (rank * 2) + 2;
		defence = rank + 2;
		fastAttack = rank + 10;
	}
	//º¸½ºÇü À¯´Ö
	else if (unit == 4) {
		maxHealth = rank * 40;
		currentHealth = maxHealth;
		damage = (rank * 4) + 2;
		defence = (rank * 2) - 1;
		fastAttack = rank + 10;
	}
}

int EnemyCharacter::EnemyUnitHeal(int rank) {
	int current = currentHealth;
	int healPoint = 5 + (rank * 2);
	current += healPoint;
	if (current >= maxHealth) {
		current = maxHealth;
	}
	return current;
}

void EnemyCharacter::Reset() {
	maxHealth = 20;
	currentHealth = maxHealth;
	damage = 3;
	defence = 2;
	fastAttack = 5;
}