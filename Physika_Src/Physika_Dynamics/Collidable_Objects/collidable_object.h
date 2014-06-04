/*
 * @file  collidable_object.h
 * @brief abstract base class of all collidable objects, provide common interface
 * @author Fei Zhu
 * 
 * This file is part of Physika, a versatile physics simulation library.
 * Copyright (C) 2013 Physika Group.
 *
 * This Source Code Form is subject to the terms of the GNU General Public License v2.0. 
 * If a copy of the GPL was not distributed with this file, you can obtain one at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 *
 */

#ifndef PHYSIKA_DYNAMICS_COLLIDABLE_OBJECTS_COLLIDABLE_OBJECT_H_
#define PHYSIKA_DYNAMICS_COLLIDABLE_OBJECTS_COLLIDABLE_OBJECT_H_

namespace Physika{

template <typename Scalar,int Dim> class Vector;

template <typename Scalar,int Dim>
class CollidableObject
{
public:
    CollidableObject(){}
    virtual ~CollidableObject(){}
    //given position and velocity of a ponit, resovle collision with the collidable object (detect&&resolve)
    virtual bool collide(const Vector<Scalar,Dim> &position, const Vector<Scalar,Dim> &velocity) const=0;
    //given position and velocity of a point, detect collision with the collidable object
    virtual bool detectCollision(const Vector<Scalar,Dim> &position, const Vector<Scalar,Dim> &velocity) const=0;
protected:
};

}  //end of namespace Physika

#endif //PHYSIKA_DYNAMICS_COLLIDABLE_OBJECTS_COLLIDABLE_OBJECT_H_













