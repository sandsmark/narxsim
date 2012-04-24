#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h>
#include "narx2.h"
#include <QtGui/QApplication>

#include "Unit.h"
#include "Activation_functions.h"
#include "InputUnit.h"
#include "OutputUnit.h"
#include "stdlib.h"
#include "time.h"
#include "NARX.h"

//#include <QtGui/QApplication>

NARX2 *w;

NARX *mynarx = NULL;


void train_progress_inc()
{
	w->ui.progressbar_train->setValue(w->ui.progressbar_train->value() + 1);
}

void LOG(QString text)
{ 
	 
	 w->ui.text_log->appendPlainText(text); 
}


int series_generated = 0;

float series_start, series_end;
int series_len;
int series_func;
int series_noise;
float *series = 0;

int epochs = 100;





int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	NARX2 w;
	//w.ui.Frame_generate->setHidden(true);
	w.show();
	::w=&w;

	srand(time(0));
	
	/* QtCore.QObject.connect(self.radioButton1,QtCore.SIGNAL("toggled(bool)"),self.radio_activateInput) */
	w.ui.tabWidget->setCurrentIndex(0);
	w.ui.frame_1post->setHidden(true);
	//w.ui.RadioButton_generate_series->connect(w.ui, QtCore::SIGNAL("toggled(bool)"),
	QObject::connect ( w.ui.actionAbout, SIGNAL( triggered() ), &w, SLOT( Menu_about() ) );

	QObject::connect ( w.ui.RadioButton_generate_series, SIGNAL( clicked() ), &w, SLOT( RadioButton_generate_series() ) );
	QObject::connect ( w.ui.RadioButton_load_series, SIGNAL( clicked() ), &w, SLOT( RadioButton_load_series() ) );
	QObject::connect ( w.ui.Button_tab12, SIGNAL( clicked() ), &w, SLOT( Button_12() ) );
	QObject::connect ( w.ui.Button_tab21, SIGNAL( clicked() ), &w, SLOT( Button_21() ) );
	QObject::connect ( w.ui.Button_tab23, SIGNAL( clicked() ), &w, SLOT( Button_23() ) );
	QObject::connect ( w.ui.Button_tab34, SIGNAL( clicked() ), &w, SLOT( Button_34() ) );
	QObject::connect ( w.ui.Button_tab32, SIGNAL( clicked() ), &w, SLOT( Button_32() ) );
	QObject::connect ( w.ui.Button_tab43, SIGNAL( clicked() ), &w, SLOT( Button_43() ) );
	QObject::connect ( w.ui.Button_tab45, SIGNAL( clicked() ), &w, SLOT( Button_45() ) );
	QObject::connect ( w.ui.Button_start_training, SIGNAL( clicked() ), &w, SLOT( Button_start_train() ) );


	


	w.ui.tabWidget->setTabEnabled(1, false);
	w.ui.tabWidget->setTabEnabled(2, false);
	w.ui.tabWidget->setTabEnabled(3, false);
	w.ui.tabWidget->setTabEnabled(4, false);
	
	return a.exec();
}
