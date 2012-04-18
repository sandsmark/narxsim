#include "OutputUnit.h"


OutputUnit::OutputUnit(void)
{

	target = 0;
}


OutputUnit::~OutputUnit(void)
{
}


void OutputUnit::setTarget(double target)
{
	this->target=target;
}

double OutputUnit::error()
{
	return target - get_output();
}

void OutputUnit::adjust_weights()
{
	deltao = activation_func_derv(pre_output()) * error();

	for(int i = 0; i < input_count;i ++)
	{
		input_weights[i] += Unit::alfa * deltao * input_area[i]->get_output();
	}
	
}

double OutputUnit::get_delta()
{
	return deltao;
}