#include <iostream>
#include <string>
#include "Interface.h"

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
	if (sessions != nullptr)
	{
		for (size_t i = 0; i < other.sessions_counter; ++i)
		{
			sessions[i] = other.sessions[i];
		}
		current = &sessions[current_ID];
		sessions_counter = other.sessions_counter;
		current_ID = other.current_ID;
		ID_counter = other.ID_counter;
	}
	else
	{
		std::cout << "Error with memmory allocation(copy Interface)";
		return;
	}
		return;
}

bool Interface::load(std::string* words, size_t lenght,size_t &ID_counter)
{
	Session* sessions_temp = nullptr;
	sessions_temp = new(std::nothrow) Session[sessions_counter];
	if (sessions_temp != nullptr)
	{
		for (size_t i = 0; i < sessions_counter; ++i)
		{
			sessions_temp[i] = sessions[i];
		}
		delete[] sessions;
		sessions = new(std::nothrow) Session[sessions_counter + 1];
		if (sessions != nullptr)
		{
			for (size_t i = 0; i < sessions_counter; ++i)
			{
				sessions[i] = sessions_temp[i];
			}

			for (size_t i = 1; i < lenght; ++i)
			{
				if (sessions[sessions_counter].add(words[i]) == false)
					std::cout << "Error\nImage " << words[i] << " not added\n";
			}
			if (sessions[sessions_counter].is_default() == true)
			{
				delete[] sessions;
				sessions = sessions_temp;
				return false;
			}
			else
			{
				delete[] sessions_temp;
				current_ID = sessions_counter;
				current = &sessions[current_ID];
				ID_counter++;
				current->set_ID(ID_counter);
				sessions_counter += 1;
				
				return true;
			}
		}
		else
		{
			sessions = sessions_temp;
			return false;
		}
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
		//std::cout << "current->get_ID(): " << current->get_ID() << std::endl;
		Session* new_sessions;
		new_sessions = new(std::nothrow) Session[sessions_counter - 1];
		if (new_sessions != nullptr)
		{
			for (size_t i = 0; i < close_ID; ++i)
			{
				new_sessions[i] = sessions[i];
			}
			//here
			size_t new_session_ID = close_ID;
			for (size_t i = close_ID + 1; i < sessions_counter; ++i)
			{
				new_sessions[new_session_ID] = sessions[i];
				new_session_ID++;
			}
			
			delete[] sessions;
			sessions = new_sessions;
			sessions_counter--;
			current_ID = sessions_counter-1;
			//std::cout << "current->get_ID(): " << current->get_ID() << std::endl;
			//std::cout << "sessions[sessions_counter].get_ID(): " << sessions[sessions_counter-1].get_ID() << std::endl;
			current = &sessions[sessions_counter-1];
			//std::cout << "current->get_ID(): " << current->get_ID() << std::endl;

			return true;
		}
		else return false;
	}
}

void Interface::split_input(char* input, size_t lenght)
{
	size_t counter = 1;
	std::string* words;
	for (size_t i = 0; i < lenght; ++i)
	{
		if (input[i] == ' ')
		{
			counter++;
			while (input[i] == ' ')
				i++;
		}
	}
	words = new (std::nothrow) std::string[counter];
	if (words != nullptr)
	{
		size_t current = 0;
		for (size_t i = 0; i < counter; ++i)
		{
			for (size_t j = current; j < lenght; ++j)
			{
				if (input[j] == ' ')
				{
					while (input[j] == ' ')
					{
						j++;
					}
					current = j;
					break;
				}
				words[i] = words[i] + input[j];

			}
		}
		detect_function(words, counter);
		delete[] words;
	}
	else
	{
		std::cout << "Error with memory allocation" << std::endl;
	}
	return;

}

void Interface::detect_function(std::string* words, size_t lenght)
{
	if (words[0] == "load")
	{
		if (load(words, lenght,ID_counter) == true)
		{
			std::cout << "Session with ID:" << current->get_ID() << " started\n";
		}
		else
		{
			std::cout << "Error with memory allocation\nDid not start session\n";
		}
		return;
	}
	if (words[0] == "close")
	{
		std::cout << "Closing sessions with ID:" << current->get_ID() << "\n";
		//std::cout << "current_ID:" << current_ID<<std::endl;
		if(close(current_ID)==true)
		{ 
		if(current!=nullptr)
		{ 
		
		std::cout<< "Current acitve one is:" << current->get_ID()<< std::endl;
		}
		}
		else
		{
			std::cout << "Could not close session:" << current_ID << std::endl;
		}
		return;
	}
	if (words[0] == "add")
	{
		if(current!=nullptr)
		{ 
		for(size_t i=1;i<lenght;++i)
		{ 
			if (current->add(words[i]) == true)
			{
				std::cout << "Image " << words[i] << " added\n";
			}
			else
			{
				std::cout << "Coutld not add image " << words[i] << std::endl;
			}
		}
		}
		else
		{
			std::cout << "No active current session\nSwitch to an active session\n";
		}
		return;
	}
	if (words[0] == "save" && words[1] != "as")
	{

	}
	if (words[0] == "save" && words[1] == "as")
	{

	}	
	if (words[0] == "help")
	{

	}
	if (words[0] == "switch")
	{
		//std::cout << "sessions_counter" << sessions_counter << std::endl;
		size_t id = std::stoi(words[1]);
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
	if (words[0] == "session" && words[1] == "info")
	{
		if (current != nullptr)
		{
			std::cout << "Current session ID:" << current->get_ID() << std::endl;
			current->print_images_file_names();
			current->print_changes();
			return;
		}
		else
			std::cout << "No active session\n";
		return;
	}


	if (words[0] == "grayscale")
	{
	}
	if (words[0] == "monochrome")
	{
	}
	if (words[0] == "negative")
	{
	}
	if (words[0] == "rotate")
	{
		
		if (words[1] == "right")
		{
			
		}

		if (words[1] == "left")
		{
			
		}
		
	}
	if (words[0] == "collage")
	{

	}
	if (words[0] == "undo")
	{
		current->undo();
	}
	if(words[0]!="exit"||lenght>1)
	std::cout << "Invalid input\n";

}
