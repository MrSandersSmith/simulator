#ifndef ELEMENT_H_
#define ELEMENT_H_

namespace Simulator {

	class Element {
		
	public:
		Element();
		~Element();
		virtual bool update();
	};

}

#endif