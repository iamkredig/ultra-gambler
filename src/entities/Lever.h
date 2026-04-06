#ifndef ULTRA_GAMBLER_LEVER_H
#define ULTRA_GAMBLER_LEVER_H

class Lever {
public:
    bool isAnimating = false;
    bool goingDown = true;
    const float frameTime = 0.1f;
    float timer = 0.0f;
    int frame = 0;

    void Pull();
    void Animate(float elapsedTime);
};

#endif //ULTRA_GAMBLER_LEVER_H