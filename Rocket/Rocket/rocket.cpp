#include "Dependencies/freeglut/glut.h"
#include <sys/timeb.h>
#include <sys/utime.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <ctime>
#include<math.h>
#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH  1100
#define FPS 60
#define TIMER (1000/FPS)
#define PI 3.14159265358979
#define STEPS 40

//format chữ
const int font1 = (int)GLUT_BITMAP_TIMES_ROMAN_24;
const int font2 = (int)GLUT_BITMAP_HELVETICA_18;
const int font3 = (int)GLUT_BITMAP_8_BY_13;
char s[30];
void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

bool start = false;
double r, g, b;
double r2, g2, b2;
int speedE = 1;//tốc độ chạy của enermy
int enermyPass = 0;//số lượng enermy đã chạy qua
int score = 0;//điểm chơi game=0

GLint xRocket = 50; //vị trí rocket x
GLint yRocket = 100; //vị trí rocket khi bắt đầu game là ở giữa đường

GLint yEnermy1, yEnermy2; //vị trí enermy khi bắt đầu game
GLint xEnermy1, xEnermy2; //vị trí enermy khi bắt đầu game

GLint xMay1, yMay1;
GLint xMay2, yMay2;


//hàm khởi tạo enemy 
void initGame() {
    yEnermy1 = (rand() % 3) * 100; //vị trí enermy 1 khi bắt đầu game
    yEnermy2 = (rand() % 3) * 100; //vị trí enermy 2 khi bắt đầu game

    xEnermy1 = SCREEN_WIDTH;// vị trí ở bên góc ngoài phải màn hình
    xEnermy2 = xEnermy1 + 600;//khoảng cách giữa 2 con enemy

    //yMay1 = 0; //vị trí enermy 1 khi bắt đầu game
    //yMay2 = 0; //vị trí enermy 2 khi bắt đầu game

    //xMay1 = SCREEN_WIDTH;// vị trí ở bên góc ngoài phải màn hình
    //xMay2 = xMay1 + 600;//khoảng cách giữa 2 con enemy


    xRocket = 50;//vị trí rocket
    yRocket = 100;//vị trí rocket
    speedE = 3;//tốc độ khởi đầu
    enermyPass = 0;//số enemy vượt qua
    int score = 0;
    r = rand() % 10 * 0.1;
    g = rand() % 10 * 0.1;
    b = rand() % 10 * 0.1;

    r2 = rand() % 10 * 0.1;
    g2 = rand() % 10 * 0.1;
    b2 = rand() % 10 * 0.1;
}
//lấy milli giây
int getMilliCount() {
    timeb tb;
    ftime(&tb);
    int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
    return nCount;
}
//hàm ngủ
void sleep(int sleeptime)
{
    int count = 0;
    int beginsleep = getMilliCount();
    while (getMilliCount() - beginsleep < sleeptime)
    {
        count++;
    }
}
//vẽ kẻ địch
void drawEnermy(GLfloat x, GLfloat y) {
    //vẽ thân đầu đạn
    
    glBegin(GL_QUADS);//vẽ hình chữ nhật
    glVertex2f(x + 25, y + 25);
    glVertex2f(x + 25, y + 75);
    glVertex2f(x + 125, y + 75);
    glVertex2f(x + 125, y + 25);
    glEnd();

    //vẽ sọc
 
    glBegin(GL_QUADS);//vẽ hình chữ nhật
    glVertex2f(x + 45, y + 25);
    glVertex2f(x + 45, y + 75);
    glVertex2f(x + 100, y + 75);
    glVertex2f(x + 100, y + 25);
    glEnd();

    //vẽ mũi đầu đạn
    glBegin(GL_TRIANGLES);//vẽ tam giác
    glVertex2f(x+25 , y+25 );
    glVertex2f(x+25 , y + 75);
    glVertex2f(x -50, y +50);
    glEnd();
   
}
//vẽ mây


