#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***********************
 * PREPROCESSOR MACROS *
 ***********************/

// The default starting capacity for a newly created Vector
#define DEFAULT_CAPACITY 8

// The factor that a Vector's capacity increases by every time it is resized
#define GROWTH_FACTOR 1.5

// Returns the larger of two values
#define max(x,y) (x >= y ? x : y)
#define min(x,y) (x <= y ? x : y)

/**************
 * STRUCTURES *
 **************/

/*
 * Metadata of the vector. 
 * Contains the function pointers for working with the abstracted vector data.
 */
typedef struct vectorHead {
	int length;					// The number of elements currently stored in the vector
	int capacity;				// The number of elements the vector can hold before needing to be resized
	void **data;				// The actual data stored by the vector
	void (*deleteData)(void *);	// Function pointer to free an element in the vector
	char *(*printData)(void *);	// Function pointer to create a string from a vector element
	int (*compare)(const void *, const void *);	// Function pointer to create a string from a vector element
} Vector;


/*************
 * FUNCTIONS *
 *************/

/*
 * Function to initialize the Vector metadata head to the appropriate function pointers.
 * Allocates memory to the struct, unless any of the function pointers are NULL. In this
 * case, NULL is returned instead and no memory is allocated. NULL is also returned
 * if any memory allocation fails.
 *
 * The Vector provides an interface to a generic collection of data. The three
 * function pointers allow the struct to print, delete, and compare its data.
 * The (void *) arguments are to be casted into their proper data type (i.e. whatever
 * data type the vector will be holding) and do as follows:
 *
 * 	char *printFunc(void *toPrint)  : return a string representation of `toPrint`
 * 	void deleteFunc(void *toDelete) : free all memory associated with `toDelete`
 * 	int compare(const void *a, const void *b) : return the value of `a` - `b` (same return value logic as strcmp)
 *
 * Examples of these functions are provided for string (char *) data in the README.
 */
Vector *vecNew(void (*deleteFunc)(void *), char *(*printFunc)(void *), int (*compareFunc)(const void *, const void *));


/*
 * Identical to vecNew, but the starting capacity can be manually specified instead
 * of using the library default value (determined by DEFAULT_CAPACITY).
 *
 * Returns NULL if cap is negative, if any of the function pointers are NULL, or if
 * any memory allocation fails.
 * Returns a newly allocated vector otherwise.
 */
Vector *vecNewCap(int cap, void (*deleteFunc)(void *), char *(*printFunc)(void *), int (*compareFunc)(const void *, const void *));


/*
 * Frees every element in the Vector without deleting the vector itself.
 */
void vecClear(Vector *vec);


/*
 * Frees all memory associated with the Vector, including the vector itself.
 */
void vecFree(Vector *vec);


/*
 * Increases the capacity of the Vector, allowing it to store more elements.
 * The new capacity is equal to the vector's previous capacity multiplied by the GROWTH_FACTOR macro.
 *
 * Returns false if the vector is NULL or if any memory allocation fails.
 * Returns true otherwise, indicating a successful resize operation.
 */
bool vecGrow(Vector *vec);


/*
 * Forcibly sets the Vector's capacity to a new value. If this causes the vector's
 * capacity to decrease, all truncated elements are removed from the vector and freed.
 *
 * Returns false if the new capacity is negative, if the vector is NULL,
 * or if any memory allocation fails.
 *
 * Returns true otherwise, indicating a successful resize operation.
 * True is also returned if the new capacity equals the old capacity,
 * in which case this function does nothing and returns immediately.
 */
bool vecResize(Vector *vec, int newCap);


/*
 * Adds the data to the end of the Vector.
 *
 * Returns false if the vector is NULL or if any memmory alloation fails.
 * Returns true otherwise, indicating a successful push operation.
 */
bool vecPush(Vector *vec, void *data);


/*
 * Inserts the data at the specified index within the Vector.
 * All elements to the right of this index are shifted to the right to make room.
 *
 * Returns false if the vector is NULL, if the index goes off the end of the vector,
 * or if any memory allocation fails.
 * Returns true otherwise, indicating a successful insertion operation.
 */
bool vecInsert(Vector *vec, int index, void *data);


/*
 * Uses the Vector's comparison function pointer to place the element in the
 * appropriate position within the vector such that its data remains sorted.
 *
 * This should be the only insertion function used if sorted data is required.
 *
 * Returns false if the vector is NULL, or if any memory allocation fails.
 * Returns true otherwise, indicating a successful insertion.
 */
bool vecInsertSorted(Vector *vec, void *data);


/*
 * Returns the element at the specified index in the Vector.
 *
 * Returns NULL if the vector itself is NULL, if the vector is empty,
 * if the index is negative, or if the index goes off the end of the vector.
 */
void *vecGet(Vector *vec, int index);


/*
 * Returns and removes the last element from the Vector.
 *
 * Returns NULL if the vector itself is NULL, or if the vector is empty.
 */
void *vecPop(Vector *vec);


/*
 * Returns and removes the element at the specified index in the Vector.
 * All elements to the right of this index are shifted to the left to fill the gap.
 *
 * Returns NULL if the vector itself is NULL, if the vector is empty,
 * if the index is negative, or if the index goes off the end of the vector.
 */
void *vecRemove(Vector *vec, int index);


/*
 * Returns the first element in the Vector that causes the compareFunc to return true.
 * Returns NULL if no element was found that matches the criteria given by the searchRecord
 * and the compareFunc.
 *
 * The comparefunc compares the searchRecord and an alement from the vector for equality.
 * Much like the vector's own function pointers, the compareFunc must cast its two arguments
 * into the same type as stored by the vector.
 *
 * An example of using this function can be found in the README.
 */
void *vecFind(Vector *vec, bool (*compareFunc)(const void *, const void *), const void *searchRecord);


/*
 * Similar to vecFind, but the found element is removed from the vector after it's found.
 */
void *vecFindRemove(Vector *vec, bool (*compareFunc)(const void *, const void *), const void *searchRecord);


/*
 * Returns the number of elements currently stored in the Vector.
 *
 * If the vector is NULL, -1 is returned.
 */
int vecLength(const Vector *vec);


/*
 * Returns the number of items the Vector can store before needing to be resized.
 *
 * If the vector is NULL, -1 is returned.
 */
int vecCapacity(const Vector *vec);


/*
 * Returns true if the Vector contains no elements, and false otherwise.
 *
 * False is also returned when the Vector is a NULL pointer.
 */
bool vecIsEmpty(const Vector *vec);


/*
 * Returns true if the vector's length is equal to its capacity, and false otherwise.
 *
 * False is also returned when the Vector is a NULL pointer.
 */
bool vecIsFull(const Vector *vec);


/*
 * Returns a string representing the data in the Vector at a specific index using the vector's
 * `printData` function pointer to create the string.
 *
 * The string must be freed by the calling function after use.
 */
char *vecIndexToString(const Vector *vec, int idx);


/*
 * A convenient alias for printing the string returned by `vecIndexToString(vec, idx)`
 * and then freeing the string that was created after printing it.
 * A newline is printed after the string is done printing.
 */
void vecPrintIndex(const Vector *vec, int idx);


/*
 * Returns a string representing the Vector using the vector's `printData` function pointer
 * to create the string.
 *
 * The string must be freed by the calling function after use.
 */
char *vecToString(const Vector *vec);


/*
 * A convenient alias for printing the string returned by `vecToString(vec)`
 * and then freeing the string that was created after printing it.
 * A newline is printed after the string is done printing.
 */
void vecPrint(const Vector *vec);

#endif	// VECTOR_H

