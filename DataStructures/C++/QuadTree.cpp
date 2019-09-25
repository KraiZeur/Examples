

#define MAX_WIDTH 600
#define MAX_HEIGHT 480

#include <vector>
#include <iostream>
class Point
{
  private:
    int _x;
    int _y;

  public:
    Point()
    {
        _x = 0;
        _y = 0;
    }

    Point(int x, int y)
    {
        _x = x;
        _y = y;
    }

    int getX() const
    {
        return _x;
    }
    int getY() const
    {
        return _y;
    }
};

class Rectangle
{
  private:
    int _x;
    int _y;
    int _width;
    int _height;

  public:
    Rectangle(int x, int y, int width, int height) : _x(x), _y(y), _width(width), _height(height)
    {
    }

    bool contains(Point *p)
    {
        return (p->getX() >= _x - _width && p->getX() <= _x + _width && p->getY() >= _y - _height && p->getY() <= _y + _height);
    }

    bool intersect(Rectangle *rec)
    {
        return !(rec->_x - rec->_width > _x + _width || rec->_x + rec->_width < _x - _width || rec->_y - rec->_height > _y + _height || rec->_y + rec->_height < _y - _height);
    }
};

template <typename T>
class QuadTree
{
  private:
    QuadTree *_northWest;
    QuadTree *_northEast;
    QuadTree *_southWest;
    QuadTree *_southEast;

    Rectangle *_boundaries;

    int _x;
    int _y;
    int _width;
    int _height;

    int _capacity;

    bool _divided;

    std::vector<T> _data;

  public:
    QuadTree(int x, int y, int width, int height, int capacity) : _x(x), _y(y), _width(width), _height(height)
    {
        _boundaries = new Rectangle(x, y, width, height);

        _northWest = NULL;
        _northEast = NULL;
        _southWest = NULL;
        _southEast = NULL;

        _capacity = capacity;

        _divided = false;
    }
    ~QuadTree()
    {
        delete _boundaries;
    }

    bool insert(T data)
    {
        if (!_boundaries->contains(data))
        {
            return false;
        }

        if (_data.size() < _capacity)
        {
            _data.push_back(data);
            return true;
        }

        if (!_divided)
        {
            subdivide();
        }

        if (_northWest->insert(data))
            return true;
        if (_northEast->insert(data))
            return true;
        if (_southWest->insert(data))
            return true;
        if (_southEast->insert(data))
            return true;

        return false;
    }

    void subdivide()
    {
        if (!_divided)
        {
            int upperDivide = _x + _width / 2;
            int upperLowDivide = _x; // _x - _width / 2;
            int lowerDivide = _y;    //_y - _height / 2;
            int lowerLowDivide = _y + _height / 2;

            std::cout << " " << upperDivide << " " << upperLowDivide << " " << lowerDivide << " " << lowerLowDivide << std::endl;

            int newWidth = _width / 2;
            int newHeight = _height / 2;

            _northEast = new QuadTree<T>(upperDivide, lowerDivide, newWidth, newHeight, _capacity);
            _northWest = new QuadTree<T>(upperLowDivide, lowerDivide, newWidth, newHeight, _capacity);
            _southEast = new QuadTree<T>(upperDivide, lowerLowDivide, newWidth, newHeight, _capacity);
            _southWest = new QuadTree<T>(upperLowDivide, lowerLowDivide, newWidth, newHeight, _capacity);

            /*
            typename std::vector<T>::const_iterator it;
            for (it=_data.begin(); it!=_data.end(); it++)
            {
                bool inserted=false;

                if (!inserted)
                    inserted = _northWest->insert((*it));
                if (!inserted)
                    inserted = _northEast->insert((*it));
                if (!inserted)
                    inserted = _southWest->insert((*it));
                if (!inserted)
                    inserted = _southEast->insert((*it));
            }
            */

            _divided = true;
        }
    }

    bool queryRange(std::vector<T> &result, Rectangle &area)
    {
        if (!_boundaries->intersect(&area))
        {
            return false;
        }

        typename std::vector<T>::const_iterator it;
        for (it = _data.begin(); it != _data.end(); it++)
        {
            if (area.contains((*it)))
            {
                result.push_back((*it));
            }
        }

        if (_divided)
        {
            _northWest->queryRange(result, area);
            _northEast->queryRange(result, area);
            _southWest->queryRange(result, area);
            _southEast->queryRange(result, area);
        }

        return true;
    }
};

int main(int argc, char const *argv[])
{
    QuadTree<Point *> *tree = new QuadTree<Point *>(0, 0, MAX_WIDTH, MAX_HEIGHT, 4);
    tree->insert(new Point(50, 50));
    tree->insert(new Point(150, 50));
    tree->insert(new Point(50, 200));
    tree->insert(new Point(50, 250));
    tree->insert(new Point(50, 75));
    tree->insert(new Point(50, 75));
    tree->insert(new Point(0, 75));
    tree->insert(new Point(0, 75));

    std::vector<Point *> points;

    Rectangle area(0, 0, 200, 200);

    tree->queryRange(points, area);

    std::vector<Point *>::const_iterator it;
    for (it = points.begin(); it != points.end(); it++)
    {
        std::cout << "found ! " << (*it)->getX() << " " << (*it)->getY() << std::endl;
    }

    return 0;
}
