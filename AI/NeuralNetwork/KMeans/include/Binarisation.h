#ifndef STI_BINARISATION_H
#define STI_BINARISATION_H

#include "include/ByteImage.h"
#include "include/log.h"

#define STI_BIN_MAX_VALUE 255

namespace sti {

/**
 * Generate a new byte image when filtering
 */
class Binarisation : public Filter {
private:
    unsigned int _threshold;
public:
    Binarisation(Image* image, const char* path, unsigned int threshold) : Filter(image,path), _threshold(threshold) {
    }

    virtual Image* filter() {
        log_trace("Starting Binarisation filter");
        ImageInfos filterInfos = _inImage->getInfos();
        Image* outImage = new ByteImage(filterInfos, _path.c_str());
        byte** image=_inImage->getByteImage();

        byte **filtered_image = bmatrix (filterInfos.nrl, filterInfos.nrh, filterInfos.ncl, filterInfos.nch);

        for (int i=filterInfos.nrl; i<filterInfos.nrh; i++)
        {
            for (int j=filterInfos.ncl; j<filterInfos.nch; j++)
            {
                if (image[i][j] > _threshold) {
                    outImage->getByteImage()[i][j] = STI_BIN_MAX_VALUE;
                } else {
                    outImage->getByteImage()[i][j] = 0;
                }
            }
        }
        return outImage;
    }

    ~Binarisation() {
    }
};

}

#endif /* STI_BINARISATION_H */
