#include <stdio.h>
#include <GL/glut.h>
#include <config.h>
#include <tractor.h>
#include <Scene.h>

Simulator::Config config;
Simulator::Scene scene;

void display() {
	scene.update();
	glutSwapBuffers();
}

void reshape(int width, int height) {
	config.set_window_width(width).set_window_height(height);
	scene.init(config);
}

void keyboard(unsigned char key, int x, int y) {
	scene.on_keyboard(key);
}

void special(int key, int x, int y) {
	scene.on_keyboard(key);
}

int main( int argc, char** argv ) {
	config.set_window_width(1024).set_window_height(768);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(config.get_window_width(), config.get_window_height());
	glutCreateWindow("Simulator");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	Simulator::Tractor tractor;
	tractor.set_position(0.0, 0.0, 0.0).set_direction(0.0, 0.0, 0.0);
	scene.add_element("tractor", &tractor);
	glutMainLoop();

	#ifdef TEST_DEFINE
	printf("TEST_DEFINE\n");
	#endif

	return 0;
}