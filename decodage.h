#include <iostream>
#include <map>

const char* bips(int& non_null);
const char* espaces(int& null);
std::map<int, const char*> wav_to_morse(const char* adresse);
int octets_par_bloc(const char* adresse);
const char* map(int bip);
int decodage();