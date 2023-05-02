/* PLEASE include these headers */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

char *tolowerstrdynamic(char *str)
{
  char *nstr = strdup(str);
  for (char *pstr = nstr; *pstr; pstr++)
  {
    *pstr = tolower(*pstr);
  }
  return nstr;
}

char *tolowerstr(char *str)
{
  for (char *pstr = str; *pstr; pstr++)
  {
    *pstr = tolower(*pstr);
  }
  return str;
}

size_t clean_filename(char *filename)
{
  size_t i = 0;
  size_t e = 0;
  for (; filename[e] != '\0'; i++, e++)
  {
    if (!strncmp(filename + e, "%25", 3))
    {
      filename[e] = '%';
      filename[i] = filename[e];
      e += 2;
    }
    else if (!strncmp(filename + e, "%20", 3))
    {
      filename[e] = ' ';
      filename[i] = filename[e];
      e += 2;
    }
    else
    {
      filename[i] = filename[e];
    }
  }
  filename[i] = filename[e];
}

const char *get_end_pos(const char *first_header)
{
  return strchr(first_header, '\r') - 9;
}

char *content_type(const char *extension_str)
{
  if (extension_str == NULL)
  {
    return "application/octet-stream";
  }
  // printf("EXTENSION STR: %s\n", extension_str);
  if (!strncmp(extension_str, ".html", 5))
  {
    return "text/html";
  }
  else if (!strncmp(extension_str, ".htm", 4))
  {
    return "text/html";
  }
  else if (!strncmp(extension_str, ".txt", 4))
  {
    return "text/plain";
  }
  else if (!strncmp(extension_str, ".jpg", 4))
  {
    return "image/jpeg";
  }
  else if (!strncmp(extension_str, ".jpeg", 5))
  {
    return "image/jpeg";
  }
  else if (!strncmp(extension_str, ".png", 4))
  {
    return "image/png";
  }
}

FILE *open_case_insensitive(char *filename)
{
  char *lower_filename = tolowerstrdynamic(filename);
  // printf("%s\n\n", lower_filename);
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      char *lower_direntname = tolowerstrdynamic(dir->d_name);
      // printf("%s\n", lower_direntname);

      if (!strcmp(lower_filename, lower_direntname))
      {
        free(lower_direntname);
        free(lower_filename);
        return fopen(dir->d_name, "r");
      }
      free(lower_direntname);
    }
    closedir(d);
  }
  free(lower_filename);
  return NULL;
}