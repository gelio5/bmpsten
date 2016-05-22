#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define TRUE 1
#define FALSE 0
#define shif 5
#define deshif 9
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
int read(char *name);
int write(char *name);
unsigned char *img;
int test(unsigned char *img);
void aimwrite(unsigned char *img);

int main(int argc, char *argv[]){
	if ((argc!=5)||strcmp(argv[1],"-f")!=0||strcmp(argv[3],"-i")!=0){
		printf("Для работы программы необходимы ключи:\n");
		printf("-f\t-для обозначения файла который необходимо зашифровать.\n");
		printf("-i\t-для изображения в которое необходимо зашифровать.\n");
	}
	struct stat st;
		stat(argv[2],&st);
	long s=st.st_size;
		if (read(argv[4]) == 0){ 
			printf("Невозможно открыть\\прочитать изначальный файл\n");
			exit(3);
		}
		if (write(argv[4])==0){
			printf("Невозможно записать в файл");
			exit(3);
		}
	long maxlen=BMPHDR.biWidth*BMPHDR.biHeight/8*3;
		if (maxlen<s){
			printf("Файл для шифрования не возможно поместить в данное изображение\n");
			exit(2);
		}
		if (test(img)==5){
			long step=maxlen/s+1;
			FILE *Key;
			Key=fopen("y.ek","wb");
			fwrite(&step,4,1,Key);
			fclose(Key);
			aimwrite(img);
		}else{
			FILE *Key;
			unsigned int key;
				if (fopen("y.ek","rb")==0){
					printf("Файл, содержащий ключ для дешифрования, не найден.");
					exit(1);
				}
				Key=fopen("y.ek","rb");
				fread(&key,4,1,Key);
				fclose(Key);
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

int test(unsigned char *img){
	if (img[0]&1!=1) return shif;
	if (img[1]&1!=1) return shif;
	if (img[2]&1!=0) return shif;
	if (img[3]&1!=0) return shif;
	if (img[4]&1!=0) return shif;
	if (img[5]&1!=0) return shif;
	if (img[6]&1!=0) return shif;
	if (img[7]&1!=0) return shif;
	if (img[8]&1!=1) return shif;
	if (img[9]&1!=1) return shif;
	if (img[10]&1!=1) return shif;
	if (img[11]&1!=0) return shif;
	if (img[12]&1!=0) return shif;
	if (img[13]&1!=1) return shif;
		else
			return deshif;
}
void aimwrite(unsigned char *img){
	img[0]|=1;
	img[1]|=1;
	img[2]&=254;
	img[3]&=254;
	img[4]&=254;
	img[5]&=254;
	img[6]&=254;
	img[7]&=254;
	img[8]|=1;
	img[9]|=1;
	img[10]|=1;
	img[11]&=254;
	img[12]&=254;
	img[13]|=1;
}