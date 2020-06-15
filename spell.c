//CS-GY 9163 Application Security//
///////////////////////////////////
//////Assignment 1 - spell.c///////
///////////////////////////////////
//////////Tianxiang Chen///////////
///////////////////////////////////
//new

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"


/**
 * Returns true if word is in dictionary else false.
 */
/**
 * Inputs:
 *  word:       A word to check the spelling of.
 *  hashtable:  The hash table used to determine spelling
 *
 * Returns:
 *  bool:       A boolean value indicating if the word was correctly spelled.
 *
 * Modifies:
 *
 * Example:
 *  bool correct  = check_word(word, hashtable);
 **/
bool check_word(const char* word, hashmap_t hashtable[]) {
    //printf("%s\n", word);
 
    int hash_val = 0;
    //int charLength = strlen(word);

    hash_val = hash_function(word);
    //printf("hash value is %d\n", hash_val);
    hashmap_t new_node = hashtable[hash_val];
    //printf("word is %s\n", new_node->word);
    while (new_node != NULL) {
        if (strcmp(new_node->word, word) == 0) {
            return true;
        } else {
            new_node = new_node->next;
        }
    }
    return false;
}



// Load the dictionary file into the memory if such dictionary exists
/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
/**
 * Inputs:
 *  dictionary_file:    Path to the words file.
 *  hashtable:          The hash table to be populated.
 *
 * Returns:
 *  bool:       Whether or not the hashmap successfully populated.
 *
 * Modifies:
 *  hashtable: This hashmap should be filled with words from the file provided.
 *
 * Example:
 *  bool success = load_dictionary("wordlist.txt", hashtable);
 **/
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){

    char *line = NULL;
    size_t length = 0;
    ssize_t nextWordSize = -1;
    int hashValue = 0;


    for(int t = 0; t< HASH_SIZE; t++){
        hashtable[t] = NULL;
    }
    if (hashtable == NULL || dictionary_file == NULL){
        printf("Hashtable or Dictionary File is NULL.");
        return false;
    }

    FILE *fp = fopen(dictionary_file, "r");
    if (fp == NULL){
        printf("Dictionary file does not exist.\n");
        return false;
    }

    while ((nextWordSize = getline(&line, &length, fp)) != -1 ){
        if (nextWordSize > LENGTH){
            printf("Cannot handle words that are too long.\n");
            continue;
        }
  
        for (int j = 0; j < strlen(line); j++) {
            line[j] = tolower(line[j]);
        }
        line[strlen(line)-1] = '\0';

        hashValue = hash_function(line);
        hashmap_t newNode = (hashmap_t)malloc(sizeof(node));
        //newNode->next = NULL;
        strcpy(newNode->word, line);
        if(hashtable[hashValue] != NULL){
             newNode->next = hashtable[hashValue];
             hashtable[hashValue] = newNode;
        }else{
        newNode->next = NULL;
        hashtable[hashValue] = newNode;
        //newNode = newNode->next;

       }
    }
    
    free(line);
    fclose(fp);
    return true;

}

/**
 * Array misspelled is populated with words that are misspelled. Returns the length of misspelled.
 */
/**
 * Inputs:
 *  fp:         A file pointer to the document to check for spelling errors.
 *  hashtable:  The hash table used to determine spelling
 *  misspelled: An empty char* array to be populated with misspelled words.
 *              This array will never be greater than 1000 words long.
 *
 * Returns:
 *  int:        The number of words in the misspelled arary.
 *
 * Modifies:
 *  misspelled: This array will be filled with misspelled words.
 *
 * Example:
 *  int num_misspelled = check_words(text_file, hashtable, misspelled);
 **/
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {

    int num_misspelled = 0;
    char *string = NULL;
    char *word = NULL;
    
    //char *tem = NULL;
    size_t string_length = 0;
    ssize_t wordNum = -1;
    char delim[] = " ";

    if (fp == NULL) {
        printf("No such file exists.\n");
        exit(0);
    }

    while ((wordNum = getline(&string, &string_length, fp) != -1)) {
        word = strtok(string, delim);
        while (word != NULL) {
            if (word[strlen(word) - 1] == '\n') {
                word[strlen(word) - 1] = '\0';
            }

            //Continue to get the next word if the current word is too long
            if (strlen(word) > LENGTH) {
                printf("The word length is invalid.\n");
                word = strtok(NULL, delim);
                continue;
            }

            //remove punctuations in word into tem for comparison
            //the code is adjusted from stack overflow
            char *tem = malloc(sizeof(char) * (LENGTH + 1));
            int aux = 0;
            int temLen = 0;
            int dot = 0;
            while (word[aux] != '\0') {
                if (ispunct(word[aux])) {
                    //Fractional number is considered valid, but only one dot is
                    //allowed and the dot cannot be at index of 0. 
                    //Also, for numeric string starting with number 0, the dot has
                    //to be at index of 1. 
                    //eg. 0.12 is valid, but 01.2 is not valid.
                    if(word[aux] == '.' && aux != (strlen(word)-1) && (word[0] != '0'|| (word[0] == '0'&& word[1] == '.'))){
                        dot++;
                    }
                    aux++;
                    //continue;
                } else if (isupper(word[aux])) {
                    tem[temLen] = tolower(word[aux]);
                    aux++;
                    temLen++;
                    //continue;
                } else {
                    tem[temLen] = word[aux];
                    aux++;
                    temLen++;
                }
            }
            

            int digitNum = 0;
            for (int y = 0; y < strlen(tem); y++) {
                if (!isdigit(tem[y])) {
                    break;
                } else {
                    digitNum++;
                }
            }

            //tem is numeric then it is considered spelled correctly
            //even if it is not in the hashtable
            if (digitNum == strlen(word) || (digitNum == (strlen(word)-1) && dot == 1)) {
                word = strtok(NULL, delim);
            } else {
                int result = check_word(tem, hashtable);
                if (result == false) {
                    misspelled[num_misspelled] = malloc(strlen(tem));
                    strcpy(misspelled[num_misspelled], word);
                    num_misspelled++;
                    if (num_misspelled >= MAX_MISSPELLED) {

                        return num_misspelled;
                    }
                }
                word = strtok(NULL, delim);
            }
        }
    }
    free(word);
    return num_misspelled;
}
