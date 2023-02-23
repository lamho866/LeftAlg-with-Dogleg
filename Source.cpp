#include <bits/stdc++.h>
using namespace std;

struct PinCol
{
    int col, up, down;
    void set(int _curSt, int _curUp, int _curDown){col = _curSt, up = _curUp, down = _curDown;}
    PinCol(int _col = -1, int _up = -1, int _down = -1) : col(_col), up(_up), down(_down){}
}pinCur[1005];

struct Edge
{
    PinCol st, ed;
    int track, id;
    bool used;
    Edge(int _id, PinCol _st, PinCol _ed) : id(_id), st(_st), ed(_ed){used = false;}
    int wireLen(int totalTrack){
        int wr = 0;
        wr += (id == st.up) ? track : totalTrack - track + 1;
        wr += (id == ed.down) ? track : totalTrack - track + 1;
        return wr;
    }
};

struct Cost
{
    int wr, v;
    Cost (int _wr = 0,int _v = 0): wr(_wr), v(_v){}
    bool operator < (const Cost &bCost){
        if(wr == bCost.wr)
            return v < bCost.v;
        return wr < bCost.wr;
    }
};


bool edgeCmp(Edge a, Edge b){
    if(a.st.col == b.st.col)
        return a.st.up == a.id;
    return a.st.col < b.st.col;
}

int upLayer[1005];
int dowLayer[1005];
int colBound, netBound; 
vector<Edge> edgeSet;

void readFile(char argv[]){
    ifstream rFile(argv);
    rFile >> colBound >> netBound;
    for(int i = 0; i < colBound; ++i)
        rFile >> upLayer[i] >> dowLayer[i];
    rFile.close();
}

void addEdge(int pinIdx, PinCol cur){
    if(pinIdx == 0) return;
    edgeSet.push_back(Edge(pinIdx, pinCur[pinIdx], cur));
    pinCur[pinIdx] = cur;
}

void edgeInit(){
    //Initial
    for(int i = 0; i < colBound; ++i){
        int upPin = upLayer[i], downPin = dowLayer[i];
        if(pinCur[upPin].col == -1)
            pinCur[upPin].set(i + 1, upPin, downPin);
        else
            addEdge(upPin, PinCol(i + 1, upPin, downPin));
        
        if(pinCur[downPin].col == -1)
            pinCur[downPin].set(i + 1, upPin, downPin);
        else
            addEdge(downPin, PinCol(i + 1, upPin, downPin));
    }
    sort(edgeSet.begin(), edgeSet.end(), edgeCmp);
}

void leftEdge(int &finalTrack, Cost &finalCost){
    int track = 1, lst, lstId;
    bool canPlace;
    vector<Edge> temp;
    do{
        lst = 0, lstId = 0;
        canPlace = false;
        for(auto &e: edgeSet){
            if(!e.used){
                if(lst < e.st.col || (lst == e.st.col && lstId == e.id)){
                    //if id is same, doesn't need addition v
                    if(lstId == e.id)
                        finalCost.v -= 2;

                    e.track = track, e.used = true;
                    temp.push_back(e);
                    lst = e.ed.col, lstId = e.id;
                    finalCost.v += 2;
                }else{
                    canPlace = true;
                }
            }
        }
        // printf("lst: %d\n", lst);
        if(lst)
            track++;
    }while (lst != 0);
    edgeSet = temp;
    finalTrack = track - 1;
    //Cal the vertical wireLen Cost
    int preTrack = 0, preId = 0;
    for(auto &e: edgeSet){
        //Cal the vertical wireLen Cost
        finalCost.wr += e.wireLen(finalTrack);
        //Cal Horizontal wireLen
        finalCost.wr += (e.ed.col) - e.st.col;
        if(preTrack == e.track && preId == e.id)
            finalCost.wr -= 1;
        //Same net, Same track case
        preTrack = e.track, preId = e.id;
    }
}

bool noOverLap(int i, int j){
     Edge &a = edgeSet[i], &b = edgeSet[j];
     return a.ed.col < b.st.col ||
        a.ed.col == b.ed.col && a.id == b.id; 
}

//Move edge i to j
bool canPlace(int i, int j){
    Edge &target = edgeSet[i];
    //Condition 01
    bool cond1 = (j - 1 < 0)? true : noOverLap(j - 1, i);
    bool cond2 = (j + 1 >= edgeSet.size())? true : noOverLap(i, j + 1);
    return cond1 && cond2;
}

bool canEdgeSwap(int i, int j){
    Edge &a = edgeSet[i], &b = edgeSet[j];
    return a.track != b.track && canPlace(i, j) && canPlace(j, i);
}


//0 = " ", 1 = "-", 2 = "|", 3 = "    x", 4 = "---x"
int g[500][1000] = {0};

void verticalDraw(int totalTrack, int curTrack,int col , bool isUp){
    if(isUp)
        for(int i = 1; i <= curTrack; ++i)
            g[i][col] = 2;
    else
        for(int i = curTrack; i <= totalTrack; ++i)
            g[i][col] = 2;
}

void printGraph(int totalTrack){
    //draw vertical
    for(auto e: edgeSet){
        verticalDraw(totalTrack, e.track, e.st.col, e.st.up == e.id);
        verticalDraw(totalTrack, e.track, e.ed.col, e.ed.up == e.id);
    }
    //draw horizontal
    for(int i = 0; i < edgeSet.size(); ++i){
        Edge &e = edgeSet[i];
        for(int i = e.st.col; i <= e.ed.col; ++i)
            g[e.track][i] = 1;

        if(i - 1 >= 0 && edgeSet[i - 1].track == e.track && edgeSet[i - 1].id == e.id)
            g[e.track][e.st.col] = 4;
        else
            g[e.track][e.st.col] = 3;
        g[e.track][e.ed.col] = 4;
    }


    for(int i = 0; i < colBound; ++i)
        printf("%4d", upLayer[i]);
    printf("\n");
    for(int i = 0; i < colBound; ++i)
        printf("----");
    printf("----\n");
    
    for(int i = 1; i <= totalTrack; ++i){
        for(int j = 1; j <= colBound; ++j){
            if(g[i][j] == 1) printf("----");
            else if(g[i][j] == 2) printf("   |");
            else if(g[i][j] == 3) printf("   x");
            else if(g[i][j] == 4) printf("---x");
            else printf("    ");
        }
        printf("\n");
    }

    for(int i = 0; i < colBound; ++i)
        printf("----");
    printf("----\n");
    for(int i = 0; i < colBound; ++i)
        printf("%4d", dowLayer[i]);
    printf("\n");
    
}


int main(int argc, char* argv[]){
    readFile(argv[1]);
    edgeInit();
    // for(auto e : edgeSet)
    //     printf("track: %d, net: %d, (%d, %d)\n", e.track, e.id, e.st.col, e.ed.col);

    int track;
    Cost c;
    leftEdge(track, c);
    
    // printf("edgeSet: %d\n", edgeSet.size());
    int preTrack = 0;
    for(auto e : edgeSet){
        if(preTrack != e.track){
            printf("\ntrack:%d\n", e.track);
            preTrack = e.track;
        }
        printf("net: %d, (%d, %d)\n", e.id, e.st.col, e.ed.col);
    
    }
    printf("\nCost c: (%d,%d,%d)\n", track, c.wr, c.v);
    printf("\n--------------Graph------------------------\n\n");
    printGraph(track);
}
