#include <ctype.h> // tolower
#include <errno.h>
#include <stdio.h>  // printf, fsprintf, fopen, fclose, fgets, perror
#include <string.h> // strstr


static int find_substring_ci_index(const char *haystack, const char *needle) {
  if (!*needle)
    return 0; // empty pattern matches at start

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

static FILE *open_file_read(const char *filename) {
#if defined(_MSC_VER)
  FILE *f = NULL;
  errno_t err = fopen_s(&f, filename, "r");
  if (err != 0) {
    return NULL;
  }

  return f;

#else
  return fopen(filename, "r");
#endif
}

#define MAX_LINE 1024

int main(int argc, char *argv[]) {

  if (argc != 3 && argc > 5) {
    fprintf(stderr, "Usage %s <file> <pattern> [-i] [--col]\n", argv[0]);
    return 1;
  }

  int ignore_case = 0;
  int show_column = 0;
  int count_only = 0;

  for (int i = 3; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0) {
      ignore_case = 1;
    }

    else if (strcmp(argv[i], "--col") == 0) {
      show_column = 1;
    }

    else if (strcmp(argv[i], "--count") == 0) {
      count_only = 1;
    }

    else {
      fprintf(stderr, "Unknown options: %s\n", argv[i]);
      fprintf(stderr, "Usage: %s <file> <pattern> [-i] [--col] [--count]\n",
              argv[0]);
    }
  }

  const char *filename = argv[1];
  const char *pattern = argv[2];

  FILE *f = open_file_read(filename);

  if (f == NULL) {
    perror("Error opening file");
    return 1;
  }

  char line[MAX_LINE];

  int line_no = 0;
  int matches = 0;

  while (fgets(line, sizeof(line), f) != NULL) {

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

      matches++;

      if (!count_only) {
        printf("[%d] %s", line_no, line);

        if (show_column) {
          char prefix[64];
          int prefix_len = snprintf(prefix, sizeof(prefix), "[%d] ", line_no);

          // 1) Print spaces to align under the "[line] " prefix
          for (int k = 0; k < prefix_len; k++)
            putchar(' ');

          // 2) Print spaces up to the match start column (idx)
          for (int k = 0; k < idx; k++)
            putchar(' ');

          // 3) Print carets with the same length as the pattern
          for (size_t k = 0; pattern[k] != '\0'; k++)
            putchar('^');

          putchar('\n');
        }
      }

      matches++;
    }
  }

  fclose(f);

  if(!count_only){
    printf("%d\n", matches);
  }else{
    printf("Total matches: %d\n", matches);
  }
 
  return 0;
}
