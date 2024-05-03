#include <iostream>
#include <vector>
using namespace std;

struct Node {
  int key;
  int distance;
  Node *next;
};

struct GraphNode {
  int key;
  Node *listpointer;
  int sourceDist;
  char state;
};


class Graph {
private:
  vector<GraphNode> adjlist;
public:
  Graph() {};
  ~Graph() {};
  void AddNewGraphNode(int newgraphnode);
  void AddNewEdgeBetweenGraphNodes(int A, int B, int distance);
  void DijkstraAlgorithm();
  char stateReturn(int key);
  void stateUpdate(int key, int dist);
};

struct HeapNode {
  int data;
  int distance;
};


class Heap {
private:
  vector<HeapNode> Heaplist;
  int last;
public:
  Heap() {last = -1;};
  ~Heap() {};
  void InsertHeap(int newthing, int dist);
  bool Delete(int item);
  void DeleteRoot();
  int Heapsize();
  int HeapKeyExtract();
  int HeapDistExtract();
};

int main() {
  Graph mygraph;
  Heap test;
  int n, m, indx, v, w, l;
  cin >> n >> m;

  for (indx = 0; indx <= (n - 1); indx++) {
    mygraph.AddNewGraphNode(indx);
  }
  for (indx = 0; indx < m; indx++) {
    cin >> v >> w >> l;
    mygraph.AddNewEdgeBetweenGraphNodes(v, w, l);
  }
  mygraph.DijkstraAlgorithm();
}



void AddNodetoFront(Node*& listpointer, int newkey, int newdistance) {
  Node *temp;
  temp = new Node;
  temp -> key = newkey;
  temp -> distance = newdistance;
  temp -> next = listpointer;
  listpointer = temp;
}


void Graph::AddNewGraphNode(int newgraphnode) {
  GraphNode temp;
  temp.key = newgraphnode;
  temp.listpointer = NULL;
  temp.state = 't';
  temp.sourceDist = 1e6;
  adjlist.push_back(temp);
}

void Graph::AddNewEdgeBetweenGraphNodes(int A, int B, int distance) {
  int a;
  for (a = 0; a < (int) adjlist.size(); a++){
    if (A == adjlist[a].key) break;
  }
  AddNodetoFront(adjlist[a].listpointer, B, distance);
}

void NodeNeighbourFind(Node*& listpointer, int givenDist, Graph &graph, Heap &heap) {
  int final, carry, carryKey;
  Node *temp;
  temp = listpointer;
  carry = 100;
  carryKey = 0;
  while(temp!=NULL) {
    final = (temp -> distance) + givenDist;
    if (graph.stateReturn(temp -> key) == 't') {
      if (final < carry) {
        carry = final;
        carryKey = temp -> key;
      }
    }
    temp = temp -> next;
  }
  if (graph.stateReturn(carryKey) == 't') {
    heap.InsertHeap(carryKey, carry);
    graph.stateUpdate(carryKey, carry);
  }
}

void Graph::DijkstraAlgorithm() {
  Heap temp;
  int indx, key, dist;
  int source = adjlist[0].key;
  adjlist[source].sourceDist = 0;
  adjlist[source].state = 'p';
  temp.InsertHeap(adjlist[0].key, adjlist[0].sourceDist);
  while (temp.Heapsize() != 0) {
    key = temp.HeapKeyExtract();
    dist = temp.HeapDistExtract();
    temp.DeleteRoot();
    NodeNeighbourFind(adjlist[key].listpointer, dist, *this, temp);
  }
  for (indx = 0; indx < (int) adjlist.size(); indx++) {
    if (adjlist[indx].state == 't') {
      cout << "inf ";
    } else {
      cout << adjlist[indx].sourceDist << " ";
    }
  }

}


void Graph::stateUpdate(int key, int dist) {
  adjlist[key].state = 'p';
  adjlist[key].sourceDist = dist;
}

char Graph::stateReturn(int key) {
  return adjlist[key].state;
}


void Heap::InsertHeap(int newthing, int dist){
  HeapNode temp;
  temp.data = newthing;
  temp.distance = dist;
  Heaplist.push_back(temp);
  last=last+1;
  if (last==0) return;
  int child_index = last;
  int par_index = 0;
  bool swapping = true;
  while (swapping==true){
    swapping=false;
    if (child_index % 2 == 0) {
      par_index = child_index / 2 - 1;

    } else {
      par_index = child_index / 2;
    }
    if(par_index >= 0){
      if(Heaplist[child_index].distance < Heaplist[par_index].distance) {
        swap(Heaplist[child_index].distance ,Heaplist[par_index].distance);
        swapping=true;
        child_index=par_index;
      }
    }
    if (child_index == 0) {break;}
  }
}


void Heap::DeleteRoot(){
  if(last < 0) return;
  Heaplist[0].distance = Heaplist[last].distance;
  Heaplist[last].distance = 0;
  last = last - 1;
  int parindex = 0;
  int leftindex = parindex*2+1;
  int rightindex = parindex*2+2;

  while (leftindex <= last && (Heaplist[parindex].distance > Heaplist[leftindex].distance || Heaplist[parindex].distance > Heaplist[rightindex].distance)) {
    if (Heaplist[rightindex].distance > Heaplist[leftindex].distance){
      swap(Heaplist[leftindex].distance , Heaplist[parindex].distance);
      parindex = leftindex;
    } else {
      swap(Heaplist[rightindex].distance , Heaplist[parindex].distance);
      parindex = rightindex;
    }
    leftindex = parindex*2+1;
    rightindex = parindex*2+2;
    if(leftindex > last) { break;}
    else {
      if(rightindex > last){
        if (Heaplist[parindex].distance > Heaplist[leftindex].distance) {
          swap(Heaplist[parindex].distance , Heaplist[leftindex].distance);
        }
        break;
      }
    }
  }
  Heaplist.pop_back();
  return;
}

int Heap::Heapsize() {
  return Heaplist.size();
}

int Heap::HeapKeyExtract() {
  return Heaplist[0].data;
}

int Heap::HeapDistExtract() {
  return Heaplist[0].distance;
}
