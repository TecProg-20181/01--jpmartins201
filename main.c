#include <stdio.h>
#define RED 0
#define GREEN 1
#define BLUE 2


typedef struct _pixel {
  unsigned short int red;
  unsigned short int green;
  unsigned short int blue;
} Pixel;

typedef struct _image {
  // [width][height][rgb]
  unsigned short int pixel[512][512][3];
  unsigned int width;
  unsigned int height;
} Image;

int pixel_media(unsigned short int pixel[512][512][3], int meter, int counter)
{
  int media = (pixel[meter][counter][0] + pixel[meter][counter][1] + pixel[meter][counter][2])/3;
  return media;
}

void copy(unsigned short int pixel[512][512][3], int media, int meter, int counter)
{
  pixel[meter][counter][RED] = media;
  pixel[meter][counter][GREEN] = media;
  pixel[meter][counter][BLUE] = media;
}

Image gray_scale(Image img)
{
  int meter, counter;
  for (meter = 0; meter < img.height; ++meter)
  {
    for (counter = 0; counter < img.width; ++counter)
    {
      int media = pixel_media(img.pixel, meter, counter);
      copy(img.pixel, media, meter, counter);
    }
  }
  return img;
}

void pixel_blur(Pixel media, unsigned short int pixel[512][512][3], int size, unsigned int i, unsigned int j)
{
  media.red /= size * size;
  media.green /= size * size;
  media.blue /= size * size;
  pixel[i][j][RED] = media.red;
  pixel[i][j][GREEN] = media.green;
  pixel[i][j][BLUE] = media.blue;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], int size, unsigned int width)
{
  for (unsigned int i = 0; i < height; ++i)
  {
    for (unsigned int j = 0; j < width; ++j)
    {
      Pixel media = {0, 0, 0};

      int menor_height = (height - 1 > i + size/2) ? i + size/2 : height - 1;
      int min_width = (width - 1 > j + size/2) ? j + size/2 : width - 1;
      for(int x = (0 > i - size/2 ? 0 : i - size/2); x <= menor_height; ++x)
      {
        for(int y = (0 > j - size/2 ? 0 : j - size/2); y <= min_width; ++y)
        {
          media.red += pixel[x][y][RED];
          media.green += pixel[x][y][GREEN];
          media.blue += pixel[x][y][BLUE];
        }
      }
      pixel_blur(media, pixel, size, i, j);
      // printf("%u", media.r)
    }
  }
}

void rotate_image(Image img, Image rotated)
{
  rotated.width = img.height;
  rotated.height = img.width;
}

Image rotate90right(Image img)
{
  Image rotated;
  rotate_image(img, rotated);

  for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y)
  {
    for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x)
    {
      rotated.pixel[i][j][RED] = img.pixel[x][y][RED];
      rotated.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
      rotated.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];
    }
  }
  return rotated;
}

void invert_colors(unsigned short int pixel[512][512][3],unsigned int width, unsigned int height)
{
  for (unsigned int i = 0; i < height; ++i)
  {
    for (unsigned int j = 0; j < width; ++j)
    {
      pixel[i][j][RED] = 255 - pixel[i][j][RED];
      pixel[i][j][GREEN] = 255 - pixel[i][j][GREEN];
      pixel[i][j][BLUE] = 255 - pixel[i][j][BLUE];
    }
  }
}

Image cut_image(Image img, int x, int y, int width, int height)
{
  Image cut;
  cut.width = width;
  cut.height = height;

  for(int i = 0; i < height; ++i)
  {
    for(int j = 0; j < width; ++j)
    {
      cut.pixel[i][j][RED] = img.pixel[i + y][j + x][RED];
      cut.pixel[i][j][GREEN] = img.pixel[i + y][j + x][GREEN];
      cut.pixel[i][j][BLUE] = img.pixel[i + y][j + x][BLUE];
    }
  }
  return cut;
}

void print_pixels(Image img)
{
  for (unsigned int i = 0; i < img.height; ++i)
  {
    for (unsigned int j = 0; j < img.width; ++j)
      printf("%hu %hu %hu ", img.pixel[i][j][RED],img.pixel[i][j][GREEN],img.pixel[i][j][BLUE]);
    printf("\n");
  }
}

void read_pixels(Image img)
{
  // read all pixels of image
  for (unsigned int i = 0; i < img.height; ++i)
  {
    for (unsigned int j = 0; j < img.width; ++j)
      scanf("%hu %hu %hu", &img.pixel[i][j][RED], &img.pixel[i][j][GREEN], &img.pixel[i][j][BLUE]);
  }
}

int main()
{
  Image img;

  // read type of image
  char p3[4];
  scanf("%s", p3);

  // read width height and color of image
  int max_color;
  scanf("%u %u %d", &img.width, &img.height, &max_color);

  read_pixels(img);
  int n_options;
  scanf("%d", &n_options);

  for(int i = 0; i < n_options; ++i)
  {
    int option;
    scanf("%d", &option);

    switch(option)
    {
      case 1:
      { // Escala de Cinza
        img = gray_scale(img);
        break;
      }
      case 2:
      { // Filtro Sepia
        for (unsigned int x = 0; x < img.height; ++x)
        {
          for (unsigned int j = 0; j < img.width; ++j)
          {
            unsigned short int pixel[3];
            pixel[0] = img.pixel[x][j][RED];
            pixel[1] = img.pixel[x][j][GREEN];
            pixel[2] = img.pixel[x][j][BLUE];

            int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j][RED] = menor_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j][GREEN] = menor_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j][BLUE] = menor_r;
          }
        }
        break;
      }
      case 3:
      { // Blur
        int size = 0;
        scanf("%d", &size);
        blur(img.height, img.pixel, size, img.width);
        break;
      }
      case 4:
      { // Rotacao
        int turns = 0;
        scanf("%d", &turns);
        turns %= 4;
        for (int j = 0; j < turns; ++j)
        {
          img = rotate90right(img);
        }
        break;
      }
      case 5:
      { // Espelhamento
        int horizontal = 0;
        scanf("%d", &horizontal);

        int width = img.width, height = img.height;

        if (horizontal == 1)
          width /= 2;
        else
          height /= 2;

        for (int i2 = 0; i2 < height; ++i2)
        {
          for (int j = 0; j < width; ++j)
          {
            int x = i2, y = j;

            if (horizontal == 1)
              y = img.width - 1 - j;
            else
              x = img.height - 1 - i2;

            Pixel aux1;
            aux1.red = img.pixel[i2][j][RED];
            aux1.green = img.pixel[i2][j][GREEN];
            aux1.blue = img.pixel[i2][j][BLUE];

            img.pixel[i2][j][0] = img.pixel[x][y][RED];
            img.pixel[i2][j][1] = img.pixel[x][y][GREEN];
            img.pixel[i2][j][2] = img.pixel[x][y][BLUE];

            img.pixel[x][y][0] = aux1.red;
            img.pixel[x][y][1] = aux1.green;
            img.pixel[x][y][2] = aux1.blue;
          }
        }
        break;
      }
      case 6:
      { // Inversao de Cores
        invert_colors(img.pixel, img.width, img.height);
        break;
      }
      case 7:
      { // Cortar Imagem
        int x, y;
        scanf("%d %d", &x, &y);
        int width, height;
        scanf("%d %d", &width, &height);

        img = cut_image(img, x, y, width, height);
        break;
      }
    }
  }

  printf("P3\n");
  printf("%u %u\n255\n", img.width, img.height);
  print_pixels(img);
  return 0;
}
