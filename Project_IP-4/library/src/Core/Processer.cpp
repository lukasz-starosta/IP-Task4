#include "Processer.h"



int Processer::truncate(double value)
{
	if (value < 0) return 0;
	if (value > 255) return 255;

	return (int)value;
}


Processer::Processer()
{
}

Processer::Processer(std::string imageName, int option, double value)
	:imageName(imageName), option(option), value(value)
{
}


Processer::~Processer()
{
}
