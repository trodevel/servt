/*

IObject.

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

// $Revision: 3309 $ $Date:: 2016-01-28 #$ $Author: serge $

#ifndef WORKT_I_OBJECT_H
#define WORKT_I_OBJECT_H

#include "namespace_lib.h"    // NAMESPACE_WORKT_START

NAMESPACE_WORKT_START

class IObject
{
public:
    virtual ~IObject() {}
};

NAMESPACE_WORKT_END

#endif  // WORKT_I_OBJECT_H
