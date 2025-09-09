#include <stdio.h>

void least_frequently_used(int n, int arr[n], int size) {
    int ans = 0;
    int frame[size]; 
    int frequency[size];
    int age[size];
    
    for (int i = 0; i < size; i++) {
        frame[i] = -1;
        frequency[i] = 0;
        age[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < size; j++) {
            if (frame[j] == arr[i]) { 
                found = 1; 
                frequency[j]++;
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
                frequency[empty] = 1;
                age[empty] = i;
            } else {
                int remove_index = 0;
                int min_frequency = frequency[0];
                int oldest_age = age[0];
                
                for (int j = 1; j < size; j++) {
                    if (frequency[j] < min_frequency) {
                        min_frequency = frequency[j];
                        oldest_age = age[j];
                        remove_index = j;
                    }
                    else if (frequency[j] == min_frequency && age[j] < oldest_age) {
                        oldest_age = age[j];
                        remove_index = j;
                    }
                }
                
                frame[remove_index] = arr[i];
                frequency[remove_index] = 1;
                age[remove_index] = i;
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
    printf("Enter the reference string: ");
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    printf("Enter frame size: ");
    scanf("%d", &size);

    least_frequently_used(n, arr, size);
    return 0;
}