#pragma once

#include<iostream>

using namespace std;

class EnemyCharacter {
private:
	int maxHealth;
	int currentHealth;
	int damage;
	int defence;
	int fastAttack;

public:
	EnemyCharacter();
	int DefaultAttack(int number, int damage);
	int ImHit(int attackDamage);
	void EnemyUnit(int unitNumber, int rank);
	int EnemyUnitHeal(int rank);
	void Reset();

	int getHealth() {
		return maxHealth;
	}
	int getCurrentHealth() {
		return currentHealth;
	}
	int getFastAttack() {
		return fastAttack;
	}
	int getDamage() {
		return damage;
	}
	int getDefence() {
		return defence;
	}

	void setMaxHealth(int current) {
		maxHealth = current;
	}
	void setCurrentHealth(int current) {
		currentHealth = current;
	}

	void setDefenceHealth(int current) {
		defence = current;
	}
};