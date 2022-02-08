#pragma once

class Engraving
{
public:
	int strPoint;
	int dpPoint;
	int hpPoint;
	int fastPoint;
	int criPoint;
	int costArray[5];

public:
	Engraving();
	void UpStrPoint();
	void UpDpPoint();
	void UpHpPoint();
	void UpFastPoint();
	void UpCriPoint();
	void Reset();

};

