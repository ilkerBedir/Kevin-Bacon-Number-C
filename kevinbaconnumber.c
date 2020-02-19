#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
typedef struct actor{//aktor structi
	char name [100];//aktorun ismi
    struct movie **films;//aktorun filmleri
    int film_count;//aktorun oynadigi film sayilari
    short visited;//bfs de kuyru�a att�ktan sonra tek at�lmas�n� �nlemek i�in visited  degiskeni
	struct movie *prefilm;//kuyruk yolunu yazd�r�rken hangi filmden geldi�ini anlamka i�in kullan�ld�
}actor;

typedef struct movie{//film struct�
	char mname[100];//filmlerin ad�
    struct actor **actors ;//filmde oynayan aktorlerin dizisi
    short visited;//bfs de kuyru�a att�ktan sonra tek at�lmas�n� �nlemek i�in visited  degiskeni
    struct actor *preactor;//kuyruk yolunu yazd�r�rken hangi aktorden geldi�ini anlamak i�in kullan�ld�
    int actor_count;//filmde oynayan aktorler
}movie;

struct actor **aktorkuyruk;//bfs i�lemleri 2 kuyruk tutarak yapt�m,Kuyruklardan ilki aktor kuyru�u
struct movie **filmkuyruk;//ikinci film kuyrugu
int filmbaslangic=-1;//film kuyru�undaki ba�lang�� indisi
int filmbitis=0;//film kuyru�unda biti� indisi
int aktorbaslangic=-1;//aktor kuyru�undaki ba�lang�� indisi
int aktorbitis=0;//aktor kuyru�unda biti� indisi

void filmekle (struct movie * film);//film kuyru�una film ekleme fonksiyonu
void filmsil();//film kuyru�unda film silme fonksiyonu
void aktorekle(struct actor*aktor);//aktor kuyru�una aktor eklem fonksiyonu
void aktorsil();//aktor kuyru�undan aktor silme fonksiyonu
void yolgoster(actor* aktor2, int yol);//bulunan yolu gosterme
int dosyaSatirSayisiBulma();//dosyada ka� tane film oldu�unu bulma fonksiyonu
int hash(char *icerik,int mod);//aktorler ve filmler dizileri i�in hash fonksiyonu
void BFS ( actor aktorler[250000], movie movies[30000],int secim);//Breadth First Search in yapildigi yer ayr�ca kullan�c�dan ismlerin al�nd��� yer

int main(int argc, char *argv[]) {//main fonksiyonumda dosyadab gelen bilgileri okuyarak bipartite graf olu�turuyorum
	int satirsayisi,i;//satirsayisi dosyadan gelen sayi tutmak i�in, i ise indis i�in
	char ch,name[100],fname[100];//fname dosyadan gelen filmin ismini tutmak i�in, name ise aktor ismini tutmak i�in,ch ise karakter tutmak i�in
	int f,j,c;//f dosyadan gelen de�erin filmin mi aktor mu oldugunu belirten flag,j d�ng� de�i�keni,c film count de�i�keni
	int indis,indis1;//indis aktorlerin hash tablosuna giden indisi,indis1 ise filmleri hash tablosuna giden indisi
	int m,n;//m aktorler hash art�rma indisi,n ise filmler hash art�rma indisi 
	FILE *fp;//dosya 
	movie *movies;//filmler dizi
	actor *aktorler;// aktorler dizi
	fp=fopen("input-mpaa.txt","r");// dosya okuma
	satirsayisi=dosyaSatirSayisiBulma();//dosyada ka�tane film oldu�unu bulma
	aktorler=(actor*)malloc(250000*sizeof(actor));//aktor dizi olu�turma
	movies=(movie*)malloc(30000*sizeof(movie));//film dizisi olu�turma
	for(i=0;i<satirsayisi;i++){//ka� kere d�necegimiz �nceden hesaplad�g�m ivin d�nme
		ch=getc(fp);
		f=0;// ilk olarak f = 0 ise film atacak	
		while(ch!='\n'){//film sonuna gelene kadar
			if(f==0){//film dir
				j=0;// harf sayisi
				while(ch!='/'){// ilk / i�aretine kadar filmi ad�n� harf harf al�yor
		        fname[j]=ch;		
				j++;
				ch=getc(fp);	
				}
				fname[j]='\0';// bitti�ini g�stermek i�in bo� i�areti
				indis1=hash(fname,30000);//hash yerle�tirmek i�in indis al�yor
				n=0;//hash art�rma indisi ilk ba�ta 0
				while( n < 30000 && movies[ (indis1+n) % 30000 ].mname[1]!='\0'){// hash bo� olana kadar indisi art�r�yor
					n++;
			    }
			    indis1 = (indis1+n) % 30000;// eger dizinin boyutundan buyukse tekrar mod al�p diziye at�yor
			    strcpy(movies[ indis1 ].mname,fname);
			    movies[ indis1 ].visited=0;
				f=1;// f yi 1 yap�p aktorlere ge�iyor
				c=0;//aktor say�s�n� da 0 dan ba�lat�yor
			}
			else{//burada ise aktorleri diziye at�yor
				ch=getc(fp);// karakteri al�yor
				j=0;	    
				while(ch!='/' && ch!='\n' && ch!=EOF){//slash veya sonuna gelene kadar devam edip isim al�yor
				name[j]=ch;
				j++;
				ch=getc(fp);	
				}
				name[j]='\0';// bitti�ini g�stermek i�in bo� i�areti
				if(c==0){//eger ilk aktorunu atacaksa malloc ile a��yor 
					movies[indis1].actors=(actor**)malloc(sizeof(actor*));
				}
			    else{//diger elemanlar� icin ise dizi realloc ile art�r�yor
			    	movies[indis1].actors=(actor**)realloc(movies[indis1].actors, sizeof(actor*) * (c + 1));
				}
				indis=hash(name,250000);// dizi i�in kullan�lan indise bak�yor
				m=0;// hash bo� olana kadar veya ayn� isimli aktorgelene kadar indisi art�r�yor
				while( m < 250000 && aktorler[(indis+m) % 250000].name[1]!='\0' && strcmp(aktorler[(indis+m) % 250000].name,name)!=0){
				 	m++;
				}
				indis = (indis + m) % 250000;//gelen indis sizedan b�y�kse tekrar mod al�yor
				if(aktorler[indis].name[1]=='\0'){//eger ayn� isimli aktor yok ise yeni aktor at�yor ve bilgilerini yazd�r�yor
					strcpy(aktorler[indis].name,name);
				    aktorler[indis].films=(movie**)malloc(sizeof(movie*)*1);
				    aktorler[indis].visited=0;
				    aktorler[indis].films[0] = &movies[indis1];
				    aktorler[indis].film_count++;
				}
				else{//yok ise film count ve diziyi art�r�p filmi at�yor  
				 	aktorler[indis].films=(movie**)realloc( aktorler[indis].films,sizeof(movie*) * (aktorler[indis].film_count+1) );
					aktorler[indis].films[ aktorler[indis].film_count ] = &movies[indis1];
					aktorler[indis].film_count++;
			    }
			    movies[indis1].actors[c] = &aktorler[ indis ];//dizinin aktorlerine de bilgileri at�yor
				c++;	
				movies[ indis1 ].actor_count++;//
			}	
        }
    }
    int secim;//menu icin gerekli secim
	printf("1-) Kevin Bacon sayisini bulma\n");
	printf("2-) Iki oyuncunu baglantisini bulma\n");
	scanf("%d",&secim);
	fflush(stdin);
	 switch (secim)
   {
     case 1: BFS(aktorler,movies,secim);
        break;
     case 2: BFS(aktorler,movies,secim);
        break;
     default: printf("Yanlis Secim. \n");
        break;
   } 
return 0;
}

