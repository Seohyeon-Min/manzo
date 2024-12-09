#pragma once

#include <ft2build.h>
#include <freetype.h>

#include <iostream>
#include <map>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vec2.h"
#include "vec3.h"
#include <filesystem>
#include <GLBuffers.h>

// FreeType

/// Holds all state information relevant to a character as loaded using FreeType
struct Character
{
	ivec2   Size;      // Size of glyph (width & height)
	ivec2   Bearing;   // Offset from baseline to left/top of glyph

	// note that advance is number of 1/64 pixels)
	// (ch.Advance >> 6) // bitshift by 6 to get value in pixels (2^6 = 64)
	unsigned int Advance;   // Horizontal offset to advance to next glyph

	vec2 top_left; // location of this character in the atlas - top left [0,0]
	vec2 bot_right; // location of this character in the atlas - bot right [1,1]
};

class font_atlas
{
public:
	unsigned int textureID;
	unsigned int TextureWidth; // Total width of the atlas
	unsigned int TextureHeight; // Total height of the atlas
	std::map<char, Character> ch_atlas;

	font_atlas();
	~font_atlas();
	void create_atlas(const char* file_path); // Function to create the atlas
	void Bind_atlas();
	void UnBind_atlas();
};

struct label_text
{
	// Store the individual label
	std::string label;
	vec2 label_loc;
	float label_angle;
	float label_size; 
	vec3 label_color;
};

class label_text_store
{
	// Stores all the labels
public:
	font_atlas main_font;
	unsigned int total_char_count = 0;
	gBuffers label_buffers;
	std::vector<label_text> labels;

	label_text_store();
	~label_text_store();
	void init(const char* file_path);
	void add_text(std::string text, vec2 text_loc, float font_angle, float font_size, vec3 label_color);
	void set_buffers();
	void paint_text();
private:
	void get_buffer(label_text& label, float* vertices, unsigned int& vertex_index, unsigned int* indices, unsigned int& indices_index);
	vec2 rotate_pt(vec2& rotate_about, vec2 pt, float& rotation_angle);

};
