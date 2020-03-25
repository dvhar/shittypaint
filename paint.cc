#include <GL/glut.h>

#include <ctime>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

// set global vars and 2D vectors for the curves
int currentLine = 0, lmouse = 0;
vector<vector<int> > curves;
vector<vector<GLubyte> > colors;
GLfloat width = 1.0;
int keys[256];
int randColor = 0;
int currentColor = 0;
int currentBG = 7;

GLubyte gcolors[][4] = {
	{255, 255, 255, 255},  // white
	{255, 0, 0, 255},      // red
	{0, 255, 0, 255},      // green
	{0, 0, 255, 255},      // blue
	{255, 0, 255, 255},    // pink
	{0, 255, 255, 255},    // teal
	{255, 255, 0, 255},    // yellow
	{0, 0, 0, 255},        // black
	{255, 100, 0, 255},    // orange
	{125, 125, 125, 255},  // gray
	{125, 0, 255, 255},    // purple
};

void init() {
	for (int i = 0; i < 256; i++) keys[i] = 0;
	glutInitWindowSize(640, 480);
	glutCreateWindow("shitty paint");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// glutIgnoreKeyRepeat(1);
	glLineWidth(width);
	float w = glutGet(GLUT_WINDOW_WIDTH);
	float h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0.0, w, h, 0.0);
	srand(time(0));
}

void click(int lr, int ud, int x, int y) {
	// clear drawing on right click
	float w = glutGet(GLUT_WINDOW_WIDTH);
	float h = glutGet(GLUT_WINDOW_HEIGHT);
	glLoadIdentity();
	gluOrtho2D(0.0, w, h, 0.0);
	if (lr == GLUT_RIGHT_BUTTON && ud == GLUT_DOWN) {
		curves.clear();
		colors.clear();
		currentLine = 0;
		glutPostRedisplay();
	}

	// create a new line on left click
	else if (lr == GLUT_LEFT_BUTTON && ud == GLUT_DOWN) {
		curves.push_back(vector<int>());
		colors.push_back(vector<GLubyte>());
		for (int i = 0; i < 3; i++) {
			srand(rand());
			if (randColor)
				colors[currentLine].push_back(rand() % 256);
			else
				colors[currentLine].push_back(gcolors[currentColor][i]);
		}
		lmouse = 1;
	}

	// register end of line on mouse up
	else if (lr == GLUT_LEFT_BUTTON && ud == GLUT_UP) {
		lmouse = 0;
		currentLine++;
	}
}

void drag(int x, int y) {
	// add new vertices to line when dragging mouse
	if (lmouse) {
		curves[currentLine].push_back(x);
		curves[currentLine].push_back(y);
		glutPostRedisplay();
	}
}

void arrowKeys(int key, int x, int y) {
	// adjust line width on up and down arrows
	if (key == GLUT_KEY_DOWN && width > 1.0) {
		width -= 1.0;
		glutPostRedisplay();
	} else if (key == GLUT_KEY_UP) {
		width += 1.0;
		glutPostRedisplay();
	}
}
void keyup(unsigned char key, int x, int y) { keys[key] = 0; }
void keydown(unsigned char key, int x, int y) {
	if (key == 'q') exit(0);
	keys[key] = 1;
	if (keys['z']) {
		currentColor = 0;
		if (glutGetModifiers()) currentColor = 1;
	}
	if (keys['c']) {
		if (keys['w']) currentColor = 0;
		if (keys['r']) currentColor = 1;
		if (keys['g']) currentColor = 2;
		if (keys['b']) currentColor = 3;
		if (keys['p']) currentColor = 4;
		if (keys['t']) currentColor = 5;
		if (keys['y']) currentColor = 6;
		if (keys['l']) currentColor = 7;
		if (keys['o']) currentColor = 8;
		if (keys['e']) currentColor = 9;
		if (keys['u']) currentColor = 10;
		if (keys['n']) randColor ^= 1;
	} else if (keys['b']) {
		if (keys['w']) currentBG = 0;
		if (keys['r']) currentBG = 1;
		if (keys['g']) currentBG = 2;
		if (keys['u']) currentBG = 3;
		if (keys['p']) currentBG = 4;
		if (keys['t']) currentBG = 5;
		if (keys['y']) currentBG = 6;
		if (keys['l']) currentBG = 7;
	} else {
		if (keys['u']) {
			if (currentLine > 0) {
				curves.pop_back();
				colors.pop_back();
				currentLine--;
			}
		}
	}
	glClearColor(gcolors[currentBG][0], gcolors[currentBG][1],
				 gcolors[currentBG][2], gcolors[currentBG][2]);
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
}

void drawing(void) {
	glLineWidth(width);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw each vertex for each curve
	for (int j = 0; j < curves.size(); j++) {
		glColor3ub(colors[j][0], colors[j][1], colors[j][2]);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < curves[j].size(); i += 2)
			glVertex2i(curves[j][i], curves[j][i + 1]);
		glEnd();
	}
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	init();
	glutDisplayFunc(drawing);
	glutMouseFunc(click);
	glutMotionFunc(drag);
	glutSpecialFunc(arrowKeys);
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutMainLoop();
	return 0;
}
