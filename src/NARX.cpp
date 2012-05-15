// NARX.cpp : Defines the entry point for the console application.
//


#include "Unit.h"
#include "Activation_functions.h"
#include "InputUnit.h"
#include "OutputUnit.h"
#include "stdlib.h"
#include "time.h"
#include "NARX.h"



#include "narx_util.h"

extern float *series;
extern int series_len;
extern int epochs;
extern float **exogenous_series;
extern int *used_exogenous;

QString narx_log_str;


NARX::NARX(ARCH arch, int H, int a, int b, int M)
{

	this->H = H;
	this->a = a;
	this->b = b;
	this->arch = arch;
	this->M = M;

	ee = new EvaluationEngine(MAX_SERIES_LEN);

	hunits = new Unit*[H];
	inputs = new InputUnit*[b];
	for(int i=0;i<H;i++) 
	{
		hunits[i] = new Unit();
		hunits[i]->set_activation_func(Activation_functions::sigmoid);
		hunits[i]->set_activation_func_derv(Activation_functions::sigmoid_derv);
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
	output_unit = new OutputUnit();
	//output_unit->setTarget(series[series_index+1]);
	output_unit->set_activation_func(Activation_functions::identity);
	output_unit->set_activation_func_derv(Activation_functions::identity_derv);
	
	for(int i=0;i<H;i++)
		output_unit->add_input_unit(hunits[i]);

	
	
		
}

ARCH NARX::getArch()
{
	return arch;
}

void NARX::train(int epochs)
{
	for(int i=0;i<epochs - 1;i++) {
		
		trainEpoch();
		emit training_epoch_finished();
	}
	trainEpoch(false);
	emit training_epoch_finished();
}

void NARX::trainEpoch(bool logging)
{
//	int ;
	ee->reset();

	QString str;
	int feedback_index = 0;
	
	for(int series_index = a; series_index < series_len - 1; series_index ++)
	{

	for(int i=0;i<((b>a)?a:b);i++) 
	{
		inputs[i]->set_input(series[series_index - i ]);
	}
	
	
		//exogenous[0]->set_input(series[series_index]);

	for(int i=0;i<M;i++)
	{
		if(used_exogenous[i]) {
			for(int j=0;j<a+1;j++) 
				exogenous[j+i*a]->set_input(exogenous_series[i][series_index - j]);
			//_log(QString("ok %1").arg(exogenous_series[i][series_index]));
		}
	}

	
	output_unit->setTarget(series[series_index + 1]);

	ee->insertvalue(series[series_index + 1], output_unit->get_output());
	
	//QString str = QString("input target:index %1 : %2, output narx: %3").arg(series_index + 1).arg(series[series_index+1]).arg(output_unit->get_output());
	//output_unit
	//a += ;
	output_unit->adjust_weights();
	double delta = output_unit->get_delta();
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

	str = QString("F1 = %1; F2 = %2; F3 = %3; F4 = %4 KS1= %5; KS2=%6; KS12=%7; DA = %8")
		.arg(ee->F1()).arg(ee->F2()).arg(ee->F3()).arg(ee->F4())
		.arg(ee->KS1())
		.arg(ee->KS2()).arg(ee->KS12()).arg(ee->DA());
	_log(str);
	
}


NARX::~NARX(void)
{

	delete output_unit;
	for(int i=0;i<H;i++) delete hunits[i];
	delete [] hunits;
	for(int i=0;i<a;i++) delete inputs[i];
	delete [] inputs;
	for(int i=0;i<b;i++) delete feedbacks[i];
	delete [] feedbacks;

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