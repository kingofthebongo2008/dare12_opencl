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



