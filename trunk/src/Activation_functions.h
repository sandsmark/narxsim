#pragma once
class Activation_functions
{
public:
	static double sigmoid(double arg);
	static double test(double arg);
	static double linear(double arg);
	static double identity(double arg);


	static double sigmoid_derv(double arg);
	static double identity_derv(double arg);
};
