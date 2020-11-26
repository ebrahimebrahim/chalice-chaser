#ifndef STB_IMAGE_WRAPPER_H
#define STB_IMAGE_WRAPPER_H
/** Jut an RAII wrapper for stb_image */

class StbImage {
    unsigned char * data = nullptr;
    int width{};
    int height{};
    int num_channels{};
public:
    StbImage(const char * filename);
    StbImage(const StbImage &) = delete;
    StbImage(StbImage && src) : data(src.data), height(src.height), num_channels(src.num_channels) { src.data=nullptr; }
    StbImage & operator=(const StbImage &) = delete;
    StbImage & operator=(StbImage && src);
    ~StbImage();

    int get_width() const {return width;}
    int get_height() const {return height;}
    int get_num_channels() const {return num_channels;}
};



#endif // STB_IMAGE_WRAPPER_H