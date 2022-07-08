# dft-

<img src="/img/fourier.jpg" height="200">

An OpenMP Multi-threaded CPU Discrete Fourier Transform computation in Visual C++.

<img src="/img/formula.png">

(C++ DFT <--> OpenMP <--> CPU <--> 8 cpu threads)

# Other versions

* [VUDA - Vulkan](https://github.com/rodolphe74/Dft_Vuda)
* [CUDA - NVidia](https://github.com/rodolphe74/Dft_Cuda)

# Timing
Version | Command                                    | GPU/CPU | Device                                                  | Threads  | Time (sec) |
---     | -------                                    | ------- | ------                                                  | -------: | ----:      |
OpenMP  | Measure-Command { .\x64\Release\dft+.exe } | CPU     | Intel Core i5-8300H @ 2.30/4 GHz                        | 1        | 173        |
OpenMP  | Measure-Command { .\x64\Release\dft+.exe } | CPU     | Intel Core i5-8300H @ 2.30/4 GHz                        | 8        | 38         |
OpenMP  | Measure-Command { .\x64\Release\dft+.exe } | CPU     | AMD Ryzen 5 3400GE with Radeon Vega Graphics @ 3.30 GHz | 1        | 174        |
OpenMP  | Measure-Command { .\x64\Release\dft+.exe } | CPU     | AMD Ryzen 5 3400GE with Radeon Vega Graphics @ 3.30 GHz | 8        | 36         |



