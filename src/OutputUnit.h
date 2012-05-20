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
#include "Unit.h"
class OutputUnit :
	public Unit
{
protected:

	double target;

	double deltao;

public:
	OutputUnit(void);
	~OutputUnit(void);

	void setTarget(double target);
	double error();

	virtual void adjust_weights();

	double get_delta();
};

