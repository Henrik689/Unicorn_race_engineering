#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./canparser.h"


char* stradd(const char* a, const char* b){
	size_t len = strlen(a) + strlen(b);
	char *ret = (char*)malloc(len * sizeof(char) + 1);
	*ret = '\0';
	return strcat(strcat(ret, a) ,b);
}


int main(int argc, char const *argv[]){
	int i;
	int k;
	
	if(argc < 2){
		printf("Usage: %s path/file\n", argv[0]);
		return EXIT_FAILURE;
	}

	// loop over each argument
	for (i = 1; i < argc; ++i){
		FILE *fp = fopen(argv[i], "rb"); // open first argument as read binary

		if( fp == NULL ){
			perror("Failed to open file");
			return EXIT_FAILURE;
		}
		
		fseek(fp, 0L, SEEK_END); // Seek to the end
		size_t fSize = ftell(fp); // get the file size
		fseek(fp, 0L, SEEK_SET); // Reset the seeker to the beginning of the file

		// open the output file
		char *outname = stradd(argv[i], ".csv"); // remember to free
		FILE *outfp = fopen(outname, "w"); // open outputfile as the input.csv
		if(outfp == NULL){
			perror("Failed to open outputfile");
			return EXIT_FAILURE;
		}

		config_t cfg[] = INIT_CONFIG;
		parser_t p = INIT_PARSER(cfg);
		fprintf(outfp, "id,name,value\n");
		for (k = 0; k < fSize; ++k){
			int rc = getc(fp);
			if(rc == EOF){
				printf("An error occurred while reading the file \"%s\" (Unexpected EOF).\n", argv[i]);
				return EXIT_FAILURE;
			}

			sensor_t s;
			int rv = parseNext((uint8_t)rc, &s, &p);
			if(p.sensorFound){
				fprintf(outfp, "%d,\"%s\",%.2f\n", s.id, s.name, s.value);
			}else if( rv < 0){
				printf("Invalid ID found: %d in \"%s\" at offset %d\n", -rv, argv[i], k);
			}
		}
		fclose(fp);
		fclose(outfp); free(outname);
	}
	
	return EXIT_SUCCESS;
}
