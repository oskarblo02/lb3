#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMESIZE 20 + 1
#define CONTENTSIZE 200 + 1
#define DATASIZE 1000 + 1
#define UNITSIZE 10 + 1
#define HALFTIMESIZE  + 1

typedef struct{
    char substanceName[NAMESIZE];
    int substanceMassNumber;
    double substanceDecayTime;
} Substance;

typedef struct{
    double time;
    char unit[UNITSIZE];
} Halftime;

Substance initNewSubstance(char aName[], int aMassNumber, double aDecayTime);
Halftime initHalftime(double aHalftime, char aUnit[]);
void viewMenu();
void addSubstancesToStruct(Substance aSubstance[], char aFileName[], int *pSize);
void viewSubstance(Substance *pSubstance);
void viewAllSubstances(Substance aSubstance[], int size);
double halfTimeConverter(Halftime *pHalftime);
void registerNewSubstance(Substance aSubstance[], int *pSize);
int compareName(const void *pSubstanceVoid1, const void *pSubstanceVoid2);
int compareMassNumber(const void *pSubstanceVoid1, const void *pSubstanceVoid2);
int compareHalftime(const void *pSubstanceVoid1, const void *pSubstanceVoid2);
void sortSubstances(Substance aSubstance[], int size);
void searchSubstanceName(Substance aSubstance[], Substance aFoundSubstance[], int size, int *pNewSize, int *pSubstancePlace);
void searchSubstanceMassNumber(Substance aSubstance[], Substance aFoundSubstance[], int size, int *pNewSize, int *pSubstancePlace);
void searchSubstanceDecayTime(Substance aSubstance[], Substance aFoundSubstance[], int size, int *pNewSize, int ifInChangeHalftime, int *pSubstancePlace);
void searchSubstance(Substance aSubstance[], Substance aFoundSubstance[], int size, int *pNewSize, int *pSubstancePlace);
void changeHalftimeWithName(Substance aSubstance[], Substance aFoundSubstance[], Substance aSearchResult[], int size, int *pNewSize, int *pSubstancePlace, int *pFoundSubstancePlace);
void changeHalftimeWithMassNumber(Substance aSubstance[], Substance aFoundSubstance[], Substance aSearchResult[], int size, int *pNewSize, int *pSubstancePlace, int *pFoundSubstancePlace);
void changeHalftimeWithDecayTime(Substance aSubstance[], Substance aFoundSubstance[], Substance aSearchResult[], int size, int *pNewSize, int *pSubstancePlace, int *pFoundSubstancePlace);
void changeHalfTime(Substance aSubstance[], Substance aFoundSubstance[], int size, int *pNewSize, int *pSubstancePlace, int *pFoundSubstancePlace);
void removeSubstance(Substance aSubstance[], Substance aFoundSubstance[], int *pSize, int *pNewSize, int *pSubstancePlace);
void saveSubstances(Substance aSubstance[], char aFileName[], int size);
    

int main(void){

    printf("starting up decay register data base\n");
    int run = 0;
    int used = 0;
    int newUsed = 0;
    int substancePlace = 0;
    char fileName[CONTENTSIZE];
    int foundSubstancePlace = 0; 
    Substance mySubstances[DATASIZE];
    Substance foundSubstances[DATASIZE];
    addSubstancesToStruct(mySubstances, fileName, &used);
    do{
        viewMenu();
        scanf("%d", &run);
        switch (run) {
            case 1 : registerNewSubstance(mySubstances, &used); break;
            case 2 : viewAllSubstances(mySubstances, used); break;
            case 3 : sortSubstances(mySubstances, used); break; 
            case 4 : searchSubstance(mySubstances, foundSubstances, used, &newUsed, &substancePlace); break;  
            case 5 : changeHalfTime(mySubstances, foundSubstances, used, &newUsed, &substancePlace, &foundSubstancePlace); break; 
            case 6 : removeSubstance(mySubstances, foundSubstances, &used, &newUsed, &substancePlace); break; 
            case 7 : saveSubstances(mySubstances, fileName, used); run++; break; 
            default: break;
        }
    } while (run != 8);
}

