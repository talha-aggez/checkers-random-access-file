#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;
//!!!!! DAMA TASÝ HAREKETÝ ÝLE ÝLGÝLÝ UFAK UYARI eger yiyceðiniz tasýn arkasýna koymazsanýz uyarý verip sýrayý öbür tarafa geçiriyor....
//!!!!! Normal tas hareketinde zorunlu yeme islemi var yememiz gereken tas varsa onu yemeliyiz....
const char tas_rengi[2] = { 'w','b' };//tas turu...
char harf[8] = { 'A','B','C','D','E','F','G','H' };
char kucuk_harf[8] = { 'a','b','c','d','e','f','g','h' };
const int sayi[8] = { 1,2,3,4,5,6,7,8 };
const char char_sayi[8] = { '1','2','3','4','5','6','7','8' };
const char bosluk = ' ';
const char enter = '\n';
int flag1 = 0, flag2 = 0;
char sira = 'b';
string adim;
FILE* stream;
FILE* stream2;
int bul(char a, char b);//Taþýn bulunduðu yerin harf ,sayi koordinatlarýný girerek bulunur..
int char_to_int(char hareket);//Char sayiyi int' a çevirmek için kullanýlýr..
int char_to_buyuk(char hareket);//harf'in dizideki bulunduðu yeri döndürür..
bool hareket1(char hareket_0, char hareket_1, char hareket_2, char hareket_3);//Hareketin düzgün olup olmadýðýný kontrol eder...
void yenebilen_tas_var_mi();//Yenilebilen taþlarýn yerini belirler
void tablo_oku();//Tabloyu okur bastýrarak...
void tablo_oku_printfsiz();//Tabloyu bastýrmadan okur...
bool tablo_dama_tasi_bul(char hareket_0, char hareket_1);//Tablodaki dama tasini bulur...
int dama_tasi_bul(char hareket_0, char hareket_1, char sira);//Vector üzerindeki dama tasini girilen koordinatlara göre bulur...
int bul1(char hareket_0, char hareket_1, char sira1);//Taþýn bulunduðu yerin harf ,sayi ve reng koordinatlarýný girerek bulunur..
bool yenilebilir_tasi_check();
void tas_hareketi();
void hamleYaz(string hareket);
void sira_bastir();//yeni sirayi sona bastýrýr....
//int yenilebilen_tas_miktarý();
int yenilebilir_tasi_bul(char hareket_2, char hareket_3);
typedef struct tas {
	int sayi;//tasin bulundugu sayi
	char harf;//tasin bulundugu harfi
	char reng;//tasin rengi
}tas;
tas white_tas[16];
tas black_tas[16];
vector<tas> yenilebilen_tas;//yenilebilen taslar
vector<tas> dama;//Dama olan taslar
void tablo_guncelle_dama(tas temp);//Dama Tas harketlerini güncellediðimiz fonksiyon
tas bos;
void tablo_guncelle(tas temp);//Normal taslarýn hareketlerini güncellediðmiz fonksiyon...
void dosya_ac() {
	errno_t err;
	err = fopen_s(&stream, "tahta.dat", "wb");
	if (err != 0)
	{
		printf("The file 'tahta.dat' was not opened\n");
		return;
	}
}
void tahta_olustur() {
	int flag = 0;
	int sayac = 0;
	for (int i = 0; i < 9; i++) {//Toplam dokuz satýr olduðu için i yi 9'a kadar götürüyoruz...
		if (i != 8) {
			//cout << sayi[i];
			fwrite(&char_sayi[i], sizeof(char), 1, stream);//Sayilari bastirirz...
		}
		if (i == 8) {
			flag = 1;
		}
		for (int j = 0; j < 9; j++) {
			if (flag == 1) {//flag 1 olan yerlerde harfleri bastiririz...
				//cout << " ";
				fwrite(&bosluk, sizeof(char), 1, stream);
				if (j != 0) {
					//cout << harf[sayac] << " " ;
					//fwrite(&bosluk, sizeof(char), 1, stream);
					fwrite(&bosluk, sizeof(char), 1, stream);
					fwrite(&harf[sayac], sizeof(char), 1, stream);
					sayac++;
				}
			}
			else if ((flag == 0 || j == 0) && j != 8 && (i == 1 || i == 2)) {// Sayilarýn 1 ve 2 olan kýsýmlarýnda beyaz taslari bastýrrýz...
				//cout << " " << "w" << " ";
				fwrite(&bosluk, sizeof(char), 1, stream);
				fwrite(&bosluk, sizeof(char), 1, stream);
				fwrite(&tas_rengi[0], sizeof(char), 1, stream);
				//fwrite(&bosluk, sizeof(char), 1, stream);
			}
			else if ((flag == 0 || j == 0) && j != 8 && (i == 5 || i == 6)) {//sayilarýn 5 ve 6 olan yerlerinde siyah taslari bastiririz...
				//cout << " " << "b" << " ";
				fwrite(&bosluk, sizeof(char), 1, stream);
				fwrite(&bosluk, sizeof(char), 1, stream);
				fwrite(&tas_rengi[1], sizeof(char), 1, stream);
				//fwrite(&bosluk, sizeof(char), 1, stream);
			}
			else if ((flag == 0 && j != 8)) {
				fwrite(&bosluk, sizeof(char), 1, stream);
				fwrite(&bosluk, sizeof(char), 1, stream);
				fwrite(&bosluk, sizeof(char), 1, stream);
			}
			if (j == 8) {
				fwrite(&bosluk, sizeof(char), 1, stream);
			}
		}
		//cout << endl;
		fwrite(&enter, sizeof(char), 1, stream);//Her satýrýn sonunda \n sembolünü bastýrýrýz...
	}
	fwrite(&tas_rengi[0], sizeof(char), 1, stream);//Son olarak sýranin kimde olduðunu bastýrýrz...
}
void tahta_bastir() {
	int flag = 0;
	int sayac = 0;
	int temp;
	int beyaz_tas = 0, siyah_tas = 0;
	char read;
	char bosluk1;
	errno_t err = fopen_s(&stream, "tahta.dat", "rb");
	for (int i = 0; i < 9; i++) {
		if (i != 8) {
			//cout << sayi[i];
			fread(&read, sizeof(char), 1, stream);//Sayilarý okuruz...
			cout << read;
		}
		if (i == 8) {
			flag = 1;
		}
		for (int j = 0; j < 9; j++) {
			if (flag == 1) {//8. satýrdaki harfleri okuruz...
				fread(&read, sizeof(char), 1, stream);//Bastaki boslugu okuruz...
				cout << read;
				if (j != 0) {
					//cout << harf[sayac] << " ";
					/*fread(&read, sizeof(char), 1, stream);
					cout << read;*/
					fread(&read, sizeof(char), 1, stream);//Harfleri okuruz....
					cout << read;
					fread(&read, sizeof(char), 1, stream);
					cout << read;
					sayac++;
				}
			}
			else if ((flag == 0 || j == 0) && j != 8 && (i == 1 || i == 2)) {
				fread(&read, sizeof(char), 1, stream);
				cout << read;
				fread(&read, sizeof(char), 1, stream);
				cout << read;
				fread(&read, sizeof(char), 1, stream);//okuduðumuz beyaz taslari atama ile atarýz...
				white_tas[beyaz_tas].harf = harf[j];
				white_tas[beyaz_tas].reng = read;
				white_tas[beyaz_tas].sayi = i + 1;
				cout << read;
				++beyaz_tas;//beyaz taslarin sayacý
			}
			else if ((flag == 0 || j == 0) && j != 8 && (i == 5 || i == 6)) {
				fread(&read, sizeof(char), 1, stream);
				cout << read;
				fread(&read, sizeof(char), 1, stream);
				cout << read;
				fread(&read, sizeof(char), 1, stream);//okudugumuz siyah taslari atama ile atarýz..
				black_tas[siyah_tas].harf = harf[j];
				black_tas[siyah_tas].reng = read;
				black_tas[siyah_tas].sayi = i + 1;
				cout << read;
				++siyah_tas;//siyah taslarin sayaci
			}
			else if ((flag == 0 && j != 8)) {//Bos satýrlarý okumak icin
				fread(&read, sizeof(char), 1, stream);
				cout << read;
				fread(&read, sizeof(char), 1, stream);
				cout << read;
				fread(&read, sizeof(char), 1, stream);
				cout << read;
			}
			if (j == 8) {
				fread(&read, sizeof(char), 1, stream);//bos satir basmak icin
				cout << read;
			}
		}
		//cout << endl;
		fread(&read, sizeof(char), 1, stream);//üsteki gibi endl basmak icin...
		cout << read;
	}
	fread(&read, sizeof(char), 1, stream);//sira icerigini okumak icin
	sira = read;
	cout << sira;
}
bool input_kontrol(string hareket) {
	bool validate = false;
	tablo_oku();
	int sayac = 0, flag = 0;
	int a;
	if ((a = hareket.size()) > 4 || hareket.size() < 4)//Girilen komutun boyutunu kontrol ediyoruz...
		return false;
	else if (a == 4) {//Eger girilen komutun dizisi 4 elemanlý ise..
		for (int i = 0; i < 8; i++) {
			if (hareket[0] == harf[i] || hareket[0] == kucuk_harf[i]) {//girilen komutun  ilk elemenaý  harflerin dýsýna çýkmýs mý...
				++sayac;
			}
			if (hareket[1] == char_sayi[i]) {//komutun ikinci elemaný sayilarin dýsýna çýkmýs mý...
				++sayac;
			}
			if (hareket[2] == harf[i] || hareket[2] == kucuk_harf[i]) {//girilen komutun üçüncü elemaný harflerin dýsýna çýkmýþ mý...
				++sayac;
			}
			if (hareket[3] == char_sayi[i]) {//komutun 4 elemaný sayilarýn dýsýnda baska bir sey girilmis mi....
				++sayac;
			}
			//cout << sayac << endl;
		}
		if (sayac == 4)
			validate = true;//Eger dordude dogru girildiyse true...
		else
			validate = false;
		if (validate) {
			if (dama_tasi_bul(hareket[0], hareket[1], 'w') != -1 || dama_tasi_bul(hareket[0], hareket[1], 'b') != -1) {//Girilen bölgede dama tasi var mý kontrol et....
				int flagx = 0, flag15 = 0, flag16 = 0,flag17 = 0,flag18 = 0,flagy = 0,flagx1 = 0;
				//flag15 = 1;
				char sira2;
				if (sira == 'w') {//sira beyazdaysa sira2 siyah yap...
					sira2 = 'b';
				}
				else if(sira == 'b')//sira siyahdaysa sira2 beyaz tas yap...
					sira2 = 'w';
				//yukardan aþaðý...
				int sayac = 0;
				if (hareket[0] == hareket[2]) {// y ekseninde hareket için...
					flagy = 1;
					if (char_to_int(hareket[3]) > char_to_int(hareket[1])) {//Gidilen noktanýn sayisal degeri ilk konumdan büyük ise...
						flag15 = 1;
						for (int i = char_to_int(hareket[1])+1; i < char_to_int(hareket[3]); i++) {
							if (bul1(hareket[0], char_sayi[i - 1], sira) != -1 || dama_tasi_bul(hareket[0], char_sayi[i - 1], sira) != -1) {//gideceði bölgede kendi tasina rastlamýssa flagx = 1;
								//cout << "flagx" << white_tas[bul1(harf[i - 1], hareket[1], sira)].harf << " " << white_tas[bul1(harf[i - 1], hareket[1], sira)].sayi << endl;
								flagx = 1;
							}
							if (bul1(hareket[0], char_sayi[i - 1], sira2) != -1 || dama_tasi_bul(hareket[0], char_sayi[i - 1], sira2) != -1) {//gideceði bölgeye giderken baska tasa rastlamýs mý...
								++sayac;
							}
						}
					}
					else if (char_to_int(hareket[3]) < char_to_int(hareket[1])) {//gidecegi noktanýn sayýsal degeri kendinden küçükse...
						flag16 = 1;
						for (int i = char_to_int(hareket[3]); i < char_to_int(hareket[1]); i++) {
							if (bul1(hareket[0], char_sayi[i - 1], sira) != -1 || dama_tasi_bul(hareket[0], char_sayi[i - 1], sira) != -1) {//gideceði bölgede kendi tasina rastlamissa flagx = 1;
								flagx = 1;
							}
							if (bul1(hareket[0], char_sayi[i - 1], sira2) != -1 || dama_tasi_bul(hareket[0], char_sayi[i - 1], sira2) != -1) {//gideceði bölgeye giderken kendi rengi dýsýnda tasa rastlamýs mý...
								++sayac;
							}
						}
					}
				}
				else if (hareket[1] == hareket[3]) {//saða sola hareket...
					flagx1 = 1;
					if (char_to_buyuk(hareket[0]) > char_to_buyuk(hareket[2])) {//gideceði bölgenin sayýsal degeri kendinden kucuk ise...
						flag17 = 1;
						for (int i = char_to_buyuk(hareket[2])+1; i <= char_to_buyuk(hareket[0]); i++) {
							if (bul1(harf[i - 1], hareket[1], sira) != -1 || dama_tasi_bul(harf[i - 1], hareket[1], sira) != -1) {//gideceði bölgede kendi tasýna rastlamýs mý
								//cout <<  "flagx" <<white_tas[bul1(harf[i - 1], hareket[1], sira)].harf << " " << white_tas[bul1(harf[i - 1], hareket[1], sira)].sayi << endl;
								flagx = 1;
							}
							if (bul1(harf[i - 1], hareket[1], sira2) != -1 || dama_tasi_bul(harf[i - 1], hareket[1], sira2) != -1) {//gidecegi yerde kendinden farklý renkte bir tasa rastlamis mi....
								//cout << "flagx1" << white_tas[bul1(harf[i - 1], hareket[1], sira)].harf << " " << white_tas[bul1(harf[i - 1], hareket[1], sira)].sayi << endl;
								++sayac;
							}
						}
					}
					else if (char_to_buyuk(hareket[0]) < char_to_buyuk(hareket[2])) {//gidecegi bölgenin sayýsal degeri kendinden büyük ise
						flag18 = 1;
						for (int i = char_to_buyuk(hareket[0]) +2; i <= char_to_buyuk(hareket[2]); i++) {
							if (bul1(harf[i - 1], hareket[1], sira) != -1 || dama_tasi_bul(harf[i - 1], hareket[1], sira) != -1) {//gidecegi yerde kendi tasina rastlamis mi
								//cout << "flagx3" << white_tas[bul1(harf[i - 1], hareket[1], sira)].harf << " " << white_tas[bul1(harf[i - 1], hareket[1], sira)].sayi << endl;
								flagx = 1;
							}
							if (bul1(harf[i - 1], hareket[1], sira2) != -1 || dama_tasi_bul(harf[i - 1], hareket[1], sira2) != -1) {//gidecegi yerde kendi tasindan farklý rengte tasa rastlamýs mý...
								//cout << "flagx2" << white_tas[bul1(harf[i - 1], hareket[1], sira)].harf << " " << white_tas[bul1(harf[i - 1], hareket[1], sira)].sayi << endl;
								++sayac;
							}
						}
					}
				}
				//cout << "\nsira2: " << sira2 << endl;
				//cout << "\nsayac" << sayac << "flagx" << flagx << endl;
				//cout << "flag15:" << flag15 << " flag16: " << flag16 << endl;
				if (sayac == 0 && flagx == 0 &&(flagx1 == 1 || flagy == 1)) {
					if (bul1(hareket[2], hareket[3], sira2) == -1 || dama_tasi_bul(harf[char_to_buyuk(hareket[2])], hareket[3], sira2) == -1 || bul1(hareket[2], hareket[3], sira) == -1 || dama_tasi_bul(harf[char_to_buyuk(hareket[2])], hareket[3], sira) == -1) {
						//yukarýdaki if te gideceði bölgede tas var mý yoksa alttaki islemleri yap diyoruz...
						for (int i = 0; i < dama.size(); i++) {
							if (dama[i].harf == harf[char_to_buyuk(hareket[0])] && dama[i].sayi == char_to_int(hareket[1]) && dama[i].reng == sira) {
								dama[i].harf = harf[char_to_buyuk(hareket[2])]; dama[i].sayi = char_to_int(hareket[3]);//dama taslarinin yeni konumlarýný atýyoruz...
							}
						}
						tas temp; temp.harf = hareket[0]; temp.sayi = char_to_int(hareket[1]); temp.reng = ' '; tablo_guncelle(temp); tablo_guncelle_dama(temp);//tabloda koordinatlarý girerek güncelleme yapýyoruz...
						temp.harf = hareket[2]; temp.sayi = char_to_int(hareket[3]); temp.reng = sira; tablo_guncelle(temp); tablo_guncelle_dama(temp);//gideceði bölgenin koordinatlarýna taslarý koyuyoruz...
						for (int i = 0; i < 16; i++) {//Beyaz ve siyah taslarin iceriklerini bosaltýyoruz...
							white_tas[i].harf = ' '; white_tas[i].reng = ' '; white_tas[i].sayi = 0;
							black_tas[i].harf = ' '; black_tas[i].reng = ' '; black_tas[i].sayi = 0;
						}
						tablo_oku();
						sira_bastir();
						return true;
					}
				}
				else if (sayac == 1 && flagx == 0) {//gidecegi yerde bir tane yiyebilceði tasa rastlamýs mý....
					tas temp2, temp3; int flagc = 0;
					if (bul1(hareket[2], hareket[3], sira2) == -1 || dama_tasi_bul(harf[char_to_buyuk(hareket[2])], hareket[3], sira2) == -1 || bul1(hareket[2], hareket[3], sira) == -1 || dama_tasi_bul(harf[char_to_buyuk(hareket[2])], hareket[3], sira) == -1) {
						//gideceði yerde tas olmadýgýný denetliyoruz...
						if (flag15 == 1) {//Asagidaki if else blogunda tas hareketlerini güncelliyoruz....
							if (dama_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2], sira2) != -1) {
								temp2.harf = harf[char_to_buyuk(hareket[0])]; temp2.sayi = char_to_int(hareket[3]) - 1; temp2.reng = ' '; tablo_guncelle(temp2); tablo_guncelle_dama(temp2);
								flagc = 1;
							}
							if (bul1(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2], sira2) != -1) {
								temp2.harf = harf[char_to_buyuk(hareket[0])]; temp2.sayi = char_to_int(hareket[3]) - 1; temp2.reng = ' '; tablo_guncelle(temp2);
								flagc = 1;
							}
						}
						if (flag16 == 1) {
							if (dama_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3])], sira2) != -1) {
								temp2.harf = harf[char_to_buyuk(hareket[0])]; temp2.sayi = char_to_int(hareket[3])+1; temp2.reng = ' '; tablo_guncelle(temp2); tablo_guncelle_dama(temp2);
								flagc = 1;
							}
							if (bul1(hareket[0], char_sayi[char_to_int(hareket[3])], sira2) != -1) {
								temp2.harf = harf[char_to_buyuk(hareket[0])]; temp2.sayi = char_to_int(hareket[3])+1; temp2.reng = ' '; tablo_guncelle(temp2);
								flagc = 1;
							}
						}
						if (flag17 == 1) {
							if (dama_tasi_bul(harf[char_to_buyuk(hareket[2]) + 1], char_sayi[char_to_int(hareket[1])-1], sira2) != -1) {
								temp2.harf = harf[char_to_buyuk(hareket[2]) + 1]; temp2.sayi = char_to_int(hareket[1]); temp2.reng = ' '; tablo_guncelle(temp2); tablo_guncelle_dama(temp2);
								flagc = 1;
							}
							if (bul1(harf[char_to_buyuk(hareket[2]) + 1], char_sayi[char_to_int(hareket[1])-1], sira2) != -1) {
								temp2.harf = harf[char_to_buyuk(hareket[2]) + 1]; temp2.sayi = char_to_int(hareket[1]); temp2.reng = ' '; tablo_guncelle(temp2);
								flagc = 1;
							}
						}
						if (flag18 == 1) {
							if (dama_tasi_bul(harf[char_to_buyuk(hareket[2])], char_sayi[char_to_int(hareket[1])], sira2) != -1) {
								temp2.harf = harf[char_to_buyuk(hareket[2])-1]; temp2.sayi = char_to_int(hareket[1]); temp2.reng = ' '; tablo_guncelle(temp2); tablo_guncelle_dama(temp2);
								flagc = 1;
							}
							if (bul1(harf[char_to_buyuk(hareket[2])-1], char_sayi[char_to_int(hareket[1])-1], sira2) != -1) {
								temp2.harf = harf[char_to_buyuk(hareket[2])-1]; temp2.sayi = char_to_int(hareket[1]); temp2.reng = ' '; tablo_guncelle(temp2);
								flagc = 1;
							}
						}
						//cout << "flagc:" << flagc << endl;
						if (flagc == 1) {
							for (int i = 0; i < dama.size(); i++) {
								if (dama[i].harf == harf[char_to_buyuk(hareket[0])] && dama[i].sayi == char_to_int(hareket[1]) && dama[i].reng == sira) {
									dama[i].harf = harf[char_to_buyuk(hareket[2])]; dama[i].sayi = char_to_int(hareket[3]);//dama tasinin konumlarýný düzeltiyoruz...
								}
							}
							tas temp; temp.harf = hareket[0]; temp.sayi = char_to_int(hareket[1]); temp.reng = ' '; tablo_guncelle(temp); tablo_guncelle_dama(temp);//tabloda taslarýn hareketlerini güncelliyoruz..
							temp.harf = hareket[2]; temp.sayi = char_to_int(hareket[3]); temp.reng = sira; tablo_guncelle(temp); tablo_guncelle_dama(temp);
							for (int i = 0; i < 16; i++) {
								white_tas[i].harf = ' '; white_tas[i].reng = ' '; white_tas[i].sayi = 0;//beyaz ve siyah taslarin iceriklerini bosaltýyoruz....
								black_tas[i].harf = ' '; black_tas[i].reng = ' '; black_tas[i].sayi = 0;
							}
							tablo_oku();
							tas_hareketi();
						}
					}
				}
				else
					return false;
			}
			else if (bul(hareket[0], hareket[1]) != -1) { // gitceði noktada normal bir taþ varsa....
				if (hareket1(hareket[0], hareket[1], hareket[2], hareket[3]) && yenilebilen_tas.size() == 0) {//yaptýgý hareket düzgün ise
					validate = true;
					int x = bul(hareket[0], hareket[1]);//tasin bulundugu yer ile white_tas dizisinin elemanlarýnýn nerede oldugunun indisini döndürür...
					if (sira == 'w') {
						white_tas[x].harf = harf[char_to_buyuk(hareket[2])];//yeni yerlerinin degerleri girilir...
						white_tas[x].sayi = char_to_int(hareket[3]);
						//cout << "white_tas[x]" << white_tas[x].reng << endl;
						tablo_guncelle(white_tas[x]);//tabloda tasýn yeri güncellenir...
						white_tas[x].harf = ' ';
						white_tas[x].sayi = 0;
						bos.harf = harf[char_to_buyuk(hareket[0])];
						bos.sayi = char_to_int(hareket[1]);
						bos.reng = ' ';
						tablo_guncelle(bos);//eski bulundugu yer bos bir tas bastýrlýr...
						if (char_to_int(hareket[3]) == 8) {//eger 8 satýra gelmisse dama tasi bastirilir...
							if (dama_tasi_bul(harf[char_to_buyuk(hareket[2])], char_to_int(hareket[3]), 'w') == -1) {
								tas temp; temp.harf = harf[char_to_buyuk(hareket[2])]; temp.sayi = 8; temp.reng = 'w';
								tablo_guncelle_dama(temp);
							}
						}
					}
					else if (sira == 'b') {
						black_tas[x].harf = harf[char_to_buyuk(hareket[2])];//tasin yeni konumlarý atanýr...
						black_tas[x].sayi = char_to_int(hareket[3]);
						//cout << "white_tas[x]" << white_tas[x].reng << endl;
						tablo_guncelle(black_tas[x]);//tasin yeni yeri bastirilir...
						black_tas[x].harf = ' ';
						black_tas[x].sayi = 0;
						bos.harf = harf[char_to_buyuk(hareket[0])];
						bos.sayi = char_to_int(hareket[1]);
						bos.reng = ' ';
						tablo_guncelle(bos);//tasin eski konumuna bos bastirilir...
						if (char_to_int(hareket[3]) == 1) {
							if (dama_tasi_bul(harf[char_to_buyuk(hareket[2])], char_to_int(hareket[3]), 'b') == -1) {
								tas temp; temp.harf = harf[char_to_buyuk(hareket[2])]; temp.sayi = 1; temp.reng = 'b';//eger 1 bölgeye geldiyse dama tasi basilir....
								tablo_guncelle_dama(temp);
							}
						}
					}
				}
				else if (hareket1(hareket[0], hareket[1], hareket[2], hareket[3]) && yenilebilen_tas.size() != 0) {//yaðtýgý hareket dogru ve yenilebilen tas varsa
					validate = true;
					int x = bul(hareket[0], hareket[1]);
					if (sira == 'w') {
						white_tas[x].harf = harf[char_to_buyuk(hareket[2])];//Tasin yeni konumlarý atanir....
						white_tas[x].sayi = char_to_int(hareket[3]);
						//cout << "white_tas[x]" << white_tas[x].reng << endl;
						tablo_guncelle(white_tas[x]);
						bos.harf = harf[char_to_buyuk(hareket[0])];
						bos.sayi = char_to_int(hareket[1]);
						bos.reng = ' ';
						tablo_guncelle(bos);//tasin eski konumu bos olarak güncellenir...
						if (flag1 == 1) {//y ekseni hareket için...
							if (yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2]) != -1) {//gidecegi bölgenin arkasýnda yenilebilcek tas var mi..
								flag = 1;
								yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2])].reng = ' ';
								if (dama_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3])-2], 'b') != -1) {//gideceði bölgede dama tasi varsa
									tablo_guncelle_dama(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2])]);
								}
								black_tas[bul1(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2], 'b')].harf = ' ';
								black_tas[bul1(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2], 'b')].sayi = 0;
								tablo_guncelle(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2])]);//yenilen tasin yerine bos basilir...
								yenilebilen_tas.erase(yenilebilen_tas.begin() + yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2]));//ve bunu sileriz...
								if (char_to_int(hareket[3]) == 8) {//8 satira ulaþmýssa dama tasi basilir...
									if (dama_tasi_bul(harf[char_to_buyuk(hareket[2])], char_to_int(hareket[3]), 'w') == -1) {
										tas temp; temp.harf = harf[char_to_buyuk(hareket[2])]; temp.sayi = 8; temp.reng = 'w';
										tablo_guncelle_dama(temp);
									}
								}
							}
						}
						else if (flag2 == 1) {// x ekseni.. hareket için
							if (yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) - 1], char_sayi[char_to_int(hareket[3]) - 1]) != -1) {//yenilcek tas varsa...
								flag = 1;
								yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) - 1], char_sayi[char_to_int(hareket[3]) - 1])].reng = ' ';//yenilcek tasa bos reng
								if (dama_tasi_bul(harf[char_to_buyuk(hareket[0])-1], char_sayi[char_to_int(hareket[3]) - 1], 'b') != -1) {
									tablo_guncelle_dama(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) - 1], char_sayi[char_to_int(hareket[3]) - 1])]);//eger dama olan bir tassa ona göre güncelleme yapilir..
								}
								tablo_guncelle(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) - 1], char_sayi[char_to_int(hareket[3]) - 1])]);//yenilen tasin yerini günceller..
								yenilebilen_tas.erase(yenilebilen_tas.begin() + yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) - 1], char_sayi[char_to_int(hareket[3]) - 1]));
							}
							if (yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1]) != -1) {
								flag = 1;
								yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1])].reng = ' ';
								if (dama_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1], 'b') != -1) {
									tablo_guncelle_dama(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1])]);
								}
								tablo_guncelle(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1])]);
								yenilebilen_tas.erase(yenilebilen_tas.begin() + yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1]));
							}
						}
					}
					else if (sira == 'b') {//beyaz için yapýlan islemlerin aynisi bunun icinde yapýlýr...
						yenebilen_tas_var_mi();
						black_tas[x].harf = harf[char_to_buyuk(hareket[2])];
						black_tas[x].sayi = char_to_int(hareket[3]);
						//cout << "white_tas[x]" << white_tas[x].reng << endl;
						tablo_guncelle(black_tas[x]);
						bos.harf = harf[char_to_buyuk(hareket[0])];
						bos.sayi = char_to_int(hareket[1]);
						bos.reng = ' ';
						tablo_guncelle(bos);
						if (flag1 == 1) {//y ekseni
							//cout << "burdamýsýnlan" << endl;
							if (yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3])]) != -1) {
								flag = 1;
								yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3])])].reng = ' ';
								if (dama_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3])], 'w') != -1) {
									tablo_guncelle_dama(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3])])]);
								}
								white_tas[bul1(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2], 'w')].harf = ' ';
								white_tas[bul1(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3]) - 2], 'w')].sayi = 0;
								tablo_guncelle(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3])])]);
								yenilebilen_tas.erase(yenilebilen_tas.begin() + yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0])], char_sayi[char_to_int(hareket[3])]));
								if (char_to_int(hareket[3]) == 1) {
									if (dama_tasi_bul(harf[char_to_buyuk(hareket[2])], char_to_int(hareket[3]), 'b') == -1) {
										tas temp; temp.harf = harf[char_to_buyuk(hareket[2])]; temp.sayi = 1; temp.reng = 'b';
										tablo_guncelle_dama(temp);
									}
								}
							}
						}
						else if (flag2 == 1) {// x ekseni..
							if (yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) - 1], char_sayi[char_to_int(hareket[3]) - 1]) != -1) {
								flag = 1;
								yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) - 1], char_sayi[char_to_int(hareket[3]) - 1])].reng = ' ';
								if (dama_tasi_bul(harf[char_to_buyuk(hareket[0])-1], char_sayi[char_to_int(hareket[3]-1)], 'w') != -1) {
									tablo_guncelle_dama(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) - 1], char_sayi[char_to_int(hareket[3]) - 1])]);
								}
								tablo_guncelle(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) - 1], char_sayi[char_to_int(hareket[3]) - 1])]);
								yenilebilen_tas.erase(yenilebilen_tas.begin() + yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) - 1], char_sayi[char_to_int(hareket[3]) - 1]));
							}
							if (yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1]) != -1) {
								flag = 1;
								yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1])].reng = ' ';
								if (dama_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3] - 1)], 'w') != -1) {
									tablo_guncelle_dama(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1])]);
								}
								tablo_guncelle(yenilebilen_tas[yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1])]);
								yenilebilen_tas.erase(yenilebilen_tas.begin() + yenilebilir_tasi_bul(harf[char_to_buyuk(hareket[0]) + 1], char_sayi[char_to_int(hareket[3]) - 1]));
							}
						}
					}
					tablo_oku();
				}
				else {
					validate = false;
				}
			}
			else {
				validate = false;
			}
		}
	}
	for (int i = 0; i < 16; i++) {//beyaz ve siyah tas'ýn dizi degerleri bosaltýlýr...
		white_tas[i].harf = ' '; white_tas[i].reng = ' '; white_tas[i].sayi = 0;
		black_tas[i].harf = ' '; black_tas[i].reng = ' '; black_tas[i].sayi = 0;
	}
	dama.clear();
	yenilebilen_tas.clear();
	tablo_oku_printfsiz();//tabloyu okur...
	yenebilen_tas_var_mi();
	if (yenilebilen_tas.size() > 0 && flag == 1) {//Eger yapýlan hereketlere ragmen yenilebilen tas varsa sira yine degismez..
		//int sayacx = 0;
		if (sira == 'w') {
			cout << "Team white your turn.." << endl;
		}
		else
			cout << "team black your nturn " << endl;
		cin >> hareket;
		tablo_oku_printfsiz();
		yenilebilen_tas.clear();
		tablo_oku_printfsiz();
		yenebilen_tas_var_mi();
		cout << "yenilebilen_tas.size()" << yenilebilen_tas.size() << endl;
		for (int i = 0; i < yenilebilen_tas.size(); i++) {
			//cout << "yenilebilen_tas" << yenilebilen_tas[i].harf << yenilebilen_tas[i].sayi << yenilebilen_tas[i].reng << endl;
			tablo_oku_printfsiz();
			yenebilen_tas_var_mi();
			if ((bul1(yenilebilen_tas[i].harf, char_sayi[yenilebilen_tas[i].sayi - 1], 'w') == -1 || bul1(yenilebilen_tas[i].harf, char_sayi[yenilebilen_tas[i].sayi - 1], 'b') == -1)) {
				yenilebilen_tas.erase(yenilebilen_tas.begin() + yenilebilir_tasi_bul(yenilebilen_tas[i].harf, char_sayi[yenilebilen_tas[i].sayi - 1]));//eger birden fazla ayný tas varsa silinir...
			}
		}
		//tablo_oku();
		hamleYaz(hareket);
		return input_kontrol(hareket);
	}
	tablo_oku();
	sira_bastir();
	return validate;
}
bool yenilebilir_tasi_check() {
	for (int i = 0; i < yenilebilen_tas.size(); i++) {
		if (yenilebilen_tas[i].reng == 'w') {
			if (bul1(yenilebilen_tas[i].harf, char_sayi[char_to_int(yenilebilen_tas[i].sayi)], 'b') != -1) {
				//cout << "1...." << endl;
				return true;
			}
			else if (bul1(harf[char_to_buyuk(yenilebilen_tas[i].harf) + 1], char_sayi[char_to_int(yenilebilen_tas[i].sayi) - 1], 'b') != -1 || bul1(harf[char_to_buyuk(yenilebilen_tas[i].harf) - 1], char_sayi[char_to_int(yenilebilen_tas[i].sayi) - 1], 'b') != -1) {
				//cout << "2...." << endl;
				return true;
			}
		}
		else if (yenilebilen_tas[i].reng == 'b') {
			if (bul1(yenilebilen_tas[i].harf, char_sayi[char_to_int(yenilebilen_tas[i].sayi) - 2], 'w') != -1) {
				//cout << "3...." << endl;
				return true;
			}
			else if (bul1(harf[char_to_buyuk(yenilebilen_tas[i].harf) + 1], char_sayi[char_to_int(yenilebilen_tas[i].sayi) - 1], 'w') != -1 || bul1(harf[char_to_buyuk(yenilebilen_tas[i].harf) - 1], char_sayi[char_to_int(yenilebilen_tas[i].sayi) - 1], 'w') != -1) {
				//cout << "4...." << endl;
				return true;
			}
		}
	}
	return false;
}
int yenilebilir_tasi_bul(char hareket_2, char hareket_3) {//girilen komutlar ile yenilebilen tasi bulunur...
	int a = -1;
	for (int i = 0; i < yenilebilen_tas.size(); i++) {
		if ((yenilebilen_tas[i].harf == hareket_2 && char_sayi[yenilebilen_tas[i].sayi-1] == hareket_3)) {
			//cout << "yenilebilen_tas" << yenilebilen_tas[i].harf << " " << yenilebilen_tas[i].sayi << " " << yenilebilen_tas[i].reng << endl;
			return i;
		}
	}
	return a;
}
int bul(char hareket_0, char hareket_1) {//dama tasi bulma...
	//cout << "sira" << sira << endl;
	tas* temp = black_tas;
	int a = -1;
	int sayac1 = 0;
	char temp3 = harf[char_to_buyuk(hareket_0)];
	int temp2 = char_to_int(hareket_1);
	if (sira == 'w') {
		//cout << "sira" << sira << endl;
		temp = white_tas;
	}
	/*else
		temp = black_tas;*/
	for (int i = 0; i < 16; i++) {//Bulunan tasin indisini gönderirir...
		if (temp3 == temp[i].harf && temp2 == temp[i].sayi)
			return i;
	}
	return a;
}
int bul1(char hareket_0, char hareket_1, char sira1) {//dama tasi bulma...
	//cout << "sira" << sira << endl;
	tas* temp = black_tas;
	int a = -1;
	int sayac1 = 0;
	char temp3 = ' '; int temp2 = 0;
	if (char_to_buyuk(hareket_0) != -1 && char_to_int(hareket_1) != -1) {
		temp3 = harf[char_to_buyuk(hareket_0)];
		temp2 = char_to_int(hareket_1);
	}
	if (sira1 == 'w') {
		//cout << "sira" << sira << endl;
		temp = white_tas;
	}
	/*else
		temp = black_tas;*/
	for (int i = 0; i < 16; i++) {
		if (temp3 == temp[i].harf && temp2 == temp[i].sayi)//Bulunmasýný istedðimiz taþýn indisini gönderir..
			return i;
	}
	return a;//Eger bulunamazsa a degerini -1 olarak gönderir..
}
bool hareket1(char hareket_0, char hareket_1, char hareket_2, char hareket_3) {
	int adim_y = 0;
	int adim_x = 0;
	flag1 = 0, flag2 = 0;
	yenebilen_tas_var_mi();
	if (hareket_0 == hareket_2) {//y ekseninde hareket..
		flag1 = 1;
		adim_y = char_to_int(hareket_3) - char_to_int(hareket_1);
	}
	else if (hareket_1 == hareket_3) {// x ekseninde hareket
		flag2 = 1;
		adim_x = char_to_buyuk(hareket_0) - char_to_buyuk(hareket_2);
	}
	else
		return false;
	if (adim_y < 0 && sira == 'w' || adim_y > 0 && sira == 'b') {//y ekseninde yapýlan hareketin dogrulugu denetleniyor...
		return false;
	}
	/*else if (adim_x > 0 && sira == 'w' || adim_x < 0 && sira == 'b')
		return false;*/
	else if ((abs(adim_y) == 1 || abs(adim_x) == 1) && yenilebilen_tas.size() == 0) {
		if (bul1(hareket_2, hareket_3, 'w') == -1 && bul1(hareket_2, hareket_3, 'b') == -1 && dama_tasi_bul(harf[char_to_buyuk(hareket_2)], hareket_3, 'w') == -1 && dama_tasi_bul(harf[char_to_buyuk(hareket_2)], hareket_3, 'b') == -1)
			return true;
	}
	else if ((abs(adim_y) == 2 || abs(adim_x) == 2) && yenilebilen_tas.size() != 0) {
		if (flag1 == 1) {
			if (char_to_int(hareket_3) - 2 > 0 || char_to_int(hareket_3) < 8) {
				if (bul1(hareket_2, char_sayi[char_to_int(hareket_3) - 2], 'w') != -1 || bul1(hareket_2, char_sayi[char_to_int(hareket_3) + 1], 'b') != -1 || dama_tasi_bul(hareket_2, char_sayi[char_to_int(hareket_3) - 2], 'w') != -1 || dama_tasi_bul(hareket_2, char_sayi[char_to_int(hareket_3) + 1], 'b') != -1) {}
				return true;
			}
		}
		if (flag2 == 1) {
			if (sira == 'w') {
				if (bul1(harf[char_to_buyuk(hareket_2) - 1], hareket_3, 'b') != -1 || bul1(harf[char_to_buyuk(hareket_2) + 1], hareket_3, 'b') != -1 || dama_tasi_bul(harf[char_to_buyuk(hareket_2) - 1], hareket_3, 'b') != -1 || dama_tasi_bul(harf[char_to_buyuk(hareket_2) + 1], hareket_3, 'b') != -1)
					return true;
			}
			else {
				if (bul1(harf[char_to_buyuk(hareket_2) - 1], hareket_3, 'w') != -1 || bul1(harf[char_to_buyuk(hareket_2) + 1], hareket_3, 'w') != -1 || dama_tasi_bul(harf[char_to_buyuk(hareket_2) - 1], hareket_3, 'w') != -1 || dama_tasi_bul(harf[char_to_buyuk(hareket_2) + 1], hareket_3, 'w') != -1)
					return true;
			}
		}
	}
	return false;
}
int char_to_int(char hareket) {
	int x = -1;
	for (int i = 0; i < 8; i++) {
		if (hareket == char_sayi[i])
			return x = i + 1;
	}
	return x;
}
int char_to_buyuk(char hareket) {
	int x = -1;
	for (int i = 0; i < 8; i++) {
		if (kucuk_harf[i] == hareket) {
			//cout << "x: " << i << endl;
			x = i;
		}
		else if (harf[i] == hareket) {
			x = i;
		}
	}
	return x;
}
void tas_hareketi() {
	string hareket;
	do {
		if (sira == 'w') {
			cout << "Team white enter your command..(Cýkmak icin -2)" << endl;
		}
		else
			cout << "Team black enter your command..(Cýkmak icin -2)" << endl;
		cin >> hareket;
		adim = hareket;//
		if (hareket != "-2") {
			while (input_kontrol(hareket) == false) {
				if (input_kontrol(hareket) == false) {
					cout << "Wrong command...";
					cin >> hareket;
				}
				adim = hareket;
			}

			hamleYaz(hareket);
		}
	} while (hareket != "-2");
}//
void tablo_guncelle(tas temp) {
	//gönderilen tasýn degerlerine baglý olarak tasýn bulundugu yer güncellenir...
	errno_t err = fopen_s(&stream, "tahta.dat", "rb+");
	char read;
	int a = 0;
	for (int i = 0; i < temp.sayi; i++) {
		if (i == temp.sayi - 1 && temp.harf != 'A') {
			a = a + char_to_buyuk(temp.harf) + 3 + char_to_buyuk(temp.harf) + char_to_buyuk(temp.harf);
		}
		else if (i == temp.sayi - 1) {
			a = a + char_to_buyuk(temp.harf) + 3 + char_to_buyuk(temp.harf);
			//cout << "char_to_buyuk('a')" << char_to_buyuk(temp.harf) << endl;
		}
		else
			a = a + 27;//Bir satýrda 27 tane char elemaný var..
	}
	//cout << "a:" << a << endl;
	fseek(stream, sizeof(char) * (a), SEEK_SET);
	fwrite(&temp.reng, sizeof(char), 1, stream);
	fclose(stream);
}//
void tablo_oku() {
	errno_t err = fopen_s(&stream2, "tahta.dat", "rb");
	char read, temp_read;
	//int flag7 = 0;
	char sayi1 = NULL, harf1 = NULL, renk = NULL;
	int harf_sayaci = 0, sayac = 0, sayac1 = 0, temp = -2;
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 27; i++) {
			fread(&read, sizeof(char), 1, stream2);
			cout << read;
			if (i == 0) {
				sayi1 = read;
			}
			if (i != 0 && i % 3 == 0) {
				harf1 = harf[harf_sayaci];
				++harf_sayaci;
				if (harf_sayaci > 7)
					harf_sayaci = 0;
			}
			if (read == 'w') {
				if (temp + 1 == i && temp_read == 'w') {
					if (dama_tasi_bul(harf1, char_to_int(sayi1), 'w') == -1) {
						tas temp_tas; temp_tas.harf = harf1; temp_tas.reng = 'w'; temp_tas.sayi = char_to_int(sayi1);
						dama.push_back(temp_tas);
					}
				}
				else {
					renk = read;
					white_tas[sayac1].harf = harf1;
					white_tas[sayac1].sayi = char_to_int(sayi1);
					white_tas[sayac1].reng = renk;
					//cout << "white_tas[sayac1].harf" << white_tas[sayac1].harf << endl;
					++sayac1;
				}
				temp = i;
				temp_read = read;
			}
			if (read == 'b') {
				if (temp + 1 == i) {
					if (dama_tasi_bul(harf1, char_to_int(sayi1), 'b') == -1) {
						tas temp_tas; temp_tas.harf = harf1; temp_tas.reng = 'b'; temp_tas.sayi = char_to_int(sayi1);
						dama.push_back(temp_tas);
					}
				}
				else {
					temp_read = read;
					temp = i;
					renk = read;
					black_tas[sayac].harf = harf1;
					black_tas[sayac].sayi = char_to_int(sayi1);
					black_tas[sayac].reng = renk;
					//cout << "black_tas[sayac1].harf" << black_tas[sayac].harf <<  endl;
					++sayac;
				}
				temp = i;
				temp_read = read;
			}
		}
	}
	fread(&sira, sizeof(char), 1, stream2);
	cout << sira;
	fclose(stream2);
	for (int i = 0; i < dama.size(); i++) {
		if (bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'w') != -1) {
			white_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'w')].harf = ' '; white_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'w')].reng = ' '; white_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'w')].sayi = 0;
		}
		else if (bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'b') != -1) {
			black_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'b')].harf = ' '; black_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'b')].reng = ' '; black_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'b')].sayi = 0;
		}
	}
}////
void tablo_oku_printfsiz() {
	errno_t err = fopen_s(&stream2, "tahta.dat", "rb");
	char read, temp_read;
	//int flag7 = 0;
	char sayi1 = NULL, harf1 = NULL, renk = NULL;
	int harf_sayaci = 0, sayac = 0, sayac1 = 0, temp = -2;
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 27; i++) {
			fread(&read, sizeof(char), 1, stream2);
			//cout << read;
			if (i == 0) {
				sayi1 = read;
			}
			if (i != 0 && i % 3 == 0) {
				harf1 = harf[harf_sayaci];
				++harf_sayaci;
				if (harf_sayaci > 7)
					harf_sayaci = 0;
			}
			if (read == 'w') {
				if (temp + 1 == i && temp_read == 'w') {
					if (dama_tasi_bul(harf1, char_to_int(sayi1), 'w') == -1) {
						tas temp_tas; temp_tas.harf = harf1; temp_tas.reng = 'w'; temp_tas.sayi = char_to_int(sayi1);
						dama.push_back(temp_tas);
					}
				}
				else {
					renk = read;
					white_tas[sayac1].harf = harf1;
					white_tas[sayac1].sayi = char_to_int(sayi1);
					white_tas[sayac1].reng = renk;
					//cout << "white_tas[sayac1].harf" << white_tas[sayac1].harf << endl;
					++sayac1;
				}
				temp = i;
				temp_read = read;
			}
			if (read == 'b') {
				if (temp + 1 == i) {
					if (dama_tasi_bul(harf1, char_to_int(sayi1), 'b') == -1) {
						tas temp_tas; temp_tas.harf = harf1; temp_tas.reng = 'b'; temp_tas.sayi = char_to_int(sayi1);
						dama.push_back(temp_tas);
					}
				}
				else {
					temp_read = read;
					temp = i;
					renk = read;
					black_tas[sayac].harf = harf1;
					black_tas[sayac].sayi = char_to_int(sayi1);
					black_tas[sayac].reng = renk;
					//cout << "black_tas[sayac1].harf" << black_tas[sayac].harf <<  endl;
					++sayac;
				}
				temp = i;
				temp_read = read;
			}
		}
	}
	fread(&sira, sizeof(char), 1, stream2);
	cout << sira;
	fclose(stream2);
	for (int i = 0; i < dama.size(); i++) {
		if (bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'w') != -1) {
			white_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'w')].harf = ' '; white_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'w')].reng = ' '; white_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'w')].sayi = 0;
		}
		else if (bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'b') != -1) {
			black_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'b')].harf = ' '; black_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'b')].reng = ' '; black_tas[bul1(dama[i].harf, char_sayi[dama[i].sayi - 1], 'b')].sayi = 0;
		}
	}
}
void yenebilen_tas_var_mi() {
	//dama tahatsýndaki yenilebilcek taslarin yerini buluruz...
	tas* temp; tas* temp2;
	char sira2;
	int a, b,c,d;
	if (sira == 'w') {
		temp = white_tas;
		temp2 = black_tas;
		sira2 = 'b';
	}
	else {
		temp = black_tas;
		temp2 = white_tas;
		sira2 = 'w';
	}
	for (int i = 0; i < 16; i++) {
		//int a = bul(temp[i].harf, char_sayi[temp[i].sayi-1]);
		//cout << i << ":" <<temp[i].harf << temp[i].sayi << endl;
		if (sira == 'w') {
			if (temp[i].sayi < 8 && temp[i].sayi + 1 < 8) {
				a = bul1(harf[char_to_buyuk(temp[i].harf)], char_sayi[temp[i].sayi], 'b');
				b = bul1(harf[char_to_buyuk(temp[i].harf)], char_sayi[temp[i].sayi + 1], 'b');
				c = dama_tasi_bul(harf[char_to_buyuk(temp[i].harf)], char_sayi[temp[i].sayi], 'b');
				//cout << "ses: " << a << endl;
				if ((a != -1 ||c != -1) && b == -1) {
					//cout << 'a1.' << ":" << temp2[a].harf << temp2[a].sayi << endl;
					if (a == -1) { temp2 = &dama[0]; d = c; }else { d = a; }
					if (yenilebilir_tasi_bul(temp2[d].harf, char_sayi[temp2[d].sayi - 1]) == -1)
						yenilebilen_tas.push_back(temp2[d]);
					else if (yenilebilen_tas.size() == 0) {
						yenilebilen_tas.push_back(temp2[d]);
						//cout << "temp2[a]" << temp2[d].harf << temp2[d].sayi << temp2[d].reng << endl;
					}
				}
			}
		}
		if (sira == 'b') {
			if (temp[i].sayi - 1 > 0 && temp[i].sayi - 2 > 0) {
				a = bul1(harf[char_to_buyuk(temp[i].harf)], char_sayi[temp[i].sayi - 2], 'w');
				b = bul1(harf[char_to_buyuk(temp[i].harf)], char_sayi[temp[i].sayi - 3], 'w');
				c = dama_tasi_bul(harf[char_to_buyuk(temp[i].harf)], char_sayi[temp[i].sayi-2], 'w');
				//cout << "ses: " << a << endl;
				if ((a != -1 || c != -1) && b == -1) {
					//cout << 'a1.' << ":" << temp2[a].harf << temp2[a].sayi << endl;
					if (a == -1) { temp2 = &dama[0];d = c; }else { d = a; }
					if (yenilebilir_tasi_bul(temp2[d].harf, char_sayi[temp2[d].sayi - 1]) == -1)
						yenilebilen_tas.push_back(temp2[d]);
					else if (yenilebilen_tas.size() == 0) {
						yenilebilen_tas.push_back(temp2[d]);
					}
				}
			}
		}
		if (char_to_buyuk(temp[i].harf) + 1 < 8 && char_to_buyuk(temp[i].harf) + 2 < 8) {
			a = bul1(harf[char_to_buyuk(temp[i].harf) + 1], char_sayi[temp[i].sayi - 1], sira2);
			b = bul1(harf[char_to_buyuk(temp[i].harf) + 2], char_sayi[temp[i].sayi - 1], sira2);
			c = dama_tasi_bul(harf[char_to_buyuk(temp[i].harf)+1], char_sayi[temp[i].sayi - 1], sira2);
			//cout << "ses: " << a << endl;
			if ((a != -1 || c != -1) && b == -1) {
				//cout << 'a1.' << ":" << temp2[a].harf << temp2[a].sayi << endl;
				if (a == -1) { temp2 = &dama[0]; d = c; }else { d = a; }
				if (yenilebilir_tasi_bul(temp2[d].harf, char_sayi[temp2[d].sayi - 1]) == -1)
					yenilebilen_tas.push_back(temp2[d]);
				else if (yenilebilen_tas.size() == 0) {
					yenilebilen_tas.push_back(temp2[d]);
				}
			}
		}
		if (char_to_buyuk(temp[i].harf) - 1 > 0 && char_to_buyuk(temp[i].harf) - 2 >= 0) {
			a = bul1(harf[char_to_buyuk(temp[i].harf) - 1], char_sayi[temp[i].sayi - 1], sira2);
			b = bul1(harf[char_to_buyuk(temp[i].harf) - 2], char_sayi[temp[i].sayi - 1], sira2);
			c = dama_tasi_bul(harf[char_to_buyuk(temp[i].harf)-1], char_sayi[temp[i].sayi - 1], sira2);
			//cout << "ses: " << a << endl;
			if ((a != -1 || c != -1) && b == -1) {
				//cout << 'a1.' << ":" << temp2[a].harf << temp2[a].sayi << endl;
				if (a == -1) { temp2 = &dama[0]; d = c; }else { d = a; }
				if (yenilebilir_tasi_bul(temp2[d].harf, char_sayi[temp2[d].sayi - 1]) == -1)
					yenilebilen_tas.push_back(temp2[d]);
				else if (yenilebilen_tas.size() == 0) {
					yenilebilen_tas.push_back(temp2[d]);
				}
			}
		}
	}
}
void sira_bastir() {
	errno_t err = fopen_s(&stream, "tahta.dat", "rb+");
	char read;//for döngüsü ile dosyanýn sonuna gelip güncelleme yapýyoruz...
	int a = 0;
	for (int i = 0; i < 10; i++) {
		if (i == 9) {

		}
		else
			a = a + 27;
	}
	//cout << "a:" << a << endl;
	fseek(stream, sizeof(char) * (a), SEEK_SET);
	if (sira == 'w') {
		sira = 'b';
	}
	else
		sira = 'w';
	fwrite(&sira, sizeof(char), 1, stream);
	fclose(stream);
}
void tablo_guncelle_dama(tas temp) {
	errno_t err = fopen_s(&stream, "tahta.dat", "rb+");//for döngüleri dama tasýnýn bulundugu konuma gelip güncelleme yapýyoruz..
	char read; char bosluk = '\n';
	int a = 0, b = 0;
	for (int i = 0; i < temp.sayi; i++) {
		if (i == temp.sayi - 1 && temp.harf != 'A') {
			a = a + char_to_buyuk(temp.harf) + 4 + char_to_buyuk(temp.harf) + char_to_buyuk(temp.harf);
		}
		else if (i == temp.sayi - 1) {
			a = a + char_to_buyuk(temp.harf) + 4 + char_to_buyuk(temp.harf);
			//cout << "char_to_buyuk('a')" << char_to_buyuk(temp.harf) << endl;
		}
		else
			a = a + 27;
	}
	//cout << "a:" << a << endl;
	fseek(stream, sizeof(char) * (a), SEEK_SET);
	fwrite(&temp.reng, sizeof(char), 1, stream);
	fclose(stream);
}
int dama_tasi_bul(char hareket_0, char hareket_1, char sira) {
	int a = -1;//dama tasinin indisini döndürür bulamazsa a'nýn degeri -1 gönderir...
	hareket_0 = harf[char_to_buyuk(hareket_0)];
	for (int i = 0; i < dama.size(); i++) {
		if (dama[i].harf == hareket_0 && dama[i].sayi == char_to_int(hareket_1) && dama[i].reng == sira) {
			return i;
		}
	}
	return a;
}
void hamleYaz(string hamle) {
	errno_t err; char enter = '\n';
	char g = ':', sira2; if (sira == 'w') { sira2 = 'b'; }else { sira2 = 'w'; }
	err = fopen_s(&stream, "hamle.dat", "a+");
	if (err != 0)
	{
		printf("The file 'hamle.dat' was not opened\n");
		return;
	}
	//fseek(stream, sizeof(char) * 1, SEEK_CUR);
	fwrite(&sira2, sizeof(char), 1, stream); fwrite(&g, sizeof(char), 1, stream);
	fwrite(&hamle[0], sizeof(char), 1, stream); fwrite(&hamle[1], sizeof(char), 1, stream); fwrite(&hamle[2], sizeof(char), 1, stream); fwrite(&hamle[3], sizeof(char), 1, stream); fwrite(&enter, sizeof(char), 1, stream);
	//fseek(stream, sizeof(char) * 1, SEEK_END);
	fclose(stream);
}
void hamleYaz_sil() {
	errno_t err = fopen_s(&stream2, "hamle.dat", "w+");
	if (err != 0)
	{
		printf("The file 'crt_fopen_s.c' was not opened\n");
		return;
	}
	fclose(stream2);
}
int main() {
	//int i = 0;
	string n, a,i;
	tas* yenilebilir = NULL;
	//cout << i << '\n';
	while (i != "3") {
		cout << "\n1-) New Game 2-) Contiune Game 3-) End Game: ";
		cin >> i;
		//i = stoi(a);
		
		if (i == "1") {
			hamleYaz_sil();
			dosya_ac();
			tahta_olustur();
			fclose(stream);
			tahta_bastir();
			fclose(stream);
		}
		else if (i == "2") {
			//yenebilen_tas_var_mi();
			tablo_oku();
			tas_hareketi();
			sira_bastir();
		}
		else if(i == "3")
			cout << "cikis\n";
		else
			cout << "Yanlis Girdiniz...\n";
	}
}