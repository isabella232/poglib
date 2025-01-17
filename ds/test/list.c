#define DEBUG
#include "../../basic.h"
#include "../list.h"
#include "../../str.h"




typedef struct foo {
    const char *label;
    int list[5];
} foo;


void print_foo(void *x)
{
    foo *tmp = (foo *) x;
    printf("%s ", tmp->label);
    for (int i = 0; i < 5; i++)
        printf(" %i",tmp->list[i]);
    printf("\n");
}

void print_pfoo(void *x)
{
    foo *tmp = *(foo **) x;
    printf("%s ", tmp->label);
    for (int i = 0; i < 5; i++)
        printf(" %i",tmp->list[i]);
    printf("\n");
}

list_t dummy(void)
{
    foo a = {
        .label = "a",
        .list = {1,2,3,4,5}
    };

    foo b = {
        .label = "b",
        .list = {6,7,8,9,10}
    };

    foo c = {
        .label = "c",
        .list = {11,12,13,14,15}
    };
    list_t list = list_init(3, foo);

    /*list_dump(&list);*/

    list_append(&list, a);
    list_append(&list, b);
    list_append(&list, c);
    list_append(&list, a);
    list_append(&list, b);
    list_append(&list, c);
    list_append(&list, a);
    list_append(&list, b);
    list_append(&list, c);
    return list;
}

int oldmain(void)
{
    // Pointer

    foo a = {
        .label = "a",
        .list = {1,2,3,4,5}
    };

    foo b = {
        .label = "b",
        .list = {6,7,8,9,10}
    };

    foo c = {
        .label = "c",
        .list = {11,12,13,14,15}
    };

    foo *values[3] = { &a, &b, &c };
    list_t list = list_init(3, foo *);

    /*list_dump(&list);*/

    list_append(&list, values[0]);
    list_append(&list, values[1]);
    list_append(&list, values[2]);
    list_append(&list, values[0]);
    list_append(&list, values[1]);
    list_append(&list, values[2]);
    list_append(&list, values[0]);
    list_append(&list, values[1]);
    list_append(&list, values[2]);
    list_print(&list, print_pfoo);


    list_delete(&list, 0);
    list_delete(&list, 2);
    list_print(&list, print_pfoo);
    list_delete(&list, 2);
    list_print(&list, print_pfoo);
    list_delete(&list, 2);
    list_print(&list, print_pfoo);

    list_dump(&list);

    list_destroy(&list);

    return 0;
}

int oldmain02(void)
{
    // Value

    list_t list = dummy();
    list_print(&list, print_foo);


    list_delete(&list, 0);
    list_delete(&list, 2);
    list_print(&list, print_foo);
    list_delete(&list, 2);
    list_print(&list, print_foo);
    list_delete(&list, 2);
    list_print(&list, print_foo);

    list_dump(&list);

    list_destroy(&list);

    return 0;
}




// Using Primitvie datattypes

void print_u32(void *arg)
{
    u32 num = *(u32 *)arg;
    printf("%i ", num);
}

void print_str(void *x)
{
    str_t *y = (str_t *)x;

    str_print(y);
}

int main(void)
{
    dbg_init();
    list_t list = list_init(10, str_t );

    str_t words[] = {
        str("yo"),
        str("bruh"),
        str("gucci"),
    };

    for (u32 i = 0; i < ARRAY_LEN(words); i++)
    {
        list_append(&list, words[i]);
    }

    list_print(&list, print_str);

    list_destroy(&list);

    dbg_destroy();
    return 0;
}


int ooldmain(void)
{
    list_t list = list_init(2, u32);

    list_print(&list, print_u32);

    u32 values[10] = {1,2,3,4,5,6,7,8,9,10};


    for (int i = 0; i < 10; i++)
    {
        list_append(&list, values[i]);
        list_print(&list, print_u32);
    }

    list_delete(&list, 4);
        list_print(&list, print_u32);
    list_delete(&list, 0);
        list_print(&list, print_u32);
    list_delete(&list, 1);
        list_print(&list, print_u32);
    list_delete(&list, 6);
        list_print(&list, print_u32);
    
    list_delete(&list, 0);
        list_print(&list, print_u32);

    list_delete(&list, 0);
        list_print(&list, print_u32);

    list_delete(&list, 0);
        list_print(&list, print_u32);

    list_delete(&list, 0);
        list_print(&list, print_u32);
    list_delete(&list, 0);
        list_print(&list, print_u32);
    list_delete(&list, 0);
        list_print(&list, print_u32);

    list_dump(&list);


    list_destroy(&list);

    return 0;
}

int oldmain04(void)
{
    list_t list = list_init(4, list_t );

    for (int i = 0; i < 4; i++)
    {
        list_t tmp = list_init(4, foo);
        list_append(&list, tmp);
    }

    foo a = {
        .label = "a",
        .list = {1,2,3,4,5}
    };

    foo b = {
        .label = "b",
        .list = {6,7,8,9,10}
    };

    foo c = {
        .label = "c",
        .list = {11,12,13,14,15}
    };

    list_t *tmp = (list_t *)list_get_element_by_index(&list, 3);
    list_append(tmp, a);
    list_append(tmp, b);
    list_append(tmp, c);

    tmp = (list_t *)list_get_element_by_index(&list, 2);
    list_append(tmp, b);

    tmp = (list_t *)list_get_element_by_index(&list, 1);
    list_append(tmp, c);

    list_print(&list, list_dump);

    for (int i = 0; i < list.len; i++)
    {
        list_t *bar = (list_t *)list_get_element_by_index(&list, i);
        if (!bar->len) {
            printf("list %i list is empty\n", i);
            continue;
        }
        printf("list %i is \n",i);
        for (int j = 0; j < bar->len; j++)
        {
            foo *x = (foo *)list_get_element_by_index(bar, j);
            print_foo(x);

        }
    }

    printf("\n\n");

    list_delete(&list, 0);
    for (int i = 0; i < list.len; i++)
    {
        list_t *bar = (list_t *)list_get_element_by_index(&list, i);
        if (!bar->len) {
            printf("list %i list is empty\n", i);
            continue;
        }
        printf("list %i is \n",i);
        for (int j = 0; j < bar->len; j++)
        {
            foo *x = (foo *)list_get_element_by_index(bar, j);
            print_foo(x);

        }
    }


    for (int i = 0; i < list.len; i++)
    {
        list_t *tmp = (list_t *)list_get_element_by_index(&list, i);
        list_destroy(tmp);
    }
    list_destroy(&list);

    return 0;
}
