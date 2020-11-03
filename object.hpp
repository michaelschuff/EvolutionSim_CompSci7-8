//
//  object.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/2/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef object_hpp
#define object_hpp

#include "color.hpp"

class object {
public:
    bool isVisable;
    
    virtual std::string to_string() = 0;
    virtual void print() = 0;
    
protected:
    object(bool visable = true) : isVisable(visable) {}
};


#endif /* object_hpp */
