# dft-

<img src="/img/fourier.jpg" height="200">

An OpenMP Multi-threaded CPU Discrete Fourier Transform computation in Visual C++.

<img src="/img/formula.png">

(C++ DFT <--> OpenMP <--> CPU <--> 8 cpu threads)

# Other versions

* [VUDA - Vulkan](https://github.com/rodolphe74/Dft_Vuda)
* [CUDA - NVidia](https://github.com/rodolphe74/Dft_Cuda)

# Timing

All measures where done on img/coin.png (278x244).
Compilations flags are /GL /arch:AVX2 /favor:blend /fp:fast 

Version | Command                                    | GPU/CPU | Device                                                     | Threads  | Time (sec) |
---     | -------                                    | ------- | ------                                                     | -------: | ----:      |
OpenMP  | Measure-Command { .\x64\Release\dft+.exe } | CPU     | Intel Core i5-8300H @ 2.30/4 GHz                           | 1        | 173        |
OpenMP  | Measure-Command { .\x64\Release\dft+.exe } | CPU     | Intel Core i5-8300H @ 2.30/4 GHz                           | 8        | 38         |
OpenMP  | Measure-Command { .\x64\Release\dft+.exe } | CPU     | AMD Ryzen 5 3400GE with Radeon Vega Graphics @ 3.30 GHz    | 1        | 174        |
OpenMP  | Measure-Command { .\x64\Release\dft+.exe } | CPU     | AMD Ryzen 5 3400GE with Radeon Vega Graphics @ 3.30 GHz    | 8        | 36         |
VUDA    | Measure-Command { .\x64\Release\dft+.exe } | GPU     | Intel Core i5-8300H @ 2.30/4 GHz - UHD Graphics 630        | 256      | 332        |
VUDA    | Measure-Command { .\x64\Release\dft+.exe } | GPU     | Intel Core i5-8300H @ 2.30/4 GHz - NVIDIA GeForce GTX 1050 | 256      | 40         |
VUDA    | Measure-Command { .\x64\Release\dft+.exe } | GPU     | AMD Ryzen 5 3400GE with Radeon Vega Graphics @ 3.30 GHz    | 256      | 53         |
CUDA    | Measure-Command { .\x64\Release\dft+.exe } | GPU     | Intel Core i5-8300H @ 2.30/4 GHz - NVIDIA GeForce GTX 1050 | 278      | 10         |

# Conclusion

If your algorithm can be parallelized and it's possible to limit memory transfer between host and gpu device, GPU parallelization can bring you inordinate speed. The CUDA version on a good GPU chip shows a ratio of 17 compared to 1 thread CPU version.
That said, programming with OpenMP is far more easy and we can achieve good stopwatches with strong portability.
A possibility which was not explored would be to use OpenMP over GPU but it seems that the software is at the very first step.

