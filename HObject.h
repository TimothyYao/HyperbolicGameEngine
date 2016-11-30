//
//  HObject.h
//  hyperbolic game engine
//
//  Created by Timothy Yao on 11/30/16.
//  Copyright © 2016 Timothy Yao. All rights reserved.
//

#ifndef HObject_h
#define HObject_h

#include "HPosition.h"

class HObject {
public:
    HPosition location;
    std::string name;
    
    HObject(HPosition location, std::string name);
};


#endif /* HObject_h */
