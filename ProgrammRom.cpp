// ProgrammRom.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#define CLI   0x80000000
#define CSetL   0b01000000000000000000000000000000
#define CSetH   0b00100000000000000000000000000000
#define CE      0b00010000000000000000000000000000
#define COH     0b00001000000000000000000000000000
#define COE     0b00000100000000000000000000000000
#define MARI    0b00000010000000000000000000000000
#define ML      0b00000001000000000000000000000000
#define ME      0b00000000100000000000000000000000
#define IOO     0b00000000010000000000000000000000
#define IOI     0b00000000001000000000000000000000
#define IOARLI  0b00000000000100000000000000000000
#define IOARHI  0b00000000000010000000000000000000
#define RALU    0b00000000000001000000000000000000
#define CSI     0b00000000000000100000000000000000
#define IRO     0b00000000000000010000000000000000
#define IRI     0b00000000000000001000000000000000
#define THO     0b00000000000000000100000000000000
#define THI     0b00000000000000000010000000000000
#define BO      0b00000000000000000001000000000000
#define BI      0b00000000000000000000100000000000
#define AO      0b00000000000000000000010000000000
#define AI      0b00000000000000000000001000000000
#define ALUI    0b00000000000000000000000100000000
#define ALUO    0b00000000000000000000000010000000
#define RINT    0b00000000000000000000000001000000
#define SUB     0b00000000000000000000000000100000
#define INV     0b00000000000000000000000000010000
#define AND     0b00000000000000000000000000001000
#define OR      0b00000000000000000000000000000100
#define NOR     0b00000000000000000000000000000010
#define RESET     1



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

 string numToHex(unsigned int a) {
    char c = 0;
    string b = "";
    for (int i = 0; i < 8; i++) {
        int t = a % 16;
        switch (t)
        {
        case 0:
            c = '0';
            break;
        case 1:
            c = '1';
            break;
        case 2:
            c = '2';
            break;
        case 3:
            c = '3';
            break;
        case 4:
            c = '4';
            break;
        case 5:
            c = '5';
            break;
        case 6:
            c = '6';
            break;
        case 7:
            c = '7';
            break;
        case 8:
            c = '8';
            break;
        case 9:
            c = '9';
            break;
        case 10:
            c = 'a';
            break;
        case 11:
            c = 'b';
            break;
        case 12:
            c = 'c';
            break;
        case 13:
            c = 'd';
            break;
        case 14:
            c = 'e';
            break;
        case 15:
            c = 'f';
            break;
        default:
            break;
        }
        a = a >> 4;
        b = c + b;
    }
    return b;
}
 
 void writeROM(vector<string> x) {
     string strROM1;
     string strROM2;
     string strROM3;
     string strROM4;

     for (vector<string>::iterator i = x.begin(); i != x.end(); i++) {
         strROM1 += (*i)[0];
         strROM1 += (*i)[1];
         strROM1 += " ";
         strROM2 += (*i)[2];
         strROM2 += (*i)[3];
         strROM2 += " ";
         strROM3 += (*i)[4];
         strROM3 += (*i)[5];
         strROM3 += " ";
         strROM4 += (*i)[6];
         strROM4 += (*i)[7];
         strROM4 += " ";
     }
     fstream ROM1 = fstream("ROM1", fstream::out);
     ROM1 << strROM1.c_str();
     ROM1.close();
     fstream ROM2 = fstream("ROM2", fstream::out);
     ROM2 << strROM2.c_str();
     ROM2.close();
     fstream ROM3 = fstream("ROM3", fstream::out);
     ROM3 << strROM3.c_str();
     ROM3.close();
     fstream ROM4 = fstream("ROM4", fstream::out);
     ROM4 << strROM4.c_str();
     ROM4.close();
 }
 
 vector<string> connectInstructions(vector<vector<string>> x) {
     vector<string> instrSet = {};
     for (vector<vector<string>>::iterator i = x.begin(); i != x.end(); ++i) {
         int k = 16 - (*i).size();
         for (int j = 0; j < k; j++) {
             (*i).push_back("00000000");
         }
         for (vector<string>::iterator j = (*i).begin(); j != (*i).end(); ++j) {
             instrSet.push_back(*j);
         }
     }
     return instrSet;
 }


