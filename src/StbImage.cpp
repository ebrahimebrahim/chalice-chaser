#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <StbImage.h>

StbImage::StbImage(const char * filename) {
    data = stbi_load(filename, &width, &height, &num_channels, 0);
}

StbImage & StbImage::operator=(StbImage && src) {
    if (&src == this) return *this;
    data = src.data;
    src.data = nullptr;
    width=src.width;
    height=src.height;
    num_channels=src.num_channels;
    return *this;
}


StbImage::~StbImage() {
    stbi_image_free(data);
}

