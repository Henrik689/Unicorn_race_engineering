#include <stdio.h>
#include <stdlib.h>
#include <canparser.h>

/*
void bench(const char *path, int iterations){
	int i;
	FILE *fp = fopen(path, "rb"); // open first argument as read binary

	if( fp == NULL ){
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}
	
	fseek(fp, 0L, SEEK_END); // Seek to the end
	size_t fSize = ftell(fp); // get the file size
	fseek(fp, 0L, SEEK_SET); // Reset the seeker to the beginning of the file

	char *buff = (char*)calloc(fSize, sizeof(char));
	for (i = 0; i < fSize; ++i){
		int rc = getc(fp);
		if(rc == EOF){
			printf("Error %d: EOF\n", EOF);
			exit(EXIT_FAILURE);
		}
		buff[i] = rc;
	}

	for (i = 0; i < iterations; ++i){
		int k = 0;
		config_t cfg[] = INIT_CONFIG;
		parser_t p = INIT_PARSER(cfg);
		for (k = 0; k < fSize; ++k){
			sensor_t s;
			int rv = parseNext((uint8_t)buff[k], &s, &p);
			if(p.sensorFound){
				printf("%d,\"%s\",%.2f\n", s.id, s.name, s.value);
			}else if( rv < 0){
				printf("Invalid ID found: %d in \"%s\" at offset %d\n", -rv, path, k);
			}
		}
	}
}
*/

int main(int argc, char const *argv[]){
	int i;
	
	if(argc < 2){
		printf("Usage: %s path/file\n", argv[0]);
		return EXIT_FAILURE;
	}

	// loop over each argument
	for (i = 1; i < argc; ++i){
		canfile2csv(argv[i]);
	}

	//bench(argv[1], 1000);
	
	return EXIT_SUCCESS;
}
