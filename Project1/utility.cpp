typedef unsigned int uint;

inline static int clamp(int value, int min, int max) {
    return (value < min) ? min : (value > max) ? max : value;
}