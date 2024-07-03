#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>


int main(int argc, char *argv[]) {
	
	setlocale(LC_ALL, "Turkish");
	
	FILE *csvDosya , *jsonDosya;
	printf("CSV dosyasýný JSONA'a donusturme %s\n", *argv);
	
	
	char okunanKelime[50];
	bool Satir = true;
	int Sutun = 0;
	char *kolon = "Kolon";
	char Anahtar[20][200];
	
	//Csv Dosyasý komut satýrýnda ilk argümandýr
	csvDosya = fopen(argv[1], "r");
	
	if(csvDosya == NULL)
	{
		printf ("Dosya acilamadi");
		exit(0);
	}
    //Json dosyasý komut satýrýnda ikinci argümandýr
	jsonDosya = fopen(argv[2], "w");
	
	if (jsonDosya == NULL)
	{
		printf ("json dosyasi acilmadi");
    	exit(1);
    }
	
	char okunanKarakter;
	int i;
	
	fprintf (jsonDosya, "{\n");
	
	//Csv dosyasýnýn sonuna kadar okunmasý
	while(!feof(csvDosya)) {
	  //Csv dosyasýndaki karakterleri tek tek okumak için
		okunanKarakter = getc(csvDosya);
		
		if (Satir) {
			
			//Komut satýrýndaki üçüncü argüman header header="on" iken csv jsona çevrilir
			if (!strcmp(argv[3], "header=ON")) {
				
				//Okunan karakter yeni satýrsa false döndürüp if kapatýlýr
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
			
			//header="off" ise jsonu yazdýrýr
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
				
				//okunan kelime sýfýrlanýr yeniden kullanýlabilmesi için
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
