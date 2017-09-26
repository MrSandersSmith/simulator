#include <stdio.h>
#include <GL/glut.h>
#include <config.h>
#include <Element.h>
#include <Scene.h>

namespace Simulator {

	Scene::Scene() {
	}

	Scene::~Scene() {
	}

	bool Scene::add_element( Element* el ) {
	}

	bool Scene::remove_element( Element* el ) {
	}

	void Scene::update() {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glBegin( GL_TRIANGLES );
			glColor3f(  1.0f,  0.0f, 0.0f);
			glVertex3f( 0.0f, -1.0f, 0.0f);
			glColor3f(  0.0f,  1.0f, 0.0f);
			glVertex3f(-1.0f,  1.0f, 0.0f);
			glColor3f(  0.0f,  0.0f, 1.0f);
			glVertex3f( 1.0f,  1.0f, 0.0f);
		glEnd();
	}

	void Scene::init( Config& config ) {
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		GLsizei windowWidth = config.get_window_width();
		GLsizei windowHeight = config.get_window_height();
		glViewport( 0, 0, windowWidth, windowHeight );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		double aspect = (double) windowWidth / windowHeight;
		if (windowWidth >= windowHeight) {
			gluOrtho2D(-5.0 * aspect, 5.0 * aspect, -5.0, 5.0);
		}
		else {
			gluOrtho2D(-5.0, 5.0, -5.0 / aspect, 5.0 / aspect);
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}