Substance initNewSubstance(char aName[], int aMassNumber, double aDecayTime){
    Substance newSubstance;
    strcpy(newSubstance.substanceName, aName);
    newSubstance.substanceMassNumber = aMassNumber;
    newSubstance.substanceDecayTime = aDecayTime;
    return newSubstance;
}

Halftime initHalftime(double aHalftime, char aUnit[]){
    Halftime newHalftime;
    newHalftime.time = aHalftime;
    strcpy(newHalftime.unit, aUnit);
    return newHalftime;
}

void viewMenu(){
    printf("\n(1) Registrera nya radioaktiva amnen\n");
    printf("(2) Alla amnen\n");                        
    printf("(3) Sortera amnen\n");
    printf("(4) Soka efter amne\n");
    printf("(5) Andra halveringstid\n");
    printf("(6) Avregistrera amne\n");
    printf("(7) Avsluta\n"); 
    printf("\nAnge val: ");
}

void addSubstancesToStruct(Substance aSubstance[], char aFileName[], int *pSize){
    FILE *pFile;
    int count  = 0;
    char name[CONTENTSIZE];
    int mass = 0;
    float decayTime = 0;
    int wrongInput = 1;
    do{
        printf("Ange text fil (.txt): ");
        scanf("%s", aFileName);
        if ((pFile = fopen(aFileName, "r"))){
            while ((count = getc(pFile)) != EOF){
                count = fscanf(pFile, "%s %d %e", name, &mass, &decayTime);
                count = count / 3;
                for (int i = 0; i < count; i++){
                    (*pSize)++;
                }
                aSubstance[(*pSize) - 1] = initNewSubstance(name, mass, decayTime);
            }
            wrongInput = 0;
        } else{
            printf("ERROR: Ange ny fil.\n");
        }
    } while (wrongInput);
    fclose(pFile);
}

void viewSubstance(Substance *pSubstance){
    printf("%-16s", pSubstance -> substanceName);
    printf("%-12d", pSubstance -> substanceMassNumber);
    printf("%.4e\n", pSubstance -> substanceDecayTime);
}

void viewAllSubstances(Substance aSubstance[], int size){       
    printf("\n");
    printf("Namn            Masstal     Halveringstid(s)\n");
    printf("_____________________________________________\n");
    for (int i = 0; i < size; i++){
        viewSubstance(&aSubstance[i]);
    }
    printf("\n");
}

double halfTimeConverter(Halftime *pHalftime){
    double halftimeSec = 0;
    if(strstr(pHalftime[0].unit, "min")){
        halftimeSec = (pHalftime -> time);
        halftimeSec = halftimeSec * 60; 
    } else if(strstr(pHalftime[0].unit, "h")){
        halftimeSec = (pHalftime -> time);
        halftimeSec = halftimeSec * 60 * 60; 
    } else if(strstr(pHalftime[0].unit, "dagar")){
        halftimeSec = (pHalftime -> time);
        halftimeSec = halftimeSec  * 60 * 60 * 24;
    } else if(strstr(pHalftime[0].unit, "ar")){
        halftimeSec = (pHalftime -> time);
        halftimeSec = halftimeSec * 60 * 60 * 24 * 365;
    } else {
        halftimeSec = (pHalftime -> time);
    }
    return halftimeSec;
}