void filmekle(struct movie *film){//her film eklendi�inde realloc ile boyut art�r�l�yor be son elemana yaz�l�yor
    if(filmbaslangic==-1){
	filmbaslangic++;
	filmkuyruk[filmbaslangic]=film;
    }  
	else{
	filmbitis++;
	filmkuyruk = (struct movie**)realloc(filmkuyruk,sizeof(movie*)*(filmbitis+1));
	filmkuyruk[filmbitis ]=film;
     }
}

void filmsil(){//her film silindiginde cekilecek filmin indisi art�r�l�yor
      filmbaslangic++;  
}

void aktorekle(struct actor *aktor){//her aktor eklendi�inde realloc ile boyut art�r�l�yor be son elemana yaz�l�yor
	if(aktorbaslangic==-1){
		aktorbaslangic++;
		aktorkuyruk[aktorbaslangic]=aktor;
	}
	else{
	aktorbitis++;
	aktorkuyruk = (struct actor**)realloc(aktorkuyruk,sizeof(actor*)*(aktorbitis+1));
	aktorkuyruk[aktorbitis]=aktor;
    }
}

void aktorsil(){//her aktor silindiginde cekilecek aktor indisi art�r�l�yor
      aktorbaslangic++;     
}

void yolgoster(actor* aktor2, int yol){// bulunan yol kadar son aktorden once gelen filmlerin ve aktorlerin yazd�r�yor 
	int i;
	for(i=0;i<yol;i++){
		printf("%s - %s : \"%s\"  \n", aktor2->name, aktor2->prefilm->preactor->name, aktor2->prefilm->mname);
		aktor2=aktor2->prefilm->preactor;
	}
	fflush(stdout);
}

int dosyaSatirSayisiBulma(){//al�nan dosyan�n sat�r say�s� hesaplan�yor
	FILE *fileptr;
	char chr;		
    int count_lines = 0;
    fileptr = fopen("input-mpaa.txt", "r");
    chr = getc(fileptr);
    while (!feof(fileptr))
    {
        if (chr == '\n')
        {
            count_lines = count_lines + 1;
        }
        chr = getc(fileptr);
    }
    fclose(fileptr); 
   return count_lines;	
}

  int hash(char *icerik,int mod){// hash fonksiyonu 2 tane size oldugu i�in mod degiskeni tutulup indis hesaplan�yor
	int r=31;//kelime harf tutarken genelde 31 sayisi yap�l�r
	int i;// dongu degiskeni
	int indis;// donen deger
	unsigned long int key;
	key=0;//kelimenin degeri
	int m=strlen(icerik);
	for(i=0;icerik[i]!='\0';i++){
		key=key+(icerik[i] * pow(r,strlen(icerik)-i-1));
	}
	indis=(key%mod);
	return indis;
}

