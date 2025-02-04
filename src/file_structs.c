/**
    * @file: file_structs.c
    * @author: without eyes
    *
    * This file contains implementation of functions related
    * to files and file nodes.
*/

#include "../include/file_structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"

struct FileNode* create_file_node(struct FileNode* parent, const char* fileNodeName, const enum FileType fileType) {
    struct FileNode* fileNode = malloc(sizeof(struct FileNode));
    fileNode->attributes.name = malloc(strlen(fileNodeName) + 1);
    strcpy(fileNode->attributes.name, fileNodeName);
    fileNode->attributes.createdAt = get_current_time();
    fileNode->attributes.type = fileType;
    fileNode->attributes.directoryContent = NULL;
    fileNode->attributes.fileContent = NULL;
    fileNode->attributes.symlinkTarget = NULL;
    fileNode->next = NULL;
    fileNode->parent = strcmp(fileNodeName, "\\") == 0 ? fileNode : parent;
    return fileNode;
}

void add_to_dir(struct FileNode* parent, struct FileNode* child) {
    if (parent->attributes.directoryContent == NULL) {
        parent->attributes.directoryContent = child;
        return;
    }

    struct FileNode* current = parent->attributes.directoryContent;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = child;
}

char get_file_type_letter(const enum FileType type) {
    switch (type) {
        case FILE_TYPE_DIR:
            return 'd';

        case FILE_TYPE_FILE:
            return 'f';

        case FILE_TYPE_SYMLINK:
            return 's';

        case FILE_TYPE_UNKNOWN:
            return '-';

        default:
            return '?';
    }
}

void write_to_file(struct FileNode* fileNode, char* text) {
    if (fileNode->attributes.type == FILE_TYPE_SYMLINK) {
        fileNode->attributes.symlinkTarget->attributes.fileContent = text;
    } else if (fileNode->attributes.type == FILE_TYPE_FILE) {
        fileNode->attributes.fileContent = text;
    }
}

void print_file_content(const struct FileNode* fileNode) {
    if (fileNode->attributes.type == FILE_TYPE_SYMLINK) {
        printf("%s\n", fileNode->attributes.symlinkTarget->attributes.fileContent);
    } else if (fileNode->attributes.type == FILE_TYPE_FILE) {
        printf("%s\n", fileNode->attributes.fileContent);
    }
}

struct FileNode* find_file_node_in_curr_dir(const struct FileNode* currentDir, const char* fileNodeName) {
    struct FileNode* currentFile = currentDir->attributes.directoryContent;
    while (currentFile != NULL && strcmp(currentFile->attributes.name, fileNodeName) != 0) {
        currentFile = currentFile->next;
    }
    return currentFile;
}

struct FileNode* find_file_node_in_fs(const struct FileNode* rootDir, const char* fileNodeName) {
    if (!rootDir || !fileNodeName) return NULL;

    if (strcmp(rootDir->attributes.name, fileNodeName) == 0) {
        return (struct FileNode*)rootDir;
    }

    struct FileNode* currFileNode = rootDir->attributes.directoryContent;
    while (currFileNode) {
        struct FileNode* result = find_file_node_in_fs(currFileNode, fileNodeName);
        if (result) return result;
        currFileNode = currFileNode->next;
    }

    return NULL;
}

char* get_file_node_path(struct FileNode* currentDir, const char* fileNodeName) {
    while (strcmp(currentDir->attributes.name, "\\") != 0) {
        currentDir = currentDir->parent;
    }

    struct FileNode* fileNode = find_file_node_in_fs(currentDir, fileNodeName);

    struct FileNode* tempFileNode = fileNode;
    size_t pathLength = 1;
    while (strcmp(tempFileNode->attributes.name, "\\") != 0) {
        pathLength += strlen(tempFileNode->attributes.name) + 1;
        tempFileNode = tempFileNode->parent;
    }

    char* path = malloc(pathLength);
    path[pathLength - 1] = '\0';
    size_t pos = pathLength - 1;
    struct FileNode* currFileNode = fileNode;
    while (strcmp(currFileNode->attributes.name, "\\") != 0) {
        const size_t nameLen = strlen(currFileNode->attributes.name);
        pos -= nameLen;
        memcpy(&path[pos], currFileNode->attributes.name, nameLen);
        if (pos > 0) {
            path[--pos] = '\\';
        }
        currFileNode = currFileNode->parent;
    }

    return path;
}

void delete_file_node(struct FileNode* currentDir, const char* fileNodeName) {
    struct FileNode* fileNodeToDelete = find_file_node_in_curr_dir(currentDir, fileNodeName);
    if (fileNodeToDelete->attributes.type == FILE_TYPE_DIR || fileNodeToDelete->attributes.directoryContent != NULL) {
        return;
    }

    struct FileNode* currentFileNode = currentDir->attributes.directoryContent;
    if (currentFileNode == fileNodeToDelete) {
        currentDir->attributes.directoryContent = currentDir->attributes.directoryContent->next;
        free(fileNodeToDelete);
        return;
    }

    while (currentFileNode->next != fileNodeToDelete) {
        currentFileNode = currentFileNode->next;
    }

    currentFileNode->next = currentFileNode->next->next;
    free(fileNodeToDelete);
}