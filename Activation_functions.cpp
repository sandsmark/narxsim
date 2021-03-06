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

#include "Activation_functions.h"
#include "math.h"
#include "qmath.h"


double Activation_functions::sigmoid(double arg)
{
    return 1.0 / (1.0 + exp( - arg));
}
double Activation_functions::Bsigmoid(double arg)
{
    return 1.0 / (1.0 + exp( - 0.1 * arg));
}

double Activation_functions::sigmoid_derv(double arg)
{
    return sigmoid(arg) * (1 - sigmoid(arg));
}
double Activation_functions::Bsigmoid_derv(double arg)
{
    return 0.1 * Bsigmoid(arg) * (1 - Bsigmoid(arg));
}

double Activation_functions::test(double arg)
{
    return arg + 1;
}

double Activation_functions::linear(double arg)
{
    return arg >= 1.0;
}

double Activation_functions::identity(double arg)
{
    return arg;
}

double Activation_functions::aslog(double arg)
{
    if(arg > 0) {
        return qLn (1 + arg);
    } else {
        return - qLn(1 - arg);
    }
}

double Activation_functions::identity_derv(double arg)
{
    return 1;
}

double Activation_functions::pol(double arg)
{
    return arg / qSqrt(1 + arg * arg);
}

double Activation_functions::pol_derv(double arg)
{
    return qSqrt( ((1 + arg * arg) - arg * arg / qSqrt(1 + arg * arg) ) / (1 + arg * arg));
}

double Activation_functions::aslog_derv(double arg)
{
    if(arg > 0) {
        return 1 / (1 + arg);
    } else {
        return 1 / (1 - arg);
    }
}