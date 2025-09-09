#include <stdio.h>

void fifo(int n,int arr[n],int size){
    int ans = 0;

    int frame[size];for(int i=0;i<size;i++)frame[i]=-1;
    int current_first_element = -1;

    for(int i=0;i<n;i++){
        int found = 0;
        for(int j=0;j<size;j++){
            if(frame[j] == arr[i]) {found=1;break;}
        }
        if(found){
            continue;
        }
        else{
            ans++;
            frame[(++current_first_element)%size] = arr[i];
            current_first_element=current_first_element%size;
        }

        for(int j=0;j<size;j++){
            if(frame[j] == -1) continue;
            printf("%d ",frame[j]);
        }
        printf("\n");
    }

    printf("Memory fault occured %d time",ans); 
    return;
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

    fifo(n,arr,size);
    return 0;
}