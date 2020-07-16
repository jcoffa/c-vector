#include "Vector.h"


Vector *vecNew(void (*deleteFunc)(void *), char *(*printFunc)(void *), int (*compareFunc)(const void *, const void *)) {
	return vecNewCap(DEFAULT_CAPACITY, deleteFunc, printFunc, compareFunc);
}


Vector *vecNewCap(int cap, void (*deleteFunc)(void *), char *(*printFunc)(void *), int (*compareFunc)(const void *, const void *)) {
	if (cap < 0 || deleteFunc == NULL || printFunc == NULL || compareFunc == NULL) {
		return NULL;
	}

	Vector *toReturn = malloc(sizeof(Vector));

	// Can't assume malloc works every time, no matter how unlikely
	if (toReturn == NULL) {
		return NULL;
	}

	toReturn->data = malloc(sizeof(void*) * cap);
	if (toReturn->data == NULL) {
		return NULL;
	}
	toReturn->length = 0;
	toReturn->capacity = cap;
	toReturn->deleteData = deleteFunc;
	toReturn->printData = printFunc;
	toReturn->compare = compareFunc;

	return toReturn;
}


void vecClear(Vector *vec) {
	if (vec == NULL) {
		return;
	}

	for (int i = 0; i < vec->length; i++) {
		vec->deleteData((vec->data)[i]);
	}

	vec->length = 0;
}


void vecFree(Vector *vec) {
	if (vec == NULL) {
		return;
	}

	vecClear(vec);
	free(vec->data);
	free(vec);
}


bool vecGrow(Vector *vec) {
	if (vec == NULL) {
		return false;
	}

	// This max function ensures the vector always increases in size,
	// even when its original capacity was 0 or 1
	return vecResize(vec, max(vec->capacity * GROWTH_FACTOR, vec->capacity + 1));
}


bool vecResize(Vector *vec, int newCap) {
	if (vec == NULL) {
		return false;
	}

	// One of these days humanity may discover a negative length.
	// This is not that day.
	if (newCap < 0) {
		return false;
	}

	// No sense trying anything if the resize won't do anything anyways
	if (vec->capacity == newCap) {
		return true;
	}

	// Allocate the new data storage.
	// New storage is allocated instead of just realloc'ing the vector's own
	// data storage just in case something goes wrong along the way. This way,
	// the original vector is only modified after the memory for the new storage
	// has been allocated successfully.
	//
	// If we did it the other way where the vector's data storage was realloc'd,
	// we would have to delete truncated elements before the realloc. If the realloc
	// fails at this point, thereby leaving the vector's data untouched, we still can't
	// bring back the data that was truncated. We lost all those elements for no reason.
	void **newStorage = malloc(sizeof(void*) * newCap);
	// Can't assume malloc works every time, no matter how unlikely
	if (newStorage == NULL) {
		return false;
	}

	// Move the elements over to the new storage
	memmove(newStorage, vec->data, sizeof(void*) * min(vec->length, newCap));

	// Free truncated elements if necessary
	for (int i = newCap; i < vec->length; i++) {
		printf("Freeing truncated data at index %d\n", i);	// TODO remove
		vec->deleteData((vec->data)[i]);
	}

	free(vec->data);
	vec->data = newStorage;
	vec->capacity = newCap;

	return true;
}


bool vecPush(Vector *vec, void *data);


bool vecInsert(Vector *vec, int index, void *data);


bool vecInsertSorted(Vector *vec, void *data);


void *vecPop(Vector *vec);


void *vecRemove(Vector *vec, int index);


void *vecFind(Vector *vec, bool (*compareFunc)(const void *, const void *), const void *searchRecord);


void *vecFindRemove(Vector *vec, bool (*compareFunc)(const void *, const void *), const void *searchRecord);


int vecLength(const Vector *vec) {
	if (vec == NULL) {
		return -1;
	}
	return vec->length;
}


int vecCapacity(const Vector *vec) {
	if (vec == NULL) {
		return -1;
	}
	return vec->capacity;
}


bool vecIsEmpty(const Vector *vec) {
	if (vec == NULL) {
		return false;
	}
	return vec->length == 0;
}


bool vecIsFull(const Vector *vec) {
	if (vec == NULL) {
		return false;
	}
	return vec->length == vec->capacity;
}


bool *vecIndexToString(const Vector *vec, int idx);


void vecPrintIndex(const Vector *vec, int idx);


char *vecToString(const Vector *vec);


void vecPrint(const Vector *vec);

