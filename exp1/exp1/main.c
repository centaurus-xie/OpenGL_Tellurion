#include <GL/glut.h>

/*void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // ����ƽ��ǰ��ͼ��
    glColor3f(1.0, 0.0, 0.0); // ��ɫ
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(0.5, 1.0);
    glEnd();

    // ƽ�Ʊ任
    // glTranslatef(1.0, 1.0, 0.0); // ������ƽ��1����λ
    
    glLoadIdentity(); // ����ģ����ͼ����
    glTranslatef(0.5, 0.5, 0.0); // �ƶ�����ת����
    glRotatef(45.0, 0.0, 0.0, 1.0); // ��Z����ת45��
    glTranslatef(-0.5, -0.5, 0.0); // �ƻ�ԭ��
    
    // ���Ʊ仯���ͼ��
    glColor3f(0.0, 1.0, 0.0); // ��ɫ
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(0.5, 1.0);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("2D Translation");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}*/

#include <math.h>

// ��ʼ��OpenGL����
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); // ���ñ�����ɫΪ��ɫ
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-5.0, 5.0, -5.0, 5.0); // ������ͼ��С
}

// ����С���Ǻ���
void drawFlag(float x, float y, float size) {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); // ��ɫ
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x, y + size * sqrt(3) / 2);
    glEnd();

    glBegin(GL_QUADS); // �������
    glColor3f(0.5, 0.5, 0.5); // ��ɫ
    glVertex2f(x, y);
    glVertex2f(x + 0.05, y);
    glVertex2f(x + 0.05, y - 3);
    glVertex2f(x, y - 3);
    glEnd();
}

// ��ʾ����
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT); // �����ɫ������

    // ƽ��
    glPushMatrix();
    glTranslatef(2.0, 2.0, 0.0);
    drawFlag(0, 0, 1.0); // ƽ�ƺ��λ��
    glPopMatrix();

    // ��ת
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0); // ����ת�����Ƶ�ԭ��
    glRotatef(45.0, 0.0, 0.0, 1.0); // ��Z����ת45��
    drawFlag(0, 0, 1.0); // ��ת���λ��
    glPopMatrix();

    // ����
    glPushMatrix();
    glTranslatef(-2.0, -2.0, 0.0);
    glScalef(2.0, 2.0, 1.0); // ��X��Y��Ŵ�2��
    drawFlag(0, 0, 1.0); // ���ź��λ��
    glPopMatrix();

    // ��ת��С���Ǻ���
    static float angle = 0.0;
    glPushMatrix();
    glTranslatef(-2.0, 2.0, 0.0); // ����ת�����Ƶ���˵ײ�
    glRotatef(angle, 0.0, 1.0, 0.0); // ��y����ת
    drawFlag(0, 0, 1.0); // ������ת������
    glPopMatrix();
    angle += 0.1; // ÿ֡����0.1��
    if (angle > 360.0) angle -= 360.0; // ���ֽǶ���0-360��֮��

    glutSwapBuffers(); // ����ǰ�󻺳���
}

// ������
int main(int argc, char** argv) {
    glutInit(&argc, argv); // ��ʼ��GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // ������ʾģʽ
    glutInitWindowSize(800, 600); // ���ô��ڴ�С
    glutCreateWindow("2Dͼ�α任ʵ��"); // ��������
    init(); // ��ʼ��OpenGL����
    glutDisplayFunc(display); // ������ʾ�ص�����
    glutIdleFunc(display); // ���ÿ���ʱ���õĺ���
    glutMainLoop(); // �����¼�ѭ��
    return 0;
}


