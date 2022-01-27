#include "Engraving.h"

Engraving::Engraving() {
	strPoint = 0;
	dpPoint = 0;
	hpPoint = 0;
	fastPoint = 0;
	for (int i = 0; i <= 3; i++) {
		costArray[i] = 10;
	}
}

void Engraving::UpStrPoint() {
	costArray[0] += 5;
	strPoint++;
}

void Engraving::UpDpPoint() {
	costArray[1] += 5;
	dpPoint++;
}

void Engraving::UpHpPoint() {
	costArray[2] += 5;
	hpPoint++;
}

void Engraving::UpFastPoint() {
	costArray[3] += 10;
	fastPoint++;
}

void Engraving::Reset() {
	strPoint = 0;
	dpPoint = 0;
	hpPoint = 0;
	fastPoint = 0;
	for (int i = 0; i <= 3; i++) {
		costArray[i] = 10;
	}
}