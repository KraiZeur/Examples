#include <iostream>

class CodeGenerator
{
private:
public:

};


class AbstractElement
{


};

template <typename T>
class FunctionParameterElement : public AbstractElement
{

};

template <typename T>
class FunctionElement : public AbstractElement
{


};


class CodeGeneratorCPP : public CodeGenerator
{
private:
  // map include files
  // function maps
  // implementation map
public:
  void addIncludeFile(std::string pathToFile, bool directive)
  {

  }
  void addFunction(std::string name, std::string code)
  {

  }
};

/**

int compute(int value)
{
  int value2=value;
  return value2;
}


*/

int main(int argc, char const *argv[]) {


  CodeGeneratorCPP cppGenerator = CodeGeneratorCPP("file1");
  cppGenerator.add( new Function("int", "compute", new Arguments("int", "value"))
                         .setCode(new Line("int value2=value;")
                                      .addLine("return value2;")
                 )

  )

  return 0;
}
