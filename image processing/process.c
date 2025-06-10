/* This coursework specification, and the example code provided during the
 * course, is Copyright 2024 Heriot-Watt University.
 * Distributing this coursework specification or your solution to it outside
 * the university is academic misconduct and a violation of copyright law. */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The RGB values of a pixel. */
struct Pixel
{
    int red;
    int green;
    int blue;
};

/* An image loaded from a file. */
struct Image
{
    /* TODO: Question 1 */
    int width;
    int height;
    struct Pixel *pixels;
};

/* Free a struct Image */
void free_image(struct Image *img)
{
    /* TODO: Question 2a */
    if (img != NULL)
    {
        free(img->pixels);
        free(img);
    }
}

/* Opens and reads an image file, returning a pointer to a new struct Image.
 * On error, prints an error message and returns NULL. */
struct Image *load_image(const char *filename)
{
    /* Open the file for reading as first in task*/
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("file couldn't be opened.\n");
        return NULL;
    }
    /* Read the header */
    char format[6]; // to store "HSHEX\0"
    int width, height;
    if (fscanf(f, "%5s %d %d", format, &width, &height) != 3 || strcmp(format, "HSHEX") != 0)
    {
        printf("invalid header in the file.\n");
        fclose(f);
        return NULL;
    }
    /* Allocate memory for the image  */
    struct Image *img = malloc(sizeof(struct Image));
    if (img == NULL)
    {
        printf("memory allocation failed.\n");
        fclose(f);
        return NULL;
    }
    img->width = width;
    img->height = height;

    /* Allocate memory for pixels */
    img->pixels = malloc(width * height * sizeof(struct Pixel));
    if (img->pixels == NULL)
    {
        printf("memory allocation failed.\n");
        fclose(f);
        free(img);
        return NULL;
    }
    /* Read pixel data from file*/
    for (int i = 0; i < width * height; i++)
    {
        unsigned short int red, green, blue;
        if (fscanf(f, "%hx %hx %hx", &red, &green, &blue) != 3)
        {
            printf("error readding pixel data.\n");
            fclose(f);
            free(img->pixels);
            free(img);
            return NULL;
        }
        img->pixels[i].red = red;
        img->pixels[i].green = green;
        img->pixels[i].blue = blue;
    }

    /* Close the file */
    fclose(f);

    return img;
}

/* Write img to file filename. Return true on success, false on error. */
bool save_image(const struct Image *img, const char *filename)
{
    /* TODO: Question 2c */

    /* Open the file for writin*/
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        fprintf(stderr, "file %s couldn't be opened for writing.\n", filename);
        return false;
    }

    /* Write the beginning of the output file */
    fprintf(f, "HSHEX %d %d\n", img->width, img->height);

    /* Write pixel data aftetr*/
    for (int i = 0; i < img->width * img->height; i++)
    {
        fprintf(f, "%04hx %04hx %04hx\n", img->pixels[i].red, img->pixels[i].green, img->pixels[i].blue);
    }

    /* Close the file */
    fclose(f);

    return true;
}
/* Allocate a new struct image and copy an existing struct image's contents*/
/* into it. On error, returns NULL. */
struct Image *copy_image(const struct Image *source)
{
    /* TODO: Question 2d */
    /*Allocate memmory for the new image*/
    struct Image *new_img = malloc(sizeof(struct Image));
    if (new_img == NULL)
    {
        fprintf(stderr, "memory allocation failed.\n");
        return NULL;
    }
    new_img->width = source->width;
    new_img->height = source->height;
    /*Allocate memory for the pixelss */
    new_img->pixels = malloc(source->width * source->height * sizeof(struct Pixel));
    if (new_img->pixels == NULL)
    {
        fprintf(stderr, "memory allocation failed.\n");
        free(new_img);
        return NULL;
    }
    /*Copy pixel data*/
    memcpy(new_img->pixels, source->pixels, source->width * source->height * sizeof(struct Pixel));
    return new_img;
}

/* Perform your first task.
 * function whitch replace some pixels with red or green or blue colors
 * with a specific opacity and you can apply different strength
 * Returns a new struct Image containing the result */
/* TODO: Question 3 */
struct Image *apply_NOISE(const struct Image *source, int noise_strength)
{
    /* Allocate memory for the new image */
    struct Image *noisy_img = copy_image(source);
    if (noisy_img == NULL)
    {
        printf("failed to copy image.\n");
        return NULL;
    }
    /* Apply noise to pixels */
    for (int i = 0; i < noisy_img->width * noisy_img->height; i++)
    {
        /* Generate random noise values in the given range */
        int red_noise = rand() % (2 * noise_strength + 1) - noise_strength;
        int green_noise = rand() % (2 * noise_strength + 1) - noise_strength;
        int blue_noise = rand() % (2 * noise_strength + 1) - noise_strength;

        /* Add noise tothe pixel components */
        noisy_img->pixels[i].red += red_noise;
        noisy_img->pixels[i].green += green_noise;
        noisy_img->pixels[i].blue += blue_noise;
        /* Ensure thatt pixel values are within the valid range looks huge
         * but I couldn't think of anything better :( */
        if (noisy_img->pixels[i].red < 0)
            noisy_img->pixels[i].red = 0;
        if (noisy_img->pixels[i].green < 0)
            noisy_img->pixels[i].green = 0;
        if (noisy_img->pixels[i].blue < 0)
            noisy_img->pixels[i].blue = 0;
        if (noisy_img->pixels[i].red > 255)
            noisy_img->pixels[i].red = 255;
        if (noisy_img->pixels[i].green > 255)
            noisy_img->pixels[i].green = 255;
        if (noisy_img->pixels[i].blue > 255)
            noisy_img->pixels[i].blue = 255;
    }

    return noisy_img;
}

/* Perform your second task.
 * this function computes a histogram of the average value of the red, green,
 *and blue components of each pixel in the image then prints the histogram,
 * Returns text in terminal*/
/* TODO: Question 4 */
void apply_HIST(const struct Image *source)
{
    int histogram[256] = {0};
    /* Compute and  print histogram */
    for (int i = 0; i < source->width * source->height; i++)
    {
        int pixel_value = (source->pixels[i].red + source->pixels[i].green + source->pixels[i].blue) / 3;
        histogram[pixel_value]++;
    }
    for (int i = 0; i < 256; i++)
    {
        printf("Value %d: %d pixels\n", i, histogram[i]);
    }
}
int main(int argc, char *argv[])
{
    /* Initialise the random number generator, using the time as the seed */
    /* TODO: Remove this if you don't need to use rand() */
    srand(time(NULL));

    /* Check command-line arguments */
    if (argc != 4)
    {
        fprintf(stderr, "Usage: process INPUTFILE OUTPUTFILE\n");
        return 1;
    }

    /* Load the input image */
    struct Image *in_img = load_image(argv[1]);
    if (in_img == NULL)
    {
        return 1;
    }

    /* Apply the first process */
    int noise_strength = atoi(argv[3]);
    struct Image *noisy_img = apply_NOISE(in_img, noise_strength);
    if (noisy_img == NULL)
    {
        fprintf(stderr, "Noise process failed.\n");
        free_image(in_img);
        return 1;
    }

    /* Apply the second process */
    apply_HIST(noisy_img);

    /* Save the output image */
    if (!save_image(noisy_img, argv[2]))
    {
        fprintf(stderr, "Saving image to %s failed.\n", argv[2]);
        free_image(in_img);
        free_image(noisy_img);
        return 1;
    }

    free_image(in_img);
    free_image(noisy_img);
    return 0;
}
