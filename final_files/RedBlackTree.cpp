#include <iostream>
#include <string>

using std::string;
using std::to_string;
using std::cout;

/**
 * enum mo�liwych kolor�w w�z��w drzewa
 */
enum nodeColour {
    RED, BLACK
};

/**
 * klasa reprezentuj�ca drzewo czerwono-czarne, stworzona z pomoc� https://en.wikipedia.org/wiki/Red�black_tree
 */
class RedBlackTree {

    /**
     * pomocnicza klasa reprezentuj�ca jeden w�ze� drzewa
     */
    class Node {
    public:
        int value;
        Node* parent;
        Node* left;
        Node* right;
        nodeColour colour;

        explicit Node(int val, Node* p = nullptr, Node* l = nullptr, Node* r = nullptr, nodeColour givenColour = RED) {
            value = val;
            parent = p;
            left = l;
            right = r;
            colour = givenColour;
        }
    };

    Node* nil; //wartownik
    Node* root; //korze�

    /**
     * metoda wywo�ywana podczas usuwania drzewa, odpowiada za usuni�cie wszystkich w�z��w drzewa
     * @param node
     */
    void removeNodeFromMemory(Node* node) {
        if (node == nil)
            return;
        removeNodeFromMemory(node->left);
        removeNodeFromMemory(node->right);
        delete node;
    }

public:

    //stringi wykorzystywane do wy�wietlania ga��zi drzewa
    string cr;
    string cl;
    string cp;

    /**
     * konstruktor tworz�cy nowe drzewo czerwono-czarne, albo puste, albo wype�nione podan� tablica
     * @param array
     * @param arraySize
     */
    explicit RedBlackTree(int* array = nullptr, int arraySize = 0) {
        nil = new Node(0, nullptr, nullptr, nullptr, BLACK);
        nil->parent = nil;
        nil->left = nil;
        nil->right = nil;
        root = nil;
        for (unsigned int i = 0; i < arraySize; ++i) {
            add(array[i]);
        }
    }

    /**
     * destruktor
     */
    ~RedBlackTree() {
        removeNodeFromMemory(root);
        delete nil;
    }

    /**
     * rotacja danego w�z�a w lewo, je�li jest mo�liwa
     * @param a
     */
    void rotateLeft(Node* a) {
        if (a == nil)
            return;
        Node* b = a->right;
        Node* parent = a->parent;
        if (b != nil) {
            a->right = b->left;
            b->left = a;
            a->parent = b;
            if (a->right != nil) {
                a->right->parent = a;
            }
            b->parent = parent;
            if (parent != nil) {
                if (a == parent->left) {
                    parent->left = b;
                }
                else if (a == parent->right) {
                    parent->right = b;
                }
            }
        }
    }

    /**
     * rotacja danego w�z�a w prawo, je�li jest mo�liwa
     * @param a
     */
    void rotateRight(Node* a) {
        if (a == nil)
            return;
        Node* b = a->left;
        Node* parent = a->parent;
        if (b != nil) {
            a->left = b->right;
            b->right = a;
            a->parent = b;
            if (a->left != nil) {
                a->left->parent = a;
            }
            b->parent = parent;
            if (parent != nil) {
                if (a == parent->right) {
                    parent->right = b;
                }
                else if (a == parent->left) {
                    parent->left = b;
                }
            }
        }
    }

    /**
     * dodanie liczby do drzewa - algorytm - Cormen - wprowadzenie do algorytm�w 13.3
     * @param number
     */
    void add(int number) {
        Node* y = nil;
        Node* x = root;
        while (x != nil) {
            y = x;
            if (number < x->value)
                x = x->left;
            else
                x = x->right;
        }
        Node* z = new Node(number, y, nil, nil);
        if (y == nil)
            root = z;
        else if (number < y->value) {
            y->left = z;
        }
        else y->right = z;
        addFixTree(z);
    }

