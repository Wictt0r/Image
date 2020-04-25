#include "R_Image.h"
#include<iostream>
#include<string>
#include<fstream>

R_Image::R_Image() :type('\0'), width(0), height(0), matrix(nullptr), pixel_max(0),file_name("") {}
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
	for (size_t i = 0; i < width; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	return;
}
void R_Image::copy(const R_Image& other)
{
	//std::cout << other.type << " " << other.width << " " << other.height << " " << other.pixel_max << " " << other.file_name << " \n";
		
	matrix = new (std::nothrow)size_t* [other.width];
	if (matrix != nullptr)
	{
		for (size_t i = 0; i < other.width; ++i)
		{
			matrix[i] = new (std::nothrow)size_t[other.height];
			if (matrix[i] != nullptr)
			{
				for (size_t j = 0; j < height; ++j)
				{
					matrix[i][j] = other.matrix[i][j];
				}
			}
			else
			{
				std::cout << "Error with memory allocation3\n" << "Image deleted\n";
				del();
				return;
			}
		}
				type = other.type;
				width = other.width;
				height = other.height;
				pixel_max = other.pixel_max;
				file_name = other.file_name;
	}
	else
	{
		std::cout << "Error with memory allocation4\n"<<"Image deleted\n";
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
	if (type == other.type && width == other.width && height == other.height && pixel_max == other.pixel_max && file_name == other.file_name)
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

bool R_Image::getImage(const std::string name)
{
	
	std::ifstream file;
	file.open(name);
	if (file.good()==true)
	{
		std::string line;
		getline(file, line);
		//while (line[0] == '#');
		//getline(file,trash);
		type = line[1];
		if (type != '1' && type != '2' && type != '3')
		{
			std::cout << "Not a valid file type";
			return false;
		}
		//getline(file, line);
		//while (line[0] == '#');
		//getline(file, trash);
		file >> width >> height;
		//std::cout << width <<std::e ndl <<height;
		if (type == '3')width *= 3;
		if (type == '2')
		{
			//getline(file, line);
			//while (line[0] == '#');
			//getline(file, line);
			file >> pixel_max;
			//getline(file, line);
			//while (line[0] == '#');
			//getline(file, line);
		}
		matrix = new(std::nothrow)size_t * [width];
		if (matrix != nullptr)
		{
			for (size_t i = 0; i < width; ++i)
			{
				matrix[i] = new (std::nothrow)size_t[height];
				if (matrix[i] != nullptr)
				{
					for (size_t j = 0; j < height; ++j)
					{
						if (type == '1')
						{
							matrix[i][j] = file.get();
						}
						else
						{
							file >> matrix[i][j];
						}
					}
				}
				else
				{
					del();
					std::cout << "Error with memory allocation";
				}
			}
		}
		else std::cout << "Error with memory allocation";
		file.close();
		file_name = name;
		return true;
		/*for (size_t i = 0; i < width; ++i)
		{
			for (size_t j = 0; j < height; ++j)
			{
				std::cout << matrix[i][j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;*/
	}
	else return false;
}

bool R_Image:: grayscale()
{
	return false;
}

bool R_Image::rotate(const std::string& direction)
{

	if (height < 35)
	{
		size_t** matrix_temp;
		matrix_temp = new(std::nothrow) size_t * [height];
		if (matrix_temp != nullptr)
		{
			for (size_t i = 0; i < height; ++i)
			{
				matrix_temp[i] = new(std::nothrow) size_t[width];
				if (matrix_temp[i] == nullptr)
				{
					for (size_t p = 0; p <= i; ++p)
						delete[] matrix_temp[p];
					delete[] matrix_temp;
					return false;
				}
			}

		if (type == '1' || type == '2')
		{
			
				if (direction == "right")
					for (size_t i = 0; i < width; ++i)
					{
						for (size_t j = 0; j < height; ++j)
						{
							matrix_temp[height - j - 1][i] = matrix[i][j];
						}
					}
				if (direction == "left")
					for (size_t i = 0; i < width; ++i)
					{
						for (size_t j = 0; j < height; ++j)
						{
							matrix_temp[j][height - i] = matrix[i][j];
						}
					}
				del();
				matrix = matrix_temp;
			}
		
		else if (type == '3')
		{
			if (direction == "right")
				for (size_t i = 0; i < width; i+=3)
				{
					for (size_t j = 0; j < height; j+=3)
					{
						matrix_temp[height - j - 1][i] = matrix[i][j];
					}
				}
			if (direction == "left")
				for (size_t i = 0; i < width; ++i)
				{
					for (size_t j = 0; j < height; ++j)
					{
						matrix_temp[j][height - i] = matrix[i][j];
					}
				}
			del();
			matrix = matrix_temp;
		}
		}
		else return false;
	}
	else
	{
		std::cout << "Image height exceeds standard image width\n";
		return false;
	}
}