int main()
{
    //nop
    vector<string> nop = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(RESET)
    };
    //nop with flags set
    vector<string> anop = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(RALU),
        numToHex(RESET)
    };
    //nop with 2 clock increments for conditional jmp
    vector<string> jnop = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(CE),
        numToHex(CE),
        numToHex(RESET)
    };

    vector<vector<string>> l(4 * 4 * 4 * 2, nop);//= { i0,i1,i2,i3,i4,i5,i6,i7,i8,i9,ia,ib, };
    //lda
    {//lda (imm)
        vector<string> lda1 = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | AI | CE),
        numToHex(RESET) };
        l[0b0000111] = lda1;
        //lda b
        vector<string> lda2 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BO | AI),
            numToHex(RESET)
        };
        l[0b0000001] = lda2;
        //lda io
        vector<string> lda3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(IOO | AI),
            numToHex(RESET)
        };
        l[0b0000010] = lda3;
        //lda [addr]
        vector<string> lda4 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IRI | CE),
            numToHex(COE | MARI),
            numToHex(ME | THI | CE),
            numToHex(THO | IRO | MARI),
            numToHex(ME | AI),
            numToHex(RESET)
        };
        l[0b0000011] = lda4;
        //lda b, [haddr]
        vector<string> lda5 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | THI | CE),
            numToHex(THO | BO | MARI),
            numToHex(ME | AI),
            numToHex(RESET)
        };
        l[0b0000100] = lda5; }
    //sta
    {//sta b
        vector<string> sta1 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BI | AO),
            numToHex(RESET)
        };
        l[0b0100001] = sta1;
        //sta io
        vector<string> sta2 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(AO | IOI),
            numToHex(RESET)
        };
        l[0b0100010] = sta2;
        //sta [addr]
        vector<string> sta3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IRI | CE),
            numToHex(COE | MARI),
            numToHex(ME | THI | CE),
            numToHex(THO | IRO | MARI),
            numToHex(ML | AO),
            numToHex(RESET)
        };
        l[0b0100011] = sta3;
        //sta b, [haddr]
        vector<string> sta4 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | THI | CE),
            numToHex(THO | BO | MARI),
            numToHex(AO | ML),
            numToHex(RESET)
        };
        l[0b0100100] = sta4; 
        //sta c
        vector<string> sta5 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(CSI | AO),
                numToHex(RESET)
        };
        l[0b0100101] = sta5; }
    //ldb
    {//ldb (imm)
        vector<string> ldb1 = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | BI | CE),
        numToHex(RESET) };
        l[0b0001111] = ldb1;
        //ldb a
        vector<string> ldb2 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BI | AO),
            numToHex(RESET)
        };
        l[0b0001001] = ldb2;
        //ldb io
        vector<string> ldb3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(IOO | BI),
            numToHex(RESET)
        };
        l[0b0001010] = ldb3;
        //ldb [addr]
        vector<string> ldb4 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IRI | CE),
            numToHex(COE | MARI),
            numToHex(ME | THI | CE),
            numToHex(THO | IRO | MARI),
            numToHex(ME | BI),
            numToHex(RESET)
        };
        l[0b0001011] = ldb4;}
    //stb
    {//stb a
        vector<string> stb1 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BO | AI),
            numToHex(RESET)
        };
        l[0b0101000] = stb1;
        //stb io
        vector<string> stb2 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(BO | IOI),
            numToHex(RESET)
        };
        l[0b0101010] = stb2;
        //stb [addr]
        vector<string> stb3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IRI | CE),
            numToHex(COE | MARI),
            numToHex(ME | THI | CE),
            numToHex(THO | IRO | MARI),
            numToHex(ML | BO),
            numToHex(RESET)
        };
        l[0b0101011] = stb3;
        //sta c
        vector<string> stb4 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(CSI | BO),
                numToHex(RESET)
        };
        l[0b0101101] = stb4; }
    //ldc
    {//ldc (imm)
        vector<string> ldc1 = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | CSI | CE),
        numToHex(RESET) };
        l[0b0010111] = ldc1;
        //ldc a
        vector<string> ldc2 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(AO | CSI),
            numToHex(RESET)
        };
        l[0b0010000] = ldc2;
        //ldc b
        vector<string> ldc3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BO | CSI),
            numToHex(RESET)
        };
        l[0b0010001] = ldc3;
        //ldc io
        vector<string> ldc4 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(IOO | CSI),
            numToHex(RESET)
        };
        l[0b0010010] = ldc4;
        //ldc [addr]
        vector<string> ldc5 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IRI | CE),
            numToHex(COE | MARI),
            numToHex(ME | THI | CE),
            numToHex(THO | IRO | MARI),
            numToHex(ME | CSI),
            numToHex(RESET)
        };
        l[0b0010011] = ldc5;
        //ldc b, [haddr]
        vector<string> ldc6 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | THI | CE),
            numToHex(THO | BO | MARI),
            numToHex(ME | CSI),
            numToHex(RESET)
        };
        l[0b0010100] = ldc6; }
    //stcounter
    {
        //stcounterh [addr]
        vector<string> stcounterla= {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(AI | COE),
            numToHex(RESET)
        }; 
        l[0b0110000] = stcounterla;

        //stcounterh [addr]
        vector<string> stcounterha = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(AI | COH),
            numToHex(RESET)
        };
        l[0b0110001] = stcounterha;

        //stcounterh [addr]
        vector<string> stcounterlb= {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BI | COE),
            numToHex(RESET)
        }; 
        l[0b0110010] = stcounterlb;

        //stcounterh [addr]
        vector<string> stcounterhb = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BI | COH),
            numToHex(RESET)
        };
        l[0b0110011] = stcounterhb;
    }
    //ldio
    {//ldio (imm) imm, la, ha
        vector<string> ldio1 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOI | CE),
            numToHex(RESET) };
        l[0b0011111] = ldio1;
        //ldio a
        vector<string> ldio2 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(AO | IOI),
            numToHex(RESET)
        };
        l[0b0011000] = ldio2;
        //ldio b
        vector<string> ldio3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(BO | IOI),
            numToHex(RESET)
        };
        l[0b0011001] = ldio3;
}
    //stio
    {//stio a
        vector<string> stio1 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(IOO | AI),
            numToHex(RESET)
        };
        l[0b0111000] = stio1;
        //stio b
        vector<string> stio2 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(BI | IOO),
            numToHex(RESET)
        };
        l[0b0111001] = stio2;
        //stio c
        vector<string> stio5 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | IOARLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | IOARHI | CE),
                numToHex(CSI | IOO),
                numToHex(RESET)
        };
        l[0b0111101] = stio5; }

    //ALU:
    //add
    {//add (imm)
    vector<string> add1 = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | ALUI | CE),
        numToHex(ALUO | AI),
        numToHex(RESET)
    };
    l[0b1000111] = add1;
    //add a
    vector<string> add2 = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(AO | ALUI),
        numToHex(ALUO | AI),
        numToHex(RESET)
    };
    l[0b1000000] = add2;
    //add b
    vector<string> add3 = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(BO | ALUI),
        numToHex(ALUO | AI),
        numToHex(RESET)
    };
    l[0b1000001] = add3;
    //add io
    vector<string> add4 = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | IOARLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | IOARHI | CE),
        numToHex(IOO | ALUI),
        numToHex(ALUO | AI),
        numToHex(RESET)
    };
    l[0b1000010] = add4;
    //add [addr]
    vector<string> add5 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IRI | CE),
            numToHex(COE | MARI),
            numToHex(ME | THI | CE),
            numToHex(THO | IRO | MARI),
            numToHex(ME | ALUI),
            numToHex(ALUO| AI),
            numToHex(RESET)
    };
    l[0b1000011] = add5;
    vector<string> add6 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | THI | CE),
            numToHex(THO | BO | MARI),
            numToHex(ME | ALUI),
            numToHex(AI | ALUO),
            numToHex(RESET)
    };
    l[0b1000100] = add6;
    }
    //sub
    {//sub (imm)
        vector<string> sub1 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | ALUI | CE),
            numToHex(SUB | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1001111] = sub1;
        //sub a
        vector<string> sub2 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(AO | ALUI),
            numToHex(SUB | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1001000] = sub2;
        //sub b
        vector<string> sub3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BO | ALUI),
            numToHex(SUB | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1001001] = sub3;
        //sub io
        vector<string> sub4 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(IOO | ALUI),
            numToHex(SUB | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1001010] = sub4;
        //sub [addr]
        vector<string> sub5 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | IRI | CE),
                numToHex(COE | MARI),
                numToHex(ME | THI | CE),
                numToHex(THO | IRO | MARI),
                numToHex(ME | ALUI),
                numToHex(SUB | ALUO | AI),
                numToHex(RESET)
        };
        l[0b1001011] = sub5;
        //sub b, haddr
        vector<string> sub6 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | THI | CE),
                numToHex(THO | BO | MARI),
                numToHex(ME | ALUI),
                numToHex(SUB | AI | ALUO),
                numToHex(RESET)
        };
        l[0b1001100] = sub6;
    }
    //and
    {//and (imm)
        vector<string> and1 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | ALUI | CE),
            numToHex(AND | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1010111] = and1;
        //and a
        vector<string> and2 = anop;
        l[0b1010000] = and2;
        //and b
        vector<string> and3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BO | ALUI),
            numToHex(AND | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1010001] = and3;
        //and io
        vector<string> and4 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(IOO | ALUI),
            numToHex(AND | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1010010] = and4;
        //and [addr]
        vector<string> and5 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | IRI | CE),
                numToHex(COE | MARI),
                numToHex(ME | THI | CE),
                numToHex(THO | IRO | MARI),
                numToHex(ME | ALUI),
                numToHex(AND | ALUO | AI),
                numToHex(RESET)
        };
        l[0b1010011] = and5;
        //and b, haddr
        vector<string> and6 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | THI | CE),
                numToHex(THO | BO | MARI),
                numToHex(ME | ALUI),
                numToHex(AND | AI | ALUO),
                numToHex(RESET)
        };
        l[0b1010100] = and6;
    }
    //or
    {//or (imm)
        vector<string> or1 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | ALUI | CE),
            numToHex(OR | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1011111] = or1;
        //or a
        vector<string> or2 = anop;
        l[0b1011000] = or2;
        //or b
        vector<string> or3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BO | ALUI),
            numToHex(OR | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1011001] = or3;
        //or io
        vector<string> or4 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(IOO | ALUI),
            numToHex(OR | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1011010] = or4;
        //or [addr]
        vector<string> or5 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | IRI | CE),
                numToHex(COE | MARI),
                numToHex(ME | THI | CE),
                numToHex(THO | IRO | MARI),
                numToHex(ME | ALUI),
                numToHex(OR | ALUO | AI),
                numToHex(RESET)
        };
        l[0b1011011] = or5;
        //or b, haddr
        vector<string> or6 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | THI | CE),
                numToHex(THO | BO | MARI),
                numToHex(ME | ALUI),
                numToHex(OR | AI | ALUO),
                numToHex(RESET)
        };
        l[0b1011100] = or6;
    }
    //nor
    {//nor (imm)
        vector<string> nor1 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | ALUI | CE),
            numToHex(NOR | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1100111] = nor1;
        //nor a
        vector<string> nor2 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(AO | ALUI),
            numToHex(NOR | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1100000] = nor2;
        //nor b
        vector<string> nor3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BO | ALUI),
            numToHex(NOR | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1100001] = nor3;
        //nor io
        vector<string> nor4 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(IOO | ALUI),
            numToHex(SUB | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1100010] = nor4;
        //nor [addr]
        vector<string> nor5 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | IRI | CE),
                numToHex(COE | MARI),
                numToHex(ME | THI | CE),
                numToHex(THO | IRO | MARI),
                numToHex(ME | ALUI),
                numToHex(NOR | ALUO | AI),
                numToHex(RESET)
        };
        l[0b1100011] = nor5;
        //nor b, haddr
        vector<string> nor6 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | THI | CE),
                numToHex(THO | BO | MARI),
                numToHex(ME | ALUI),
                numToHex(NOR | AI | ALUO),
                numToHex(RESET)
        };
        l[0b1100100] = nor6;
    }
    //inv
    {//inv (imm)
        vector<string> inv1 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | ALUI | CE),
            numToHex(INV | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1101111] = inv1;
        //inv a
        vector<string> inv2 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(AO | ALUI),
            numToHex(INV | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1101000] = inv2;
        //inv b
        vector<string> inv3 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(BO | ALUI),
            numToHex(INV | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1101001] = inv3;
        //inv io
        vector<string> inv4 = {
            numToHex(COE | MARI),
            numToHex(ME | CLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARLI | CE),
            numToHex(COE | MARI),
            numToHex(ME | IOARHI | CE),
            numToHex(IOO | ALUI),
            numToHex(INV | ALUO | AI),
            numToHex(RESET)
        };
        l[0b1101010] = inv4;
        //inv [addr]
        vector<string> inv5 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | IRI | CE),
                numToHex(COE | MARI),
                numToHex(ME | THI | CE),
                numToHex(THO | IRO | MARI),
                numToHex(ME | ALUI),
                numToHex(INV | ALUO | AI),
                numToHex(RESET)
        };
        l[0b1101011] = inv5;
        //inv b, haddr
        vector<string> inv6 = {
                numToHex(COE | MARI),
                numToHex(ME | CLI | CE),
                numToHex(COE | MARI),
                numToHex(ME | THI | CE),
                numToHex(THO | BO | MARI),
                numToHex(ME | ALUI),
                numToHex(INV | AI | ALUO),
                numToHex(RESET)
        };
        l[0b1101100] = inv6;
    }
    //jmp
    vector<string> jmp = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | IRI | CE),
        numToHex(COE | MARI),
        numToHex(ME | CSetH),
        numToHex(IRO | CSetL),
        numToHex(RESET)
    };
    for (int i = 112; i < 120; i++) {
        l[i] = jmp;
    }
    vector<string> rintjmp = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | IRI | CE),
        numToHex(COE | MARI),
        numToHex(ME | CSetH),
        numToHex(IRO | CSetL | RINT),
        numToHex(RESET)
    };
    l[119] = rintjmp; //instruction that cannot be used by a programmer it frees the interrupt so anotherone can be recieved

