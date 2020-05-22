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
	
	bool add(const char*);
	bool add(const R_Image&);
	void undo();
	bool create_previous();
	bool is_default()const;
	size_t get_ID()const { return ID; }
	bool add_changes(const char*);
	bool apply_to_all(bool (R_Image::*)());
	void save_all();
	void save_as_all();
	void collage(char*,char*,char*,char*);
	R_Image* find_image(const char*);
	void set_ID(size_t id) { ID = id; }
	void print_changes()const;
	void print_images_file_names()const;
	void del();
	void copy(const Session&);
private:
	R_Image* images;
	size_t images_count;
	R_Image* previous;
	size_t ID,changes_count;
	char** changes;
};