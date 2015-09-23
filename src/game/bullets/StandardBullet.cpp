#include "StandardBullet.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <math.h> 
#endif

namespace violetland {

StandardBullet::StandardBullet(float x, float y, float dX, float dY,
		bool explosive) :
	Bullet(x, y, dX, dY, BULLET_STANDARD) {
	Speed = 3;
	Damage = 1;
	MaxRange = 1000;
	Alpha = 1.0f;
	prevX = x;
	prevY = y;
	m_explosive = explosive;
}

bool StandardBullet::isExplosive() {
	return m_explosive;
}

void StandardBullet::process(int deltaTime) {
	if (m_active) {
		prevX = X;
		prevY = Y;
		X -= cos((Angle + 90) * M_PI / 180) * deltaTime * Speed;
		Y -= sin((Angle + 90) * M_PI / 180) * deltaTime * Speed;
	}

	m_range += Speed * deltaTime;
	Alpha = MaxRange - m_range;
	if (Alpha <= 0) {
		Alpha = 0;
		m_active = 0;
		m_readyToRemove = 1;
	} else {
		Alpha /= MaxRange;
		m_readyToRemove = 0;
	}
}

void StandardBullet::draw() {

	glDisable(GL_TEXTURE_2D);
	glLineWidth(BigCalibre || Penetrating ? 7.5f : 2.5f);

#ifndef HAVE_GLES
	glBegin(GL_LINES);
	glColor4f(1.0f, Penetrating ? 0.5f : 1.0f, Penetrating ? 0.2f : 1.0f, Alpha);
	glVertex3f(X, Y, 0);
	glColor4f(1.0f, Penetrating ? 0.5f : 1.0f, Penetrating ? 0.2f : 1.0f, 0.0f);
	glVertex3f(dStartX, dStartY, 0);
	glEnd();
#else
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	const GLfloat cols[] = {
		1.0f, Penetrating ? 0.5f : 1.0f, Penetrating ? 0.2f : 1.0f, Alpha,
		1.0f, Penetrating ? 0.5f : 1.0f, Penetrating ? 0.2f : 1.0f, 0.0f
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

bool StandardBullet::checkHit(Object* objRef) {
	float hx, hy;
	if (m_active && objRef->detectCollide(prevX, prevY, X, Y, &hx, &hy)) {

		X = hx;
		Y = hy;

		if (!(BigCalibre || Penetrating))
			deactivate();
		return true;
	} else {
		return false;
	}
}

}
