#include <GL/gl.h>
#include <Element.h>

namespace Simulator {

	Element::Element() {
	}

	Element::~Element() {
	}

	Element& Element::set_position(double x, double y, double z) {
		position = {x, y, z};
		return *this;
	}

	Element& Element::set_direction(double x, double y, double z) {
		direction = {x, y, z};
		return *this;
	}

	void Element::draw_rect(double x1, double y1, double x2, double y2, bool isWireframe) {
		glBegin(isWireframe ? GL_LINE_STRIP : GL_QUADS);
			glVertex2d(x1, y1);
			glVertex2d(x2, y1);
			glVertex2d(x2, y2);
			glVertex2d(x1, y2);
			if (isWireframe) {
				glVertex2d(x1, y1);
			}
		glEnd();
	}
}