void registerNewSubstance(Substance aSubstance[], int *pSize){
    char name[NAMESIZE];
    int value = 0;
    int keepGoing = 1;
    int massNumber = 0;
    double decayTime = 0.0f;
    char timeUnit[UNITSIZE];
    Halftime theHalftime[HALFTIMESIZE];
    printf("Registrera nya radioaktiva amnen\n");
    do{
        if((*pSize) < DATASIZE){
            printf("Ange namn (q for att avsluta): ");
            scanf("%s", name);
            if (strlen(name) > 1){
                printf("Ange masstal: ");
                scanf("%d", &massNumber);
                for (int i = 0; i < (*pSize); i++){
                    if (!strcmp(aSubstance[i].substanceName, name) && (aSubstance[i].substanceMassNumber == massNumber)){
                        value++;
                    }
                }
                if(value > 0){
                    printf("Amnet ar redan registrerat!\n");
                } else {
                    printf("Halveringstid (s,min,h,dagar,ar): ");
                    scanf("%lf %s", &decayTime, timeUnit);
                    theHalftime[0] = initHalftime(decayTime, timeUnit);
                    decayTime = halfTimeConverter(&theHalftime[0]);
                    aSubstance[*pSize] = initNewSubstance(name, massNumber, decayTime);
                    (*pSize)++;
                }
            } 
            if (name[0] == 'q'){
                keepGoing = 0;
            }
        } else {
        printf("Databas full.");
        keepGoing = 0;
        }
    }while(keepGoing); 
}

int compareName(const void *pSubstanceVoid1, const void *pSubstanceVoid2){
    const Substance *pSubstance1 = (const Substance *)pSubstanceVoid1;
    const Substance *pSubstance2 = (const Substance *)pSubstanceVoid2;
    return strcmp(pSubstance1 -> substanceName, pSubstance2 -> substanceName);
}

int compareMassNumber(const void *pSubstanceVoid1, const void *pSubstanceVoid2){
    const Substance *pSubstance1 = (const Substance *)pSubstanceVoid1;
    const Substance *pSubstance2 = (const Substance *)pSubstanceVoid2;
    if (pSubstance1 -> substanceMassNumber == pSubstance2 -> substanceMassNumber){
        return 0;
    }
    if (pSubstance1 -> substanceMassNumber > pSubstance2 -> substanceMassNumber){
        return 1;
    }
    if (pSubstance1 -> substanceMassNumber < pSubstance2 -> substanceMassNumber){
        return -1;
    } 
}

int compareHalftime(const void *pSubstanceVoid1, const void *pSubstanceVoid2){
    const Substance *pSubstance1 = (const Substance *)pSubstanceVoid1;
    const Substance *pSubstance2 = (const Substance *)pSubstanceVoid2;
    if (pSubstance1 -> substanceDecayTime == pSubstance2 -> substanceDecayTime){
        return 0;
    }
    if (pSubstance1 -> substanceDecayTime > pSubstance2 -> substanceDecayTime){
        return 1;
    }
    if (pSubstance1 -> substanceDecayTime < pSubstance2 -> substanceDecayTime){
        return -1;
    }
}

void sortSubstances(Substance aSubstance[], int size){
    int ans = 0;
    printf("Sortera amnen\n");
    printf("Ville du soka pa namn (1), masstal (2), halveringstid (3): ");
    scanf("%d", &ans);
    switch (ans){
        case 1 : qsort(aSubstance, size, sizeof(Substance), compareName); break;
        case 2 : qsort(aSubstance, size, sizeof(Substance), compareMassNumber); break;
        case 3 : qsort(aSubstance, size, sizeof(Substance), compareHalftime); break;
        default: break;
    }
}

void searchSubstanceName(Substance aSubstance[], Substance aFoundSubstance[], int size, int *pNewSize, int *pSubstancePlace){ //done
    char name[NAMESIZE]; 
    *pNewSize = 0;
    
    printf("Ange sokstrang: ");
    scanf("%s", name);
    for (int i = 0; i < size; i++){
        if (strstr(aSubstance[i].substanceName, name) != NULL){
            aFoundSubstance[(*pNewSize)] =  initNewSubstance(aSubstance[i].substanceName, 
                                                            aSubstance[i].substanceMassNumber,
                                                            aSubstance[i].substanceDecayTime);
            (*pNewSize)++;              
        }
    }
    for (int i = 0; i < size; i++){
        if (strstr(aSubstance[i].substanceName, aFoundSubstance[0].substanceName) != NULL){
            (*pSubstancePlace) = i;
        }
    }
    viewAllSubstances(aFoundSubstance, *pNewSize); 
} 

