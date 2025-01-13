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

static int insert(struct _c_vector *obj, unsigned int index, void *data)
{
    int ret = VECTOR_ALL_OK;
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    unsigned int pos = 0;
    if(impl == NULL)
    {
        return VECTOR_ERR_INVALID_INSTANCE;
    }
    if(index > impl->length || data == NULL)
    {
        return VECTOR_ERR_PARAM;
    }

    if(VECTOR_ALL_OK != (ret = l_make_new_capacity(impl, impl->length + 1)))
        return ret;
    
    for(pos = impl->length; pos > index; pos--)
    {
        impl->content[pos] = impl->content[pos - 1];
    }
    impl->content[index] = (unsigned long)data;
    impl->length++;
    return VECTOR_ALL_OK;
}

static int insert_vec(struct _c_vector* obj, unsigned int index, const struct _c_vector *vec)
{
    int ret = VECTOR_ALL_OK;
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    c_vector_impl_t *impl_src = (c_vector_impl_t*)vec;
    unsigned int pos = 0;
    if(impl == NULL || impl_src == NULL)
    {
        return VECTOR_ERR_INVALID_INSTANCE;
    }
    if(index > impl->length || impl_src->length == 0)
    {
        return VECTOR_ERR_PARAM;
    }

    if(VECTOR_ALL_OK != (ret = l_make_new_capacity(impl, impl->length + impl_src->length)))
        return ret;
    if(impl_src->length > impl->length = index)
    {
        memcpy(impl->content + index + impl_src->, impl_src->content, sizeof(unsigned long) * impl_src->length);
    }
    for(pos = impl->length; pos > index; pos--)
    {
        impl->content[pos] = impl->content[pos - 1];
    }
    memcpy(impl->content + index, impl_src->content, sizeof(unsigned long) * impl_src->length);
    impl->length += impl_src->length;
    return VECTOR_ALL_OK;
}

static int push_back(struct _c_vector *obj,void *data)
{
    int ret = VECTOR_ALL_OK;
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL)
    {
        return VECTOR_ERR_INVALID_INSTANCE;
    }
    if(data == NULL)
    {
        return VECTOR_ERR_PARAM;
    }
    if(VECTOR_ALL_OK != (ret = l_make_new_capacity(impl, impl->length + 1)))
        return ret;
    impl->content[impl->length] = (unsigned long)data;
    impl->length++;
    return VECTOR_ALL_OK;
}

static int size(struct _c_vector *obj)
{
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL)
    {
        return VECTOR_ERR_INVALID_INSTANCE;
    }
    return impl->length;
}

static int swap(struct _c_vector *obj, struct _c_vector *vec)
{
    c_vector_impl_t *temp;
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    c_vector_impl_t *impl_src = (c_vector_impl_t*)vec;
    unsigned long* content = NULL;
    unsigned int length = 0;
    unsigned int capacity = 0;
    if(impl == NULL || impl_src == NULL)
    {
        return VECTOR_ERR_INVALID_INSTANCE;
    }
    memcpy(&temp, impl, sizeof(c_vector_impl_t));
    memcpy(impl, impl_src, sizeof(c_vector_impl_t));
    memcpy(impl_src, &temp, sizeof(c_vector_impl_t));
    return VECTOR_ALL_OK;
}

static int sort(struct _c_vector *obj, fun_sort_t fun)
{
    unsigned long temp;
    unsigned int pos,pos_sort,pos_max;
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL)
    {
        return VECTOR_ERR_INVALID_INSTANCE;
    }
    if(fun == NULL)
    {
        return VECTOR_ERR_PARAM_NULL;
    }
    if(impl->length == 2)
    {
        if(-1 == fun((void*)impl->content[0], (void*)impl->content[1]))
        {
            temp = impl->content[0];
            impl->content[0] = impl->content[1];
            impl->content[1] = temp;
        }
        return VECTOR_ALL_OK;
    }
    for(pos = 0; pos < impl->length - 1; pos++)
    {
        pos_sort = pos;
        pos_max = pos;
        for(pos_sort = pos + 1; pos_sort < impl->length; pos_sort++)
        {
            if(-1 == fun((void*)impl->content[pos_sort], (void*)impl->content[pos_max]))
            {
                pos_max = pos_sort;
            }
        }
        if(pos_max != pos)
        {
            temp = impl->content[pos];
            impl->content[pos] = impl->content[pos_max];
            impl->content[pos_max] = temp;
        }
    }
    return VECTOR_ALL_OK;
}

