#include <iostream>
#include <fstream>
#include <map>

#include "decodage.h"

//la fonction bips (respectivement espaces) évalue la longueur du bip (resp du silence)
//à partir du nombre de valueurs non nulles (resp nulles) relevées dans le fichier .wav
//dans un intervalle uniforme donné et renvoie le caractère en morse correspondant


//dans les conditions if des fonctions nous évaluons si la longueur du segment est contenue dans un intervalle
//centré en la longueur attendue (des bips ou silences) pour être sûrs de vérifier la condition
//(un simple condition avec "==" aurait été limitante dans le sens où on peut facilement avoir une valeur en plus ou en moins)

//la valeur 44100 correspond à WAVFILE_SAMPLES_PER_SECOND déclaré dans codage.h

const char* bips(int& non_null){
    if ( non_null > 0.1*44100 && non_null < 0.3*44100 ){
        return ".";
    }
    else if ( non_null > 0.5*44100 && non_null < 0.7*44100 ){
        return "-";
    }
    else {
        return "erreur";
    }
}

const char* espaces(int& null){
    if ( null > 0.1*44100 && null < 0.3*44100 ){
        return "espace_court";
    }
    else if ( null > 0.5*44100 && null < 0.7*44100 ){
        return "espace_normal";
    }
    else if ( null > 1.3*44100 && null < 1.5*44100 ){
        return "espace_long";
    }
    else {
        return "erreur";
    }
}

//la fonction octets_par_bloc permet de lire l'information correspondant
//au nombre d'octets par bloc dans l'entête du fichier .wav
//afin de savoir sur combien d'octets est codée chaque valeur
//ce sera nécessaire lors de la lecture du fichier

int octets_par_bloc(const char* adresse){
    typedef struct  WAV_HEADER {
        /* RIFF Chunk Descriptor */
        uint8_t         RIFF[4];        // RIFF Header Magic header
        uint32_t        ChunkSize;      // RIFF Chunk Size
        uint8_t         WAVE[4];        // WAVE Header
        /* "fmt" sub-chunk */
        uint8_t         fmt[4];         // FMT header
        uint32_t        Subchunk1Size;  // Size of the fmt chunk
        uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
        uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
        uint32_t        bytesPerSec;    // bytes per second
        uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
        uint16_t        bitsPerSample;  // Number of bits per sample
        /* "data" sub-chunk */
        uint8_t         Subchunk2ID[4]; // "data"  string
        uint32_t        Subchunk2Size;  // Sampled data length
    } wav_hdr;
    wav_hdr wavHeader;
    int headerSize = sizeof(wav_hdr), filelength = 0;
    FILE* wavFile = fopen(adresse, "r");
    size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
    return wavHeader.blockAlign;
}

//la fonction wav_to_morse parcourt le fichier .wav et utilise les fonctions précédentes
//pour traduire les alternances entre bruit et silence en morse
//elle renvoie un map avec toutes les valeurs du message en morse dans l'ordre

//détail : il faut deux valeurs a et b dans la boucle car comme on relève un signal sinusoïdal
//il peut y avoir des 0 au milieu du "bruit" et on ne veut pas séparer le bruit en paquets périodiques compris entre les 0

std::map<int, const char*> wav_to_morse(const char* adresse){

    int oct = octets_par_bloc(adresse);

    std::ifstream f(adresse, std::ifstream::binary);

    f.seekg(0, std::ios_base::end);
    long int taille_fichier = f.tellg();
    f.seekg(0, std::ios_base::beg);
    char c[44];
    f.read((char*)c, 44);
    

    std::map<int, const char*> code_morse;
    int a;
    f.read((char*)&a, oct);
    int b = a;
    int null = 0;
    int non_null = 0;
    int k = 0;
    for ( int i=oct; i < taille_fichier-44; i+=oct ){
        f.read((char*)&a, 2);
        if ( a == 0 && a == b ){
            null += 1;
            if ( non_null!=0 ){
                code_morse[k] = bips(non_null);
                k += 1;
                non_null = 0;
            }
        }
        if ( a!=0 && b!=0 ){
            non_null += 1;
            if ( null!=0 ){
                code_morse[k] = espaces(null);
                k += 1;
                null = 0;
            }
        }
        b = a;
    }

    return code_morse;
}

