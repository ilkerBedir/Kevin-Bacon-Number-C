#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
typedef struct actor{//aktor structi
	char name [100];//aktorun ismi
    struct movie **films;//aktorun filmleri
    int film_count;//aktorun oynadigi film sayilari
    short visited;//bfs de kuyruða attýktan sonra tek atýlmasýný önlemek için visited  degiskeni
	struct movie *prefilm;//kuyruk yolunu yazdýrýrken hangi filmden geldiðini anlamka için kullanýldý
}actor;

typedef struct movie{//film structý
	char mname[100];//filmlerin adý
    struct actor **actors ;//filmde oynayan aktorlerin dizisi
    short visited;//bfs de kuyruða attýktan sonra tek atýlmasýný önlemek için visited  degiskeni
    struct actor *preactor;//kuyruk yolunu yazdýrýrken hangi aktorden geldiðini anlamak için kullanýldý
    int actor_count;//filmde oynayan aktorler
}movie;

struct actor **aktorkuyruk;//bfs iþlemleri 2 kuyruk tutarak yaptým,Kuyruklardan ilki aktor kuyruðu
struct movie **filmkuyruk;//ikinci film kuyrugu
int filmbaslangic=-1;//film kuyruðundaki baþlangýç indisi
int filmbitis=0;//film kuyruðunda bitiþ indisi
int aktorbaslangic=-1;//aktor kuyruðundaki baþlangýç indisi
int aktorbitis=0;//aktor kuyruðunda bitiþ indisi

void filmekle (struct movie * film);//film kuyruðuna film ekleme fonksiyonu
void filmsil();//film kuyruðunda film silme fonksiyonu
void aktorekle(struct actor*aktor);//aktor kuyruðuna aktor eklem fonksiyonu
void aktorsil();//aktor kuyruðundan aktor silme fonksiyonu
void yolgoster(actor* aktor2, int yol);//bulunan yolu gosterme
int dosyaSatirSayisiBulma();//dosyada kaç tane film olduðunu bulma fonksiyonu
int hash(char *icerik,int mod);//aktorler ve filmler dizileri için hash fonksiyonu
void BFS ( actor aktorler[250000], movie movies[30000],int secim);//Breadth First Search in yapildigi yer ayrýca kullanýcýdan ismlerin alýndýðý yer