static for_each(struct _c_vector *obj, fun_each_t fun_each)
{
    unsigned int pos;
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL)
    {
        return VECTOR_ERR_INVALID_INSTANCE;
    }
    if(fun_each == NULL)
    {
        return VECTOR_ERR_PARAM_NULL;
    }
    for(pos = 0; pos < impl->length; pos++)
    {
        fun_each((void*)impl->content[pos]);
    }
    return VECTOR_ALL_OK;
}

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

int c_vector_mgr_init(void)
{
    l_mgr = (c_vector_list_objs_mgr*)malloc(sizeof(c_vector_list_objs_mgr));
    if(l_mgr == NULL)
    {
        return VECTOR_ERR_NO_MEMORY;
    }
    memset(l_mgr, 0, sizeof(c_vector_list_objs_mgr));
    return VECTOR_ALL_OK;
}

void c_vector_mgr_release(void)
{
    if(l_mgr)
    {
        c_vector_list_objs *temp = NULL;
        while(l_mgr->head)
        {
            free(l_mgr->head->obj->content);
            free(l_mgr->head->obj);
            temp = l_mgr->head;
            l_mgr->head = l_mgr->head->next;
            free(temp);
        }
        free(l_mgr);
        l_mgr = NULL;
    }
}

c_vector *c_vector_create(void)
{
    c_vector_impl_t *obj = NULL;
    c_vector_list_objs *node = NULL;
    if(l_mgr)
    {
        node = l_mgr->head;
        while(node)
        {
            if(node->b_valid == 0)
            {
                node->b_valid = 1;
                return (c_vector*)node->obj;
            }
            node = node->next;
        }
    }

    obj = (c_vector_impl_t*)malloc(sizeof(c_vector_impl_t));
    if(obj == NULL)
    {
        return NULL;
    }
    if(l_mgr)
    {
        node = (c_vector_list_objs*)malloc(sizeof(c_vector_list_objs));
        if(node == NULL)
        {
            free(obj);
            return NULL;
        }
    }
    memset(obj, 0, sizeof(c_vector_impl_t));

    obj->base.copy_vec = copy_vec;
    obj->base.at = at;
    obj->base.front = front;
    obj->base.back = back;
    obj->base.clear = clear;
    obj->base.empty = empty;
    obj->base.erase = erase;
    obj->base.insert = insert;
    obj->base.insert_vec = insert_vec;
    obj->base.push_back = push_back;
    obj->base.size = size;
    obj->base.swap = swap;
    obj->base.sort = sort;
    obj->base.for_each = for_each;

    if(l_mgr)
    {
        node->obj = obj;
        node->b_valid = 1;
        node->next = NULL;
        if(l_mgr->head == NULL)
        {
            l_mgr->head = node;
            l_mgr->tail = node;
        }
        else
        {
            l_mgr->tail->next = node;
            l_mgr->tail = node;
        }
        l_mgr->count++;
    }
    return (c_vector*)obj;
}

void c_vector_release(c_vector *obj)
{
    c_vector_impl_t *impl = (c_vector_impl_t*)obj;
    if(impl == NULL)
    {
        return;
    }
    if(l_mgr)
    {
        c_vector_list_objs *node = l_mgr->head;
        while(node)
        {
            if(node->obj == impl)
            {
                if(node->obj->capacity > DEFAULT_CAPACITY_RELEASE_LIMIT)
                {
                    free(node->obj->content);
                    node->obj->content = NULL;
                    node->obj->capacity = 0;
                }
                node->obj->length = 0;
                node->b_valid = 0;
                return;
            }
            node = node->next;
        }
    }
    free(impl->content);
    free(impl);
}