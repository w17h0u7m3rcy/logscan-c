#include <stdio.h> // printf, fsprintf, fopen, fclose, fgets, perror
#include <string.h> // strstr
#include <ctype.h> // tolower
#include <errno.h>


static FILE *open_file_read(const char *filename){
    #if defined(_MSC_VER)
        FILE *f = NULL;
        errno_t err = fopen_s(&f, filename, "r");
        if (err != 0){
            return NULL;
        }

        return f;

    #else
        return fopen(filename, "r");
    #endif
}

static int contains_substring_ci(const char *haystack, const char *needle) {
    if (!*needle) return 1; // empty pattern

    for (size_t i = 0; haystack[i] != '\0'; i++) {
        size_t j = 0;

        while (needle[j] != '\0' && haystack[i + j] != '\0') {
            unsigned char a = (unsigned char)haystack[i + j];
            unsigned char b = (unsigned char)needle[j];

            if (tolower(a) != tolower(b)) {
                break;
            }
            j++;
        }

        if (needle[j] == '\0') {
            return 1; // pattern found
        }
    }

    return 0; // pattern not found
}


#define MAX_LINE 1024

int main(int argc, char *argv[]){

    if (argc != 3 && argc != 4){
        fprintf(stderr, "Usage %s <file> <pattern> [-i]\n", argv[0]);
        return 1;
    }

    int ignore_case = 0;

    if (argc == 4){
        if (strcmp(argv[3], "-i") !=0){
            fprintf(stderr, "Usage: %s <file> <pattern> [-i]\n", argv[0]);
            return 1;
        }

        ignore_case = 1;
    }


    const char *filename = argv[1];
    const char *pattern = argv[2];

    FILE *f = open_file_read(filename);

    if (f == NULL){
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE];

    int line_no = 0;
    int matches = 0;

    while (fgets(line, sizeof(line), f) != NULL){

        line_no++;

        int found = 0;

        if (!ignore_case){
            found = (strstr(line, pattern) != NULL);
        } else {
            found = contains_substring_ci(line, pattern);
        }

        if (found){
            printf("[%d] %s", line_no, line);
            matches++;
        }

    }

    fclose(f);

    printf("Total matches: %d\n", matches);
    return 0;

}