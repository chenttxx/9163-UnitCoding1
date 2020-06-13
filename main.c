#include <stdlib.h>

#include "dictionary.h"


void cleanup_mem(hashmap_t hashtable[], char* misspelled[])
{
    for (int i = 0; i < MAX_MISSPELLED; i++) free(misspelled[i]);


    for (int i = 0; i < HASH_SIZE; i++)
    {
        node* check_node = hashtable[i];

        while (check_node != NULL)
        {
            node* node_to_free = check_node;
            check_node = check_node->next;
            if (node_to_free != NULL) free(node_to_free);
        }
    }

}


int main(int argc, char* argv[]) 
{

    if (argc != 3)
    {
        printf("This program takes two arguments - please provide wordlist and text to check.\n");
        exit(EXIT_FAILURE);
    }

    hashmap_t hashtable[HASH_SIZE];
    
    load_dictionary(argv[1], hashtable);
/*for(int k = 0; k<HASH_SIZE; k++){
    if(hashtable[k] != NULL){
         printf("The word is: %s\n" , hashtable[k]->word);
         printf("The location is: %d\n", k);
}
}*/

    char *misspelled[MAX_MISSPELLED];
    for (int i = 0; i < MAX_MISSPELLED; i++) misspelled[i] = NULL;


    FILE *fp = fopen(argv[2], "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    for (int i = 0; i < num_misspelled; i++) 
    {
       printf("misspelled: %s\n", misspelled[i]);
    }
    printf("Mispelled: %d\n", num_misspelled);

    fclose(fp);

    cleanup_mem(hashtable, misspelled);

    return 0;
}

