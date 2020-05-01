#pragma once
#include<iostream>
#include<string>
#include<fstream>
#define PBMA '1'
#define PGMA '2'
#define PPMA '3'



class R_Image
{
public:
	R_Image();
	R_Image(const R_Image&);
	~R_Image();

	R_Image& operator=(const R_Image&);
	bool operator==(const R_Image&)const;
	char* skip_comment(std::ifstream&);
	bool getImage(const char*);
	bool get_PBMA(std::ifstream&);
	bool get_PGMA(std::ifstream&);
	bool get_PPMA(std::ifstream&);
	bool grayscale();
	bool monochrome();
	bool negative();
	bool rotate_right();
	bool rotate_left();
	void save();
	void save_as(const char *);
	
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
