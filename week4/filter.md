# filter

headers:

- BITMAPFILEHEADER, is 14 bytes long
- BITMAPINFOHEADER, is 40 bytes long

## grayscale

In fact, to ensure each pixel of the new image still has the same general brightness or darkness as the old image, we can take the average of the red, green, and blue values to determine what shade of grey to make the new pixel.

## sepia

```c
sepiaRed   = .393 * originalRed + .769 * originalGreen + .189 * originalBlue
sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue
sepiaBlue  = .272 * originalRed + .534 * originalGreen + .131 * originalBlue

// can't be greater then max 8bit value, aka 255. use min(newColor, 255)
// round them (float) to be uint8_t 
```

## reflection

pixels on the left side of the image should end up on the right, and vice versa.

## blur

value by averaging the color values of neighboring pixels
https://cs50.harvard.edu/x/2022/psets/4/filter/less/#blur