int main(int argc, char *argv[]) {//main fonksiyonumda dosyadab gelen bilgileri okuyarak bipartite graf oluþturuyorum
	int satirsayisi,i;//satirsayisi dosyadan gelen sayi tutmak için, i ise indis için
	char ch,name[100],fname[100];//fname dosyadan gelen filmin ismini tutmak için, name ise aktor ismini tutmak için,ch ise karakter tutmak için
	int f,j,c;//f dosyadan gelen deðerin filmin mi aktor mu oldugunu belirten flag,j döngü deðiþkeni,c film count deðiþkeni
	int indis,indis1;//indis aktorlerin hash tablosuna giden indisi,indis1 ise filmleri hash tablosuna giden indisi
	int m,n;//m aktorler hash artýrma indisi,n ise filmler hash artýrma indisi 
	FILE *fp;//dosya 
	movie *movies;//filmler dizi
	actor *aktorler;// aktorler dizi
	fp=fopen("input-mpaa.txt","r");// dosya okuma
	satirsayisi=dosyaSatirSayisiBulma();//dosyada kaçtane film olduðunu bulma
	aktorler=(actor*)malloc(250000*sizeof(actor));//aktor dizi oluþturma
	movies=(movie*)malloc(30000*sizeof(movie));//film dizisi oluþturma
	for(i=0;i<satirsayisi;i++){//kaç kere dönecegimiz önceden hesapladýgým ivin dönme
		ch=getc(fp);
		f=0;// ilk olarak f = 0 ise film atacak	
		while(ch!='\n'){//film sonuna gelene kadar
			if(f==0){//film dir
				j=0;// harf sayisi
				while(ch!='/'){// ilk / iþaretine kadar filmi adýný harf harf alýyor
		        fname[j]=ch;		
				j++;
				ch=getc(fp);	
				}
				fname[j]='\0';// bittiðini göstermek için boþ iþareti
				indis1=hash(fname,30000);//hash yerleþtirmek için indis alýyor
				n=0;//hash artýrma indisi ilk baþta 0
				while( n < 30000 && movies[ (indis1+n) % 30000 ].mname[1]!='\0'){// hash boþ olana kadar indisi artýrýyor
					n++;
			    }
			    indis1 = (indis1+n) % 30000;// eger dizinin boyutundan buyukse tekrar mod alýp diziye atýyor
			    strcpy(movies[ indis1 ].mname,fname);
			    movies[ indis1 ].visited=0;
				f=1;// f yi 1 yapýp aktorlere geçiyor
				c=0;//aktor sayýsýný da 0 dan baþlatýyor
			}
			else{//burada ise aktorleri diziye atýyor
				ch=getc(fp);// karakteri alýyor
				j=0;	    
				while(ch!='/' && ch!='\n' && ch!=EOF){//slash veya sonuna gelene kadar devam edip isim alýyor
				name[j]=ch;
				j++;
				ch=getc(fp);	
				}
				name[j]='\0';// bittiðini göstermek için boþ iþareti
				if(c==0){//eger ilk aktorunu atacaksa malloc ile açýyor 
					movies[indis1].actors=(actor**)malloc(sizeof(actor*));
				}
			    else{//diger elemanlarý icin ise dizi realloc ile artýrýyor
			    	movies[indis1].actors=(actor**)realloc(movies[indis1].actors, sizeof(actor*) * (c + 1));
				}
				indis=hash(name,250000);// dizi için kullanýlan indise bakýyor
				m=0;// hash boþ olana kadar veya ayný isimli aktorgelene kadar indisi artýrýyor
				while( m < 250000 && aktorler[(indis+m) % 250000].name[1]!='\0' && strcmp(aktorler[(indis+m) % 250000].name,name)!=0){
				 	m++;
				}
				indis = (indis + m) % 250000;//gelen indis sizedan büyükse tekrar mod alýyor
				if(aktorler[indis].name[1]=='\0'){//eger ayný isimli aktor yok ise yeni aktor atýyor ve bilgilerini yazdýrýyor
					strcpy(aktorler[indis].name,name);
				    aktorler[indis].films=(movie**)malloc(sizeof(movie*)*1);
				    aktorler[indis].visited=0;
				    aktorler[indis].films[0] = &movies[indis1];
				    aktorler[indis].film_count++;
				}
				else{//yok ise film count ve diziyi artýrýp filmi atýyor  
				 	aktorler[indis].films=(movie**)realloc( aktorler[indis].films,sizeof(movie*) * (aktorler[indis].film_count+1) );
					aktorler[indis].films[ aktorler[indis].film_count ] = &movies[indis1];
					aktorler[indis].film_count++;
			    }
			    movies[indis1].actors[c] = &aktorler[ indis ];//dizinin aktorlerine de bilgileri atýyor
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

void filmekle(struct movie *film){//her film eklendiðinde realloc ile boyut artýrýlýyor be son elemana yazýlýyor
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

void filmsil(){//her film silindiginde cekilecek filmin indisi artýrýlýyor
      filmbaslangic++;  
}

void aktorekle(struct actor *aktor){//her aktor eklendiðinde realloc ile boyut artýrýlýyor be son elemana yazýlýyor
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

void aktorsil(){//her aktor silindiginde cekilecek aktor indisi artýrýlýyor
      aktorbaslangic++;     
}

void yolgoster(actor* aktor2, int yol){// bulunan yol kadar son aktorden once gelen filmlerin ve aktorlerin yazdýrýyor 
	int i;
	for(i=0;i<yol;i++){
		printf("%s - %s : \"%s\"  \n", aktor2->name, aktor2->prefilm->preactor->name, aktor2->prefilm->mname);
		aktor2=aktor2->prefilm->preactor;
	}
	fflush(stdout);
}

int dosyaSatirSayisiBulma(){//alýnan dosyanýn satýr sayýsý hesaplanýyor
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

  int hash(char *icerik,int mod){// hash fonksiyonu 2 tane size oldugu için mod degiskeni tutulup indis hesaplanýyor
	int r=31;//kelime harf tutarken genelde 31 sayisi yapýlýr
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

void BFS ( actor aktorler[250000], movie movies[30000],int secim){//BFS de parametre olarak aktorler dizisi ve film dizisi alýndý
	aktorkuyruk = (actor**) malloc(sizeof(actor*));//aktor kuyrugu icin yer açýldý
	filmkuyruk = (movie**) malloc(sizeof(movie*));// film kuyrugu icin yer acýldý
    char isim[40];//kullanýcýdan alýnan isimler
	char isim1[40];
	int i,m,indis;//i dongu degiskeni,m hash indis artýrma degiskeni,indis hash indis degiskeni 
	int yol;// yol 6 adýmlý olacagý icin 
	actor* aktor1 = (actor*) malloc(sizeof(actor));//ayrý bir yerde tutmak icin kullanýlan degiskenler
	actor* aktor2 = (actor*) malloc(sizeof(actor));
	movie* movie1 = (movie*) malloc(sizeof(movie));
	yol=0;
  
    if(secim==1){//Menu sonucunda gelen secimi gore programin nasýl çalýþacagý için gerekli kosul
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
	aktorekle(aktor1);// ilk aldýgýmýz aktoru kuyruga atma
	while( aktor1 != aktor2 && aktorbaslangic<=aktorbitis ){//eger bfs iþlemþ tamamlanmýssa veya yol 6 dan buyuk olmussa
	     while( aktor1 != aktor2 && aktorbaslangic<=aktorbitis ){//aktor kuyrugu bos olana kadar yanþ bitis ve baslagic ayni olana kadar
	     	aktor1 = aktorkuyruk[ aktorbaslangic ];//kuyruktaki aktoru degiskene atama
	     	aktorsil();//daha sonra degeri cekme
	     	if( aktor1 != aktor2 ){//eger cekilen aktor hedef aktore eþit deðilse çekilen aktorun film sayisi kadar filmini film kuyruguna atama
		     		for(i=0; i < aktor1->film_count; i++ ){
		     		if(aktor1->films[i]->visited == 0){
		     			aktor1->films[i]->visited=1;
		     	    	aktor1->films[i]->preactor = aktor1;
		     			filmekle(aktor1->films[i]);
	     	 		}
			 	}
			 }
		 }
		 if( aktor1 != aktor2){//eger cekilen aktor hedef aktore eþit deðilse
		 while(filmbaslangic <= filmbitis){//film kuyrugu bos olana kadar yani baslangic bitise eþit olana kadar 
		 	movie1 = filmkuyruk[filmbaslangic];//filmleri çekip aktorlerini aktor kuyruguna atma
		 	filmsil();
		 		for(i=0;i < movie1->actor_count;i++ ){
		 			if(movie1->actors[i]->visited==0){
		 				movie1->actors[i]->visited=1;
		 				movie1->actors[i]->prefilm = movie1;
		 				aktorekle(movie1->actors[i]);
					 }
				 }	   
		 }
		 yol++;// yol artýrma
		 }	
	}
	if(secim==1 && yol>6){//1 iþlem için kontrol
		printf("\nKevin Bacon sayisi 6'dan buyukdur...'");
	}
	else if(aktorbaslangic>aktorbitis){//Eðer aktor kuyrugu boþ ise baglanti yoktur...
		printf("Baglanti yoktur");
    }
    else{
    	printf("YOL: %d \n",yol);
	    yolgoster(aktor2, yol);
	}
}
}
}

