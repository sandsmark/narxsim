#include "Unit.h"
#include "narx_util.h"
#include "stdlib.h"


double Unit::alfa = 0.1;

Unit::Unit(void)
{
	input_area = new Unit * [MAX_INPUTS_PER_UNIT];
	input_count = 0;
	input_weights = new double[MAX_INPUTS_PER_UNIT];

	bias = (double) (rand() % 100) / 100;

	output = 0;
}


Unit::~Unit(void)
{
	delete [] input_area;
	delete [] input_weights;
}


void Unit::set_activation_func( double (*f) (double arg))
{
	activation_func = f;
}

void Unit::set_activation_func_derv( double (*f) (double arg))
{
	activation_func_derv = f;
}

int Unit::add_input_unit (Unit *unit)
{
	if (input_count >= MAX_INPUTS_PER_UNIT) return -1;

	input_weights[input_count] = (double) (rand() % 100) / 100;
	input_area[input_count] = unit;
	
	//printf("%f\n", input_weights[input_count]);
	return input_count++;
}

double Unit::pre_output()
{
	double preoutput = 0;
	for (int i=0; i < input_count; i++)
		preoutput += input_area[i]->get_output() * input_weights[i];

	preoutput += bias;
	return preoutput;
}

void Unit::compute_output()
{
	output = activation_func(pre_output());
}

double Unit::get_output()
{
	compute_output();
	return output;
}

void Unit::adjust_weights()
{

}

void Unit::adjust_weights(double superior_layer_delta)
{
	
	double deltah =  activation_func_derv(pre_output()) * superior_layer_delta * get_output();
	
	for(int i = 0; i < input_count;i ++)
	{
		input_weights[i] += Unit::alfa * deltah * input_area[i]->get_output();
	}
}