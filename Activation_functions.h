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
class Activation_functions
{
public:
	static double sigmoid(double arg);
	static double Bsigmoid(double arg);
	static double test(double arg);
	static double linear(double arg);
	static double identity(double arg);
	static double aslog(double arg);
	static double pol(double arg);


	static double sigmoid_derv(double arg);
	static double identity_derv(double arg);
	static double pol_derv(double arg);
	static double Bsigmoid_derv(double arg);
	static double aslog_derv(double arg);
};
