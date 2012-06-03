/*
Copyright 2012 Eugen Hristev Licensed under the
Educational Community License, Version 2.0 (the "License"); you may
not use this file except in compliance with the License. You may
obtain a copy of the License at

http://www.osedu.org/licenses/ECL-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an "AS IS"
BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
or implied. See the License for the specific language governing
permissions and limitations under the License.
*/

#include "OutputUnit.h"
#include "Unit.h"
#include "assert.h"
#include "narx_util.h"


OutputUnit::OutputUnit(void):Unit()
{

	target = 0;
	old_weights = new double[MAX_INPUTS_PER_UNIT];
	for(int i =0; i < MAX_INPUTS_PER_UNIT;i ++)
		old_weights[i] = input_weights[i];
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
	//deltao = activation_func_derv(pre_output()) * error();

	for(int i = 0; i < input_count;i ++)
	{
		input_weights[i] += Unit::alfa * deltao * input_area[i]->get_output();
		
	}
	
}

void OutputUnit::compute_delta()
{
	deltao = activation_func_derv(pre_output()) * error();
}

double OutputUnit::get_delta(Unit *u)
{

	for (int i = 0;i<input_count;i++)
		if (u == input_area[i])
	      return deltao * old_weights[i];

	assert(false);
	return 0;
}

void OutputUnit::fix_weights()
{
	for(int i = 0; i < input_count;i ++)
	old_weights[i] = input_weights[i];
}