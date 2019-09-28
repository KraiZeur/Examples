

// DDR3 SDRAM Chip
// Double Data Rate Synchronous Dynamic RAM

// CAS
// RAS

// Specificatitions

// Frequency : Mhz
// Protocol  : DDR3
// Timings   : 10-10-10
// Capacity  : 1024 Mbits
// Count     : 1



// Caches
// (S, E, B, m)
// S=2^s sets
// E lines per set
// B = 2^b bytes per cache block
// Cache size C = BxExS

/*

S = 4  => 2 set
E = 2  => 2 lines per set
B = 4  => 2 bytes per cache block

m = log2(M) Number of physical (main memory address bits)

4x2x4 = 24 bytes


*/

struct line {
  bool _valid;
  int _tag;
  int _block;
};

struct set {
  line* _lines;
}

class Cache
{
private:
  std::vector<struct set> _sets;
public:
  Cache(size_t S, size_t E, size_t B)
  {

  }
  virtual ~Cache()
  {

  }
};

Cache* createCache(size_t S, size_t E, size_t B)
{
  return new Cache(S,E,B);
}
