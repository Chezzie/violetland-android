#pragma once

#ifdef WIN32
	#ifdef HAVE_GLES
		#include <GLES/gl.h>
		// #include <glues.h>
	#else
		#include <SDL_opengl.h>
		#include <GL/glu.h>
	#endif
#else
	#include <SDL_opengles.h>
	// #include <glues.h>
#endif

#if defined(HAVE_GLES)
#define GLdouble     GLfloat
#ifdef GL_CLAMP
#undef GL_CLAMP
#endif
#define GL_CLAMP     GL_CLAMP_TO_EDGE
#define glClearDepth glClearDepthf
#define glOrtho      glOrthof
#define glFrustum    glFrustumf

#define glColor4fv(a) glColor4f(a[0], a[1], a[2], a[3])
#define glColor3fv(a) glColor4f(a[0], a[1], a[2], 1.0f)
#define glColor3f(a,b,c) glColor4f(a, b, c, 1.0f)

struct GLES_LIST {
	virtual void draw() = 0;
	virtual ~GLES_LIST() {}
};

#define glDeleteLists(/*GLuint*/ p, /*int*/ range) \
{ \
	SDL_assert(range == 1); \
	delete reinterpret_cast<GLES_LIST*>(p); \
}

#define glCallList(/*GLuint*/ p) \
{ \
	reinterpret_cast<GLES_LIST*>(p)->draw(); \
}

template<typename V>
inline bool is_power_of_2(V v)
{
	return v > 0 && !(v & (v-1));
}

#endif