    /**
     * naprawianie w�a�ciwo�ci drzewa czerwono-czarnego po dodaniu do niego liczby - algorytm - Cormen - wprowadzenie do algorytm�w 13.3
     * @param z
     */
    void addFixTree(Node* z) {
        while (z->parent->colour == RED) {
            Node* grandparent = z->parent->parent;
            Node* y;
            if (z->parent == grandparent->left) {
                y = grandparent->right;
                if (y->colour == RED) {
                    z->parent->colour = BLACK;
                    y->colour = BLACK;
                    grandparent->colour = RED;
                    z = grandparent;
                }
                else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->colour = BLACK;
                    grandparent->colour = RED;
                    rotateRight(grandparent);
                }
            }
            else {
                y = grandparent->left;
                if (y->colour == RED) {
                    z->parent->colour = BLACK;
                    y->colour = BLACK;
                    grandparent->colour = RED;
                    z = grandparent;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->colour = BLACK;
                    grandparent->colour = RED;
                    rotateLeft(grandparent);
                }
            }
        }
        //wa�ne, bo korze� po operacjach wy�ej mo�e ju� nie by� aktualnym korzeniem
        while (root->parent != nil) {
            root = root->parent;
        }
        root->colour = BLACK;
    }

    /**
     * szukanie i usuwanie danej liczby
     * @param number
     * @return true, je�li znaleziono i usuni�to liczb�, false w przeciwnym wypadku
     */
    bool removeGivenNumber(int number) {
        Node* temp = root;
        while (temp != nil) {
            if (number == temp->value) {
                removeGivenElement(temp);
                return true;
            }
            else {
                if (number < temp->value)
                    temp = temp->left;
                else
                    temp = temp->right;
            }
        }
        return false;
    }

    /**
     * usuwanie ju� danego elementu drzewa, algorytm - Cormen - Wprowadzenie do algorytm�w 13.4
     * @param node
     */
    void removeGivenElement(Node* node) {
        Node* y;
        if (node->left == nil || node->right == nil)
            y = node;
        else {
            //node ma dw�ch potomk�w, y b�dzie nast�pnikiem node
            y = node->right;
            while (y->left != nil)
                y = y->left;
        }
        Node* x;
        x = y->right;
        if (x != nil)
            x->parent = y->parent;
        if (y->parent == nil) {
            root = x;
        }
        else if (y == y->parent->left) {
            x = y->parent->left;
        }
        else x = y->parent->right;
        if (y != node)
            node->value = y->value;
        if (y->colour == BLACK)
            removeFixTree(x);
        //usuni�cie odniesienia do danego elementu u rodzica usuwanego elementu
        Node* parentOfDeletedNode = y->parent;
        if (parentOfDeletedNode->left == y)
            parentOfDeletedNode->left = nil;
        else
            parentOfDeletedNode->right = nil;
        delete y;
    }

    /**
     * naprawianie w�a�ciwo�ci drzewa czerwono-czarnego po usuni�ciu elementu, algorytm - Cormen - wprowadzenie do algorytm�w 13.4
     * @param node
     */
    void removeFixTree(Node* node) {
        while (node != root && node->colour == BLACK) {
            if (node == node->parent->left) {
                Node* w = node->parent->right;
                if (w->colour == RED) {
                    w->colour = BLACK;
                    node->parent->colour = RED;
                    rotateLeft(node->parent);
                    w = node->parent->right;
                }
                if (w->left->colour == BLACK && w->right->colour == BLACK) {
                    w->colour = RED;
                    node = node->parent;
                }
                else if (w->right->colour == BLACK) {
                    w->left->colour = BLACK;
                    w->colour = RED;
                    rotateRight(w);
                    w = node->parent->right;
                }
                w->colour = node->parent->colour;
                node->parent->colour = BLACK;
                w->right->colour = BLACK;
                rotateLeft(node->parent);
                node = root;
            }
            else {
                Node* w = node->parent->left;
                if (w->colour == RED) {
                    w->colour = BLACK;
                    node->parent->colour = RED;
                    rotateRight(node->parent);
                    w = node->parent->left;
                }
                if (w->right->colour == BLACK && w->left->colour == BLACK) {
                    w->colour = RED;
                    node = node->parent;
                }
                else if (w->left->colour == BLACK) {
                    w->right->colour = BLACK;
                    w->colour = RED;
                    rotateLeft(w);
                    w = node->parent->left;
                }
                w->colour = node->parent->colour;
                node->parent->colour = BLACK;
                w->left->colour = BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
        node->colour = BLACK;
    }

    /**
     * wyszukanie podanej liczby w drzewie
     * @param number
     * @return true, je�li znaleziono liczb�, false, je�li nie znaleziono liczby
     */
    bool findGivenNumber(int number) {
        Node* temp = root;
        while (temp != nil) {
            if (number == temp->value)
                return true;
            else {
                if (number < temp->value)
                    temp = temp->left;
                else
                    temp = temp->right;
            }
        }
        return false;
    }

    /**
     * tylko do test�w, aby wydoby� losowy element drzewa do usuni�cia
     * @return
     */
    Node* getNodeWithGivenNumber(int number) {
        Node* temp = root;
        while (temp != nil) {
            if (number == temp->value)
                return temp;
            else {
                if (number < temp->value)
                    temp = temp->left;
                else
                    temp = temp->right;
            }
        }
        return nullptr;
    }

    /**
     * wy�wietlenie drzewa w konsoli, analogicznie jak w kopcu; kolory w�z��w s� oznaczane przez liter� R (red) lub B (black) na ko�cu liczby,
     */
    void print() {
        cr = cl = cp = "  ";
        cr[0] = 218;
        cr[1] = 196;
        cl[0] = 192;
        cl[1] = 196;
        cp[0] = 179;
        printRecursive("", "", root);
        cout << '\n';
    }

    void printRecursive(const string& sp, const string& sn, Node* node) {
        if (node != nil) {
            string s = sp; //"tekst do wy�wietlenia w wierszach po�rednich dla syn�w"
            if (sn == cr)
                s[s.length() - 2] = ' ';
            printRecursive(s + cp, cr, node->right);
            s = s.substr(0, sp.length() - 2);
            string value = (node->colour == RED) ? to_string(node->value) + "R" : to_string(node->value) + "B";
            cout << s << sn << value << '\n';
            s = sp;
            if (sn == cl)
                s[s.length() - 2] = ' ';
            printRecursive(s + cp, cl, node->left);
        }
    }

};