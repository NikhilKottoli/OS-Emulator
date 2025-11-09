#include <stdio.h>

int search(int *arr, int start, int target) {
    for (int i = start - 1; i >= 0; i--) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

void least_recently_used(int n, int arr[n], int size) {
    int ans = 0;
    int frame[size]; 
    for (int i = 0; i < size; i++) frame[i] = -1;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < size; j++) {
            if (frame[j] == arr[i]) { 
                found = 1; 
                break; 
            }
        }

        if (!found) {
            int empty = -1;
            for (int j = 0; j < size; j++) {
                if (frame[j] == -1) { 
                    empty = j; 
                    break; 
                }
            }

            if (empty != -1) {
                frame[empty] = arr[i];
            } else {
                int remove_index = 0;
                int min = n;
                for (int j = 0; j < size; j++) {
                    int temp = search(arr, i, frame[j]);
                    if (temp < min) {
                        min = temp;
                        remove_index = j;
                    }
                }
                frame[remove_index] = arr[i];
            }
            ans++;
        }

        for (int j = 0; j < size; j++) {
            if (frame[j] != -1) printf("%d ", frame[j]);
        }
        printf("\n");
    }

    printf("Page faults : %d\n", ans);
}

int main() {
    int n, size;
    printf("Enter the length of reference string: ");
    scanf("%d", &n);

    int arr[n];
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    printf("Enter frame size: ");
    scanf("%d", &size);

    least_recently_used(n, arr, size);
    return 0;
}
