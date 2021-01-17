#include <iostream>
#include <map>
#include <string>
#include <fstream>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <map>

#include "codage.h"

//les trois premières fonctions wavfile_open, wavfile_write et wavfile_close
//permettent respectivement de créer un fichier .wav et écrire son entête, écrire les données dans celui-ci
//et compléter le fichier lorque toutes les valeurs sont saisies en renseignant la longueur des données dans l'entête

struct wavfile_header {
	char	riff_tag[4];
	int	riff_length;
	char	wave_tag[4];
	char	fmt_tag[4];
	int	fmt_length;
	short	audio_format;
	short	num_channels;
	int	sample_rate;
	int	byte_rate;
	short	block_align;
	short	bits_per_sample;
	char	data_tag[4];
	int	data_length;
};

FILE * wavfile_open( const char *filename )
{
	struct wavfile_header header;

	int samples_per_second = WAVFILE_SAMPLES_PER_SECOND;
	int bits_per_sample = 16;

	strncpy(header.riff_tag,"RIFF",4);
	strncpy(header.wave_tag,"WAVE",4);
	strncpy(header.fmt_tag,"fmt ",4);
	strncpy(header.data_tag,"data",4);

	header.riff_length = 0;
	header.fmt_length = 16;
	header.audio_format = 1;
	header.num_channels = 1;
	header.sample_rate = samples_per_second;
	header.byte_rate = samples_per_second*(bits_per_sample/8);
	header.block_align = bits_per_sample/8;
	header.bits_per_sample = bits_per_sample;
	header.data_length = 0;

	FILE * file = fopen(filename,"wb+");
	if(!file) return 0;

	fwrite(&header,sizeof(header),1,file);

	fflush(file);

	return file;

}

void wavfile_write( FILE *file, short data[], int length )
{
	fwrite(data,sizeof(short),length,file);
}

void wavfile_close( FILE *file )
{
	int file_length = ftell(file);

	int data_length = file_length - sizeof(struct wavfile_header);
	fseek(file,sizeof(struct wavfile_header) - sizeof(int),SEEK_SET);
	fwrite(&data_length,sizeof(data_length),1,file);

	int riff_length = file_length - 8;
	fseek(file,4,SEEK_SET);
	fwrite(&riff_length,sizeof(riff_length),1,file);

	fclose(file);
}

//en utilisant les trois premières fonctions, les fonctions point, tiret, espace_court, espace_normal, espace_long
//permettent d'écrire dans un fichier .wav des silences ou des bips d'une longueur donnée
//il s'agit d'une sinusoide pour les bips et de zéors pour les silences

int point(FILE *file){
    const int NUM_SAMPLES = (WAVFILE_SAMPLES_PER_SECOND*0.2);
	short waveform[NUM_SAMPLES];
	double frequency = 440.0;
	int volume = 32000;
	int length = NUM_SAMPLES;

	int i;
	for(i=0;i<length;i++) {
		double t = (double) i / WAVFILE_SAMPLES_PER_SECOND;
		waveform[i] = volume*sin(frequency*t*2*M_PI);
	}

	wavfile_write(file, waveform, length);

	return 0;
}

int tiret(FILE *file){
    const int NUM_SAMPLES = (WAVFILE_SAMPLES_PER_SECOND*0.6);
	short waveform[NUM_SAMPLES];
	double frequency = 440.0;
	int volume = 32000;
	int length = NUM_SAMPLES;

	int i;
	for(i=0;i<length;i++) {
		double t = (double) i / WAVFILE_SAMPLES_PER_SECOND;
		waveform[i] = volume*sin(frequency*t*2*M_PI);
	}

	wavfile_write(file, waveform, length);

	return 0;
}

int espace_court(FILE *file){
    const int NUM_SAMPLES = (WAVFILE_SAMPLES_PER_SECOND*0.2);
	short waveform[NUM_SAMPLES];
	double frequency = 440.0;
	int volume = 32000;
	int length = NUM_SAMPLES;

	int i;
	for(i=0;i<length;i++) {
		waveform[i] = 0;
	}

	wavfile_write(file, waveform, length);

	return 0;
}

int espace_normal(FILE *file){
    const int NUM_SAMPLES = (WAVFILE_SAMPLES_PER_SECOND*0.6);
	short waveform[NUM_SAMPLES];
	double frequency = 440.0;
	int volume = 32000;
	int length = NUM_SAMPLES;

	int i;
	for(i=0;i<length;i++) {
		waveform[i] = 0;
	}

	wavfile_write(file, waveform, length);

	return 0;
}

int espace_long(FILE *file){
    const int NUM_SAMPLES = (WAVFILE_SAMPLES_PER_SECOND*1.4);
	short waveform[NUM_SAMPLES];
	double frequency = 440.0;
	int volume = 32000;
	int length = NUM_SAMPLES;

	int i;
	for(i=0;i<length;i++) {
		waveform[i] = 0;
	}

	wavfile_write(file, waveform, length);

	return 0;
}

