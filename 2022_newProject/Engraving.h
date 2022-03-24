#pragma once

class Engraving
{
public:
	int strPoint;
	int dpPoint;
	int hpPoint;
	int fastPoint;
	int criPoint;
	int manaPoint;
	int costArray[6];

public:
	Engraving();
	void UpStrPoint();
	void UpDpPoint();
	void UpHpPoint();
	void UpFastPoint();
	void UpCriPoint();
	void UpManaPoint();
	void Reset();

};