//hàm vẽ rocket
void drawRocket(GLfloat x, GLfloat y) {
    //vẽ đuôi
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);//vẽ hình chữ nhật
    glVertex2f(x, y + 5);
    glVertex2f(x, y + 95);
    glVertex2f(x + 25, y + 75);
    glVertex2f(x + 25, y + 25);

    //vẽ thân máy bay
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);//vẽ hình chữ nhật
    glVertex2f(x + 25, y + 25);
    glVertex2f(x + 25, y + 75);
    glVertex2f(x + 100, y + 75);
    glVertex2f(x + 100, y + 25);
    glEnd();

    //vẽ thân trong máy bay
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);//vẽ hình chữ nhật
    glVertex2f(x + 50, y + 35);
    glVertex2f(x + 85, y + 35);
    glVertex2f(x + 85, y + 65);
    glVertex2f(x + 50, y + 65);
    glEnd();

    //vẽ mũi máy bay
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);//vẽ tam giác
    glVertex2f(x + 100, y + 25);
    glVertex2f(x + 100, y + 75);
    glVertex2f(x + 130, y + 50);
    //vẽ cánh trên
    glVertex2f(x + 25, y + 100);
    glVertex2f(x + 25, y + 75);
    glVertex2f(x + 80, y + 75);
    //vẽ cánh dưới
    glVertex2f(x + 25, y);
    glVertex2f(x + 25, y + 25);
    glVertex2f(x + 80, y + 25);
    glEnd();
}
//vẽ khung hình chữ nhật
void drawRectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    glBegin(GL_QUADS);//vẽ hình chữ nhật
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}
//hàm vẽ hình tròn
void glCircle(GLint x, GLint y, GLint radius)
{
    GLfloat twicePi = (GLfloat)2.0f * PI;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2i(x, y);
    for (int i = 0; i <= STEPS; i++)
    {
        glVertex2i((GLint)(x + (radius * cos(i * twicePi / STEPS)) + 0.01), (GLint)(y + (radius * sin(i * twicePi / STEPS)) + 0.01));
    }
    glEnd();
}
//hàm khởi tạo
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);//xóa màu nền
    glShadeModel(GL_FLAT);

}
//hàm vẽ màn hình chính
void drawMenuScreen() {
    //vẽ bầu trời
    glColor3f(0.0, 1.0, 1.0);
    drawRectangle(0.0, 300.0, SCREEN_WIDTH, 300.0);

    //vẽ đường bay 1
    glColor3f(0.3, 0.3, 0.3);//chọn màu xám
    drawRectangle(0.0, 0.0, SCREEN_WIDTH, 100.0);
    //vẽ đường bay 2
    glColor3f(0.5, 0.5, 0.5);//chọn màu xám++
    drawRectangle(0.0, 100.0, SCREEN_WIDTH, 100.0);
    //vẽ đường bay 3
    glColor3f(0.7, 0.7, 0.7);//chọn màu xám+++
    drawRectangle(0.0, 200.0, SCREEN_WIDTH, 100.0);

    //vẽ núi 1
    glColor3f(0.235, 0.702, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(130, 300);
    glVertex2f(320, 550);
    glVertex2f(350 + 190, 300);
    glEnd();
    //vẽ núi 2
    glColor3f(0.000, 0.502, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 300);
    glVertex2f(130, 450);
    glVertex2f(260, 300);
    glEnd();
    //vẽ núi 3
    glColor3f(0.000, 0.502, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(0 + 100, 300);
    glVertex2f(130 + 100, 460);
    glVertex2f(260 + 100, 300);
    glEnd();
    //vẽ núi 4
    glColor3f(0.235, 0.702, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(400, 300);
    glVertex2f(550, 525);
    glVertex2f(700, 300);
    glEnd();

    //vẽ mặt trời
   
    glColor3f(1.0, 0, 0);
    glCircle(1030, 540, 50);

    //vẽ mây 1
    glColor3f(1.0, 1.0, 1.0);
    glCircle(880, 550, 20);
    glCircle(870, 540, 20);
    glCircle(900, 550, 20);
    glCircle(920, 530, 20);
    glCircle(900, 530, 20);
    glCircle(880, 530, 20);
    glCircle(920, 550, 20);
    glCircle(930, 540, 20);
    //vẽ mây 2
    glColor3f(1.0, 1.0, 1.0);
    glCircle(880 - 230, 550 - 50, 20);
    glCircle(870 - 230, 540 - 50, 20);
    glCircle(900 - 230, 550 - 50, 20);
    glCircle(920 - 230, 530 - 50, 20);
    glCircle(900 - 230, 530 - 50, 20);
    glCircle(880 - 230, 530 - 50, 20);
    glCircle(920 - 230, 550 - 50, 20);
    glCircle(930 - 230, 540 - 50, 20);
    //vẽ mây 3
    glColor3f(1.0, 1.0, 1.0);
    glCircle(880 - 450, 550 + 10, 20);
    glCircle(870 - 450, 540 + 10, 20);
    glCircle(900 - 450, 550 + 10, 20);
    glCircle(920 - 450, 530 + 10, 20);
    glCircle(900 - 450, 530 + 10, 20);
    glCircle(880 - 450, 530 + 10, 20);
    glCircle(920 - 450, 550 + 10, 20);
    glCircle(930 - 450, 540 + 10, 20);
    //vẽ mây 4
    glColor3f(1.0, 1.0, 1.0);
    glCircle(880 - 750, 550 - 20, 20);
    glCircle(870 - 750, 540 - 20, 20);
    glCircle(900 - 750, 550 - 20, 20);
    glCircle(920 - 750, 530 - 20, 20);
    glCircle(900 - 750, 530 - 20, 20);
    glCircle(880 - 750, 530 - 20, 20);
    glCircle(920 - 750, 550 - 20, 20);
    glCircle(930 - 750, 540 - 20, 20);

    //vẽ rocket xịn sò 1
    //vẽ cánh
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(100, 150);
    glVertex2f(100, 260);
    glVertex2f(250, 225 - 20);
    glEnd();

    //vẽ thân máy bay
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(100, 200 - 20);
    glVertex2f(100, 250 - 20);
    glVertex2f(200, 250 - 20);
    glVertex2f(200, 200 - 20);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(100, 200 - 20);
    glVertex2f(100, 250 - 20);
    glVertex2f(120, 250 - 20);
    glVertex2f(120, 200 - 20);
    glEnd();

    //vẽ mũi máy bay
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(200, 250 - 20);
    glVertex2f(200, 200 - 20);
    glVertex2f(250, 225 - 20);
    glEnd();

    //vẽ đuôi máy bay
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(100, 180);
    glVertex2f(100, 230);
    glVertex2f(50, 240);
    glVertex2f(100, 180);
    glVertex2f(100, 230);
    glVertex2f(50, 170);
    glVertex2f(100, 180);
    glVertex2f(100, 230);
    glVertex2f(30, 205);
    glEnd();

    //vẽ rocket xịn sò 2
    //vẽ cánh
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(100 + 400, 150 - 50 - 50);
    glVertex2f(100 + 400, 260 - 50 - 50);
    glVertex2f(250 + 400, 225 - 20 - 50 - 50);
    glEnd();

    //vẽ thân máy bay
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(100 + 400, 200 - 20 - 50 - 50);
    glVertex2f(100 + 400, 250 - 20 - 50 - 50);
    glVertex2f(200 + 400, 250 - 20 - 50 - 50);
    glVertex2f(200 + 400, 200 - 20 - 50 - 50);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(100 + 400, 200 - 20 - 50 - 50);
    glVertex2f(100 + 400, 250 - 20 - 50 - 50);
    glVertex2f(120 + 400, 250 - 20 - 50 - 50);
    glVertex2f(120 + 400, 200 - 20 - 50 - 50);
    glEnd();

    //vẽ mũi máy bay
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(200 + 400, 250 - 20 - 50 - 50);
    glVertex2f(200 + 400, 200 - 20 - 50 - 50);
    glVertex2f(250 + 400, 225 - 20 - 50 - 50);
    glEnd();

    //vẽ đuôi máy bay
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(100 + 400, 180 - 50 - 50);
    glVertex2f(100 + 400, 230 - 50 - 50);
    glVertex2f(50 + 400, 240 - 50 - 50);
    glVertex2f(100 + 400, 180 - 50 - 50);
    glVertex2f(100 + 400, 230 - 50 - 50);
    glVertex2f(50 + 400, 170 - 50 - 50);
    glVertex2f(100 + 400, 180 - 50 - 50);
    glVertex2f(100 + 400, 230 - 50 - 50);
    glVertex2f(30 + 400, 205 - 50 - 50);
    glEnd();

    //vẽ rocket xịn sò 3
    //vẽ cánh
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(100 + 400 + 200 + 200, 150);
    glVertex2f(100 + 400 + 200 + 200, 260);
    glVertex2f(250 + 400 + 200 + 200, 225 - 20);
    glEnd();

    //vẽ thân máy bay
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(100 + 400 + 200 + 200, 200 - 20);
    glVertex2f(100 + 400 + 200 + 200, 250 - 20);
    glVertex2f(200 + 400 + 200 + 200, 250 - 20);
    glVertex2f(200 + 400 + 200 + 200, 200 - 20);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(100 + 400 + 200 + 200, 200 - 20);
    glVertex2f(100 + 400 + 200 + 200, 250 - 20);
    glVertex2f(120 + 400 + 200 + 200, 250 - 20);
    glVertex2f(120 + 400 + 200 + 200, 200 - 20);
    glEnd();

    //vẽ mũi máy bay
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(200 + 400 + 200 + 200, 250 - 20);
    glVertex2f(200 + 400 + 200 + 200, 200 - 20);
    glVertex2f(250 + 400 + 200 + 200, 225 - 20);
    glEnd();

    //vẽ đuôi máy bay
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(100 + 400 + 200 + 200, 180);
    glVertex2f(100 + 400 + 200 + 200, 230);
    glVertex2f(50 + 400 + 200 + 200, 240);
    glVertex2f(100 + 400 + 200 + 200, 180);
    glVertex2f(100 + 400 + 200 + 200, 230);
    glVertex2f(50 + 400 + 200 + 200, 170);
    glVertex2f(100 + 400 + 200 + 200, 180);
    glVertex2f(100 + 400 + 200 + 200, 230);
    glVertex2f(30 + 400 + 200 + 200, 205);
    glEnd();

    //Tên game
    glColor3f(0.0, 0.0, 0.5);
    renderBitmapString(500 - 50, 450, (void*)font1, "Rocket 2D Game ");//vị trí x,y, kiểu font1 và tên game

    //màu vẽ,vị trí x,y, kiểu font và hướng dẫn
    glColor3f(0.9, 1.0, 0.5);
    renderBitmapString(500 - 50, 400 - 30, (void*)font2, "Nhan Space de bat dau");
    renderBitmapString(500 - 50, 400 - 60, (void*)font2, "Nhan ESC de ket thuc");
    glColor3f(0.000, 0.000, 0.000);
    renderBitmapString(500 - 50, 400 - 90, (void*)font3, "Nhan W de bay len");
    renderBitmapString(500 - 50, 400 - 120, (void*)font3, "Nhan S de bay xuong");
    glColor3f(0.000, 1.000, 1.000);
    renderBitmapString(50, 60, (void*)font3, "Du an duoc Remake boi:");
    renderBitmapString(50, 40, (void*)font3, "Double Thang");

    
}
//hàm vẽ start game
void drawMay(GLfloat x, GLfloat y)
{  //vẽ núi 1
    glColor3f(0.235, 0.702, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(x+550, y+300);
    glVertex2f(x+750, y+480);
    glVertex2f(x+900, y+300);
    glEnd(); 
}
void drawGame() {
    //vẽ bầu trời
    glColor3f(0.0, 1.0, 1.0);//chọn màu xanh
    drawRectangle(0.0, 300.0, SCREEN_WIDTH, 300.0);

    //Số điểm 
    char buffer[50];
    sprintf_s(buffer, "SCORE: %d", score);
    glColor3f(0.0, 0.0, 0.5);
    renderBitmapString(470, 570, (void*)font1, buffer);

    //vẽ thân cây 1
    glColor3f(0.5, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(100, 300);
    glVertex2f(115, 300);
    glVertex2f(115, 350);
    glVertex2f(100, 350);
    glEnd();
    //vẽ ngọn cây 1
    glColor3f(0.000, 0.502, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(75, 350);
    glVertex2f(107.5, 400);
    glVertex2f(140, 350);
    glEnd();

    //vẽ thân cây 2
    glColor3f(0.5, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(200, 300);
    glVertex2f(215, 300);
    glVertex2f(215, 420);
    glVertex2f(200, 420);
    glEnd();
    //vẽ ngọn cây 2
    glColor3f(0.000, 0.502, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(175, 420);
    glVertex2f(207.5, 470);
    glVertex2f(240, 420);
    glEnd();

    //vẽ thân cây 3
    glColor3f(0.5, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(300, 300);
    glVertex2f(315, 300);
    glVertex2f(315, 380);
    glVertex2f(300, 380);
    glEnd();
    //vẽ ngọn cây 3
    glColor3f(0.000, 0.502, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(275, 380);
    glVertex2f(307.5, 430);
    glVertex2f(340, 380);
    glEnd();

    //vẽ mặt trời
    glColor3f(1.0, 0, 0);
    glCircle(100, 540, 50);

    //vẽ mây 1
    glColor3f(1.0, 1.0, 1.0);
    glCircle(980, 500, 20);
    glCircle(970, 490, 20);
    glCircle(1000, 500, 20);
    glCircle(1020, 480, 20);
    glCircle(1000, 480, 20);
    glCircle(980, 480, 20);
    glCircle(1020, 500, 20);
    glCircle(1030, 490, 20);
    //vẽ mây 2
    glColor3f(1.0, 1.0, 1.0);
    glCircle(230, 560, 20);
    glCircle(220, 550, 20);
    glCircle(230, 540, 20);
    glCircle(250, 560, 20);
    glCircle(250, 540, 20);
    glCircle(270, 560, 20);
    glCircle(270, 540, 20);
    glCircle(280, 550, 20);
    //vẽ mây 3
    glColor3f(1.0, 1.0, 1.0);
    glCircle(480, 510, 20);
    glCircle(470, 500, 20);
    glCircle(480, 490, 20);
    glCircle(500, 510, 20);
    glCircle(500, 490, 20);
    glCircle(520, 510, 20);
    glCircle(520, 490, 20);
    glCircle(530, 500, 20);
    //vẽ mây 4
    glColor3f(1.0, 1.0, 1.0);
    glCircle(730, 560, 20);
    glCircle(720, 550, 20);
    glCircle(730, 540, 20);
    glCircle(750, 560, 20);
    glCircle(750, 540, 20);
    glCircle(770, 560, 20);
    glCircle(770, 540, 20);
    glCircle(780, 550, 20);

    //vẽ núi 1
    glColor3f(0.235, 0.702, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(550, 300);
    glVertex2f(750, 480);
    glVertex2f(900, 300);
    glEnd();
    //vẽ núi 2
    glColor3f(0.235, 0.602, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(800, 300);
    glVertex2f(950, 440);
    glVertex2f(1050, 300);
    glEnd();
    //vẽ núi 3
    glColor3f(0.235, 0.502, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(900, 300);
    glVertex2f(1050, 420);
    glVertex2f(1100, 300);
    glEnd();

    //vẽ đường bay 1
    glColor3f(0.3, 0.3, 0.3);//chọn màu xám
    drawRectangle(0.0, 0.0, SCREEN_WIDTH, 100.0);

    //vẽ đường bay 2
    glColor3f(0.5, 0.5, 0.5);//chọn màu xám++
    drawRectangle(0.0, 100.0, SCREEN_WIDTH, 100.0);

    //vẽ đường bay 3
    glColor3f(0.7, 0.7, 0.7);//chọn màu xám+++
    drawRectangle(0.0, 200.0, SCREEN_WIDTH, 100.0);

    glColor3f(1.0, 1.0, 0.0);//chọn màu vàng
    drawRocket(xRocket, yRocket);//gọi từ hàm vẽ rocket

    glColor3f(r, g, b);
    drawEnermy(xEnermy1, yEnermy1);//gọi từ hàm vẽ enemy

    glColor3f(r2, g2, b2);
    drawEnermy(xEnermy2, yEnermy2);//gọi từ hàm vẽ enemy

    ////
    //drawMay(xMay1, yMay1);
    //drawMay(xMay2, yMay2);
}
//hàm vẽ
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//xóa tất cả buffer
    glPushMatrix();//đưa ma trận vào
    if (!start)//nếu start khác false 
    {
        drawMenuScreen();//hiển thị màn hình chính
        score = 0;
    }        
    else  
        drawGame();//ngược lại thì vào chơi game
    glPopMatrix();//lấy ma trận ra
    glutSwapBuffers();//hoán đổi buffer
}
//hàm kiểm tra va chạm
bool CheckCollision(GLfloat xRocket1, GLfloat  yRocket1, GLfloat xheight1, GLfloat xWidth1, GLfloat xRocket2, GLfloat  yRocket2, GLfloat xheight2, GLfloat xWidth2)
{
    // va chạm trục x
    bool collisionX = xRocket1 + xWidth1 > xRocket2 &&
        xRocket2 + xWidth2 > xRocket1;
    // va chạm trục y
    bool collisionY = yRocket1 + xheight1 > yRocket2 &&
        yRocket2 + xheight2 > yRocket1;
    // xảy ra va chạm nếu chạm cả 2 trục x,y
    return collisionX && collisionY;
}
//hàm updateworld
void updateWorld() {
    int beginFrame = getMilliCount();//lấy thời gian hiện tại

    if (enermyPass > 5)//nếu chạy qua 5 enermy thì tăng tốc độ lên 5     
        speedE = 5;
    if (enermyPass > 10)//nếu chạy qua 10 enermy thì tăng tốc độ lên 6   
        speedE = 6;
    if (enermyPass > 15)//nếu chạy qua 15 enermy thì tăng tốc độ lên 8  
        speedE = 8;

    //kiểm tra va chạm
    if (CheckCollision(xRocket, yRocket, 100, 130, xEnermy1, yEnermy1, 100, 130))//thay đổi chiều dài,chiều rộng va chạm 1
        start = false;
    if (CheckCollision(xRocket, yRocket, 100, 130, xEnermy2, yEnermy2, 100, 130))//thay đổi chiều dài,chiều rộng va chạm 2
        start = false;
    xEnermy1 -= speedE;//chuyển động enemy 1
    xEnermy2 -= speedE;//chuyển động anemy 2

    xMay1 -= speedE;
    xMay2 -= speedE;



    if (xEnermy1 + 130 < 0) {//130 là độ dài của enemy
        xEnermy1 = SCREEN_WIDTH;
        yEnermy1 = (rand() % 3) * 100;//random vị trí enemy
        enermyPass++;//tăng sau khi vượt qua một enemy
        score++;//tăng điểm

        r = rand() % 10 * 0.1;
        g = rand() % 10 * 0.1;
        b = rand() % 10 * 0.1;
    }
    if (xEnermy2 + 130 < 0) {//130 là độ dài của enemy
        xEnermy2 = SCREEN_WIDTH;
        yEnermy2 = (rand() % 3) * 100;//random vị trí enemy
        enermyPass++;//tăng sau khi vượt qua một enemy
        score++;//tăng điểm

        r2 = rand() % 10 * 0.1;
        g2 = rand() % 10 * 0.1;
        b2 = rand() % 10 * 0.1;
    }
   
    glutPostRedisplay();
    int timeDriff = getMilliCount() - beginFrame;//thời gian chạy hết 1 frame
    if (timeDriff < TIMER) // 60 khung hình / giây
    {
        sleep(TIMER - timeDriff);
    }
}
//hàm các thao tác cần làm khi cửa sổ bị thay đổi kích thước
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}
//hàm bắt sự kiện bắt đầu game
void keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'w'://nhấn w để di chuyển lên
        if (yRocket < 200)
            yRocket += 20;//khoảng cách di chuyển của rocket trên tọa độ y
        break;
    case 's'://nhấn s để di chuyển lên
        if (yRocket > 0)
            yRocket -= 20;//khoảng cách di chuyển của rocket trên tọa độ y
        break;
    case ' '://chọn dấu cách sẽ khởi start game
        initGame();
        start = true;
        break;
    case 27: //ESC đóng màn hình
        init();
        start = false;
        break;
    default:
        break;
    }
}
//hàm main
int main(int argc, char* argv[])
{
    srand(time(NULL));//giúp cho hàm random không bị lặp lại
    glutInit(&argc, argv);//khởi tọa glut
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);//khởi tạo chế độ single buffer, hệ màu RGBA
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);//kích thước màn hình hiển thị
    glutInitWindowPosition(200, 20);//vị trí màn hình hiển thị
    glutCreateWindow("Rocket F1"); //tên game
    init();//hàm khởi tạo
    initGame();//hàm khởi tạo khi start game
    glutDisplayFunc(display);//đăng ký sự kiện cho hàm vẽ là hàm display
    glutReshapeFunc(reshape); //đăng ký hàm reshape cho sự kiện cửa sổ bị thay đổi kích thước 
    glutIdleFunc(updateWorld); //đăng ký sự kiện hàm update
    glutKeyboardFunc(keyboard); //đăng ký sự kiện bàn phím
    glutMainLoop();//bắt đầu chu trình lặp thể hiện vẽ
    return 0;
}

