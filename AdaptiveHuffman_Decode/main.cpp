#include <bits/stdc++.h>

using namespace std;


int m = 26;
int e, r;

unordered_map<char, int> exists, order;


string input, theSet = "abcdefghijklmnopqrstuvwxyz", decoded;


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

void add(char c){

    if(exists[c]){
        findNodeWithChar(c);
        (*nodeToBeIncremented).weight++;
        update(nodeToBeIncremented);


    }else{

        node *newNode = new node;
        node *newNYT = new node;

        //set the new node
        (*newNode).weight++;
        (*newNode).nodeNumber = (*NYT).nodeNumber - 1;
        (*newNode).c = c;
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
    exists[c]++;
}


int evalBits(string currBits){

    int power = currBits.size() - 1;
    int res = 0;

    for(int i = 0; i < currBits.size(); i++){
        res += (currBits[i] - '0')*pow(2, currBits.size() - i - 1);
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




    int i = 0;
    node *currNode = &root;

    while(true){

        char currBit = input[i];
        if(!((*currNode).left) && !((*currNode).right)) {
            if(currNode == NYT){
                string currBits = "";
                currBits = input.substr(i, e);
                int currValue = evalBits(currBits);

                i += e;

                if(currValue < r){
                    currBits += input[i];
                    currValue = evalBits(currBits);
                    i++;
                }else{
                    currValue += r;
                }

                decoded += theSet[currValue];
                add(theSet[currValue]);
                cout << "i = " << i << " bits " << currBits << "  value = " << currValue << endl;
                cout << "\n\n\n";
                printTree();
                cout << "\n\n\n";

            }else{
                decoded += (*currNode).c;
                add((*currNode).c);

                cout << "i = " << i << endl;
                cout << "\n\n\n";
                printTree();
                cout << "\n\n\n";
            }
            currNode = &root;
        }else if(i < input.size()){
            if(currBit == '1'){
                cout << "from " << (*currNode).nodeNumber << " " << (*currNode).weight << " " << (*currNode).c << endl;
                currNode = (*currNode).right;
                cout << "to " << (*currNode).nodeNumber << " " << (*currNode).weight << " " << (*currNode).c << endl;
            }else{
                cout << "from " << (*currNode).nodeNumber << " " << (*currNode).weight << " " << (*currNode).c << endl;
                currNode = (*currNode).left;
                cout << "to " << (*currNode).nodeNumber << " " << (*currNode).weight << " " << (*currNode).c << endl;
            }
            i++;
            cout << ">> " << i << endl;
        }else{
            break;
        }


    }

    cout << decoded << endl;




    return 0;
}
