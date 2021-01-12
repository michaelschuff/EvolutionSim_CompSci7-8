//
//  garbage.hpp
//  CompSci78
//
//  Created by Michael Schuff on 1/11/21.
//  Copyright © 2021 Michael Schuff. All rights reserved.
//

#ifndef garbage_hpp
#define garbage_hpp

#define HandleKeyPressedEvent switch (event.key.code) { \
case Keyboard::A: \
    aDown = true; \
    break; \
case Keyboard::D: \
    dDown = true; \
    break; \
case Keyboard::S: \
    sDown = true; \
    break; \
case Keyboard::W: \
    wDown = true; \
    break; \
case Keyboard::I: \
    iDown = true; \
    break; \
case Keyboard::J: \
    jDown = true; \
    break; \
case Keyboard::K: \
    kDown = true; \
    break; \
case Keyboard::L: \
    lDown = true; \
    break; \
case Keyboard::LShift: \
    qDown = true; \
    break; \
case Keyboard::LControl: \
    lcontrolDown = true; \
    break; \
case Keyboard::Space: \
    eDown = true; \
    break; \
case Keyboard::Left: \
    aDown = true; \
    break; \
case Keyboard::Right: \
    dDown = true; \
    break; \
case Keyboard::Up: \
    wDown = true; \
    break; \
case Keyboard::Down: \
    sDown = true; \
    break; \
default: \
    break; \
}



#define HandleKeyReleasedEvent switch (event.key.code) { \
case Keyboard::A: \
    aDown = false; \
    break; \
case Keyboard::D: \
    dDown = false; \
    break; \
case Keyboard::S: \
    sDown = false; \
    break; \
case Keyboard::W: \
    wDown = false; \
    break; \
case Keyboard::I: \
    iDown = false; \
    break; \
case Keyboard::J: \
    jDown = false; \
    break; \
case Keyboard::K: \
    kDown = false; \
    break; \
case Keyboard::L: \
    lDown = false; \
    break; \
case Keyboard::LShift: \
    qDown = false; \
    break; \
case Keyboard::LControl: \
    lcontrolDown = false; \
    break; \
case Keyboard::Space: \
    eDown = false; \
    break; \
case Keyboard::Left: \
    aDown = false; \
    break; \
case Keyboard::Right: \
    dDown = false; \
    break; \
case Keyboard::Up: \
    wDown = false; \
    break; \
case Keyboard::Down: \
    sDown = false; \
    break; \
default: \
    break; \
}


#define MoveCamera /*Vector2i mouse_position = Mouse::getPosition(window); \
if (mouse_position.x != width / 2) { \
    cam.rotate(vector3(0, 1, 0), sensitivity * -(mouse_position.x - width/2) * 3.14159 / 180.0); \
} \
if (mouse_position.y != height / 2 && cam.up.rotated(cam.right, sensitivity * -(mouse_position.y - height/2) * 3.14159 / 180.0).y > 0) { \
    cam.rotate(cam.right, sensitivity * -(mouse_position.y - height/2) * 3.14159 / 180.0); \
} \
Mouse::setPosition(Vector2i(width/2, height/2), window); */ \
if (iDown) { \
    cam.rotate(cam.right, sensitivity * 20 * 3.14159 / 180.0); \
} if (kDown) { \
    cam.rotate(cam.right, -sensitivity * 20 * 3.14159 / 180.0); \
} if (jDown) { \
    cam.rotate(vector3(0, 1, 0), sensitivity * 20 * 3.14159 / 180.0); \
} if (lDown) { \
    cam.rotate(vector3(0, 1, 0), -sensitivity * 20 * 3.14159 / 180.0); \
} if (wDown) { \
    cam.position += speed * cam.forward / 60.0; \
} if (sDown) { \
    cam.position -= speed * cam.forward / 60.0; \
} if (aDown) { \
    cam.position -= speed * cam.right / 60.0; \
} if (dDown) { \
    cam.position += speed * cam.right / 60.0; \
} if (eDown) { \
    cam.position += speed * cam.up / 60.0; \
} if (qDown) { \
    cam.position -= speed * cam.up / 60.0; \
}
#endif /* garbage_hpp */
