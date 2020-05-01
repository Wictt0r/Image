#include "R_Image.h"
#include<iostream>
#include<fstream>
#include<cstring>
#pragma warning(disable:4996)

#define PBMA '1'
#define PGMA '2'
#define PPMA '3'
#define MAX_LINE_LENGHT 70

R_Image::R_Image() :type('\0'), width(0), height(0), matrix(nullptr), pixel_max(0),file_name(nullptr) {}
R_Image::R_Image(const R_Image& other)
{
	copy(other);
}
R_Image::~R_Image()
{
	del();
}

void R_Image::del()
{
	for (size_t i = 0; i < height; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	delete[] file_name;
	return;
}
void R_Image::copy(const R_Image& other)
{
	//std::cout << other.type << " " << other.width << " " << other.height << " " << other.pixel_max << " " << other.file_name << " \n";
		
	matrix = new (std::nothrow)size_t* [other.height];
	if (matrix != nullptr)
	{
		for (size_t i = 0; i < other.height; ++i)
		{
			matrix[i] = new (std::nothrow)size_t[other.width];
			if (matrix[i] != nullptr)
			{
				for (size_t j = 0; j < other.width; ++j)
				{
					matrix[i][j] = other.matrix[i][j];
				}
			}
			else
			{
				std::cout << "Error with memory allocation3\n";
				del();
				return;
			}
		}
		file_name = new(std::nothrow) char[strlen(other.file_name) + 1];
		if (file_name == nullptr)
		{
			std::cout << "Error with memmory allocation\n";
			del();
			return;
		}
		strcpy(file_name, other.file_name);
				type = other.type;
				width = other.width;
				height = other.height;
				pixel_max = other.pixel_max;
				/*for (int i = 0; i < width; ++i)
				{
					for (int j = 0; j < height; ++j)
						std::cout << matrix[i][j] << " ";
					std::cout << std::endl;
				}*/

	}
	else
	{
		std::cout << "Error with memory allocation4\n";
		return;
	}
	
}

void R_Image::print_file_name()const
{
	std::cout << file_name;
	return;
}

//void R_Image::print()
//{
//	if(this!=nullptr)
//	for (size_t i = 0; i < width; ++i)
//	{
//		if (matrix[i] != nullptr)
//			for (size_t j = 0; j < height; ++j)
//			{
//
//				std::cout << matrix[i][j] << " ";
//			}
//		else std::cout << "matrix[i] is nullptr\n";
//		std::cout << std::endl;
//	}
//	else
//	{
//		std::cout << "matrix is nullptr\n";
//		return;
//	}
//
//}

R_Image& R_Image ::operator= (const R_Image& other)
{

	if(this != &other)
	{
	del();
	copy(other);
	}
	return *this;
}

bool R_Image::operator==(const R_Image& other) const
{
	if (type == other.type && width == other.width && height == other.height && pixel_max == other.pixel_max && strcmp(file_name,other.file_name)==0)
	{
		for (size_t i = 0; i < width; ++i)
		{
			for (size_t j = 0; j < height; ++j)
			{
				if (matrix[i][j] != other.matrix[i][j]) return false;

			}
		}
		return true;
	}
	else return false;
}


//bool R_Image::is_default() const
//{
//	if (type == '\0' && width == 0 && height == 0 && matrix == nullptr && pixel_max == 0 && file_name == "")
//		return true;
//	else return false;
//}

/*
void R_Image::set_default() 
{
	type = '\0';
	width = 0; height = 0;
	matrix = nullptr; 
	pixel_max = 0;
	file_name = "";
	return;
}
*/
char* R_Image::skip_comment(std::ifstream &file)
{
	char word[100];
	file >> word;
	while (word[0] == '#' && file.good())
	{
		file.ignore(MAX_LINE_LENGHT, '\n');
		file >> word;
	}
	return word;
}

bool R_Image::getImage(const char * name)
{
	std::ifstream file;
	file.open(name);
	file_name = new(std::nothrow) char[strlen(name) + 1];
	if (file_name == nullptr)
		return false;
	strcpy(file_name, name);
	if (file.good() == true)
	{
		char word[100];
		strcpy(word, skip_comment(file));
		type = word[1];
		strcpy(word, skip_comment(file));
		width = atoi(word);//converts char* to int
		file >> word;
		height = atoi(word);//converts char* to int
		if (type == PBMA)
			return get_PBMA(file);
		if (type == PGMA)
			return get_PGMA(file);
		if (type == PPMA)
			return get_PPMA(file);
	}
	del();
	file.close();
	return false;
}
bool R_Image::get_PBMA(std::ifstream& file)
{
	matrix = new(std::nothrow) size_t * [height];
	if (matrix == nullptr)
	{
		del();
		return false;
	}
	for (size_t i = 0; i < height; ++i)
	{
		matrix[i] = new(std::nothrow)size_t[width];
		if (matrix[i] == nullptr)
		{
			del();
			return false;
		}
	}
	for (size_t i = 0; i < height && file.good(); ++i)
	{
		for (size_t j = 0; j < width && file.good(); ++j)
		{
			char letter;
			file.get(letter);
			if ( letter == '#')
			{
				file.ignore(MAX_LINE_LENGHT, '\n');
				file.get(letter);
			}
			while (letter == '\n' || letter == ' ')
				file.get(letter);
			matrix[i][j] = letter - '0';
		}
	}
	file.close();
	/*for (size_t i = 0; i < height; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			std::cout << matrix[i][j] << ' ';
		}
		std::cout << std::endl;
	}*/
	return true;
}

bool R_Image::get_PGMA(std::ifstream& file)
{
	char word[100];
	strcpy(word, skip_comment(file));
	pixel_max = atoi(word);
	matrix = new(std::nothrow) size_t * [height];
	if (matrix == nullptr)
	{
		del();
		return false;
	}
	for (size_t i = 0; i < height; ++i)
	{
		matrix[i] = new(std::nothrow)size_t[width];
		if (matrix[i] == nullptr)
		{
			del();
			return false;
		}
	}
	for (size_t i = 0; i < height && file.good(); ++i)
	{
		for (size_t j = 0; j < width && file.good(); ++j)
		{
			strcpy(word, skip_comment(file));
			matrix[i][j] = atoi(word);
		}
	}
	file.close();
	return true;
}

bool R_Image::get_PPMA(std::ifstream&file)
{
	width *= 3;
	return get_PGMA(file);
}

bool R_Image:: grayscale()
{
	return false;
}

bool R_Image::monochrome()
{
	return false;
}

bool R_Image::negative()
{
	return false;
}

bool R_Image::rotate_right()
{
		
	if (type == PBMA || type == PGMA)
	{
		size_t** matrix_temp;
		matrix_temp = new(std::nothrow) size_t * [width];
		if (matrix_temp != nullptr)
		{
			for (size_t i = 0; i < width; ++i)
			{
				matrix_temp[i] = new(std::nothrow) size_t[height];
				if (matrix_temp[i] == nullptr)
				{
					for (size_t p = 0; p <= i; ++p)
						delete[] matrix_temp[p];
					delete[] matrix_temp;
					return false;
				}
			}
			
			for (size_t i = 0; i < width; ++i)
			{
				for (size_t j = 0; j < height; ++j)
				{
					matrix_temp[i][j] = matrix[height - j - 1][i];
				}
			}
			for (size_t i = 0; i < height; ++i)
			{
				delete[] matrix[i];
			}
			delete[] matrix;
			matrix = matrix_temp;
			width += height;
			height = width - height;
			width -= height;
			
		}
	}
		else if (type == PPMA)
		{
				size_t** matrix_temp;
				matrix_temp = new(std::nothrow) size_t * [width/3];
				if (matrix_temp != nullptr)
				{
					for (size_t i = 0; i < width/3; ++i)
					{
						matrix_temp[i] = new(std::nothrow) size_t[height*3];
						if (matrix_temp[i] == nullptr)
						{
							for (size_t p = 0; p <= i; ++p)
								delete[] matrix_temp[p];
							delete[] matrix_temp;
							return false;
						}
					}

				for (size_t i = 0; i < width; i+=3)
				{
					for (size_t j = 0; j < height; ++j)
					{
						matrix_temp[height - j - 1][i] = matrix[i][j];
						matrix_temp[height - j - 1][i+1] = matrix[i+1][j];
						matrix_temp[height - j - 1][i+2] = matrix[i+2][j];
					}
				}
				for (size_t i = 0; i < height; ++i)
				{
					delete[] matrix[i];
				}
				delete[] matrix;
			matrix = matrix_temp;
			width += height;
			height = width - height;
			width -= height;
		}
		}
		else return false;
}

bool R_Image::rotate_left()
{
	if (type == PBMA || type == PGMA)
	{
		size_t** matrix_temp;
		matrix_temp = new(std::nothrow) size_t * [width];
		for (size_t i = 0; i < width; ++i)
		{
			matrix_temp[i] = new(std::nothrow) size_t[height];
			if (matrix_temp[i] == nullptr)
			{
				for (size_t p = 0; p <= i; ++p)
					delete[] matrix_temp[p];
				delete[] matrix_temp;
				return false;
			}
		}
			for (size_t i = 0; i < width; ++i)
			{
				for (size_t j = 0; j < height; ++j)
				{
					matrix_temp[i][j] = matrix[j][width - i - 1];
				}
			}
			for (size_t i = 0; i < height; ++i)
			{
				delete[] matrix[i];
			}
			delete[] matrix;
			matrix = matrix_temp;
			width += height;
			height = width - height;
			width -= height;
			matrix = matrix_temp;
		
	}
		else if (type == PPMA)
		{
				size_t** matrix_temp;
				matrix_temp = new(std::nothrow) size_t * [width / 3];
				if (matrix_temp != nullptr)
				{
					for (size_t i = 0; i < width / 3; ++i)
					{
						matrix_temp[i] = new(std::nothrow) size_t[height * 3];
						if (matrix_temp[i] == nullptr)
						{
							for (size_t p = 0; p <= i; ++p)
								delete[] matrix_temp[p];
							delete[] matrix_temp;
							return false;
						}
					}
				for (size_t i = 0; i < width; i+=3)
				{
					for (size_t j = 0; j < height; ++j)
					{
						matrix_temp[j][height - (i)] = matrix[i][j];
						matrix_temp[j][height - (i+1)] = matrix[i+1][j];
						matrix_temp[j][height - (i+2)] = matrix[i+2][j];
					}
				}
				for (size_t i = 0; i < height; ++i)
				{
					delete[] matrix[i];
				}
				delete[] matrix;
			matrix = matrix_temp;
			width += height;
			height = width - height;
			width -= height;
		}
	}
	else return false;
}

void R_Image::save() 
{
	save_as(file_name);
	return;
}

void R_Image::save_as(const char* new_name) 
{
	std::ofstream file;
	file.open(new_name,std::ios::trunc);
	file << 'P' << type << std::endl;
	if(type==PPMA)
	file << width/3 << ' ' << height << std::endl;
	else
	file << width << ' ' << height << std::endl;
	if (type == PPMA || type == PGMA)
	{
		file << pixel_max << std::endl;
	}
	size_t line_limit_counter = 0,line_limit=0;
	if (type == PPMA) line_limit = 12;
	if (type == PGMA) line_limit = 19;
	if (type == PBMA) line_limit = 39;
	for (size_t i = 0; i < height; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			file << matrix[i][j] << ' ';
			line_limit_counter += 1;
			if (line_limit_counter == line_limit)
			{
				file<< std::endl;
				line_limit_counter = 0;
			}
			
		}
	}
	file.close();
	return;
}
