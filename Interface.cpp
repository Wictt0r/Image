#include <iostream>
#include "Interface.h"
#pragma warning(disable:4996)

Interface::Interface(const Interface& other)
{
	copy(other);
}

Interface::~Interface()
{
	del();
}

Interface& Interface::operator=(const Interface& other)
{
	if (this != &other)
	{
		del();
		copy(other);
	}
	return *this;
}

void Interface::del()
{
	delete[] sessions;
}

void Interface::copy(const Interface& other)
{
	sessions = new(std::nothrow)Session[other.sessions_counter];
	if (sessions == nullptr)
	{
		std::cout << "Error with memmory allocation";
		return;
	}
		for (size_t i = 0; i < other.sessions_counter; ++i)
		{
			sessions[i] = other.sessions[i];
		}
		current = &sessions[current_ID];
		sessions_counter = other.sessions_counter;
		current_ID = other.current_ID;
		ID_counter = other.ID_counter;
		return;
}

bool Interface::load(char** words, size_t lenght,size_t &ID_counter)
{
	Session* sessions_temp = new(std::nothrow) Session[sessions_counter+1];
	if (sessions_temp == nullptr)
	{
		return false;
	}
		for (size_t i = 0; i < sessions_counter; ++i)
		{
			sessions_temp[i] = sessions[i];
			if (sessions_temp[i].is_default() == true)
			{
				delete[] sessions_temp;
				return false;
			}
		}
			for (size_t i = 1; i < lenght; ++i)
			{
				if (sessions_temp[sessions_counter].add(words[i]) == false)
					std::cout << "Error\nImage " << words[i] << " not added\n";
			}
			if (sessions_temp[sessions_counter].is_default() == false)
			{
				delete[] sessions;
				sessions = sessions_temp;
				current_ID = sessions_counter;
				current = &sessions[current_ID];
				ID_counter++;
				current->set_ID(ID_counter);
				sessions_counter += 1;

				return true;
			}
			else
			{
				return false;
			}
}

bool Interface::close(const size_t close_ID)
{
	if (sessions_counter == 1)
	{
		sessions[current_ID].del();
		sessions_counter = 0;	
		sessions = nullptr;
		current = nullptr;
		current_ID = 0;
		return true;
	}
	else
	{
		Session* new_sessions = new(std::nothrow) Session[sessions_counter - 1];
		if (new_sessions == nullptr)
		{
			return false;
		}
		size_t new_session_ID = 0;
			for (size_t i = 0 ; i < sessions_counter; ++i)
			{
				if (i == close_ID)
					continue;
				new_sessions[new_session_ID] = sessions[i];
				new_session_ID++;
			}
			
			delete[] sessions;
			sessions = new_sessions;
			sessions_counter--;
			current_ID = sessions_counter-1;
			current = &sessions[sessions_counter-1];

			return true;
		
	}
}

void Interface::split_input(char* input, size_t lenght)
{
	size_t counter = 1;
	for (size_t i = 0; i < lenght; ++i)
	{
		if (input[i] == ' ')
		{
			counter++;
			while (input[i] == ' ')
				i++;
		}
	}
	char** split_input= new (std::nothrow) char*[counter];
	if (split_input == nullptr)
	{
		std::cout << "Error with memory allocation" << std::endl;
		return;
	}
	size_t split_input_counter = 0;
	char* token = strtok(input, " ");
	split_input[split_input_counter]= new(std::nothrow) char[strlen(token) + 1];
	if (split_input[split_input_counter] == nullptr)
	{
		std::cout << "Error\n";
	}
	strcpy(split_input[split_input_counter], token);
	++split_input_counter;
	while (token != nullptr)
	{
		token = strtok(nullptr, " ");
		if (token != nullptr)
		{
			split_input[split_input_counter] = new(std::nothrow) char[strlen(token) + 1];
			if (split_input[split_input_counter] == nullptr)
			{
				std::cout << "Error\n";
				for (size_t i = 0; i <= split_input_counter; ++i)
					delete[] split_input[i];
				delete [] split_input;
				return;
			}
			strcpy(split_input[split_input_counter], token);
			++split_input_counter;
		}
	}
		detect_function(split_input, counter);
		for (size_t i = 0; i < split_input_counter; ++i)
			delete[] split_input[i];
		delete[] split_input;
		return;
}

