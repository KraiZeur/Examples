#include <iostream>
#include <vector>

class MP4;
class FLV;

/**
 * This design pattern allows to use RTTI principle.
 * RTTI = Run Time Type Information
 */

#define REGISTER_ACCEPTOR \
virtual void accept(AbstractVideoFormatVisitor* v) { \
  return v->visit(this); \
}

class AbstractVideoFormatVisitor {
private:
public:
  AbstractVideoFormatVisitor() {}
  virtual ~AbstractVideoFormatVisitor() {}
  virtual void visit(MP4* video)=0;
  virtual void visit(FLV* video)=0;
};

class Visitor : public AbstractVideoFormatVisitor {
private:
  unsigned int _flvCount;
  unsigned int _mp4Count;
public:
  Visitor() {}
  virtual ~Visitor() {}
  virtual void visit(MP4 * video) {
    std::cout << "Visited a MP4 video" << std::endl;
    _mp4Count++;
  }
  virtual void visit(FLV* video) {
    std::cout << "Visited a FLV video" << std::endl;
    _flvCount++;
  }
  unsigned int getFLVCount() {
    return _flvCount;
  }
  unsigned int getMP4Count() {
    return _mp4Count;
  }
};

class VideoFormat {
private:
public:
  VideoFormat() {}
  virtual ~VideoFormat() {}
  virtual void accept(AbstractVideoFormatVisitor* v)=0;
};

class MP4 : public VideoFormat {
private:
public:
  MP4() {}
  virtual ~MP4() {}
  REGISTER_ACCEPTOR
};

class FLV : public VideoFormat {
private:
public:
  FLV() {}
  virtual ~FLV() {}
  REGISTER_ACCEPTOR
};

int main(void) {
  VideoFormat* mp4Video = new MP4;
  VideoFormat* flvVideo = new FLV;

  std::vector<VideoFormat*> videos;
  videos.push_back(mp4Video);
  videos.push_back(flvVideo);

  Visitor* visitor = new Visitor;

  std::vector<VideoFormat*>::const_iterator it;
  for(it=videos.begin();it!=videos.end();it++) {
    (*it)->accept(visitor);
  }

  std::cout << "Vector is composed of : " << visitor->getFLVCount()
                                              << " flv videos"<< std::endl;
  std::cout << "Vector is composed of : " << visitor->getMP4Count()
                                              << " mp4 videos"<< std::endl;
  delete visitor;

  for(it=videos.begin();it!=videos.end();it++) {
    delete (*it);
  }

  return 0;
}
