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
		std::cout << "Image:" << file_name << " added\n";
		delete[] images;
		images = images_temp;
		images_count++;
		return true;
	}
}

bool Session::add(const R_Image& image)
{
	R_Image* images_temp = new(std::nothrow) R_Image[images_count + 1];
	if (images_temp == nullptr)
	{
		std::cout << "Error with memory allocation\n" << "Image not added\n";
		return false;
	}
	for (size_t i = 0; i < images_count; ++i)
	{
		images_temp[i] = images[i];
	}
	images_temp[images_count] = image;
	if (images_temp[images_count]==image)
	{
		std::cout << "Image:" << image.get_file_name() << " added\n";
		delete[] images;
		images = images_temp;
		images_count++;
		return true;
	}
	else
	{
		std::cout << "Error with memory allocation\n" << "Image not added\n";
		return false;
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

bool Session::create_previous()
{
	previous = new(std::nothrow)R_Image[images_count];
	if (previous == nullptr)
		return false;
	for (size_t i = 0; i < images_count; ++i)
	{
		previous[i] = images[i];
		if (!(previous[i] == images[i]))
		{
			delete[] previous;
			return false;
		}
	}
	return true;
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
				delete[] changes_temp[j];
			delete[] changes_temp;
			return false;
		}
		strcpy(changes_temp[i], changes[i]);

	}
	changes_temp[changes_count] = new(std::nothrow)char[strlen(change) + 1];
	if (changes_temp[changes_count] == nullptr)
	{
		for (size_t i = 0; i <=changes_count; ++i)
			delete[] changes_temp[i];
		delete[] changes_temp;
		return false;
	}
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
	if (create_previous() == false)
		return false;
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
	{
		images[i].save();
		std::cout << images[i].get_file_name() <<" saved\n";
	}
	return;
}

void Session::save_as_all()
{
	for (size_t i = 0; i < images_count; ++i)
	{
		char* image_name = images[i].new_name();
		if (image_name == nullptr)
		{
			std::cout << "image: " << images[i].get_file_name() << "not saved\n";
			continue;
		}
		images[i].save_as(image_name);
		std::cout << images[i].get_file_name()<< " saved as " << image_name << std::endl;
		delete[] image_name;
	}
	return;
}

void Session::collage(char*direction,char*fist_image_name,char*second_image_name,char*collage_name)
{
	R_Image* first_image = find_image(fist_image_name),
		* second_image = find_image(second_image_name);
	if (first_image == nullptr || second_image == nullptr)
	{
		std::cout << "Image not found\nCollage not added\n";
		return;
	}
	R_Image collage;
	if (strcmp(direction, "horizontal") == 0)
	{
		if (collage.collage_horizontal(first_image, second_image, collage_name) == false)
		{
			std::cout << "Error\nCollage not added\n";
			return;
		}
		add(collage);
		return;
	}
	if (strcmp(direction, "vertical") == 0)
	{
		if (collage.collage_vertical(first_image, second_image, collage_name) == false)
		{
			std::cout << "Error\nCollage not added\n";
			return;
		}
		add(collage);
		return;
	}
}

R_Image* Session::find_image(const char*image_name)
{
	for (size_t i = 0; i < images_count; ++i)
		if (strcmp(images[i].get_file_name(), image_name) == 0)
			return &images[i];
	return nullptr;
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
			std::cout<<images[i].get_file_name();
			if(i!=images_count-1)
			std::cout << ",";
		}
		std::cout << std::endl;
	}
	return;
}
