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
	matrix = R_Image::create_matrix(other.height,other.width);
	file_name = new(std::nothrow) char[strlen(other.file_name) + 1];
	if (matrix == nullptr || file_name==nullptr)
	{
		std::cout << "Error with memmory allocation\n";
		del();
		return;
	}
		for (size_t i = 0; i < other.height; ++i)
		{
			for (size_t j = 0; j < other.width; ++j)
			{
				matrix[i][j] = other.matrix[i][j];
			}
			
		}
		strcpy(file_name, other.file_name);
		type = other.type;
		width = other.width;
		height = other.height;
		pixel_max = other.pixel_max;
}

char* R_Image::get_file_name()const
{
	return file_name;
}

char* R_Image::new_name()
{
	char* new_name = new(std::nothrow) char[strlen(file_name) + 5];
	if (new_name == nullptr)
	{
		return nullptr;
	}
	for (size_t i = 0; i < strlen(file_name)-4; ++i)
		new_name[i] = file_name[i];
	new_name[strlen(file_name) - 4] = '\0';
	strcat(new_name, "_new");
	if (type == '1')
		strcat(new_name, ".pbm");
	if (type == '2')
		strcat(new_name, ".pgm");
	if (type == '3')
		strcat(new_name, ".ppm");
	return new_name;
}

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
		for (size_t i = 0; i < height; ++i)
		{
			for (size_t j = 0; j < width; ++j)
			{
				if (matrix[i][j] != other.matrix[i][j])
				{
					std::cout << matrix[i][j] << " " << other.matrix[i][j];
					return false;
				}

			}
		}
		return true;
	}
	else return false;
}

