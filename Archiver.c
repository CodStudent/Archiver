#include <stdio.h>
#include <string.h>
#include <malloc.h>

long GetFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    return size;
}

int main(int argc, char** argv) {
    if (argc <= 3) {
        return 0;
    }
    else {
        if (argv[1][2] != 'f')
            return 0;
    }
    char* ArchiveName = argv[2];

    if ((argv[3][2] == 'c') && (argc > 4)) {
        unsigned int CurrentFile = 0, KolvOfFiles = argc - 4;
        FILE* arc = fopen(ArchiveName, "wb");
        if (arc == NULL) {
            fclose(arc);
            printf("I can't create file\n");
            return 0;
        }
        while (CurrentFile != KolvOfFiles) {
            CurrentFile++;
            FILE* f = fopen(argv[CurrentFile + 3], "rb");
            unsigned char LenOfFileName = strlen(argv[CurrentFile + 3]);
            fputc(LenOfFileName, arc);
            fwrite(argv[CurrentFile + 3], sizeof(unsigned char), LenOfFileName, arc);
            long size = GetFileSize(f);
            fwrite(size, sizeof(long), 1, arc);
            unsigned char* buffer = calloc(size, sizeof(unsigned char));
            fread(buffer, sizeof(unsigned char), size, f);
            fwrite(buffer, sizeof(unsigned char), size, arc);
            free(buffer);
            fclose(f);
        }
        fclose(arc);
    }
    else if (argv[3][2] == 'e') {
        FILE* arc = fopen(ArchiveName, "rb");
        if (arc == NULL) {
            fclose(arc);
            printf("I can't create file\n");
            return 0;
        }
        while (1 != -1) {
            int LenOfFileName;
            LenOfFileName = fgetc(arc);
            printf("Len of name: %d\n", LenOfFileName);
            if (LenOfFileName > 255) {
                printf("Name is long\n");
                fclose(arc);
                return 0;
            }
            char Name[256];
            printf("Name:");
            for (int i = 0; i < LenOfFileName; i++) {
                Name[i] = fgetc(arc);
                printf("%c", Name[i]);
            }
            printf("\n");
            long FileSize = fgetc(arc);
            printf("Size: %d\n", FileSize);
            FILE* file = fopen(Name, "wb");
            if (file == NULL) {
                printf("I can't create file\n");
                fclose(file);
                break;
            }
            char* buffer = calloc(sizeof(char), FileSize);
            fread(buffer, sizeof(char), FileSize, arc);
            fwrite(buffer, sizeof(char), FileSize, file);
            free(buffer);
            fclose(file);
        }
        fclose(arc);
    }
    else if (argv[3][2] == 'l') {
        FILE* archive = fopen(ArchiveName, "rb");
        if (archive == NULL) {
            printf("File not found\n");
            return 0;
        }
        fclose(archive);
    }
    return 0;
}
