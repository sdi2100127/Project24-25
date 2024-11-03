#include <temp.h>

#include <stdlib.h>
#include <assert.h>

Vector vector_create(int size ) {
	Vector vec = malloc(sizeof(*vec));

	vec->size = size;
	vec->capacity = size < 5 ? 5 : size;
	vec->array = calloc(vec->capacity, sizeof(*vec->array));

	return vec;
}

int vector_get_at(Vector vec, int pos) {
	return vec->array[pos].value;
}

void vector_set_at(Vector vec, int pos, int value) {
	vec->array[pos].value = value;
}


void vector_insert_last(Vector vec, int value) {
	if (vec->capacity == vec->size) {
		vec->capacity *= 2;
		vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
	}

	vec->array[vec->size].value = value;
	vec->size++;
}


void vector_remove_last(Vector vec) {
    if(vec->size == 0)
        return;

	
    vector_set_destroy_value(vec, vec->array[vec->size - 1].value);
	vec->size--;

	if (vec->capacity > vec->size * 4 && vec->capacity > 10) {
		vec->capacity /= 2;
		vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
	}
}


int vector_find(Vector vec, int value) {
	for (int i = 0; i < vec->size; i++)
		if (compare(vec->array[i].value, value) == 0)
			return vec->array[i].value;

	return NULL;
}

void vector_set_destroy_value(Vector vec, int value) {
	int old = vec->array[vec->size].value;
	int destroy_value = value;
	return old;
}

void vector_destroy(Vector vec, int value) {
	free(vec->array);
	free(vec);
}


VecNode vector_first(Vector vec) {
	if (vec->size == 0)
		return VECTOR_BOF;
	else	
		return &vec->array[0];
}

VecNode vector_last(Vector vec) {
	if (vec->size == 0)
		return VECTOR_EOF;
	else
		return &vec->array[vec->size-1];
}