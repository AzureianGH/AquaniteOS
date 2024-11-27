#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct 
{
    uint32_t magic;
    uint16_t version;
    uint32_t metadata_ptr;
    uint32_t data_ptr;
    uint32_t total_files;
    uint32_t total_directories;

} __attribute__((packed)) amfs_superblock_t;
typedef struct 
{
    char name[32];
    uint32_t parent_idx;
} __attribute__((packed)) amfs_common_metadata_t;
typedef struct 
{
    amfs_common_metadata_t common;
    uint32_t first_child_idx;
    uint32_t sibling_idx;
} __attribute__((packed)) amfs_directory_metadata_t;
typedef struct 
{
    amfs_common_metadata_t common;
    uint32_t file_offset;
    uint32_t file_size;
} __attribute__((packed)) amfs_file_metadata_t;
typedef union 
{
    amfs_directory_metadata_t directory;
    amfs_file_metadata_t file;
} __attribute__((packed)) amfs_metadata_t;

typedef void* amfs_handle_t;

#define INVALID_INDEX 0xFFFFFFFF

uint8_t* amfs_read_file(amfs_handle_t fs_handle, const char* file_path, uint32_t* size); //example: amfs_read_file(fs_handle, "/testing/file.txt", &size);
int64_t amfs_get_file_size(amfs_handle_t handle, const char* file_path); //example: amfs_get_file_size(fs_handle, "/testing/file.txt");
void amfs_list_directory(amfs_handle_t handle, const char* directory_path); //example: amfs_list_directory(fs_handle, "/testing");
amfs_handle_t amfs_init(uint8_t* fs_start); //example: amfs_init(fs_start);