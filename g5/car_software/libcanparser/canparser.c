#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "./canparser.h"

#define ARR_LEN(x)  (sizeof(x) / sizeof(x[0]))
#define MIN(x, y)	((x < y) ? x : y)
#define MAX(x, y)	((x > y) ? x : y)

static double roundn(double value, int to){
	//double places = pow(10.0, to);
	//return round(value * places) / places;
	return value;
}

// As C does not support any form of lambda functions
// each convertion function must be declared here and
// then added to the list(array) of sensor configs 

double stdConv(int x, int rounddec){
	return roundn((x*1+0), rounddec);
}

double StatusLambdaV2Conv(int x, int rounddec){
	double val;
	if(x > 32768){
		x = -(65535-x);
	}
	val = 70-x/64;
	return roundn(val, rounddec);
}

double airAndWaterTempConv(int x, int rounddec){
	double val = (x * (-150.0/3840) + 120);
	return roundn(val, rounddec);
}

double potmeterConvert(int x, int rounddec){
	double val = ((x-336)/26.9);
	return roundn(val, rounddec);
}

double rpmConv(int x, int rounddec){
	double val = (x*0.9408);
	return roundn(val, rounddec);
}

double mBarConv(int x, int rounddec){
	double val = (x*0.75);
	return roundn(val, rounddec);
}

double batteryConv(int x, int rounddec){
	double val = (x*(1/210)+0);
	return roundn(val, rounddec);
}

double StatusLambdaV2Conv2(int x, int rounddec){
	double val;
	if(x > 32768){
		x = -(65535-x);
	}
	val = 70-x/64;
	return roundn(val/100, rounddec);
}

double InjectorAndIgnitionTimeConv(int x, int rounddec){
	double val = -0.75*x+120;
	return roundn(val, rounddec);
}

double GXGYGZconv(int x, int rounddec){
	double val;
	if(x > 32768){
		x = -(65535 - x);
	}
	val = x * (1/16384);
	return roundn(val, rounddec);
}

double gearboardTempConv(int x, int rounddec){
	double val;
	double resistance = ((10240000/(1024 - x)) - 10000);
	double temp = log(resistance);
	temp = 1 / (0.001129148 + (0.000234125 * temp) + (0.0000000876741 * temp * temp * temp));
	val = temp - 273.15;
	return roundn(val, rounddec);
}

double waterInOutletTemoConv(int x, int rounddec){
	double val = 127.5 * exp(-0.003286*x);
	return roundn(val, rounddec);
}

double gearNeutralConv(int x, int rounddec){
	double val;
	if( x > 100){
		x=1;
	}
	val = x;
	return roundn(val, rounddec);
}


static inline int getConfigFromID(int id, const config_t *config, size_t confLength){
	int i = 0;
	for (i = 0; i < confLength; ++i){
		if(id == config[i].id){
			return i; // index in the config list
		}
	}
	return -1; // No ID was found
}

int parseNext(uint8_t dataByte, sensor_t *sensor, parser_t *p){
	const uint8_t startSequence[] = {255, 123, 10};

	p->sensorFound = 0; // we dont yet know if a sensor is found

	if((p->package_start_counter == 0) && (dataByte == startSequence[0]))
		p->package_start_counter = 1;
	else if((p->package_start_counter == 1) && (dataByte == startSequence[1]))
		p->package_start_counter = 2;
	else if((p->package_start_counter == 2) && (dataByte == startSequence[2])){
		p->package_start_counter = 0;
		p->package_start = true;
		return PARSER_NEEDNEXT; // we are ready for next byte
	}

	if(p->package_start){
		// Reset
		p->package_start = false;
		p->bytesToRead = -1;
		p->valOut = 0;

		p->confIndex = getConfigFromID(dataByte, p->config, p->confLength);
		if(p->confIndex == -1){
			// Invalid id found at currByte !
			return -(int)dataByte; // return the negative value as all other return codes are unsigned
		}
		p->bytesToRead = p->config[p->confIndex].datalength/8;
		return PARSER_NEEDNEXT; // Ready for next byte
	}

	if(p->bytesToRead > 0){
		p->valOut = p->valOut + (dataByte << (8*(p->bytesToRead-1)));
		p->bytesToRead -= 1; // We have read a byte so we obviously have one less to read
		return PARSER_NEEDNEXT; // We have read and added the byte, So ready for the next
	}

	if(p->bytesToRead == 0){
		const char* name = p->config[p->confIndex].name;
		double value = p->config[p->confIndex].conv(p->valOut, p->config[p->confIndex].rounddec);
		value = MIN(value, p->config[p->confIndex].max);
		value = MAX(value, p->config[p->confIndex].min);

		// Copy the value into the sensor object
		sensor->name = name;
		sensor->id = p->config[p->confIndex].id;
		sensor->confIndex = p->confIndex;
		sensor->value = value;

		p->sensorFound = 1; // we have found a sensor

		// Reset 
		p->bytesToRead = -1;
		p->valOut = 0;

		// Are the a next data byte?
		p->confIndex = getConfigFromID(dataByte, p->config, p->confLength);
		if(p->confIndex == -1){
			// No more data
			return PARSER_NOMOREDATA;
		}

		p->bytesToRead = p->config[p->confIndex].datalength/8;
		return PARSER_NEEDNEXT;
		
	}
	return PARSER_NOTHINGTODO;
}


static inline char* stradd(const char* a, const char* b){
	size_t len = strlen(a) + strlen(b);
	char *ret = (char*)malloc(len * sizeof(char) + 1);
	*ret = '\0';
	return strcat(strcat(ret, a) ,b);
}


void canfile2csv(const char *path){
	int i;
	FILE *fp = fopen(path, "rb"); // open first argument as read binary

	if( fp == NULL ){
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}
	
	fseek(fp, 0L, SEEK_END); // Seek to the end
	size_t fSize = ftell(fp); // get the file size
	fseek(fp, 0L, SEEK_SET); // Reset the seeker to the beginning of the file

	// open the output file
	char *outname = stradd(path, ".csv"); // remember to free
	FILE *outfp = fopen(outname, "w"); // open outputfile as the input.csv
	if(outfp == NULL){
		perror("Failed to open outputfile");
		exit(EXIT_FAILURE);
	}

	config_t cfg[] = INIT_CONFIG;
	parser_t p = INIT_PARSER(cfg);
	fprintf(outfp, "id,name,value\n");
	for (i = 0; i < fSize; ++i){
		int rc = getc(fp);
		if(rc == EOF){
			printf("An error occurred while reading the file \"%s\" (Unexpected EOF).\n", path);
			exit(EXIT_FAILURE);
		}

		sensor_t s;
		int rv = parseNext((uint8_t)rc, &s, &p);
		if(p.sensorFound){
			fprintf(outfp, "%d,\"%s\",%.2f\n", s.id, s.name, s.value);
		}else if( rv < 0){
			printf("Invalid ID found: %d in \"%s\" at offset %d\n", -rv, path, i);
		}
	}
	fclose(fp);
	fclose(outfp); free(outname);
}

