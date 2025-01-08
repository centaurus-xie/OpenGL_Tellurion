#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLuint earthTexture;

// 球体细分参数
#define LATITUDE_BINS 512
#define LONGITUDE_BINS 512

// 鼠标状态变量
int mouseOldX = 0;
int mouseOldY = 0;
int mouseButtons = 0;
GLfloat scale = 1.0f;
GLfloat glMatrix[16];

// 函数声明
GLuint loadBMP(const char* imagepath);
void display();
void init();
void reshape(int width, int height);
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);

// 处理24位BMP文件
typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} RGBPixel;

GLuint loadBMP(const char* imagepath) {
    // 打开文件
    FILE* file = fopen(imagepath, "rb");
    if (!file) {
        printf("Image could not be opened\n");
        return 0;
    }

    // 读取文件头
    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54) {
        printf("Not a correct BMP file\n");
        return 0;
    }
    for (int i = 0; i < 54; i++) {
        printf("herader[%d]:%d\n", i, header[i]);
    }
    // 读取图像宽度和高度
    int width = 512;// *(int*)&header[18];
    int height = 256;//*(int*)&header[22];
    // printf("w:%d\nh:%d\n", width, height);
    // 检查是否为24位BMP
    /*
    if (*(int*)&header[28] != 24) {
        printf("Not a 24bit BMP file\n");
        return 0;
    }*/

    // 跳过文件信息头
    fseek(file, 54, SEEK_SET);

    // 读取像素数据
    unsigned char* data = (unsigned char*)malloc(width * height * 3 * sizeof(unsigned char));
    if (!data) {
        printf("Memory error\n");
        fclose(file);
        return 0;
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            RGBPixel pixel;
            fread(&pixel, 1, 3, file);
            data[(i * width + j) * 3 + 0] = pixel.red;
            data[(i * width + j) * 3 + 1] = pixel.green;
            data[(i * width + j) * 3 + 2] = pixel.blue;
        }
        // BMP文件每行像素数据必须是4的倍数，跳过填充字节
        fseek(file, (4 - (width * 3) % 4) % 4, SEEK_CUR);
    }

    // 关闭文件
    fclose(file);

    // 创建OpenGL纹理
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // GL_BGR不支持
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // 释放内存
    free(data);

    return textureID;
}

// 球体顶点生成函数
void drawSphere() {
    struct Point3D {
        double x, y, z;
    };

    // 绘制球体的顶点和纹理坐标
    for (int i = 0; i <= LATITUDE_BINS; i++) {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / LATITUDE_BINS);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / LATITUDE_BINS);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= LONGITUDE_BINS; j++) {
            double lng = 2 * M_PI * (double)(j - 1) / LONGITUDE_BINS;
            double x = cos(lng);
            double y = sin(lng);

            glTexCoord2f((float)(j - 1) / LONGITUDE_BINS, (float)(i - 1) / LATITUDE_BINS);
            glVertex3f(x* zr0, y* zr0, z0);

            glTexCoord2f((float)j / LONGITUDE_BINS, (float)i / LATITUDE_BINS);
            glVertex3f(x* zr1, y* zr1, z1);
        }
        glEnd();
    }
}

// 设置光源为平行光并固定其方向
void setParallelLightDirection() {
    // 定义光源的方向向量（平行光）
    // 第四个参数为0，表示这是一个方向光
    GLfloat light_direction[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    // 设置光源方向
    glLightfv(GL_LIGHT0, GL_POSITION, light_direction);
}

// 初始化OpenGL环境和加载纹理
void init() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // 设置光源为平行光并固定其方向
    // setParallelLightDirection();
    // 设置材质颜色
    GLfloat mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 100.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // 加载纹理
    earthTexture = loadBMP("earth3.bmp");

    // 初始化模型视图矩阵
    glGetFloatv(GL_MODELVIEW_MATRIX, glMatrix);
}

// 设置光照
void setupLighting() {
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 }; // 光源位置，最后一个参数为1.0表示这是一个无限远的光源
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };  // 环境光成分
    GLfloat light_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };  // 减少漫反射光成分，使光更"硬"
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // 增加镜面反射光成分，使高光更明显

    // 如果使用聚光灯，设置其属性
    GLfloat spot_direction[] = { -1.0, -1.0, -1.0 }; // 聚光灯方向
    GLfloat spot_exponent = 60.0; // 聚光灯指数，值越大，光照越集中，边缘越硬
    GLfloat spot_cutoff = 800.0; // 聚光灯的扩散角度，值越小，光照越集中

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // 如果使用聚光灯，则设置聚光灯属性
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
}


// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 设置视角
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    // 设置光照
    setupLighting();
    
    // 应用全局模型视图矩阵(最后一次添加)
    glMultMatrixf(glMatrix);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, earthTexture);

    // 绘制地球仪
    drawSphere();

    glutSwapBuffers();
}

// 窗口大小改变回调函数
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 1.0, 20.0);
}

// 鼠标按钮事件处理函数
void mouseButton(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        mouseButtons |= 1 << button;
        // printf("Left button pressed.\n");
    }
    else if (state == GLUT_UP) {
        mouseButtons = 0;
    }
    mouseOldX = x;
    mouseOldY = y;
}

// 鼠标移动事件处理函数
void mouseMove(int x, int y) {
    int dx = x - mouseOldX;
    int dy = y - mouseOldY;

    if (mouseButtons & (1 << GLUT_LEFT_BUTTON)) {
        // 拖动旋转
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); // 保存当前矩阵状态
        glLoadIdentity(); // 重置矩阵，但不会影响之前的状态
        glRotatef(dx, 0.0f, 1.0f, 0.0f);
        glRotatef(dy, 1.0f, 0.0f, 0.0f);
        // printf("Mouse dragged x%d y%d.\n", dx, dy);
        glMultMatrixf(glMatrix); // 应用旋转到当前矩阵
        glGetFloatv(GL_MODELVIEW_MATRIX, glMatrix); // 更新全局矩阵
        glPopMatrix(); // 恢复矩阵状态
        glutPostRedisplay();
    }
    else if (mouseButtons & (1 << GLUT_RIGHT_BUTTON)) {
        // 按住右键上下缩放
        scale *= (1.0f + dy * 0.001f);
        if (scale < 0.99f) scale = 0.99f;
        if (scale > 1.01f) scale = 1.01f;
        printf("sca%f\n", scale);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); // 保存当前矩阵状态
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, 0.0f); // 设置观察点
        glScalef(scale, scale, scale);
        glMultMatrixf(glMatrix); // 应用缩放到当前矩阵
        glGetFloatv(GL_MODELVIEW_MATRIX, glMatrix); // 更新全局矩阵
        glPopMatrix(); // 恢复矩阵状态
        glutPostRedisplay();
    }
    mouseOldX = x;
    mouseOldY = y;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Earth Globe");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    glutMainLoop();
    return 0;
}
