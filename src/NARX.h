#pragma once
#include <qthread.h>
#include "Unit.h"
#include "InputUnit.h"
#include "OutputUnit.h"

class NARX : public QThread
{
	Q_OBJECT

protected:
	
	int H;
	int a;
	int b;

	Unit **hunits;
	OutputUnit *output_unit;
	InputUnit **inputs;

	void trainEpoch(bool logging = false);
	void _log(QString str);

	void run();

public:
	NARX(void);
	~NARX(void);

	void train(int epochs);

signals:
	void training_epoch_finished();
	void log();
};

