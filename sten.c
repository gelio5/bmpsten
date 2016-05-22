#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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

int main(int argc, char * argv[]){
	int par;
	for (par=1;par<argc;++par){
		if (strcmp(argv[par], "-c")==0){
			struct stat st;
			FILE *sh,*bmp; //sh - файл который необхожимо зашифровать,bmp - исходный файл
			char nsh[30],isx[30];
				printf("Введите название BMP-файла, который будет взят за основу для шифрования.\n");
				scanf("%s",&isx[30]);
				printf("Введите название файла, который необходимо зашифровать в BMP-изображение, указанное выше.\n");
				scanf("%s",&nsh[30]);
				stat(&nsh[30],&st);
			long s=st.st_size;
			if (read(&isx[30]) == 0) 
				printf("Невозможно открыть\\прочитать изначальный файл\n");
			if (write(&isx[30])==0)
				printf("Невозможно записать в файл");
			long maxlen=BMPHDR.biWidth*BMPHDR.biHeight/8;
			if (maxlen<s)
				printf("Файл для шифрования не возможно поместить в данное изображение\n");
		int i=0;
		long step,size;
		}
	}
	return 0;
}

int read(char *name){
	FILE *fp;
	if ((fp=fopen(name, "rb"))==NULL) 
		return FALSE;	
	fread(&BMPHDR.bfType1, 1, 1, fp);
	fread(&BMPHDR.bfType2, 1, 1, fp);
	fread(&BMPHDR.bfSize, 4, 1, fp);
	fread(&BMPHDR.bfReserved1, 2, 1, fp);
	fread(&BMPHDR.bfReserved2, 2, 1, fp);
	fread(&BMPHDR.bfOffBits, 4, 1, fp);
	fread(&BMPHDR.biSize, 4, 1, fp);
	fread(&BMPHDR.biWidth, 4, 1, fp);
	fread(&BMPHDR.biHeight, 4, 1, fp);
	fread(&BMPHDR.biPlanes, 2, 1, fp);
	fread(&BMPHDR.biBitCount, 2, 1, fp);
	fread(&BMPHDR.biCompression, 4, 1, fp);
	fread(&BMPHDR.biSizeImage, 4, 1, fp);
	fread(&BMPHDR.biXPelsPerMeter, 4, 1, fp);
	fread(&BMPHDR.biYPelsPerMeter, 4, 1, fp);
	fread(&BMPHDR.biClrUsed, 4, 1, fp); 
	fread(&BMPHDR.biClrImportant, 4, 1, fp);	
	if(BMPHDR.bfType1 !='B' || BMPHDR.bfType2 !='M' || BMPHDR.biBitCount !=24) 
		return FALSE;
	int x=BMPHDR.biWidth, y=BMPHDR.biHeight;
	int nx=(3*x+3) & (-4);
		img=(unsigned char *) calloc(nx*y, sizeof(char));
		fread(img, 1, nx*y,fp);
		fclose(fp);
	return TRUE;
}
int write(char *name){
	FILE *fp;
	if ((fp=fopen(name, "wb"))==NULL)
		return FALSE;
	int x=BMPHDR.biWidth, y=BMPHDR.biHeight;
	int nx=(3*x+3) & (-4);
		fwrite(&BMPHDR.bfType1, 1, 1, fp);
		fwrite(&BMPHDR.bfType2, 1, 1, fp);
		fwrite(&BMPHDR.bfSize, 4, 1, fp);
		fwrite(&BMPHDR.bfReserved1, 2, 1, fp);
		fwrite(&BMPHDR.bfReserved2, 2, 1, fp);
		fwrite(&BMPHDR.bfOffBits, 4, 1, fp);
		fwrite(&BMPHDR.biSize, 4, 1, fp);
		fwrite(&BMPHDR.biWidth, 4, 1, fp);
		fwrite(&BMPHDR.biHeight, 4, 1, fp);
		fwrite(&BMPHDR.biPlanes, 2, 1, fp);
		fwrite(&BMPHDR.biBitCount, 2, 1, fp);
		fwrite(&BMPHDR.biCompression, 4, 1, fp);
		fwrite(&BMPHDR.biSizeImage, 4, 1, fp);
		fwrite(&BMPHDR.biXPelsPerMeter, 4, 1, fp);
		fwrite(&BMPHDR.biYPelsPerMeter, 4, 1, fp);
		fwrite(&BMPHDR.biClrUsed, 4, 1, fp); 
		fwrite(&BMPHDR.biClrImportant, 4, 1, fp);
		fwrite(img, sizeof (char), nx*y, fp);
		fclose(fp);
	return TRUE;
}