//la fonction map permet simplement de traduire du morse vers les lettres
//la valeur 1 correspond à un bip long, la valeur 0 correspond à un bip court
//Nous rajoutons le chiffre 1 devant chaque code morse pour faciliter le décodage dans la fonction decodage :
//il est difficile d'obtenir des entiers avec des 0 comme premier chiffre (comme 0010 par exemple)

const char* map(int bip){
    std::map<int, const char*> alphabet;
    alphabet[101] = "A";
    alphabet[101101] = "À";
    alphabet[11000] = "B";
    alphabet[11010] = "C";
    alphabet[110100] = "Ç";
    alphabet[1100] = "D";
    alphabet[10] = "E";
    alphabet[101001] = "È";
    alphabet[100100] = "É";
    alphabet[10010] = "F";
    alphabet[1110] = "G";
    alphabet[10000] = "H";
    alphabet[100] = "I";
    alphabet[10111] = "J";
    alphabet[1101] = "K";
    alphabet[10100] = "L";
    alphabet[111] = "M";
    alphabet[110] = "N";
    alphabet[1111] = "O";
    alphabet[10110] = "P";
    alphabet[11101] = "Q";
    alphabet[1010] = "R";
    alphabet[1000] = "S";
    alphabet[11] = "T";
    alphabet[1001] = "U";
    alphabet[10001] = "V";
    alphabet[1011] = "W";
    alphabet[11001] = "X";
    alphabet[11011] = "Y";
    alphabet[11100] = "Z";
    alphabet[101111] = "1";
    alphabet[100111] = "2";
    alphabet[100011] = "3";
    alphabet[100001] = "4";
    alphabet[100000] = "5";
    alphabet[110000] = "6";
    alphabet[111000] = "7";
    alphabet[111100] = "8";
    alphabet[111110] = "9";
    alphabet[111111] = "0";
    alphabet[1010101] = ".";
    alphabet[1110011] = ",";
    alphabet[1001100] = "?";
    alphabet[1101011] = "!";
    alphabet[1111000] = ":";
    alphabet[1101010] = ";";
    alphabet[110010] = "/";
    alphabet[1011110] = "'";
    alphabet[101010] = "+";
    alphabet[1100001] = "-";
    alphabet[110001] = "=";
    alphabet[110110] = "(";
    alphabet[1101101] = ")";
    alphabet[1001101] = "_";
    alphabet[10001001] = "$";
    alphabet[1011010] = "@";
    alphabet[101000] = "&";
    return alphabet[bip];
}

//la fonction decodage prend le nom du fichier .wav à décoder et utilise le map obtenu avec wav_to_morse
//pour traduire du morse vers le français
//le message obtenu est enregistré au fur et à mesure dans un fichier .txt créé

int decodage(){

    const char* fichier;
    std::string input;
    std::cout << "Écrivez ici le nom du fichier .wav que vous souhaitez traduire : " << std::endl;
    getline(std::cin, input);
    fichier = const_cast<char*>(input.c_str());

    std::map<int, const char*> code_morse;
    code_morse = wav_to_morse(fichier);
    
    std::ofstream file_out;
    file_out.open ("message_traduit.txt");

    int k = code_morse.size();
    int morse = 1;
    int iter = 0;

    //nous itérons sur les bips et enregistrons le code morse dans la variable morse pour chaque lettre
    //à chaque silence la lettre est enregistrée
    //et s'il s'agit d'un silence_long nous enregistrons de même un espace dans le fichier
    for ( int i=0; i<k; i++ ){
        if ( code_morse[i][0] == '-' ){
            morse = 10*morse + 1;
        }
        else if ( code_morse[i][0] == '.' ){
            morse = 10*morse;
        }
        else if ( code_morse[i][7] == 'n' ){
            file_out << map(morse);
            iter += 1;
            morse = 1;
        }
        else if ( code_morse[i][7] == 'l' ){
            file_out << map(morse);
            file_out << " ";
            iter += 1;
            morse = 1;
        }
    }
    //au cas où il n'y aurait pas de silence à la fin du message
    //la dernière lettre est enregistrée dans morse mais n'est pas affichée par la boucle for
    //on crée donc une condition pour l'ajouter
    if ( morse!=1 ){
        file_out << map(morse);
    }
    file_out << "\n" << std::endl;
    file_out.close();

    return 0;
}