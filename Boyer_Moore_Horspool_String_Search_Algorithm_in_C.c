#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

	struct timespec begin;
	struct timespec end;
	
	 	// MAÝN
		 	 	
		int main(){
		
		clock_t start_t, end_t;    
			
		int i,caseSens,yaziSize,cikanSize,yeniSize,fark,degisim;
		char dosya[20],yeniKelime[100],cikanKelime[100],yazi[300],a[10];
			
			
		printf("Dosya adini giriniz: ");
		scanf("%s",&dosya);
		printf("Cikarmak istediginiz kelime: ");
		gets(a);
		gets(cikanKelime);
		printf("Yerine gececek kelime: ");
		gets(yeniKelime);
		printf("Case sensitive kullanilsin mi?(Kullanilsin -> 1  ,  Kullanilmasin -> 0)");
		scanf("%d",&caseSens);
		
		
		yeniSize=strlen(yeniKelime);
		cikanSize=strlen(cikanKelime);					//kelimelerin boyutunun hesaplanmasý
		fark=yeniSize-cikanSize;
	
	
		
		FILE * ptr;
		
			
		dosyadanAl(dosya,yazi,ptr);   					   	// yaziyi aldým
		
		yaziSize=strlen(yazi);  						    //yazýnýn boyu hesaplandý
		
	 	clock_gettime(CLOCK_MONOTONIC,&begin); 				// sürenin baþlangýcý
		
		degisim=KelimeArama(yazi,cikanKelime,yeniKelime,yaziSize,cikanSize,caseSens);			//search ve replace iþemi bu fonksiyonda		
				
		clock_gettime(CLOCK_MONOTONIC,&end); 				// sürenin bitiþi
		
		yaziSize=yaziSize+(fark*degisim);					//yazdirma isleminde dizinin sonuna kadar gitmek icin gereken islem
			
		printf("\n");
	
		long int gecenSure=(long int) (end.tv_sec-begin.tv_sec)*1000000000 + (end.tv_nsec-begin.tv_nsec); // sure hesaplamasý
		
		yazdir(yaziSize,yazi,degisim,gecenSure);
			
				
		ptr=fopen(dosya,"w");	 // DOSYAYA YAZDIRMA
		
		for(i=0;i<yaziSize;i++){
			
			fprintf(ptr,"%c",yazi[i]);
			
		}
		
		fprintf(ptr,"\nSure: %ld nanosaniye",(long int) (end.tv_sec-begin.tv_sec)*1000000000 + (end.tv_nsec-begin.tv_nsec));
		fprintf(ptr,"\nYapilan degisim sayisi: %d",degisim);
		
		fclose(ptr);
	}
		
	
	//            FONKSÝYONLAR 
	
	//////////////////////////////////////////////////////////////////////

	void dosyadanAl(char dosya[] ,char yazi[],FILE * ptr){
	
		int i=0;
		ptr=fopen(dosya,"r"); 	  	// dosyayý read modunda açýyorum
	
		if(ptr==NULL){
			printf("Dosya acilamiyor...");				// eðer dosya boþsa HATA veriyorum
			return;
		}
	
		while(!feof(ptr)){
			fscanf(ptr,"%c", &yazi[i]);				//dosyanýn içindeki yazýlarý yazi[] dizisinin içine kopyalýyorum.
			i++;
		}
			
	}

	
	////////////////////////////////////////////////////////////////////////////////////
	
	int kaymaMiktari(char a,char cikanKelime[],int cikanSize){ 	// Boyer-Moore Horspool algoritmasýnda geçen shift Table ý oluþturuyorum
		
		int i,j,k=0,sonOtele=1,harfYok=1;
		
	
		if(a==cikanKelime[cikanSize-1]){		//EÐER SON HARF DENK GELDÝYSE
				
				for(j=cikanSize-2;j>=0;j--){		
					
					if(a==cikanKelime[j]){			//Tam bir string boyu ötelemeden önce acaba baþka son harften var mý kontrol ediyorum.
						return sonOtele;
					}
					sonOtele++;					// Son harften bir tane daha var mý diye ararken bulamadýðým zaman öteleme miktarýný arttýrýyorum
				}								// ki bulduðum zaman o kadar öteleyeyim.

				return sonOtele;					//oteleme miktarýný döndürüyorum.
			
			}
			
		
	
		for(i=cikanSize-2;i>=0;i--){
			
			if(a==cikanKelime[i]){
				
				return harfYok;				//Eðer harf stringde varsa sondan kaçýncý olduðuna göre öteliyorum.(rightmosta göre)
			}								// Burada harf bulunuyor fakat harfYok 'un döndürülmesinin sebebi ekstra deðiþken tanýmlamamak.
			
			
			
			harfYok++;
			
			if(harfYok==cikanSize)
				return harfYok;				//Eðer harf string de yoksa tam bir string öteliyorum
			
		}
		
		
	}
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	int KelimeArama(char yazi[],char cikanKelime[],char yeniKelime[],int size,int cikanSize,int caseSens){
	
		int i,j,degisim=0,k=cikanSize,kontrol=0,otele,oteleKontrol=0,bos;
		char harf;
	
		
		if(cikanSize==1){			// Eðer çýkarýlan kelime tek harfliyse
			
			if(caseSens==1){  //caseSensivity aktif
				
				for(i=0;i<size;i++){						// tek tek tüm harflerle karþýlaþtýrýyorum eðer harfler aynýysa
					
					if(yazi[i]==cikanKelime[0]){					// cýkarýlan kelime yerine yenisini yerleþtiriyorum
						size=replace(yazi,yeniKelime,i,cikanSize);
						degisim++;									// kelime deðiþtiði zaman deðiþimi arttýrýyorum
					}
					
					
				}
				return degisim;
				
			}
			
			else {			// Eðer çýkan kelime tek harfli ve caseSensivity aktif deðilse
				
				for(i=0;i<size;i++){
					
					if(yazi[i]==cikanKelime[0] || yazi[i]+32 == cikanKelime[0] || yazi[i]==cikanKelime[0]+32){ //tüm olasýlýklarý karþýlaþtýrýyorum
						size=replace(yazi,yeniKelime,i,cikanSize); 		// cýkan kelime yerine yenisnini yerleþtiriyorum
						degisim++;	
					}
					
					
				}
				return degisim;
			}
			
			
			
			
		}
		
		for(i=cikanSize-1;i<size;i++){ // i deðiþeni dosyadan almýþ olduðum ana yazý üstünde geziniyor. cikanSize-1 den baþlýyor çünkü algoritma
									   // gereði önce son harf kontrol ediliyor.
			k=cikanSize;
			kontrol=0;
			oteleKontrol=0;
			
			if(caseSens==1){    // eðer case sensivity aktif ise bu iþlem yapýlýyor
			
				for(j=i;j>i-cikanSize;j--){
				
				if(yazi[j]==cikanKelime[k-1]){		// ana yazidaki harf ile çýkarmak istediðimiz harfin eþitliði kontrol ediliyor
					kontrol++;						// eðer eþitse kontrol deðiþkeni 1 arttýrýlýyor daha sonra kontrol çýkacak olan	
					if(kontrol==cikanSize){			// kelimenin boyuyla karþýlaþtýrýlýyor eþitse iþlem tamamlanmýþ oluyor ve deðiþim gerçekleþiyor.
						
						size=replace(yazi,yeniKelime,j,cikanSize); //iþlem sonunda size deðiþebileceðinden size döndürülüyor.
						i=i+cikanSize-1;
						degisim++;  		// degisim gerceklestikce arttrýlýyor finalde yazdýrýlýyor
						break;
					}
				}
				else {
					harf=yazi[j];
					otele = kaymaMiktari(harf,cikanKelime,cikanSize); // kayma miktari hesaplanýp o kadar oteleniyor
					i=i+otele-1;
					break;
				}
				
				k--;  // k bir azaltýlarak cikan kelimenin 1 önceki harfine geçiliyor.
			}
			}
			
			else{  		// EÐER CASE SENSÝVÝTY AKTÝF DEÐÝLSE BURAYA GEÇÝLÝYOR
			
				for(j=i;j>i-cikanSize;j--){
				
				if(yazi[j]==cikanKelime[k-1] || yazi[j]+32 == cikanKelime[k-1] || yazi[j]==cikanKelime[k-1]+32){ //case sensivity aktif olmadýðýndan
					kontrol++;                                       // burada kelimelerin büyük küçük her türlü eþitliði kontrol ediliyor.
					oteleKontrol++;									//oteleKontrol degiskeni ile eger önceden bir esitlik tespit edilmiþse ve daha
					if(kontrol==cikanSize){ 						// sonrasýnda bir eþitsizlik elde edilmiþse deiðiþimi ilk eþleþen harfe göre 
																	// yapmak için bir kontrol gerçekleþtiriliyor
																	
						size=replace(yazi,yeniKelime,j,cikanSize);  // eleman degisince boyut da degisebileceginden size döndürülüyor.
						i=i+cikanSize-1;
						degisim++;
						break;
					}
				}
				else {
					if(oteleKontrol!=0){
						harf=cikanKelime[cikanSize-1];			// harf eþleþmemesi gerçkelmiþse ve daha önceden harf eþleþmesi varsa
																// kaydýrma miktarýmýzý ilk harfe göre belirliyoruz.
					}
					else{
						harf=yazi[j];
					}
					
					otele = kaymaMiktari(harf,cikanKelime,cikanSize);	// oteleme miktarý hesaplanýp öteleniyor
					i=i+otele-1;
					break;
				}
				
				k--;  // k bir azaltýlarak cikan kelimenin 1 önceki harfine geçiliyor.
			}
			}
			
			
			
			
		}
	
	
	return degisim; // degisim en son yazdýrýlacaðýndan döndürülüyor
	
	}	


	////////////////////////////////////////////////////////////////////////////////////////////////

	int replace (char yazi[],char yeniKelime[],int j,int cikanSize){		// YER DEÐÝÞTÝRME
		
		int i,k,yeniSize,yaziSize,sayac=0,fark,t;				

		yeniSize=strlen(yeniKelime);			//yeni kelimenin ve tüm dizinin boyutu bulunuyor.
		yaziSize=strlen(yazi);
		
		
			if(yeniSize < cikanSize){ 			// Eðer yeni giren kelime çýkan kelimeden daha kýsaysa
			fark=cikanSize-yeniSize;
			
			for(i=j;i<j+yeniSize;i++){
												// yeni kelime çýkan kelimeninin yerine yerleþtirliyor (zaten fonksiyona gönderilen j indexi
				yazi[i]=yeniKelime[sayac];		// kelimenin ilk harfini gösterdiði için direk j deðerinden baþlayarak üstüne yazýyoruz.
				sayac++;	
			}
			t=i+1;
			
			for(i=j+yeniSize;i<j+yeniSize+fark;i++){	//Kalan bölümlere boþluk atýlýyor
				yazi[i]=' ';
			}
			
			for(i=t;i<yaziSize;i++){		// Oluþan stringde boþluklar oluþacaðý için dizinin elemanlarý boþluklarý kapayacak
											//  þekilde geri çekiliyor.
				yazi[i]=yazi[i+fark];
			}
			yaziSize=yaziSize-fark;
			return yaziSize;
		}
		
		
		
		else if(yeniSize > cikanSize){  		// Eðer yeni giren kelime eski kelimeden daha uzunsa
			fark=yeniSize-cikanSize;
			
			
			for(k=yaziSize-1;k>=j+cikanSize;k--){	// Önce aralarýndaki fark kadar (kelimenin btiþinden itibaren) saða kaydýrma iþlemi yapýlýyor.
				yazi[k+fark]=yazi[k];				
			}
			
			for(i=j;i<j+yeniSize;i++){				// Daha sonra kelime direk yerleþtiriliyor
				
				yazi[i]=yeniKelime[sayac];
				sayac++;
			}
			return yaziSize;
		}
		
	
		
		else{				//Eðer iki kelimenin boyutlarý eþitse direkyerine yerleþtirliyor boyut ayarlamasý yapýlmýyor.
			for(i=j;i<j+cikanSize;i++){
				
				yazi[i]=yeniKelime[sayac];		// Yeni kelimeyi eski kelimenin üstüne yazýyor
				sayac++;
			}
			return yaziSize;
		}
	
	}
	
	
	void yazdir(int yaziSize,char yazi[],int degisim,long int gecenSure){
		int i;
		
		for(i=0;i<yaziSize;i++){
			
			printf("%c",yazi[i]);			// yazýnýn son halinin ekrana bastýrýlmasý
			
		}
		
		printf("\n\n*************************\n\n");
		printf("Yapilan degisim sayisi: %d\n",degisim); // Deðiþtirilen kelime sayýsý
		printf("Sure: %ld nanosaniye",gecenSure); // fonksiyon zamaný
	}
