/*

Generic Message Pool.

Copyright (C) 2014 Sergey Kolevatov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// $Id: message_pool.h 1281 2014-12-24 14:45:05Z serge $

#ifndef SERVT_MESSAGE_POOL_H
#define SERVT_MESSAGE_POOL_H

#include <deque>    // deque
#include <mutex>    // mutex

#include "namespace_lib.h"       // NAMESPACE_SERVT_START

NAMESPACE_SERVT_START

template <class _T>
class MessagePool
{
public:
    typedef std::deque<_T*> Type;

    ~MessagePool()
    {
    }

    static MessagePool & get()
    {
        static MessagePool pool_;

        return pool_;
    }

private:
    MessagePool()
    {
    }

public:
    Type        pool_;
    std::mutex  mutex_;
};

template <class _T>
_T * get()
{
    MessagePool<_T> & message_pool = MessagePool<_T>::get();

    std::lock_guard<std::mutex> lock( message_pool.mutex_ );

    typename MessagePool<_T>::Type & pool  = message_pool.pool_;

    _T* res = nullptr;

    if( pool.empty() )
    {
        res = new _T;
    }
    else
    {
        size_t size = pool.size() - 1UL;
        res = pool[size];
        pool.resize( size );
    }

    return res;
}

template <class _T>
void put( _T * o )
{
    MessagePool<_T> & message_pool = MessagePool<_T>::get();

    std::lock_guard<std::mutex> lock( message_pool.mutex_ );

    typename MessagePool<_T>::Type & pool  = message_pool.pool_;

    pool.push_back( o );
}

template <class _T>
void put( const _T * o )
{
    put( const_cast<_T*>( o ) );
}

NAMESPACE_SERVT_END

#endif // SERVT_MESSAGE_POOL_H
