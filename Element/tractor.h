#ifndef TRACTOR_H_
#define TRACTOR_H_

#define RAD_PER_DEG 0.017453293

#include <Element.h>

namespace Simulator {

	class Element;

	class Tractor: public Element {

	public:
		Tractor();
		~Tractor();
		void update();
		void move_forward();
		void move_back();
		void turn_left();
		void turn_right();

	protected:
		void draw_wheel(double radius, double width);
		void draw_body_part(double length, double width);

	private:
		double step;
		double angle;
	};
}

#endif