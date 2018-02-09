#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_SIZE 128

struct _NODE;
typedef struct _NODE* Position;
typedef struct _NODE{
    char* name ;
    int bodovi;
    int postotak;
    Position next;
}node;

int readFromFile(char* ,Position* );
Position createNode();
int sortedInsert(Position,Position);
char* formatName(char*,char*);
int mainProgram();
int printList(Position);
int printGradeDivision(Position);
Position elementAt(Position,int);
int numberOfElements(Position);
int writeToFile(char*,Position);

int main(int argc,char** argv){
    mainProgram();
    return 0;
}

int mainProgram(){
    char fName[BUFF_SIZE];
    Position list=createNode();
    printf("Unesite ime datoteke u kojoj se nalaze rezultati kolokvija: ");
    scanf(" %s",fName);
    if(readFromFile(fName,&list)!=0){
        printf("Greska pri otvaranju filea!\n");
        return -1;
    }
    printList(list);
    printGradeDivision(list);
    writeToFile(fName,list);
    return 0;
}


int readFromFile(char* fName,Position* list){
    FILE* fp=fopen(fName,"r");
    Position tmp=NULL;
    if(fp==NULL){
        printf("Error reading file!\n");
        return -1;
    }
    if(list==NULL){
        printf("NULL pointer to list!\n");
        return -2;
    }
    while(!feof(fp)){
        char ime[BUFF_SIZE],prezime[BUFF_SIZE];
        int bodovi,postotak;
        tmp=NULL;
        fscanf(fp,"%s %s %d %d",ime,prezime,&bodovi,&postotak);
        if((tmp=createNode())==NULL){
            printf("Error allocating memory!\n");
            continue;
        }
        tmp->name=formatName(ime,prezime);
        tmp->bodovi=bodovi;
        tmp->postotak=postotak;
        sortedInsert(*list,tmp);
    }
    fclose(fp);
    return 0;
}

int printList(Position list){
    if(list==NULL){
        printf("NULL pointer exception!\n");
        return -1;
    }
    Position iter=list;
    printf("Ime\tBodovi\tPostotak\n");
    while(iter->next!=NULL){
        printf("%s\t%d\t%d\n",iter->next->name,iter->next->bodovi,iter->next->postotak);
        iter=iter->next;
    }
    return 0;
}

int sortedInsert(Position list,Position node){
    if(list==NULL){
        printf("NULL list !\n");
        return -1;
    }
    if(node==NULL){
        printf("NULL node!\n");
        return -2;
    }
    Position iter=list;
    while(iter->next!=NULL && iter->next->bodovi>node->bodovi){
        iter=iter->next;
    }
    node->next=iter->next;
    iter->next=node;
    return 0;
}

int writeToFile(char* fName,Position list){
    FILE*fp=NULL;
    if(list==NULL){
        printf("Empty list!\n");
        return -1;
    }
    strcat(fName,"sortirano.txt");
    fp=fopen(fName,"w");
    Position iter=list;
    while(iter->next!=NULL){
        fprintf(fp,"%s\t\t%d\t\t\n",iter->next->name,iter->next->bodovi);
        iter=iter->next;
    }
    return 0;
}

char* formatName(char* name,char*surname){
    char* fullName=(char*)malloc(3*BUFF_SIZE*sizeof(char));
    memset(fullName,0,3*BUFF_SIZE);
    strcat(fullName,name);
    strcat(fullName," ");
    strcat(fullName,surname);
    return fullName;
}

int printGradeDivision(Position list){
    if(list==NULL){
        printf("NULL pointer exception!\n");
        return -1;
    }
    int n=numberOfElements(list);
    printf("Ukupno je polagalo %d ljudi.\n",n);
    Position tmp=elementAt(list,n*(0.15));
    if(tmp==NULL){
        printf("Problem!\n");
        return -2;
    }
    printf("Za pet je potrebno barem %d bodova(%d posto)\n",tmp->bodovi,tmp->postotak);
    return 0;
}

int numberOfElements(Position list){
    if(list==NULL){
         printf("NULL pointer exception!\n");
        return -1;
    }
    Position iter=list;
    int n=0;
    while(iter->next!=NULL){
        n++;
        iter=iter->next;
    }
    return n;
}

Position elementAt(Position list,int index){
     if(list==NULL){
        printf("NULL pointer exception!\n");
        return NULL;
    }
    int i=0;
    printf("Dobio sam indeks%d",index);
    Position iter=list;
    while(iter->next!=NULL&&index>i){
        iter=iter->next;
        i++;
    }
    if(i==index){
        return iter;
    }
    return NULL;
}

Position createNode(){
    Position p=NULL;
    p=(Position)malloc(sizeof(node));
    if(p==NULL){
        free(p);
        printf("Error allocating memory!\n");
        return p;
    }
    p->next=NULL;
    return p;
}
