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


bool vecGrow(Vector *vec);


bool vecResize(Vector *vec, int newCap);


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

