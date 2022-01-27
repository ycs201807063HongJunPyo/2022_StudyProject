#pragma once

class Engraving
{
public:
	int strPoint;
	int dpPoint;
	int hpPoint;
	int fastPoint;
	int costArray[4];

public:
	Engraving();
	void UpStrPoint();
	void UpDpPoint();
	void UpHpPoint();
	void UpFastPoint();
	void Reset();

};

