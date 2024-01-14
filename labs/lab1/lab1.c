/*********************************************************************
 * Aiden Pratt CS 6013 1/8/24
 * File header comments go here... including student name, class, date,
 * assignment, description, etc.
 * used a classmates test from Lindsay Haslam
 */
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void bubbleSort(unsigned char *units, int numUnits);
unsigned char extractByte(unsigned long arg, int bytePos);

/*********************************************************************
 *
 * The C functions in this lab use patterns and functionality often found in
 * operating system code. Your job is to implement them.  Additionally, write some test
 * cases for each function (stick them in functions [called xyzTests(), etc or similar])
 * Call your abcTests(), etc functions in main().
 *
 * Write your own tests for each function you are testing, then share/combine
 * tests with a classmate.  Try to come up with tests that will break people's code!
 * Easy tests don't catch many bugs! [Note this is one specific case where you may
 * share code directly with another student.  The test function(s) from other students
 * must be clearly marked (add '_initials' to the function name) and commented with the
 * other student's name.
 *
 * Note: you may NOT use any global variables in your solution.
 *
 * Errata:
 *   - You can use global vars in your testing functions (if necessary).
 *   - Don't worry about testing the free_list(), or the draw_me() functions.
 *
 * You must compile in C mode (not C++).  If you compile from the commandline
 * run clang, not clang++. Submit your solution files on Canvas.
 *
 *********************************************************************/

/*********************************************************************
 *
 * byte_sort()
 *
 * specification: byte_sort() treats its argument as a sequence of
 * 8 bytes, and returns a new unsigned long integer containing the
 * same bytes, sorted numerically, with the smaller-valued bytes in
 * the lower-order byte positions of the return value
 *
 * EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
 * Ah, the joys of using bitwise operators!
 *
 * Hint: you may want to write helper functions for these two functions
 *
 *********************************************************************/

unsigned long byte_sort( unsigned long arg )
{
    //declaring a char array to store 8 bytes
    unsigned char bytes[8];
    //iterate through to store in bytes array
    for (int i = 0; i < 8; i++){
        bytes[i] = extractByte(arg, i);
    }

    //sorting using bubble sort
    bubbleSort(bytes, 8);

    unsigned long result = 0;
    for(int i = 0; i < 8; i++){
        result |= ((unsigned long )bytes[i]<<(i*8));
    }
    return result;
}

unsigned char extractByte(unsigned long arg, int bytePos) {
    return (arg >> (bytePos * 8)) & 0xFF;
}


/*********************************************************************
 *
 * nibble_sort()
 *
 * specification: nibble_sort() treats its argument as a sequence of 16 4-bit
 * numbers, and returns a new unsigned long integer containing the same nibbles,
 * sorted numerically, with smaller-valued nibbles towards the "small end" of
 * the unsigned long value that you return
 *
 * the fact that nibbles and hex digits correspond should make it easy to
 * verify that your code is working correctly
 *
 * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
 *
 *********************************************************************/

unsigned long nibble_sort (unsigned long arg)
{
    //declaring array of 16 nibbles which are 4 bits each
    unsigned char nibbles[16];

    //iterating through each nibble of the input arg
    for (int i = 0; i < 16; i++){
        nibbles[i] = (arg >> (i * 4)) & 0xF;
    }

    //sorting
    bubbleSort(nibbles, 16);

    unsigned long result = 0;

    for(int i = 0; i < 16; i++){
        //use bitwise OR to combine sorted nibbles
        result |= ((unsigned long)nibbles[i] << (i * 4));
    }

    return result;
}

/*********************************************************************/

typedef struct elt {
    char val;
    struct elt *link;
} Elt;

/*********************************************************************/

/* Forward declaration of "free_list()"... This allows you to call   */
/* free_list() in name_list() [if you'd like].                       */
void free_list( Elt* head ); // [No code goes here!]

/*********************************************************************
 *
 * name_list()
 *
 * specification: allocate and return a pointer to a linked list of
 * struct elts
 *
 * - the first element in the list should contain in its "val" field the first
 *   letter of your first name; the second element the second letter, etc.;
 *
 * - the last element of the linked list should contain in its "val" field
 *   the last letter of your first name and its "link" field should be a null
 *   pointer
 *
 * - each element must be dynamically allocated using a malloc() call
 *
 * - you must use the "name" variable (change it to be your name).
 *
 * Note, since we're using C, not C++ you can't use new/delete!
 * The analog to delete is the free() function
 *
 * - if any call to malloc() fails, your function must return NULL and must also
 *   free any heap memory that has been allocated so far; that is, it must not
 *   leak memory when allocation fails
 *
 * Implement print_list and free_list which should do what you expect.
 * Printing or freeing a nullptr should do nothing.
 *
 * Note: free_list() might be useful for error handling for name_list()...
 *
 *********************************************************************/

