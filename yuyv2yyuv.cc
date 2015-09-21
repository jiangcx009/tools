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

    string out_y = out + "-y";
    string out_u = out + "-u";
    string out_v = out + "-v";

    cout << "inwrite file :" << in << "outwrite file :" << out << endl;
    ifstream fin;
    ofstream fouty, foutu, foutv;
    fin.open(in.c_str(), fstream::binary);
    fouty.open(out_y.c_str(), fstream::binary );
    foutu.open(out_u.c_str(), fstream::binary );
    foutv.open(out_v.c_str(), fstream::binary );

    cout << "open temp files:" << out_y << ", " << out_u << ", " << out_v << endl;
    unsigned cnt = 0, cnt_y = 0;
    char c[4];
    cout << "fin is eof?:" << fin.eof() << endl;
    while (!fin.eof()) {
        fin.read(c, 4);
        /*y bytes*/
        fouty.write(&c[0], 1); 
        fouty.write(&c[3], 1); 
        foutu.write(&c[2], 1);
        foutv.write(&c[4], 1);
        cnt += 4;
        cnt_y += 2;
    }
    cout << "file size:" << cnt << " y cnt: " << cnt_y << endl;

    fin.close();
    fouty.close();
    foutu.close();
    foutv.close();

    ifstream finy, finu, finv;
    ofstream fout;
    finy.open(out_y.c_str(), fstream::binary);
    finu.open(out_u.c_str(), fstream::binary);
    finv.open(out_v.c_str(), fstream::binary);
    fout.open(out.c_str(), fstream::binary);
    char cache[1024];
    while (!finy.eof()) {
        memset(cache, 0, 1024);
        finy.read(cache, 1024);
        fout.write(cache, 1024);
    }

    while (!finu.eof()) {
        memset(cache, 0, 1024);
        finu.read(cache, 1024);
        fout.write(cache, 1024);
    }

    while (!finv.eof()) {
        memset(cache, 0, 1024);
        finv.read(cache, 1024);
        fout.write(cache, 1024);
    }
    fout.close();
}
