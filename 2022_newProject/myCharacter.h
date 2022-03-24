#pragma once

#include<iostream>

using namespace std;

class MyCharacter {
private:
	int maxHealth;
	int currentHealth;
	int maxMana;
	int currentMana;

	int damage;
	int defence;
	int fastAttack;
	int critical;

public:
	MyCharacter();
	int DefaultAttack(int number, int damage);
	int ImHit(int attackDamage);
	int ImRealHit(int attackDamage);
	void Reset();

	int getHealth() {
		return maxHealth;
	}
	int getCurrentHealth() {
		return currentHealth;
	}
	int getMana() {
		return maxMana;
	}
	int getCurrentMana() {
		return currentMana;
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
	int getCritical() {
		return critical;
	}

	void setMaxHealth(int current) {
		maxHealth = current;
	}
	void setCurrentHealth(int current) {
		currentHealth = current;
	}
	void setMaxMana(int current) {
		maxMana = current;
	}
	void setCurrentMana(int current) {
		currentMana = current;
	}

	//포인트 찍었을때
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
	void setMana(int dpStat) {
		maxMana += dpStat;
		currentMana += dpStat;
	}
	void setFastAttack(int dpStat) {
		fastAttack += dpStat;
	}
	void setCritical(int dpStat) {
		critical += dpStat;
	}
};


