#include "Session.h"
#include "R_Image.h"
#include<iostream>
#include<string>
#include<fstream>

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
	if (images != nullptr)
	{
		if(other.previous!=nullptr)
		{ 
		previous = new(std::nothrow) R_Image[other.images_count];
		if(previous!=nullptr)
		{ 
		for (size_t i = 0; i < other.images_count; ++i)
		{
			previous[i] = other.previous[i];
		}
		}
		}
		for (size_t i = 0; i < other.images_count; ++i)
		{//here
			images[i] = other.images[i];
		}
		ID = other.ID;
		images_count = other.images_count;
		changes_count = other.changes_count;
	}
	else
	{
		std::cout << "Error with memory allocation\n"<<"Images not copied\n";
		return;
	}
}
bool Session::add(const std::string file_name)
{//doesnt return false whn it should
	//because of unfinished input?
	if( images!=nullptr)
	{ 
	R_Image* images_temp=nullptr;
	images_temp = new(std::nothrow) R_Image[images_count];
	if (images_temp != nullptr)
	{
		for (size_t i = 0; i < images_count; ++i)
		{//here
			images_temp[i] = images[i];
		}
		delete[] images;
		images = new(std::nothrow) R_Image[images_count + 1];
			if (images != nullptr)
			{
				for (size_t i = 0; i < images_count; ++i)
				{
					images[i] = images_temp[i];

				}
				
				if(images[images_count].getImage(file_name)==false)
				{
					delete[] images;
					images = images_temp;
					
					std::cout << "Error with memory allocation1\n" << "Image not added\n";
					return false;
				}
				else
				{
					delete[] images_temp;
					images_count++;
					return true;
					
				}
				
			}
			else
			{
				images = images_temp;
				std::cout << "Error with memory allocation2\n"<<"Image not added\n";
				return false;

			}
	}
	else
	{
		std::cout << "Error with memory allocation3\n" << "Image not added\n";
		return false;
	}
	}
	else
	{
		images = new(std::nothrow) R_Image[images_count + 1];
		if (images != nullptr)
		{
			if (images[images_count].getImage(file_name) == false)
			{
				delete[] images;
				images = nullptr;
				return false;
			}
			else
			{
				images_count++;
				return true;
			}
		}
		else return false;
	}
}
void Session::del()
{
	delete[] images;
	delete[] previous;
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

bool Session::add_changes(const std::string& change)
{
	std::string* changes_temp = new(std::nothrow) std::string[images_count];
	if (changes_temp != nullptr)
	{
		for (size_t i = 0; i < changes_count; ++i)
		{
			changes_temp[i] = changes[i];
		}
		delete[]changes;
		changes = new(std::nothrow)std::string[images_count + 1];
		if (changes != nullptr)
		{
			for (size_t i = 0; i < changes_count; ++i)
			{
				changes[i] = changes_temp[i];
			}

			changes[changes_count] = change;
			changes_count++;
			return true;
		}
		else 
		{ 
			changes = changes_temp;
			return false; 
		}
	}
	else return false;

}


bool Session::apply_to_all(std::string* words, size_t lenght)
{
	return false;
}

void Session::print_changes() const
{
	if(changes_count!=0)
	{ 
		std::cout << "Pending changes:";
	for (size_t i = 0; i < changes_count; ++i)
	{
		std::cout << changes[i];
		if(i!=changes_count-1)
			std::cout<< ",";
	}
	std::cout << std::endl;
	}
	else
	{
		std::cout << "No changes made\n";
	}
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
