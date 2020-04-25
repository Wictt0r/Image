#pragma once
#include<iostream>
#include<string>


class R_Image
{
public:
	R_Image();
	R_Image(const R_Image&);
	~R_Image();

	R_Image& operator=(const R_Image&);
	bool operator==(const R_Image&)const;
	bool getImage(const char*);
	bool grayscale();
	bool rotate(const char*);
	
	void print_file_name()const;
	void del();
	void copy(const R_Image&);
	//void print();
	//bool is_default()const;
	//void set_default();

private:
	char type;
	size_t width, height;
	size_t** matrix;
	size_t pixel_max;
	char* file_name;

	
};
