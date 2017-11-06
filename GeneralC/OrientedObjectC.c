typedef struct {
  (void*) (ctor*)(void* self);
  (void*) (dtor*)(void* self);

  uint32_t size;
} class_t;


typedef struct {
  class_t* _class;
  int x;
  int y;
} point_t;


void* new(const void* self) {
  const class_t* cls = (class_t*)self;

  void* ptr = calloc(1, cls->size);

  * (const class_t**) ptr = cls;

  if (cls->ctor) {
    ptr = cls->ctor(ptr);
  }

  return ptr;
}

int main(int argc, char const *argv[]) {

  new();

  return 0;
}
