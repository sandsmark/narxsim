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
class EvaluationEngine
{
protected:
	int series_len;
	int curlen;

	double *series;
	double *predicted;


	double Fd(double val);
	double Fy(double val);


public:
	EvaluationEngine(int slen);
	~EvaluationEngine(void);

	void insertvalue(double target, double pred);

	double F3();
	double F1();
	double F2();
	double F4();

	double KS1();
	double KS2();
	double KS12();

	double DA();

	void reset();
};

