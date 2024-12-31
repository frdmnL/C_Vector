/*
 * @Author: liz kishimeru474@gmail.com
 * @Description: 
 * @Date: 2024-12-27 18:17:39
 * Copyright (c) liz, All Rights Reserved. 
 */
#include "c_vector/c_vector.h"
#include "c_vector/c_typedef.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_CAPACITY_SIZE_ADD 512
#define DEFAULT_CAPACITY_LIMIT 512
#define DEFAULT_CAPACITY_NEW_MUL 1.5
#define DEFAULT_CAPACITY_RELEASE_LIMIT 1024

typedef struct 
{
    c_vector base;
    unsigned long* content;
    unsigned int length;
    unsigned int capacity;
} c_vector_impl_t;

typedef struct _list_objs
{
    c_vector_impl_t *obj;
    unsigned char b_valid;
    struct _list_objs *next;
} c_vector_list_objs;

typedef struct
{
    c_vector_list_objs *head;
    c_vector_list_objs *tail;
    unsigned int count;
} c_vector_list_objs_mgr;

static c_vector_list_objs_mgr* l_mgr;
static int copy_vec(struct _c_vector *obj, const struct _c_vector *vec);
static void* at(struct _c_vector *obj, unsigned index);
static void* front(struct _c_vector *obj);
static void* back(struct _c_vector *obj);
static void clear(struct _c_vector *obj);
static unsigned char empty(struct _c_vector *obj);
static int erase(struct _c_vector *obj, unsigned int pos_start,int num);
static int insert(struct _c_vector *obj, unsigned int index, void *data);
static int insert_vec(struct _c_vector *obj, unsigned int index, const struct _c_vector *vec);
static int push_back(struct _c_vector *obj,void *data);
static int size(struct _c_vector *obj);
static int swap(struct _c_vector *obj, struct _c_vector *vec);
static int sort(struct _c_vector *obj, fun_sort_t fun);
static void for_each(struct _c_vector *obj, fun_each_t fun_each);

static int l_make_new_capacity(c_vector_impl_t* impl,unsigned int capacity_new)
{
    int cap = (int)(capacity_new * DEFAULT_CAPACITY_NEW_MUL);
    if(capacity_new < impl->capacity)
    {
        return VECTOR_ALL_OK;
    }

    if(cap < DEFAULT_CAPACITY_LIMIT)
    {
        cap = capacity_new + DEFAULT_CAPACITY_SIZE_ADD;
        unsigned int long* data = (unsigned long*)malloc(sizeof(unsigned long) * cap);
        if(data == NULL)
        {
            return VECTOR_ERR_NO_MEMORY;
        }
        if(impl->length)
        {
            memcpy(data, impl->content, sizeof(unsigned long) * impl->length);
        }
        if(impl->content)
        {
            free(impl->content);
        }
        impl->content = data;
        impl->capacity = cap;

        return VECTOR_ALL_OK;
    }
}

static int copy_vec(struct _c_vector *obj, const struct _c_vector *vec)
{
    int ret = VECTOR_ALL_OK;
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    c_vector_impl_t *impl_src = (c_vector_impl_t*)vec;
    if(impl == NULL || impl_src == NULL)
    {
        return VECTOR_ERR_INVALID_INSTANCE;
    }
    if(!impl_src->length)
    {
        impl->length = 0;
        return VECTOR_ALL_OK;
    }
    if(VECTOR_ALL_OK != (l_make_new_capacity(impl, impl_src->length)))
    {
        return VECTOR_ERR_NO_MEMORY;
    memcpy(impl->content, impl_src->content, sizeof(unsigned long) * impl_src->length);
    impl->length = impl_src->length;

    return VECTOR_ALL_OK;
}

static void* at(struct _c_vector *obj, unsigned index)
{
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL)
    {
        return NULL;
    }
    if(index >= impl->length)
    {
        return NULL;
    }
    return (void*)(impl->content[index]);
}

static void* front(struct _c_vector *obj)
{
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL || impl->length == 0)
    {
        return NULL;
    }

    if(impl->length == 0)
    {
        return NULL;
    }
    return (void*)(impl->content[0]);
}

static void* back(struct _c_vector *obj)
{
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL || impl->length == 0)
    {
        return NULL;
    }

    if(impl->length == 0)
    {
        return NULL;
    }
    return (void*)(impl->content[impl->length - 1]);
}

static void clear(struct _c_vector *obj)
{
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL)
    {
        return;
    }
    impl->length = 0;
}

static unsigned char empty(struct _c_vector *obj)
{
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL)
    {
        return 1;
    }
    return (impl->length == 0);
}

static int erase(struct _c_vector *obj, unsigned int pos_start,int num)
{
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL)
    {
        return VECTOR_ERR_INVALID_INSTANCE;
    }
    if((pos_start && pos_start >= impl->length) || num < -1)
    {
        return VECTOR_ERR_PARAM;
    }
    if(pos_start == 0 && num == 0)
    {
        return VECTOR_ALL_OK;
    }
    if(pos_start + num >= impl->length || num == -1)
    {
        num = impl->length - pos_start;
    }

    if(pos_start + num < impl->length)
    {
        unsigned long* p_s = impl->content + pos_start;
        int num_temp = impl->length - pos_start;
        while (num_temp)
        {
            *p_s = *(p_s + num);
            p_s++;
            num_temp--;
        }
    }
    c_vector_impl_t->length -= num;
    return VECTOR_ALL_OK;
}