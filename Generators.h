#ifndef GENERATORS_H
#define GENERATORS_H
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <ostream>
#include <string>
#include <fstream>
#include <iostream>
#include <assert.h>
using namespace std;
class Generators
{
public:
	double Random(double &kernel_);
	double Random(int precision_, double& kernel_);
	double Random(double min, double max, double& kernel_);
	double Random(double min, double max, int precision_, double& kernel_);
	double RndExp(double lambda, int precision_, double& kernel_);
	int RndZeroOne(double p, double& kernel_);
	Generators();
	~Generators();
	void UpdateKernels();//Wylosuj nowe kernele jeœli liczba wyst¹pieñ przekroczy³a 100 000
	//Metody zwiêkszaj¹ce o 1 liczbê wyst¹pieñ odpowiednich kerneli: 
	void IncreaseCGPkKernelCounter(int index);
	void IncreaseCTPkKernelCounter(int index);
	void IncreaseRKernelConuter(int index);
	void IncreaseTERKernelCounter(int index);
	void IncreasePTKernelCounter(int index);
	//Kernels:
	double kernels_for_CGPk_[10];
	double kernel_for_CTPk[10];
	double kernel_for_R_[10];
	double kernel_for_TER_[10];
	double kernel_for_PT_[10];
	//
	string GetOldKernelsAsString();
	void InitializeKernels();
private:
	void LineToMarkups(int line, string& markup_begin, string& markup_end);
	bool ParseFile(vector<double>& values, fstream& file_, string begin_, string end);
	double Floor(double number_);
	int FloorToInt(double number_);
	size_t FloorToSizeT(double number_);
	double SetPrecision(double value_, int numbers_after_coma_);
	double M = 2147483647.0;
	double A = 16807.0;
	double Q = 127773.0;
	double R = 2836;
	//Liczniki losowañ:
	int CGPk_kernel_count_[10];
	int CTPk_kernel_count_[10];
	int R_kernel_count_[10];
	int TER_kernel_count_[10];
	int PT_kernel_count_[10];
	//Kernele do zapisania:
	vector<vector<double>> kernels_for_CGPk_to_store_ = { {},{},{},{},{},{},{},{},{},{} };
	vector<vector<double>> kernel_for_CTPk_to_store_ = { {},{},{},{},{},{},{},{},{},{} };
	vector<vector<double>> kernel_for_R_to_store_ = { {},{},{},{},{},{},{},{},{},{} };
	vector<vector<double>> kernel_for_TER_to_store_ = { {},{},{},{},{},{},{},{},{},{} };
	vector<vector<double>> kernel_for_PT_to_store_ = { {},{},{},{},{},{},{},{},{},{} };
};

#endif // !GENERATORS_H


