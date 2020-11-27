#ifndef STB_IMAGE_WRAPPER_H
#define STB_IMAGE_WRAPPER_H
/** Jut an RAII wrapper for stb_image */

class StbImage {
    unsigned char * data = nullptr;
    int width{};
    int height{};

    /** The number of components that would be there if we loaded the file while telling stb_image to choose the number of channels */
    int num_channels_inherent_to_file{}; 

    /** The actually number of channels in `data` */
    int num_channels{};
public:
    StbImage(const char * filename);
    StbImage(const StbImage &) = delete;
    StbImage(StbImage && src) : data(src.data), height(src.height), num_channels(src.num_channels), num_channels_inherent_to_file(src.num_channels_inherent_to_file)
        { src.data=nullptr; }
    StbImage & operator=(const StbImage &) = delete;
    StbImage & operator=(StbImage && src);
    ~StbImage();

    int get_width() const {return width;}
    int get_height() const {return height;}
    int get_num_channels() const {return num_channels;}
    unsigned char * get_data() {return data;}
};



#endif // STB_IMAGE_WRAPPER_H