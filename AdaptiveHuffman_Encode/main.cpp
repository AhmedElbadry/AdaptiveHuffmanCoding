#include <bits/stdc++.h>

using namespace std;


int m = 26;
int e, r;

unordered_map<char, int> exists, order;


string input, theSet = "abcdefghijklmnopqrstuvwxyz", encoded;


struct node {
    int weight = 0;
    int nodeNumber;
    char c = ' ';

    node *parent = NULL;
    node *left = NULL;
    node *right = NULL;
};

node root;
node *NYT = &root;
node *nodeToBeIncremented;

void printTree(node *n = &root){
    if(!n)
        return;

    printTree((*n).left);
    cout << (*n).weight << " " << (*n).nodeNumber << " " << (*n).c << endl;
    printTree((*n).right);
}

void findNodeWithChar(char c, node *n = &root, bool found = false){
    if(!n || found)
        return;

    if(c == (*n).c && !(*n).left && !(*n).right){
//        cout << "found: " << (*n).weight << " " << (*n).nodeNumber << " " << (*n).c << endl;
//        cout << n << endl;
        found = true;
        nodeToBeIncremented = n;
    }

    findNodeWithChar(c, (*n).left, found);
    findNodeWithChar(c, (*n).right, found);
}

void update(node *parent){

    while((*parent).parent){

        parent = (*parent).parent;

        node *left = (*parent).left;
        node *right = (*parent).right;

        bool isSw = false;


        cout << (*left).weight << "  ===  " << (*right).weight << endl;
        if((*left).weight - 1 >= (*right).weight){
            cout << "switching:" << endl;
            cout << (*((*parent).left)).weight << " " << (*((*parent).left)).nodeNumber << " " << (*((*parent).left)).c << endl;
            cout << (*((*parent).right)).weight << " " << (*((*parent).right)).nodeNumber << " " << (*((*parent).right)).c << endl;
            cout << "::\n";

            //switch nodes
            int leftNodeNumberTemp = (*left).nodeNumber;
            (*left).nodeNumber = (*right).nodeNumber;
            (*right).nodeNumber = leftNodeNumberTemp;

            (*parent).left = right;
            (*parent).right = left;

            cout << "switching:" << endl;
            cout << (*((*parent).left)).weight << " " << (*((*parent).left)).nodeNumber << " " << (*((*parent).left)).c << endl;
            cout << (*((*parent).right)).weight << " " << (*((*parent).right)).nodeNumber << " " << (*((*parent).right)).c << endl;
            cout << "::\n";

            isSw = true;

        }

        (*parent).weight++;
    }
}



string getPath(node* n){
    string res = "";
    while((*n).parent){
        node *parent = (*n).parent;

        if((*parent).left == n){
            res += "0";
        }else{
            res += "1";
        }
        n = parent;
    }
    reverse(res.begin(), res.end());
    return res;
}


string toNbits(int num, int nBits){

    string res = "";

    for(int i = 0; i < nBits; i++){
        res += "0";
    }

    int i = nBits - 1;

    while(num){
        res[i] = '0' + num%2;
        num /= 2;
        i--;
    }

    return res;
}


int main()
{
    freopen("input.txt", "rt", stdin);

    cin >> theSet >> input;


    //initialize the root
    m = theSet.size();
    root.nodeNumber = 2*m - 1;

    e = log2(m);
    r = m - pow(2, e);

    //to get the order of chars
    for(int i = 0; i < theSet.size(); i++){
        order[theSet[i]] = i + 1;
    }




    for(int i = 0; i < input.size(); i++){
        string encodedChar = "";


        if(exists[input[i]]){
            findNodeWithChar(input[i]);

            encodedChar = getPath(nodeToBeIncremented);

            (*nodeToBeIncremented).weight++;
            update(nodeToBeIncremented);


        }else{

            encodedChar += getPath(NYT);

            int k = order[input[i]];

            if(k <= 2*r){
                encodedChar += toNbits(k-1, e+1);
            }else{
                encodedChar += toNbits(k - r - 1, e);
            }

            node *newNode = new node;
            node *newNYT = new node;

            //set the new node
            (*newNode).weight++;
            (*newNode).nodeNumber = (*NYT).nodeNumber - 1;
            (*newNode).c = input[i];
            (*newNode).parent = NYT;


            //set the new NYT
            (*newNYT).nodeNumber = (*NYT).nodeNumber - 2;
            (*newNYT).parent = NYT;



            //increment old NYT
            //cout << "old nyt" << (*NYT).weight << endl;
            //(*NYT).weight++;

            //connect new nodes
            (*NYT).right = newNode;
            (*newNode).parent = NYT;

            (*NYT).left = newNYT;
            (*newNYT).parent = NYT;


            NYT = newNYT;

            update(newNode);


        }
        exists[input[i]]++;

        encoded += encodedChar + " ";

        cout << "input: " << input[i] << " encoded to " << encodedChar << endl;



        cout << "\n\n\n";
        printTree();
        cout << "\n\n\n";

    }

    cout << encoded << endl;

    return 0;
}
