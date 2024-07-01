#include <assert.h>
#include <SDL_image.h>
#include "Texture.h"
#include "Utility.h"

//======================================================================================================
Texture::Texture(const std::string& filename)
{
	SDL_Surface* textureData = IMG_Load((filename).c_str());

	if (!textureData)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error loading texture file \"" + (filename)+"\"\n\n"
			"Possible causes could be a corrupt or missing file. Another reason could be "
			"that the filename and/or path are incorrectly spelt.", Utility::Severity::Failure);
	}

	auto width = textureData->w;
	auto height = textureData->h;
	auto pixels = reinterpret_cast<Uint8*>(textureData->pixels);
	auto depth = textureData->format->BytesPerPixel;
	auto format = ((depth == 4) ? GL_RGBA : GL_RGB);

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(textureData);
}
//======================================================================================================
Texture::~Texture()
{
	//We need to properly delete the textures
	//glDeleteTextures(1, &m_ID);
}
//======================================================================================================
void Texture::Bind() const
{
	assert(ID > 0);
	glBindTexture(GL_TEXTURE_2D, ID);
}
//======================================================================================================
void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}