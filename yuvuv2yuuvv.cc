/*
 * convert yuvuv type to yyuuvv type
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
    cout << " YUVUV2YUUVV: ./yuvuv2yuuvv in.yuv out.yuv" << endl;
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

    string out_v = out + "_v.bin";
    cout << "inwrite file :" << in << "outwrite file :" << out << endl;
    ifstream fin;
    ofstream fout;
    ofstream foutv;
    fin.open(in.c_str(), fstream::binary);
    fout.open(out.c_str(), fstream::binary);
    foutv.open(out_v.c_str(), fstream::app | fstream::binary);

    int length = 0, size = 0, pos = 0;
    fin.seekg(0, ios_base::end);
    size = fin.tellg();
    fin.seekg(0, ios_base::beg);
    char c;
    int y_len = size * 2 / 3;
    while (pos < size) {
        fin.read(&c, 1);
        if (pos < y_len ) {
            fout.write(&c,1);
        } 
        else if ( (pos - y_len ) & 0x1 ) {
            foutv.write(&c,1);
        } else {
            fout.write(&c,1);
        }
        pos += 1;
    }

    pos = 0;
    foutv.close();
    fin.close();

    fin.open(out_v.c_str(), fstream::binary);
    while (pos < size / 6) {
        fin.read(&c, 1);
        fout.write(&c, 1);
        pos ++;
    }


    fout.close();
    fin.close();

    //remove(out_v.c_str());
}