void searchSubstanceMassNumber(Substance aSubstance[], Substance aFoundSubstance[], int size, int *pNewSize, int *pSubstancePlace){ //done
    int massNumber = 0;
    *pNewSize = 0;  
    printf("Ange masstal: ");
    scanf("%d", &massNumber);
    for (int i = 0; i < size; i++){
        if (aSubstance[i].substanceMassNumber == massNumber){
            aFoundSubstance[(*pNewSize)] =  initNewSubstance(aSubstance[i].substanceName, 
                                                            aSubstance[i].substanceMassNumber,
                                                            aSubstance[i].substanceDecayTime);
            (*pNewSize)++;              
        }
    }
    for (int i = 0; i < size; i++){
        if (strstr(aSubstance[i].substanceName, aFoundSubstance[0].substanceName) != NULL){
            (*pSubstancePlace) = i;
        }
    }
    viewAllSubstances(aFoundSubstance, *pNewSize);
}

void searchSubstanceDecayTime(Substance aSubstance[], Substance aFoundSubstance[], int size, int *pNewSize, int ifInChangeHalftime, int *pSubstancePlace){ //done
    double minDecayTime = 0;
    double maxDecayTime = 0;
    *pNewSize = 0;
    if(ifInChangeHalftime == 0){
        printf("Ange minsta halveringstid: ");
        scanf("%lf", &minDecayTime);
        printf("Ange storsta halveringstid: ");
        scanf("%lf", &maxDecayTime);
        for (int i = 0; i < size; i++){
            if ((minDecayTime <= aSubstance[i].substanceDecayTime) && (aSubstance[i].substanceDecayTime <= maxDecayTime)){
                aFoundSubstance[(*pNewSize)] = initNewSubstance(aSubstance[i].substanceName, 
                                                                aSubstance[i].substanceMassNumber,
                                                                aSubstance[i].substanceDecayTime);
                (*pNewSize)++;              
            }
        }
    } else {
        printf("Ange halveringstid: ");
        scanf("%lf", &minDecayTime);
        for (int i = 0; i < size; i++){
            if (minDecayTime == aSubstance[i].substanceDecayTime){
                aFoundSubstance[(*pNewSize)] = initNewSubstance(aSubstance[i].substanceName, 
                                                                aSubstance[i].substanceMassNumber,
                                                                aSubstance[i].substanceDecayTime);
                (*pNewSize)++;
            }
        }
    }
    for (int i = 0; i < size; i++){
        if (strstr(aSubstance[i].substanceName, aFoundSubstance[0].substanceName) != NULL){
            (*pSubstancePlace) = i;
        }
    }
    viewAllSubstances(aFoundSubstance, *pNewSize);
}

void searchSubstance(Substance aSubstance[], Substance aFoundSubstance[], int size, int *pNewSize, int *pSubstancePlace){ //done
    int ans = 0; 
    do{
        printf("Hur vill du soka\n");
        printf("(1) namn\n");
        printf("(2) masstal\n");
        printf("(3) halveringstid\n");
        printf("(4) huvudmeny\n");
        printf("Ange val: ");
        scanf("%d", &ans);
        switch (ans) {
            case 1 : searchSubstanceName(aSubstance, aFoundSubstance, size, pNewSize, pSubstancePlace); break;
            case 2 : searchSubstanceMassNumber(aSubstance, aFoundSubstance, size, pNewSize, pSubstancePlace); break;
            case 3 : searchSubstanceDecayTime(aSubstance, aFoundSubstance, size, pNewSize, 0, pSubstancePlace); break;
            default : break;
        }
    } while (ans != 4);
}

