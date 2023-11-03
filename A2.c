#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(char argc, char *argv[]){
    if(argc != 4){
        fprintf(stderr, "Usage: %s <input_file> <output_file> <option>\n", argv[0]);
        return 1;
    }

    int option = atoi(argv[3]);
    if(option < 1 || option > 3){
        fprintf(stderr, "Invalid option: Choose 1, 2, or 3.\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if(inputFile == NULL){
        fprintf(stderr, "Error: Could not open input file\n");
        return 1;
    }

    char *ANum = "AXXXXXX_AXXXXXX_AXXXXXX"; // Add numbers before submitting
    FILE *outputFile = fopen(ANum, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Failed to create the output file.\n");
        fclose(inputFile);
        return 1;
    }

    FILE *tempFile = fopen(argv[2], "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Failed to create the temp file.\n");
        fclose(inputFile);
        return 1;
    }

        


}
