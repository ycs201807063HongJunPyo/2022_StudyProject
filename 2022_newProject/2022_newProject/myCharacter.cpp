#include "myCharacter.h"


MyCharacter::MyCharacter(){
	maxHealth = 80;
	currentHealth = maxHealth;
	damage = 4;
	defence = 1;
	fastAttack = 15;
	critical = 10;
}

int MyCharacter::DefaultAttack(int number, int playerDamage) {

	int sumDamage = number + playerDamage;
	return sumDamage;
}

int MyCharacter::ImHit(int attackDamage) {
	int current = currentHealth;
	if (attackDamage >= defence) {
		current = currentHealth - (attackDamage - defence);
	}
	return current;
}

int MyCharacter::ImRealHit(int attackDamage) {
	int current = currentHealth;
	current = currentHealth - (attackDamage);
	return current;
}

void MyCharacter::Reset() {
	maxHealth = 80;
	currentHealth = maxHealth;
	damage = 4;
	defence = 1;
	fastAttack = 15;
	critical = 10;
}