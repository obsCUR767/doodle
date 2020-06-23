#pragma once
#include <stdlib.h>
#include <string.h>




struct obList
{
    void* data;
    obList* next;
    obList* prev;
};


inline obList* listInsertSH(obList* _where, void* _what)
{

    obList* node = (obList*)malloc(sizeof(obList));
    if (!node) return NULL;
    node->data = _what;
    node->next = _where->next;
    node->prev = _where;
    _where->next->prev = node;
    _where->next = node;
    return node;
}

inline obList* listStartSH( void* _what)
{
    obList* _l = (obList*)malloc(sizeof(obList));
    if (!_l) 
        return NULL;

    _l->prev = _l->next = _l;
    _l->data = _what;
    return _l;
}


inline obList* listInsertDP(obList* _where, const void* _what, const size_t _sz)
{
    void* dataCopy = malloc(_sz);
    memcpy(dataCopy, _what, _sz);
    obList* node = listInsertSH(_where, dataCopy);

    if (node)
    {
        free(dataCopy);
        dataCopy = NULL;
    };

    return node;
}

inline obList* listStartDP(void* _what, const size_t _sz)
{
    obList* _l = (obList*)malloc(sizeof(obList));
    if (!_l)
        return NULL;

    _l->prev = _l->next = _l;
    listInsertDP(_l, _what, _sz);
    return _l;
}



inline obList* listRemoveSH(obList* _where)
{
    obList* prev = _where->prev;
    obList* next = _where->next;
    prev->next = _where->next;
    next->prev = _where->prev;
    free(_where);
    _where = NULL;
    return prev;
}

inline void listClearSH(obList* _l)
{
    do
    {
        _l = listRemoveSH(_l);
    } while (_l != _l->prev);

    listRemoveSH(_l);
}
inline void listClearDP(obList* _l)
{
    do
    {
        free(_l->data);
        _l->data = NULL;
        _l = listRemoveSH(_l);
    }
    while (_l != _l->prev);

    free(_l->data);
    _l->data = NULL;

    listRemoveSH(_l);
}

inline void listIterOper(obList* _l, void (*_cb)(void*))
{
    obList* _itr = _l;
    do
    {
        (*_cb)(_itr->data);
        _itr = _itr->next;
    } while (_itr != _l);
}