void changeHalftimeWithName(Substance aSubstance[], Substance aFoundSubstance[], Substance aSearchResult[], int size, int *pNewSize, int *pSubstancePlace, int *pFoundSubstancePlace){ //done
    searchSubstanceName(aSubstance, aFoundSubstance, size, pNewSize, pSubstancePlace);
    for (int i = 0; i < size; i++){
        if (strstr(aSubstance[i].substanceName, aFoundSubstance[0].substanceName) != NULL){
            (*pSubstancePlace) = i;
            if ((*pNewSize) > 1){
                searchSubstanceMassNumber(aSubstance, aFoundSubstance, size, pNewSize, pSubstancePlace);
                for (int j = 0; j < (*pNewSize); j++){
                    if (strstr(aSubstance[j].substanceName, aFoundSubstance[j].substanceName) != NULL){
                        (*pFoundSubstancePlace) = j;
                        aSearchResult[0] = initNewSubstance(aFoundSubstance[(*pFoundSubstancePlace)].substanceName, 
                                                        aFoundSubstance[(*pFoundSubstancePlace)].substanceMassNumber,
                                                        aFoundSubstance[(*pFoundSubstancePlace)].substanceDecayTime);
                        (*pNewSize) = 1;
                        viewAllSubstances(aSearchResult, (*pNewSize));
                    }
                }
            } else {
                aSearchResult[0] = initNewSubstance(aSubstance[(*pSubstancePlace)].substanceName, 
                                                   aSubstance[(*pSubstancePlace)].substanceMassNumber,
                                                   aSubstance[(*pSubstancePlace)].substanceDecayTime);
            }
        }
    }       
}

void changeHalftimeWithMassNumber(Substance aSubstance[], Substance aFoundSubstance[], Substance aSearchResult[], int size, int *pNewSize, int *pSubstancePlace, int *pFoundSubstancePlace){ //done
    searchSubstanceMassNumber(aSubstance, aFoundSubstance, size, pNewSize, pSubstancePlace);
    for (int i = 0; i < size; i++){
        if (strstr(aSubstance[i].substanceName, aFoundSubstance[0].substanceName) != NULL){
            (*pSubstancePlace) = i;
            if ((*pNewSize) > 1){
                for (int j = 0; j < (*pNewSize); j++){
                    if (strstr(aSubstance[j].substanceName, aFoundSubstance[j].substanceName) != NULL){
                        (*pFoundSubstancePlace) = j;
                        aSearchResult[0] = initNewSubstance(aFoundSubstance[(*pFoundSubstancePlace)].substanceName, 
                                                        aFoundSubstance[(*pFoundSubstancePlace)].substanceMassNumber,
                                                        aFoundSubstance[(*pFoundSubstancePlace)].substanceDecayTime);
                        (*pNewSize) = 1;
                        viewAllSubstances(aSearchResult, (*pNewSize));
                    }
                }
            }
        }
    }       
}

void changeHalftimeWithDecayTime(Substance aSubstance[], Substance aFoundSubstance[], Substance aSearchResult[], int size, int *pNewSize, int *pSubstancePlace, int *pFoundSubstancePlace){ //done
    searchSubstanceDecayTime(aSubstance, aFoundSubstance, size, pNewSize, 1, pSubstancePlace);
    for (int i = 0; i < size; i++){
        if (strstr(aSubstance[i].substanceName, aFoundSubstance[0].substanceName) != NULL){
            (*pSubstancePlace) = i;
            if ((*pNewSize) > 1){
                for (int j = 0; j < (*pNewSize); j++){
                    if (strstr(aSubstance[j].substanceName, aFoundSubstance[j].substanceName) != NULL){
                        (*pFoundSubstancePlace) = j;
                        aSearchResult[0] = initNewSubstance(aFoundSubstance[(*pFoundSubstancePlace)].substanceName, 
                                                        aFoundSubstance[(*pFoundSubstancePlace)].substanceMassNumber,
                                                        aFoundSubstance[(*pFoundSubstancePlace)].substanceDecayTime);
                        (*pNewSize) = 1;
                        viewAllSubstances(aSearchResult, (*pNewSize));
                    }
                }
            }
        }
    }       
}

