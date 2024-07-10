#include <iostream>
#include <windows.h>
#include <string.h>
#include "graphics.h"

int xmouse = 0,ymouse = 0;//���������� ������� (�������� ����������� � ��������)
int gametable[11][11];//������ �������� ����
int copy_gametable[11][11];//����� ������� �������� ����
int computer_xy[2]= {1,9}; //������ ������ ����������

void filling();//������� ���������� ���������� �������� ����
void menu();// ������� ��������� ����
void buttons();//������� ���������� ������� �� ������
void about();//������� ��������� ���� "�� ����"
void rules();//������� ��������� ���� "�������"
void game();//������� ��������� ���� " ������ ����"
void draw_turn(int colo);//������� ��������� ���� "����� %���� ������%"
void draw_winner(int colo);//������� ��������� ���� "������� %���� ������% ����� !%"
void logic_comp(int x,int y);//������� ������ ����������

int game_engine(int x, int y, int player);//������� ���������� ������� "���������"
int winner_question(int x, int y, int who, int input_array[11][11]);//������� �������� ������ ������
IMAGE *l;
IMAGE *ll;
IMAGE *lll; 
IMAGE *llll;
IMAGE *lllll;
IMAGE *llllll;
IMAGE *lllllll;
IMAGE *llllllll;
int main() {
   
   initwindow(1200, 800, "Bridge-it",200,200);
   l = loadBMP("menu.bmp");
   ll = loadBMP("about.bmp");
   lll= loadBMP("rules.bmp");
   llll = loadBMP("game3.bmp");
   lllll = loadBMP("red_turn.bmp");
   llllll = loadBMP("blue_turn.bmp");
   lllllll= loadBMP("red_win.bmp");
   llllllll = loadBMP("blue_win.bmp");
   menu();
}

void menu() { //������� ��������� ����
   filling();
   clearviewport();
   
   putimage(0,0,l,OR_PUT);
   buttons();
}

void filling() { // ������� ���������� ���������� �������� ����
   for (int i = 1; i < 11; i = i + 2)
      for (int j = 0; j < 11; j = j + 2) {
         if (i % 2 == j % 2)
            gametable[i][j] = 0;
         if (i % 2 != j % 2 || i == 0 || j == 0 || i == 10 || j ==10)
            gametable[i][j] = 2;
      }

   for (int i = 0; i < 11; i = i + 2)
      for (int j = 1; j < 11; j = j + 2) {
         if (i % 2 == j % 2)
            gametable[i][j] = 0;
         if (i % 2 != j % 2 || i == 0 || j == 0 || i == 10 || j ==10)
            gametable[i][j] = -2;
      }

   for (int i = 0; i < 11; i++)
      gametable[i][0] = 2;
   for (int i = 0; i < 11; i++)
      gametable[i][10] = 2;
   for (int j = 0; j < 11; j++)
      gametable[0][j] = -2;
   for (int j = 0; j < 11; j++)
      gametable[10][j] = -2;

   gametable[0][0] = 3;
   gametable[0][10] = 3;
   gametable[10][0] = 3;
   gametable[10][10] = 3;

   for (int i = 0; i < 11; i++)//��������� �������� ���� ����� ���������� ����
      for (int j = 0; j < 11; j++)
         if (gametable[i][j] == 1 || gametable[i][j] == -1)
            gametable[i][j] = 0;

   for (int i = 0; i < 11; i++)
      for (int j = 0; j < 11; j++)
         copy_gametable[i][j] = gametable[i][j];

   computer_xy[0] = 1;//��������� ������ ����������
   computer_xy[1] = 9;
}

int keyPressed(int key) {
   return (GetAsyncKeyState(key) & 0x8000 != 0);
}

void buttons() { // ������� ���������� ������� �� ������
   while (1) {
      xmouse = mousex();
      ymouse = mousey();

      if (!keyPressed(VK_LBUTTON)) {
         continue;
      }

      if (xmouse > 71 && xmouse < 290)//��������� ��������� ������� ������
         if (ymouse > 221 && ymouse < 290) { // ������� �� "������ ����"
            game();
            break;
         }
         else if (ymouse > 317 && ymouse < 388) { // ������� �� "�������"
            rules();
            break;
         }
         else if (ymouse > 416 && ymouse < 482) { // �������  ��"�� ����"
            about();
            break;
         }
         else if (ymouse > 511 && ymouse < 579) { // ������� �� "�����"
            exit(0);
            break;
         }
   }
}

