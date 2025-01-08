#include <GL/glut.h>

/*void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制平移前的图形
    glColor3f(1.0, 0.0, 0.0); // 红色
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(0.5, 1.0);
    glEnd();

    // 平移变换
    // glTranslatef(1.0, 1.0, 0.0); // 向左下平移1个单位
    
    glLoadIdentity(); // 重置模型视图矩阵
    glTranslatef(0.5, 0.5, 0.0); // 移动到旋转中心
    glRotatef(45.0, 0.0, 0.0, 1.0); // 绕Z轴旋转45度
    glTranslatef(-0.5, -0.5, 0.0); // 移回原点
    
    // 绘制变化后的图形
    glColor3f(0.0, 1.0, 0.0); // 绿色
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

// 初始化OpenGL环境
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); // 设置背景颜色为黑色
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-5.0, 5.0, -5.0, 5.0); // 设置视图大小
}

// 绘制小三角红旗
void drawFlag(float x, float y, float size) {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); // 红色
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x, y + size * sqrt(3) / 2);
    glEnd();

    glBegin(GL_QUADS); // 绘制旗杆
    glColor3f(0.5, 0.5, 0.5); // 灰色
    glVertex2f(x, y);
    glVertex2f(x + 0.05, y);
    glVertex2f(x + 0.05, y - 3);
    glVertex2f(x, y - 3);
    glEnd();
}

// 显示函数
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区

    // 平移
    glPushMatrix();
    glTranslatef(2.0, 2.0, 0.0);
    drawFlag(0, 0, 1.0); // 平移后的位置
    glPopMatrix();

    // 旋转
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0); // 将旋转中心移到原点
    glRotatef(45.0, 0.0, 0.0, 1.0); // 绕Z轴旋转45度
    drawFlag(0, 0, 1.0); // 旋转后的位置
    glPopMatrix();

    // 缩放
    glPushMatrix();
    glTranslatef(-2.0, -2.0, 0.0);
    glScalef(2.0, 2.0, 1.0); // 沿X和Y轴放大2倍
    drawFlag(0, 0, 1.0); // 缩放后的位置
    glPopMatrix();

    // 旋转的小三角红旗
    static float angle = 0.0;
    glPushMatrix();
    glTranslatef(-2.0, 2.0, 0.0); // 将旋转中心移到旗杆底部
    glRotatef(angle, 0.0, 1.0, 0.0); // 绕y轴旋转
    drawFlag(0, 0, 1.0); // 绘制旋转的旗帜
    glPopMatrix();
    angle += 0.1; // 每帧增加0.1度
    if (angle > 360.0) angle -= 360.0; // 保持角度在0-360度之间

    glutSwapBuffers(); // 交换前后缓冲区
}

// 主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv); // 初始化GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // 设置显示模式
    glutInitWindowSize(800, 600); // 设置窗口大小
    glutCreateWindow("2D图形变换实验"); // 创建窗口
    init(); // 初始化OpenGL环境
    glutDisplayFunc(display); // 设置显示回调函数
    glutIdleFunc(display); // 设置空闲时调用的函数
    glutMainLoop(); // 进入事件循环
    return 0;
}


