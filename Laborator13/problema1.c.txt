#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_VOWELS 10
char vowels[NUM_VOWELS] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};

int vowelCount = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* count_vowels(void* arg) {
    char* word = (char*)arg;
    int count = 0;
    
    for (int i = 0; i < strlen(word); i++) {
        for (int j = 0; j < NUM_VOWELS; j++) {
            if (word[i] == vowels[j]) {
                count++;
                break;
            }
        }
    }
    
    pthread_mutex_lock(&mutex);
    vowelCount += count;
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s word1 word2 ... wordN\n", argv[0]);
        return 1;
    }
    
    int num_words = argc - 1;
    pthread_t threads[num_words];

    for (int i = 0; i < num_words; i++) {
        if (pthread_create(&threads[i], NULL, count_vowels, argv[i + 1]) != 0) {
            perror("Failed to create thread");
            return 2;
        }
    }
    
    for (int i = 0; i < num_words; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 3;
        }
    }
    
    printf("Total number of vowels: %d\n", vowelCount);

    pthread_mutex_destroy(&mutex);
    return 0;
}
