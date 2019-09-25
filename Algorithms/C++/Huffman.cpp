#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

namespace node_ns {
  typedef struct node {
    int value;
    double frequency;
    struct node* left;
    struct node* right;
    bool operator < (const struct node& node) const {
        std::cout << node.frequency << " " << frequency << std::endl;
        return (frequency < node.frequency);
    }
  } node_t;

  bool comparePointer(node_t* a, node_t* b) {
    return (a->frequency < b->frequency);
  }

  node_t * node_alloc_node(int value, double frequency) {
    node_t * node = new node_t;
    (*node).value=value;
    (*node).frequency=frequency;
    (*node).right=(*node).left=NULL;
    return node;
  }

  void node_preorder(node_t ** node, void (*callback)(node_t *)) {
    if ((*node)==NULL) {
      return;
    }

    callback((*node));
    node_preorder(&((*node)->left),callback);
    node_preorder(&((*node)->right),callback);
  }

  bool is_leaf(node_t* node) {
    return (node->left == NULL && node->right == NULL);
  }
}

void cb(node_ns::node_t* node) {
  std::cout << node << std::endl;
  if (is_leaf(node)) {
    std::cout << ">> " << node->value << " " << node->frequency;
  } else {
    std::cout << "left : " << node->left << " right : " << node->right << std::endl;
  }
}

template <typename T>
class FrequencyContainer {
private:
  std::map<T,double> _frequencies;
public:
  class Iterator {
  private:
    typename std::map<T,double>::iterator _it;
  public:
    Iterator() {
    }
    Iterator(typename std::map<T,double>::iterator it) {
      _it=it;
    }
    virtual ~Iterator() {}
    Iterator operator++(int) {
        _it++;
        return (*this);
    }
    T first() {
      return _it->first;
    }
    double second() {
      return _it->second;
    }
    bool operator==(const Iterator& other) {
      return other.equals(_it);
    }
    bool operator!=(const Iterator& other) {
      return !(*this == other);
    }
    bool equals(typename std::map<T,double>::iterator it) const {
      return _it==it;
    }
  };
  Iterator begin() {
    Iterator it(_frequencies.begin());
    return it;
  }

  Iterator end() {
    Iterator it(_frequencies.end());
    return it;
  }
public:
  FrequencyContainer() {
  }
  virtual ~FrequencyContainer() {
  }
  void addElementFrequency(T element, double frequency) {
    _frequencies[element] = frequency;
  }
  double getFrequency(T element) { // need a O(1) accessor
    return _frequencies[element];
  }
  int getCount() {
    return _frequencies.size();
  }
};

template <typename T>
T pick_node(std::vector<T>* list) {
  T nd=list->front();
  list->erase(list->begin());

  return nd;
}

template <typename T>
class Huffman {
private:
  FrequencyContainer<T>* _container;
  std::map<T,int> _lut;
  std::map<int,T> _lut2;
public:
  Huffman(FrequencyContainer<T>* container) {
    _container=container;
  }
  void generate() {
    // Transform all values to node_t

    // add all node_t to a list sorted by frequency
    std::vector<node_ns::node_t*> _nodes;
    typename FrequencyContainer<T>::Iterator it;
    for (it=_container->begin(); it!=_container->end(); it++) {
      _nodes.push_back(node_ns::node_alloc_node(it.first(),it.second()));
    }

    // Sort all nodes added by frequency
    std::sort(_nodes.begin(), _nodes.end(), node_ns::comparePointer);

    while(_nodes.size()>1) {
      node_ns::node_t* node1=pick_node<node_ns::node_t*>(&_nodes);
      node_ns::node_t* node2=pick_node<node_ns::node_t*>(&_nodes);

      node_ns::node_t* node=node_ns::node_alloc_node(0,node1->frequency+node2->frequency); // don't care about the value

      node->left = node1;
      node->right = node2;

      // find proper position in descending order
      std::vector<node_ns::node_t*>::iterator it = std::lower_bound(_nodes.begin(), _nodes.end(), node, node_ns::comparePointer);
      _nodes.insert(it, node); // insert before iterator it
    }

    node_ns::node_t* root = _nodes.front();
    node_ns::node_preorder(&root,cb);
  }
  void createBinary(node_ns::node_t* root) {

  }
  int encode(T element) {
    return _lut[element];
  }
  T decode(int code) {
    return _lut2[code];
  }
};

int main(int argc, char* argv[]) {
  FrequencyContainer<char> container;
  container.addElementFrequency('a',8.167);
  container.addElementFrequency('b',1.492);
  container.addElementFrequency('c',2.782);
  container.addElementFrequency('d',4.253);
  container.addElementFrequency('e',12.702);
  container.addElementFrequency('f',2.228);
  container.addElementFrequency('g',2.015);
  container.addElementFrequency('h',6.094);

  // AbstractHuffman* huffman = generate(&container); // = will generate a new Huffman<char>(&container);

  Huffman<char>* huffman = new Huffman<char>(&container);
  huffman->generate();

  int value=huffman->encode('c');
  char character=huffman->decode(0x05);

  std::cout << "'c' -> " << value << std::endl;
  std::cout << "0000 0101 -> " << character << std::endl;

  return 0;
}