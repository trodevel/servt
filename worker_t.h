/*

Generic Worker Template.

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

// $Revision: 3540 $ $Date:: 2016-03-02 #$ $Author: serge $

#ifndef WORKT_WORKER_T_H
#define WORKT_WORKER_T_H

#include <deque>                        // std::deque
#include <atomic>                       // std::atomic
#include <thread>                       // std::thread
#include <mutex>                        // std::mutex
#include <condition_variable>           // std::condition_variable
#include "../utils/mutex_helper.h"      // MUTEX_SCOPE_LOCK

#include "namespace_lib.h"       // NAMESPACE_WORKT_START

NAMESPACE_WORKT_START

template <class _OBJ, class _IMPL>
class WorkerT
{
    typedef _IMPL Plug;

public:
    WorkerT( Plug * impl );
    ~WorkerT();

protected:

    void consume( _OBJ obj );

    void start();

    // quasi-interface threcon::IControllable
    void shutdown();

private:

    void thread_func();

    void handle_messages();

private:
    typedef std::deque< _OBJ > ObjectQueue;

private:
    std::thread                     worker_;
    std::atomic<bool>               is_done_;

    mutable std::mutex              mutex_wait_;
    mutable std::condition_variable cond_;
    mutable std::atomic<bool>       should_wakeup_myself_;

    mutable std::mutex              input_queue_mutex_;
    ObjectQueue                     input_queue_;
    _IMPL                           * impl_;
};



template <class _OBJ, class _IMPL>
WorkerT<_OBJ,_IMPL>::WorkerT( Plug * impl ):
    is_done_( false ),
    should_wakeup_myself_( false ),
    impl_( impl )
{
}

template <class _OBJ, class _IMPL>
WorkerT<_OBJ,_IMPL>::~WorkerT()
{
}

template <class _OBJ, class _IMPL>
void WorkerT<_OBJ,_IMPL>::consume( _OBJ obj )
{
    MUTEX_SCOPE_LOCK( input_queue_mutex_ );

    input_queue_.push_back( obj );

    cond_.notify_one();
}

template <class _OBJ, class _IMPL>
void WorkerT<_OBJ,_IMPL>::start()
{
    worker_   = std::thread( std::bind( & WorkerT<_OBJ,_IMPL>::thread_func, this ) );
};

template <class _OBJ, class _IMPL>
void WorkerT<_OBJ,_IMPL>::thread_func()
{
    while( is_done_ == false )
    {
        if( should_wakeup_myself_ )
        {
            should_wakeup_myself_ =  false;
        }
        else
        {
            std::unique_lock<std::mutex> _( mutex_wait_ );
            cond_.wait( _ );
        }

        handle_messages();

        {
            MUTEX_SCOPE_LOCK( input_queue_mutex_ );
            if( input_queue_.empty() == false )
            {
                should_wakeup_myself_   = true;
            }
        }
    }
};

template <class _OBJ, class _IMPL>
void WorkerT<_OBJ,_IMPL>::handle_messages()
{
    ObjectQueue temp;

    {
        MUTEX_SCOPE_LOCK( input_queue_mutex_ );

        if( input_queue_.empty() == false )
        {
            temp    = input_queue_;
            input_queue_.clear();
        }
    }

    for( auto c : temp )
        impl_->handle( c );
}


template <class _OBJ, class _IMPL>
void WorkerT<_OBJ,_IMPL>::shutdown()
{
    is_done_    = true;

    cond_.notify_one();

    worker_.join();
}


NAMESPACE_WORKT_END

#endif  // WORKT_WORKER_T_H
