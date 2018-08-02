/////////////////////////////////////////////////
/// @mainpage Welcome to the Autobalancing BST Library
/// @description The purpose of this library is to
/// create an autobalancing binary search tree datastructure.
/// Please view the <a href="https://github.com/dvigne/BST">source code</a>
/// and class list and feel free to modify the code to fit your applicaiton and learn
/// @author Derick Vigne
/// @copyright MIT License
/////////////////////////////////////////////////

#ifndef __BST__
#define __BST__

/////////////////////////////////////////////////
/// Node Object Structure
/// Used to create nodes that get autobalanced by the BST class
/// @author Derick Vigne
/// @copyright MIT License
/////////////////////////////////////////////////

template <class T>
class Node {
public:
  int weight; //!< Weight of the node
  T value; //!< Value of the the node
  Node<T> *left, *right, *parent;

/////////////////////////////////////////////////
/// @var Node<T> *left
/// @brief Left node in reference to current node
/// @var Node<T> *right
/// @brief Right node in reference to current node
/// @var Node<T> *parent
/// @brief Parent node in reference to current node
/////////////////////////////////////////////////

  Node(int weight, T value);
};

/////////////////////////////////////////////////
/// BST class
/// Autobalancing Binary Search Tree Library
/// @author Derick Vigne
/// @copyright MIT License
/////////////////////////////////////////////////

template <class T>
class BST {
private:
  Node<T>* root; // Root node created when the BST() constructor is executed
  void replace_node_in_parent(Node<T> *currentNode, Node<T> *newNode);

public:
  BST(int weight, T value) { this->root = new Node<T>(weight, value); }
  Node<T>* search(Node<T> *root, T value); //!< Returns pointer to node with value
  Node<T>* insert(Node<T>* root, int weight, T value); //!< Inserts a value auto-balancing a tree
  void traverse(Node<T>* root, void (*callback)(int)); //!< Traverse to node based on weight
  void deleteNode(Node<T> *currentNode, int weight); //!< Deletes a node and rebalances the tree
  Node<T>* balance(); //!< Performs a manual balance of the tree
  Node<T>* findMinimum(); //!< Finds the global minimum
  Node<T>* findMaximum(); //!< Finds the global maximum
  Node<T>* findMinimum(Node<T>* root); //!< Finds the minimum given a node
  Node<T>* findMaximum(Node<T>* root); //!< Finds the maximum given a node
  Node<T>* getRoot(); //!< Returns the root of the BST
  bool isBalanced(Node<T>* root, int minWeight, int maxWeight); //!< Verify the BST is balanced
};

/////////////////////////////////////////////////
/// @fn BST<T>::BST(int weight, T value)
/// @brief Constructor
///
/// Used to bootstrap and create
/// the first root node of the BST
/// @param weight Sets the weight of the new root node
/// @param value Sets the value of the new root node
/// @return void

/////////////////////////////////////////////////
/// @brief Constructor used to create a node
/// @param weight weight of the node
/// @param value value of the node
/// @return void*/
/////////////////////////////////////////////////

template <class T>
Node<T>::Node(int weight, T value) {
  this->weight = weight;
  this->value = value;
}

/////////////////////////////////////////////////
/// @param root Current node to begin searching from
/// @param value Value to search for
/// @return Node<T>* Returns pointer to found node
/////////////////////////////////////////////////

template <class T>
Node<T>* BST<T>::search(Node<T> *root, T value) {
  if (value < this->findMinimum()->value || value > this->findMaximum()->value) {
    return root;
  }
  if(root->value == value) {
    return root;
  }
  if(value < root->value) {
    return this->search(root->left, value);
  }
  return this->search(root->right, value);
}

/////////////////////////////////////////////////
/// @param root Current node to traverse from. This is assumed to be the top most node
/// @param callback Pointer to callback function to pipe integer weights into
/// as an `int` paramater for verification of balance
/// @warning Your callback function must be defined/prototyped as
/// @code
/// void func(int param)
/// @endcode
/// @note An example for using the callback function as verification of proper binary search tree balancing
/// @code
///    BST<int> myBST(5,5);
///
///    myBST.insert(myBST.getRoot(), 1,1);
///    myBST.insert(myBST.getRoot(), 2,2);
///    myBST.insert(myBST.getRoot(), 3,3);
///    myBST.insert(myBST.getRoot(), 4,4);
///    myBST.insert(myBST.getRoot(), 6,6);
///    myBST.insert(myBST.getRoot(), 7,7);
///
///    void print(int weight) {
///    	std::cout << weight << ", ";
///    }
///
///    myBST.traverse(myBST.getRoot(), &print);
/// @endcode
/// @endnote
/// @return void
/////////////////////////////////////////////////

