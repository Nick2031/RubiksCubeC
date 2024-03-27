#include <stdio.h>
#include <time.h>
#include <unistd.h>

enum {
    w, b, o, g, r, y
};

char cL(int num) { //short for code lookup
    switch (num) {
        case 0:
            return 'w';
        case 1:
            return 'b';
        case 2:
            return 'o';
        case 3:
            return 'g';
        case 4:
            return 'r';
        case 5:
            return 'y';
    }
}

int getBit(int num, int bit) {
    return (num >> bit * 4) & 0xF;
}

void printCube(int *cube) {
    printf("   %c%c%c      \n", cL(getBit(cube[y], 0)), cL(getBit(cube[y], 1)), cL(getBit(cube[y], 2)));
    printf("   %cy%c      \n", cL(getBit(cube[y], 7)), cL(getBit(cube[y], 3)));
    printf("   %c%c%c       \n", cL(getBit(cube[y], 6)), cL(getBit(cube[y], 5)), cL(getBit(cube[y], 4)));
    printf("%c%c%c%c%c%c%c%c%c%c%c%c\n", cL(getBit(cube[o], 2)), cL(getBit(cube[o], 3)), cL(getBit(cube[o], 4)),
           cL(getBit(cube[b], 2)), cL(getBit(cube[b], 3)), cL(getBit(cube[b], 4)), cL(getBit(cube[r], 0)),
           cL(getBit(cube[r], 1)), cL(getBit(cube[r], 2)), cL(getBit(cube[g], 2)), cL(getBit(cube[g], 3)),
           cL(getBit(cube[g], 4)));
    printf("%co%c%cb%c%cr%c%cg%c\n", cL(getBit(cube[o], 1)), cL(getBit(cube[o], 5)),
           cL(getBit(cube[b], 1)), cL(getBit(cube[b], 5)), cL(getBit(cube[r], 7)), cL(getBit(cube[r], 3)),
           cL(getBit(cube[g], 1)), cL(getBit(cube[g], 5)));
    printf("%c%c%c%c%c%c%c%c%c%c%c%c\n", cL(getBit(cube[o], 0)), cL(getBit(cube[o], 7)), cL(getBit(cube[o], 6)),
           cL(getBit(cube[b], 0)), cL(getBit(cube[b], 7)), cL(getBit(cube[b], 6)), cL(getBit(cube[r], 6)),
           cL(getBit(cube[r], 5)), cL(getBit(cube[r], 4)), cL(getBit(cube[g], 0)), cL(getBit(cube[g], 7)),
           cL(getBit(cube[g], 6)));
    printf("   %c%c%c      \n", cL(getBit(cube[w], 0)), cL(getBit(cube[w], 1)), cL(getBit(cube[w], 2)));
    printf("   %cw%c      \n", cL(getBit(cube[w], 7)), cL(getBit(cube[w], 3)));
    printf("   %c%c%c       \n", cL(getBit(cube[w], 6)), cL(getBit(cube[w], 5)), cL(getBit(cube[w], 4)));
    printf("\n");
}

void printHex(int num);


void setBit(int *num, int bit, int data) {
    *num &= ~(0xF << bit * 4);
    *num += data << bit * 4;
}


void rotateInt(int *num, int magnitude) { //rotates with respect to hex notation
    if (magnitude == 0) {
        return;
    }
    int res = *num << (magnitude * 4);
    res |= *num >> ((8 - magnitude) * 4);
    *num = res;
}

void rotateLong(long long *num, int magnitude) {
    if (magnitude == 0) {
        return;
    }
    long long res = *num << (magnitude * 4);
    res |= *num >> (12 - magnitude) * 4;
    *num = res;
}

void rotateFace(int *side, int rotations) {
    rotateInt(side, rotations * 2); //*2 to keep edge pieces edge pieces etc.
}

int getStrip(int num, int strip) {
    rotateFace(&num, -strip);
    return num & 0xFFF;
}

long long getStripLong(long long num, int strip) {
    num = num >> (strip * 12);
    return num & 0xFFF;
}

void setStrip(int *num, int strip, int data) {
    int temp = *num;
    rotateFace(&temp, -strip);
    temp &= ~0xFFF;
    temp += data;
    rotateFace(&temp, strip);
    *num = temp;
}


void printHex(int num) {
    printf("%08x\n", num);
}


void rotateFull(int *cube, int face, int magnitude) {
    rotateFace(&cube[face], magnitude);

    int data[6];   // 0x{face3}{strip}{face2}{strip} etcccc, basically magic numbers, dont touch
    data[w] = 0x23334213; //
    data[b] = 0x00435222; //
    data[o] = 0x03105332; //
    data[g] = 0x02205041; //
    data[r] = 0x12013051; //
    data[y] = 0x21114031; //

    long long temp = 0;
    for (int i = 0; i < 4; i++) {
        int associatedFace = getBit(data[face], i * 2 + 1);
        int strip = getBit(data[face], i * 2);
        long long a = getStrip(cube[associatedFace], strip);
        temp += a << 12 * i;
    }
    rotateLong(&temp, magnitude * 3);
    for (int i = 0; i < 4; i++) {
        int associatedFace = getBit(data[face], i * 2 + 1);
        int strip = getBit(data[face], i * 2);
        int a = getStripLong(temp, i);
        setStrip(&cube[associatedFace], strip, a);
    }
}


int main() {
    int cube[6];
    cube[w] = 0x00000000;
    cube[b] = 0x11111111;
    cube[o] = 0x22222222;
    cube[g] = 0x33333333;
    cube[r] = 0x44444444;
    cube[y] = 0x55555555;
    printCube(cube);
    clock_t start = clock(), diff;
    diff = clock() - start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("%d\n", msec);
    printCube(cube);
    return 0;
}
