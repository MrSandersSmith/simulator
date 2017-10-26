#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <map>
#include <string>

namespace Simulator {

	class Config;
	class Element;

	class Scene {

	public:
		Scene();
		~Scene();
		void add_element(Element* el);
		void add_element(std::string name, Element* el);
		bool remove_element(Element* el);
		void update();
		void init(Config& config);
		void on_keyboard(unsigned char key);
		void on_keyboard(int key);

	private:
		// std::vector<Element*> elements;
		std::map<std::string, Element*> elements;
	};
}

#endif