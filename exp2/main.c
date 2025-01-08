#include <GL/glut.h>
#include <math.h>

// 定义球体的半径、经线和纬线数目
#define RADIUS 1.0
#define LATS 20
#define LONGS 20
#define M_PI 3.14159265359

// 初始化OpenGL环境
// 初始化OpenGL环境
void init(void) {
    // 设置背景颜色为黑色
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // 设置光照和材质属性
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat material_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat material_shininess[] = { 50.0 };

    // 启用光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // 设置光源属性
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // 设置材质属性
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    // 设置深度测试
    glEnable(GL_DEPTH_TEST);
}


// 绘制圆球
void drawSphere(GLfloat radius, GLint lats, GLint longs) {
    int i, j;
    for (i = 0; i <= lats; i++) {
        GLfloat lat0 = M_PI * (-0.5 + (GLfloat)(i - 1) / lats);
        GLfloat z0 = sin(lat0);
        GLfloat zr0 = cos(lat0);

        GLfloat lat1 = M_PI * (-0.5 + (GLfloat)i / lats);
        GLfloat z1 = sin(lat1);
        GLfloat zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            GLfloat lng = 2 * M_PI * (GLfloat)(j - 1) / longs;
            GLfloat x = cos(lng);
            GLfloat y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0 * radius, y * zr0 * radius, z0 * radius);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1 * radius, y * zr1 * radius, z1 * radius);
        }
        glEnd();
    }
}

// 显示函数
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 设置视图变换
    gluLookAt(0.0, 0.0, 5.0,  // 眼睛位置
        0.0, 0.0, 0.0,  // 观察点位置
        0.0, 1.0, 0.0); // 上方向向量

    // 绘制圆球
    drawSphere(RADIUS, LATS, LONGS);

    glutSwapBuffers();
}

// 主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D编程基础实验");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
