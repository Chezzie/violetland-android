#include "Texture.h"
#include <JNIBridge.h>

/* Texture::Texture(int width, int height, GLenum type, GLint filter)
	: m_type(type)
{
	m_width = width;
	m_height = height;

	char* colorBits = new char[width * height * 3];
	memset(colorBits, 0, width * height * 3);

	glGenTextures(1, &m_textureId);
	glBindTexture(m_type, m_textureId);

	glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, filter);

	glTexImage2D(m_type, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
			colorBits);

	delete[] colorBits;
} */

Texture::Texture(SDL_Surface *surface, GLenum type, GLint filter, bool takeCareOfSurface)
	: m_type(type)
{
	m_width = surface->w;
	m_height = surface->h;
	m_npot = !is_power_of_2(m_width) || !is_power_of_2(m_height);
	m_npot |= surface->format->Rmask != 0x000000ff; // nasty hack

	int p2width, p2height;
	if (m_npot)
	{
		p2width = 1;
		while (p2width < m_width) p2width <<= 1;
		p2height = 1;
		while (p2height < m_height) p2height <<= 1;
		m_texRight = GLfloat(m_width) / p2width;
		m_texBottom = GLfloat(m_height) / p2height;
		SDL_Surface* potSurface = SDL_CreateRGBSurface(0, p2width, p2height, surface->format->BitsPerPixel, 
			#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                0xff000000,
                0x00ff0000,
                0x0000ff00,
                0x000000ff
            #else
                0x000000ff,
                0x0000ff00,
                0x00ff0000,
                0xff000000
            #endif
		);

		if (surface->format->BytesPerPixel == 3)
		{
			SDL_Rect npot_r = { 0, 0, m_width, m_height };
			SDL_BlitSurface(surface, &npot_r, potSurface, &npot_r);
		}
		else
		{
			for (int y = 0; y < m_height; ++y) {
				memcpy(
					(char*)potSurface->pixels + y*potSurface->pitch,
					(char*)surface->pixels + y*surface->pitch,
					4 * m_width);
			}
		}

		if (surface && takeCareOfSurface) {
			SDL_FreeSurface(surface);
		}
		surface = potSurface;
	}
	else
	{
		p2width = m_width;
		p2height = m_height;
		m_texRight = 1.0f;
		m_texBottom = 1.0f;
	}

	GLint nOfColors;
	GLenum texture_format;

	bool colorsReverted = false;
	nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4) {
		texture_format = GL_RGBA;
	} else if (nOfColors == 3) {
		texture_format = GL_RGB;
	} else {
		std::cout << "Couldn't create GL texture from the SDL surface!" << std::endl;
		throw 1;
	}

	glGenTextures(1, &m_textureId);
	CHECK_GL_ERROR();
	glBindTexture(m_type, m_textureId);
	CHECK_GL_ERROR();

	glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, filter);
	CHECK_GL_ERROR();
	glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, filter);
	CHECK_GL_ERROR();

	/*glTexImage2D(m_type, 0, nOfColors, surface->w, surface->h, 0,
			texture_format, GL_UNSIGNED_BYTE, surface->pixels);*/
	glTexImage2D(m_type, 0, texture_format, p2width, p2height, 0,
		texture_format, GL_UNSIGNED_BYTE, surface->pixels);
	CHECK_GL_ERROR();

	if (m_npot || colorsReverted) {
		SDL_FreeSurface(surface);
	}
	else if (surface && takeCareOfSurface) {
		SDL_FreeSurface(surface);
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &m_textureId);
}
