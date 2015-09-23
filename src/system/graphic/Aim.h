#ifndef AIM_H_
#define AIM_H_

#include "StaticObject.h"
#include "../Configuration.h"

class Aim {
private:
	GLuint m_aimDListId;
	GLuint m_pointDListId;
#ifndef HAVE_GLES
	void constructCircle(float r);
	void constructFilledCircle(float r);
#else
	GLfloat* constructCircle(float r);
	GLfloat* constructFilledCircle(float r);
#endif
	void constructAim(float cDarkR,float cDarkG,float cDarkB,float cLightR,float cLightG,float cLightB);
public:
	Aim(Configuration* config);
	Aim(float cDarkR,float cDarkG,float cDarkB,float cLightR,float cLightG,float cLightB);
	void draw(float x, float y, float scale, float pointScale);
	virtual ~Aim();
};

#endif /* AIM_H_ */
