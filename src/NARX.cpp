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


#include "Unit.h"
#include "Activation_functions.h"
#include "InputUnit.h"
#include "OutputUnit.h"
#include "stdlib.h"
#include "time.h"
#include "NARX.h"



#include "narx_util.h"

extern double **series;
extern double **Nseries;
extern int series_len;
extern int epochs;
extern double **exogenous_series;
extern int *used_exogenous;

QString narx_log_str;


NARX::NARX(ARCH arch, int H, int hact, int a, int b, int M, int N)
{

	this->H = H;
	this->a = a;
	this->b = b;
	this->arch = arch;
	this->M = M;
	this->N = N;

	ee = new EvaluationEngine(series_len);
	rw = new EvaluationEngine(series_len);

	hunits = new Unit*[H];
	inputs = new InputUnit*[b];
	for(int i=0;i<H;i++) 
	{
		hunits[i] = new Unit();
		if(hact == 2)
		{
			hunits[i]->set_activation_func(Activation_functions::aslog);
		    hunits[i]->set_activation_func_derv(Activation_functions::aslog_derv);
		}
		else if (hact == 1)
		{
			hunits[i]->set_activation_func(Activation_functions::Bsigmoid);
		    hunits[i]->set_activation_func_derv(Activation_functions::Bsigmoid_derv);
		}
		else if (hact == 0)
		{
			hunits[i]->set_activation_func(Activation_functions::sigmoid);
			hunits[i]->set_activation_func_derv(Activation_functions::sigmoid_derv);
		}
		
		
	}

	for(int i=0;i<b;i++)
	{
		inputs[i] = new InputUnit();
		inputs[i]->set_input(0);
	}
	for(int i=0;i<H;i++)
		for(int j=0;j<b;j++)
			hunits[i]->add_input_unit(inputs[j]);

	feedbacks = new InputUnit*[b];

	exogenous = new InputUnit*[M*(a+1)];
	for(int i=0;i<M*(a+1);i++)
	{
		exogenous[i] = new InputUnit();
		exogenous[i] ->set_input(0);
		

	}
	for(int i=0;i<H;i++)
		for(int j=0;j<M*(a+1);j++)
	hunits[i]->add_input_unit(exogenous[j]);

	

	//printf("%d\n", index);
	output_units = new OutputUnit*[N];
	for(int i=0;i<N;i++)
	{
		output_units[i]=new OutputUnit();
	
		output_units[i]->set_activation_func(Activation_functions::identity);
		output_units[i]->set_activation_func_derv(Activation_functions::identity_derv);
	}
	
	for(int i=0;i<H;i++)
		for(int j=0;j<N;j++)
		output_units[j]->add_input_unit(hunits[i]);

	
	//hunits[0]->
		
}

ARCH NARX::getArch()
{
	return arch;
}

void NARX::train(int epochs)
{
	for(int i=0;i<epochs;i++) {
		
		trainEpoch(false, i);
		emit training_epoch_finished();
	}
	//trainEpoch(false, epochs);
	//emit training_epoch_finished();
}

void NARX::trainEpoch(bool logging, int epo)
{
//	int ;
	ee->reset();
	rw->reset();

	

	QString str;
	int feedback_index = 0;
	
	for(int series_index = a; series_index < series_len ; series_index ++)
	{

	for(int i=1;i<=b;i++) 
	{
		
		inputs[i - 1]->set_input(Nseries[0][ series_index - i - a + b  ]);
	}
	
	
		//exogenous[0]->set_input(series[series_index]);

	for(int i=0;i<M;i++)
	{
		if(used_exogenous[i]) {
			for(int j=0;j<a+1;j++) 
				exogenous[j+i*a]->set_input(exogenous_series[i][series_index - j]);
			//_log(QString("ok %1").arg(exogenous_series[i][series_index]));
			//FLOG(QString("ok exo=%1\n").arg(exogenous_series[i][series_index]).toStdString().c_str());
		}
	}

	for(int i=0;i<N;i++)
		output_units[i]->setTarget(Nseries[i][series_index]);

	ee->insertvalue(Nseries[0][series_index], output_units[0]->get_output());
	rw->insertvalue(Nseries[0][series_index], Nseries[0][series_index>0 ? series_index - 1 : 0]);
	
	//FLOG(QString("input target:index %1 : %2, output narx: %3\n").arg(series_index).arg(series[series_index]).arg(output_unit->get_output()).toStdString().c_str());
	//output_unit
	//a += ;
	output_units[0]->adjust_weights();
	double delta = output_units[0]->get_delta();
	//FLOG(QString("ok delta=%1\n").arg(delta).toStdString().c_str());
	for(int i=0;i<H;i++) 
	{
		hunits[i]->adjust_weights(delta);
	}

	/*if (!feedback_index) // we must connect the feedback now
	{
		for(int i=0;i<H;i++) 
		{
			feedbacks[i]->set_input(output_unit->get_output());
			hunits[i]->add_input_unit(feedback);
		}
	}*/


	if(logging)
		_log(str);

	}
	if(logging)
		_log("Epoch finished :)");

	str = QString("epoch %1: F1 = %2; F2 = %3; F3 = %4; F4 = %5; KS1= %6; KS2=%7; KS12=%8; DA = %9"
		"; F1RW=%10; F2RW=%11; F3RW=%12; F4RW=%13; KS1=%14; KS2=%15; KS12=%16; DA=%17").arg(epo)
		.arg(ee->F1()).arg(ee->F2()).arg(ee->F3()).arg(ee->F4())
		.arg(ee->KS1())
		.arg(ee->KS2()).arg(ee->KS12()).arg(ee->DA())
		.arg(rw->F1()).arg(rw->F2()).arg(rw->F3()).arg(rw->F4())
		.arg(rw->KS1()).arg(rw->KS2()).arg(rw->KS12()).arg(rw->DA());
	_log(str);
	
}


NARX::~NARX(void)
{
	for(int i=0;i<N;i++) delete output_units[i];
	delete []output_units;
	for(int i=0;i<H;i++) delete hunits[i];
	delete [] hunits;
	for(int i=0;i<b;i++) delete inputs[i];
	delete [] inputs;
	for(int i=0;i<b;i++) delete feedbacks[i];
	delete [] feedbacks;
	for(int i=0;i<M*(a+1);i++) delete exogenous[i];
	delete [] exogenous;

	delete ee;

}

void NARX::run()
{
	train(epochs);
}

void NARX::_log(QString str)
{
	narx_log_str = str;
	emit log();
}