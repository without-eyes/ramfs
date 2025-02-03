/**
    * @file: file_structs.h
    * @author: without eyes
    *
    * This file contains structs and functions related to files
    * and file nodes.
*/

#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#define MAX_NAME_SIZE 32

enum FileType {
    FILE_TYPE_UNKNOWN = 0,
    FILE_TYPE_FILE = 1,
    FILE_TYPE_DIR = 2
};

struct FileNode;

struct Timestamp {
    uint8_t hour;
    uint8_t minute;
};

struct FileAttributes {
    char* name;
    union {
        struct FileNode* directoryContent;
        char* fileContent;
    };
    struct Timestamp createdAt;
    enum FileType type;
};

struct FileNode {
    struct FileAttributes attributes;
    struct FileNode* parent;
    struct FileNode* next;
};

/**
    * Create file node in "parent" directory. The caller is
    * responsible for freeing the memory allocated for the
    * file node by calling free(). If name of the directory is
    * "\" , it will be it's own parent.
    *
    * @param[in] parent The directory where file will be located.
    * @param[in] fileNodeName The name of new file node.
    * @param[in] fileType The type of new file node.
    * @return Returns file as pointer to struct FileNode.
    *
    * @pre parent != NULL
*/
struct FileNode* create_file_node(struct FileNode* parent, const char* fileNodeName, enum FileType fileType);

/**
    * Add "child" file/directory to "parent" directory's linked list.
    *
    * @param[in,out] parent The directory where file/directory will be
    * located.
    * @param[in] child The file/directory that will be added to "parent"
    * directory.
    *
    * @pre parent != NULL && child != NULL
*/
void add_to_dir(struct FileNode* parent, struct FileNode* child);

/**
    * Get file type first letter(e.g. 'd' or 'f').
    *
    * @param[in] type The value from enum FileType.
    * @return Returns first letter of type as char.
*/
char get_file_type_letter(enum FileType type);

/**
    * Write text into file.
    *
    * @param[in] file The file in which text will be written.
    * @param[in] text The text which will be written into file.
*/
void write_to_file(struct FileNode* file, char* text);

/**
    * Reads content from file.
    *
    * @param[in] file The file from which content will be read.
*/
void print_file_content(const struct FileNode* file);

/**
    * Find file node by name in current directory.
    *
    * @param[in,out] currentDir The directory where user is currently
    * located.
    * @param[in] fileNodeName The name of the file node which user
    * wants to find.
    *
    * @pre currentDir != NULL && fileNodeName != NULL
*/
struct FileNode* find_file_node_in_curr_dir(const struct FileNode* currentDir, const char* fileNodeName);

/**
    * Find file node by name in entire file system.
    *
    * @param[in,out] rootDir The root directory.
    * @param[in] fileNodeName The name of the file node which user
    * wants to find.
    *
    * @pre rootDir != NULL && fileNodeName != NULL
*/
struct FileNode* find_file_node_in_fs(const struct FileNode* rootDir, const char* fileNodeName);

/**
    * Get file node path.The caller is responsible for freeing
    * the memory allocated for the file node by calling free().
    *
    * @param[in,out] currentDir The directory where user is currently
    * located.
    * @param[in] fileNodeName The name of the file node which path
    * user wants to get.
    *
    * @pre currentDir != NULL && fileNodeName != NULL
*/
char* get_file_node_path(struct FileNode* currentDir, const char* fileNodeName);

/**
    * Delete file node in current directory. CAN'T DELETE NOT EMPTY
    * FOLDERS!
    *
    * @param[in,out] currentDir The directory where user is currently
    * located.
    * @param[in] fileNodeName The name of the file node which path
    * user wants to delete.
    *
    * @pre currentDir != NULL && fileNodeName != NULL
*/
void delete_file_node(struct FileNode* currentDir, const char* fileNodeName);

#endif //FILE_H