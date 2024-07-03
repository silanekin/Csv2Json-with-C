#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>


int main(int argc, char *argv[]) {
	
	setlocale(LC_ALL, "Turkish");
	
	FILE *csvDosya , *jsonDosya;
	printf("CSV dosyas�n� JSONA'a donusturme %s\n", *argv);
	
	
	char okunanKelime[50];
	bool Satir = true;
	int Sutun = 0;
	char *kolon = "Kolon";
	char Anahtar[20][200];
	
	//Csv Dosyas� komut sat�r�nda ilk arg�mand�r
	csvDosya = fopen(argv[1], "r");
	
	if(csvDosya == NULL)
	{
		printf ("Dosya acilamadi");
		exit(0);
	}
    //Json dosyas� komut sat�r�nda ikinci arg�mand�r
	jsonDosya = fopen(argv[2], "w");
	
	if (jsonDosya == NULL)
	{
		printf ("json dosyasi acilmadi");
    	exit(1);
    }
	
	char okunanKarakter;
	int i;
	
	fprintf (jsonDosya, "{\n");
	
	//Csv dosyas�n�n sonuna kadar okunmas�
	while(!feof(csvDosya)) {
	  //Csv dosyas�ndaki karakterleri tek tek okumak i�in
		okunanKarakter = getc(csvDosya);
		
		if (Satir) {
			
			//Komut sat�r�ndaki ���nc� arg�man header header="on" iken csv jsona �evrilir
			if (!strcmp(argv[3], "header=ON")) {
				
				//Okunan karakter yeni sat�rsa false d�nd�r�p if kapat�l�r
				if (okunanKarakter == '\n')
				{
					Satir = false;
					Sutun = 0;
					i = 0;
				}
				
				else if (okunanKarakter == ',')
				{
					Sutun++;
					i = 0;
				}	
		        else
				{
					if (okunanKarakter != ' ')
					{
						Anahtar[Sutun][i] = okunanKarakter;
						i++;
					}
				}
			}
			
			//header="off" ise jsonu yazd�r�r
			else if (!strcmp(argv[3], "header=OFF")) {
				 Satir = false;
			}
		    }
		    
		else {
			if (okunanKarakter == ',') 
			{  i = 0;
			
				if (Sutun == 0)
					fprintf(jsonDosya, "\t{");
				
				if (!strcmp(argv[3], "header=ON")) 
				{
					
					fprintf(jsonDosya,"\n\t\"%s\"", Anahtar[Sutun]);
					fprintf(jsonDosya,": \"%s\",", okunanKelime);
					
				}
				
				else if (!strcmp(argv[3], "header=OFF")) 
				{
					
					fprintf(jsonDosya, "\n\t Kolon %d\"",Sutun+1);
					fprintf(jsonDosya, ": \"%s\",", okunanKelime);
				}
				
				Sutun++;
				
				//okunan kelime s�f�rlan�r yeniden kullan�labilmesi i�in
				memset(okunanKelime,0,sizeof okunanKelime);
			}
			
			else if (okunanKarakter == '\n') 
			{
				if (!strcmp(argv[3], "header=ON")) 
				{
					
					fprintf(jsonDosya, "\n\t\"%s\""), Anahtar[Sutun];
					fprintf(jsonDosya, ": \"%s\"\n\t},t \n",okunanKelime);
				}
				
				else if (!strcmp(argv[3], "header=OFF")) 
				{
				
					fprintf(jsonDosya, "\n\t\"Kolon %d\"",Sutun+1);
					fprintf(jsonDosya,":\"%s\"\n\t},\n",okunanKelime);
				}
				
				memset(okunanKelime,0,sizeof okunanKelime);
				Sutun = 0;
				i = 0;
			}
			else {
				if (okunanKarakter != ' ') {
					okunanKelime[i] = okunanKarakter;
					i++;
				}
			}
		}
	}
	
	fprintf(jsonDosya, "}");
	
	fclose(csvDosya);
	fclose(jsonDosya);
	
	
	
	
	return 0;
}
