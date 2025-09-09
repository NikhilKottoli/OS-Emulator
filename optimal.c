#include <stdio.h>

#define N 100

int search(int * arr,int start,int end,int target){
    int ans = -1;
    for(int i=start;i<end;i++){
        if(arr[i] == target){
            ans = i;
            break;
        }
    }
    return ans;
}

void optical(int n,int arr[n],int size){
    int ans = 0;

    int frame[size];for(int i=0;i<size;i++)frame[i]=-1;

    for(int i=0;i<n;i++){
        for(int j=0;j<size;j++){
            if(frame[j] == -1) continue;
            printf("%d ",frame[j]);
        }
        printf("\n");

        int found = 0;
        for(int j=0;j<size;j++){
            if(frame[j] == arr[i]) {found=1;break;}
        }
        if(found){
            continue;
        }
        else{
            int remove_index = 0;
            int max = -1;
            for(int j=0;j<size;j++){
                int temp;
                temp = search(arr,i+1,n,frame[j]);
                
                if(temp == -1) {
                    remove_index = j;
                    break;
                }
                else{
                    if(temp > max){
                        max = temp;
                        remove_index = j;
                    }
                }
            }
            // printf("[%d]",remove_index);
            frame[remove_index] = arr[i];
            ans++;
        }
    }

    for(int j=0;j<size;j++){
        if(frame[j] == -1) continue;
        printf("%d ",frame[j]);
    }
    printf("\n");

    printf("Page Faults : %d\n",ans);
}

int main(){
    int n;
    int size;
    printf("Enter the length of reference string:");
    scanf("%d",&n);

    int arr[n];

    for(int i=0;i<n;i++) scanf("%d",&arr[i]);

    printf("Enter frame size :");
    scanf("%d",&size);
    //input done

    optical(n,arr,size);
    return 0;
}
/*
15
6 7 8 9 6 7 1 6 7 8 9 1 7 9 6
3
*/