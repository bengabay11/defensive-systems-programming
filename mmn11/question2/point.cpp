#include <iostream>

class Point
{
   int* _coord;
public:
   Point()
   {
      _coord = new int[2];
      _coord[0] = _coord[1] = 0;
   }

   Point(int x, int y)
      {
         _coord = new int[2];
         _coord[0] = x;
         _coord[1] = y;
      }

   Point(const Point& other)
   {
      _coord = other._coord;
   }

   ~Point()
   {
      delete _coord;
   }

   void setX(int value) { _coord[0] = value; }
   void setY(int value) { _coord[1] = value; }

   friend std::ostream& operator<<(std::ostream& os, const Point& p)
   {
      os << "(" << p._coord[0] << "," << p._coord[1] << ")";
      return os;
   }
};

int example()
{
   Point p1(1, 2);
   Point p2 = p1;

   p2.setX(5);

   std::cout << "p1=" << p1 << std::endl;
   std::cout << "p2=" << p2 << std::endl;

   return 0;
}
