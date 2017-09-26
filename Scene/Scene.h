#ifndef SCENE_H_
#define SCENE_H_

namespace Simulator {

	class Config;
	class Element;

	class Scene {

	public:
		Scene();
		~Scene();
		bool add_element( Element* el );
		bool remove_element( Element* el );
		void update();
		void init( Config& config );
	};
}

#endif