void about() { // ������� ��������� ���� "�� ����"
   clearviewport();
   
   putimage(0,0,ll,OR_PUT);

   buttons();
}

void rules() { // ������� ��������� ���� "�������"
   clearviewport();
   
   putimage(0,0,lll,OR_PUT);

   buttons();
}

void game() { // ������� ��������� ���� "������ ����"(�������� ������ ������� � ��������� "��������")
   int player = -1,// ������ �����(1 - ����� , -1 - �������)
       winner = 0;// ���������� ���������� �� ������ � ���������� ����������� ������(1 - �����, -1 - �������, 0 - �����)

   clearviewport();
   
   putimage(0,0,llll,OR_PUT);

   while (1) {
      xmouse = mousex();
      ymouse = mousey();

      if (mousebuttons()==1 && xmouse > 1019 && xmouse < 1183 && ymouse > 20 && ymouse < 70) { // �������� �� ������� ������ "� ����"
         menu();
      }
      else if ((mousebuttons()==1 || player == -1) && winner == 0) { // ������� ����, � ����� ����������� ����� ����� �������� ������
         int what;//����������, ����������� �� ��, ��������� �� ������� �����(1 - ���� ��� ��������� � ����� ���������� ����, 0 - ����� ������ ������ ��������)
         xmouse = (xmouse - 219) / 69;
         ymouse = (ymouse - 19) / 69;//������� �� ��������� ����� � ����������� ������� �������� ����

         draw_turn(player);//

         if (player == 1) //
            what = game_engine(xmouse,ymouse,player);
         if (player == -1) {
            what = game_engine(computer_xy[0],computer_xy[1],player);
            delay(60);
         }

         if (what == 1 && player == 1)//��������� �������� ������
            if (gametable[xmouse+1][ymouse] == -2 && gametable[xmouse - 1][ymouse] == -2) {
               setfillstyle(SOLID_FILL, COLOR(71, 84, 255));
               bar(xmouse*69 + 202,ymouse*69 + 36,xmouse*69 +315,ymouse*69 + 71);
            } else if (gametable[xmouse][ymouse+1] == -2 && gametable[xmouse][ymouse-1] == -2) {
               setfillstyle(SOLID_FILL, COLOR(71, 84, 255));
               bar(xmouse*69+236,ymouse*69-7,xmouse*69+271,ymouse*69+115);
            }

         if (what == 1 && player == -1)// ��������� �������� ����������
            if (gametable[computer_xy[0]+1][computer_xy[1]] == 2 && gametable[computer_xy[0] - 1][computer_xy[1]] == 2) {
               setfillstyle(SOLID_FILL, COLOR(255, 71, 71));
               bar(computer_xy[0]*69 + 202,computer_xy[1]*69 + 36,computer_xy[0]*69 +315,computer_xy[1]*69 + 71);
            } else if (gametable[computer_xy[0]][computer_xy[1]+1] == 2 && gametable[computer_xy[0]][computer_xy[1]-1] == 2) {
               setfillstyle(SOLID_FILL, COLOR(255, 71, 71));
               bar(computer_xy[0]*69+236,computer_xy[1]*69-7,computer_xy[0]*69+271,computer_xy[1]*69+115);
            }

         if (player == 1) //�������� ������� � ��������� ����� ������ � ����������
            for (int i = 1; i < 11 && winner == 0; i = i+2)
               if (gametable[i][1] == player)
                  winner = winner_question(1,i,player,gametable);
         if (player == -1)
            for (int i = 1; i < 11 && winner == 0; i = i+2)
               if (gametable[1][i] == player)
                  winner = winner_question(i,1,player,gametable);
         if (winner != 0) //���� �������� ���������� � ����, �� ��������� ����
            break;

         if (player == 1 && what == 1) { //����� ��������� ������ ����� ����
            player = -1;
            logic_comp(xmouse,ymouse);//������� �������� ���������� � ����� �� ��� ������
         }
         else if (player == -1 && what == 1)
            player = 1;

      }
   }
   draw_winner(winner);//������� ��������� ���� "����� %���� ������%"

   while (1) {
      xmouse = mousex();
      ymouse = mousey();

      if (mousebuttons()==1 && xmouse > 1019 && xmouse < 1183 && ymouse > 20 && ymouse < 70)// �������� �� ������� ������ "� ����"
         menu();
   }
}

