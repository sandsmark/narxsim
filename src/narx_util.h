#pragma once
#define MAX_INPUTS_PER_UNIT 100
#define MAX_SERIES_LEN 50000
#include <QtGui/QApplication>

void LOG(QString);
void FLOG(char *);

enum ARCH { MLP = 1000, NAR_D};