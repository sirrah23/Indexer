#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: ./index <inverted-index filename> <directory or file name>\n");
        return 1;
    }

    struct stat obj;
    stat(argv[2], &obj);
    if(S_ISREG(obj.st_mode))
        printf("File is regular.\n");
    else if (S_ISDIR(obj.st_mode))
        printf("File is a directory.\n");

    return 0;
}
