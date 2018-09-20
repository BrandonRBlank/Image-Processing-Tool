# Image-Processing-Tool

You don't need no stinkin' $700 photoshop to add intensity, or binarize, or scale images, or convert to grey, or apply blur, or perform histogram equalization, or use sobel edge detection, or canny edge detection, when you've got my Image Processing Tool.

That's right handsome, you can use this tool, written in Visual C++, to do all that, for F R E E!

## Licensing
Feel free to use at your leasure. Don't steal this please :)


## This project is built under Visual Studio 2015 with OpenCV 3.1 under Windows 7 64-bit

This software is architectured as follows:
* iptool/iptool.cpp- This file includes the main function.
* iptool/iptools -This folder hosts the files that are compiled into a static library. 
* image - This folder hosts the files that define an image.
* utility- this folder hosts the files with implemented image algorithms.

### INSTALATION 

On Windows

Open the project by double click iptool.sln.
The environment is setting follow the link:
http://opencv-srf.blogspot.com/2013/05/installing-configuring-opencv-with-vs.html

### Run the program:

Directly debug in Visual Studio.
You can find the output image in output folder.

### FUNCTIONS 

1. Add intensity: add
Increase the intensity for a gray-level image.
parameter: intensity

2. Binarization: binarize
Binarize the pixels with the threshold.
parameter: threshold

3. Scaling: Scale
Reduce or expand the heigh and width with twp scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.
parameter: scaling_factor

### FUNCTIONS (with OpenCV) 

1. Gray Scale: gray
Change whole image to gray-level.
parameter: no

2. Average Blur: blur_avg
Uniform smoothing.
parameter: window_size

3. Histogram Equalization: histEqual
Equalizes histogram for all color channels.
parameter: ROI begin x, y and ROI size x, y; as well as number of ROIs (Max 3)

4. Sobel Edge Detection (dy and dy): sobel
Performs edge detection using the Sobel operator.
parameter: ROI begin x, y and ROI size x, y; as well as number of ROIs (Max 3)

5. Canny  Edge Detection: canny
Performs edge detection using canny operator
parameter: ROI begin x, y and ROI size x, y; as well as number of ROIs (Max 3)

### NOTE: 

Entering 4 for number of ROIs will run combination of histogram equalization and the desired edge detector on image
Entering 0 for number of ROIs will perform operation on whole image

### PARAMETERS 

The first parameter of the parameters.txt is the number of operations (lines).
There are four parameters for each operation (line):
1. the input file name;
2. the output file name;
3. opencv / FUNCTIONS (without OpenCV): skip to 5 for parameters
4. the name of the function with OpenCV. Check FUNCTIONS (with OpenCV).
5. parameters (Check FUNCTIONS (with OpenCV) for parameters)

### EXAMPLE:
---
1

../input/11.jpg ../output/11_cannyHist.jpg opencv canny 2

100 100 200 200

300 100 200 200

---

This will perform Canny operator (from openCV) on 1 image (11.jpg) with 2 ROIs 
(first at (100, 100)size: 200x200 and second at (300, 100) size: 200x200)

### Important information 

Application assumes the next format of input image (ppm/pgm) file:
line1: <version>
line2: <#columns> <#rows>
line3: <max_value>
line4-end-of-file:<pix1><pix2>...<pix_n>

if it is a grayscale image then every pixel is a char value. If it is a RGB image then every pixel is a set of 3 char values: <red><green><blue>

Thus, if you have a problem with reading image, the first thing you should check is input file format.
