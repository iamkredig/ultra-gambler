#include "./Lever.h"

void Lever::Pull() {
    if (isAnimating) return;
    isAnimating = true;
    goingDown = true;
    frame = 0;
    timer = 0.0f;
}

void Lever::Animate(float elapsedTime) {
    if (isAnimating) {
        timer += elapsedTime;
        if (timer >= frameTime) {
            timer -= frameTime;

            if (goingDown) {
                frame++;
                if (frame >= 3) {
                    goingDown = false;
                }
            } else {
                frame--;
                if (frame <= 0) {
                    frame = 0;
                    isAnimating = false;
                }
            }
        }
    }
}