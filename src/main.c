/*
 * main.c
 *
 *  Created on: Apr 24, 2018
 *      Author: Batuhan Faik Derinbay
 */

#include <stdio.h>
#include <string.h>
#include <locale.h>

#define MAXCHAR 1000
#define MAXWORD_LENGTH 48 /*onerilen uzunluklar
						   * 20 - kisa kelimelerden olusan basit cumleler icin
						   * 32 - turemis yapida kelimeler iceren basit cumleler icin
						   * 48 - öntanımlı (default) değer
						   * 64 - turemis yapida kelimeleri bolca iceren karmasik cumleler icin
						   * 128 - kelimelerin wordlist'te bulunma ihtimali olmadigi ve
						   * 	   baglaclarla birlestirilmis uzun cumleler icin */
#define MAXCHAR_WORDLIST 610000
#define WORDLIST_LENGTH 610000

int main() {
	setlocale(LC_ALL, "Turkish");

	FILE *atxt;
	FILE *btxt;
	FILE *wordList;
	char nonSpaced[MAXCHAR], wordListArray[MAXCHAR_WORDLIST], spaced[MAXCHAR];
	char* fileAtxt =
			"/home/batuhanfaik/eclipse/workspace/cpp-oxygen/SozcukAyirma_BBG2/texts/a.txt";
	char* fileBtxt =
			"/home/batuhanfaik/eclipse/workspace/cpp-oxygen/SozcukAyirma_BBG2/texts/b.txt";
	char* fileWordList =
			"/home/batuhanfaik/eclipse/workspace/cpp-oxygen/SozcukAyirma_BBG2/texts/wordlist.txt";

	//Dosyalari acma ve okuma
	atxt = fopen(fileAtxt, "r");
	if (atxt == NULL) {
		printf("Dosya acilamadi %s", fileAtxt);
		return 1;
	}
	btxt = fopen(fileBtxt, "w+");
	if (atxt == NULL) {
		printf("Dosya acilamadi %s", fileBtxt);
		return 1;
	}
	wordList = fopen(fileWordList, "r");
	if (atxt == NULL) {
		printf("Dosya acilamadi %s", fileWordList);
		return 1;
	}

	//Degiskenleri tanimla
	int nonSpacedLoc, spacedLoc, wordListLoc, found = 0;

	// Okudugunu diziye alma ve alternatif olaraktan yazdirma (a.txt)
	while (fgets(nonSpaced, MAXCHAR, atxt) != NULL) {
		fscanf(atxt, "%c", nonSpaced);

		//Yazdirma
		//printf("%s", nonSpaced);
	}

	//a.txt'den okunanlari kucuk harfe cevirme
	for (int i = 0; i < strlen(nonSpaced); i++) {
		nonSpaced[i] = tolower(nonSpaced[i]);
	}

	// WordListArray olusturma (wordlist.txt)
	for (wordListLoc = 0; wordListLoc < WORDLIST_LENGTH; wordListLoc++) {
		wordListArray[wordListLoc] = fgetc(wordList);
	}

	/* Kelimeleri satir satir dizen kod --- Memory allocation ve C'de matris kullanim zorlugundan dolayi vazgectim
	 int wordNo, wordEnds = 0;
	 // WordListArray olusturma (wordlist.txt)
	 for (wordListLoc = 0; wordListLoc < WORDLIST_LENGTH; wordListLoc++) {
	 if (fgetc(wordList) == '\n') {
	 wordNo++;
	 wordEnds = 0;
	 printf("\n");
	 } else {
	 wordListArray[wordNo][wordEnds] = fgetc(wordList);
	 wordEnds++;
	 }

	 printf("%c",wordListArray[wordNo][wordEnds]);
	 }
	 */

	//nonSpaced ve spaced location baslatma
	spacedLoc = 0;
	nonSpacedLoc = 0;

	//isWord ve wordTemp dizileri ile locator'larini olusturma
	char isWord[MAXWORD_LENGTH], wordTemp[MAXWORD_LENGTH],
			number[MAXWORD_LENGTH] = { };
	int isWordLoc, wordTempLoc = 1;
	int numberLoc = 0;

	//isWord'u ve wordTemp'i wordListArray'de aranabilcek formata getirme
	isWord[0] = '\n';
	wordTemp[0] = '\n';

	//Ilk harf atamasi
	wordTemp[1] = 'q';
	wordTemp[wordTempLoc + 1] = '\n'; //wordListArray'de bulunmayan formata getirme

	//nonSpaced dizisini spaced dizisine donusturme
	while (nonSpaced[nonSpacedLoc] != NULL) {

		//nonSpaced'tee kelime koku bulup wordTemp'e atma
		while ((strstr(wordListArray, wordTemp) == '\0') && (found == 0)
				&& (wordTempLoc <= MAXWORD_LENGTH)) {
			wordTemp[wordTempLoc] = nonSpaced[nonSpacedLoc];
			wordTemp[wordTempLoc + 1] = '\n'; //wordListArray'de aranabilcek formata getirmek icin gerekli

			//Noktalama isaretleriyle karsilasirsa
			if (((wordTemp[wordTempLoc] >= 33) && (wordTemp[wordTempLoc] <= 47))
					|| (wordTemp[wordTempLoc] == 63 /*Soru isareti*/)) {
				found = 1;
			}

			wordTempLoc++;
			nonSpacedLoc++;
		}
		//found'i sifirla
		found = 0;

		//Bulunan koku isWord'e atip wordTemp'i bosaltma
		strcpy(isWord, wordTemp);
		isWordLoc = strlen(isWord);

		//wordTemp'i bosaltma (flush)
		memset(wordTemp, '\0', MAXWORD_LENGTH);
		wordTemp[0] = '\n';
		wordTempLoc = 1;

		//Ilk harf atamasi
		wordTemp[1] = 'q';
		wordTemp[wordTempLoc + 1] = '\n'; //wordListArray'de bulunmayan formata getirme
		isWordLoc--;

		//isWord'un "ilk" karakterinin NULL oldugu durumdaki bug icin
		if (isWord[1] == '\0') {
			isWordLoc++;
		}

		//isWord'te ve wordTemp'te birer kelime bulunana kadar nonSpaced'te ilerleme
		while ((strstr(wordListArray, wordTemp) == '\0')
				&& (strlen(isWord) < MAXWORD_LENGTH) && (found == 0)) {
			//Sayilari direkt bastir, onune arkasina bosluk koy
			while ((nonSpaced[nonSpacedLoc] >= 48)
					&& (nonSpaced[nonSpacedLoc] <= 57)) {
				number[numberLoc] = nonSpaced[nonSpacedLoc];
				nonSpacedLoc++;
				numberLoc++;
			}

			//wordTemp'ten isWord'e tasima
			wordTemp[wordTempLoc] = nonSpaced[nonSpacedLoc];
			isWord[isWordLoc] = wordTemp[wordTempLoc];

			//Aranabilir hale getirme
			wordTemp[wordTempLoc + 1] = '\n';
			isWord[isWordLoc + 1] = '\n';

			//Noktalama isaretleriyle karsilasirsa
			if (((wordTemp[wordTempLoc] >= 33) && (wordTemp[wordTempLoc] <= 47))
					|| (wordTemp[wordTempLoc] == 63 /*Soru isareti*/)) {
				found = 1;
			}

			if (strstr(wordListArray, isWord) != '\0' || found == 1) { //isWord'deki anlamliysa wordTemp'i bosalt
			//wordTemp flush ve fill
				memset(wordTemp, '\0', MAXWORD_LENGTH);
				wordTemp[0] = '\n';
				wordTemp[1] = 'q';
				wordTempLoc = 1;
			} else {
				//isWord'teki anlamsizsa wordTemp'te ilerle
				wordTempLoc++;
			}

			//Birer adim ilerle
			nonSpacedLoc++;
			isWordLoc++;
		}
		//found'i sifirla
		found = 0;

		//Bu noktada kesinlikle bir kelime bulmus olduk bunu spaced'e tasiyalim
		//Tasima kolaylassin diye isWord'un formatini duzeltelim
		isWord[0] = '\0'; //Bastaki newline'i sil
		isWord[isWordLoc - wordTempLoc + 1] = ' '; //Sona bosluk

		//Kopyala v1 - Dongu ile kopyayi isleri kolaylastirdigindan ve kontrol edilebileceginden tercih ettim
		//strncat(spaced, isWord, isWordLoc - wordTempLoc + 2);

		//Kopyala v2
		for (int i = 1; i <= isWordLoc - wordTempLoc + 1; i++) {
			//Cumlenin ilk harfini buyut
			if (spaced[spacedLoc - 2] == '.' || spaced[spacedLoc - 2] == '!'
					|| spaced[spacedLoc - 2] == '?' || (spacedLoc - 1 < 0)){
				isWord[i] = toupper(isWord[i]);
			}
				spaced[spacedLoc] = isWord[i];
			spacedLoc++;
		}

		if (numberLoc != 0) {
			for (int i = 0; i < numberLoc; i++) {
				spaced[spacedLoc] = number[i];
				spacedLoc++;
			}
			spaced[spacedLoc] = ' ';
			spacedLoc++;
			numberLoc = 0;
		}

		//isWord flush
		memset(isWord, '\0', MAXWORD_LENGTH);
	}

	//Ciktinin son kontrolu
	if (strstr(spaced, "q") != '\0') {
		strcat(spaced, "\n\nBIR SEYLER TERS GITTI! :(");
	} else {
		strcat(spaced, "\n\nCIKTI BASARILI! :)");
	}

	spacedLoc = 0;
	//spaced dizisini b.txt belgesine aktarma
	while (spaced[spacedLoc] != '\0') {
		fprintf(btxt, "%c", spaced[spacedLoc]);

		spacedLoc++;
	}

	/*//WordList'i yazdirma (acma :D 63k kelime var)
	 while (fgets(wordListArray, MAXCHAR_WORDLIST, wordList) != NULL){
	 //Yazdirma
	 printf("%s", wordListArray);
	 }*/

	fclose(atxt);
	fclose(btxt);
	fclose(wordList);
	return 0;
}