void Interface::detect_function(char** split_input, size_t lenght)
{
	if (strcmp(split_input[0], "load")==0)
	{
		if (load(split_input, lenght,ID_counter) == true)
		{
			std::cout << "Session with ID:" << current->get_ID() << " started\n";
		}
		else
		{
			std::cout << "Error with memory allocation\nDid not start session\n";
		}
		return;
	}
	if (strcmp(split_input[0], "close") == 0)
	{
		std::cout << "Closing sessions with ID:" << current->get_ID() << "\n";
		if(close(current_ID)==true)
		{ 
			if (current == nullptr)
			{
				std::cout << "No active sessions\n";
				return;
			}
		else
		{
			std::cout << "Current acitve session is:" << current->get_ID() << std::endl;
		}
		}
		else
		{
			std::cout << "Could not close session:" << current_ID << std::endl;
		}
		return;
	}
	if (strcmp(split_input[0], "add") == 0)
	{
		if (current == nullptr)
		{
			std::cout << "No active sessions\n";
			return;
		}
		for(size_t i=1;i<lenght;++i)
		{ 
			if (current->add(split_input[i]) == false)
			{
				std::cout << "Coutld not add image "<< std::endl;
			}
		}
		
		return;
	}
	if (strcmp(split_input[0], "save") == 0 && strcmp(split_input[1], "as") == 0)
	{
		if (current == nullptr)
		{
			std::cout << "No active sessions\n";
			return;
		}
		current->save_as_all();
		return;
	}
	if (strcmp(split_input[0], "save") == 0 && lenght==1)
	{
		if (current == nullptr)
		{
			std::cout << "No active sessions\n";
			return;
		}
		current->save_all();
		return;
	}	
	if (strcmp(split_input[0], "help") == 0)
	{
		help();
	}
	if (strcmp(split_input[0], "switch") == 0)
	{
		size_t id = std::stoi(split_input[1]);
		for (size_t i = 0; i < sessions_counter; ++i)
		{
			if (sessions[i].get_ID()==id)
			{
				current = &sessions[i];
				current_ID = i;
				std::cout << "You switched to session with ID:" << current->get_ID()<< std::endl;
				current->print_images_file_names();
				current->print_changes();
				return;
			}
		}
		std::cout << "The session does not exist or is closed\n";
		return;
	}
	if (strcmp(split_input[0], "session") == 0 && strcmp(split_input[1], "info") == 0)
	{
		if (current != nullptr)
		{
			std::cout << "Current session ID:" << current->get_ID() << std::endl;
			current->print_images_file_names();
			current->print_changes();
			return;
		}
		else
			std::cout << "No active session with that ID\n";
		return;
	}
	if (strcmp(split_input[0], "grayscale") == 0)
	{
		if (current == nullptr)
		{
			std::cout << "No active sessions\n";
			return;
		}
		if (current->apply_to_all(&R_Image::grayscale) == true)
		{
			if (current->add_changes("grayscale") == false)
			{
				std::cout << "Error\nNo changes made\n";
				current->undo();
				return;
			}
				std::cout << "Images grayscaled\n";
		}
		else
		{
			std::cout << "Error\nNo changes made\n";
		}
		return;
	}
	if (strcmp(split_input[0], "monochrome") == 0)
	{
		if (current == nullptr)
		{
			std::cout << "No active sessions\n";
			return;
		}
		if (current->apply_to_all(&R_Image::monochrome) == true)
		{
			if (current->add_changes("monochrome") == false)
			{
				std::cout << "Error\nNo changes made\n";
				current->undo();
				return;
			}
			std::cout << "Images monochromed\n";
		}
		else
		{
			std::cout << "Error\nNo changes made\n";
		}
		return;
	}
	if (strcmp(split_input[0], "negative") == 0)
	{
		if (current == nullptr)
		{
			std::cout << "No active sessions\n";
			return;
		}
		if (current->apply_to_all(&R_Image::negative) == true)
		{
			if (current->add_changes("negative") == false)
			{
				std::cout << "Error\nNo changes made\n";
				current->undo();
				return;
			}
			std::cout << "Images converted to negatives\n";
		}
		else
		{
			std::cout << "Error\nNo changes made\n";
		}
		return;
	}
	if (strcmp(split_input[0], "rotate") == 0)
	{
		if (current == nullptr)
		{
			std::cout << "No active sessions\n";
			return;
		}
		if (strcmp(split_input[1], "right") == 0)
		{
			if (current->apply_to_all(&R_Image::rotate_right)==true)
			{
				if (current->add_changes("rotate right") == false)
				{
					std::cout << "Error\nNo changes made\n";
					current->undo();
					return;
				}
				std::cout << "Images rotated right\n";	
			}
			else
			{
				std::cout << "Error\nDid not rotate images\n";
			}
			return;
		}
		if (strcmp(split_input[1], "left") == 0)
		{
			if (current->apply_to_all(&R_Image::rotate_left) == true)
			{
				if (current->add_changes("rotate left") == false)
				{
					std::cout << "Error\nNo changes made\n";
					current->undo();
					return;
				}
				std::cout << "Images rotated left\n";
			}
			else
			{
				std::cout << "Error\nDid not rotate images\n";
			}
			return;
		}
		
	}
	if (strcmp(split_input[0], "collage") == 0)
	{
		if (current == nullptr)
		{
			std::cout << "No active sessions\n";
			return;
		}
		current->collage(split_input[1],split_input[2], split_input[3], split_input[4]);
	}
	if (strcmp(split_input[0], "undo") == 0)
	{
		if (current == nullptr)
		{
			std::cout << "No active sessions\n";
			return;
		}
		current->undo();
		return;
	}
	if(strcmp(split_input[0], "exit") != 0 ||lenght>1)
	std::cout << "Invalid input\n";
}

void Interface::help()
{
	std::cout << "The following commands are supported:\n";
	std::cout << "load <image>: loads a new session with <image>\n";
	std::cout << "add <image>: adds <image> to current session\n";
	std::cout << "close: closes current session and switches to last one if possible\n";
	std::cout << "save: saves all images in the current session with the names given\n";
	std::cout << "save as: saves all images as the name given + _new\n";
	std::cout << "help: prints this\n";
	std::cout << "exit: exits the program\n";
	std::cout << "grayscale: grayscales all images in current session\n";
	std::cout << "monochrome: monochromes all images in current session\n";
	std::cout << "negative: convers all images in current session to negatives\n";
	std::cout << "rotate <direction>: rotates all images in current session in the give directino(left/right)\n";
	std::cout << "undo: undoes last change in current session\n";
	std::cout << "session info: prints all images and changes in current session\n";
	std::cout << "switch<session ID>: switches the session with ID <session ID>\n";
	std::cout << "collage<vertical/horizontal> <image1> <image2> <collage name>: adds a collage with <image1> and <image2> to current session named <collage name>\n";

}
