#pragma once

#include<iostream>

using namespace std;

class MyCharacter {
private:
	int maxHealth;
	int currentHealth;
	int damage;
	int defence;
	int fastAttack;

public:
	MyCharacter();
	int DefaultAttack(int number, int damage);
	int ImHit(int attackDamage);
	void Reset();

	int getHealth() {
		return maxHealth;
	}
	int getCurrentHealth() {
		return currentHealth;
	}
	int getDamage() {
		return damage;
	}
	int getDefence() {
		return defence;
	}
	int getFastAttack() {
		return fastAttack;
	}
	

	void setMaxHealth(int current) {
		maxHealth = current;
	}
	void setCurrentHealth(int current) {
		currentHealth = current;
	}
	void setDamage(int strStat) {
		damage += strStat;
	}
	void setDefence(int dpStat) {
		defence += dpStat;
	}
	void setHealth(int dpStat) {
		maxHealth += dpStat;
		currentHealth += dpStat;
	}
	void setFastAttack(int dpStat) {
		fastAttack += dpStat;
	}
};


