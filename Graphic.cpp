#include<iostream>
#include<string>
#include"R_Image.h"
#include"Session.h"
#include"Interface.h"

Interface Final;

void split_input(char *input, size_t lenght)
{
	size_t counter = 1;
	std::string* words;
	for (size_t i = 0; i < lenght; ++i)
	{
		if (input[i] == ' ')
		{
			counter++;
			while (input[i] == ' ')
				i++;
		}
	}
	words = new (std::nothrow) std::string[counter];
	if(words!=nullptr)
	{ 
		size_t current=0;
		for (size_t i = 0; i < counter; ++i)
		{
			for (size_t j = current; j < lenght; ++j)
			{
				if (input[j] == ' ')
				{
					while (input[j] == ' ')
					{
						j++;
					}
					current = j;
					break;
				}
				words[i] = words[i]+input[j];
				
			}
		}
		Final.detect_function(words, counter);
		delete[] words;
	}
	else
	{
		std::cout << "Error with memory allocation" << std::endl;
	}
	return;
	
}

//add missing functions ,rewrite getImage creating new functions for each type of image,maybe rewrite rotate for each type of image
int main()
{
	char input[100];
	do
	{
		std::cout << ">";
		std::cin.getline(input,100);
		
		split_input(input, strlen(input));
	} while (bool(strcmp(input, "exit"))==true);
	
	return 0;
}