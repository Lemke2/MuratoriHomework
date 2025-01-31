# Intro to the Course
The course consists of multiple parts:

0. **Prelude**: High level overview of what makes programs fast or slow.
    - Waste (in terms of clock cycles)
    - Dependency chains
    - SIMD

1. **First part**: Decoding bits to ASM instructions like `mov` (`100010`), `cmp`, `add`, etc.

2. **Second part**: Simulating CPU behavior from input bits, using a library provided as a DLL and its interface (`sim86_shared.h`).

3. **Third part**: Performance profiling

---

## Part 1
Parsing binary to assembly instructions (to see how the cpu decodes instructions)

- **Homework 1.1**
Learning how the assembler decodes binary to assembly instructions, focusing on register-to-register `mov` operations.  
Finished in Java. *(Listing 38)*

Skipped the rest of the homework as it seemed mostly the same as the first one, just tedious bookkeeping.

---

## Part 2
Simulating CPU behavior.

- **Homework 2.1**: `mov` operations. (Listing 43, Listing 44)
- **Homework 2.2**: `cmp`, `add`, `sub`, `mov` instructions, as well as handling zero-flag and sign-flag. (Listing 46)
- **Homework 2.3**: `adding jumps` (Listing 49)
- **Homework 2.4**: `adding memory and flags`, simulating a CPU. (Listing 52)


---

## Part 3


Performance profiling our code (A json parser and a Haversine distance calculator)

---


## How to Run

**Homework 1**: is just normal java, hardcoded file path to listing. So run it like normal java as long as you have the matching listing file in the same dir as the code file.

**Homework 2**: is cpp. So if you are like me and don't know how to build C stuff here is a simple way to build and run homework from part 2 and onwards:

- g++ Homework24.cpp -o shared/Homework24.exe -Lshared -lsim86_shared 

- shared/Homework24.exe listings/listing_0052_memory_add_loop

This builds an executable from the homework file you declare to the shared library and links it with the interface file and the dll.


