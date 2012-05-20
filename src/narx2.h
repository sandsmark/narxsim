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

#ifndef NARX2_H
#define NARX2_H

#include <QtGui/QMainWindow>
#include "ui_narx2.h"

class NARX2 : public QMainWindow
{
	Q_OBJECT

public:
	NARX2(QWidget *parent = 0, Qt::WFlags flags = 0);
	~NARX2();

public slots:
	void RadioButton_generate_series();
	void RadioButton_load_series();
	void Button_12();
	void Button_21();
	void Button_23();
	void Button_34();
	void Button_32();
	void Button_43();
	void Button_45();
	void Button_54();
	void Button_56();
	void Menu_about();

	void Button_browse_action();

	void Button_start_train();

	void Button_predict();

	void train_progress_inc();
	void log();

public:
	Ui::NARX2Class ui;
};

#endif // NARX2_H
