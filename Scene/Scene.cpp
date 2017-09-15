#include <stdio.h>
#include <GL/gl.h>
#include <Element.h>
#include <Scene.h>

namespace Simulator {

	Scene::Scene() {
		printf("Hello Scene!");
	}
	Scene::~Scene() {
		printf("Buy Scene!");
	}
	bool Scene::add_element(Element *el) {

	}
	bool Scene::remove_element(Element *el) {

	}
	void Scene::update() {
		glClear( GL_COLOR_BUFFER_BIT );
		glBegin(GL_TRIANGLES);
			glColor3f(  1.0f,  0.0f, 0.0f);
			glVertex3f( 0.0f, -1.0f, 0.0f);
			glColor3f(  0.0f,  1.0f, 0.0f);
			glVertex3f(-1.0f,  1.0f, 0.0f);
			glColor3f(  0.0f,  0.0f, 1.0f);
			glVertex3f( 1.0f,  1.0f, 0.0f);
		glEnd();
	}
	void Scene::resize(unsigned int width, unsigned int height) {
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 1, 0, 1, -1, +1);		
	}
}