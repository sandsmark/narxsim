#pragma once
#define MAX_INPUTS_PER_UNIT 100
#define MAX_SERIES_LEN 50000
#include <QtGui/QApplication>

void LOG(QString);
void FLOG(const char *);
void train_result_log(QString);

enum ARCH { MLP = 1000, NAR_D, TDNN_X, NARX_D, NARX_Y, NARX_DY, NAR_Y, NAR_DY};