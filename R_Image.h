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
	bool getImage(const char*);
	bool grayscale();
	bool monochrome();
	bool negative();
	bool rotate_right();
	bool rotate_left();
	static size_t** create_matrix(size_t, size_t);
	bool collage_vertical(const R_Image*,const R_Image*,const char*);
	bool collage_horizontal(const R_Image*,const R_Image*,const char*);
	void save();
	void save_as(const char *);
	
	char* get_file_name()const;
	char* new_name();
	void del();
	void copy(const R_Image&);
private:
	char type;
	size_t width, height;
	size_t** matrix;
	size_t pixel_max;
	char* file_name;

	char* skip_comment(std::ifstream&);
	bool get_PBMA(std::ifstream&);
	bool get_PGMA(std::ifstream&);
	bool get_PPMA(std::ifstream&);
};
