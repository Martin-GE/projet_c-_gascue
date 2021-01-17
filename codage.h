#ifndef WAVFILE_H
#define WAVFILE_H

#include <stdio.h>
#include <inttypes.h>

FILE * wavfile_open( const char *filename );
void wavfile_write( FILE *file, short data[], int length );
void wavfile_close( FILE * file );
int point(FILE *file);
int tiret(FILE *file);
int espace_court(FILE *file);
int espace_normal(FILE *file);
int espace_long(FILE *file);
int codage();

#define WAVFILE_SAMPLES_PER_SECOND 44100

#endif