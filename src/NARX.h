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

#pragma once
#include <qthread.h>
#include "Unit.h"
#include "InputUnit.h"
#include "OutputUnit.h"
#include "EvaluationEngine.h"
#include "narx_util.h"

class NARX : public QThread
{
	Q_OBJECT

protected:
	
	int H;
	int a;
	int b;
	int M;
	int N;

	int feedback, targets;

	ARCH arch;


	int hact;

	Unit **hunits;
	OutputUnit **output_units;
	InputUnit **inputs;

	InputUnit **feedbacks;

	InputUnit **exogenous;

	double **Y;

	EvaluationEngine **ee;
	EvaluationEngine **rw;

	void trainEpoch(bool logging, int epo);
	void _log(QString str);

	void push_weights();

	void run();

public:
	NARX(ARCH arch, int H = 1, int hact = 2, int a = 0, int b = 0, int M = 0, int N = 1, int feedback = 0, int targets = 0);
	~NARX(void);

	void train(int epochs);

	ARCH getArch();

	void copy(NARX *n);
	void sum(NARX *n);
	void divide(int len);

signals:
	void training_epoch_finished();
	void log();
};

