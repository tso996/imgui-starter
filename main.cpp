#include<iostream>

int main(int argc, char* argv[]){
    std::cout<<argc<<std::endl;
    if(argv[argc]==NULL){
        std::cout<<"null found"<<std::endl;
    }
    char j;
    for(int i = 0; i < argc; i++){
        j = ' ';
        while(j!='\0'){
            j = *argv[i];
            std::cout<<j<<std::endl;
            argv[i]++;
        }
    }
    std::cout<<"hello world!"<<std::endl;
    return 0;
}