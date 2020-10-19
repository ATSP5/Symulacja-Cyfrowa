#ifndef NUMERICINTEGRATOR_H
#define NUMERICINTEGRATOR_H

class NumericIntegrator
{
    //Object of this class is to be used as vector members in histogram build. 
private:
    double number_of_occurance_;
    double value_;
public:
    NumericIntegrator(double val_);
    ~NumericIntegrator();
    double GetValue();
    double GetNumberOfOcurance();
    void IncreaseNumberOfOcurance(double number_to_add_);
};


#endif // !NUMERICINTEGRATOR_H


