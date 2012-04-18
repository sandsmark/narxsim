#include "Activation_functions.h"
#include "math.h"


double Activation_functions::sigmoid(double arg)
{
	return 1.0 / (1.0 + exp( - arg));
}

double Activation_functions::sigmoid_derv(double arg)
{
	return sigmoid(arg)*(1 - sigmoid(arg));
}

double Activation_functions::test(double arg)
{
	return arg + 1;
}

double Activation_functions::linear(double arg)
{
	return arg>=1.0;
}