Elt *name_list()
{
    //pointer name to string "aiden"
    char *name = "Aiden";
    //pointer head to NULL - empty linked list, nothing after
    Elt *head = NULL;
    Elt *current = NULL;

    // \0 is the null terminator, when this is encountered, we know the string is done
    for(int i = 0; name[i] != '\0'; i++){
        //memory allocate
        Elt *newNode = (Elt*)malloc(sizeof(Elt)); //requesting memory allocation for this elt structure using the size of Elt
        if(newNode == NULL){
            free_list(head); //allocated incase of fail?
            return NULL; //if the new node memory allocation fails it returns null
        }
        newNode->val = name[i];
        newNode->link = NULL;

        if(head == NULL){
            //need to link to next , if its the first node set head
            head = newNode;
            current = head;
        } else{ //linking new node to the current node
            current->link = newNode;
            current = newNode;
        }
    }

    return head;
}


/*********************************************************************/

void print_list( Elt* head )
{
    Elt *current = head;
    while(current != NULL){
        printf("%c ->", current->val);
        current = current->link;
    }
    printf("Null\n");
}

/*********************************************************************/

void free_list( Elt* head )
{
    while (head != NULL){
        Elt *temp = head;
        head = head->link;
        free(temp);
    }
}

//taking pointer to an array of unsiged char
//allows function to work directly with memory locations
void bubbleSort(unsigned char *units, int numUnits) {
    for(int i = 0; i < numUnits - 1; i++){
        for (int j = 0; j < numUnits - i - 1; j++){
            if (units[j] > units[j+1]){
                //swapping those units
                unsigned char temp = units[j];
                units[j] = units[j +1];
                units[j + 1] = temp;
            }
        }
    }
}

/*********************************************************************
 *
 * draw_me()
 *
 * This function creates a file called 'me.txt' which contains an ASCII-art
 * picture of you (it does not need to be very big).
 *
 * Use the C stdlib functions: fopen, fclose, fprintf, etc which live in stdio.h
 * - Don't use C++ iostreams
 *
 *********************************************************************/

void draw_me()
{
    FILE *pfile = fopen("me.txt", "w");

    if(pfile != NULL){
        //art
        fprintf(pfile,"             \n");
        fprintf(pfile,"   o     o   \n");
        fprintf(pfile,"      .      \n");
        fprintf(pfile,"  |       |  \n");
        fprintf(pfile,"   -------   \n");
        fprintf(pfile,"             \n");

        fclose(pfile);
    }
    else{
        printf("error making me\n");
    }
}

/*********************************************************************
 *
 * Test Code - Place your test functions in this section:
 */

    int equalTest(unsigned long a, unsigned long b){
        return a == b;
    }

    void test_1(){
        //----tests for byte and nibble sort----//
        unsigned long testCase1 = 0x0403deadbeef0201;
        unsigned long expectedByte = 0xefdebead04030201;
        unsigned long expectedNibble = 0xfeeeddba43210000;

        unsigned long byteSortResult = (byte_sort(testCase1));
        unsigned long nibbleSortResult = (nibble_sort(testCase1));

        assert(equalTest(byteSortResult, expectedByte));
        assert(equalTest(nibbleSortResult, expectedNibble));

        printf("tests for nibble and byte passed, from Aiden\n");
    }

    void test_2(){
        unsigned long testCase2 = 0x0987654321abcdef;

        unsigned long expectedByte2 = 0xefcdab8765432109;
        unsigned long expectedNibble2 = 0xfedcba9876543210;

        unsigned long nibbleSortResult2 = nibble_sort(testCase2);
        unsigned long byteSortResult2 = byte_sort(testCase2);


        printf("actual result: 0x%016lx\n" , nibbleSortResult2);
        printf("byte result: 0x%016lx\n", byteSortResult2);
        assert(equalTest(byteSortResult2, expectedByte2));
        assert(equalTest(nibbleSortResult2, expectedNibble2));
        printf("test for second nibble sort passed.\n");

    }

void test_name_list() {
    Elt *head = name_list();

    char expectedName[] = "Aiden";

    for (int i = 0; head != NULL; i++) {
        assert(head->val == expectedName[i]);
        head = head->link;
    }

    printf("namelist test passed\n");
}

//TEST BYTE SORT!
//From Linsday Haslam!!!!!!!!!
void testByteSort() {
    //Normal scenario.
    unsigned long test_val1 = 0x0403deadbeef0201;
    unsigned long expected1 = 0xefdebead04030201;
    assert(byte_sort(test_val1) == expected1);

    //All bytes are the same.
    unsigned long test_val2 = 0x1111111111111111;
    unsigned long expected2 = 0x1111111111111111;
    assert(byte_sort(test_val2) == expected2);

    //Obvious increasing order.
    unsigned long test_val3 = 0x0102030405060708;
    unsigned long expected3 = 0x0807060504030201;
    assert(byte_sort(test_val3) == expected3);

    //Already in decreasing order.
    unsigned long test_val4 = 0x0807060504030201;
    unsigned long expected4 = 0x0807060504030201;
    assert(byte_sort(test_val4) == expected4);

    printf("BYTE SORT: All tests passed.\n");
}



/*********************************************************************
 *
 * main()
 *
 * The main driver program.  You can place your main() method in this
 * file to make compilation easier, or have it in a separate file.
 *
 *
 *********************************************************************/

int main(){

    test_1();

    test_name_list();

    test_2();

    testByteSort();

    draw_me();

}