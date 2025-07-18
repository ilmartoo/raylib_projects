#pragma once
#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

#include <stdlib.h>

#define DYNAMIC_ARRAY_DEFAULT_SIZE 10

#define __DYNAMIC_ARRAY_DEFINITION(element_type, struct_name) \
    typedef struct { element_type *elements; size_t length; size_t size; } struct_name; \
    static inline struct_name struct_name##CreateSized(size_t initial_size) { return (struct_name){ (element_type *)malloc(sizeof(element_type) * initial_size), 0, initial_size }; } \
    static inline struct_name struct_name##Create() { return struct_name##CreateSized(DYNAMIC_ARRAY_DEFAULT_SIZE); } \
    static inline void struct_name##Delete(struct_name* array) { free(array->elements); } \
    static inline void struct_name##Add(struct_name* array, element_type element, size_t index) { if (index >= array->length) { index = array->length - 1; } if (array->length == array->size) { array->size *= 2; array->elements = (element_type *)realloc(array->elements, array->size); } for (size_t i = index; i < array->length; ++i) { array->elements[i + 1] = array->elements[i]; } array->length++; array->elements[index] = element; } \
    static inline element_type struct_name##Remove(struct_name* array, size_t index) { if (index >= array->length) { index = array->length - 1; } element_type element = array->elements[index]; for (size_t i = index; i < array->length; ++i) { array->elements[i] = array->elements[i + 1]; } array->length--; return element; } \
    static inline void struct_name##Push(struct_name* array, element_type element) { struct_name##Add(array, element, array->length - 1); } \
    static inline element_type struct_name##Pop(struct_name* array) { return struct_name##Remove(array, array->length - 1); } \
    static inline void struct_name##Shift(struct_name* array, element_type element) { struct_name##Add(array, element, 0); } \
    static inline element_type struct_name##Unshift(struct_name* array) { return struct_name##Remove(array, 0); } \
    static inline void struct_name##Clear(struct_name* array) { array->length = 0; }

/**
 * Abstract definition of a dynamic array in C.
 * To create an implementation, this macro must be called with the element type and the name of the dynamic array type.
  * 
 * @param element_type The type for the array elements stored.
 * @param struct_name The name of the dynamic array struct type to be defined.
 *
 * @note This macro defines a dynamic array struct and a set of functions to work with it:
 * 
 * Structs: 
 *   
 * - `<struct_name>`: The dynamic array struct defined by the macro with `<elem_type>` as the array element's type.
 * 
 * Functions:  
 * 
 * - `<struct_name> <struct_name>Create()`: Creates a dynamic array with the default size.
 * 
 * - `<struct_name> <struct_name>CreateSized(size_t initial_size)`: Creates a dynamic array custom sized.
 * 
 * - `void <struct_name>Delete(<struct_name>* array)`: Deletes a dynamic array.
 * 
 * - `void <struct_name>Add(<struct_name>* array, <elem_type> element, size_t index)`: Adds a new element at the specified position.
 * 
 * - `<elem_type> <struct_name>Remove(<struct_name>* array, size_t index)`: Removes the element at the specified position.
 * 
 * - `void <struct_name>Push(<struct_name>* array, <elem_type> element)`: Appends a new element at the end of the array.
 * 
 * - `<elem_type> <struct_name>Pop(<struct_name>* array)`: Removes the last element of the array.
 * 
 * - `void <struct_name>Shift(<struct_name>* array, <elem_type> element)`: Adds a new element at the start of the array.
 * 
 * - `<elem_type> <struct_name>Unshift(<struct_name>* array)`: Removes the first element of the array.
 * 
 * - `void <struct_name>Clear(<struct_name>* array)`: Removes all elements from the array.
 */
#define DYNAMIC_ARRAY_DEFINITION(element_type, struct_name) __DYNAMIC_ARRAY_DEFINITION(element_type, struct_name)

/**
 * A custom for loop to iterate over all elements of the dynamic array by reference.
 * @param element_type Type of the elements stored in the array
 * @param var Name of the variable to store the current element.
 * @param array Dynamic array to iterate over.
 */
#define for_dynamic_array(element_type, var, array) for (element_type* var = array->elements; var < array->elements + array->length; var++)

#endif  // __DYNAMIC_ARRAY_H__