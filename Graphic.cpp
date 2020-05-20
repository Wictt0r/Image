#include<iostream>
#include<string>
#include"R_Image.h"
#include"Session.h"
#include"Interface.h"


//add missing functions ,rewrite getImage creating new functions for each type of image,maybe rewrite rotate for each type of image
int main()
{
	Interface interface;
	char input[100];
	do
	{
		std::cout << ">";
		std::cin.getline(input,100);
		
		interface.split_input(input, strlen(input));
	} while (strcmp(input, "exit")!=0);
	
	return 0;//test comment
}