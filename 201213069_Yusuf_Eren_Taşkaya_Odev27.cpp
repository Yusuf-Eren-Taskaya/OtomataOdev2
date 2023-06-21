#include<stdio.h>
#include <stdlib.h>
#include<string.h>


#define MAX_SAT_UZ 100


int durumSayisi=0;
int sutunSayisi=0;

int** gecisTablosu;
char* basliklar;
char girisStringi[MAX_SAT_UZ];


int CharSay(char* s,char c)
{
	int sy=0;
	int i=0;
	while(s[i]!='\0')
	{
		if(s[i]==c)sy++;
		i++;
	}
	return sy;
}

void DosyaOku()
{
    /***************************************
    GECISDIYAGRAMI.TXT girislere gore, gecis ve cikti verilerini tasir. Dosyasi formati;
    ilk satir basliklari tasir ve tek karakterden olusur. her deger tab karakteriyle ayrilir.
    Baslik;
        Q   a   b   ...
        ilk veri durum semboludur (Q), ondan sonraki degerler giris alfabesini olusturan sembol karakterleridir

    Basliktan sonraki satirlarda;
        ilk sutun durum indislerini tasir
        daha sonraki sutunlar, her giris sebolu icin ikiserli gruplar halinde degerlendirilir;
            grubun ilk sutunu GecilecekDurumIndisini, ikinci sutunu ise CikisDegerini belirtir

        her sutun tek bir tab ile ayrilir.
    Ornek
        Q   a   b               -> baslik satiri giris alfabesi ={a,b}
        0	2	0	3	1       -> 0. durumda iken; a girisi olursa gecilecek durum indisi 2, cikis degeri 0 dir,
                                                    b girisi olursa gecilecek durum indisi 3, cikis degeri 1 dir,
    ****************************************/
	char satir[MAX_SAT_UZ];
	int temp=0;
	FILE *fp;
	if ((fp=fopen("GECISDIYAGRAMI.TXT", "r")) == NULL) {
      printf("GECISDIYAGRAMI.TXT Dosyasi acilamadi!\n");
      exit(1);
	}

	if(fgets(satir, MAX_SAT_UZ, fp)==NULL)
    {
        printf("Dosyada uygun veri yok.!\n");
        exit(1);
    }
    else
    {
        // sutun sayisi bulunuyor
        sutunSayisi=CharSay(satir,'\t')+1;
        if(sutunSayisi<2)
        {
            printf("Dosyada en az bir durum, bir giris verisi bulunmali.!\n");
            exit(1);
        }
        basliklar =(char*)malloc(sutunSayisi * sizeof(char));// basliklik dizisi olusturuluyor
        for(int i=0;i<strlen(satir);i++)
        {
            if(satir[i]!='\t')
            {
                basliklar[temp++]=satir[i];   // basliklar diziye ataniyor
            }
        }
    }

    while (fgets(satir, MAX_SAT_UZ, fp)!=NULL)
    {
        //satirlar sayilarak durum sayisi bulunuyor
        durumSayisi++;
    }
    fclose (fp);


    gecisTablosu =(int**)malloc(durumSayisi * sizeof(int*));   //2 boyutlu gecis tablosu icin yer ayriliyor
    for(int i=0;i<durumSayisi;i++)
    {
        gecisTablosu[i]=(int*)malloc((((sutunSayisi-1)*2)+1) * sizeof(int));
    }

    fp=fopen("GECISDIYAGRAMI.TXT", "r");
    fgets(satir, MAX_SAT_UZ, fp);// baslik satiri geciliyor

    for(int i=0;i<durumSayisi;i++)
    {
        for(int j=0;j<(((sutunSayisi-1)*2)+1);j++)
        {
            fscanf(fp,"%d",&gecisTablosu[i][j]);
           // printf("%d  ",gecisTablosu[i][j]);
        }
        //printf("\n");
    }
    fclose(fp);
}

void TabloyuYaz()
{
    int temp=0;
    int gck=0;
    printf("             Durum Kumesi :{");
    for(int i=0;i<durumSayisi;i++)printf("%c%d,",basliklar[0],i);
    printf("\b}\n");
    printf("    Giris sembol Alfabesi :{");
    for(int i=1;i<sutunSayisi;i++)printf("%c,",basliklar[i]);
    printf("\b}\n");
    printf("Cikti sembolleri Alfabesi :{");
    int* ck= (int*)malloc((sutunSayisi-1)*durumSayisi * sizeof(int));
    int cikisSayisi=0;
    for(int i=0;i<durumSayisi;i++)
    {

        for(int j=2;j<=((sutunSayisi-1)*2);j+=2)
        {
            temp=0;
            for(int k=0;k<cikisSayisi;k++)
            {
                if(ck[k]==gecisTablosu[i][j])
                {
                    temp=1;
                    break;
                }
            }
            if(temp==0)ck[cikisSayisi++]=gecisTablosu[i][j];
        }
    }

    for(int i=0;i<cikisSayisi;i++)
    {
        printf("%d,",ck[i]);
    }
    printf("\b}\n\n");

    printf("Gecis Diyagram Tablosu\n");
    printf("-----------------------------\n");

    printf("      |");
    for(int j=1;j<sutunSayisi;j++)
    {
        printf("  Giris %c  |",basliklar[j]);
    }
    printf("\nMevcut|");
    for(int j=1;j<(((sutunSayisi-1)*2)+1);j+=2)
    {
        printf("Yeni :     |");
    }
    printf("\nDurum |");
    for(int j=1;j<(((sutunSayisi-1)*2)+1);j+=2)
    {
        printf("Durum:Cikis|");
    }
    printf("\n");

    for(int i=0;i<durumSayisi;i++)
    {
        printf("  Q%d  |",gecisTablosu[i][0]);
        for(int j=1;j<(((sutunSayisi-1)*2)+1);j+=2)
        {
            printf("  Q%d :  %d  |",gecisTablosu[i][j],gecisTablosu[i][j+1]);
        }
        printf("\n");
    }
}

int GirisSembolIndisi(char s)
{
    //Giris karakterini tablodaki index numarasini dondurur
    int ndx=0;
    for(int i=1;i<sutunSayisi;i++)
    {
        if(basliklar[i]==s)
        {
            ndx=i;
            break;
        }
    }
    ndx--;
    if(ndx<0 || ndx>=sutunSayisi-1)
    {
        printf("\n'%c' karakteri giris alfabesine uygun degil.!\n",s);
        exit(1);
    }
    return ndx;
}
void DurumlarArasiGecis(char * giris)
{
    int drm=0,drm_old=0,girTabloSut=0;

    printf("Mevcut           Gecilen  \n");
    printf("Durum   Giris    Durum    Cikis\n");
    printf("-----   -----    -----    -----\n");
    for(int i=0;i<strlen(giris);i++)
    {
        drm_old=drm;
        girTabloSut=GirisSembolIndisi(giris[i])*2+1;
        drm=gecisTablosu[drm][girTabloSut];
        printf("  Q%d      %c        Q%d      %d\n",drm_old,giris[i],drm,gecisTablosu[drm_old][girTabloSut+1]);
    }
}
int main()
{
    DosyaOku();
    TabloyuYaz();
    printf("\nGiris stringini gir (arada bosluk birakmadan) :");
    scanf("%s",girisStringi);
    printf("\n");
    DurumlarArasiGecis(girisStringi);
    getchar();
}

