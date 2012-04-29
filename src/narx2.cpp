#include "narx2.h"
#include "Unit.h"
#include "InputUnit.h"
#include "OutputUnit.h"
#include "NARX.h"
#include <QtGui/QMessageBox>
#include <QtCore/qmath.h>
#include "narx_util.h"

extern float series_start, series_end;
extern int series_len;
extern int series_func;
extern int series_noise;
extern int series_generated;
extern float *series;
extern int epochs;
extern void train_progress_inc();

extern NARX *mynarx;

int narx_stage1_5 = 0;
//initialize NARX:
ARCH arch = MLP;

NARX2::NARX2(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	ui.Combo_base_function->addItem( "sin");
	ui.Combo_base_function->addItem( "log");
	ui.Combo_base_function->addItem( "exp");
	ui.Combo_base_function->addItem( "sqr");
	ui.Combo_base_function->addItem( "hip");
	ui.Combo_base_function->addItem( "lin");

}

NARX2::~NARX2()
{

}

void NARX2::train_progress_inc()
{
	::train_progress_inc();
}
void NARX2::log()
{
	extern QString narx_log_str;
	LOG(narx_log_str);
}


void NARX2::Menu_about() 
{
    QMessageBox::information(this, "About", "Artificial Intelligence Master Project\neng. Eugen Hristev 2012.\n"
		"Thanks to Dr. Rene Alquezar Mancho for coordinating my project." );
}

void NARX2::Button_start_train()
{
	mynarx->start();
	ui.Button_start_training->setEnabled(false);
}

void NARX2::RadioButton_generate_series() 
{
//QMessageBox::information( this, "Information", "Just clicked Ui PushButton" ); // #include <QtGui/QMessageBox>
ui.Frame_generate->setHidden(false);
}

void NARX2::RadioButton_load_series() 
{
//QMessageBox::information( this, "Information", "Just clicked LOAD" ); // #include <QtGui/QMessageBox>
ui.Frame_generate->setHidden(true);
}

void NARX2::Button_12() 
{
	

	ui.frame1->setEnabled(false);
	ui.Frame_generate->setEnabled(false);
	ui.frame_1post->setHidden(false);

	if(series_generated) goto exit1;

	series_start = ui.lineedit_series_start->text().toFloat();
	series_end = ui.lineedit_series_end->text().toFloat();
	series_len = ui.spinbox_series_len->value();
    series_func  = ui.Combo_base_function->currentIndex();
	series_noise = ui.spinbox_noise->value();

	//switch(series_f)
	series = new float[series_len];

	float step = ( series_end - series_start ) / series_len;

	float cur = series_start;

	int i;

	for(i=1; i <= series_len; i++)
	{
		ui.table_series->insertRow(i - 1);
		QTableWidgetItem *newItem1 = new QTableWidgetItem(tr("%1").arg(cur));
		ui. table_series->setItem(i - 1, 0, newItem1);

		float val;

		switch (series_func)
		{
		case 0:
			val = qSin(cur);
			break;
		case 1:
			val = qLn(cur);
			break;
		case 2:
			val = qExp(cur);
			break;
		case 3:
			val = cur * cur;
			break;
		case 4:
			val = 1 / cur;
			break;
		case 5:
			val = cur;
			break;
		default:
			val = 1.0;
		};

		if(series_noise)
			val += val * ((rand() % (2*series_noise) - series_noise)) / 100 ;

		QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("%1").arg(val));
		
		ui. table_series->setItem(i - 1, 1, newItem2);
		
		series [i - 1] = val;
		cur+=step;
	}
	//ui.table_series->removeRow(i);

	QMessageBox::information( this, "Proceed", "The series has been generated" );

	

	
exit1:
	//if(series_generated)
	ui.tabWidget->setCurrentIndex(1);
    ui.tabWidget->setTabEnabled(1, true);
	series_generated = 1;
    //ui.tabWidget->setTabEnabled(0, false);

	

}

void NARX2::Button_21() 
{
	//
	ui.tabWidget->setCurrentIndex(0);
}

void NARX2::Button_23()
{
	ui.tabWidget->setTabEnabled(2, true);
	ui.tabWidget->setCurrentIndex(2);
}

void NARX2::Button_32()
{
	ui.tabWidget->setTabEnabled(1, true);
	ui.tabWidget->setCurrentIndex(1);
}

void NARX2::Button_34()
{
	

	//
	Unit::alfa = ui.lineedit_learningrate->text().toFloat();
	epochs = ui.lineedit_epochs->text().toFloat();
	ui.progressbar_train->setMaximum(epochs);

	

	/* main NARX code */
	if(!narx_stage1_5)
	{
		
		
		if (!ui.check_del_targets->isChecked() && !ui.check_del_outputs->isChecked() && !ui.check_exogenous->isChecked())
		{
			QMessageBox::information( this, "NARX", "Selected architecture: MLP" );
			arch = MLP;
		}
		if (ui.check_del_targets->isChecked() && !ui.check_del_outputs->isChecked() && !ui.check_exogenous->isChecked())
		{
			QMessageBox::information( this, "NARX", "Selected architecture: NAR-D" );
			arch = NAR_D;
		}

		narx_stage1_5 = 1;
		
	
	} 
	/* end of main NARX code */

	if (!ui.check_del_targets->isChecked())
		ui.spinbox_dregressor->setEnabled(false);
	if (!ui.check_del_outputs->isChecked())
		ui.spinbox_yregressor->setEnabled(false);
	if (!ui.check_exogenous->isChecked())
		ui.spinbox_xregressor->setEnabled(false);

	ui.tabWidget->setTabEnabled(3, true);
	ui.tabWidget->setCurrentIndex(3);
	ui.check_del_targets->setEnabled(false);
	ui.lineedit_epochs->setEnabled(false);
	ui.lineedit_learningrate->setEnabled(false);
	ui.check_exogenous->setEnabled(false);
	ui.check_del_outputs->setEnabled(false);
	ui.spinbox_hidden_units->setEnabled(false);
}
void NARX2::Button_43()
{
	ui.tabWidget->setTabEnabled(2, true);
	ui.tabWidget->setCurrentIndex(2);
}
void NARX2::Button_45()
{
	mynarx = new NARX(arch, ui.spinbox_hidden_units->value(), 0, ui.spinbox_dregressor->value());

	QObject::connect ( mynarx, SIGNAL( training_epoch_finished() ), this, SLOT( train_progress_inc() ) );
	QObject::connect ( mynarx, SIGNAL( log() ), this, SLOT( log() ), Qt::BlockingQueuedConnection );

	ui.tabWidget->setTabEnabled(4, true);
	ui.tabWidget->setCurrentIndex(4);
}