//la fonction encodage demande à l'utilisateur dans un premier temps de saisir
//le texte qu'il souhaite traduire en morse en l'écrivant ou en renseignant le nom d'un fichier .txt contenant le texte
//ensuite un fichier .wav est automatiquement créé et les données sont renseignées grâce aux fonctions précédentes
//le map morse permet de traduire le texte en morse

int codage(){

    std::map<char, const char*> morse;
    morse['A'] = ".-";
    morse['a'] = ".-";
    //morse['À'] = ".--.-";
    //morse['à'] = ".--.-";
    morse['B'] = "-...";
    morse['b'] = "-...";
    morse['C'] = "-.-.";
    morse['c'] = "-.-.";
    //morse['Ç'] = "-.-..";
    //morse['ç'] = "-.-..";
    morse['D'] = "-..";
    morse['d'] = "-..";
    morse['E'] = ".";
    morse['e'] = ".";
    //morse['È'] = ".-..-";
    //morse['è'] = ".-..-";
    //morse['É'] = "..-..";
    //morse['é'] = "..-..";
    morse['F'] = "..-.";
    morse['f'] = "..-.";
    morse['G'] = "--.";
    morse['g'] = "--.";
    morse['H'] = "....";
    morse['h'] = "....";
    morse['I'] = "..";
    morse['i'] = "..";
    morse['J'] = ".---";
    morse['j'] = ".---";
    morse['K'] = "-.-";
    morse['k'] = "-.-";
    morse['L'] = ".-..";
    morse['l'] = ".-..";
    morse['M'] = "--";
    morse['m'] = "--";
    morse['N'] = "-.";
    morse['n'] = "-.";
    morse['O'] = "---";
    morse['o'] = "---";
    morse['P'] = ".--.";
    morse['p'] = ".--.";
    morse['Q'] = "--.-";
    morse['q'] = "--.-";
    morse['R'] = ".-.";
    morse['r'] = ".-.";
    morse['S'] = "...";
    morse['s'] = "...";
    morse['T'] = "-";
    morse['t'] = "-";
    morse['U'] = "..-";
    morse['u'] = "..-";
    morse['V'] = "...-";
    morse['v'] = "...-";
    morse['W'] = ".--";
    morse['w'] = ".--";
    morse['X'] = "-..-";
    morse['x'] = "-..-";
    morse['Y'] = "-.--";
    morse['y'] = "-.--";
    morse['Z'] = "--..";
    morse['z'] = "--..";
    morse['1'] = ".----";
    morse['2'] = "..---";
    morse['3'] = "...--";
    morse['4'] = "....-";
    morse['5'] = ".....";
    morse['6'] = "-....";
    morse['7'] = "--...";
    morse['8'] = "---..";
    morse['9'] = "----.";
    morse['0'] = "-----";
    morse['.'] = ".-.-.-";
    morse[','] = "--..--";
    morse['?'] = "..--..";
    morse['!'] = "-.-.--";
    morse[':'] = "---...";
    morse[';'] = "-.-.-.";
    morse['/'] = "-..-.";
    morse["'"[0]] = ".----.";
    morse['+'] = ".-.-.";
    morse['-'] = "-....-";
    morse['='] = "-...-";
    morse['('] = "-.--.";
    morse[')'] = "-.--.-";
    morse['_'] = "..--.-";
    morse['$'] = "...-..-";
    morse['@'] = ".--.-.";
    morse['&'] = ".-...";

    std::cout << "Si vous voulez écrire votre message tapez 1 \
    si vous voulez traduire un message écrit dans un fichier .txt tapez 2" << std::endl;
    std::string info;
    std::getline (std::cin, info);

    const char* message;
    const char* fichier;
    std::string input;
    if ( info == "1" ){
        std::cout << "Écrivez ici votre message : " << std::endl;
        getline(std::cin, input);
    }
    else if ( info == "2" ){
        std::cout << "Écrivez ici le nom du fichier : " << std::endl;
        std::cin >> input;
        std::cin.get();
        fichier = input.c_str();

        std::ifstream file_in;
        file_in.open(fichier);
        std::string texte1;
        std::getline(file_in, texte1);
        std::string texte2 = texte1;
        while ( file_in.eof()==0 ){
            std::getline(file_in, texte1);
            texte2 = texte2 + " " + texte1;
        }
        input = texte2;
    }
    else{
        printf("La commande saisie n'est pas valide");
    }
    message = const_cast<char*>(input.c_str());

    FILE* f = wavfile_open("message_en_morse.wav");
    
    int j = 0;
    while ( message[j] != '\0' ){
        if ( message[j] == ' ' ) {
            espace_long(f);
            j++;
        }
        else {
            const char* code = morse[message[j]];
            int k = 0;
            while ( code[k] != '\0' ){
                if ( code[k] == '.' ){
                    point(f);
                }
                else if ( code[k] == '-' ){
                    tiret(f);
                }
                k++;
                if ( code[k] != '\0' ) {espace_court(f);}
            }
            j++;
            if ( message[j] != '\0' && message[j] != ' ' ) {espace_normal(f);}
        }
    }
	espace_normal(f);

    wavfile_close(f);

    return 0;
}