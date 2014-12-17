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

// $Id: i_object.h 1266 2014-12-16 19:17:06Z serge $

#ifndef SERVT_I_OBJECT_H
#define SERVT_I_OBJECT_H

#include "namespace_lib.h"    // NAMESPACE_SERVT_START

NAMESPACE_SERVT_START

class IObject
{
public:
    virtual ~IObject() {}
};

NAMESPACE_SERVT_END

#endif  // SERVT_I_OBJECT_H
