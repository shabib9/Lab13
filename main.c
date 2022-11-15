//  bestWordle.c
//  Author: Taha Habibullah
//
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/
//

#include <stdio.h>
#include <stdlib.h>   // for exit( -1)
#include <string.h>

// struct to contain the word and score pairs
struct wordCountStruct {
  int score;
  char* word;
};

//-----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  Parameters are declared to be a
// generic type, so they will match with anything.
// This is a two-part comparison.  First the scores are compared.  If they are the same,
// then the words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction( const void * a, const void * b) {
    // Before using parameters we have cast them into the actual type they are in our program
    // and then extract the numerical value used in comparison
    int firstScore = ((struct wordCountStruct*)a)->score;
    int secondScore = ((struct wordCountStruct*)b)->score;

    // If scores are different, then that's all we need for our comparison.
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        // Scores are equal, so check words themselves, to put them in alphabetical order
	  // Hint: return the value of strcmp( parameter a word,  parameter b word)
        return strcmp(((struct wordCountStruct*)a)->word, ((struct wordCountStruct*)b)->word);
    }
} //end compareFunction(..)


    // ...
    // Sort the allWords array in descending order by score, and within score they should also
    // be sorted into ascending order alphabetically.  Use the built-in C quick sort qsort(...).
    // The last parameter is the name of the comparison function we use (a function pointer).
    //
    // ... uncomment out the line below once you have it in a meaningful context in your code.
    // qsort( theArray, numElementsInArray, sizeof( your struct), compareFunction);



// -----------------------------------------------------------------------------------------

// frees the dynamically allocated words array
void freeArr(char** words) {
  for(int i = 0; i < sizeof(words); i++)
    free(words[i]);
  free(words);
}

// gets the score count for each word after the answers have been iterated over by the top word
struct wordCountStruct* getSecondScores(char** words, char* topWord, int size, int aSize) {
  // dynamically allocates memory for the struct of second word and score pairs
  struct wordCountStruct* second = malloc(sizeof(struct wordCountStruct) * size);
  // keeps track of the score
  int s = 0;
  // contains the result of the answers being iterated over by the top word
  char** frames = malloc(sizeof(char*) * aSize);
  // holds each subframe for the checks
  char temp[6];
  // holds the char when a match is found
  char c;
  // initializes the frame array
  for(int i = 0; i < aSize; i++)
    frames[i] = malloc(sizeof(char) * 6);
  // iterates the top word over the answers
  for(int i = 0; i < aSize; i++) {
    // copies the current subframe to temp
    strcpy(temp, words[i]);
    for(int j = 0; j < 5; j++) {
      // if a match is found, the character is blanked out
      if(topWord[j] == temp[j])
        temp[j] = ' ';
      // if a match is not found, a check is made to see if the character
      // exists in the frame at all
      else if(strchr(temp, topWord[j]) != NULL) 
        temp[(int)((strchr(temp, topWord[j])-temp))] = ' ';
    }
    // copies the blanked out frame to the frames array
    strcpy(frames[i], temp);
  }
  // goes through each word, scoring them
  for(int i = 0; i < size; i++) {
    // assigns the word member of the struct
    second[i].word = words[i];
    for(int j = 0; j < aSize; j++) {
      // copies the blanked out frames from before back to temp
      strcpy(temp, frames[j]);
      for(int k = 0; k < 5; k++) {
        // if the words are exactly the same, the whole word is blanked out
        if(words[i] == frames[j]) {
          for(int l = 0; l < 5; l++)
            temp[l] = ' ';
          // score is incremented by 15 since 3*5=15
          s+=15;
          // breaks out of loop since this frame is complete
          break;
        }
        // if a match is found, the character is blanked out
        if(words[i][k] == temp[k]) {
          c = temp[k];
          for(int l = 0; l < 5; l++) {
            if(temp[l] == c)
              temp[l] = ' ';
          }
          // score is incremented by 3 for a full match
          s+=3;
        }
      }
      // checks if the character is present in the frame at all
      for(int k = 0; k < 5; k++) {
        // if it is, the score is incremented by 1 and the character is blanked out
        if(strchr(temp, words[i][k]) != NULL) {
          s++;
          temp[(int)((strchr(temp, words[i][k])-temp))] = ' ';
        }
      }
    }
    // assigns the score member of the struct
    second[i].score = s;
    // sets the score to 0 for the next iteration
    s = 0;
  }
  // returns the dynamically allocated struct array
  return second;
}

