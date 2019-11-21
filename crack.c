#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN=50;        // Maximum any password can be
const int HASH_LEN=33;        // Length of MD5 hash strings


// Stucture to hold both a plaintext password and a hash.
struct entry 
{
    char word[PASS_LEN];
    char *hashed_word;
};
// function to compare entries for qsort
int entryCompare(const void *a, const void *b){
    struct entry * aa=(struct entry*)a;
    struct entry* bb=(struct entry*)b;
    return(strcmp((*aa).hashed_word,(*bb).hashed_word));
}
//function to compare entries for b search
int entryBcompare(const void * target, const void * elem){
    char * targetString=(char*)target;
    struct entry * elemEntry=(struct entry *)elem;
    return(strcmp(targetString,elemEntry->hashed_word));
}

// Read in the dictionary file and return an array of structs.
// Each entry should contain both the hash and the dictionary
// word.
struct entry *read_dictionary(char *filename, int *size)
{
    int count=0;
    int lines=50;
    char line[1000];
    FILE *fread=fopen(filename, "r");
    struct entry *dict=malloc(lines*sizeof(struct entry));
    while (fgets(line,1000,fread)!=NULL){
        if (count==lines){
            lines+=50;
            dict=realloc(dict,lines*sizeof(struct entry));
        }
        line[strlen(line)-1]='\0'; //strip new line character before assignment and hashing;
        strcpy(dict[count].word,line);
        dict[count].hashed_word=md5(dict[count].word,strlen(dict[count].word));
        count++;
    }
    fclose(fread);
    *size = count;
    return dict;
}

int main(int argc, char *argv[])
{
    //check for enough arguments
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

   
    int dlen;
    //open and store dictionary
    struct entry *dict = read_dictionary(argv[2], &dlen);

    //Sort dict before doing a bsearch
    qsort(dict, dlen, sizeof(struct entry), entryCompare);

    // Open the hash file for reading.
    FILE * hashes=fopen(argv[1],"r");
    char line[1000];
    struct entry * found;
    // TODO
    // For each hash, search for it in the dictionary using
    // binary search.
    // If you find it, get the corresponding plaintext dictionary word.
    // Print out both the hash and word.
    // Need only one loop. (Yay!)
  
    while(fgets(line,1000,hashes)!=NULL){
        line[strlen(line)-1]='\0'; //strip new line character  
        found=bsearch(line,dict,dlen,sizeof(struct entry),entryBcompare); //search through using binary search
        if(found!=NULL){ //if found print
            printf("%s %s\n",found->word,found->hashed_word);
        }
        
    }
    for(int i=0;i<dlen;i++){
        free(dict[i].hashed_word);
    }
    free(dict); 
    fclose(hashes); 
}
