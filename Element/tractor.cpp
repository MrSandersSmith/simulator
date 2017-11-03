#include <GL/glut.h>
#include <tractor.h>

namespace Simulator {

	Tractor::Tractor() : step(0.0), angle(0.0) {
	}

	Tractor::~Tractor() {
	}

	void Tractor::draw_wheel(double radius, double width) {
		double halfWidth = width / 2.0;
		glColor3f(0.0f, 0.0f, 0.0f);
		Element::draw_rect(-halfWidth, -radius, halfWidth, radius);
	}

	void Tractor::draw_body_part(double length, double width) {
		double halfLength = length / 2.0;
		double halfWidth = width / 2.0;
		glColor3f(1.0f, 1.0f, 0.0f);
		draw_rect(-halfLength, -halfWidth, halfLength, halfWidth);
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslated(0.0, 0.0, 0.001);
		draw_rect(-halfLength, -halfWidth, halfLength, halfWidth, true);
	}

	void Tractor::move_forward() {
		step += 0.1;
	}

	void Tractor::move_back() {
		step -= 0.1;
	}

	void Tractor::turn_left() {
		angle += 1.0;
	}

	void Tractor::turn_right() {
		angle -= 1.0;
	}

	void Tractor::update() {
		glPushMatrix();
			glRotated(angle, 0.0, 0.0, 1.0);
			glTranslated(step * cos(angle * RAD_PER_DEG), step * sin(angle * RAD_PER_DEG), 0.0);
			glPushMatrix();
				glTranslated(-2.25, 0.0, 0.0);
				draw_wheel(1.8, 1.0);
				glTranslated(0.25, 0.0, 0.1);
				draw_body_part(1.0, 4.0);
			glPopMatrix();
			glPushMatrix();
				glTranslated(2.25, 0.0, 0.0);
				draw_wheel(1.8, 1.0);
				glTranslated(-0.25, 0.0, 0.1);
				draw_body_part(1.0, 4.0);
			glPopMatrix();
			glPushMatrix();
				glTranslated(0.0, 1.0, 0.5);
				draw_body_part(4.0, 4.0);
				glTranslated(0.0, 4.0, -0.1);
				draw_body_part(2.0, 4.0);
				glTranslated(0.0, 1.0, -0.1);
				draw_body_part(4.0, 0.25);
			glPopMatrix();
			glPushMatrix();
				glTranslated(-2.25, 6.0, 0.0);
				draw_wheel(1.0, 0.5);
			glPopMatrix();
			glPushMatrix();
				glTranslated(2.25, 6.0, 0.0);
				draw_wheel(1.0, 0.5);
			glPopMatrix();
		glPopMatrix();
	}
}