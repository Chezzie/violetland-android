#include "StaticObject.h"

StaticObject::StaticObject(float x, float y, int w, int h, Texture* texture,
		bool takeCareOfTexture) :
	Object(x, y, w, h) {
	m_texture = texture;
	m_takeCareOfTexture = takeCareOfTexture;
}

#ifdef HAVE_GLES
struct COMPLEX_FACE_LIST : GLES_LIST
{
	float RMask, GMask, BMask, AMask;
	GLfloat* vtx;
	GLfloat* tex;
	GLsizei length;

	void draw()
	{
		glNormal3f(0.0f, 0.0f, 1.0f);
		glColor4f(RMask, GMask, BMask, AMask);
	
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 
		glVertexPointer(3, GL_FLOAT, 0, vtx);
		glTexCoordPointer(2, GL_FLOAT, 0, tex);
		glDrawArrays(GL_TRIANGLES, 0, length);
 
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	~COMPLEX_FACE_LIST()
	{
		delete vtx;
		delete tex;
	}
};
#endif

GLuint StaticObject::createComplexFace(int facesCount) {
#ifndef HAVE_GLES
	GLuint dListId = glGenLists(1);

	glNewList(dListId, GL_COMPILE);
	float faceWidth = (float) m_width / facesCount;
	float faceHeight = (float) m_height / facesCount;
	glBegin( GL_QUADS);
	for (int i = 0; i < facesCount; i++) {
		for (int j = 0; j < facesCount; j++) {
			float faceStartX = (float) i / facesCount;
			float faceStartY = (float) j / facesCount;
			float faceEndX = (float) (i + 1) / facesCount;
			float faceEndY = (float) (j + 1) / facesCount;

			glNormal3f(0.0f, 0.0f, 1.0f);
			glColor4f(RMask, GMask, BMask, AMask);

			glTexCoord2f(faceStartX, faceStartY);
			glVertex3f(m_left + faceWidth * i, m_top + faceHeight * j, 0);
			glTexCoord2f(faceEndX, faceStartY);
			glVertex3f(m_left + faceWidth * (i + 1), m_top + faceHeight * j, 0);
			glTexCoord2f(faceEndX, faceEndY);
			glVertex3f(m_left + faceWidth * (i + 1), m_top + faceHeight * (j + 1), 0);
			glTexCoord2f(faceStartX, faceEndY);
			glVertex3f(m_left + faceWidth * i, m_top + faceHeight * (j + 1), 0);
		}
	}
	glEnd();
	glEndList();
#else
	COMPLEX_FACE_LIST* pList = new COMPLEX_FACE_LIST();
	pList->RMask = RMask;
	pList->GMask = GMask;
	pList->BMask = BMask;
	pList->AMask = AMask;
	pList->vtx = new GLfloat[6 * 3 * facesCount * facesCount];
	pList->tex = new GLfloat[6 * 2 * facesCount * facesCount];
	pList->length = 6 * facesCount * facesCount;
	float faceWidth = (float) m_width / facesCount;
	float faceHeight = (float) m_height / facesCount;
	GLfloat* vtx = pList->vtx;
	GLfloat* tex = pList->tex;
	for (int i = 0; i < facesCount; i++) {
		for (int j = 0; j < facesCount; j++) {
			float faceStartX = (float) i / facesCount;
			float faceStartY = (float) j / facesCount;
			float faceEndX = (float) (i + 1) / facesCount;
			float faceEndY = (float) (j + 1) / facesCount;
			*tex++ = faceStartX; *tex++ = faceStartY;
			*vtx++ = m_left + faceWidth * i; *vtx++ = m_top + faceHeight * j; *vtx++ = 0;
			*tex++ = faceEndX; *tex++ = faceStartY;
			*vtx++ = m_left + faceWidth * (i + 1); *vtx++ = m_top + faceHeight * j; *vtx++ = 0;
			*tex++ = faceEndX; *tex++ = faceEndY;
			*vtx++ = m_left + faceWidth * (i + 1); *vtx++ = m_top + faceHeight * (j + 1); *vtx++ = 0;

			*tex++ = faceStartX; *tex++ = faceStartY;
			*vtx++ = m_left + faceWidth * i; *vtx++ = m_top + faceHeight * j; *vtx++ = 0;
			*tex++ = faceStartX; *tex++ = faceEndY;
			*vtx++ = m_left + faceWidth * i; *vtx++ = m_top + faceHeight * (j + 1); *vtx++ = 0;
			*tex++ = faceEndX; *tex++ = faceEndY;
			*vtx++ = m_left + faceWidth * (i + 1); *vtx++ = m_top + faceHeight * (j + 1); *vtx++ = 0;
		}
	}

	GLuint dListId = (GLuint)static_cast<GLES_LIST*>(pList);
#endif

	return dListId;
}

void StaticObject::draw(bool hreflect, bool vreflect, float x, float y,
		float angle, float scale) {
	GLfloat right = 1;
	GLfloat bottom = 1;

	// if (m_texture->getType() == 0x84F5) {
	//	glEnable(0x84F5); //GL_TEXTURE_RECTANGLE_NV = GL_TEXTURE_RECTANGLE_ARB = 0x84F5
	//	right = m_texture->getWidth();
	//	bottom = m_texture->getHeight();
	// }

	glBindTexture(m_texture->getType(), m_texture->getTextureId());

	glPushMatrix();

	glTranslatef(x, y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, scale);

#ifndef HAVE_GLES
	glBegin( GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);

	glColor4f(RMask, GMask, BMask, AMask);

	glTexCoord2f(hreflect ? right : 0, vreflect ? bottom : 0);
	glVertex3f(m_left, m_top, 0);
	glTexCoord2f(hreflect ? 0 : right, vreflect ? bottom : 0);
	glVertex3f(m_right, m_top, 0);
	glTexCoord2f(hreflect ? 0 : right, vreflect ? 0 : bottom);
	glVertex3f(m_right, m_bottom, 0);
	glTexCoord2f(hreflect ? right : 0, vreflect ? 0 : bottom);
	glVertex3f(m_left, m_bottom, 0);
	glEnd();
#else
	glNormal3f(0.0f, 0.0f, 1.0f);
	glColor4f(RMask, GMask, BMask, AMask);

	const GLfloat vtx1[] = {
		m_left, m_top, 0,
		m_right, m_top, 0,
		m_right, m_bottom, 0,
		m_left, m_bottom, 0
	};

	const GLfloat tex1[] = {
		hreflect ? right : 0, vreflect ? bottom : 0,
		hreflect ? 0 : right, vreflect ? bottom : 0,
		hreflect ? 0 : right, vreflect ? 0 : bottom,
		hreflect ? right : 0, vreflect ? 0 : bottom
	};

 
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 
    glVertexPointer(3, GL_FLOAT, 0, vtx1);
    glTexCoordPointer(2, GL_FLOAT, 0, tex1);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
 
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

	glPopMatrix();

	/* if (m_texture->getType() == 0x84F5)
		glDisable(0x84F5); //GL_TEXTURE_RECTANGLE_NV = GL_TEXTURE_RECTANGLE_ARB = 0x84F5 */
}

void StaticObject::draw(bool hreflect, bool vreflect) {
	draw(hreflect, vreflect, X, Y, Angle, Scale);
}

void StaticObject::draw(GLuint dListId) {
	glBindTexture(m_texture->getType(), m_texture->getTextureId());

	glPushMatrix();

	glTranslatef(X, Y, 0.0f);
	glRotatef(Angle, 0.0f, 0.0f, 1.0f);
	glScalef(Scale, Scale, Scale);

	glCallList(dListId);

	glPopMatrix();
}

void StaticObject::setTexture(Texture* texture, bool takeCareOfTexture) {
	if (m_takeCareOfTexture)
		delete m_texture;

	m_texture = texture;
	m_takeCareOfTexture = takeCareOfTexture;
}

StaticObject::~StaticObject() {
	if (m_takeCareOfTexture)
		delete m_texture;
}
