 /*
 ============================================================================
 Name        : Lab6.c
 Author      : Austin Tian, Julian Imperial 
 Revised by  : 
 Version     :
 Copyright   : Copyright 2023
 Description : Lab 6 in C, ANSI-C Style
 ============================================================================
 */

#include "header.h"

// Menu for the testing.
char *menu =    "\n" \
                " ***********Please select the following options**********************\n" \
                " *    This is the memory operation menu (Lab 6)                     *\n" \
                " ********************************************************************\n" \
                " *    1. Write a double-word (32-bit) to the memory                 *\n"  \
                " ********************************************************************\n" \
                " *    2. Read a byte (8-bit) data from the memory                   *\n" \
                " *    3. Read a double-word (32-bit) data from the memory           *\n" \
                " ********************************************************************\n" \
                " *    4. Generate a memory dump from any memory location            *\n" \
                " ********************************************************************\n" \
                " *    e. To Exit, Type 'e'  or 'E'                                  *\n" \
                " ********************************************************************\n";

//---------------------------------------------------------------
// Generate a random number between 0x00 and 0xFF.
unsigned char rand_generator()
{
    return rand()%255;  // generate a random number between 0 and 255.
}
//---------------------------------------------------------------
void free_memory(char *base_address)
{
    free(base_address);  // free memory after use to avoid memory leakage.
    return;
}
//---------------------------------------------------------------
char *init_memory()
{
    char *mem = malloc(MEM_SIZE);  // allocating the 1MB of memory using malloc

    //Always checking if allocation of Memory was successful 
    if (mem == NULL) {
        printf("Error: Memory allocation has failed.\n");
        exit(1); //exits the program if failed
    }

    //Filling the entire memory space with random values ranging from 0x00 - 0xFF
    for (int i = 0; i < MEM_SIZE; i++) {
        mem[i] = rand_generator();
    }

    //Returing the base address of the allocated memory 
    return mem;
}

//---------------------------------------------------------------
void write_dword(const char *base_address, const int offset, const unsigned int dword_data){
    // Step 2: write a double-word to address: "base_address + offset".
    char *mem = (char *)base_address;

    //A double word is 4 bytes (32 bits)
    //Splitting the 32-bit value into 4 separate bytes 

    //Big Endian Format
    mem[offset] = (dword_data >> 24) & 0xFF;
    mem[offset + 1] = (dword_data >> 16) & 0xFF;
    mem[offset + 2] = (dword_data >> 8) & 0xFF;
    mem[offset + 3] = dword_data & 0xFF;

    //Confirmation of written operation 
    printf("Written %08X to memory at offset %X\n", dword_data, offset);
}
//---------------------------------------------------------------
unsigned char read_byte(const char *base_address, const int offset){
    // Step 3: return and print the byte from address: "base_address + offset".
    //Read a single byte for the offset
    unsigned char value = *(base_address + offset);
    
    //Printing result in HEX format (2 digits)
    printf("Byte at offset %X: %02X\n", offset, value);
    return value;
}
//---------------------------------------------------------------
unsigned int read_dword(const char *base_address, const int offset){
    // Step 4: return and print the double-word from address: "base_address + offset".
    unsigned int value = 0;

    //Reconstructing the 32-bit value from 4 consecutive bytes
    value |= ((unsigned char)*base_address + offset) << 24; //Most significant byte
    value |= ((unsigned char)*(base_address + offset + 1)) << 16;
    value |= ((unsigned char)*(base_address + offset + 2)) << 8;
    value |= ((unsigned char)*(base_address + offset + 3));

    //Printing result in 8-digit Hex Format
    printf("Double word at offset %X: %08X\n", offset, value);
    return value;
}

//---------------------------------------------------------------
void memory_dump(const char *base_address, const int offset, unsigned int dumpsize){
    // Step 5: Generate a memory dump display starting from address "base_address + offset".
    //Ensuring Dump size is within valid range
    if (dumpsize < MIN_DUMP_SIZE || dumpsize > MEM_SIZE) {
        dumpsize = MIN_DUMP_SIZE;  // make sure the min dumpsize is 256
    }

    //Loop through memory in rows of 16 bytes
    for (unsigned int i = 0; i < dumpsize; i += DUMP_LINE) 
    {
        //Print starting address of this row
        printf("%08X: ", offset + i);

        // Hex Values 
        // Print 16 bytes in Hex Format
        for (int j = 0; j < DUMP_LINE; j++) {
            unsigned char byte = *(base_address + offset + i + j);
            printf("%02X ", byte);
        }

        printf(" "); //Spacing for ASCII and Hex 

        // ASCII Representation 
        //Convert each byte to ASCCI if printable
        for (int j = 0; j < DUMP_LINE; j++) {
            unsigned char byte = *(base_address + offset + i + j);

            //Printable ASCII range is 0x20 to 0x7E
            if (byte >= 0x20 && byte <= 0x7E) {
                printf("%c", byte);
            } else {
                //Non-printable characters are shown as dots
                printf(".");
            }
            }
            printf("\n"); //New line after each row of dump
        }

    }


//---------------------------------------------------------------
void setup_memory()
{
    // Now we need to setup the memory controller for the computer system we
    // will build. Basic requirements:
    // 1. Memory size needs to be 1M Bytes
    // 2. Memory is readable/writable with Byte and Double-Word Operations.
    // 3. Memory can be dumped and shown on screen.
    // 4. Memory needs to be freed (released) at the end of the code.
    // 6. For lab 6, we need to have a user interface to fill in memory,
    //                                      read memory and do memory dump.
    char *mem = init_memory();  // initialize the memory.
    char options =0;
    unsigned int offset, dumpsize;
    char tempchar;
    unsigned int dword_data;      // 32-bit operation.
    do{
        if (options != 0x0a)  // if options has a return key input, skip it.
        {
            puts(menu); /* prints Memory Simulation */
            printf ("\nThe base address of your memory is: %I64Xh (HEX)\n", (long long unsigned int)(mem));  // output base memory first.
            puts("Please make a selection:");  // output base memory first.
        }
            options = getchar();

            switch (options)
            {
                case '1':  // write double word.
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", (unsigned int*)&offset);    // input an offset address (in HEX) to write.
                    puts("Please input your DOUBLE WORD data to be written (in HEX):");
                    scanf("%x", (unsigned int*)&dword_data);    // input data
                    write_dword (mem, offset, dword_data);  // write a double word to memory.
                    continue;
                case '2':  // read byte.
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", &offset);    // input an offset address (in HEX) to write.
                    read_byte(mem, offset);
                    continue;
                case '3':  // read double word.
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", &offset);    // input an offset address (in HEX) to write.
                    read_dword(mem, offset);
                    continue;
                case '4':  // generate memory dump starting at offset address (in HEX).
                    puts("Please input your memory's offset address (in HEX, should be a multiple of 0x10h):");
                    scanf("%x", &offset);    // input an offset address (in HEX) to start.
                    puts("Please input the size of the memory to be dumped (a number between 256 and 1024 ):");
                    scanf("%d", &dumpsize);    // The size of the memory dump
                    memory_dump(mem, offset, dumpsize);  // generate a memory dump display of dumpsize bytes.
                    continue;
                case 'e':
                case 'E':
                    puts("Code finished, press any key to exit");
                    free_memory(mem);
                    while ((tempchar = getchar()) != '\n' && tempchar != EOF);  // wait for the enter.
                    tempchar = getchar();
                    return;  // return to main program.
                default:
                    // puts("Not a valid entry, please try again");
                    continue;
            }
    }while (1);  // make sure the only exit is from 'e'.
    return;
}
