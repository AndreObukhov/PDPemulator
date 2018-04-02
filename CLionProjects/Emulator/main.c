#include <stdio.h>
#include <assert.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

byte mem[64*1024];

#define LO(x) ((x) & 0xFF)
#define HI(x) (((x) >> 8) & 0xFF)

byte b_read (adr a) {
    return mem[a];
}

void b_write (adr a, byte val) {
    mem[a] = val;
}

void w_write (adr a, word val) {
    assert(a % 2 == 0);

    mem[a] = (byte)(val & 0xFF);
    mem[a + 1] = (byte)((val >> 8) & 0xFF);
}

word w_read (adr a) {
    assert(a % 2 == 0);
    word w = (word)(mem[a + 1]);
    w = w << 8;
    w = w | (word)(mem[a]);
    return w;
}

void load_file() {
    FILE *f = stdin;
    unsigned int address;
    unsigned int n;
    unsigned int x;
    unsigned int i = 0;
    while(fscanf(f, "%x%x", &address, &n) == 2)
    {
        for(i = 0; i < n; i ++)
        {
            fscanf(f, "%x", &x);
            b_write(address + i, x);
        }
    }
}

void mem_dump(adr start, word n) {
    assert(n % 2 == 0);
    int i = 0;
    word w;

    for (i = 0; i < n; i += 2)
    {
        w = w_read(start + i);
        printf("%06o : %06o\n", start + i, w);
    }
}

void testmem(){
    byte b0, b1;
    word w;
    b0 = 0x0a;
    b1 = 0x0b;
    b_write(2, b0);
    b_write(3, b1);
    w = w_read(2);
    printf("%04x = %02x%02x\n", w, b1, b0);

    w = 0x0c0d;
    w_write(4, w);
    b0 = b_read(4);
    b1 = b_read(5);
    printf("%04x = %02x%02x\n", w, b1, b0);

    assert(b1 == 0x0c);
    assert(b0 == 0x0d);
}

int main() {
    testmem();
    return 0;
}