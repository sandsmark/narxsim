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

#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h>
#include "narx2.h"
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

#include "Unit.h"
#include "Activation_functions.h"
#include "InputUnit.h"
#include "OutputUnit.h"
#include "stdlib.h"
#include "time.h"
#include "NARX.h"
#include "qmath.h"

//#include <QtGui/QApplication>

NARX2 *w;
FILE *outfile = 0;
int normalize = 1;


NARX *mynarx = NULL;





void train_progress_inc()
{
	w->ui.progressbar_train->setValue(w->ui.progressbar_train->value() + 1);
	if(w->ui.progressbar_train->value() == w->ui.progressbar_train->maximum())
	{
		 QMessageBox::information(w, "Training complete", "The training is complete."
		);
		 LOG("NARX training complete.");
	}
}

void LOG(QString text)
{ 
	 
	 w->ui.text_log->appendPlainText(text); 
}

void FLOG(const char* text)
{
	fprintf(outfile,text);
	fflush(outfile);
}

void train_result_log(QString text)
{
	w->ui.textedit_training->appendPlainText(text); 
}


int series_generated = 0;

double series_start, series_end;
int series_len;
int series_func;
int series_noise;
double **series = 0;
double **exogenous_series;

double **Nseries = 0;

double *Nvariance ;
double *N_E ;
extern int N;


int *used_exogenous;

int epochs = 100;

int M=0;

void normalize_f()
{
	if(!normalize) {Nseries = series;return;}

	N_E =  new double[N];
	Nvariance = new double[N];

	for(int i=0;i<N;i++)
	{
		N_E[i] = 0;
		Nvariance[i] = 1;
	}
	for(int j=0;j<N;j++)
	{
		for(int i=0;i<series_len;i++) N_E[j]+=series[j][i];
	    N_E[j]/=series_len;
	}
	for(int j=0;j<N;j++)
	{
		for(int i=0;i<series_len;i++) Nvariance[j]+=qPow(series[j][i]-N_E[j], 2);
		Nvariance[j]/=series_len;
	}

	Nseries = new double*[N];
	for (int i=0;i<N;i++)
	{
		Nseries[i]=new double[series_len];
		
	}

	

	for(int j=0;j<N;j++)
	
		for(int i=0;i<series_len;i++) {
			Nseries[j][i] = (series[j][i] - N_E[j])/Nvariance[j];
			FLOG(QString("norm series:%1\n").arg(Nseries[j][i]).toStdString().c_str());
		}
}



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	outfile = fopen("log.txt","wt");

	NARX2 w;
	//w.ui.Frame_generate->setHidden(true);
	w.show();
	::w=&w;

	srand(time(0));
	
	/* QtCore.QObject.connect(self.radioButton1,QtCore.SIGNAL("toggled(bool)"),self.radio_activateInput) */
	w.ui.tabWidget->setCurrentIndex(0);
	w.ui.frame_1post->setHidden(true);
	w.ui.frame_load->setHidden(true);
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
	QObject::connect ( w.ui.Button_tab54, SIGNAL( clicked() ), &w, SLOT( Button_54() ) );
	QObject::connect ( w.ui.Button_tab56, SIGNAL( clicked() ), &w, SLOT( Button_56() ) );
	QObject::connect ( w.ui.Button_start_training, SIGNAL( clicked() ), &w, SLOT( Button_start_train() ) );
	QObject::connect ( w.ui.button_predict, SIGNAL( clicked() ), &w, SLOT( Button_predict() ) );

	QObject::connect ( w.ui.button_browse, SIGNAL( clicked() ), &w, SLOT( Button_browse_action() ) );


	


	w.ui.tabWidget->setTabEnabled(1, false);
	w.ui.tabWidget->setTabEnabled(2, false);
	w.ui.tabWidget->setTabEnabled(3, false);
	w.ui.tabWidget->setTabEnabled(4, false);
	w.ui.tabWidget->setTabEnabled(5, false);
	
	return a.exec();
}