struct wordCountStruct* getFirstScores(char** words, int size, int aSize) {
  // dynamically allocates memory for the struct of second word and score pairs
  struct wordCountStruct* wordScores = malloc(sizeof(struct wordCountStruct) * size);
  // keeps track of the score
  int s = 0;
  // contains the result of the answers being iterated over by the top word
  char c;
  // holds each frame for the checks
  char temp[6];
  // goes through each word, scoring them
  for(int i = 0; i < size; i++) {
    // assigns the word member of the struct
    wordScores[i].word = words[i];
    for(int j = 0; j < aSize; j++) {
      // copies each answer frame to the temp cstring
      strcpy(temp, words[j]);
      for(int k = 0; k < 5; k++) {
        // if the words are exactly the same, the whole word is blanked out
        if(words[i] == words[j]) {
          for(int l = 0; l < 5; l++)
            temp[l] = ' ';
          // score is incremented by 15 since 3*5=15
          s+=15;
          // breaks out of the loop since the frame is complete
          break;
        }
        // if a match is found, the character is blanked out
        if(words[i][k] == temp[k]) {
          c = temp[k];
          for(int l = 0; l < 5; l++) {
            if(temp[l] == c)
              temp[l] = ' ';
          }
          // score is incremented by 3 for a full  match
          s+=3;
        }
      }
      // checks if the character is present in the frame at all
      for(int k = 0; k < 5; k++) {
        // if it is, the score is incremented by 1 and the character is blanked out
        if(strchr(temp, words[i][k]) != NULL) {
          s++;
          temp[(int)((strchr(temp, words[i][k])-temp))] = ' ';
        }
      }
    }
    // assigns the score member of the struct
    wordScores[i].score = s;
    // score is set to 0 for the next iteration
    s = 0;
  }
  // returns the dynamically allocated struct array
  return wordScores;
}

int main() {
    char answersFileName[81] = "answersTiny.txt"; // Stores the answers file name
    char guessesFileName[81] = "guessesTiny.txt";  // Stores the guesses file name
    // size counter for the words array
    int aCount = 0;

    printf("Default file names are %s and %s\n", answersFileName, guessesFileName);

    // Display menu, to allow partial credit for different program components
    int menuOption = 0;
    do {
        printf("\n");
        printf("Menu Options:\n");
        printf("  1. Display best first words only\n");
        printf("  2. Display best first and best second words\n");
        printf("  3. Change answers and guesses filenames\n");
        printf("  4. Exit\n");
        printf("Your choice: ");
        scanf("%d", &menuOption);

        // Handle menu option 3 to exit the program
        if (menuOption == 4) {
            exit(1); // Exit the program
        } else if (menuOption == 3) {
            // Change file names.  Menu will then be redisplayed.
            printf("Enter new answers and guesses filenames: ");
            scanf("%s %s", answersFileName, guessesFileName);

        }
    } while( menuOption == 3);

    // Sample code to read in from a file
    FILE *inFilePtrA  = fopen(answersFileName, "r");  // Connect logical name to filename
    char *inputStringA[ 81];
    FILE *inFilePtrG  = fopen(guessesFileName, "r");  // Connect logical name to filename
    char inputStringG[ 81];

    // Sanity check: ensure file open worked correctly
    if( inFilePtrA == NULL ) {
        printf("Error: could not open %s for reading\n", answersFileName);
        exit(-1);    // must include stdlib.h
    }
    // dynamically allocated 2d array to hold all the words
    char** words = malloc(sizeof(char*));
    // reads in each word from the answers text file
    while( fscanf(inFilePtrA, "%s", inputStringA) != EOF) {
      aCount++;
      // copies the current line from the file to the newA cstring
      char* newA = malloc(sizeof(char) * 6);
      strcpy(newA, inputStringA);
      // resizes the words array as needed
      words = realloc(words, sizeof(char*) * aCount);
      words[aCount-1] = malloc(sizeof(char) * 6);
      // inserts the word into the array
      words[aCount-1] = newA;
    }
    // outputs the amount of words in the answers file
    printf("%s has %d words\n", answersFileName, aCount);
    // closes the file
    fclose(inFilePtrA);

    // total size of the words array
    int size = aCount;
    // reads in each word from the guesses text file
    while( fscanf(inFilePtrG, "%s", inputStringG) != EOF) {
      size++;
      // copies the current line from the file to the newG cstring
      char* newG = malloc(sizeof(char) * 6);
      strcpy(newG, inputStringG);
      // resizes the words array as needed
      words = realloc(words, sizeof(char*) * size);
      words[size-1] = malloc(sizeof(char) * 6);
      // inserts the word into the array
      words[size-1] = newG;
    }
    // outputs the amount of words in the answers file
    printf("%s has %d words\n\n", guessesFileName, size-aCount);
    // closes the file
    fclose(inFilePtrG);

    // gets the first scores for each word
    struct wordCountStruct* wordScores = getFirstScores(words, size, aCount);
    // sorts the struct array
    qsort(wordScores, size, sizeof(struct wordCountStruct), compareFunction);

    
    int i = 0;
    // outputs the results for the first words
    if(menuOption == 1) {
      printf("Words and scores for top first words:\n");
      while(wordScores[i].score == wordScores[0].score) {
        printf("%s %d\n", wordScores[i].word, wordScores[i].score);
        i++;
      }
    }
    // outputs the results for the first and second words
    else if(menuOption == 2) {
      printf("Words and scores for top first words and second words:\n");
      while(wordScores[i].score == wordScores[0].score && menuOption == 2) {
      printf("%s %d\n", wordScores[i].word, wordScores[i].score);
      // gets the second scores for each word
      struct wordCountStruct* secondScores = getSecondScores(words, wordScores[i].word, size, aCount);
      // sorts the struct array
      qsort(secondScores, size, sizeof(struct wordCountStruct), compareFunction);
      int j = 0;
        // outputs the results for the second words
        while(secondScores[j].score == secondScores[0].score) {
          printf("  %s %d", secondScores[j].word, secondScores[j].score);
          j++;
        }
        printf("\n");
        i++;
      }
    }
    // frees the dynamically allocated words array
    freeArr(words);
    printf("Done\n");
    return 0;
} // end main()

