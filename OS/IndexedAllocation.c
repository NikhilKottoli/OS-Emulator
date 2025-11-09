// Indexed Allocation
#include <stdio.h>
#define DISK_SIZE 10
#define MAX_FILE_BLOCKS 10

typedef struct {
    int indexBlock;
    int fileBlocks[MAX_FILE_BLOCKS];
} IndexedFile;

int allocateIndexed(IndexedFile *file, int diskStatus[], int fileBlockCount) {
    if (fileBlockCount > MAX_FILE_BLOCKS) return 0; // Limit exceeded
    
    for (int i = 0; i < DISK_SIZE; i++) {
        if (diskStatus[i] == 0) { // Find first free block for index block
            file->indexBlock = i;
            diskStatus[i] = 1;
            break;
        }
    }
    
    int allocated = 0;
    for (int i = 0; i < DISK_SIZE && allocated < fileBlockCount; i++) {
        if (diskStatus[i] == 0) {
            file->fileBlocks[allocated++] = i;
            diskStatus[i] = 1;
        }
    }
    return allocated == fileBlockCount; // Return success if all blocks allocated
}

int main() {
    IndexedFile file;
    int fileBlockCount;
    printf("Enter number of file blocks (max %d): ", MAX_FILE_BLOCKS);
    scanf("%d", &fileBlockCount);

    if (fileBlockCount > MAX_FILE_BLOCKS || fileBlockCount <= 0) {
        printf("Allocation failed: Invalid block count.\n");
        return 0;
    }
    
    int diskStatus[DISK_SIZE];
    printf("Enter disk status (0 for free, 1 for occupied, space-separated): ");
    for (int i = 0; i < DISK_SIZE; i++) {
        scanf("%d", &diskStatus[i]);
    }
    
    if (allocateIndexed(&file, diskStatus, fileBlockCount)) {
        printf("File allocated successfully using Indexed Allocation.\n");
        printf("Index Block: %d\n", file.indexBlock);
        printf("Allocated File Blocks Indices: ");
        for (int i = 0; i < fileBlockCount; i++) {
            printf("%d ", file.fileBlocks[i]);
        }
        printf("\n");
    } else {
        printf("Allocation failed: Not enough free blocks available.\n");
    }
    return 0;
}
