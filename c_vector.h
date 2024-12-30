/*
 * @Author: liz kishimeru474@gmail.com
 * @Description: 
 * @Date: 2024-12-27 18:00:00
 * Copyright (c) liz, All Rights Reserved. 
 */
#ifndef __C_VECTOR_H__
#define __C_VECTOR_H__

#define VECTOR_ALL_OK 0 /*no error */
#define VECTOR_ERR_NO_MEMORY -100 /*memroy not enough*/
#define VECTOR_ERR_INVALID_INSTANCE -101 /*invalid instance*/
#define VECTOR_ERR_PARAM_NULL -102 /*param error*/


typedef int (*fun_sort_t)(void *param1, void *param2);
typedef void (*fun_each_t)(void *param);
typedef struct _c_vector
{
    int (*copy_vec)(struct _c_vector *obj, struct _c_vector *vec);
    void* (*at)(struct _c_vector *obj, unsigned index);
    void* (*front)(struct _c_vector *obj);
    void* (*back)(struct _c_vector *obj);
    void (*clear)(struct _c_vector *obj);
    unsigned char (*empty)(struct _c_vector *obj);
    int (*erase)(struct _c_vector *obj, unsigned int pos_start,int num);
    int (*insert)(struct _c_vector *obj, unsigned int index, void *data);
    int (*insert_vec)(struct _c_vector *obj, unsigned int index, struct _c_vector *vec);
    int (*push_back)(struct _c_vector *obj,void *data);
    int (*size)(struct _c_vector *obj);
    int (*swap)(struct _c_vector *obj, struct _c_vector *vec);
    int (*sort)(struct _c_vector *obj, fun_sort_t fun);
    void (*for_each)(struct _c_vector *obj, fun_each_t fun_each);
}c_vector;

int c_vector_mgr_init(void);
void c_vector_mgr_release(void);
c_vector* c_vector_create(void);
void c_vector_destroy(c_vector *obj);

#define VECTORMGR_INIT() c_vector_mgr_init()
#define VECTORMGR_RELEASE() c_vector_mgr_release()
#define VECTOR_CreateInstance(p_instance) p_instance = c_vector_create()
#define VECTOR_Release(p_instance) c_vector_release(p_instance)
#define VECTOR_Copy(obj, vec) obj->copy_vec(obj, vec)
#define VECTOR_At(obj, index) obj->at(obj, index)
#define VECTOR_Front(obj) obj->front(obj)
#define VECTOR_Back(obj) obj->back(obj)
#define VECTOR_Clear(obj) obj->clear(obj)
#define VECTOR_Empty(obj) obj->empty(obj)
#define VECTOR_Erase(obj, pos_start, num) obj->erase(obj, pos_start, num)
#define VECTOR_Insert(obj, index, data) obj->insert(obj, index, data)
#define VECTOR_InsertVec(obj, index, vec) obj->insert_vec(obj, index, vec)
#define VECTOR_PushBack(obj, data) obj->push_back(obj, data)
#define VECTOR_Size(obj) obj->size(obj)
#define VECTOR_Swap(obj, vec) obj->swap(obj, vec)
#define VECTOR_Sort(obj, fun) obj->sort(obj, fun)
#define VECTOR_ForEach(obj, fun) obj->for_each(obj, fun)


#endif // __C_VECTOR_H__
