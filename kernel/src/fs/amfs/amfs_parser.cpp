#include <amfs/amfs_dec.h>
#include <flanterm/flantermglobal.h>
#include <memory/gccmemory.h>
#include <memory/dlmalloc.h>
#include <string/string.h>

amfs_handle_t amfs_init(uint8_t* fs_start) {
    if (!fs_start) {
        printf("Invalid filesystem start address.\n");
        return NULL;
    }
    amfs_superblock_t* superblock = (amfs_superblock_t*) fs_start;

    // Check magic number to verify it's a valid AMFS
    if (superblock->magic != 0xACF5) {
        return NULL;  // Invalid AMFS
    }

    return (amfs_handle_t) fs_start;  // Return pointer to the filesystem start
}

int64_t amfs_get_file_size(amfs_handle_t handle, const char* file_path) {
    if (!handle) {
        printf("File system not initialized.\n");
        return -1;
    }
    if (!file_path) {
        printf("Invalid file path.\n");
        return -1;
    }
    amfs_superblock_t* superblock = (amfs_superblock_t*) handle;
    uint8_t* metadata_block = (uint8_t*) handle + superblock->metadata_ptr;

    // Traverse the metadata to find the file
    uint32_t num_files = superblock->total_files;
    amfs_metadata_t* metadata = (amfs_metadata_t*) metadata_block;

    for (uint32_t i = 0; i < num_files; ++i) {
        amfs_file_metadata_t* file_meta = &metadata[i].file;

        // Compare file names
        if (strcmp(file_meta->common.name, file_path) == 0) {
            return file_meta->file_size;
        }
    }

    return -1;  // File not found (-1 becomes )
}

uint8_t* amfs_read_file(amfs_handle_t fs_handle, const char* file_path, uint32_t* size) {
    if (!fs_handle) {
        printf("File system not initialized.\n");
        return nullptr;
    }
    if (!file_path) {
        printf("Invalid file path.\n");
        return nullptr;
    }
    if (!size) {
        printf("Invalid size pointer.\n");
        return nullptr;
    }
    amfs_superblock_t* superblock = (amfs_superblock_t*) fs_handle;
    uint8_t* metadata_block = (uint8_t*) fs_handle + superblock->metadata_ptr;

    // Traverse the metadata to find the file
    uint32_t num_files = superblock->total_files;
    amfs_metadata_t* metadata = (amfs_metadata_t*) metadata_block;

    for (uint32_t i = 0; i < num_files; ++i) {
        amfs_file_metadata_t* file_meta = &metadata[i].file;

        // Compare file names
        if (strcmp(file_meta->common.name, file_path) == 0) {
            // Allocate memory for the file data
            uint8_t* file_data = (uint8_t*) malloc(file_meta->file_size);
            if (!file_data) {
                return NULL;  // Memory allocation failure
            }

            // Copy the file data into the allocated memory
            uint8_t* data_block = (uint8_t*) fs_handle + superblock->data_ptr;
            memcpy(file_data, data_block + file_meta->file_offset, file_meta->file_size);

            // Set the size of the file
            *size = file_meta->file_size;

            return file_data;  // Return the pointer to the file data
        }
    }

    return NULL;  // File not found
}

void amfs_list_directory(amfs_handle_t fs_handle, const char* directory_path) {
    if (!fs_handle) {
        printf("File system not initialized.\n");
        return;
    }
    if (!directory_path) {
        printf("Invalid directory path.\n");
        return;
    }
    amfs_superblock_t* superblock = (amfs_superblock_t*) fs_handle;
    uint8_t* metadata_block = (uint8_t*) fs_handle + superblock->metadata_ptr;

    uint32_t num_directories = superblock->total_directories;
    amfs_metadata_t* metadata = (amfs_metadata_t*) metadata_block;

    // If the directory path is "/", list the root directory
    if (strcmp(directory_path, "/") == 0) {
        for (uint32_t i = 0; i < num_directories; ++i) {
            amfs_directory_metadata_t* dir_meta = &metadata[i].directory;

            // List all files and subdirectories within the root directory
            uint32_t child_idx = dir_meta->first_child_idx;

            while (child_idx != INVALID_INDEX) {
                if (child_idx >= superblock->total_files + superblock->total_directories) {
                    // This index is out of bounds, skip it
                    break;
                }

                amfs_metadata_t* child_meta = &metadata[child_idx];

                if (child_meta->file.file_size > 0) {
                    printf("File: %s (%d bytes)\n", child_meta->file.common.name, child_meta->file.file_size);
                } else {
                    printf("Directory: %s\n", child_meta->directory.common.name);
                }

                // Move to next sibling
                child_idx = child_meta->directory.sibling_idx;
            }

            return;  // Root directory listing complete
        }
    } else {
        // Traverse metadata to find the directory
        for (uint32_t i = 0; i < num_directories; ++i) {
            amfs_directory_metadata_t* dir_meta = &metadata[i].directory;

            // Compare directory names
            if (strcmp(dir_meta->common.name, directory_path) == 0) {
                // List all files and subdirectories within this directory
                uint32_t child_idx = dir_meta->first_child_idx;

                while (child_idx != INVALID_INDEX) {
                    if (child_idx >= superblock->total_files + superblock->total_directories) {
                        // This index is out of bounds, skip it
                        break;
                    }

                    amfs_metadata_t* child_meta = &metadata[child_idx];

                    if (child_meta->file.file_size > 0) {
                        printf("File: %s (%d bytes)\n", child_meta->file.common.name, child_meta->file.file_size);
                    } else {
                        printf("Directory: %s\n", child_meta->directory.common.name);
                    }

                    // Move to next sibling
                    child_idx = child_meta->directory.sibling_idx;
                }

                return;  // Directory found, listing complete
            }
        }

        printf("Directory not found.\n");
    }
}
