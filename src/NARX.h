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

	ARCH arch;

	Unit **hunits;
	OutputUnit *output_unit;
	InputUnit **inputs;

	InputUnit **feedbacks;

	InputUnit **exogenous;

	EvaluationEngine *ee;

	void trainEpoch(bool logging = false);
	void _log(QString str);

	void push_weights();

	void run();

public:
	NARX(ARCH arch, int H = 1, int a = 0, int b = 0, int M = 0);
	~NARX(void);

	void train(int epochs);

	ARCH getArch();

signals:
	void training_epoch_finished();
	void log();
};

