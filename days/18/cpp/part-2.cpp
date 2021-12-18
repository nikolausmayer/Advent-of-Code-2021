#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

/// Snailfish number
struct SN {
  SN(SN* parent)
    : parent{parent}
  { }
  SN(SN* parent, int i)
    : parent{parent}, val{i}
  { }
  SN(SN* parent, SN* left, SN* right)
    : parent{parent}, left{left}, right{right}
  {
    left->parent  = this;
    right->parent = this;
  }

  SN* parent{nullptr};
  SN* left{nullptr};
  SN* right{nullptr};
  int val{0};

  SN* makeDeepCopy() const {
    SN* copy = new SN{nullptr};
    if (left) {
      copy->left  = left->makeDeepCopy();
      copy->left->parent  = copy;
    }
    if (right) {
      copy->right = right->makeDeepCopy();
      copy->right->parent = copy;
    }
    copy->val = val;
    return copy;
  }

  bool isLeft() const 
  {
    return (parent and (parent->left == this));
  }
  bool isRight() const 
  {
    return (parent and (parent->right == this));
  }

  bool isRegular() const
  {
    return (left == nullptr);
  }

  void reduce()
  {
    std::function<bool(SN*,int)> explode;
    explode = [&](SN* root, int level) -> bool {
      if (root->isRegular()) {
        return false;
      }
      if (root->left->isRegular() and
          root->right->isRegular() and
          level > 4) {
        //std::cout << "explode " << root << std::endl;
        /// Find next regular left
        SN* probe{root};
        while (probe and probe->isLeft()) {
          probe = probe->parent;
          //std::cout << "up L to " << probe << std::endl;
        }
        if (probe->parent) {
          probe = probe->parent->left;
          while (not probe->isRegular())
            probe = probe->right;
          //std::cout << "next regular left is " << probe->val << " -> + " << root->left->val << std::endl;
          probe->val += root->left->val;
        } else {
          //std::cout << "no next regular left\n";
        }
        /// Find next regular right
        probe = root;
        while (probe and probe->isRight()) {
          probe = probe->parent;
          //std::cout << "up R to " << probe << std::endl;
        }
        if (probe->parent) {
          probe = probe->parent->right;
          while (not probe->isRegular())
            probe = probe->left;
          //std::cout << "next regular right is " << probe->val << " -> + " << root->right->val << std::endl;
          probe->val += root->right->val;
        } else {
          //std::cout << "no next regular right\n";
        }
        delete root->left;  root->left  = nullptr;
        delete root->right; root->right = nullptr;
        root->val = 0;
        return true;
      }
      return (explode(root->left,  level+1) or 
              explode(root->right, level+1));
    };

    std::function<bool(SN*)> split;
    split = [&](SN* root) -> bool {
      if (root->isRegular()) {
        if (root->val >= 10) {
          /// Split
          //std::cout << "split " << root;
          root->left  = new SN{root, root->val/2};
          root->right = new SN{root, root->val/2 + (root->val%2)};
          root->val   = 0;
          //std::cout << " into " << root << std::endl;
          return true;
        } else {
          return false;
        }
      }

      return (split(root->left) or 
              split(root->right));
    };

    for (;;) {
      //std::cout << "----  " << this << std::endl;
      if (explode(this, 1))
        continue;
      if (split(this))
        continue;
      break;
    }
  }

  int getMagnitude() const {
    if (isRegular()) {
      return val;
    } else {
      return 3*left->getMagnitude() + 2*right->getMagnitude();
    }
  }

  void explode() {

  }

  friend std::ostream& operator<<(std::ostream& os, SN* s)
  {
    if (s->isRegular()) {
      os << s->val;
    } else {
      os << "[" << s->left << "," << s->right << "]";
    }
    return os;
  }
};



int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Bad arguments" << std::endl;
    return EXIT_FAILURE;
  }
  auto filename = argv[1];
  std::ifstream infile{filename};
  if (infile.bad() or not infile.is_open()) {
    std::cerr << "Cannot open \"" << argv[1] << "\"" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<SN*> numbers;
  
  {
    auto* root = new SN{nullptr};
    auto* probe = root;
    int reg{0};

    while (not infile.eof()) {
      const auto c{infile.get()};
      switch (c) {
        case '\n': {
          numbers.push_back(root);
          root = new SN{nullptr};
          probe = root;
          reg = 0;
          break;
        }
        case '[': {
          auto* tmp = new SN{probe};
          probe->left = tmp;
          probe = tmp;
          break;
        }
        case ',': {
          probe->val = reg;
          probe = probe->parent;
          auto* tmp = new SN{probe};
          probe->right = tmp;
          probe = tmp;
          reg = 0;
          break;
        }
        case ']': {
          probe->val = reg;
          probe = probe->parent;
          reg = 0;
          break;
        }
        default: {
          reg = 10*reg + (c - '0');
        }
      }
    }
  }

  auto maxMagnitude{0};
  for (auto i{0u}; i < numbers.size(); ++i) {
    for (auto j{0u}; j < numbers.size(); ++j) {
      if (i == j)
        continue;
      auto* added = new SN{nullptr, numbers[i]->makeDeepCopy(), 
                                    numbers[j]->makeDeepCopy()};
      added->reduce();
      maxMagnitude = std::max(maxMagnitude, added->getMagnitude());
    }
  }

  std::cout << maxMagnitude << std::endl;

  return EXIT_SUCCESS;
}

