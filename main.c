#include <stdio.h>

enum {
    white, blue, orange, green, red, yellow
};

int getBit(int num, int bit) {
    return (num >> bit * 4) & 0xF;
}

void rotateInt(int *num, int magnitude) { //rotates with respect to hex notation
    int res = *num << magnitude * 4;
    res |= *num >> (magnitude - 8) * 4;
    *num = res;
}

void rotateFace(int *side, int rotations) {
    rotateInt(side, rotations * 2); //*2 to keep edge pieces edge pieces etc.
}

void printFace(int *cube, int face) {
    printf("%08x", cube[white]);
}

void printCube(int *cube) {

}

/*
 * Rotation is in clockwise direction, colors definded as enumerator
 */


int main() {
    int cube[6];
    cube[white] = 0x00000001;
    cube[blue] = 0x11113111;
    cube[orange] = 0x22222222;
    cube[green] = 0x33333333;
    cube[red] = 0x44444444;
    cube[yellow] = 0x55555555;
    rotateFace(&cube[white], 3);
    printFace(cube, white);
    return 0;
}
