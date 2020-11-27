#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <StbImage.h>
#include <stdexcept>

StbImage::StbImage(const char * filename) {

    stbi_set_flip_vertically_on_load(true); // sets a global flag. no harm in setting it every time we construct an image.

    num_channels = 3;
    data = stbi_load(filename, &width, &height, &num_channels_inherent_to_file, num_channels);
    
    if (!data) {
        throw std::runtime_error(std::string("Failed to open image: ")+filename);
    }
}

StbImage & StbImage::operator=(StbImage && src) {
    if (&src == this) return *this;
    data = src.data;
    src.data = nullptr;
    width=src.width;
    height=src.height;
    num_channels=src.num_channels;
    num_channels_inherent_to_file = src.num_channels_inherent_to_file;
    return *this;
}


StbImage::~StbImage() {
    stbi_image_free(data);
}

