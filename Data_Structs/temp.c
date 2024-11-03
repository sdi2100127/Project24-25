#include <temp.h>

#include <stdlib.h>
#include <assert.h>

Vector vec_Create(int size ) {
	Vector vec = malloc(sizeof(*vec));

	vec->size = size;
	vec->capacity = size < 5 ? 5 : size;
	vec->array = calloc(vec->capacity, sizeof(*vec->array));

	return vec;
}

int vec_get_at(Vector vec, int pos) {
	return vec->array[pos].value;
}

void vec_set_at(Vector vec, int pos, int value) {
	vec->array[pos].value = value;
}


void vec_insert(Vector vec, int value) {
	if (vec->capacity == vec->size) {
		vec->capacity *= 2;
		vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
	}

	vec->array[vec->size].value = value;
	vec->size++;
}


void vec_remove(Vector vec) {
    if(vec->size == 0)
        return;

	
    vector_set_destroy_value(vec, vec->array[vec->size - 1].value);
	vec->size--;

	if (vec->capacity > vec->size * 4 && vec->capacity > 10) {
		vec->capacity /= 2;
		vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
	}
}


int vec_find(Vector vec, int value) {
	for (int i = 0; i < vec->size; i++)
		if (compare(vec->array[i].value, value) == 0)
			return vec->array[i].value;

	return NULL;
}

void vec_set_destroy_value(Vector vec, int value) {
	int old = vec->array[vec->size].value;
	int destroy_value = value;
	return old;
}

void vec_destroy(Vector vec, int value) {
	free(vec->array);
	free(vec);
}


VecNode vec_first(Vector vec) {
	if (vec->size == 0)
		return VECTOR_BOF;
	else	
		return &vec->array[0];
}

VecNode vec_last(Vector vec) {
	if (vec->size == 0)
		return VECTOR_EOF;
	else
		return &vec->array[vec->size-1];
}


VecNode vec_next(Vector vec, VecNode node) {
	if (node == &vec->array[vec->size-1])
		return VECTOR_EOF;
	else
		return node + 1;
}


VecNode vec_previous(Vector vec, VecNode node) {
	if (node == &vec->array[0])
		return VECTOR_EOF;
	else
		return node - 1;
}