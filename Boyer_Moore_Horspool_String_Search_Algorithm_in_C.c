#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

	struct timespec begin;
	struct timespec end;
	
	 	// MA�N
		 	 	
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
		cikanSize=strlen(cikanKelime);					//kelimelerin boyutunun hesaplanmas�
		fark=yeniSize-cikanSize;
	
	
		
		FILE * ptr;
		
			
		dosyadanAl(dosya,yazi,ptr);   					   	// yaziyi ald�m
		
		yaziSize=strlen(yazi);  						    //yaz�n�n boyu hesapland�
		
	 	clock_gettime(CLOCK_MONOTONIC,&begin); 				// s�renin ba�lang�c�
		
		degisim=KelimeArama(yazi,cikanKelime,yeniKelime,yaziSize,cikanSize,caseSens);			//search ve replace i�emi bu fonksiyonda		
				
		clock_gettime(CLOCK_MONOTONIC,&end); 				// s�renin biti�i
		
		yaziSize=yaziSize+(fark*degisim);					//yazdirma isleminde dizinin sonuna kadar gitmek icin gereken islem
			
		printf("\n");
	
		long int gecenSure=(long int) (end.tv_sec-begin.tv_sec)*1000000000 + (end.tv_nsec-begin.tv_nsec); // sure hesaplamas�
		
		yazdir(yaziSize,yazi,degisim,gecenSure);
			
				
		ptr=fopen(dosya,"w");	 // DOSYAYA YAZDIRMA
		
		for(i=0;i<yaziSize;i++){
			
			fprintf(ptr,"%c",yazi[i]);
			
		}
		
		fprintf(ptr,"\nSure: %ld nanosaniye",(long int) (end.tv_sec-begin.tv_sec)*1000000000 + (end.tv_nsec-begin.tv_nsec));
		fprintf(ptr,"\nYapilan degisim sayisi: %d",degisim);
		
		fclose(ptr);
	}
		
	
	//            FONKS�YONLAR 
	
	//////////////////////////////////////////////////////////////////////

	void dosyadanAl(char dosya[] ,char yazi[],FILE * ptr){
	
		int i=0;
		ptr=fopen(dosya,"r"); 	  	// dosyay� read modunda a��yorum
	
		if(ptr==NULL){
			printf("Dosya acilamiyor...");				// e�er dosya bo�sa HATA veriyorum
			return;
		}
	
		while(!feof(ptr)){
			fscanf(ptr,"%c", &yazi[i]);				//dosyan�n i�indeki yaz�lar� yazi[] dizisinin i�ine kopyal�yorum.
			i++;
		}
			
	}

	
	////////////////////////////////////////////////////////////////////////////////////
	
	int kaymaMiktari(char a,char cikanKelime[],int cikanSize){ 	// Boyer-Moore Horspool algoritmas�nda ge�en shift Table � olu�turuyorum
		
		int i,j,k=0,sonOtele=1,harfYok=1;
		
	
		if(a==cikanKelime[cikanSize-1]){		//E�ER SON HARF DENK GELD�YSE
				
				for(j=cikanSize-2;j>=0;j--){		
					
					if(a==cikanKelime[j]){			//Tam bir string boyu �telemeden �nce acaba ba�ka son harften var m� kontrol ediyorum.
						return sonOtele;
					}
					sonOtele++;					// Son harften bir tane daha var m� diye ararken bulamad���m zaman �teleme miktar�n� artt�r�yorum
				}								// ki buldu�um zaman o kadar �teleyeyim.

				return sonOtele;					//oteleme miktar�n� d�nd�r�yorum.
			
			}
			
		
	
		for(i=cikanSize-2;i>=0;i--){
			
			if(a==cikanKelime[i]){
				
				return harfYok;				//E�er harf stringde varsa sondan ka��nc� oldu�una g�re �teliyorum.(rightmosta g�re)
			}								// Burada harf bulunuyor fakat harfYok 'un d�nd�r�lmesinin sebebi ekstra de�i�ken tan�mlamamak.
			
			
			
			harfYok++;
			
			if(harfYok==cikanSize)
				return harfYok;				//E�er harf string de yoksa tam bir string �teliyorum
			
		}
		
		
	}
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	int KelimeArama(char yazi[],char cikanKelime[],char yeniKelime[],int size,int cikanSize,int caseSens){
	
		int i,j,degisim=0,k=cikanSize,kontrol=0,otele,oteleKontrol=0,bos;
		char harf;
	
		
		if(cikanSize==1){			// E�er ��kar�lan kelime tek harfliyse
			
			if(caseSens==1){  //caseSensivity aktif
				
				for(i=0;i<size;i++){						// tek tek t�m harflerle kar��la�t�r�yorum e�er harfler ayn�ysa
					
					if(yazi[i]==cikanKelime[0]){					// c�kar�lan kelime yerine yenisini yerle�tiriyorum
						size=replace(yazi,yeniKelime,i,cikanSize);
						degisim++;									// kelime de�i�ti�i zaman de�i�imi artt�r�yorum
					}
					
					
				}
				return degisim;
				
			}
			
			else {			// E�er ��kan kelime tek harfli ve caseSensivity aktif de�ilse
				
				for(i=0;i<size;i++){
					
					if(yazi[i]==cikanKelime[0] || yazi[i]+32 == cikanKelime[0] || yazi[i]==cikanKelime[0]+32){ //t�m olas�l�klar� kar��la�t�r�yorum
						size=replace(yazi,yeniKelime,i,cikanSize); 		// c�kan kelime yerine yenisnini yerle�tiriyorum
						degisim++;	
					}
					
					
				}
				return degisim;
			}
			
			
			
			
		}
		
		for(i=cikanSize-1;i<size;i++){ // i de�i�eni dosyadan alm�� oldu�um ana yaz� �st�nde geziniyor. cikanSize-1 den ba�l�yor ��nk� algoritma
									   // gere�i �nce son harf kontrol ediliyor.
			k=cikanSize;
			kontrol=0;
			oteleKontrol=0;
			
			if(caseSens==1){    // e�er case sensivity aktif ise bu i�lem yap�l�yor
			
				for(j=i;j>i-cikanSize;j--){
				
				if(yazi[j]==cikanKelime[k-1]){		// ana yazidaki harf ile ��karmak istedi�imiz harfin e�itli�i kontrol ediliyor
					kontrol++;						// e�er e�itse kontrol de�i�keni 1 artt�r�l�yor daha sonra kontrol ��kacak olan	
					if(kontrol==cikanSize){			// kelimenin boyuyla kar��la�t�r�l�yor e�itse i�lem tamamlanm�� oluyor ve de�i�im ger�ekle�iyor.
						
						size=replace(yazi,yeniKelime,j,cikanSize); //i�lem sonunda size de�i�ebilece�inden size d�nd�r�l�yor.
						i=i+cikanSize-1;
						degisim++;  		// degisim gerceklestikce arttr�l�yor finalde yazd�r�l�yor
						break;
					}
				}
				else {
					harf=yazi[j];
					otele = kaymaMiktari(harf,cikanKelime,cikanSize); // kayma miktari hesaplan�p o kadar oteleniyor
					i=i+otele-1;
					break;
				}
				
				k--;  // k bir azalt�larak cikan kelimenin 1 �nceki harfine ge�iliyor.
			}
			}
			
			else{  		// E�ER CASE SENS�V�TY AKT�F DE��LSE BURAYA GE��L�YOR
			
				for(j=i;j>i-cikanSize;j--){
				
				if(yazi[j]==cikanKelime[k-1] || yazi[j]+32 == cikanKelime[k-1] || yazi[j]==cikanKelime[k-1]+32){ //case sensivity aktif olmad���ndan
					kontrol++;                                       // burada kelimelerin b�y�k k���k her t�rl� e�itli�i kontrol ediliyor.
					oteleKontrol++;									//oteleKontrol degiskeni ile eger �nceden bir esitlik tespit edilmi�se ve daha
					if(kontrol==cikanSize){ 						// sonras�nda bir e�itsizlik elde edilmi�se dei�i�imi ilk e�le�en harfe g�re 
																	// yapmak i�in bir kontrol ger�ekle�tiriliyor
																	
						size=replace(yazi,yeniKelime,j,cikanSize);  // eleman degisince boyut da degisebileceginden size d�nd�r�l�yor.
						i=i+cikanSize-1;
						degisim++;
						break;
					}
				}
				else {
					if(oteleKontrol!=0){
						harf=cikanKelime[cikanSize-1];			// harf e�le�memesi ger�kelmi�se ve daha �nceden harf e�le�mesi varsa
																// kayd�rma miktar�m�z� ilk harfe g�re belirliyoruz.
					}
					else{
						harf=yazi[j];
					}
					
					otele = kaymaMiktari(harf,cikanKelime,cikanSize);	// oteleme miktar� hesaplan�p �teleniyor
					i=i+otele-1;
					break;
				}
				
				k--;  // k bir azalt�larak cikan kelimenin 1 �nceki harfine ge�iliyor.
			}
			}
			
			
			
			
		}
	
	
	return degisim; // degisim en son yazd�r�laca��ndan d�nd�r�l�yor
	
	}	


	////////////////////////////////////////////////////////////////////////////////////////////////

	int replace (char yazi[],char yeniKelime[],int j,int cikanSize){		// YER DE���T�RME
		
		int i,k,yeniSize,yaziSize,sayac=0,fark,t;				

		yeniSize=strlen(yeniKelime);			//yeni kelimenin ve t�m dizinin boyutu bulunuyor.
		yaziSize=strlen(yazi);
		
		
			if(yeniSize < cikanSize){ 			// E�er yeni giren kelime ��kan kelimeden daha k�saysa
			fark=cikanSize-yeniSize;
			
			for(i=j;i<j+yeniSize;i++){
												// yeni kelime ��kan kelimeninin yerine yerle�tirliyor (zaten fonksiyona g�nderilen j indexi
				yazi[i]=yeniKelime[sayac];		// kelimenin ilk harfini g�sterdi�i i�in direk j de�erinden ba�layarak �st�ne yaz�yoruz.
				sayac++;	
			}
			t=i+1;
			
			for(i=j+yeniSize;i<j+yeniSize+fark;i++){	//Kalan b�l�mlere bo�luk at�l�yor
				yazi[i]=' ';
			}
			
			for(i=t;i<yaziSize;i++){		// Olu�an stringde bo�luklar olu�aca�� i�in dizinin elemanlar� bo�luklar� kapayacak
											//  �ekilde geri �ekiliyor.
				yazi[i]=yazi[i+fark];
			}
			yaziSize=yaziSize-fark;
			return yaziSize;
		}
		
		
		
		else if(yeniSize > cikanSize){  		// E�er yeni giren kelime eski kelimeden daha uzunsa
			fark=yeniSize-cikanSize;
			
			
			for(k=yaziSize-1;k>=j+cikanSize;k--){	// �nce aralar�ndaki fark kadar (kelimenin bti�inden itibaren) sa�a kayd�rma i�lemi yap�l�yor.
				yazi[k+fark]=yazi[k];				
			}
			
			for(i=j;i<j+yeniSize;i++){				// Daha sonra kelime direk yerle�tiriliyor
				
				yazi[i]=yeniKelime[sayac];
				sayac++;
			}
			return yaziSize;
		}
		
	
		
		else{				//E�er iki kelimenin boyutlar� e�itse direkyerine yerle�tirliyor boyut ayarlamas� yap�lm�yor.
			for(i=j;i<j+cikanSize;i++){
				
				yazi[i]=yeniKelime[sayac];		// Yeni kelimeyi eski kelimenin �st�ne yaz�yor
				sayac++;
			}
			return yaziSize;
		}
	
	}
	
	
	void yazdir(int yaziSize,char yazi[],int degisim,long int gecenSure){
		int i;
		
		for(i=0;i<yaziSize;i++){
			
			printf("%c",yazi[i]);			// yaz�n�n son halinin ekrana bast�r�lmas�
			
		}
		
		printf("\n\n*************************\n\n");
		printf("Yapilan degisim sayisi: %d\n",degisim); // De�i�tirilen kelime say�s�
		printf("Sure: %ld nanosaniye",gecenSure); // fonksiyon zaman�
	}
