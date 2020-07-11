
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int PrintHex(char* ptr,int length){
    unsigned int iter;
    for(iter=0;iter<length;iter++)
        printf("%02X ",(unsigned char)ptr[iter]);

    return 0;
}

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char sig[];
} virus;

typedef struct link link;

struct link {
    link *nextVirus;
    virus *vir;
};

link * vList;           // global var

struct fun_desc {
    char *name;
    void (*fun)();
};

int min(int a,int b){
    if(a>b)
        return b;
    return a;
}

void list_print(){
    link* runner=(link*)malloc(sizeof(link));
    runner=vList;
    while(runner->vir!=NULL){
        printf("Virus name: %s \n",runner->vir->virusName);
        printf("Virus size: %d \n",runner->vir->SigSize);
        printf("signature: \n");
        PrintHex(runner->vir->sig,runner->vir->SigSize);
        printf("\n \n");
        runner=runner->nextVirus;
    }
}

link* list_append(link* virus_list, virus* data){
    if(data!=NULL){
        link* to_add=(link*)malloc(sizeof(link));
        to_add->vir=data;
        to_add->nextVirus=virus_list;
        return to_add;
    }
    return virus_list;
}

void list_free(){
    while(vList->vir!=NULL){
        free(vList->vir);
        vList=vList->nextVirus;
    }
}

void LoadSignatures(){
    char file_name[256];
    fscanf(stdin,"%s",file_name);
    fgetc(stdin);       //to reduce the enter in the end
    FILE* file=fopen(file_name,"r");
    fseek(file,0,SEEK_END);
    int file_size=ftell(file);
    fseek(file,0,SEEK_SET);     //return to the begining of the file
    int counter=0;
    while(counter<file_size){
        short virus_size;
        fread(&virus_size,1,2,file);             //to read the size of the virus
        virus * curr_virus=(virus*)malloc(sizeof(virus)+sizeof(char)*(virus_size-18));
        char buffer[virus_size-2];
        fread(buffer,1,(int)virus_size-2,file);           //to read the name+signature of the virus
        strncpy(curr_virus->virusName,buffer,16);                //copy the name ofthe virus
        curr_virus->SigSize=virus_size-18;
        int i;
        for(i=0;i<curr_virus->SigSize;i++)                        //copy the signature
            curr_virus->sig[i]=buffer[i+16];
        vList=list_append(vList,curr_virus);
        counter=counter+curr_virus->SigSize+18;
    }
    fclose(file);

}

void searchForSig(link *Cvir, int signaSize, char *buffer, unsigned int size, int i){
    while(i<size) {
        if (memcmp(Cvir->vir->sig, buffer + i, signaSize) == 0) {
            printf("virus location is: %d\n", i);
            printf("the virus name is: %s\n", Cvir->vir->virusName);
            printf("virus signature size is: %d \n",Cvir->vir->SigSize);
            printf("\n\n");
        }
        i++;
    }
}

void Detect_viruses(char *buffer, unsigned int size){     //buffer=ptr
    link* Clink=vList;
    while(Clink->vir!=NULL){
        searchForSig(Clink, Clink->vir->SigSize, buffer, size, 0);
        Clink=Clink->nextVirus;
    }

}

void pre_detect(){
    char scanFileName[256];
    fscanf(stdin,"%s",scanFileName);
    fgetc(stdin);       //to reduce the enter in the end
    FILE* SFile=fopen(scanFileName,"r");
    fseek(SFile,0,SEEK_END);
    int SFileSize=ftell(SFile);
    fseek(SFile,0,SEEK_SET);
    char Sbuffer[10000];
    unsigned int minSize=min(SFileSize,10000);
    fread(Sbuffer,1,SFileSize,SFile);
    char* ptr=Sbuffer;
    Detect_viruses(ptr,minSize);
    fclose(SFile);
}


void quit(){
    list_free();
    exit(0);
}


int main(int argc,char **argv){
    struct fun_desc farray[]={{"Load signatures",LoadSignatures},{"Print signatures",list_print},{"Detect viruses",pre_detect},{"Quit",quit}};
    vList=malloc(sizeof(link));
    int choosen;
    int i;
    while(1){
        printf("Please choose a function: \n");
        for(i=0;i<4;i++)
            printf("%d" "%s" "%s \n",i+1,") ", farray[i].name);     //i added to i 1 just to print the menu from 1 to 3 and now from 0 to 2
        printf("%s","Option: ");
        scanf("%d",&choosen);
        choosen=choosen-1;
        fgetc(stdin);       //to avoid the enter that enters after the numbers
        if(choosen==0)
            LoadSignatures();
        if(choosen==1)
            list_print();
        if(choosen==2){
            pre_detect();
            
        }
        if(choosen>=3)
            quit();

    }

    return 0;
}