{
    //sta [addr]
    vector<string> in5 = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | IRI | CE),
        numToHex(COE | MARI),
        numToHex(ME | THI | CE),
        numToHex(THO | IRI | MARI),
        numToHex(ML | AO),
        numToHex(RESET)
    };
    //sta b, [haddr]
    vector<string> in6 = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | THI | CE),
        numToHex(THO | BO | MARI),
        numToHex(AO | ML),
        numToHex(RESET)
    };
    //ldb (imm)
    vector<string> in7 = {
       numToHex(COE | MARI),
       numToHex(ME | CLI | CE),
       numToHex(COE | MARI),
       numToHex(ME | BI | CE),
       numToHex(RESET) };
    //ldb [addr]
    vector<string> in8 = {
         numToHex(COE | MARI),
         numToHex(ME | CLI | CE),
         numToHex(COE | MARI),
         numToHex(ME | IRI | CE),
         numToHex(COE | MARI),
         numToHex(ME | THI | CE),
         numToHex(THO | IRO | MARI),
         numToHex(ME | BI),
         numToHex(RESET)
    };
    //ldb a
    vector<string> in9 = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(AO | BI),
        numToHex(RESET)
    };
    //stb [addr]
    vector<string> ina = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | IRI | CE),
        numToHex(COE | MARI),
        numToHex(ME | THI | CE),
        numToHex(THO | IRI | MARI),
        numToHex(ML | BO),
        numToHex(RESET)
    };
    //add (imm)
    vector<string> inb = {
        numToHex(COE | MARI),
        numToHex(ME | CLI | CE),
        numToHex(COE | MARI),
        numToHex(ME | ALUI | CE),
        numToHex(ALUO | AI),
        numToHex(RESET)
    };
}    
    vector<string> li = connectInstructions(l);
    vector<string> lis;
    //copy list 4x
    for (int i = 0; i < 4; ++i) {
        lis.insert(lis.end(), li.begin(), li.end());
    }
    for (int i = jmp.size(); i < 16; i++){
        jmp.push_back("00000000");
    }
    for (int i = jnop.size(); i < 16; i++){
        jnop.push_back("00000000");
    }
    //jz  0b1 111 000
    vector<string> jz = jmp;
    for(int i = 0; i < 16; ++i){
        int j = 0b0111110000000 + i;
        lis[j] = jz[i];
        j = 0b1111110000000 + i;
        lis[j] = jz[i];
    }
    for(int i = 0; i < 16; ++i){
        int j = 0b0011110000000 + i;
        lis[j] = jnop[i];
        j = 0b1011110000000 + i;
        lis[j] = jnop[i];
    }
    //jnz 0b1 111 001
    vector<string> jnz = jmp;
    for(int i = 0; i < 16; ++i){
        int j = 0b0011110010000 + i;
        lis[j] = jz[i];
        j = 0b1011110010000 + i;
        lis[j] = jz[i];
    }
    for(int i = 0; i < 16; ++i){
        int j = 0b0111110010000 + i;
        lis[j] = jnop[i];
        j = 0b1111110010000 + i;
        lis[j] = jnop[i];
    }
    
    //jc  0b1 111 010
    vector<string> jc = jmp;
    for(int i = 0; i < 16; ++i){
        int j = 0b1011110100000 + i;
        lis[j] = jz[i];
        j = 0b1111110100000 + i;
        lis[j] = jz[i];
    }
    for(int i = 0; i < 16; ++i){
        int j = 0b0011110100000 + i;
        lis[j] = jnop[i];
        j = 0b0111110100000 + i;
        lis[j] = jnop[i];
    }
    //jnc 0b1 111 011
    vector<string> jnc = jmp;
    for(int i = 0; i < 16; ++i){
        int j = 0b0011110110000 + i;
        lis[j] = jz[i];
        j = 0b0111110110000 + i;
        lis[j] = jz[i];
    }
    for(int i = 0; i < 16; ++i){
        int j = 0b1011110110000 + i;
        lis[j] = jnop[i];
        j = 0b1111110110000 + i;
        lis[j] = jnop[i];
    }
    writeROM(lis);
    std::cout << "done";
}

