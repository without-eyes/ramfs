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

struct FileNode* wsfs_init(void) {
    return create_file_node(NULL, "\\", FILE_TYPE_DIR);
}

void wsfs_deinit(struct FileNode* fileNode) {
    free_file_node_recursive(fileNode);
}