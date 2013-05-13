#include <iostream>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

int maxx, maxy, maxz, maxvx, maxvy, maxvz, n;
double step;

bool used[1200][1200][1200];

int main()
{
    int x, y, z, vx, vy, vz;
    srand(time(NULL));
    cin>>step>>maxx>>maxy>>maxz>>maxvx>>maxvy>>maxvz>>n;
    used[0][0][0] = true;
    while(n--)
    {
        x =  rand() % (maxx<<1);
        y =  rand() % (maxy<<1);
        z =  rand() % (maxz<<1);
        if(used[x][y][z])
        {
            n++;
            continue;
        }
        used[x][y][z] = true;
        x -= maxx; y -= maxy; z -= maxz;
        vx = rand() % (maxvx<<1) - maxvx;
        vy = rand() % (maxvy<<1) - maxvy;
        vz = rand() % (maxvz<<1) - maxvz;
        cout<<0<<" "<<step<<" "<<x<<" "<<y<<" "<<z<<" "<<vx<<" "<<vy<<" "<<vz<<endl;
    }
    return 0;
}
