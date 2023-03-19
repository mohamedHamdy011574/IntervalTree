#include <iostream>
using namespace std;
class IntervalTreeNode
{
public:
    // for interval
    int low,high,MAX;
    IntervalTreeNode* right;
    IntervalTreeNode* left;
    // to access parent direct
    IntervalTreeNode* parent;
    //constractor that take interval and set it in node
    IntervalTreeNode(int low,int high)
    {
    this->low = low;
    this->high = high;
    this->MAX = this->high;

    right = 0;
    left = 0;
    parent=0;
    }
};

class IntervalTree
{
//private function that user cant acesss this function from main
private:
    // root of tree
    IntervalTreeNode* root;

    // private function that call from public to insert node in tree
    IntervalTreeNode* insertInterval(IntervalTreeNode* root, int low, int high)
    {
    IntervalTreeNode* newNode = new IntervalTreeNode(low,high);

    //if root not have any value insert node in root
    if(root == 0)
    {
    root = newNode;
    }
    // search for place that you want to insert on it
    else
    {
    //like binary search insert based on low
    if(newNode->low < root->low)
    {
    if(root->MAX < newNode->high)
    {
    //make max equal high of interval
    root->MAX = newNode->high;
    }
    //insert on left like binary search tree
    root->left = insertInterval(root->left,low,high);
    root->left->parent = root;
    }
    else if(newNode->low > root->low)
    {
    // change max based on high value
    if(root->MAX < newNode->high)
    {
    root->MAX = newNode->high;
    }
    // insert in right like binary search tree.
    root->right = insertInterval(root->right,low,high);
    root->right->parent = root;
    }
    // we cant insert node because have the same low interval
    else
    {
    cout<<"can not insert two intervals with the same low endpoint"<<endl;
    }
    }
    // return (that update will done )
    return root;

    }
    //this for deleting node
    void deleteInterval(IntervalTreeNode* root, int low, int high)
    {
    // check if we have node in tree or No.
    if(root != 0)
    {
    // we check interval that match the query we want to delete
    if(low == root->low && high == root->high)
    {
    //node that help us to use it to update max in path by traverse on it.
    IntervalTreeNode* nodeHelpeToUpdateMax = 0;
    // here you get leaf node that do not have right or left
    if(root->right == 0 && root->left == 0)
    {
    // go to parent
    if(root->parent != 0)
    {
    // node in right sub tree
    if(root == root->parent->right)
    // make parent  point to --->>>0
    root->parent->right = 0;
    else
    // node in left sub tree
    // make parent  point to --->>>0
    root->parent->left = 0;
    // assign the node that help us to update maximum to root->parent to update tree
    nodeHelpeToUpdateMax = root->parent;
    }

    // finally you can delete node
    delete root;
    }
    // you are not leaf you have 2 child
    else if(root->right != 0 && root->left != 0)
    {
    // (sucessor)get small value in right sub tree this allocated in most left
    IntervalTreeNode* nodeToReplace = getSmallestNodeInRight(root->right);
    // replace your node
    root->low = nodeToReplace->low;
    root->high = nodeToReplace->high;
    // update max
    root->MAX = nodeToReplace->MAX;

    // if node in right make arrow right ->point to nodeToReplace->right
    if(nodeToReplace == nodeToReplace->parent->right)
    nodeToReplace->parent->right = nodeToReplace->right;

    // if node in left make arrow left ->point to nodeToReplace->right
    else
    nodeToReplace->parent->left = nodeToReplace->right;


    // assign node help to update max == parent of node that you replace it
    nodeHelpeToUpdateMax = nodeToReplace->parent;
    // delete replace node
    delete nodeToReplace;
    }
    // node do not have child in right (nave only one child )
    else if(root->right == 0)
    {
    // updating on low and high  and max
    root->low = root->left->low;
    root->high = root->left->high;
    root->MAX = root ->left->MAX;
    root->right = root->left->right;

    IntervalTreeNode* tempToDelete = root->left;

    root->left = root->left->left;

    nodeHelpeToUpdateMax = root;

    delete tempToDelete;

    }
    // have one child in right the same update in tree like binary search tree
    else
    {
    root->low = root->right->low;
    root->high = root->right->high;
    root->MAX = root ->right->MAX;

    root->left = root->right->left;

    IntervalTreeNode* tempToDelete = root->right;

    root->right = root->right->right;

    nodeHelpeToUpdateMax = root;

    delete tempToDelete;
    }
    // function that call recursive on path that walk on it to update maximum on each node after delete
    MaxUpdate(nodeHelpeToUpdateMax);

    }
    // this for go to left sub tree like search on node to delete it
    else if(low < root->low)
    {
    deleteInterval(root->left,low,high);
    }
    // this for go to right sub tree like search on node to delete it
    else if(low > root->low)
    {
    deleteInterval(root->right,low,high);
    }
    }
    // not found the node user want to delete it
    else
    {
    cout<<"Interval with low = "<<low<<" and high = "<<high<<" is Not found"<<endl;
    }
    }
    //(successor) to get smallest value on left on right sub tree
    IntervalTreeNode* getSmallestNodeInRight (IntervalTreeNode* DeletedNode)
    {

    IntervalTreeNode* tempToTraverse = DeletedNode;

    while(tempToTraverse->left != 0)
    {
    tempToTraverse = tempToTraverse->left;
    }
    return tempToTraverse;
    }
    // update the max in each node node after deleting
    void MaxUpdate(IntervalTreeNode* targetNode)
    {
    if(targetNode != 0)
    {

    IntervalTreeNode* tempToTraverse = targetNode;

    while(tempToTraverse != 0)
    {
    // have to child
    if(tempToTraverse->left != 0 && tempToTraverse->right !=0)
    {
    // compare in my max and max in left node and max in right node
    tempToTraverse->MAX = max(tempToTraverse->high,max(tempToTraverse->left->MAX,tempToTraverse->right->MAX));
    }
    // this is leaf node
    else if(tempToTraverse->left == 0 && tempToTraverse->right == 0)
    {
    // make max = max of temp
    tempToTraverse->MAX = tempToTraverse->high;
    }
    // have one child on right subtree
    else if(tempToTraverse->left == 0)
    {
    // compare max of my node and max right child
    tempToTraverse->MAX = max(tempToTraverse->high,tempToTraverse->right->MAX);
    }
    // compare max of my node and  max left child and put max value
    else
    {
    tempToTraverse->MAX = max(tempToTraverse->high,tempToTraverse->left->MAX);
    }
    // jump on node up to make updating
    tempToTraverse = tempToTraverse->parent;
    }
    }
    }
    // search for all overlapping interval
    void searchFor(IntervalTreeNode* crrNode, int low,int high)
    {

    if(crrNode != 0)
    {
    //go to right to check interval
    if(crrNode->left && low > crrNode->left->MAX)
    {
    // condition of overlapping interval
    if(low <= crrNode->high && crrNode->low <= high)
    {
    counterToOverlapp++;
    cout<<"lo: "<<crrNode->low<<", hi: "<<crrNode->high<<endl;
    }
    // call function that get all overlapping in tree
    searchFor(crrNode->right,low,high);
    }
    // this will go to left and right sub tree because we may found overlapping in left and right
    else
    {
    searchFor(crrNode->left,low,high);

    if(low <= crrNode->high && crrNode->low <= high)
    {
    // this counter=0 if we do not have overlapping
    counterToOverlapp++;
    cout<<"lo: "<<crrNode->low<<", hi: "<<crrNode->high<<endl;
    }

    searchFor(crrNode->right,low,high);
    }
    }
    }
    // print tree on order left middle and right
    void printInOrder(IntervalTreeNode* root)
    {
    if(root != 0)
    {
    printInOrder(root->left);
    cout<<"Lo: "<<root->low<<", hi: "<<root->high<<", Max = "<<root->MAX<<endl;
    printInOrder(root->right);
    }
    }
// public function that can user use it make concept encapsulation
public:
    // counter that check if you have  overlapping or no
    int counterToOverlapp;

