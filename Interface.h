#pragma once
#include<iostream>
#include"Session.h"
#include"R_Image.h"

class Interface 
{
public:
	Interface() :sessions(nullptr),sessions_counter(0),current_ID(1),current(nullptr),ID_counter(0){};
	Interface(const Interface&);
	~Interface();

	Interface& operator= (const Interface&);


	void del();
	void copy(const Interface&);
	
	void detect_function(std::string*, size_t);
	

private:
	Session* sessions;
	Session* current;
	size_t sessions_counter ,current_ID,ID_counter;

	bool load (std::string*, size_t,size_t&);
	bool close(const size_t);
};

