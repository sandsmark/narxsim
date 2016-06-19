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
#define MAX_INPUTS_PER_UNIT 100
#define MAX_SERIES_LEN 50000
#include <QApplication>

void LOG(QString);
void FLOG(const char *);
void train_result_log(QString);

void normalize_f();

enum ARCH { MLP = 1000, NAR_D, TDNN_X, NARX_D, NARX_Y, NARX_DY, NAR_Y, NAR_DY, UNKNWN};
