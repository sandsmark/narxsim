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
#include "FeedbackInfo.h"



#include "narx_util.h"

extern double **series;
extern double **Nseries;
extern int series_len;
extern int epochs;
extern double **exogenous_series;
extern int *used_exogenous;

QString narx_log_str;




NARX::NARX(ARCH arch, int H, int hact, int a, int b, int M, int N, int feedback, int targets)
{

	this->H = H;
	this->a = a;
	this->b = b;
	this->arch = arch;
	this->M = M;
	this->N = N;
	this->feedback=feedback;
	this->hact=hact;
	this->targets=targets;
	

	ee = new EvaluationEngine*[N];
	rw = new EvaluationEngine*[N];
	for(int i=0;i<N;i++) 
	{
		ee[i] = new EvaluationEngine(series_len);
		rw[i] = new EvaluationEngine(series_len);
	}

	hunits = new Unit*[H];

	

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

	if(targets) {

	inputs = new InputUnit*[b*N];

	for(int i=0;i<b*N;i++)
	{
		inputs[i] = new InputUnit();
		inputs[i]->set_input(0);
	}
	for(int i=0;i<H;i++)
		for(int j=0;j<b*N;j++)
			hunits[i]->add_input_unit(inputs[j]);

	}

	if(feedback) {
	feedbacks = new InputUnit*[N*b];

	for(int i = 0; i < N*b; i++)
	{
		feedbacks[i] = new InputUnit();
		feedbacks[i] -> set_input(0);
	}

	for(int i=0;i<H;i++)
		for(int j=0;j<N*b;j++)
		{
			hunits[i]->add_input_unit(feedbacks[j]);
		}

	}


	exogenous = new InputUnit*[M*(a+1)];
	for(int i=0;i<M*(a+1);i++)
	{
		exogenous[i] = new InputUnit();
		exogenous[i] ->set_input(0);
		

	}
	for(int i=0;i<H;i++)
		for(int j=0;j<M*(a+1);j++)
		{
			hunits[i]->add_input_unit(exogenous[j]);
		}

	

	

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
	FLOG(QString("NARX: arch=%1, H=%2, hact =%3, a=%4, b=%5, M=%6, N=%7, feedback=%8, targets= %9\n")
		.arg(arch).arg(H).arg(hact).arg(a).arg(b).arg(M).arg(N).arg(feedback).arg(targets)
		.toStdString().c_str());
		
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
	for(int i=0;i<N;i++)
	{
		ee[i]->reset();
		rw[i]->reset();
	}

	QString str;


	if(!feedback) 
	{

	
	int feedback_index = 0;
	
	for(int series_index = a; series_index < series_len ; series_index ++)
	{

	for(int j=0;j<N;j++)
		for(int i=1;i<=b;i++) 
		{
		
			inputs[j*series_len + i - 1]->set_input(Nseries[j][ series_index - i - a + b  ]);
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
	{
		output_units[i]->setTarget(Nseries[i][series_index]);

		ee[i]->insertvalue(Nseries[i][series_index], output_units[i]->get_output());
		rw[i]->insertvalue(Nseries[i][series_index], Nseries[i][series_index>0 ? series_index - 1 : 0]);
	}
	
	//FLOG(QString("input target:index %1 : %2, output narx: %3\n").arg(series_index).arg(series[series_index]).arg(output_unit->get_output()).toStdString().c_str());
	//output_unit
	//a += ;
	for(int i=0;i<N;i++) 
	{
		output_units[i]->fix_weights();
		output_units[i]->compute_delta();
		output_units[i]->adjust_weights();
	}
	
	//FLOG(QString("ok delta=%1\n").arg(delta).toStdString().c_str());
	for(int i=0;i<H;i++) 
	{
		double delta =0;
		for(int j=0;j<N;j++)
			delta+= output_units[j]->get_delta(hunits[i]);

		hunits[i]->adjust_weights(delta);
	}
	
	}

	
	
	}

	else {
		//_log("feedback");
	
		NARX *copynarx= new NARX(arch, H, hact, a, b , M, N, feedback, targets); /* tk targets */

		

		NARX *originalnarx= new NARX(arch, H, hact, a, b , M, N, feedback, targets); /* tk targets */

		originalnarx->copy(this);
		;
		/* must do the feedback arch now */
	/*	int weight_count = output_units[0]->inputcount();
		double *final_weights= new double[weight_count];
		for(int i=0;i<weight_count;i++)
			final_weights[i] = 0;
			*/

		Y = new double*[N];
		for(int i=0;i<N;i++)
			Y[i] =  new double[series_len];

		FeedbackInfo **fi;

		fi = new FeedbackInfo*[series_len];
		for(int i=0;i<series_len;i++)
			fi[i] = new FeedbackInfo(M*(a+1), N*b, N*b);

		int t=0;

		for(;t<series_len;t++)
		{

			
		/* if (exogenous) */
		for (int i=0;i<M;i++)
		{
			for (int j = a; j >= 0 ; j++)
			{
				if(t>=j)
					exogenous[i*M + j]->set_input(exogenous_series[i][t-j]);
				else
					exogenous[i*M + j]->set_input(0);

				fi[t]->X[i*M+j] = exogenous[i*M + j]->get_input();
			}
		}

if(targets) 
		for (int i=0;i<N;i++)
		{
			for (int j = b; j > 0; j--)
			{
				if(t>=j)
					inputs[i*N + j - 1]->set_input(Nseries[i][t-j]);
				else
					inputs[i*N + j - 1]->set_input(0);

				fi[t]->D[i*N+j - 1] = inputs[i*N + j - 1]->get_input();
			}
		}

if (feedback) 
		for (int i=0;i<N;i++)
		{
			for (int j = b; j > 0; j--)
			{

				//FLOG(QString("testing%1\n").arg(fi[t]->Y[i*N+j]).toStdString().c_str());

				if(t>=j)
					feedbacks[i*N + j - 1]->set_input(Y[i][t-j]);
				else
					feedbacks[i*N + j - 1]->set_input(0);

				//FLOG(QString("testing%1\n").arg(fi[t]->Y[i*N+j]).toStdString().c_str());

				fi[t]->Y[i*N+j - 1] = feedbacks[i*N + j - 1]->get_input();
			}
		}


		for(int i=0;i<N;i++)
		{
			output_units[i]->setTarget(Nseries[i][t]);
			Y[i][t] = output_units[i]->get_output();

			//FLOG(QString("testing%1\n").arg(Y[i][t]).toStdString().c_str());

			ee[i]->insertvalue(Nseries[i][t], Y[i][t]);
			rw[i]->insertvalue(Nseries[i][t], Nseries[i][t>0 ? t - 1 : 0]);		
		}

		}

		/* backpropagating the errors */
		for (t=series_len-1;t>=0;t--)
		{
			this->copy(originalnarx);

			/* if (exogenous) */
		for (int i=0;i<M;i++)
		{
			for (int j = a; j >= 0 ; j++)
			{
					exogenous[i*M + j]->set_input(fi[t]->X[i*M+j]);
			}
		}

if(targets) 
		for (int i=0;i<N;i++)
		{
			for (int j = b; j > 0; j--)
			{
					inputs[i*N + j - 1]->set_input(fi[t]->D[i*N+j - 1]);
			}
		}

if (feedback) 
		for (int i=0;i<N;i++)
		{
			for (int j = b; j > 0; j--)
			{
					feedbacks[i*N + j - 1]->set_input(fi[t]->Y[i*N+j - 1]);
			}
		}


			for(int i=0;i<N;i++) 
			{
				output_units[i]->setTarget(Nseries[i][t]);
				output_units[i]->fix_weights();
				output_units[i]->compute_delta();
				output_units[i]->adjust_weights();
			}

			for(int i=0;i<H;i++) 
			{
				double delta = 0;
				for(int j=0;j<N;j++)
					delta+= output_units[j]->get_delta(hunits[i]);

				hunits[i]->adjust_weights(delta);
			}

			if(t==series_len - 1)
				copynarx->copy(this);
			else
				copynarx->sum(this);

		}

		copynarx->divide(series_len);
		this->copy(copynarx);
		originalnarx->copy(this);
		
		for(int i=0;i<N;i++) delete Y[i];
		delete []Y;
		for(int i=0;i<series_len;i++) delete fi[i];
		delete []fi;


	}

	if(logging)
		_log(str);

	




	if(logging)
		_log("Epoch finished :)");
	for(int i=0;i<N;i++)
	{
	str = QString("target %1:epoch %2: F1 = %3; F2 = %4; F3 = %5; F4 = %6; KS1= %7; KS2=%8; KS12=%9; DA = %10"
		"; F1RW=%11; F2RW=%12; F3RW=%13; F4RW=%14; KS1=%15; KS2=%16; KS12=%17; DA=%18")
		.arg(i)
		.arg(epo)
		.arg(ee[i]->F1()).arg(ee[i]->F2()).arg(ee[i]->F3()).arg(ee[i]->F4())
		.arg(ee[i]->KS1())
		.arg(ee[i]->KS2()).arg(ee[i]->KS12()).arg(ee[i]->DA())
		.arg(rw[i]->F1()).arg(rw[i]->F2()).arg(rw[i]->F3()).arg(rw[i]->F4())
		.arg(rw[i]->KS1()).arg(rw[i]->KS2()).arg(rw[i]->KS12()).arg(rw[i]->DA());
	_log(str);
	}
	
}


NARX::~NARX(void)
{
	for(int i=0;i<N;i++) delete output_units[i];
	delete []output_units;
	for(int i=0;i<H;i++) delete hunits[i];
	delete [] hunits;
	if(targets)
	for(int i=0;i<b*N;i++) delete inputs[i];
	delete [] inputs;
	if(feedbacks)
	for(int i=0;i<N*b;i++) delete feedbacks[i];
	delete [] feedbacks;
	for(int i=0;i<M*(a+1);i++) delete exogenous[i];
	delete [] exogenous;

	delete ee;
	delete rw;

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

void NARX::copy(NARX *n)
{
	for (int i=0;i<M*(a+1);i++)
		exogenous[i]->copy(n->exogenous[i]);
	if(targets)
	for (int i=0;i<N*b;i++)
		inputs[i]->copy(n->inputs[i]);
	if(feedback)
	for (int i=0;i<N*b;i++)
		feedbacks[i]->copy(n->feedbacks[i]);
	for(int i=0;i<H;i++)
		hunits[i]->copy(n->hunits[i]);
	for(int i=0;i<N;i++)
		output_units[i]->copy(n->output_units[i]);
}
void NARX::sum(NARX *n)
{
	for (int i=0;i<M*(a+1);i++)
		exogenous[i]->sum(n->exogenous[i]);
	if(targets)
	for (int i=0;i<N*b;i++)
		inputs[i]->sum(n->inputs[i]);
	if(feedback)
	for (int i=0;i<N*b;i++)
		feedbacks[i]->sum(n->feedbacks[i]);
	for(int i=0;i<H;i++)
		hunits[i]->sum(n->hunits[i]);
	for(int i=0;i<N;i++)
		output_units[i]->sum(n->output_units[i]);
}

void NARX::divide(int len)
{
	for (int i=0;i<M*(a+1);i++)
		exogenous[i]->divide(len);
	if(targets)
	for (int i=0;i<N*b;i++)
		inputs[i]->divide(len);
	if(feedback)
	for (int i=0;i<N*b;i++)
		feedbacks[i]->divide(len);
	for(int i=0;i<H;i++)
		hunits[i]->divide(len);
	for(int i=0;i<N;i++)
		output_units[i]->divide(len);
}