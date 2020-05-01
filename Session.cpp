#include "Session.h"
#include "R_Image.h"
#include<iostream>
#include<fstream>

#pragma warning(disable:4996)
Session::Session() :images(nullptr),images_count(0), previous(nullptr),ID(0),changes(nullptr),changes_count(0){}

Session::Session(const Session& other)
{
	copy(other);
}

Session::~Session()
{
	del();
}

Session& Session::operator=(const Session& other)
{
	if(this!=&other)
	{ 
		del();
		copy(other);
	}
	return *this;

}

void Session::copy(const Session& other)
{
	images = new (std::nothrow) R_Image[other.images_count];
	if (images == nullptr)
	{
		std::cout << "Error with memory allocation\n" << "Images not copied\n";
		return;
	}
	if (other.previous != nullptr && other.changes != nullptr)
	{
		changes = new (std::nothrow) char* [other.changes_count];
		previous = new(std::nothrow) R_Image[other.images_count];
		if (previous == nullptr || changes == nullptr)
		{
			std::cout << "Error with memory allocation\n" << "Images not copied\n";
			del();
			return;
		}
		for (size_t i = 0; i < other.images_count; ++i)
			previous[i] = other.previous[i];

		for (size_t i = 0; i < other.changes_count; ++i)
		{
			changes[i] = new(std::nothrow) char[strlen(other.changes[i]) + 1];
			if (changes[i] == nullptr)
			{
				std::cout << "Error with memory allocation\n" << "Images not copied\n";
				del();
				return;
			}
			strcpy(changes[i], other.changes[i]);
		}
	}
	

	for (size_t i = 0; i < other.images_count; ++i)
		images[i] = other.images[i];

	ID = other.ID;
	images_count = other.images_count;
	changes_count = other.changes_count;
}

bool Session::add(const char* file_name)
{ 
	R_Image* images_temp = new(std::nothrow) R_Image[images_count+1];
	if (images_temp == nullptr)
	{
		std::cout << "Error with memory allocation3\n" << "Image not added\n";
		return false;
	}
		for (size_t i = 0; i < images_count; ++i)
		{
			images_temp[i] = images[i];
		}
				
		if(images_temp[images_count].getImage(file_name)==false)
		{
			std::cout << "Error with memory allocation1\n" << "Image not added\n";
			return false;
		}
		else
		{
			delete[] images;
			images = images_temp;
			images_count++;
			return true;
		}
}

void Session::del()
{
	delete[] images;
	delete[] previous;
	for (size_t i = 0; i < changes_count; ++i)
		delete[] changes[i];
	delete[] changes;
}

void Session::undo()
{
	if(previous!=nullptr)
	{ 
	for (size_t i = 0; i < images_count; ++i)
		images[i] = previous[i];
	}
	else
	{
		std::cout << "No changes made\n";
	}

	return;
}

bool Session::is_default()const
{
	if (images == nullptr && images_count == 0 && previous == nullptr && ID == 0 && changes_count == 0 && changes == nullptr)
		return true;
	else return false;
}

bool Session::add_changes(const char* change)
{
	char** changes_temp = new (std::nothrow) char* [changes_count + 1];
	if (changes_temp == nullptr)
		return false;
	for (size_t i = 0; i < changes_count; ++i)
	{
		changes_temp[i] = new(std::nothrow)char[strlen(changes[i]) + 1];
		if (changes_temp[i] == nullptr)
		{
			for (size_t j = 0; j < i; ++j)
				delete[] changes_temp[i];
			delete[] changes_temp;
			return false;
		}
		strcpy(changes_temp[i],changes[i]);
	}
	changes_temp[changes_count] = new(std::nothrow)char[strlen(change) + 1];
	if (changes_temp[changes_count] == nullptr)
		return false;
	strcpy(changes_temp[changes_count], change);
	for (size_t i = 0; i < changes_count; ++i)
		delete[] changes[i];
	delete[] changes;
	changes = changes_temp;
	++changes_count;
	return true;
}


bool Session::apply_to_all(bool (R_Image::*function)())
{
	previous = new(std::nothrow)R_Image[images_count];
	if (previous == nullptr)
		return false;
	for (size_t i = 0; i < images_count; ++i)
		previous[i] = images[i];

	bool flag = true;
	for (size_t i = 0; i < images_count; ++i)
	{
		if ((images[i].*function)() == false)
		{
			flag = false;
			undo();
			break;
		}
	}
	return flag;
}

void Session::save_all()
{
	for (size_t i = 0; i < images_count; ++i)
		images[i].save();
	return;
}

void Session::save_as_all()
{
	for (size_t i = 0; i < images_count; ++i)
		images[i].save_as("test.pgm");
	return;
}

void Session::print_changes() const
{
	if(changes_count==0)
	{ 
		std::cout << "No changes made\n";
		return;
	}
	std::cout << "Pending changes:";
	for (size_t i = 0; i < changes_count; ++i)
	{
		std::cout << changes[i];
		if(i!=changes_count-1)
			std::cout<< ",";
	}
	std::cout << std::endl;
	
	return;
}

void Session::print_images_file_names() const
{
	if (images != nullptr)
	{
		std::cout << "Names of the images in the session:";
		for (size_t i = 0; i < images_count; ++i)
		{
			images[i].print_file_name();
			if(i!=images_count-1)
			std::cout << ",";
		}
		std::cout << std::endl;
	}
	return;
}

//print
//void Session::print()
//{
//	for (size_t i = 0; i < images_count; ++i)
//	{
//		images[i].print();
//		std::cout << std::endl;
//	}
//	for (size_t i = 0; i < images_count; ++i)
//	{
//		previous[i].print();
//		std::cout << std::endl;
//	}
//	std::cout <<"ID:"<< ID;
//	std::cout << std::endl;
//	std::cout << "changes_count:"<<changes_count;
//	std::cout << std::endl;
//	return;
//}
