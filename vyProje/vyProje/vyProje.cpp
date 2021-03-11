#include <string>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <locale.h>
#include "ConsoleColor.h"
#include <time.h>

using namespace std;

struct node {
	node* next;		
	node* prev;		
	int data;		
	int sira;		
};

clock_t Tbasla, Tbitir, Ybasla, Ybitir;
int sayac = 0, tekrar = 0, mesafe = 100, geciciMesafe = 0;;
char c;
bool kelimeTekmi = false;  
string aranan = "";
string benzerKelime;
string metin = "";


string kelimeKucult(string kelime)  
{
	for (int i = 0; i < kelime.length(); i++)
	{
		kelime[i] = tolower(kelime[i]);
	}
	return kelime;
}

int** matrisOlustur(int m, int n)
{
	int** Matrix;

	Matrix = new int* [m];  
	for (int i = 0; i < m; i++)   
		Matrix[i] = new int[n];	

	return Matrix;
}

node* listeyeEkle(node* r, int x, int s) 
{
	if (r == NULL)				
	{
		r = new node;
		r->data = x;
		r->sira = s;
		r->next = r;
		r->prev = r;
	}
	else {						
		node* tmp;
		tmp = new node;
		tmp->data = x;
		tmp->sira = s;
		r->prev->next = tmp;
		tmp->next = r;
		tmp->prev = r->prev;
		r->prev = tmp;
	}
	return r;
}

void yalnizKelimemi(char SolHarf, char SagHarf)		 
{
	if (!((65 <= SolHarf && SolHarf <= 90) || (97 <= SolHarf && SolHarf <= 122)))   
	{
		if (!((65 <= SagHarf && SagHarf <= 90) || (97 <= SagHarf && SagHarf <= 122)))  
		{																						
			kelimeTekmi = true;
		}
	}
}


int tamEslesme(string p, string t, int i) 
{  
	int m = p.length();     
	int n = t.length();		
	int j = 0;				

	while (i < t.length())   
	{
		if (tolower(t[i]) == tolower(p[j])) 
		{
			if (j == p.length() - 1)	
			{
				if ((i - (int)p.length()) >= 0 && (i + 1) < t.length())		
				{															
					yalnizKelimemi(t[i - (int)p.length()], t[i + 1]);	
				}
				return i - j;	
			}
			else			
			{					
				i += 1;
				j += 1;
			}
		}
		else       
		{
			if (j > 0)    
			{					
				j = 0;
			}
			else  
			{
				i += 1;		
			}
		}
	}
	return -1000; 
}


pair<node*, int> kelimeleriListele(node* r, string kelime, string metinParcasi, bool Listelensinmi)
{
	sayac = 0;

	if (r != NULL)
		r = NULL;

	int BulunanIndis = kelime.length() * -1;

	if (Listelensinmi)
		Tbasla = clock();
	while (BulunanIndis != -1000)
	{
		BulunanIndis = tamEslesme(kelime, metinParcasi, BulunanIndis + kelime.length());

		if (BulunanIndis != -1000)
		{
			sayac++;

			if (Listelensinmi)
				r = listeyeEkle(r, BulunanIndis, sayac);
		}
	}
	if (Listelensinmi)
		Tbitir = clock();

	return make_pair(r, sayac);
}


int minimum(int a, int b, int c) 
{
	if (a <= b && a <= c)
		return a;
	if (b <= c && b <= a)
		return b;
	if (c <= a && c <= b)
		return c;
}

int levenshtein(string k, string o) 
{
	int m = k.length();			
	int n = o.length();			

	int** d = matrisOlustur(m + 1, n + 1);   

	for (int i = 0; i < m + 1; i++)			
		d[i][0] = i;
	for (int i = 0; i < n + 1; i++)			
		d[0][i] = i;


	for (int i = 1; i <= m; i++)		
	{
		for (int j = 1; j <= n; j++)
		{

			if (k[i - 1] == o[j - 1])	
			{
				d[i][j] = d[i - 1][j - 1];	
			}
			else
			{
				d[i][j] = minimum(					
					d[i - 1][j] + 1,  
					d[i][j - 1] + 1,  
					d[i - 1][j - 1] + 1 
				);
			}
		}
	}
	return d[m][n];		
}


void nodeIcindeGez(node* r, string kelime)  
{
	kelimeTekmi = false;		
	string yeniMetin = "";      
	int geciciTekrar = 0;		
	int aciklik = 3;			

	if (r->data < aciklik)    
		aciklik = r->data;

	if (metin.length() - r->data - kelime.length() < aciklik) 
		aciklik = metin.length() - r->data - kelime.length();

	node* yrd = r;
	do
	{
		yeniMetin = metin.substr(yrd->data - aciklik, kelime.length() + (2 * aciklik)); 
		geciciTekrar += kelimeleriListele(NULL, kelime, yeniMetin, false).second;       

		yrd = yrd->next;
	} while (yrd != r);

	if (tekrar <= geciciTekrar && mesafe >= geciciMesafe && kelimeTekmi) 
	{
		mesafe = geciciMesafe; 
		tekrar = geciciTekrar;
		benzerKelime = kelime;
	}

}

