#include "Generators.h"


double Generators::Random(double& kernel_)
{
	double h = Floor(kernel_ / Q);
	kernel_ = A * (kernel_ - Q * h) - R * h;
	if (kernel_ < 0.0)
	{
		kernel_ += M;
	}
	return kernel_ / M;
}

double Generators::Random(int precision_, double& kernel_)
{
	double h = Floor(kernel_ / Q);
	kernel_ = A * (kernel_ - Q*h) - R*h;
	if (kernel_ < 0.0)
	{
		kernel_ += M;
	}
	return SetPrecision( kernel_ / M, precision_);
}

double Generators::Random(double min, double max, int precision_, double& kernel_)
{
	return SetPrecision(Random(kernel_) * (max - min) +min, precision_);
}

double Generators::Random(double min, double max, double& kernel_)
{
	return Random(kernel_) * (max - min) + min;
}

double Generators::RndExp(double lambda, int precision_, double& kernel_)
{
	double k = Random(kernel_);
	return SetPrecision(-(1.0/ lambda) * (double)log(k), precision_);
}

int Generators::RndZeroOne(double p, double& kernel_)
{
	double k = Random(kernel_);
	if (k < p)
		return 1;
	else
		return 0;
}

Generators::Generators()
{
	srand(time(NULL));
	for (int i = 0; i < 10; i++)
	{
		CGPk_kernel_count_[i] = 0;
		CTPk_kernel_count_[i] = 0;
		R_kernel_count_[i] = 0;
		TER_kernel_count_[i] = 0;
		PT_kernel_count_[i] = 0;
	}
}

Generators::~Generators()
{
}

void Generators::UpdateKernels()
{
	for (int i = 0; i < 10; i++)
	{
		if (CGPk_kernel_count_[i] > 100000)
		{
			CGPk_kernel_count_[i] = 0;
			kernels_for_CGPk_to_store_[i].push_back(kernels_for_CGPk_[i]);
			kernels_for_CGPk_[i] = rand() % 1000000000;
		}
		if (CTPk_kernel_count_[i] > 100000)
		{
			kernel_for_CTPk_to_store_[i].push_back(kernel_for_CTPk[i]);
			kernel_for_CTPk[i] = rand() % 1000000000;
			CTPk_kernel_count_[i] = 0;
		}
		if (R_kernel_count_[i] > 100000)
		{
			kernel_for_R_to_store_[i].push_back(kernel_for_R_[i]);
			kernel_for_R_[i] = rand() % 1000000000;
			R_kernel_count_[i] = 0;
		}
		if (TER_kernel_count_[i] > 100000)
		{
			kernel_for_TER_to_store_[i].push_back(kernel_for_TER_[i]);
			kernel_for_TER_[i] = rand() % 1000000000;
			TER_kernel_count_[i] = 0;
		}
		if (PT_kernel_count_[i] > 100000)
		{
			kernel_for_PT_to_store_[i].push_back(kernel_for_PT_[i]);
			kernel_for_PT_[i] = rand() % 1000000000;
			PT_kernel_count_[i] = 0;
		}
	}
}

void Generators::IncreaseCGPkKernelCounter(int index)
{
	CGPk_kernel_count_[index]++;
}

void Generators::IncreaseCTPkKernelCounter(int index)
{
	CTPk_kernel_count_[index]++ ;
}

void Generators::IncreaseRKernelConuter(int index)
{
	R_kernel_count_[index]++;
}

void Generators::IncreaseTERKernelCounter(int index)
{
	TER_kernel_count_[index]++;
}

void Generators::IncreasePTKernelCounter(int index)
{
	PT_kernel_count_[index]++;
}

string Generators::GetOldKernelsAsString()
{
	string kernels_ = "kernels_for_CGPk_: \n";
	for (int i = 0; i < 10; i++)
	{
		kernels_ += "BS[" + to_string(i) + "]: ";
		for (int q = 0; q < kernels_for_CGPk_to_store_[i].size(); q++)
		{
			kernels_ += to_string(kernels_for_CGPk_to_store_[i][q]) +" ";
		}
		kernels_ += "\n";
	
	    kernels_ += "kernel_for_CTPk_: \n";
		kernels_ += "BS[" + to_string(i) + "]: ";
	    for (int q = 0; q < kernel_for_CTPk_to_store_[i].size(); i++)
	    {
		    kernels_ += to_string(kernel_for_CTPk_to_store_[i][q]) + " ";
	    }
	    kernels_ += "\n kernel_for_R_ \n";
		kernels_ += "BS[" + to_string(i) + "]: ";
	    for (int q = 0; q < kernel_for_R_to_store_[i].size(); q++)
	    {
		    kernels_ += to_string(kernel_for_R_to_store_[i][q]) + " ";
	    }
	    kernels_ += "\n kernel_for_TER_ \n";
		kernels_ += "BS[" + to_string(i) + "]: ";
	    for (int q = 0; q < kernel_for_TER_to_store_[i].size(); q++)
	    {
	    	kernels_ += to_string(kernel_for_TER_to_store_[i][q]) + " ";
	    }
	    kernels_ += "\n kernel_for_PT_ \n";
		kernels_ += "BS[" + to_string(i) + "]: ";
	    for (int q = 0; q < kernel_for_PT_to_store_[i].size(); q++)
	    {
	    	kernels_ += to_string(kernel_for_PT_to_store_[i][q]) + " ";
	    }
	}
	return kernels_;
}

