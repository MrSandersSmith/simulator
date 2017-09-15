#include <stdio.h>
#include <Scene.h>
#include <manager.h>

int main(int argc, char *argv[]) {

	Simulator::Scene *scene = new Simulator::Scene();
	printf("Hello world!");

	#ifdef TEST_DEFINE
	printf("TEST_DEFINE\n");
	#endif
	
	Simulator::main_loop(argc, argv, scene);

	delete scene;

	return 0;
}