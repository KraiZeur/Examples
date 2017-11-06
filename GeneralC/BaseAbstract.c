typedef struct abstract {
  int id;
} abstract_t;


typedef struct base {
  int id; /* abstract_t ? */
  int value;
} base_t;

void generate_from(abstract_t* abs) {
  switch (abs->id) {
    case 0: break;
    case 1: break;
    case 2: break;
    default: break;
  }
}