void BFS ( actor aktorler[250000], movie movies[30000],int secim){//BFS de parametre olarak aktorler dizisi ve film dizisi al�nd�
	aktorkuyruk = (actor**) malloc(sizeof(actor*));//aktor kuyrugu icin yer a��ld�
	filmkuyruk = (movie**) malloc(sizeof(movie*));// film kuyrugu icin yer ac�ld�
    char isim[40];//kullan�c�dan al�nan isimler
	char isim1[40];
	int i,m,indis;//i dongu degiskeni,m hash indis art�rma degiskeni,indis hash indis degiskeni 
	int yol;// yol 6 ad�ml� olacag� icin 
	actor* aktor1 = (actor*) malloc(sizeof(actor));//ayr� bir yerde tutmak icin kullan�lan degiskenler
	actor* aktor2 = (actor*) malloc(sizeof(actor));
	movie* movie1 = (movie*) malloc(sizeof(movie));
	yol=0;
  
    if(secim==1){//Menu sonucunda gelen secimi gore programin nas�l �al��acag� i�in gerekli kosul
    	strcpy(isim,"Bacon, Kevin");
    	printf("Oyuncuyu girin(Soyisim, Isim formatinda)\n");
    	fflush (stdin);//Input Buffer temizleme
    	gets(isim1);
	   
	}
	
	else{
	printf("Ilk oyuncuyu girin(SoyIsim, Isim formatinda)\n");// kullanicidan alma
	fflush (stdin);
	gets(isim);
	printf("Ikinci oyuncuyu girin(SoyIsim, Isim formatinda)\n");
	fflush (stdin);
	gets(isim1);
    }
	m=0;
    indis=hash(isim,250000);
    while(m<250000 && strcmp(aktorler[(indis+m) % 250000].name,isim)!=0)//alinan isim dizide hangi indisde oldugunu bulmak icin
	  	m++;
	 if(m>=250000){//eger yoksa
    	printf("Boyle bir aktor yoktur...\n");
	}
	else{
    indis=(indis+m) % 250000;
	aktor1 = &aktorler[indis];
	m = 0;
	while(m<250000 && strcmp(aktorler[(indis+m) % 250000].name,isim1)!=0)//alinan 2.isim dizide hangi indisde oldugunu bulmak icin
	  	m++;
    if(m>=250000){//eger yoksa
    	printf("Boyle bir aktor yoktur...\n");
	}
	else{
	indis=(indis+m) % 250000;
    aktor2 = &aktorler[indis];		
	aktorekle(aktor1);// ilk ald�g�m�z aktoru kuyruga atma
	while( aktor1 != aktor2 && aktorbaslangic<=aktorbitis ){//eger bfs i�lem� tamamlanm�ssa veya yol 6 dan buyuk olmussa
	     while( aktor1 != aktor2 && aktorbaslangic<=aktorbitis ){//aktor kuyrugu bos olana kadar yan� bitis ve baslagic ayni olana kadar
	     	aktor1 = aktorkuyruk[ aktorbaslangic ];//kuyruktaki aktoru degiskene atama
	     	aktorsil();//daha sonra degeri cekme
	     	if( aktor1 != aktor2 ){//eger cekilen aktor hedef aktore e�it de�ilse �ekilen aktorun film sayisi kadar filmini film kuyruguna atama
		     		for(i=0; i < aktor1->film_count; i++ ){
		     		if(aktor1->films[i]->visited == 0){
		     			aktor1->films[i]->visited=1;
		     	    	aktor1->films[i]->preactor = aktor1;
		     			filmekle(aktor1->films[i]);
	     	 		}
			 	}
			 }
		 }
		 if( aktor1 != aktor2){//eger cekilen aktor hedef aktore e�it de�ilse
		 while(filmbaslangic <= filmbitis){//film kuyrugu bos olana kadar yani baslangic bitise e�it olana kadar 
		 	movie1 = filmkuyruk[filmbaslangic];//filmleri �ekip aktorlerini aktor kuyruguna atma
		 	filmsil();
		 		for(i=0;i < movie1->actor_count;i++ ){
		 			if(movie1->actors[i]->visited==0){
		 				movie1->actors[i]->visited=1;
		 				movie1->actors[i]->prefilm = movie1;
		 				aktorekle(movie1->actors[i]);
					 }
				 }	   
		 }
		 yol++;// yol art�rma
		 }	
	}
	if(secim==1 && yol>6){//1 i�lem i�in kontrol
		printf("\nKevin Bacon sayisi 6'dan buyukdur...'");
	}
	else if(aktorbaslangic>aktorbitis){//E�er aktor kuyrugu bo� ise baglanti yoktur...
		printf("Baglanti yoktur");
    }
    else{
    	printf("YOL: %d \n",yol);
	    yolgoster(aktor2, yol);
	}
}
}
}

