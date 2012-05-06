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

QString narx_log_str;


NARX::NARX(ARCH arch, int H, int a, int b)
{

	this->H = H;
	this->a = a;
	this->b = b;
	this->arch = arch;

	ee = new EvaluationEngine(MAX_SERIES_LEN);

	hunits = new Unit*[H];
	inputs = new InputUnit*[a+1];
	for(int i=0;i<H;i++) 
	{
		hunits[i] = new Unit();
		hunits[i]->set_activation_func(Activation_functions::sigmoid);
		hunits[i]->set_activation_func_derv(Activation_functions::sigmoid_derv);
	}

	for(int i=0;i<a+1;i++)
	{
		inputs[i] = new InputUnit();
	}

	for(int i=0;i<H;i++)
		for(int j=0;j<a+1;j++)
			hunits[i]->add_input_unit(inputs[j]);

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


	for(int series_index = a; series_index < series_len - 1; series_index ++)
	{
	for(int i=0;i<a+1;i++) 
	{
		inputs[i]->set_input(series[series_index - i]);
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

	if(logging)
		_log(str);

	}
	if(logging)
		_log("Epoch finished :)");

	str = QString("F3 = %1, KS1= %2, KS2=%3, KS12=%4, DA = %5").arg(ee->F3()).arg(ee->KS1())
		.arg(ee->KS2()).arg(ee->KS12()).arg(ee->DA());
	_log(str);
	
}


NARX::~NARX(void)
{

	delete output_unit;
	for(int i=0;i<H;i++) delete hunits[i];
	delete [] hunits;
	for(int i=0;i<H;i++) delete inputs[i];
	delete [] inputs;

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