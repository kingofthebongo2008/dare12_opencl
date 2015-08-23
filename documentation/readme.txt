Description of the modules developed so far.

1. OpenCL infrastrcture
2. Algorithm phases
3. Visualization

1. In versions 1.1 to use opencl one had to ship all the source with the executables and compile them on the accelerators of the customers, moreover all errors are seen at runtime. This means when you develop your program you must run it every time, when you want to see whether your syntax in an opencl kernel is valid. In OpenCL 1.2 spir format for kernels was added which is a form of an intermediate language. It is produced from an offline compiler and kernel source codes. OpenCL drivers have ability to parse it and generate native kernels during runtime. Unfortunately, this format is supported only by Intel and AMD, NVidia support another format called ptx, which comes fron their Cuda product. We utilize the spir format by adding special tasks in the msbuild file of the visual studio project. Looks like this:

    <Intel_OpenCL_Build_Rules Include="..\src\opencl\opencl_freeform_deform_deform_points.cl">
      <FileType>Document</FileType>
      <AdditionalDependencies>$(ProjectDir)..\src\opencl\opencl_stdint.h;$(ProjectDir)..\src\opencl\opencl_radix_sort.h</AdditionalDependencies>
      <Include Condition="'$(Configuration)|$(Platform)'=='release|x64'">$(ProjectDir)..\src\opencl</Include>
      <SPIR32 Condition="'$(Configuration)|$(Platform)'=='debug|x64'">1</SPIR32>
      <SPIR32 Condition="'$(Configuration)|$(Platform)'=='release|x64'">1</SPIR32>
    </Intel_OpenCL_Build_Rules>

    The spir compiler is located in the extern/tools directory. It can be downloaded either from Khrnous. I took one version, compiled by intel.
    Spir has 2 versions 32 bit and 64 bit. In order kernels to work on most of the platforms, one must have 64bit spir kernel + 64bit host application. Other methods do not work. 

    include/opencl directory has several classes that make programming with opencl easier. They simplify api access, check for errors, provide resource managment.
    src/opencl there are all kernels that we need for our application, every kernel is in different cl file. Only 1 kernel per file.

Algorithm.

    1. Canny -> performs sobel filter for edge detection
    2. Grayscale -> converts image to grayscale
    3. Generate_Sample -> generates initial circle of patches
    4. Split the patches-> splits the patches according to the heuristings. Works like this: From every patch we have either 1 or 2 patches after the kernel execution. Since we work in parallel. Every result is independent of time. So we produce the new patches in random order and stich them after that with a sort ( bitonic sort ) procedure.
    5. Deform kernels. Patches are converted to points, Normals to the patches are calculated, Normals are converted to points also. Deform kernel is applied that fetches from the texture. After the deform, Points are stiched together to get C0 continuity and after that patches are assembled again
    6. Check for convergence with reduction.

    To support the algorithm there are several files and classes which are responsible for loading the kernels. Kernels are compiled to spir files in a temp directory and after that embedded like C arrays in the executable. This is done, so we do not have to ship 30-40 small binary files with the executable.


Visualization:
1. Visualization is done with directx. Directx has abilities to show lines and also with the help of the domain shaders and tessellation one can render bezier patches. I have developed there a library that takes the patches from opencl and loads them into directx. Everything is located src/shader directory.


so about, why flip takes so much time, the reasons are the following:
1. I have 2 papers and 1 implementation none of which seems to explain it clear enough. matlab code is not implemented also according to the papier review.pdf (normals are not calculated from bezier)
1.1. The matlab code removes one segement at a time, also it checks that then number of patches is above 30, which looks like hacks.
1.2. The papier review 1 does not explain two things. reorder  and the flipping procedures arenot at all besides a graphic
1.3. papier.pdf does not explain how topology changes are implemented except by one example: 3,8, becomes 3,6, 3,4,3,5 become 6,1,62. 

Other problems:
I see that in the papers to attract to edges some form of gaussain blurs must be performed on the image. this is not done, when the image is sampled, also it should be some kind of diffusion transform. This the reason that the curve does not stich to the boundary nicely enough.

Overall from these incomplete pieces i am trying to get viable information, which has nothing to do with either opencl, nor cuda, this is why it takes time.

1. So things that i do not understand in algorithm 4

1. It is written -> Do local test intersection on neighbouring patches -> this is fine
2. They say if not empty then do disconnect, ok, if it is empty we do what? Now i do nothing.
2. So how we are supposed to process the list, say we find 


1. Initialization of cuda or opencl drivers -> around 0.5-1 sec
2. Compile a kernel -> 1  - 2 seconds
3. Algorithm work -> heaviest is the deform procedure because it operates on the whole picture, + canny filter


1. Results opencl intel 4770k 8 cores
1.1. Total time 1446 ms
1.2. Algorithm time 1037 ms
1.3. Init
1.4. Split per iteration 1 ms
1.5. Deform per iteration 6 ms

1. Results opencl intel 4770k 8 cores
1.1. Total time 1446 ms
1.2. Algorithm time 1037 ms
1.3. Init
1.4. Split per iteration 1 ms
1.5. Deform per iteration 6 ms

1. Results opencl intel gpu HD4600 
1.1. Total time 1092 ms
1.2. Algorithm time 622 ms
1.3. Init
1.4. Split per iteration 2 ms
1.5. Deform per iteration 3 ms