int game_engine(int x, int y, int who) { //������� ���������� ������� "���������"
   if (gametable[y][x] == 0)//���� �����, ���� ����� ����� ��������� ������ ������, �������� ��� , ������ �������� ���������� what = 0
      gametable[y][x] = who;
   else
      return 0;
   return 1;
}

int winner_question(int x, int y, int who,int input_array[11][11]) { //������� �������� ������ ������
   int output_array[11][11],
       retaliate = 0;//���������� ��������� ���������� � ���, ���� �� ����� ����� �� ������ ������� ����

   for (int i = 0; i < 11; i++)
      for (int j = 0; j < 11; j++)
         output_array[i][j] = input_array[i][j];
   output_array[y][x] = 5;//���������� ������� ��� ����������� ��������

   if (who == 1 && x == 10) //� ����������� �� ������, �������� ������(����� �� �������� �� ������� ����� �����)
      retaliate = 1;
   if (who == -1 && y == 10)
      retaliate = -1;

   if ((output_array[y+1][x] == who || output_array[y+1][x] == who * 2)&& retaliate == 0 && y+1>0 && y+1 <11)//�������� ������ � ����� ����� ������� ����� ������
      retaliate = winner_question(x, y+1, who, output_array);
   if ((output_array[y-1][x] == who || output_array[y-1][x] == who * 2)&& retaliate == 0 && y-1>0 && y-1 <11)
      retaliate = winner_question(x, y-1, who, output_array);
   if ((output_array[y][x+1] == who || output_array[y][x+1] == who * 2)&& retaliate == 0 && x+1> 0 && x+1 <11)
      retaliate = winner_question(x+1, y, who, output_array);
   if ((output_array[y][x-1] == who || output_array[y][x-1] == who * 2)&& retaliate == 0 && x-1 >0 && x-1 <11)
      retaliate = winner_question(x-1, y, who, output_array);

   return retaliate;
}

void draw_turn(int colo) { //������� ��������� ���� "����� %���� ������%" (�� ������ ��������� ��������)
   setfillstyle(SOLID_FILL, COLOR(255, 255, 255));
   bar(20,20,200,200);
   
   if (colo == -1)
      putimage(20,20,lllll,AND_PUT);

   if (colo == 1)
      putimage(20,20,llllll,AND_PUT);

}

void draw_winner(int colo) { //������� ��������� ���� "������� %���� ������% ����� !%"

   if (colo == 1)
      putimage(1020,80,llllllll,AND_PUT);
   if (colo == -1)
      putimage(1020,80,lllllll,AND_PUT);
}



void logic_comp(int x,int y) { //������� ������ ����������
   if (y == 1 && x == 1) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 1 && x == 3) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 1 && x == 5) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 1 && x == 7) {
      computer_xy[1] = y;
      computer_xy[0] = x + 2;
   } else if (y == 1 && x == 9) {
      computer_xy[1] = y;
      computer_xy[0] = x - 2;
   } else if (y == 2 && x == 2) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 2 && x == 4) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 2 && x == 6) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 2 && x == 8) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 3 && x == 1) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 3 && x == 3) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 3 && x == 5) {
      computer_xy[1] = y;
      computer_xy[0] = x + 2;
   } else if (y == 3 && x == 7) {
      computer_xy[1] = y;
      computer_xy[0] = x - 2;
   } else if (y == 3 && x == 9) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 4 && x == 2) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 4 && x == 4) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 4 && x == 6) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 4 && x == 8) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 5 && x == 1) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 5 && x == 3) {
      computer_xy[1] = y;
      computer_xy[0] = x + 2;
   } else if (y == 5 && x == 5) {
      computer_xy[1] = y;
      computer_xy[0] = x - 2;
   } else if (y == 5 && x == 7) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 5 && x == 9) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 6 && x == 2) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 6 && x == 4) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 6 && x == 6) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 6 && x == 8) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 7 && x == 1) {
      computer_xy[1] = y;
      computer_xy[0] = x + 2;
   } else if (y == 7 && x == 3) {
      computer_xy[1] = y;
      computer_xy[0] = x - 2;
   } else if (y == 7 && x == 5) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 7 && x == 7) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 7 && x == 9) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 8 && x == 2) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 8 && x == 4) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 8 && x == 6) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 8 && x == 8) {
      computer_xy[1] = y + 1;
      computer_xy[0] = x + 1;
   } else if (y == 9 && x == 3) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 9 && x == 5) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 9 && x == 7) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   } else if (y == 9 && x == 9) {
      computer_xy[1] = y - 1;
      computer_xy[0] = x - 1;
   }
}