template <class T>
void BST<T>::traverse(Node<T>* root, void (*callback)(int)) {
  if (!root) {
    return;
  }
  traverse(root->left, callback);
  callback(root->weight);
  traverse(root->right, callback);
}

/////////////////////////////////////////////////
/// @param root Node to begin traversal from and insert the next corresponding weight
/// @param value Value to insert
/// @return Node<T>* Returns pointer to new node wih corresponding weight and value
/////////////////////////////////////////////////

template <class T>
Node<T>* BST<T>::insert(Node<T>* root, int weight, T value) {
  if (!root) {
    root = new Node<T>(weight, value);
  }
  else if(weight == root->weight) {
    root->value = value;
  }
  else if(weight < root->weight) {
    root->left = insert(root->left, weight, value);
    root->left->parent = root;
  }
  else {
    root->right = insert(root->right, weight, value);
    root->right->parent = root;
  }
  return root;
}

/////////////////////////////////////////////////
/// @param root Root node to begin traversal
/// @param weight Weight of node to remove
/// @return void
/////////////////////////////////////////////////

template <class T>
void BST<T>::deleteNode(Node<T> *root, int weight) {
 if (weight < root->weight) {
   deleteNode(root->left, weight);
   return;
 }
 if (weight > root->weight) {
   deleteNode(root->right, weight);
   return;
 }
 if (root->left && root->right) {
   Node<T>* successor = findMinimum(root->right);
   root->weight = successor->weight;
   root->value = successor->value;
   deleteNode(successor, successor->weight);
 }
 else if(root->left) {
   replace_node_in_parent(root, root->left);
 }
 else if(root->right) {
   replace_node_in_parent(root, root->right);
 }
 else {
   replace_node_in_parent(root, NULL);
 }
}

template <class T>
void BST<T>::replace_node_in_parent(Node<T> *currentNode, Node<T> *newNode) {
  if(currentNode->parent) {
    if(currentNode == currentNode->parent->left) {
      currentNode->parent->left = newNode;
    }
    else {
      currentNode->parent->right = newNode;
    }
    if (newNode) {
      newNode->parent = currentNode->parent;
    }
  }
}

/////////////////////////////////////////////////
/// @description Begins traversal at root node to find the smallest, minimum weight
/// @return Node<T>* Returns pointer to minimum node
/////////////////////////////////////////////////

template <class T>
Node<T>* BST<T>::findMinimum() {
  Node<T>* currentNode = this->root;
  while(currentNode->left) {
    currentNode = currentNode->left;
  }
  return currentNode;
}

/////////////////////////////////////////////////
/// @description Begins traversal at root node to find the biggest, maximum weight
/// @return Node<T>* Returns pointer to maximum node
/////////////////////////////////////////////////

template <class T>
Node<T>* BST<T>::findMaximum() {
  Node<T>* currentNode = this->root;
  while(currentNode->right) {
    currentNode = currentNode->right;
  }
  return currentNode;
}

/////////////////////////////////////////////////
/// @param root Root node to start traversal from to find minimum
/// @return Node<T>* Returns pointer to minimum node
/////////////////////////////////////////////////

template <class T>
Node<T>* BST<T>::findMinimum(Node<T>* root) {
  Node<T>* currentNode = root;
  while(currentNode->left) {
    currentNode = currentNode->left;
  }
  return currentNode;
}

/////////////////////////////////////////////////
/// @param root Root node to start traversal from to find maximum
/// @return Node<T>* Returns pointer to maximum node
/////////////////////////////////////////////////

template <class T>
Node<T>* BST<T>::findMaximum(Node<T>* root) {
  Node<T>* currentNode = root;
  while(currentNode->right) {
    currentNode = currentNode->right;
  }
  return currentNode;
}

/////////////////////////////////////////////////
/// @param root Root node to begin traversal
/// @param minWeight Minimum weight to check
/// @param maxWeight Maximum weight to check
/// @return bool For balanced or unbalanced
/////////////////////////////////////////////////

template <class T>
bool BST<T>::isBalanced(Node<T>* root, int minWeight, int maxWeight) {
  if (root  == NULL) {
    return true;
  }
  if (root->weight < minWeight || root->weight > maxWeight) {
    return false;
  }
  return isBalanced(root->left, minWeight, root->weight-1) && isBalanced(root->right, root->weight+1, maxWeight);
}

/////////////////////////////////////////////////
/// @return Node<T>* Returns root
/////////////////////////////////////////////////

template <class T>
Node<T>* BST<T>::getRoot() {
  return root;
}

#endif