char* R_Image::skip_comment(std::ifstream &file)
{
	static char word[100];
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
	{
		file.close();
		return false;
	}
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
	matrix = R_Image::create_matrix(height, width);
	if (matrix==nullptr)
	{
		file.close();
		del();
		return false;
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
	return true;
}

bool R_Image::get_PGMA(std::ifstream& file)
{
	char word[100];
	strcpy(word, skip_comment(file));
	pixel_max = atoi(word);
	matrix = R_Image::create_matrix(height, width);
	if (matrix == nullptr)
	{
		file.close();
		del();
		return false;
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
	if (type == PBMA)
	{
		std::cout << "Image " << file_name << " is already grayscale\n";
	}
	if (type == PGMA)
	{
		std::cout << "Image " << file_name << " is already grayscale\n";
	}
	if (type == PPMA)
	{
		for (size_t i = 0; i < height; ++i)
			for (size_t j = 0; j < width; j+=3)
			{
				size_t average = (matrix[i][j] + matrix[i][j + 1] + matrix[i][j + 2]) / 3;
				matrix[i][j] = average;
				matrix[i][j+1] = average;
				matrix[i][j+2] = average;
			}

	}
	return true;
}

bool R_Image::monochrome()
{
	if (type == PBMA)
	{
		std::cout << "Image "<<file_name<<" is already monochrome\n";
	}
	if (type == PGMA)
	{
		for (size_t i = 0; i < height; ++i)
			for (size_t j = 0; j < width; ++j)
			{
				if (matrix[i][j] <= pixel_max / 2)
					matrix[i][j] = 0;
				else matrix[i][j] = pixel_max;
			}
	}
	if (type == PPMA)
	{
		grayscale();
		for (size_t i = 0; i < height; ++i)
			for (size_t j = 0; j < width; j += 3)
			{
				if (matrix[i][j] <= pixel_max / 2)
				{ 
				matrix[i][j] = 0;
				matrix[i][j + 1] = 0;
				matrix[i][j + 2] = 0;
				}
				else
				{
					matrix[i][j] = pixel_max;
					matrix[i][j + 1] = pixel_max;
					matrix[i][j + 2] = pixel_max;
				}
			}

	}
	return true;
}

bool R_Image::negative()
{
	if (type == PBMA)
	{
		for (size_t i = 0; i < height; ++i)
			for (size_t j = 0; j < width; ++j)
				if (matrix[i][j] == 0)matrix[i][j] = 1;
				else matrix[i][j] = 0;
	}
	if (type == PGMA)
	{
		for (size_t i = 0; i < height; ++i)
			for (size_t j = 0; j < width; ++j)
				matrix[i][j] = pixel_max - matrix[i][j];
	}
	if (type == PPMA)
	{
		for (size_t i = 0; i < height; ++i)
			for (size_t j = 0; j < width; ++j)
			{
				matrix[i][j] = pixel_max - matrix[i][j];
			}
		
	}
	return true;
}

bool R_Image::rotate_right()
{
		
	if (type == PBMA || type == PGMA)
	{
		size_t** matrix_temp = R_Image::create_matrix(width, height);
		if (matrix_temp == nullptr)
			return false;
			
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
			return true;
		
	}
		else if (type == PPMA)
		{
				size_t** matrix_temp= R_Image::create_matrix(width/3,height*3);
				if (matrix_temp == nullptr)
					return false;
				for (size_t i = 0; i < width/3; ++i)
				{
					for (size_t j = 0; j < height; ++j)
					{
						matrix_temp[i][j*3] = matrix[height - j - 1][i*3];
						matrix_temp[i][j*3+1] = matrix[height - j - 1][i*3+1];
						matrix_temp[i][j*3+2] = matrix[height - j - 1][i*3+2];
					}
				}
				for (size_t i = 0; i < height; ++i)
				{
					delete[] matrix[i];
				}
				delete[] matrix;
			matrix = matrix_temp;
			size_t helper=height;
			height = width / 3;
			width = helper * 3;
			return true;
		
		}
		else return false;
}

bool R_Image::rotate_left()
{
	if (type == PBMA || type == PGMA)
	{
		size_t** matrix_temp=R_Image::create_matrix(width,height);
		if (matrix_temp == nullptr)
			return false;

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
			return true;
		
	}
		else if (type == PPMA)
		{
				size_t** matrix_temp= R_Image::create_matrix(width/3,height*3);
				if (matrix_temp == nullptr)
					return false;
				
				for (size_t i = 0; i < width/3; ++i)
				{
					for (size_t j = 0; j < height; ++j)
					{
						matrix_temp[i][j*3] = matrix[j][width - i*3 - 3];
						matrix_temp[i][j*3+1] = matrix[j][width - i*3 - 3+1];
						matrix_temp[i][j*3+2] = matrix[j][width - i*3 - 3+2];
					}
				}
				for (size_t i = 0; i < height; ++i)
				{
					delete[] matrix[i];
				}
				delete[] matrix;
			matrix = matrix_temp;
			size_t helper = height;
			height = width / 3;
			width = helper * 3;
			return true;
	}
	else return false;
}

size_t** R_Image::create_matrix(size_t _height, size_t _width)
{
	size_t **new_matrix = new(std::nothrow)size_t * [_height];
	if (new_matrix == nullptr)
		return nullptr;
	for (size_t i = 0; i < _height; ++i)
	{
		new_matrix[i] = new(std::nothrow)size_t[_width];
		if (new_matrix[i] == nullptr)
		{
			for (size_t p = 0; p < i; ++p)
				delete[]new_matrix[p];
			delete[] new_matrix;
			return nullptr;
		}
		
	}
	return new_matrix;
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

bool R_Image::collage_vertical(const R_Image*first_image, const R_Image*second_image, const char*collage_name)
{
	del();
	if (first_image->width != second_image->width)
	{
		std::cout << "Images have different width\n";
		return false;
	}
	if (first_image->type!=second_image->type || first_image->pixel_max!=second_image->pixel_max)
		return false;
	file_name = new(std::nothrow) char[strlen(collage_name) + 1];
	if (file_name == nullptr)
		return false;
	strcpy(file_name, collage_name);
	type = first_image->type;
	pixel_max = first_image->pixel_max;
	height = first_image->height + second_image->height;
	width = first_image->width;
	matrix = R_Image::create_matrix(height,width);
	if (matrix == nullptr)
	{
		del();
		return false;
	}
	for (size_t i = 0; i < first_image->height; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			matrix[i][j] = first_image->matrix[i][j];
		}
	}
	for (size_t i = first_image->height; i < height; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			matrix[i][j] = second_image->matrix[i-first_image->height][j];
		}
	}
	return true;
}

bool R_Image::collage_horizontal(const R_Image*first_image, const R_Image*second_image, const char*collage_name)
{
	del();
	if (first_image->height != second_image->height)
	{
		std::cout << "Images have different height\n";
		return false;
	}
	if (first_image->type != second_image->type)
		return false;
	file_name = new(std::nothrow) char[strlen(collage_name) + 1];
	if (file_name == nullptr)
		return false;
	strcpy(file_name, collage_name);
	pixel_max = first_image->pixel_max;
	type = first_image->type;
	height = first_image->height;
	width = first_image->width+second_image->width;
	matrix = R_Image::create_matrix(height,width);
	if (matrix == nullptr)
	{
		del();
		return false;
	}
	for (size_t i = 0; i < height ;++i)
	{
		for (size_t j = 0; j < first_image->width; ++j)
		{
			matrix[i][j] = first_image->matrix[i][j];
		}
		for (size_t j = first_image->width; j < width; ++j)
		{
			matrix[i][j] = second_image->matrix[i][j-first_image->width];
		}
	}
	return true;
}
