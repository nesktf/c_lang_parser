#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef struct LangEntry{
  char* key;
  char* value;
} LangEntry;

void parse_lang_file(FILE* stream, char sep, LangEntry** map) {
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, stream)) != -1) {
    // clean newline unix or CRLF, put null terminator
    if (read > 0 && line[read - 1] == '\n') {
      line[--read] = '\0';
    }
    if (read > 0 && line[read - 1] == '\r') {
      line[--read] = '\0';
    }
    if (read == 0) {
      continue;
    }

    char *sep_pos = strchr(line, sep);
    if (sep_pos != NULL) {
      *sep_pos = '\0'; // put null terminator in sep
      const char *key = line;
      char *value = sep_pos + 1;

      const size_t val_len = strlen(value);
      if (val_len > 0 && value[val_len - 1] == '\r') { // check for CRLF again
        value[val_len - 1] = '\0';
      }

      if (shgeti(*map, key) >= 0) {
        continue;
      }
      shput(*map, strdup(key), strdup(value));
    }
  }

  free(line);
}

void free_lang_map(LangEntry *map) {
  if (!map) {
    return;
  }
  for (ptrdiff_t i = 0; i < shlen(map); ++i) {
    free(map[i].key);
    free(map[i].value);
  }
  shfree(map);
}

int main() {
  const char* lang = "es_ES.lang";
  FILE* f = fopen(lang, "r");
  if (!f) {
    fprintf(stderr, "Can't open lang file\n");
    return 1;
  }
  int err = 0;
  LangEntry* lang_map = NULL;
  parse_lang_file(f, '=', &lang_map);
  if (!lang_map) {
    fprintf(stderr, "Can't open lang file\n");
    err = 1;
    goto cleanup;
  }

  printf("Items in the lang file:\n");
  size_t n = stbds_shlen(lang_map);
  for (size_t i = 0; i < n; ++i) {
    printf("- '%s': %s\n", lang_map[i].key, lang_map[i].value);
  }

  printf("\n");
  printf("Traducción: %s\n", stbds_shget(lang_map, "my.funny.string"));
  
cleanup:
  free_lang_map(lang_map);
  fclose(f);
  return err;
}
