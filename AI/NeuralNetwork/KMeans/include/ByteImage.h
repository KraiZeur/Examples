#ifndef STI_BYTE_IMAGE_H
#define STI_BYTE_IMAGE_H

#include <errno.h>
#include <stdlib.h>
#include "include/Image.h"

namespace sti {

class ByteImage : public Image {
private:
protected:
public:
    ByteImage(const char* path) : Image(path) {
        _byteImage = LoadPGM_bmatrix(const_cast<char*>(path), &_infos.nrl, &_infos.nrh, &_infos.ncl, &_infos.nch);
        if (_byteImage==NULL) {
            log_error("Error when allocating image %s", _path.c_str()); 
            return;
        }
    }

    ByteImage(ImageInfos infos, const char* path) : Image(infos, path) {
        _byteImage = bmatrix(infos.nrl, infos.nrh, infos.ncl, infos.nch);
        if (_byteImage==NULL) {
            log_error("Error when allocating image %s", _path.c_str()); 
            return;
        }
    }

    ByteImage(byte** image, ImageInfos infos, const char* path) : Image(infos,path) {
        if (image==NULL) {
            log_error("Error image %s is NULL", _path.c_str()); 
            return;
        }
        _byteImage = bmatrix(infos.nrl, infos.nrh, infos.ncl, infos.nch);
        if (_byteImage==NULL) {
            log_error("Error when allocating image %s", _path.c_str()); 
            return;
        }
        copy_bmatrix(image, infos.nrl, infos.nrh, infos.ncl, infos.nch, _byteImage, infos.nrl, infos.nrh, infos.ncl, infos.nch);
    }

    inline virtual byte** getByteImage() {
        return _byteImage;
    }

    inline virtual rgb8** getRGB8Image() {
    	if (_rgb8Image==NULL) {

            _rgb8Image = rgb8matrix(_infos.nrl, _infos.nrh, _infos.ncl, _infos.nch);
    	
        	for (int i=_infos.nrl; i<_infos.nrh ;i++) {
        	   for (int j=_infos.ncl; j<_infos.nch;j++) {
        	       _rgb8Image[i][j].r=_byteImage[i][j];
        	       _rgb8Image[i][j].g=_byteImage[i][j];
        	       _rgb8Image[i][j].b=_byteImage[i][j];
        	   }
        	}

        }

        return _rgb8Image;
    }

    inline virtual void save() {
        log_trace("here");
        std::string localPath(_path);
        size_t found;
        found=localPath.find_last_of("/\\");
        std::string dirPath = localPath.substr(0,found);

        int ret=mkdir (dirPath.c_str(), 0666);

        if (ret==-1) {
            if (errno!=EEXIST) {
                log_error("Error creating %s folder, program can't create parent directory", dirPath.c_str());
                exit(1);
            }
        }


        std::cerr << dirPath.c_str() << std::endl;

        SavePGM_bmatrix(_byteImage, _infos.nrl, _infos.nrh, _infos.ncl, _infos.nch, const_cast<char*>(_path.c_str()));
    }

    virtual void save(const char* path) {
	std::string localPath=path;
        log_trace("Entering save Byte");
        if (localPath.size()==0) {
            localPath=_path;
        }

        if (_byteImage==NULL) {
            log_error("Error image %s doesn't exist", localPath.c_str()); 
        }

        log_trace("here");
        size_t found;
        found=localPath.find_last_of("/\\");
        std::string dirPath = localPath.substr(0,found);

        int ret=mkdir (dirPath.c_str(), 0644);

        if (ret==-1) {
            if (errno!=EEXIST) {
                log_error("Error creating %s folder, program can't create parent directory", dirPath.c_str());
                exit(1);
            }
        }

        log_trace("Saving Byte image to : %s", localPath.c_str());
        SavePGM_bmatrix(_byteImage, _infos.nrl, _infos.nrh, _infos.ncl, _infos.nch, const_cast<char*>(localPath.c_str()));
    }
};

}

#endif /* STI_BYTE_IMAGE_H */
