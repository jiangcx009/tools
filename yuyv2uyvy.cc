/*
 * convert yuyv type yuv to yyuv type
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>

using namespace std;

void help(void)
{
    cout << " YYUV2YUYV: ./yuyv2yyuv in.yuv out.yuv" << endl;
}


int main(int argc, char *argv[]) {
    //char *in = NULL, *out = NULL;
    string in, out;

    if (!(argc == 3 || argc == 4))  {
        help();
        return 0;
    }
    if (argc == 3) {
        in = (char *)argv[1];
        out = (char *)argv[2];
    } else {
        in = (char *)argv[2];
        out = (char *)argv[3];
    }

    ifstream fin;
    ofstream fout;
    fin.open(in.c_str(), fstream::binary);
    fout.open(out.c_str(), fstream::binary );

    fin.seekg(0, fin.end);
    int size = fin.tellg();
    fin.seekg(0, fin.beg);

    char c[4];
    while (size > 0) {
        fin.read(c, 4);
        /*y bytes*/
        fout.write(&c[1], 1); 
        fout.write(&c[0], 1); 
        fout.write(&c[3], 1);
        fout.write(&c[2], 1);
        size -= 4;
    }

    fin.close();
    fout.close();
}
