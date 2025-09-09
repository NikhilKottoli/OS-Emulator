# OS-Emulator

This repository contains C implementations of various Operating System algorithms, including memory management and page replacement techniques. Each file demonstrates a specific algorithm commonly taught in OS courses.

## Contents

- **block_aootment.c**: Block allocation algorithm for memory management.
- **fifo.c**: First-In-First-Out (FIFO) page replacement algorithm.
- **File.c**: File allocation strategies (details depend on implementation).
- **freq.c**: Frequency-based page replacement algorithm.
- **IndexedAllocation.c**: Indexed file allocation method.
- **LRU.c**: Least Recently Used (LRU) page replacement algorithm.
- **mft.c**: Multiprogramming with a Fixed number of Tasks (MFT) memory management.
- **mvt.c**: Multiprogramming with a Variable number of Tasks (MVT) memory management.
- **optimal.c**: Optimal page replacement algorithm.

## How to Use

1. Compile any `.c` file using a C compiler:
	```sh
	gcc filename.c -o filename
	./filename
	```
2. Follow the prompts in each program to input required data.

## Algorithms Covered

- **Memory Management**: MFT, MVT, Block Allocation, Indexed Allocation
- **Page Replacement**: FIFO, LRU, Optimal, Frequency-based

## License

This project is for educational purposes.
