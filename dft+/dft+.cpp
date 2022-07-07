#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 6011)
#endif // _MS

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <complex>
#include <cmath>
#include <ostream>
#include <algorithm>
#include <vector>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


inline int32_t index(int32_t x, int32_t y, int32_t w) {
    return (y * w + x);
}

void normalize(unsigned char *image, float *&normImage, int32_t size) {
    cout << "size:" << size << endl;
    normImage = (float *) calloc(size, sizeof(float));
    for (int32_t i = 0; i < size; i++) {
        normImage[i] = image[i] / 1.0f;
    }
}

complex<float> sumCols(const float *normImage, int32_t x, int32_t w, int32_t h, int32_t u, int32_t v) {

    complex<float> sum(0, 0);
    float dx = (float) x, dw = (float) w, dh = (float) h, du = (float) u, dv = (float) v;

    complex<float> lastfxy(0, 0);
    complex<float> laste(0, 0);

    for (int32_t y = 0; y < h; y++) {
        float dy = (float) y;
        complex<float> fxy(normImage[index(x, y, w)], 0);
        lastfxy = fxy;
        // complex<float> e2(cos(-2*M_PI*( (X*U)/M + (Y*V)/N) ), sin(-2*M_PI*( (X*U)/M + (Y*V)/N) ));
        complex<float> e(0.0f, (float) (- 2 * M_PI * ((dx * du) / dw + (dy * dv) / dh)));
        laste = e;
        sum += (fxy * exp(e));
    }

    return sum;
}

float *crop(const float *normImage, int32_t stride, int32_t x, int32_t y, int32_t w, int32_t h) {
    float *cropped = (float *) calloc((size_t) (w * h), sizeof(float));
    float v;
    int32_t k = 0, l = 0;
    for (int32_t j = y; j < y + h; j++) {
        k = 0;
        for (int32_t i = x; i < x + w; i++) {
            v = normImage[index(i, j, stride)];
            cropped[index(k, l, w)] = v;
            k++;
        }
        l++;
    }
    return cropped;
}

void copy(const float *src, int32_t sw, int32_t sh, float *&target, int32_t x, int32_t y, int32_t tw) {
    for (int j = 0; j < sh; j++) {
        for (int i = 0; i < sw; i++) {
            // cout << "(" << x+i << "," << y+j << ")" << "=" << "(" << i << "," << j << ")" << endl;
            target[index(x + i, y + j, tw)] = src[index(i, j, sw)];
        }
    }
}


int main(int argc, char **argv) {

    int w, h, c;
    unsigned char *data = stbi_load("img/coin.png", &w, &h, &c, 1);

    size_t sz = (size_t) (w * h);

    cout << "size:" << w << "*" << h << endl;
    cout << "comp:" << c << endl;

    // stbi_write_png("OI.png", w, h, 1, data, w);

    float *normImage;
    normalize(data, normImage, w * h);

    float *magnitudes = (float *) calloc(sz, sizeof(float));

    for (int v = 0; v < h; v++) {
        for (int u = 0; u < w; u++) {
            complex<float> sum(0, 0);
            complex<float> one(1, 0);
            for (int x = 0; x < w; x++) {
                complex<float> currentSum = sumCols(normImage, x, w, h, u, v);
                //cout << "  currentSum:" << currentSum << endl;
                sum += currentSum;
            }
            //cout << "sum:" << sum << endl;

            float magnitude = log(abs(sum));
            // int32_t color = round(255.0 / log(1 + magnitude));
            std::cout << "\r" << "(" << u << "," << v << ")" << " magnitude:" << magnitude << std::flush;
            magnitudes[index(u, v, w)] = magnitude;
            //cout << "index(u, v, w)=" << index(u, v, w) << endl;
        }
    }
    cout << endl;

    // normalize magnitudes
    vector<float> v(w * h);
    memcpy(&v[0], magnitudes, sz * sizeof(float));
    auto minimum = std::min_element(std::begin(v), std::end(v));
    auto maximum = std::max_element(std::begin(v), std::end(v));
    cout << "min=" << *minimum << endl;
    cout << "max=" << *maximum << endl;
    for (int i = 0; i < w * h; i++) {
        magnitudes[i] = (magnitudes[i] - *minimum) / (*maximum - *minimum);
        // cout << magnitudes[i] << "; ";
    }
    // cout << endl;

    float *cropUpperLeft = crop(magnitudes, w, 0, 0, w / 2, h / 2);
    float *cropUpperRight = crop(magnitudes, w, w / 2, 0, w / 2, h / 2);
    float *cropBottomLeft = crop(magnitudes, w, 0, h / 2, w / 2, h / 2);
    float *cropBottomRight = crop(magnitudes, w, w / 2, h / 2, w / 2, h / 2);
    float *rearranged = (float *) calloc(/*w*h*/sz, sizeof(float));
    copy(cropUpperLeft, w / 2, h / 2, rearranged, w / 2, h / 2, w);
    copy(cropUpperRight, w / 2, h / 2, rearranged, 0, h / 2, w);
    copy(cropBottomLeft, w / 2, h / 2, rearranged, w / 2, 0, w);
    copy(cropBottomRight, w / 2, h / 2, rearranged, 0, 0, w);


    // create resulting dft image
    uint8_t *dftImage = (uint8_t *) calloc(/*w * h*/sz, sizeof(uint8_t));
    for (int i = 0; i < w * h; i++) {
        dftImage[i] = (uint8_t) round(magnitudes[i] * 255);
        // cout << (int32_t) dftImage[i] << "; ";
    }
    // cout << endl;
    stbi_write_png("magn.png", w, h, 1, dftImage, w);

    // create the rearranged dft image
    uint8_t *dftRearranged = (uint8_t *) calloc(/*w * h*/sz, sizeof(uint8_t));
    for (int i = 0; i < w * h; i++) {
        dftRearranged[i] = (uint8_t) round(rearranged[i] * 255);
    }
    stbi_write_png("reamagn.png", w, h, 1, dftRearranged, w);

    free(magnitudes);
    free(normImage);
    free(dftImage);
    free(cropUpperLeft);
    free(cropUpperRight);
    free(cropBottomLeft);
    free(cropBottomRight);
    free(rearranged);
    return 0;
}
