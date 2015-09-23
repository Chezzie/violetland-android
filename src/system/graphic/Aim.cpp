#include "Aim.h"

#ifdef HAVE_GLES
struct AIM_LIST : GLES_LIST
{
	float cDarkR, cDarkG, cDarkB;
	float cLightR, cLightG, cLightB;
	GLfloat* circle;

	void draw()
	{
		glEnableClientState(GL_VERTEX_ARRAY);

		// circle 1
		glLineWidth(3.75f);
		glColor4f(cDarkR, cDarkG, cDarkB, 1.0f);
		glVertexPointer(3, GL_FLOAT, 0, circle);
		glDrawArrays(GL_LINE_LOOP, 0, 360 / 4 * 2);
	
		// circle 2
		glLineWidth(1.25f);
		glColor4f(cLightR, cLightG, cLightB, 1.0f);
		glDrawArrays(GL_LINE_LOOP, 0, 360 / 4 * 2);

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	~AIM_LIST()
	{
		delete circle;
	}
};

struct POINT_LIST : GLES_LIST
{
	float cDarkR, cDarkG, cDarkB;
	float cLightR, cLightG, cLightB;
	GLfloat* circle;
	GLfloat* filledCircle;

	void draw()
	{
		glEnableClientState(GL_VERTEX_ARRAY);

		// filled circle
		glLineWidth(1.25f);
		glColor4f(cLightR, cLightG, cLightB, 1.0f);
		glVertexPointer(3, GL_FLOAT, 0, filledCircle);
		glDrawArrays(GL_TRIANGLES, 0, 360 / 4 * 3);
	
		// circle
		glLineWidth(0.3f);
		glColor4f(cDarkR, cDarkG, cDarkB, 1.0f);
		glVertexPointer(3, GL_FLOAT, 0, circle);
		glDrawArrays(GL_LINE_LOOP, 0, 360 / 4 * 2);

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	~POINT_LIST()
	{
		delete filledCircle;
		delete circle;
	}
};
#endif

#ifndef HAVE_GLES
void Aim::constructCircle(float r) {
	glBegin(GL_LINES);
	for (int i = 0; i < 360; i += 4) {
		const float radS = i * M_PI / 180;
		const float radE = (i + 4) * M_PI / 180;
		glVertex3f(r * cos(radS), r * sin(radS), 0.0f);
		glVertex3f(r * cos(radE), r * sin(radE), 0.0f);
	}
	glEnd();
}

void Aim::constructFilledCircle(float r) {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 360; i += 4) {
		const float radS = i * M_PI / 180;
		const float radE = (i + 4) * M_PI / 180;
		glVertex3f(r * cos(radS), r * sin(radS), 0.0f);
		glVertex3f(r * cos(radE), r * sin(radE), 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
	}
	glEnd();
}
#else
GLfloat* Aim::constructCircle(float r) {
	GLfloat* circle = new GLfloat[360 / 4 * 2 * 3];
	GLfloat* p = circle;
	for (int i = 0; i < 360; i += 4) {
		const float radS = i * M_PI / 180;
		const float radE = (i + 4) * M_PI / 180;
		*p++ = r * cos(radS); *p++ = r * sin(radS); *p++ = 0.0f;
		*p++ = r * cos(radE); *p++ = r * sin(radE); *p++ = 0.0f;
	}
	return circle;
}

GLfloat* Aim::constructFilledCircle(float r) {
	GLfloat* circle = new GLfloat[360 / 4 * 3 * 3];
	GLfloat* p = circle;
	for (int i = 0; i < 360; i += 4) {
		const float radS = i * M_PI / 180;
		const float radE = (i + 4) * M_PI / 180;
		*p++ = r * cos(radS); *p++ = r * sin(radS); *p++ = 0.0f;
		*p++ = r * cos(radE); *p++ = r * sin(radE); *p++ = 0.0f;
		*p++ = 0.0f; *p++ = 0.0f; *p++ = 0.0f;
	}
	return circle;
}
#endif

void Aim::constructAim(float cDarkR, float cDarkG, float cDarkB,
					   float cLightR, float cLightG, float cLightB) {

#ifndef HAVE_GLES
	m_aimDListId = glGenLists(1);

	glNewList(m_aimDListId, GL_COMPILE);
	glLineWidth(3.75f);
	glColor4f(cDarkR, cDarkG, cDarkB, 1.0f);
	constructCircle(25.0f);

	glLineWidth(1.25f);
	glColor4f(cLightR, cLightG, cLightB, 1.0f);
	constructCircle(25.0f);
	glEndList();
#else
	AIM_LIST* pAimList = new AIM_LIST();
	pAimList->cDarkR = cDarkR;
	pAimList->cDarkG = cDarkG;
	pAimList->cDarkB = cDarkB;
	pAimList->cLightR = cLightR;
	pAimList->cLightG = cLightG;
	pAimList->cLightB = cLightB;
	pAimList->circle = constructCircle(25.0f);
	m_aimDListId = (GLuint)static_cast<GLES_LIST*>(pAimList);
#endif

#ifndef HAVE_GLES
	m_pointDListId = glGenLists(1);

	glNewList(m_pointDListId, GL_COMPILE);
	glColor4f(cLightR, cLightG, cLightB, 1.0f);
	constructFilledCircle(20.0f);
	glLineWidth(0.3f);
	glColor4f(cDarkR, cDarkG, cDarkB, 1.0f);
	constructCircle(20.0f);
	glEndList();
#else
	POINT_LIST* pPointList = new POINT_LIST();
	pPointList->cDarkR = cDarkR;
	pPointList->cDarkG = cDarkG;
	pPointList->cDarkB = cDarkB;
	pPointList->cLightR = cLightR;
	pPointList->cLightG = cLightG;
	pPointList->cLightB = cLightB;
	pPointList->filledCircle = constructFilledCircle(20.0f);
	pPointList->circle = constructCircle(20.0f);
	m_pointDListId = (GLuint)static_cast<GLES_LIST*>(pPointList);
#endif
}

Aim::Aim(float cDarkR, float cDarkG, float cDarkB, float cLightR,
		float cLightG, float cLightB) {
	constructAim(cDarkR, cDarkG, cDarkB, cLightR, cLightG, cLightB);
}

Aim::Aim(Configuration* config) {
	float cDarkR = ImageUtility::getColorChR(config->AimColorA);
	float cDarkG = ImageUtility::getColorChG(config->AimColorA);
	float cDarkB = ImageUtility::getColorChB(config->AimColorA);

	float cLightR = ImageUtility::getColorChR(config->AimColorB);
	float cLightG = ImageUtility::getColorChG(config->AimColorB);
	float cLightB = ImageUtility::getColorChB(config->AimColorB);

	constructAim(cDarkR, cDarkG, cDarkB, cLightR, cLightG, cLightB);
}

void Aim::draw(float x, float y, float scale, float pointScale) {
	glPushMatrix();

	glTranslatef(x, y, 0.0f);

	glPushMatrix();

	glScalef(scale, scale, scale);
	glCallList(m_aimDListId);

	glPopMatrix();

#if 0
	if (pointScale < 0)
		pointScale = 0;

	glScalef(pointScale, pointScale, pointScale);
	glCallList(m_pointDListId);
#else
	if (pointScale > 0)
	{
		glScalef(pointScale, pointScale, pointScale);
		glCallList(m_pointDListId);
	}
#endif

	glPopMatrix();
}

Aim::~Aim() {
	glDeleteLists(m_aimDListId, 1);
	glDeleteLists(m_pointDListId, 1);
}
