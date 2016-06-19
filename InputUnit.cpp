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

#include "InputUnit.h"


InputUnit::InputUnit(double input)
{
    input_value = input;
}

InputUnit::InputUnit()
{
}


InputUnit::~InputUnit(void)
{
}

double InputUnit::get_output()
{
    return input_value;
}

void InputUnit::set_input(double arg)
{
    input_value = arg;
}

double InputUnit::get_input()
{
    return input_value;
}