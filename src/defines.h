//
//  defines.h
//  market_data_replay
//
//  Created by Fedor Lisochenko on 16.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef defines_h
#define defines_h

/** Forbids copying of the object */
#define PREVENT_COPY(class_name) class_name(const class_name&) = delete;\
class_name& operator=(const class_name&) = delete

/** Forbids moving of the object */
#define PREVENT_MOVE(class_name) class_name(class_name&&) = delete;\
class_name& operator=(class_name&&) = delete

#endif /* defines_h */
