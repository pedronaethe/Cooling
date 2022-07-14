# Cooling Table Generator and Test Scripts
Author: Pedro Naethe Motta (pedronaethemotta@usp.br)

This code generates a radiative cooling table following the equations from [Esin et al. 1996](https://ui.adsabs.harvard.edu/abs/1996ApJ...465..312E). The radiative cooling table depends on the parameters $B$, $T_e$, $n_e$ (magnetic field, electron temperature and electron number density) in CGS, using texture memory in CUDA.

## Running the code and generating the cooling table

### 1. Compile parameters_generator_mag.c and run in order to generate three .txt files containing values for $B$, $n_e$ and $T_e$. Standard code provides you 100 values for each.

To compile type in the terminal inside the folder:

```$gcc parameters_generator_mag.c -o parameters -lm```

To run:

```$./parameters$```

You'll see three ```.txt``` files inside the folder: mag.txt, ne.txt and te.txt. These are the parameters list.

**Note: Please, becareful, if you wish to change the number of values for each parameters, for example, from 100 to 200, you need to change the other .c files as well because they were made for 100 values.**

### 2. Compile cooling_table_magfield.c and run in order to generate a .txt file containing a table $(100 \times 100 \times 100)$ with parameters $B$, $n_e$ and $T_e$ and cooling values. The first line of the file indicates what each column represents.

To compile cooling_table.c, type in the terminal:

```$gcc cooling_table_magfield.c -o coolingtable -lm```

To run, type:

```$./coolingtable```

This will generate a cooling_table_log_mag.txt, which is where the cooling table is located. 

**Note: This may take a while, probably around ~40 min depending on your CPU**

### 3. Compile cooling_texture_mag.cu and run in order to test if the texture memory fetching is correct. This will help to introduce a faster cooling in GPU based GRMHD simulations. 

To compile:

```$nvcc cooling_texture_mag.cu -o coolingtexture -lm```

To run:

```$./coolingtexture```

Below is a mini tutorial showing you how to find the cooling value in the texture memory. One is advised to test whether the values are being correctly fetched.

## Testing Texture Memory

It is important to test whether texture memory is fetching the right values for different combination of the parameters. To do so you'll need to run cooling_texture_mag.cu as described above. When you run the code, you'll automatically be asked to give a $B_mag$, $n_e$ and $T_e$. Pick a value from the table and confirm that $lambda$ is equal the value you wanted.

If you didn't change tha maximum and minimum values of each parameter you don't need to change the mapping of texture coordinates, these are standard and independent of the current values. In case you did change the maximum and minimum values, refer to this formula for mapping the texture grid in normalized coordinates:

$C_V = \frac{1}{N_V}\left(\frac{log_{10}\left(\frac{V}{V_{min}}\right) * (N_V - 1)}{log_{10}\left(\frac{V_{max}}{V_{min}}\right)} + 0.5\right).$

where V refers to the coordinate you modified.

**Note: If you modified the number of values for each parameter, you'll need to adjust $N_x$, $N_y$ and $N_z$ for each function inside this file and also in GPU_Program1.cu file in case you want to implement this in H-AMR.** 

In case you want to add this cooling table to [H-AMR](https://arxiv.org/abs/1912.10192), I advise you to check my [cooling branch](https://github.com/black-hole-group/hamr/tree/Cooling_pedro)

