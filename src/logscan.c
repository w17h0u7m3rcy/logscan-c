#include <stdio.h> // printf, fsprintf, fopen, fclose, fgets, perror
#include <string.h> // strstr
#include <ctype.h> // tolower
#include <errno.h>

static int find_substring_ci_index(const char *haystack, const char *needle) {
    if (!*needle) return 0; // empty pattern matches at start

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
            return (int)i; // return column
        }
    }

    return -1;
}


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


#define MAX_LINE 1024

int main(int argc, char *argv[]){

    if (argc != 3 && argc > 5){
        fprintf(stderr, "Usage %s <file> <pattern> [-i] [--col]\n", argv[0]);
        return 1;
    }

    int ignore_case = 0;
    int show_column = 0;

    for (int i = 3; i < argc; i++){
        if (strcmp(argv[i], "-i") == 0){
            ignore_case = 1;
        }
        else if (strcmp(argv[i], "--col") == 0){
            show_column = 1;
        } else {
            fprintf(stderr, "Unknown opotions: %s\n", argv[i]);
            fprintf(stderr, "Usage: %s <file> <pattern> [-i] [--col]\n", argv[0]);
        }
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

        int idx = -1;

        if (!ignore_case) {
            const char *p = strstr(line, pattern);
            if (p != NULL) {
                idx = (int)(p - line);
            }
        } else {
            idx = find_substring_ci_index(line, pattern);
        }

        if (idx >= 0) {
            printf("[%d] %s", line_no, line);

            if (show_column) {
                char prefix[64];
                int prefix_len = snprintf(prefix, sizeof(prefix), "[%d] ", line_no);
                if (prefix_len < 0) prefix_len = 0;

                // 1) espacios del prefijo
                for (int k = 0; k < prefix_len; k++) putchar(' ');

                // 2) espacios hasta la columna del match
                for (int k = 0; k < idx; k++) putchar(' ');

                // 3) carets del largo del patrón
                for (size_t k = 0; pattern[k] != '\0'; k++) putchar('^');

                putchar('\n');
            }

            matches++;
        }

    }

    fclose(f);

    printf("Total matches: %d\n", matches);
    return 0;

}
