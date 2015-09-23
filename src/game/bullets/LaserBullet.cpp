#include "LaserBullet.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <math.h> 
#endif

namespace violetland {

LaserBullet::LaserBullet(float x, float y, float dX, float dY) :
	Bullet(x, y, dX, dY, BULLET_LASER) {
	Speed = 3;
	Damage = 1;
	MaxRange = 1000;
	Alpha = 0.75f;
	m_hit = false;
}

void LaserBullet::process(int deltaTime) {
	X = startX - cos((Angle + 90) * M_PI / 180) * MaxRange;
	Y = startY - sin((Angle + 90) * M_PI / 180) * MaxRange;

	m_active = !m_hit && Alpha > 0.15f;

	m_readyToRemove = !m_active && Alpha == 0;

	Alpha -= 0.002f * deltaTime;
	if (Alpha < 0)
		Alpha = 0;
}

void LaserBullet::draw() {
	glDisable(GL_TEXTURE_2D);

	glLineWidth(4.0f);
#ifndef HAVE_GLES
	glBegin(GL_LINES);
	glColor4f(0.3f, 1.0f, 0.3f, Alpha / 3.0f);
	glVertex3f(X, Y, 0);
	glColor4f(0.3f, 1.0f, 0.3f, Alpha);
	glVertex3f(dStartX, dStartY, 0);
	glEnd();
#else
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	const GLfloat cols[] = {
		0.3f, 1.0f, 0.3f, Alpha / 3.0f,
		0.3f, 1.0f, 0.3f, Alpha
	};

	const GLfloat vtx[] = {
		X, Y, 0,
		dStartX, dStartY, 0
	};

	glColorPointer(4, GL_FLOAT, 0, cols);
	glVertexPointer(3, GL_FLOAT, 0, vtx);
	glDrawArrays(GL_LINES, 0, 2);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif

	glEnable(GL_TEXTURE_2D);
}

bool LaserBullet::checkHit(Object* objRef) {
	bool hit = m_active && objRef->detectCollide(startX, startY, X, Y);
	m_hit = m_hit || hit;
	return hit;
}

}
