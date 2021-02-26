#include <stdio.h>
#include <string.h>
#define MAX 50


struct person {
    char *name;
    int code;
    int number;
};

int createBinFile(char *fname) {
	FILE *fp;
	struct person newrecord;

	// Open for binary writing
	fp = fopen(fname,"w");
	if (!fp) {
		printf("Unable to open file!");
		return -1;
	}

	// Just write three records, so we have
	// something to play with. Normally you would
	// do this with a loop and/or user input!

	newrecord.name="aaa bbbb";
	newrecord.code=12345;
	newrecord.number=1;
	fwrite(&newrecord, sizeof(struct person), 1, fp);
	
	newrecord.name="cc ddd";
	newrecord.code=1234578;
	newrecord.number=2;
	fwrite(&newrecord, sizeof(struct person), 1, fp);

	newrecord.name="eeee ffffff";
	newrecord.code=123456;
	newrecord.number=3;
	fwrite(&newrecord, sizeof(struct person), 1, fp);

	fclose(fp);
	return 0;
}

int readBinFile(char *fname) {
	FILE *fp;
	struct person myrecord;

	fp=fopen(fname,"r");
	if (!fp) {
		printf("Unable to open file!");
		return -1;
	}

	printf("The following records are in the binary file %s:\n", fname);
	while (fread(&myrecord,sizeof(struct person),1,fp) != NULL) {
		printf("%s\n", myrecord.name);
		printf("%d\n", myrecord.code);
		printf("%d\n\n", myrecord.number);
	}
	fclose(fp);
	return 0;
}

void delete_record(char* filename, char* record_name){
	FILE* fptr;
	FILE* fptr2;
	fptr = fopen(filename, "r");
	fptr2 = fopen("temp.txt", "w+");
	struct person myrecord;
	while(fread(&myrecord, sizeof(struct person), 1, fptr) != NULL){
		if(strcmp(record_name, myrecord.name) == 0){
			printf("Record found and deleted.\n");
		}
		else{
			fwrite(&myrecord, sizeof(struct person), 1, fptr2);
		}
	}
	fclose(fptr);
	fclose(fptr2);
	remove(filename);
	rename("temp.txt", filename);
}

void main(int argc, char *argv[]){
	char* filename = argv[1];
	createBinFile(filename);
	readBinFile(filename);
	char* record_name = "kkk cccc";
	delete_record(filename, record_name);
	readBinFile(filename);
}