//============================================================================
// Name        : openglTest.cpp
// Author      : Hanzki
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "GL/freeglut.h"

void render() {
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.5, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glEnd();

	glutSwapBuffers();

	return;
}

void loadObj(const char *filename) {
	std::string line;
	std::ifstream obj(filename);
	std::string field;
	if (obj.is_open()) {
		while (obj.good()) {
			std::vector<std::string> fields;
			getline(obj, line);
			std::istringstream liness(line);
			while (getline(liness, field, ' ')) {
				fields.insert(fields.end(), field);
			}
			if(fields[0] != "#"){
			for (std::vector<std::string>::iterator it = fields.begin(); it < fields.end(); it++)
				std::cout << ' ' << *it;
				fflush(stdout);
			std::cout << '\n';
			fflush(stdout);
			}

		}
		std::cout << "that is all\n";
		fflush(stdout);
	}
	else{
		std::cout << "couldn't open the file\n";
		fflush(stdout);
	}

}

int main(int argc, char **argv) {
	loadObj("test.obj");
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("testwin");

	glutDisplayFunc(render);
	glutMainLoop();

	return 0;
}
