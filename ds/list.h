#pragma once
#include "../basic.h"


typedef struct list_t list_t;


#define         list_init(CAPACITY, TYPE)                       __impl_list_init(CAPACITY, #TYPE, sizeof(TYPE))

#define         list_append(PLIST, VALUE)                       __impl_list_append((PLIST), &(VALUE), sizeof(VALUE)) 
void            list_delete(list_t *list, const u64 index);

void            list_destroy(list_t *list);

void            list_dump(const list_t *list);
void            list_print(const list_t *list, void (*print)(void*));

void *          list_get_element_by_index(const list_t *list, const u64 index);

#define         list_clear(PLIST)                               __impl_list_clear(PLIST)






#ifndef IGNORE_LIST_IMPLEMENTATION

struct list_t {

    // length of the list
    u64 len;
    u64 capacity;

    u8 *__array;
    i64 __top;
    u64 __elem_size;

    u64 __original_capacity;

    // This variable checks if the list is a list of pointers 
    bool __are_values_pointers;
};

void * list_get_element_by_index(const list_t *list, const u64 index)
{
    if (list->__are_values_pointers)
        return *(void **)(list->__array + index * list->__elem_size);
    else 
        return (list->__array + index * list->__elem_size);
}

void __impl_list_clear(list_t *list)
{
    assert(list);
    list->__top = -1;
    list->len = 0;
    memset(list->__array, 0, list->__elem_size * list->capacity);
}

list_t __impl_list_init(const u64 capacity, const char *type_name, u64 elem_size) 
{
    bool flag = false;

    if (!strcmp("const char *", type_name) || !strcmp("char *", type_name)) eprint("Use str_t instead of char *");
    if (type_name[strlen(type_name) - 1] == '*') flag = true;

    return (list_t ) {

        .len = 0,
        .capacity = capacity,
        .__array = (u8 *)calloc(capacity, elem_size),
        .__top = -1,
        .__elem_size = elem_size,
        .__original_capacity = capacity,
        .__are_values_pointers = flag,
    };
}

void __impl_list_append(list_t *list, void *value_addr, u64 value_size)
{
    assert(list);
    assert(value_addr);
    assert(value_size == list->__elem_size);

    if (value_size != list->__elem_size) eprint("trying to push a value of size %lu to slot of size %lu", value_size, list->__elem_size);


    if (list->__top == (i64)(list->capacity - 1)) {

        list->capacity = list->capacity * 2;
        list->__array = (u8 *)realloc(list->__array, list->capacity * list->__elem_size);
        //printf("Grew to %ld\n", list->__capacity);
    }

    list->len = ++list->__top + 1;

    memcpy(list->__array + list->__top * list->__elem_size, value_addr, list->__elem_size);
}


void list_delete(list_t *list, const u64 index)
{
    assert(list);
    if(list->__top == -1) eprint("Trying to delete an element from an empty list\n");
    assert((i64)index <= list->__top);

    if ((i64)index != list->__top) {

        memcpy(list->__array + index * list->__elem_size, 
                list->__array + (index + 1) * list->__elem_size, 
                list->__elem_size * (list->__top - index)); 
    } 

    list->len = --list->__top + 1;

    if ((list->capacity != list->__original_capacity) 
            && (list->__top == (i64)((list->capacity / 2) - 1))) { 

        list->capacity = list->capacity / 2;
        list->__array = (u8 *)realloc(list->__array, list->capacity * list->__elem_size);
        printf("Shrunk from %ld to %ld\n", list->capacity * 2, list->capacity);
    }
} 

void list_print(const list_t *list, void (*print)(void*))
{
    assert(list);
    if (list->__top == -1) {
        printf("list is empty\n");
        return ;
    }
    for (u64 i = 0; i < list->len; i++)
    {
        print(list->__array + i * list->__elem_size);
    }
    printf("\n");
}

void list_dump(const list_t *list)
{
    assert(list);

    printf("\n len = %ld\n arr = %p\n top = %ld\n capacity = %ld\n elem_size = %ld\n", 
            list->len, 
            list->__array, 
            list->__top, 
            list->capacity, 
            list->__elem_size);

    printf(" contents = [ ");
    for (u64 i = 0; i < list->capacity; i++)
    {
        printf("%p, ",list->__array + i * list->__elem_size);
    }
    printf("]\n");

}

void list_destroy(list_t *list)
{
    free(list->__array);
    list->__array = NULL;
    list->capacity = 0;
    list->__top = -1;
    list->len = 0;
    list->__elem_size = 0;
}
#endif
