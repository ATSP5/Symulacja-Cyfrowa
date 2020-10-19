#include "NumericIntegrator.h"

NumericIntegrator::NumericIntegrator(double val_)
{
	number_of_occurance_ = 1;
	value_ = val_;
}

NumericIntegrator::~NumericIntegrator()
{
}

double NumericIntegrator::GetValue()
{
	return value_;
}

double NumericIntegrator::GetNumberOfOcurance()
{
	return number_of_occurance_;
}

void NumericIntegrator::IncreaseNumberOfOcurance(double number_to_add_)
{
	number_of_occurance_ += number_to_add_;
}
