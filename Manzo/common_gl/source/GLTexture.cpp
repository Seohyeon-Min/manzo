#include "GLTexture.h"
#include "glCheck.h"
#include  <stb_image.h>

GLTexture::GLTexture(const std::filesystem::path& image_filepath, bool flip)
{
	if (!std::filesystem::exists(image_filepath)) {
		throw std::runtime_error{ "Image file does not exist: " + image_filepath.string() };
	}
	const bool loaded = LoadFromFileImage(image_filepath, flip);
	if (!loaded) {
		throw std::runtime_error{ "Failed to load the image " + image_filepath.string() };
	}
}

GLTexture::~GLTexture() noexcept
{
	delete_texture();
}

GLTexture::GLTexture(GLTexture&& other) noexcept
	: texture_handle(other.texture_handle),
	width(other.width),
	height(other.height),
	filtering(other.filtering),
	wrapping(other.wrapping)
{
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept
{
	if (this != &other) {
		// Swap the values with the temp object
		std::swap(texture_handle, other.texture_handle);
		std::swap(width, other.width);
		std::swap(height, other.height);
		std::swap(filtering, other.filtering);
		std::swap(wrapping, other.wrapping);
	}
	return *this;
}

bool GLTexture::LoadFromFileImage(const std::filesystem::path& image_filepath, bool flip_vertical) noexcept
{
	if (flip_vertical)
	{
		stbi_set_flip_vertically_on_load(true);
	}

	int channels;
	unsigned char* image_data = stbi_load(image_filepath.string().c_str(), &width, &height, &channels, 4);
	if (!image_data)
	{
		return false;
	}

	const RGBA* colors = reinterpret_cast<const RGBA*>(image_data);
	if (!LoadFromMemory(width, height, colors)) {
		stbi_image_free(image_data);
		return false;
	}
	stbi_image_free(image_data);

	return true;
}

bool GLTexture::LoadFromMemory(int image_width, int image_height, const RGBA* colors) noexcept
{
	// delete current texture
	// copy and save image width
	// copy and save image height
	if (texture_handle != 0) {
		glCheck(glDeleteTextures(1, &texture_handle));
	}
	width = image_width;
	height = image_height;

	// Create 1 texture
	glCheck(glCreateTextures(GL_TEXTURE_2D, 1, &texture_handle));
	// Create immutable storage of widthxheight RGBA8 GPU memory with only one texture level
	// Send `colors` data to GPU memory
	constexpr GLsizei ONE_TEXTURE_LEVEL = 1;
	glCheck(glTextureStorage2D(texture_handle, ONE_TEXTURE_LEVEL, GL_RGBA8, width, height));
	constexpr GLint   FIRST_LEVEL = 0;
	constexpr GLsizei OFFSET_X = 0, OFFSET_Y = 0;
	glCheck(glTextureSubImage2D(texture_handle, FIRST_LEVEL, OFFSET_X, OFFSET_Y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, colors));
	// Set Filtering to nearest pixel
	// Set Wrapping to the repeat pattern
	glCheck(glTextureParameteri(texture_handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	glCheck(glTextureParameteri(texture_handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	glCheck(glTextureParameteri(texture_handle, GL_TEXTURE_WRAP_S, GL_REPEAT));
	glCheck(glTextureParameteri(texture_handle, GL_TEXTURE_WRAP_T, GL_REPEAT));
	// return if successful
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////look one more
void GLTexture::UseForSlot([[maybe_unused]]unsigned int texture_unit) const noexcept
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
}

void GLTexture::SetFiltering([[maybe_unused]] Filtering how_to_filter) noexcept
{
	GLenum min_filter = GL_NEAREST; // 기본값
	GLenum mag_filter = GL_NEAREST; // 기본값

	switch (how_to_filter) {
	case Filtering::NearestPixel:
		min_filter = GL_NEAREST;
		mag_filter = GL_NEAREST;
		break;
	case Filtering::Linear:
		min_filter = GL_LINEAR;
		mag_filter = GL_LINEAR;
		break;
	}

	glCheck(glTextureParameteri(texture_handle, GL_TEXTURE_MIN_FILTER, min_filter));
	glCheck(glTextureParameteri(texture_handle, GL_TEXTURE_MAG_FILTER, mag_filter));
}

void GLTexture::SetWrapping([[maybe_unused]] Wrapping how_to_wrap, [[maybe_unused]] Coordinate coord) noexcept
{
	GLenum wrap_mode = GL_CLAMP_TO_BORDER; // 기본값

	switch (how_to_wrap) {
	case Wrapping::ClampToEdge:
		wrap_mode = GL_CLAMP_TO_EDGE;
		break;
	case Wrapping::MirroredRepeat:
		wrap_mode = GL_MIRRORED_REPEAT;
		break;
	case Wrapping::Repeat:
		wrap_mode = GL_REPEAT;
		break;
	}

	if (coord == Coordinate::S) {
		glCheck(glTextureParameteri(texture_handle, GL_TEXTURE_WRAP_S, wrap_mode));
	}
	else if (coord == Coordinate::T) {
		glCheck(glTextureParameteri(texture_handle, GL_TEXTURE_WRAP_T, wrap_mode));
	}
}

void GLTexture::delete_texture() noexcept
{
	// invoke glDeleteTextures
	texture_handle = 0; // so we don't accidentally double it
	width = 0;
	height = 0;
}
