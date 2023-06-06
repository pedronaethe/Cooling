# Cooling Table Generator and Test Scripts
Author: Pedro Naethe Motta (pedronaethemotta@usp.br)

This code generates a radiative cooling table following the equations from [Esin et al. 1996](https://ui.adsabs.harvard.edu/abs/1996ApJ...465..312E). The radiative cooling table depends on the parameters $H$, $B$, $T_e$, $n_e$ (scale height, magnetic field, electron temperature and electron number density) in CGS, using texture memory in CUDA. 

This code also generates a coulomb collision lookup table following [Sadowski et al. 2017](https://doi.org/10.1093/mnras/stw3116). The coulomb collisions model depends on the parameters $n_e$, $T_i$ and $T_e$ (electron number density, ion temperature and electron temperature) in CGS, using texture memory in CUDA.

I'm leaving a cooling_table.txt and source_coulomb.txt in the repository for comparison.

## Running the code and generating the cooling/coulomb table

### 1. Compile parameters_generator.c and run in order to generate four .txt files containing values for $B$, $n_e$, $T_e$ and $H_T$. 

Standard code provides you 32 values for each. These are going to be the parameters used to generate the cooling/coulomb tables. To compile type in the terminal inside the folder:

```$gcc parameters.c -o parameters -lm```

To run:

```$./parameters$```

You'll see four ```.txt``` files inside the folder: scale_height.txt, mag.txt, ne.txt and te.txt. These are the parameters list. For the coulomb table, I use the same table for $T_i$ and $T_e$, meaning they span the same interval.

**Note: Please, becareful, if you wish to change the number of values for each parameters, for example, from 100 to 200, you need to change the other .c/.cu files as well because they were made for my case.**

### 2. Compile cooling_table.c/cooling_table_threaded.c/cooling_table_threaded_mpi.c and run 

This will generate a .txt file containing a table $(32 \times 32 \times 32 \times 32)$ with parameters $H$, $B$, $n_e$ and $T_e$ and cooling values. The first line of the file indicates what each column represents. If you want, you can generate a table for each cooling component, you just have to activate the respective switch. Let's say you want to generate a individual table for the blackbody cooling, you have to adjust the switch as ```#define BLACKBODYTEST(1)```. If you want the total cooling table, just make sure that all the ```TEST``` switches are deactivated.

To compile cooling_table.c, type in the terminal:

```$gcc cooling_table.c -o coolingtable -lm```

To run, type:

```$./coolingtable```

This will generate a cooling_table.txt, which is where the cooling table is located. 

**Note: This may take a while, probably around >1h depending on your CPU**

With respect to cooling_table_threaded.c: it does the same as cooling_table.c, but it parallelizes the process in multiple threads, so it's much faster. I'm leaving both versions of the code here. It has the same feature to generate table for the components of the cooling. To compile:

To compile cooling_table_threaded.c, type in the terminal:

```$gcc -fopenmp cooling_table_threaded.c -o cooling_table_threaded```

To run, type:

```$./cooling_table_threaded```

With respect to cooling_table_threaded_mpi.c: it uses MPI combined with OpenMP so you can divide the task of calculating the tables into multiple processes and threaded. This is useful if you are trying to compute very high resolution tables, for example $100^4$ elements. 

To compile cooling_table_threaded_mpi.c, type in the terminal

```$mpicc -o cooling_mpi cooling_table_threaded_mpi.c -lm -fopenmp```

To run in computer with a single processor, you can divide into multiple processes by typing:

```$mpirun -np <number_of_processes> ./cooling_mpi```

It is useful to run this at clusters so you can take advantage of multiple processors. In this case, each job submission file will depend on the system. I suggest you look up the manual for the specific system and how to use MPI+OpenMP in it.

### 3. Compile coulomb_table.c and run

This will generate a .txt file containing a table $(100 \times 100 \times 100)$ with parameters $n_e$, $T_i$ and $T_e$ and coulomb values. The first line of the file indicates what each column represents.
To compile coulomb_table.c, type in the terminal:

```$gcc coulomb_table.c -o coulomb_table -lm```

To run, type:

```$./coulomb_table```

### 4. Compile cooling_texture.cu and run 

Do this in order to test if the texture memory fetching is correct. This will help to introduce a faster cooling in GPU based GRMHD simulations. **This will work in Nvidia GPUs only**. 

To compile:

```$nvcc -arch=sm_60 -o cooling_texture cooling_texture.cu -lm```

Change the number of your GPU compute capability, replace "60" with your own ```-arch=sm_XX```. You can find its number [here](https://developer.nvidia.com/cuda-gpus)

To run:

```$./cooling_texture```

### 5. Compile coulomb_texture.cu and run 

Do this in order to test if the texture memory fetching is correct. This will help to introduce a faster cooling in GPU based GRMHD simulations. **This will work in Nvidia GPUs only**. 

To compile:

```$nvcc -arch=sm_60 -o coulomb_texture coulomb_texture.cu -lm```

Change the number of your GPU compute capability, replace "60" with your own ```-arch=sm_XX```. You can find its number [here](https://developer.nvidia.com/cuda-gpus)

To run:

```$./coulomb_texture```

Below is a mini tutorial showing you how to find the cooling/coulomb value in the texture memory. One is advised to test whether the values are being correctly fetched.

## Testing Texture Memory

**The next line describes how the cooling_texture.cu handles the fetching. The case for the coulomb_texture.cu is analogous and won't be written here.**

It is important to test whether texture memory is fetching the right values for different combination of the parameters. To do so you'll need to run cooling_texture.cu as described above. When you run the code, you'll automatically be asked to give values for $H$, $B_{mag}$, $n_e$ and $T_e$. Pick a value from the table and confirm that it's going to print a cooling value matching the cooling table.

The code is also going to print the value of each coordinate in the texture grid. If you don't need this value, just ignore it. I put it there just for the sake of reference, in case one need to know which cell it is mapping onto.

If you didn't change the maximum and minimum values of each parameter and the quantity of them, you don't need to change the mapping of texture coordinates, these are standard and independent of the current values. If you want to read other tables, change the function where it says ```cooling_table.txt``` to the one desired, remember to adjust the other variables if the tables have different parameters or span different ranges.

In case you did change the maximum and minimum values, refer to this formula for mapping the 3D texture grid in normalized coordinates:

$C_V = \frac{1}{N_V}\left(\frac{log_{10}\left(\frac{V}{V_{min}}\right) * (N_V - 1)}{log_{10}\left(\frac{V_{max}}{V_{min}}\right)} + 0.5\right).$

where V refers to the coordinate you modified. My cooling tables are 4D, so I'm flattening the two last dimensions into one and generating a 3D texture with that.

**Note: If you modified the number of values for each parameter, you'll need to adjust $N_w$, $N_x$, $N_y$ and $N_z$ for each function inside this file and also in GPU_Program1.cu file in case you want to implement this in H-AMR.** 

In case you want to add this cooling table to [H-AMR](https://arxiv.org/abs/1912.10192), I advise you to check my [cooling branch](https://github.com/black-hole-group/hamr/tree/Cooling_pedro)

