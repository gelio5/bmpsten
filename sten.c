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
int imgread(char *name);
int imgwrite(char *name);
unsigned char *img,*fl;
int test(unsigned char *img);
void aimwrite(unsigned char *img);
int flread(char *name);
long s,k=3;
void chng(int, int);

int main(int argc, char *argv[]){
	if ((argc!=5)||strcmp(argv[1],"-f")!=0||strcmp(argv[3],"-i")!=0){
		printf("Для работы программы необходимы ключи:\n");
		printf("-f\t-для обозначения файла который необходимо зашифровать.\n");
		printf("-i\t-для изображения в которое необходимо зашифровать.\n");
	}
		if (imgread(argv[4]) == 0){ 
			printf("Невозможно открыть\\прочитать изначальный файл\n");
			exit(3);
		}
		long maxlen=BMPHDR.biWidth*BMPHDR.biHeight;	
		if (test(img)==shif){
			struct stat st;
			stat(argv[2],&st);
			s=st.st_size;
			if (maxlen<s){
				printf("Файл для шифрования не возможно поместить в данное изображение\n");
				exit(2);	
			}
			FILE *Key;
				Key=fopen("y.ek","w");
				fprintf(Key, "%ld", s); //в данный файл записывает размер файла, который необходимо зашифровать
				fclose(Key);
				aimwrite(img);
				flread(argv[2]);
				int i,j;
				for (i=0;i<s;++i){ /* цикл, записывающий один символ в один пиксель, первые два бита символа в первый байт пикселя,
					последующие три бита символа во второй байт пикселя, оставшиеся три бита символа в третий байт*/
					j=fl[i]&128; //выделяем первый бит символа
					j>>=7; 
					chng(j, 1); //включаем/выключаем 7-ой бит в первом байте пикселя
					j=fl[i]&64; //выделяем второй бит символа
					j>>=6;
					chng(j, 0); //включаем/выключаем 8-ой бит в первом байте пикселя
					k++; //переходим ко второму байту пикселя
					j=fl[i]&32; //выделяем третий бит символа
					j>>=5;
					chng(j, 2); //включаем/выключаем 6-ой бит во втором байте пикселя
					j=fl[i]&16; //выделяем четвёртый бит символа
					j>>=4;
					chng(j, 1); //включаем/выключаем 7-ой бит во втором байте пикселя
					j=fl[i]&8; //выделяем пятый бит символа
					j>>=3;
					chng(j, 0); //включаем/выключаем 8-ой бит во втором байте пикселя
					k++; //переходим к третьему байту пикселя
					j=fl[i]&4; //выделяем шестой бит символа
					j>>=2;
					chng(j, 2); //включаем/выключаем 6-ой бит в третьем байте пикселя
					j=fl[i]&2; //выделяем седьмой бит символа
					j>>=1;
					chng(j, 1); //включаем/выключаем 7-ой бит в третьем байте пикселя
					j=fl[i]&1; //выделяем восьмой бит символа
					chng(j, 0); //включаем/выключаем 8-ой бит в третьем байте пикселя
					k++; //переходим к первому байту следующего пикселя
				}	
				imgwrite(argv[4]);
		}else{
			FILE *Key;
			if ((Key=fopen("y.ek","r"))==0){
				printf("Файл, содержащий ключ для дешифрования, не найден.");
				exit(1);
			}
			fscanf(Key, "%ld", &s);
			fclose(Key);
			unsigned char b, c=0;
			FILE *ex;
			ex=fopen(argv[2],"wb");
			for (k=3;k<s*3+3; k=k+3){ //в файле s байт, каждый закодирован в 1 пиксель (в 3 байта) и первые три байта занимает символ %
				b=img[k]&3; //выделяем два последних бита, умножаем на 3 (11)
				c+=b;
				c=c<<3;
				b=img[k+1]&7; //выделяем три последних бита, умножаем на 7 (111)
				c+=b;
				c=c<<3;
				b=img[k+2]&7; //выделяем три последних бита, умножаем на 7 (111)
				c+=b;
				fwrite(&c,1,1,ex);
				c=0;}
			fclose(ex);
		}	
	return 0;
}
int imgread(char *name){ //функция чтения из bmp файла
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
	int nx=3*x;
		img=(unsigned char *) calloc(nx*y, sizeof(char));
		fread(img, 1, nx*y,fp);
		fclose(fp);
	return TRUE;
}

int imgwrite(char *name){ //функция записи в bmp файл
	FILE *fp;
	if ((fp=fopen(name, "wb"))==NULL)
		return FALSE;
	int x=BMPHDR.biWidth, y=BMPHDR.biHeight;
    int nx=3*x;
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
int test(unsigned char *img){ //в один пиксель шифруется уникальная последовательность (в данном случает это %, в двоичном виде 00100101)
	if (img[0]&3!=0) return shif; //в первом байте пикселя проверям два последних бита (должны быть 00), поэтому маска 3 (11)
	if (img[1]&7!=4) return shif; //во втором байте пикселя проверям три последних бита (должны быть 100), поэтому маска 7 (111)
	if (img[2]&7!=5) return shif; //в третьем байте пикселя проверям три последних бита (должны быть 101), поэтому маска 7 (111)
	else
			return deshif;
}	
void aimwrite(unsigned char *img){ //записываем в первый пиксель уникальную последовательность (%, в двоичном виде 00100101)
	img[0]&=252; //в первом байте пикселя заменяем два последних бита на 00, маска: &=252, т. е. &=11111100
	img[1]&=252; 
    img[1]|=4; //во втором байте пикселя заменяем три последних бита на 100, маска (img[1]&252)|4, т. е. (img[1]&11111100)|100
	img[2]&=252;  
	img[2]|5; //в третьем байте пикселя заменяем три последних бита на 101, маска (img[1]&252)|5, т. е. (img[1]&11111100)|101       
}
int flread(char *name){ //функция, читающая данные из файла
	FILE *fp;
	if ((fp=fopen(name, "rb"))==NULL) 
		return FALSE;	
  		fl=(unsigned char *) calloc(s,sizeof(char));
  		fread(fl,1,s,fp);
  		fclose(fp);
  	return TRUE;
}
void chng(int j, int z){ //функция включающая/выключающая бит
	if (j==1)
		img[k]|=1<<z; //включение бита
	else
		img[k]&=~(1<<z); //выключение бита
}	