void Generators::InitializeKernels()
{
	vector<double> loaded_values_;
	string markup_begin_, markup_end_;
	string kernelsfile_ = "";
	int linenumber = 0;
	fstream file;
	bool file_opened_ = false;
	while (file_opened_ == false)
	{
		cout << "Specify kernel filename.smf" << endl;// smf- seed markup file
		cin >> kernelsfile_;
		file.open(kernelsfile_, ios::in);
		if (file.good() == false)
		{
			cout << "File does not exist try again." << endl;
		}
		else
		{
			file_opened_ = true;
		}
	}
	bool parse_success_ = false;
	while (parse_success_ == false)
	{
		cout << "Specify line of the kernel's file. (expected number)" << endl;
		cin >> linenumber;
		LineToMarkups(linenumber, markup_begin_, markup_end_);
		parse_success_ = ParseFile(loaded_values_, file, markup_begin_, markup_end_);
		if (parse_success_ == false)
		{
			cout << "Given line does not exist! Try again." << endl;
		}
	}
	file.close();
	//////////////////////////////////////////////////////////////////////////Section:
	if (loaded_values_.size() < 50)
	{
		cout << "In the kernel file.smf was too little values in the line.\n Program will finish."<<endl;
		system("Pause");
		exit(1);
	}
	else
	{
		assert(loaded_values_.size() >= 50);
		int load_counter_ = 10;
		for (int i = 0; i < 10; i++)
		{
			kernels_for_CGPk_[i] = loaded_values_[i];
		}
		for (int i = 0; i < 10; i++)
		{
			kernel_for_CTPk[i] = loaded_values_[load_counter_];
			load_counter_++;
		}
		for (int i = 0; i < 10; i++)
		{
			kernel_for_R_[i] = loaded_values_[load_counter_];
			load_counter_++;
		}
		for (int i = 0; i < 10; i++)
		{
			kernel_for_TER_[i] = loaded_values_[load_counter_];
			load_counter_++;
		}
		for (int i = 0; i < 10; i++)
		{
			kernel_for_PT_[i] = loaded_values_[load_counter_];
			load_counter_++;
		}
	}
}
void Generators::LineToMarkups(int line, string& markup_begin, string& markup_end)
{
	markup_begin = "<L"+to_string(line)+">";
	markup_end = "<EOL"+to_string(line)+">";
}
bool Generators::ParseFile(vector<double>& values, fstream& file_, string begin_, string end)
{
	string txt_ = "";
	while (txt_ != begin_)
	{
		file_ >> txt_;
		if (txt_ == "<EOF>")
		{
			return false;
		}
	}
	size_t sz;
	file_ >> txt_;//Skip one markup
	while (txt_ != end)
	{
		values.push_back(stod(txt_, &sz));
		file_ >> txt_;
	}
	return true;
}

double Generators::Floor(double number_)
{
	double fraction_ = number_;
	double integer_ = 0;
	fraction_ = modf(number_, &integer_);
	if (number_ >= 0.0)
	{
		return integer_;
	}
	else
	{
		return integer_ - 1.0;
	}

}

int Generators::FloorToInt(double number_)
{
	if (number_ >= 0.0)
	{
		return static_cast<int> (number_);
	}
	else
	{
		return static_cast<int> (number_ - 1.0);
	}
}

size_t Generators::FloorToSizeT(double number_)
{
	if (number_ >= 0.0)
	{
		return static_cast<size_t> (number_);
	}
	else
	{
		return static_cast<size_t> (number_ - 1.0);
	}
}

double Generators::SetPrecision(double value_, int numbers_after_coma_)
{
	int multiplier = 1;
	for (int i = 0; i < numbers_after_coma_; i++)
	{
		multiplier = multiplier * 10;
	}
	value_ = value_ * (double)multiplier;
	value_ = Floor(value_);
	value_ = value_ / (double)multiplier;
	return value_;
}
