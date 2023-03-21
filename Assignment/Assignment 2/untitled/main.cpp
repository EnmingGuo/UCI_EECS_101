#include <iostream>
void cal(char * arr,int num){
    float summ=0;
    for(int i=0;i<num;i++){
        summ+=arr[i];
    }
    float avg=summ/num;
    printf("Avg: %.3f ",avg);
    float var=0;
    for(int i=0;i<num;i++){
        var+=(arr[i]-avg)*(arr[i]-avg);
    }
    var=var/num;
    printf("Var: %.3f\n",var);

}
int main() {
    //std::cout << "Hello, World!" << std::endl;
    char num[15]={8,8,9,10,8,9,8,10,10,10,11};
    cal(num,11);
    return 0;
}
