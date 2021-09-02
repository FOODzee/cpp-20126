#include <stdlib.h>

struct arr_list {
//private:
  int first;
  int last;
  int size;
  int capacity;
  void** array;

  arr_list() {
      first = last = 0;
      size = 0;
      capacity = 32;
      array = new void* [capacity];
  }

  ~arr_list() {
      delete[] array;
  }
};

arr_list *init_arr_list ();

void init(arr_list *list) {
    list->first = list->last = 0;
    list->size = 0;
    list->capacity = 32;
    list->array = (void**)malloc (list->capacity * sizeof (void *));
}

int main() {
    arr_list * clist = init_arr_list();
    arr_list * cpplist = new arr_list();

    arr_list cpplist2;

    delete cpplist;
}

void
arr_realloc (arr_list * list, int add)
{
  if (list->size + add > list->capacity)
    {
      int old_capacity = list->capacity;
      //!
      list->capacity *= 2;
      //!
      list->array = (void**) realloc (list->array, list->capacity);
      if (list->first > list->last)
        {
          for (int i = list->first; i < old_capacity; i++)
            {
              list->array[i + old_capacity] = list->array[i];
            }
          list->first += old_capacity;
        }
    }
}

arr_list *
init_arr_list ()
{
  arr_list *list = (arr_list*) malloc (sizeof (arr_list));
  init(list);
  return list;
}

void
destruct_arr_list (arr_list * list)
{
  free (list->array);
  free (list);
}

int
is_empty (arr_list * list)
{
  return list->size == 0;
}

int
get_size (arr_list * list)
{
  return list->size;
}

void *
get_first (arr_list * list)
{
  if (list->size == 0)
    {
      return NULL;
    }
  return list->array[(list->first)];
}

void *
get_last (arr_list * list)
{
  if (list->size == 0)
    {
      return NULL;
    }
  return list->array[(list->last)];
}

void *
get_id (arr_list * list, int id)
{
  if (list->size == 0)
    {
      return NULL;
    }
  return list->array[(list->first + id) % list->capacity];
}

int
add_first (arr_list * list, void *el)
{
  if (list->size == 0)
    {
      list->array[list->first] = el;
      list->size++;
    }
  else
    {
      arr_realloc (list, 1);
      list->first = (list->first + list->capacity - 1) % list->capacity;
      list->array[list->first] = el;
      list->size++;
    }
}

int
add_last (arr_list * list, void *el)
{
  if (list->size == 0)
    {
      list->array[list->last] = el;
      list->size++;
    }
  else
    {
      arr_realloc (list, 1);
      list->last = (list->last + 1) % list->capacity;
      list->array[list->last] = el;
      list->size++;
    }
}

int
add_id (arr_list * list, void *el, int id)
{
  if (list->size < id)
    {
      arr_realloc (list, id - list->size + 1);
      list->array[(list->first + id) % list->capacity] = el;
      list->size += id - list->size + 1;
    }
  else
    {
      arr_realloc (list, 1);
      int p = list->size;
      while (p > id)
        {
          list->array[(list->first + p) % list->capacity] =
            list->array[(list->first + p - 1) % list->capacity];
          p--;
        }
      list->array[(list->first + p) % list->capacity] = el;
      list->size++;
    }
}

void
remove_first (arr_list * list)
{
  if (list->size == 0)
    {
      return;
    }
  list->array[list->first] = NULL;
  list->first = (list->first + 1) % list->capacity;
  list->size--;
}

void
remove_last (arr_list * list)
{
  if (list->size == 0)
    {
      return;
    }
  list->array[list->last] = NULL;
  list->last = (list->last + list->capacity - 1) % list->capacity;
  list->size--;
}

void
remove_id (arr_list * list, int id)
{
  if (list->size <= id)
    {
      return;
    }
  else
    {
      int p = id;
      while (p < list->size)
        {
          list->array[(list->first + p) % list->capacity] =
            list->array[(list->first + p + 1) % list->capacity];
          p++;
        }
      list->size--;
    }
}
