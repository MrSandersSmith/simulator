#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <SMat.h>

namespace Simulator {

	class Element {

	public:
		Element();
		virtual ~Element();
		virtual void update() = 0;
		Element& set_position(double x, double y, double z);
		Element& set_direction(double x, double y, double z);

	protected:
		SVec position;
		SVec direction;
		static void draw_rect(double x1, double y1, double x2, double y2, bool isWireframe = false);
	};
}

#endif