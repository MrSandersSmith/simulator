#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include <config.h>
#include <Element.h>
#include <tractor.h>
#include <Scene.h>

namespace Simulator {

	Scene::Scene() {
	}

	Scene::~Scene() {
	}

	void Scene::add_element(Element* el) {
		// elements.push_back(el);
	}

	void Scene::add_element(std::string name, Element* el) {
		elements[name] = el;
	}

	void Scene::remove_element(Element* el) {
	}

	void Scene::update() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::map<std::string, Element*>::iterator itr;
		for (itr = elements.begin(); itr != elements.end(); ++itr) {
			itr->second->update();
		}
	}

	void Scene::on_keyboard(unsigned char key) {
		// switch (key) {
		// 	case 27: // ESC
		// 		// glutDestroyWindow();
		// }
	}

	void Scene::on_keyboard(int key) {
		Tractor* tractor = dynamic_cast<Tractor*>(elements["tractor"]);
		switch (key) {
			case GLUT_KEY_UP:
				tractor->move_forward();
				break;
			case GLUT_KEY_DOWN:
				tractor->move_back();
				break;
			case GLUT_KEY_LEFT:
				tractor->turn_left();
				break;
			case GLUT_KEY_RIGHT:
				tractor->turn_right();
				break;
		}
		glutPostRedisplay();
	}

	void Scene::init(Config& config) {
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClearDepth(1.0);
		glEnable(GL_DEPTH_TEST);
		GLsizei windowWidth = config.get_window_width();
		GLsizei windowHeight = config.get_window_height();
		glViewport(0, 0, windowWidth, windowHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		double clipPlane = 30.0;
		double aspect = (double) windowWidth / windowHeight;
		if (windowWidth >= windowHeight) {
			gluOrtho2D(-clipPlane * aspect, clipPlane * aspect, -clipPlane, clipPlane);
		}
		else {
			gluOrtho2D(-clipPlane, clipPlane, -clipPlane / aspect, clipPlane / aspect);
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}