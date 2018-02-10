#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_SIZE 128

struct _NODE;
typedef struct _NODE* Position;
typedef struct _NODE{
    char* name ;
    int prviKol;
    int drugiKol;
    int ukupno;
    Position next;
}node;

int readFromFile(Position* );
Position createNode();
int sortedInsert(Position,Position);
char* formatName(char*,char*);
int mainProgram();
int printList(Position);
int printGradeDivision(Position);
Position elementAt(Position,int);
int numberOfElements(Position);
int writeToFile(char*,Position);
int insert(Position,Position,int);
int readColloquium(Position* , int);
int sumPoints(Position);
Position sortByPoints(Position);
Position copyNode(Position);


int main(int argc,char** argv){
    mainProgram();
    return 0;
}

int mainProgram(){
    char fName[BUFF_SIZE];
    Position list=createNode();
   // printf("Unesite ime datoteke u kojoj se nalaze rezultati kolokvija: ");
    //scanf(" %s",fName);
    if(readFromFile(&list)!=0){
        printf("Greska pri otvaranju filea!\n");
        return -1;
    }
    sumPoints(list);
    list=sortByPoints(list);
    printList(list);
    printGradeDivision(list);
    writeToFile("izlaz.txt",list);
    return 0;
}


int readFromFile(Position* list){
    readColloquium(list,1);
    readColloquium(list,2);

}

int printList(Position list){
    if(list==NULL){
        printf("NULL pointer exception!\n");
        return -1;
    }
    Position iter=list;
    printf("Ime\tPrvi kol\tDrugi kol\t Ukupno\n");
    while(iter->next!=NULL){
        printf("%s\t%d\t%d\t%d\n",iter->next->name,iter->next->prviKol,iter->next->drugiKol,iter->next->ukupno);
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
    while(iter->next!=NULL && iter->next->ukupno>node->ukupno){
        iter=iter->next;
    }
    node->next=iter->next;
    iter->next=node;
    return 0;
}

int insert(Position list,Position node,int n){
    Position iter=NULL;
    if(list==NULL||node==NULL)return -1;
    iter=list;
    while(iter->next!=NULL){
        if(strcmp(iter->next->name,node->name)==0){
            if(n==2)
                iter->next->drugiKol=node->drugiKol;
            else
                iter->next->prviKol=node->prviKol;
            return 0;
        }
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
    fp=fopen(fName,"w");
    Position iter=list;
    while(iter->next!=NULL){
        fprintf(fp,"%s\t\t%d\t\t\%d\t%d\n",iter->next->name,iter->next->prviKol,iter->next->drugiKol,iter->next->ukupno);
        iter=iter->next;
    }
    return 0;
}

int readColloquium(Position* list,int n){
    Position tmp=NULL;
    if(list==NULL){
        printf("Null pointer\n");
        return-1;
    }
    FILE* fp=NULL;
    if(n==1){
        fp=fopen("prviKolokvij.txt","r");
    }
    else
        fp=fopen("drugiKolokvij.txt","r");
    if(fp==NULL){
        printf("error opening file!\n");
        return -2;
    }
    while(!feof(fp)){
        int bod,dud;
        char name[32],surname[32];
        if(n==1)
            fscanf(fp,"%s %s %d",name,surname,&bod);
        else{
            fscanf(fp,"%s %s %d %d",name,surname,&dud,&bod);
        }
        tmp=createNode();
        tmp->name=formatName(name,surname);
        printf("Ime je :%s\n",tmp->name);
        if(n==1)
            tmp->prviKol=bod;
        else
            tmp->drugiKol=bod;
        insert(*list,tmp,n);
    }
    fclose(fp);
}

Position sortByPoints(Position list){
    if(list==NULL){
        printf("NULL pointer exception!\n");
        return NULL;
    }
    Position tmp=NULL;
    tmp=createNode();
    Position iter=list;
    while(iter->next!=NULL){
        iter=iter->next;
        Position node=copyNode(iter);
        sortedInsert(tmp,node);
    }

    return tmp;
}

Position copyNode(Position node){
    Position p=createNode();
    p->name=node->name;
    p->drugiKol=node->drugiKol;
    p->prviKol=node->prviKol;
    p->ukupno=node->ukupno;
    return p;
}

int sumPoints(Position list){
    Position iter=list;
    if(list->next==NULL){
        printf("List is empty!\n");
        return -1;
    }
    while(iter->next!=NULL){
        iter->next->ukupno=iter->next->drugiKol+iter->next->prviKol;
        iter->next->ukupno/=2;
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
    printf("Za pet je potrebno barem %d bodova\n",tmp->prviKol+tmp->drugiKol);
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
        printf("Error allocating memory!\n");
        return p;
    }
    p->next=NULL;
    p->drugiKol=0;
    p->prviKol=0;
    return p;
}