void changeHalfTime(Substance aSubstance[], Substance aFoundSubstance[], int size, int *pNewSize, int *pSubstancePlace, int *pFoundSubstancePlace){ // done
    int ans = 0;
    double newDecayTime = 0;
    Substance searchResult[DATASIZE];
    char name [NAMESIZE];

    printf("Andra halveringstid\n");
    printf("Ville du soka pa namn (1), masstal (2), halveringstid (3): ");
    scanf("%d", &ans); 
    do{
        switch (ans){
            case 1 : changeHalftimeWithName(aSubstance, aFoundSubstance, searchResult, size, pNewSize, pSubstancePlace, pFoundSubstancePlace); break;
            case 2 : changeHalftimeWithMassNumber(aSubstance, aFoundSubstance, searchResult, size, pNewSize, pSubstancePlace, pFoundSubstancePlace); break;
            case 3 : changeHalftimeWithDecayTime(aSubstance, aFoundSubstance, searchResult, size, pNewSize, pSubstancePlace, pFoundSubstancePlace); break;
            default : break;
        }
        if(*pNewSize != 1){
            printf("Du fick inte exakt ett resultat.\n");
        }
    } while(*pNewSize != 1);
    printf("Ange ny halveringstid: ");
    scanf("%lf", &newDecayTime);
    aSubstance[(*pSubstancePlace)] = initNewSubstance(aFoundSubstance[(*pFoundSubstancePlace)].substanceName, 
                                                  aFoundSubstance[(*pFoundSubstancePlace)].substanceMassNumber,
                                                  newDecayTime);
}

void removeSubstance(Substance aSubstance[], Substance aFoundSubstance[], int *pSize, int *pNewSize, int *pSubstancePlace){ //done
    int ans = 0;
    int totalFound = 0;
    printf("Avregistrera amne\n");
    printf("Ville du soka pa namn (1), masstal (2), halveringstid (3): ");
    scanf("%d", &ans);
    switch (ans) {
        case 1 : searchSubstanceName(aSubstance, aFoundSubstance, (*pSize), pNewSize, pSubstancePlace); break;
        case 2 : searchSubstanceMassNumber(aSubstance, aFoundSubstance, (*pSize), pNewSize, pSubstancePlace); break;
        case 3 : searchSubstanceDecayTime(aSubstance, aFoundSubstance, (*pSize), pNewSize, 1, pSubstancePlace); break;
        default : break;
    }
    for (int i = 0; i < (*pNewSize); i++){
        if (aFoundSubstance[i].substanceMassNumber > 0){
            totalFound++;
        }
    }
    if (totalFound > 1){  
        printf("Specificera sokning (Välj något anna)t.\n");
        printf("Ville du soka pa namn (1), masstal (2), halveringstid (3): ");
        scanf("%d", &ans);
        switch (ans){
            case 1 : searchSubstanceName(aSubstance, aFoundSubstance, (*pSize), pNewSize, pSubstancePlace); break;
            case 2 : searchSubstanceMassNumber(aSubstance, aFoundSubstance, (*pSize), pNewSize, pSubstancePlace); break;
            case 3 : searchSubstanceDecayTime(aSubstance, aFoundSubstance, (*pSize), pNewSize, 1, pSubstancePlace); break;
            default : break;
        }  
    }
    (*pSize)--;
    for ((*pSubstancePlace); (*pSubstancePlace) < (*pSize); (*pSubstancePlace)++){
        aSubstance[(*pSubstancePlace)] = initNewSubstance(aSubstance[(*pSubstancePlace) + 1].substanceName, 
                                                        aSubstance[(*pSubstancePlace) + 1].substanceMassNumber,
                                                        aSubstance[(*pSubstancePlace) + 1].substanceDecayTime);
    }
}

void saveSubstances(Substance aSubstance[], char aFileName[], int size){ //done
    FILE *pFile;
    if((pFile = fopen(aFileName, "w"))){
        for (int i = 0; i < size; i++){
            fprintf(pFile, "%s ", aSubstance[i].substanceName);
            fprintf(pFile, "%d ", aSubstance[i].substanceMassNumber);
            fprintf(pFile, "%lf ", aSubstance[i].substanceDecayTime);
        }
    }
    fclose(pFile);    
}