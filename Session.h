#pragma once
#include"R_Image.h"
#include<string.h>

class Session 
{
public:
	Session();
	Session(const Session&);
	~Session();

	Session& operator=(const Session&);
	
	bool add(const std::string);
	void undo();
	bool is_default()const;
	size_t get_ID()const { return ID; }
	bool add_changes(const std::string&);
	bool apply_to_all(std::string*,size_t);
	/*size_t get_images_count() { return images_count; }
	R_Image* get_images() { return images; }
	R_Image* get_previous() { return previous; }
	void set_previous(R_Image* other) { previous = other; }*/
	void set_ID(size_t id) { ID = id; return; }
	void print_changes()const;
	void print_images_file_names()const;
	void del();
	void copy(const Session&);
	//void print();
private:
	R_Image* images;
	size_t images_count;
	R_Image* previous;
	size_t ID,changes_count;
	std::string* changes;


	



};

