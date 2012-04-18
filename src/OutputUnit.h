#pragma once
#include "Unit.h"
class OutputUnit :
	public Unit
{
protected:

	double target;

	double deltao;

public:
	OutputUnit(void);
	~OutputUnit(void);

	void setTarget(double target);
	double error();

	virtual void adjust_weights();

	double get_delta();
};