    IntervalTree()
    {
    counterToOverlapp = 0;
    root = 0;
    }
    // take tow interval from user and pass it to private function
    void insertInterval(int low,int high)
    {
    root = insertInterval(root,low,high);
    }
    void deleteInterval(int low, int high)
    {
    deleteInterval(root,low,high);
    }
    // function that user can access it
    void searchFor(int low,int high)
    {
    searchFor(root,low,high);

    if(counterToOverlapp == 0)
    {
    cout<<"No overlapping"<<endl;
    }
    // reset counterToOverlapp = 0
    counterToOverlapp=0;
    }

    void print()
    {
    printInOrder(root);
    cout<<endl;
    }


};

int main()
{
    IntervalTree t1;
    t1.insertInterval(5,10);
    t1.insertInterval(3,8);
    t1.insertInterval(10,15);
    t1.insertInterval(1,1);
    t1.insertInterval(9,11);
    t1.insertInterval(16,18);
    t1.print();
    cout<<"Search for Interval overlapping with 2 and 17 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t1.searchFor(2,17);
    cout<<"Search for Interval overlapping with 8 and 10 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t1.searchFor(8,10);
    t1.deleteInterval(1,1);
    t1.print();
    cout<<"Search for Interval overlapping with 19 and 22 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t1.searchFor(19,22);
    cout<<"Search for Interval overlapping with 0 and 8 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t1.searchFor(0,8);
    cout<<endl;
    /////////////////////////////////////////////////////////
    IntervalTree t2;
    t2.insertInterval(16,21);
    t2.insertInterval(25,30);
    t2.insertInterval(8,9);
    t2.insertInterval(5,8);
    t2.insertInterval(15,23);
    t2.insertInterval(17,19);
    t2.insertInterval(26,26);
    t2.insertInterval(0,3);
    t2.insertInterval(6,10);
    t2.insertInterval(19,20);
    t2.print();
    cout<<"Search for Interval overlapping with 25 and 40 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t2.searchFor(25,40);
    t2.deleteInterval(15,23);
    t2.print();
    cout<<"Search for Interval overlapping with 20 and 20 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t2.searchFor(20,20);
    cout<<"Search for Interval overlapping with 20 and 28 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t2.searchFor(20,28);
    cout<<"Search for Interval overlapping with 35 and 40 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t2.searchFor(35,40);
    cout<<endl;
    //////////////////////////////////////////////////
    IntervalTree t3;
    t3.insertInterval(15,20);
    t3.insertInterval(10,30);
    t3.insertInterval(5,20);
    t3.insertInterval(12,15);
    t3.insertInterval(17,19);
    t3.insertInterval(30,40);
    t3.print();
    cout<<"Search for Interval overlapping with 2 and 6 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t3.searchFor(2,16);
    t3.deleteInterval(15,20);
    t3.print();
    cout<<"Search for Interval overlapping with 20 and 24 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t3.searchFor(20,24);
    cout<<endl;
    ////////////////////////////////////////////////
    IntervalTree t4;
    t4.insertInterval(20,36);
    t4.insertInterval(3,41);
    t4.insertInterval(0,1);
    t4.insertInterval(29,99);
    t4.insertInterval(10,15);
    t4.print();
    cout<<"Search for Interval overlapping with 100 and 100 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t4.searchFor(100,100);
    cout<<endl;
    t4.deleteInterval(3,41);
    t4.print();
    cout<<"Search for Interval overlapping with 35 and 100 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t4.searchFor(35,100);
    cout<<"Search for Interval overlapping with 3 and 10 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t4.searchFor(3,10);
    cout<<"Search for Interval overlapping with 10 and 11 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t4.searchFor(10,11);
    cout<<endl;
    /////////////////////////////
    IntervalTree t5;
    t5.insertInterval(15,40);
    t5.insertInterval(10,28);
    t5.insertInterval(5,24);
    t5.insertInterval(11,16);
    t5.insertInterval(16,23);
    t5.insertInterval(18,60);
    t5.print();
    cout<<"Search for Interval overlapping with 5 and 20 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t5.searchFor(5,20);
    cout<<"Search for Interval overlapping with 3 and 16 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t5.searchFor(3,16);
    cout<<"Search for Interval overlapping with 5 and 17 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t5.searchFor(5,17);
    cout<<"Search for Interval overlapping with 20 and 25 "<<endl;
    cout<<"Overlapped Intervals: "<<endl;
    t5.searchFor(20,25);
    cout<<endl;
    ////////////////////////////////////////
//    IntervalTree t6;
//    t6.insertInterval(7,15);
//    t6.insertInterval(1,5);
//    t6.insertInterval(20,25);
//    t6.insertInterval(16,19);
//    t6.insertInterval(22,28);
//    t6.print();
//    cout<<"Search for Interval overlapping with 3 and 7 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t6.searchFor(3,7);
//     cout<<"Search for Interval overlapping with 12 and 25 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t6.searchFor(12,25);
//     cout<<"Search for Interval overlapping with 16 and 19 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t6.searchFor(16,19);
//     cout<<"Search for Interval overlapping with 121 and 122 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t6.searchFor(121,122);
//    cout<<endl;
/////////////////////////////
//    IntervalTree t7;
//    t7.insertInterval(7,15);
//    t7.insertInterval(1,5);
//    t7.insertInterval(20,25);
//    t7.insertInterval(16,19);
//    t7.insertInterval(22,28);
//    t7.print();
//    cout<<"Search for Interval overlapping with 4 and 10 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t7.searchFor(4,10);
//    cout<<"Search for Interval overlapping with 0 and 10 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t7.searchFor(0,10);
//    cout<<"Search for Interval overlapping with 1 and 5 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t7.searchFor(1,5);
//    cout<<"Search for Interval overlapping with 19 and 30 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t7.searchFor(19,30);
//    cout<<endl;
//////////////////////////////
//    IntervalTree t8;
//    t8.insertInterval(7,15);
//    t8.insertInterval(1,5);
//    t8.insertInterval(20,25);
//    t8.insertInterval(16,19);
//    t8.insertInterval(22,28);
//    t8.print();
//    cout<<"Search for Interval overlapping with 7 and 13 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t8.searchFor(7,13);
//    cout<<"Search for Interval overlapping with 2 and 13 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t8.searchFor(2,13);
//    cout<<"Search for Interval overlapping with 2 and 21 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t8.searchFor(2,21);
//    cout<<"Search for Interval overlapping with 5 and 55 "<<endl;
//    cout<<"Overlapped Intervals: "<<endl;
//    t8.searchFor(5,55);
//    cout<<endl;
      return 0;
}
