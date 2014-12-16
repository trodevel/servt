/*

Generic Server Template.

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

// $Id: server_t.h 1264 2014-12-15 18:14:59Z serge $

#ifndef SERVT_SERVER_T_H
#define SERVT_SERVER_T_H

#include <list>                         // std::list
#include <atomic>                       // std::atomic
#include "../utils/wrap_mutex.h"        // SCOPE_LOCK
#include "../threcon/i_controllable.h"  // threcon::IControllable

#include "namespace_lib.h"       // NAMESPACE_SERVT_START

NAMESPACE_SERVT_START

template <class _OBJ, class _IMPL>
class ServerT: virtual public threcon::IControllable
{
public:
    ServerT();
    ~ServerT();

    bool consume( _OBJ obj );

    bool start();

    // interface threcon::IControllable
    bool shutdown();

protected:
    typedef std::list< _OBJ > ObjectQueue;

protected:
    boost::mutex        mutex_;
    boost::thread       worker_;
    std::atomic<bool>   is_done_;

    ObjectQueue         input_queue_;
    _IMPL               * impl_;
};



template <class _OBJ, class _IMPL>
ServerT<_OBJ,_IMPL>::ServerT():
    is_done_( false ),
    impl_( nullptr )
{
}

template <class _OBJ, class _IMPL>
ServerT<_OBJ,_IMPL>::~ServerT()
{
}

template <class _OBJ, class _IMPL>
bool ServerT<_OBJ,_IMPL>::consume( _OBJ obj )
{
    SCOPE_LOCK( mutex_ );

    input_queue_.push_back( obj );

    return true;
}

template <class _OBJ, class _IMPL>
bool ServerT<_OBJ,_IMPL>::start()
{
    worker_ = boost::thread( [=]()
    {
        while( true )
        {
            if( is_done_ )
                break;

            _OBJ obj = nullptr;

            {
                SCOPE_LOCK( mutex_ );

                if( input_queue_.empty() == false )
                {
                    obj = input_queue_.front();

                    input_queue_.pop_front();
                }
            }

            if( obj != nullptr )
            {
                impl_->handle( obj );
            }

            THREAD_SLEEP_MS( 1 );
        }

        impl_->shutdown();
    } );
};

template <class _OBJ, class _IMPL>
bool ServerT<_OBJ,_IMPL>::shutdown()
{
    is_done_    = true;
    worker_.join();
}


NAMESPACE_SERVT_END

#endif  // SERVT_SERVER_T_H
