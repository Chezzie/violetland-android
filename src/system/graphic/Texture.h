#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../../system/utility/ImageUtility.h"

class Texture {
private:
	GLuint m_textureId;
	const GLenum m_type;
	int m_width, m_height;
	bool m_npot;
	GLfloat m_texRight, m_texBottom;
public:
	// Texture(int width, int height, GLenum type, GLint filter);
	Texture(SDL_Surface *surface, GLenum type, GLint filter,
			bool takeCareOfSurface);
	const int getWidth() const {
		return m_width;
	}
	const int getHeight() const {
		return m_height;
	}
	const GLuint getTextureId() const {
		return m_textureId;
	}
	const GLenum getType() const {
		return m_type;
	}
	const bool isNPOT() const {
		return m_npot;
	}
	const GLfloat getTexRight() const {
		return m_texRight;
	}
	const GLfloat getTexBottom() const {
		return m_texBottom;
	}

	virtual ~Texture();
};

#endif /* TEXTURE_H_ */
