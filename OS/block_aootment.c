#include <stdio.h>
#include <stdbool.h>

#define MAX_BLOCKS 30
#define MAX_PROCESSES 30

void firstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    for (int i = 0; i < n; i++) allocation[i] = -1;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }
    
    printf("First Fit Allocation:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d (%d KB) -> %s\n", i+1, processSize[i], allocation[i] != -1 ? "Allocated" : "Not Allocated");
    }
}

void bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    for (int i = 0; i < n; i++) allocation[i] = -1;
    
    for (int i = 0; i < n; i++) {
        int bestIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx]) {
                    bestIdx = j;
                }
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }
    
    printf("Best Fit Allocation:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d (%d KB) -> %s\n", i+1, processSize[i], allocation[i] != -1 ? "Allocated" : "Not Allocated");
    }
}

void worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    for (int i = 0; i < n; i++) allocation[i] = -1;
    
    for (int i = 0; i < n; i++) {
        int worstIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx]) {
                    worstIdx = j;
                }
            }
        }
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }
    
    printf("Worst Fit Allocation:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d (%d KB) -> %s\n", i+1, processSize[i], allocation[i] != -1 ? "Allocated" : "Not Allocated");
    }
}

void nextFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    for (int i = 0; i < n; i++) allocation[i] = -1;
    int lastAllocated = 0;
    
    for (int i = 0; i < n; i++) {
        int j;
        for (j = lastAllocated; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                lastAllocated = j;
                break;
            }
        }
        if (j == m) {
            for (j = 0; j < lastAllocated; j++) {
                if (blockSize[j] >= processSize[i]) {
                    allocation[i] = j;
                    blockSize[j] -= processSize[i];
                    lastAllocated = j;
                    break;
                }
            }
        }
    }
    
    printf("Next Fit Allocation:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d (%d KB) -> %s\n", i+1, processSize[i], allocation[i] != -1 ? "Allocated" : "Not Allocated");
    }
}

int main() {

    int m,n;
    printf("Enter number of blocks: ");
    scanf("%d", &m);
    printf("Enter number of processes: ");
    scanf("%d", &n);


    int blockSize[MAX_BLOCKS];
    int processSize[MAX_PROCESSES];

    printf("Enter block sizes:\n");
    for (int i = 0; i < m; i++) {
        scanf("%d", &blockSize[i]);
    }

    printf("Enter process sizes:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &processSize[i]);
    }

    int blockSizeCopy[MAX_BLOCKS];
    
    for (int i = 0; i < m; i++) blockSizeCopy[i] = blockSize[i];
    firstFit(blockSizeCopy, m, processSize, n);
    
    for (int i = 0; i < m; i++) blockSizeCopy[i] = blockSize[i];
    bestFit(blockSizeCopy, m, processSize, n);
    
    for (int i = 0; i < m; i++) blockSizeCopy[i] = blockSize[i];
    worstFit(blockSizeCopy, m, processSize, n);
    
    for (int i = 0; i < m; i++) blockSizeCopy[i] = blockSize[i];
    nextFit(blockSizeCopy, m, processSize, n);
    
    return 0;
}