/**
    * @file: wsfs.c
    * @author: without eyes
    *
    * This file contains implementation of functions related
    * to Without eyeS's File System(WSFS).
*/

#include "../include/wsfs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_structs.h"
#include "../include/ui.h"

int wsfs_init(void) {
    // create tree with root in /
    struct FileNode* rootDir = create_root_dir();

    // show UI
    struct FileNode* currentDir = rootDir;
    run_ui(currentDir);

    // free fs
    wsfs_deinit(rootDir);

    return EXIT_SUCCESS;
}

void wsfs_deinit(struct FileNode* rootDir) {
    struct FileNode* currentFile = rootDir->attributes.directoryContent;
    while (currentFile != NULL) {
        struct FileNode* nextFile = currentFile->next;
        free(currentFile->attributes.name);
        free(currentFile);
        currentFile = nextFile;
    }
    free(rootDir->attributes.name);
    free(rootDir);
}

void change_current_dir(struct FileNode** currentDir, const char* dirName) {
    struct FileNode* currentFile = (*currentDir)->attributes.directoryContent;
    while (currentFile != NULL &&
        (currentFile->attributes.type != FILE_TYPE_DIR ||
        strcmp(currentFile->attributes.name, dirName) != 0)) {
        currentFile = currentFile->next;
    }

    if (currentFile != NULL) {
        *currentDir = currentFile;
    } else {
        printf("Can't open file as directory!");
    }
}