bool benzerlikKontrol(node* r, string kelime, bool hicVarmi)
{
	benzerKelime = "-";

	Ybasla = clock();
	for (int i = -2; i <= 2; i++)
	{
		if (kelime.length() + i > 1)
		{
			string isim = to_string(kelime.length() + i) + "_karakter.txt";
			string okunan;

			ifstream dosyaoku(isim);
			while (!dosyaoku.eof())
			{
				getline(dosyaoku, okunan);
				geciciMesafe = levenshtein(kelime, okunan);
				if (geciciMesafe < 4 && kelime != okunan)
				{
					if (hicVarmi)
					{
						nodeIcindeGez(r, okunan);
					}
					else
					{
						if (mesafe >= geciciMesafe)
						{
							mesafe = geciciMesafe;
							benzerKelime = okunan;
						}
					}
				}
			}
			dosyaoku.close();
		}
	}
	Ybitir = clock();

	if (benzerKelime != "-")
	{
		cout << "Bunu mu demek istediniz? = " << benzerKelime << endl << " e : Evet " << endl << " h : Hayır " << endl;
		cout << yellow << "Yakın eşleşme hızı: " << (float)(Ybitir - Ybasla) / CLOCKS_PER_SEC << "sn" << white << endl;

		switch (tolower(_getch()))
		{
		case'e':
			return true;
			break;
		}
	}

	return false;
}


void dosyadanOku(string dosya)
{
	ifstream dosyaoku(dosya);
	string okunan;
	while (!dosyaoku.eof())		
	{
		getline(dosyaoku, okunan);		
		metin += okunan + " ";
	}
	dosyaoku.close();
}


void goster(node* r)
{								
	int aciklik = 275;     

	if (r->data < aciklik)    
		aciklik = r->data;

	cout << metin.substr(r->data - aciklik, aciklik);  

	cout << red << metin.substr(r->data, aranan.length()) << white;   

	if (metin.length() - r->data - aranan.length() < aciklik)  
		aciklik = metin.length() - r->data - aranan.length();

	cout << metin.substr((r->data) + aranan.length(), aciklik);  

	cout << "\n\n\n" << green << r->sira << white << " / " << green << sayac << white << endl;  
}																							

void gez(node* r)
{
	sayac = r->prev->sira;
	do {						
		system("cls");
		goster(r);		
		cout << yellow << "Tam eşleşme hızı: " << (float)(Tbitir - Tbasla) / CLOCKS_PER_SEC << "sn" << white << endl;

		c = _getch();
		switch (c)
		{
		case 77:   
			r = r->next;
			break;
		case 75:   
			r = r->prev;
			break;
		}
	} while (c != 'x');  
}

void dosyaSecim() 
{ 
	system("cls");
	char ds;
	cout << "1- Crime _and_Punishment.xml\n";
	cout << "2- Gullivers_Travel.html\n";
	cout << "3- Robinson_Crusoe.txt             Hangi dosyada arama yapmak istersiniz ?\n";
	cout << "4- The_Adventures.docx\n";
	cout << "5- Tüm Dosyalar                    "; ds = _getch();

	switch (ds)
	{
	case '1':
		dosyadanOku("Crime _and_Punishment.xml");
		break;
	case '2':
		dosyadanOku("Gullivers_Travel.html");
		break;
	case '3':
		dosyadanOku("Robinson_Crusoe.txt");
		break;
	case '4':
		dosyadanOku("The_Adventures.docx");
		break;
	case '5':
		dosyadanOku("Crime _and_Punishment.xml");
		dosyadanOku("Gullivers_Travel.html");
		dosyadanOku("Robinson_Crusoe.txt");
		dosyadanOku("The_Adventures.docx");
		break;
	default:
		dosyaSecim();
		break;
	}
	system("cls");
}

int main(void) 
{
	setlocale(LC_ALL, "Turkish");
	node* root;
	root = NULL;

	dosyaSecim(); 

	cout << "Aranacak Kelimeyi Giriniz  : ";
	getline(cin, aranan);
	aranan = kelimeKucult(aranan); 


	root = kelimeleriListele(root, aranan, metin, true).first;   

	if (root != NULL)		
	{
		if (kelimeTekmi)		
			gez(root);
		else				
		{
			if (benzerlikKontrol(root, aranan, true))  
			{
				aranan = benzerKelime;
				root = kelimeleriListele(root, aranan, metin, true).first; 

				if (root != NULL)  
					gez(root);
				else              
					cout << "Kelime bulunamadı.";
			}
			else                                
			{
				gez(root);
			}
		}
	}
	else						 
	{
		if (benzerlikKontrol(NULL, aranan, false))  
		{
			aranan = benzerKelime;
			root = kelimeleriListele(root, aranan, metin, true).first;  

			if (root != NULL)		
				gez(root);
			else                   
				cout << "Kelime bulunamadı.";
		}
		else                      
			cout << "Kelime bulunamadı.";
	}

	return 0;
}
