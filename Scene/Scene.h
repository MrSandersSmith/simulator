#ifndef SCENE_H_
#define SCENE_H_

namespace Simulator {

	class Element;

	class Scene {
		
	public:
		Scene();
		~Scene();
		bool add_element(Element *el);
		bool remove_element(Element *el);
		void update();
		void resize(unsigned int width, unsigned int height);
	};

}

#endif