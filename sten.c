#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#define TRUE 1
#define FALSE 0
#pragma pack(push, 2)
struct BMPheader{
	unsigned char bfType1, bfType2; //тип файла (BM)
	unsigned long bfSize; //размер файла в байтах
	unsigned short bfReserved1; //
	unsigned short bfReserved2; //
	unsigned long bfOffBits; //местонахождение битового массива
	unsigned long biSize; //размер структуры
	unsigned long biWidth; //ширина картинки в пикселях	
	unsigned long biHeight; //высота картинки в пикселях
	unsigned short biPlanes; //количество плоскостей
	unsigned short biBitCount; //количество бит на один пиксель
	unsigned long biCompression; //тип сжатия
	unsigned long biSizeImage; //размер картинки в байтах
	unsigned long biXPelsPerMeter; //горизонтальное разрешение
	unsigned long biYPelsPerMeter; //вертикальное разрешение
	unsigned long biClrUsed; //количество используемых цветов
	unsigned long biClrImportant; //количество важных цветов
}BMPHDR;
#pragma pack(pop)
int read(char *name), write(char *name);
unsigned char *img;

int main(void){
	if (read("test.bmp") == 0) {
		printf("всё плохо\n");
	}
	if (write("3.bmp") == 0) {
		printf("всё плохо\n");
	}
	int maxlen=BMPHDR.biWidth*BMPHDR.biHeight/8;
	char s[maxlen+2];
	int i=0;
	int step,size;
		//fgets(s,maxlen+2,stdin);
		//s[strlen(s)-1]='\0';
		//if (strlen(s)>maxlen)
		//	printf("Длина введенной строки превышает максимальное допустимое количество символов для шифрования\n");
	//	else{
	//		step=maxlen/strlen(s);
	//		size=strlen(s);
	//	}
	return 0;
}
