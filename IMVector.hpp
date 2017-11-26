#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

// M == branching factor,
// tells how many children there are for a branch and a leaf

const uint32_t B = 4;
const uint32_t M = 1 << B;

// gotta be fast.
// same thing could be achieved with / and %,
// but I believe this is faster.
uint32_t local_idx(uint32_t idx, uint32_t lvls) {
  return (idx >> (B * lvls)) & (M - 1);
}

using std::array;
using std::cout;
using std::endl;
using std::make_shared;
using std::make_unique;
using std::move;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

template <class T> class Node {
public:
  virtual shared_ptr<Node<T>> set(uint32_t idx, uint32_t levels, T some) = 0;
  virtual T get(uint32_t idx, uint32_t levels) = 0;
  virtual void destructive_set(uint32_t idx, uint32_t lvls, T some) = 0;
};

template <class T> class Branch : public Node<T> {
public:
  explicit Branch() { contents = array<shared_ptr<Node<T>>, M>(); }

  explicit Branch(array<shared_ptr<Node<T>>, M> c) : contents(c) {}

  explicit Branch(shared_ptr<Node<T>> &old_root) {
    contents = array<shared_ptr<Node<T>>, M>();
    // this is kinda ugly
    for (uint32_t i = 0; i < M; i++)
      contents[i] = old_root;
  }

  T get(uint32_t idx, uint32_t lvls) {
    uint32_t loc_idx = local_idx(idx, lvls);
    return contents[loc_idx]->get(idx, lvls - 1);
  }

  shared_ptr<Node<T>> set(uint32_t idx, uint32_t lvls, T some) {

    shared_ptr<Branch<T>> new_branch = make_shared<Branch<T>>(contents);

    uint32_t l_idx = local_idx(idx, lvls);

    new_branch->contents[l_idx] = contents[l_idx]->set(idx, lvls - 1, some);

    return new_branch;
  }

  void destructive_set(uint32_t idx, uint32_t lvls, T some) {
    contents[local_idx(idx, lvls)]->destructive_set(idx, lvls - 1, some);
  }

  template <typename U> Branch<U> bulk_update(std::function<U(T)> &f) {
    array<shared_ptr<Node<U>>, M> new_contents =
        array<shared_ptr<Node<U>>, M>();
    for (uint32_t i = 0; i < M; i++) {
      new_contents[i] = make_shared(f(contents[i].get()));
    }
  }

  array<shared_ptr<Node<T>>, M> contents;
};

template <class T> class Leaf : public Node<T> {
public:
  explicit Leaf() { contents = array<T, M>(); }
  explicit Leaf(array<T, M> c) : contents(c) {}

  template <typename U> Leaf<U> bulk_update(std::function<U(T)> &f) {
    array<U, M> new_contents = array<U, M>();
    for (uint32_t i = 0; i < M; i++) {
      new_contents[i] = f(contents[i]);
    }
    return Leaf<U>(new_contents);
  }

  T get(uint32_t idx, uint32_t lvls) { return contents[local_idx(idx, lvls)]; }

  shared_ptr<Node<T>> set(uint32_t idx, uint32_t lvls, T some) {

    shared_ptr<Leaf<T>> new_leaf = make_shared<Leaf<T>>(Leaf<T>(contents));

    new_leaf->contents[local_idx(idx, lvls)] = some;

    return new_leaf;
  }

  void destructive_set(uint32_t idx, uint32_t lvls, T some) {
    contents[local_idx(idx, lvls)] = some;
  }

  array<T, M> contents;
};

template <class T> class IMVector {
public:
  explicit IMVector<T>() {
    root = nullptr;
    size = 0;
    lvls = 0;
  }

  explicit IMVector<T>(shared_ptr<Node<T>> r, uint32_t s, uint32_t l)
      : root(r), size(s), lvls(l) {}

  explicit IMVector<T>(int s) {
    if (s == 0) {
      root = nullptr;
      size = 0;
      lvls = 0;
    } else if (s < M) {
      root = make_shared<Leaf<T>>(Leaf<T>());
      size = s;
      lvls = 1;
    } else {
      uint32_t level_count = 1;
      uint32_t x = s;
      while (x > 0) {
        x = B >> x;
        level_count++;
      }

      // works//
      shared_ptr<Leaf<T>> new_root = make_shared<Leaf<T>>(Leaf<T>());
      shared_ptr<Node<T>> r = new_root;
      for (int i = 1; i < level_count; i++)
        r = make_shared<Branch<T>>(Branch<T>(r));
      // don't touch... //

      root = r;
      size = s;
      lvls = level_count;
    }
  }

  uint32_t levels() { return lvls; }
  uint32_t get_size() { return size; }

  IMVector<T> push_back(T some) {

    // cout << "starting push_back" << endl;

    uint32_t new_size = size + 1;
    uint32_t idx = size;

    if (1 << (B * lvls) == new_size) {

      // cout << " must make greater size" << endl;
      // cout << "LVLS:" << lvls << endl;
      // cout << "1 << B * lvls: " << (1 << (B * lvls)) << endl;

      uint32_t new_lvls = lvls + 1; // if and only if needed

      shared_ptr<Node<T>> new_root = grow();

      IMVector new_vector(new_root, new_size, new_lvls);

      // cout << "n size " << new_vector.get_size() << endl;

      // cout << "n lvls " << new_vector.levels() << endl;

      new_vector.destructive_set(idx, some);

      return new_vector;
    } else {
      // cout << "there was enough levels" << endl;
      shared_ptr<Node<T>> new_root = root->set(idx, lvls - 1, some);
      // cout << "set was OK" << endl;
      return IMVector(new_root, new_size, lvls);
    }
  }

  void pop_back() { return IMVector(root, size - 1, lvls); }

  T get(uint32_t idx) { return root->get(idx, lvls - 1); }

  IMVector<T> set(int idx, T some) {
    shared_ptr<Node<T>> new_root = root->set(idx, lvls - 1, some);
    return IMVector(new_root, size, lvls);
  }

  void destructive_set(uint32_t idx, T some) {
    root->destructive_set(idx, lvls - 1, some);
  }

  shared_ptr<Node<T>> grow() {
    if (lvls != 0) {
      // cout << "Growing, new root is a Branch" << endl;

      // does this shit work;
      shared_ptr<Branch<T>> new_root = make_shared<Branch<T>>(Branch<T>(root));

      return new_root;

    } else {
      shared_ptr<Leaf<T>> new_root = make_shared<Leaf<T>>(Leaf<T>());
      return new_root;
    }
  }

  // shared_ptr<Node<T>> shrink() {}
  void print_contents() {
    for (int i = 0; i < size; i++) {
      std::cout << get(i) << std::endl;
    }
  }

private:
  shared_ptr<Node<T>> root;
  uint32_t size;
  uint32_t lvls;
};
