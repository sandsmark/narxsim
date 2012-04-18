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
	void Menu_about();
	void Button_start_train();

	void train_progress_inc();
	void log();

public:
	Ui::NARX2Class ui;
};

#endif // NARX2_H
