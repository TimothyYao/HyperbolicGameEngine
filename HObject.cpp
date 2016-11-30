//
//  HObject.cpp
//  hyperbolic game engine
//
//  Created by Timothy Yao on 11/30/16.
//  Copyright © 2016 Timothy Yao. All rights reserved.
//

#include "HObject.h"

HObject::HObject(HPosition location, std::string name) {
    this->location = location;
    this->name = name;
}
