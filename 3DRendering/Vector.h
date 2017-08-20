class AbstractMatrix
{
private:
  void* _buffer;
  int _rows;
  int _cols;
  int _dataSize;
protected:
public:
  AbstractMatrix(int row, int col, int size)
  {
    _rows=row;
    _cols=col;
    _dataSize=size;
    _buffer = // malloc( row*col*size );
  }
  virtual ~AbstractMatrix()
  {
    delete _buffer;
  }
  int getRowCount() const
  {
    return _rows;
  }
  int getColCount() const
  {
    return _cols;
  }
  int getTotalSize() const
  {
    return _rows*_cols*_dataSize;
  }
  int getDataSize() const
  {
    return _dataSize;
  }
};

template <typename T>
class Matrix
{
private:
public:
  Matrix(int row, int col) : AbstractMatrix(int row, int col, sizeof(T))
  {

  }
  virtual ~Matrix()
  {

  }
};


int main(int argc, char const *argv[]) {

  Matrix<int> matrix(3,3);
  Matrix<int> matrix2(3,3);

  Matrix<int> mm = matrix.multiply(matrix2);

  Matrix<int> mt = matrix.transpose();

  return 0;
}
