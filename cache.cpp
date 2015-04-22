#include <iostream>
using namespace std;
#include <string>

int main()
{
	int address, op, tag, index, offset, tagA[64], dirty[64], valid0[64]= {0}, valid1[64] = {0};
	string data; 
	string p1;
	string p2;
	string data0[64] = "", data1[64] = "";
	while (cin >> hex >> address >> op)
	{
		cin >> data;
		
		tag = (address & 65408) >> 7;   // First 9 bits
		index = (address & 126) >> 1;   // Bits 10-15
		offset = address & 1;           // Bit 16
		if (offset)
			offset = 0;
		else
			offset = 1;

		if(offset && op == 255)        //Offset 1, write operation
		{
			data1[index] = data.substr(0,7);
			dirty[index] = 1;
			tagA[index] = tag;
			valid1[index] = 1;
		}
		else if (!offset && op == 255) //Offset 0, write operation
		{
			data0[index] = data.substr(8,15);
			dirty[index] = 1;
			tagA[index] = tag;
			valid0[index] = 1;
		}	
		else                          //Read Operation
		{
			if (tag != tagA[index])     //Not in cache / miss
			{
				cout << data.substr(0+(8*offset),8+(8*offset)) << "\t" << 0 << "\t" << 0 <<endl;
				dirty[index] = 1;         //Add to cacce
				tagA[index] = tag;
				if(offset)
				{
					data1[index] = data.substr(0,8);
					valid1[index] = 0;
				}
				else
				{
					data0[index] = data.substr(9,16);
					valid0[index] = 0;
				}

			}	
			else if (offset && tag == tagA[index]) //found in cache, hit
			{	
				if(valid1[index])
					cout <<data1[index] << "\t" << 1 << "\t"  << 1 << endl;
				else
					cout <<data.substr(8,15) << "\t" << 1 << "\t"  << 0 << endl;
			}
			else if (!offset && tag == tagA[index])	
			{
				if(valid0[index])
					cout <<data0[index] << "\t" << 1 << "\t"  << 1 << endl;
				else
					cout <<data.substr(0,7) << "\t" << 1 << "\t"  << 0 << endl;
			}

			
		}
	} //while
} //main()
