#pragma once
#include <string>

class ArgumentsHandler
{
private:
	int argc;
	char** argv;

	std::string option;
	std::string value;
	std::string secondValue;
	std::string imageName;
	std::string noisyImageName;
	std::string denoisedImageName;

	std::string options[36]{
		//Task 1 options (17 options) <0, 16>
		"--help", "--brightness", "--contrast", "--negative", "--hflip", "--vflip", "--dflip", "--shrink", "--enlarge", "--min", "--max", "--median", "--mse", "--pmse", "--snr", "--psnr", "--md",
		//Task 2 options (9 options) <17, 28>
		"--histogram", "--hpower", "--cmean", "--cvariance", "--cstdev", "--cvarcoi", "--casyco", "--cflatco", "--cvarcoii", "--centropy", "--sedgesharp", "--orosenfeld",
		//Task 3 options (7 options) <29, 35>
		"--dilation", "--erosion", "--opening", "--closing", "--hmt", "--mvariant", "--merging"
	};

	std::string optionsRequiringValues[25]{
		"--brightness", "--contrast", "--shrink", "--enlarge", "--min", "--max", "--median", "--histogram", "--cmean", "--cvariance", "--cstdev", "--cvarcoi", "--casyco", "--cflatco", "--cvarcoii", "--centropy", "--hpower", "--sedgesharp", "--orosenfeld", "--dilation", "--erosion", "--opening", "--closing", "--hmt", "--merging"
	};

	std::string optionsRequiringChannel[9]{
		"--histogram", "--cmean", "--cvariance", "--cstdev", "--cvarcoi", "--casyco", "--cflatco", "--cvarcoii", "--centropy"
	};

	bool optionIsValid(std::string option) const;
	bool optionRequiresChannel(std::string option) const;
	bool optionRequiresValue(std::string option) const;
	bool isPowerOfTwo(int x) const;
	bool valueIsValid(std::string value) const;
	bool isNameOfFile(std::string name) const;
	bool isWithinPixelRange(int value) const;

public:
	ArgumentsHandler(int argc, char* argv[]);
	~ArgumentsHandler();

	enum Processers
	{
		None = 0,
		ImageProcesser,
		HistogramProcesser,
		MorphologicalProcesser,
	};

	struct Arguments
	{
		std::string imageName;
		std::string noisyImageName;
		std::string denoisedImageName;
		int option;
		double value;
		int secondValue;
		Processers processer;
	};

	bool argumentsAreValid = false;

	int convertToInt(std::string option) const;

	void helpMessage() const;
	void validateArguments();
	Processers get_currentProcesser() const;
	Arguments get_arguments() const;
};