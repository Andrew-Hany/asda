/*
	This is just a skeleton. It DOES NOT implement all the requirements.
	It only recognizes the "ADD" and "SUB" instructions and prints
	"Unkown Instruction" for all other instructions!

	References:
	(1) The risc-v ISA Manual ver. 2.1 @ https://riscv.org/specifications/
	(2) https://github.com/michaeljclark/riscv-meta/blob/master/meta/opcodes
*/

/*

what i dont know 
1)how should we use  the address 
2)what are the usage of these  S_imm, B_imm, U_imm, J_imm
3)labels 
what i am not sure if i am right or not 
1)  asigning values to these S_imm, B_imm, U_imm, J_imm 
*/

#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <bitset>
#include <vector>

using namespace std;
int i = 1; //label numbers
int j = 1;
int l = 1;
int g = 0;
int c = 1;
unsigned char buffer[8 * 1024]; // Buffer to hold the machine code
vector<signed int> offset_B;
unsigned int offset_j = 213131231;
void emitError(char *s)
{
	cout << s;
	exit(0);
}

void instDecExec(unsigned int instWord, int pc)
{
	unsigned int rd, rs1, rs2, funct3, funct7, opcode;
	signed int I_imm, J_imm;
	unsigned int S_imm, B_imm, U_imm;
	unsigned int address = pc;

	///labels
	// if(offset_B.size()-1==0){
	for (int a = 0; a < offset_B.size(); a++)
		if (pc == offset_B[a])
		{
			cout << "label"
				 << c << ":";

			c++;
			break;
		}
	// }
	/////////i did habd fa45 please help w fahmony el logic bta3y ahahahahah

	if (pc == (offset_j))
	{ //offset_j = pc+J_imm
		// cout<<(offset_j / 4) * 4 + 12;
		cout << "return function"
			 << j - 1 << ":"
			 << "\n";
		offset_j = 213131231;
	}

	opcode = instWord & 0x0000007F;
	rd = (instWord >> 7) & 0x0000001F;
	funct3 = (instWord >> 12) & 0x00000007;
	rs1 = (instWord >> 15) & 0x0000001F;
	rs2 = (instWord >> 20) & 0x0000001F;

	funct7 = (instWord >> 25) & 0x0000007F;

	I_imm = (instWord >> 20) & 0x0000000FFF;
unsigned int pred=(instWord >> 20) & 0x0000000F;
unsigned int succ=(instWord >> 24) & 0x0000000F;
	unsigned int shamt = (instWord >> 20) & 0x00000001F;
	//////////if it is negative sign /////////////
	if ((I_imm >> 11) == 1)
	{
		int temp = 0xFFFFF;
		temp = temp << 12;
		I_imm = I_imm + temp;
	}

	S_imm = ((instWord >> 7) & 0x0000001F) |		 // 0000..01 1111
			(((instWord >> 25) & 0x00000007F)) << 5; // 1111..10 0000
	//////should we consider it as 13 bit or 12 bit ??
	B_imm = 0x0 |									//first least
			((instWord >> 8) & 0x0000000F) << 1 |	//4 bits after the first bit  //     0101 0
			((instWord >> 25) & 0x00000003F) << 5 | // 6 bits after the first five bits //101010 0101 0
			((instWord >> 7) & 0x000000001) << 11 |
			((instWord >> 31) & 0x000000001) << 12;

	J_imm = ((instWord >> 21) & 0x00000003FF) |
			((instWord >> 20) & 0x000001) << 10 |
			((instWord >> 12) & 0x0000000FF) << 11 |
			((instWord >> 31) & 0x00000001) << 19;
	//negative
	if ((J_imm >> 19) == 1)
	{
		signed int temp2 = 0xFFFFF; //  1111 1111 1111 20bit
		temp2 = temp2 << 20;
		J_imm = J_imm + temp2;
	}

	U_imm = (instWord >> 12) & 0x0000000FFFFF;

	//////////////////////////begining pof R instruction/////////////////////////////////////////////////////////////
	if (opcode == 0x33)
	{ // R Instructions

		switch (funct3)
		{
		case 0:
			if (funct7 == 32)
			{
				cout << "\t\tSUBx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				//regs[rd] = regs[rs1] - regs[rs2];
			}
			else
			{

				cout << "\t\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";

				//			regs[rd] = regs[rs1] + regs[rs2];
				break;
			}
		case 1:

			cout << "\t\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			break;
		case 2:

			cout << "\t\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			break;
		case 3:

			cout << "\t\tSLTU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			break;
		case 4:

			cout << "\t\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";

			break;
		case 5:
			if (funct7 == 32)
			{
				cout << "\t\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				//regs[rd] = regs[rs1] - regs[rs2];
			}
			else
			{
				cout << "\t\tSRL\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				//			regs[rd] = regs[rs1] + regs[rs2];
			}
			break;

		case 6:

			cout << "\t\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";

			break;
		default:
			cout << "\t\tUnkown R Instruction \n";
		}
	}

	/////////////////////////////////end of  R instructions /////////////////////////////////////////////////

	//////////////////////////begining pof I -load- instruction/////////////////////////////////////////////////////////////
	else if (opcode == 0x3)
	{ //0110011	// I Instructions
		switch (funct3)
		{
		case 0:
			cout << "\t\tLB\tx" << rd << ", " << I_imm << "(x" << rs1 << ") "
				 << "\n";
			break;
		case 1:
			cout << "\t\tLH\tx" << rd << ", " << I_imm << "(x" << rs1 << ") "
				 << "\n";
			break;
		case 2:
			cout << "\t\tLW\tx" << rd << ", " << I_imm << "(x" << rs1 << ") "
				 << "\n";
			break;
		case 4:
			cout << "\t\tLBU\tx" << rd << ", " << I_imm << "(x" << rs1 << ") "
				 << "\n";
			break;
		case 5:
			cout << "\t\tLHU\tx" << rd << ", " << I_imm << "(x" << rs1 << ") "
				 << "\n";
			break;

		default:
			cout << "\tUnkown I-load- Instruction \n"
				 << funct3;
		}
	}
	///////////////////////////////////////////////////////////////////////////////

	//////////////////////////begining pof I -A/L- instruction/////////////////////////////////////////////////////////////
	else if (opcode == 0x13)
	{ // I Instructions

		switch (funct3)
		{
		case 0:
			cout << "\t\tADDI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			break;

		case 1:
			cout << "\t\tSLLi\tx" << rd << ", x" << rs1 << ", " << shamt << "\n";
			break;

		case 2:
			cout << "\t\tSLTI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			break;
		case 3:
			cout << "\t\tSLTIU\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			break;
		case 4:
			cout << "\t\tXORI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			break;
		case 5:
			if (funct7 == 32)
				cout << "\t\tSRAI\tx" << rd << ", x" << rs1 << ", " << shamt << "\n";
			else
				cout << "\t\tSRLI\tx" << rd << ", x" << rs1 << ", " << shamt << "\n";
			break;
		case 6:
			cout << "\t\tORI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			break;
		case 7:
			cout << "\t\tANDI\tx" << rd << ", x" << rs1 << ", " << I_imm << "\n";
			break;
		default:
			cout << "\t\tUnkown I-A/L- Instruction \n"
				 << funct3;
		}
	}
	///////////////////////////////////////////////////////////////////////////////

	//////////////////////////begining of S instruction/////////////////////////////////////////////////////////////
	else if (opcode == 0x23)
	{
		switch (funct3)
		{
		case 0:
			cout << "\t\tSB\tx" << rs2 << ", " << S_imm << "(x" << rs1 << ") "
				 << "\n";
			break;
		case 1:
			cout << "\t\tSH\tx" << rs2 << ", " << S_imm << "(x" << rs1 << ") "
				 << "\n";
			break;
		case 2:
			cout << "\t\tSW\tx" << rs2 << ", " << S_imm << "(x" << rs1 << ") "
				 << "\n";
			break;
		default:
			cout << "\t\tUnkown S Instruction \n"
				 << funct3;
		}
	}
	///////////////////////////////////////////////////////////////////////////////

	//////////////////////////begining of SB instruction/////////////////////////////////////////////////////////////
	////offset ////////
	////still needs some work
	else if (opcode == 0x63)
	{
		int temp;
		switch (funct3)
		{
		case 0:
			if (offset_B.size() == 0)
				offset_B.push_back(address + B_imm);
			temp = address + B_imm;

			cout << "\t\tBEQ\tx" << rs1 << ", "
				 << "x" << rs2 << ", ";
			for (int a = 0; a < offset_B.size(); a++)
				if (temp == offset_B[a] && offset_B.size() > 1)
				{
					cout << " label" << a + 1
						 << "\n";
					break;
				}
				else
				{
					cout << " label" << i
						 << "\n";
					offset_B.push_back(address + B_imm);
					i++;

					break;
				}
			break;

		case 1:
			if (offset_B.size() == 0)
				offset_B.push_back(address + B_imm);
			temp = address + B_imm;

			cout << "\t\tBNE\tx" << rs1 << ", "
				 << "x" << rs2 << ", ";
			for (int a = 0; a < offset_B.size(); a++)
				if (temp == offset_B[a] && offset_B.size() > 1)
				{
					cout << " label" << a + 1
						 << "\n";
					break;
				}
				else
				{
					cout << " label" << i
						 << "\n";
					offset_B.push_back(address + B_imm);
					i++;

					break;
				}
			break;
			break;
		case 4:
			if (offset_B.size() == 0)
				offset_B.push_back(address + B_imm);
			temp = address + B_imm;

			cout << "\t\tBLT\tx" << rs1 << ", "
				 << "x" << rs2 << ", ";
			for (int a = 0; a < offset_B.size(); a++)
				if (temp == offset_B[a] && offset_B.size() > 1)
				{
					cout << " label" << a + 1
						 << "\n";
					break;
				}
				else
				{
					cout << " label" << i
						 << "\n";
					offset_B.push_back(address + B_imm);
					i++;

					break;
				}
			break;
			break;
		case 5:
			if (offset_B.size() == 0)
				offset_B.push_back(address + B_imm);
			temp = address + B_imm;

			cout << "\t\tBGE\tx" << rs1 << ", "
				 << "x" << rs2 << ", ";
			for (int a = 0; a < offset_B.size(); a++)
				if (temp == offset_B[a] && offset_B.size() > 1)
				{
					cout << " label" << a + 1
						 << "\n";
					break;
				}
				else
				{
					cout << " label" << i
						 << "\n";
					offset_B.push_back(address + B_imm);
					i++;

					break;
				}
			break;
		case 6:
			if (offset_B.size() == 0)
				offset_B.push_back(address + B_imm);
			temp = address + B_imm;

			cout << "\t\tBLTU\tx" << rs1 << ", "
				 << "x" << rs2 << ", ";
			for (int a = 0; a < offset_B.size(); a++)
				if (temp == offset_B[a] && offset_B.size() > 1)
				{
					cout << " label" << a + 1
						 << "\n";
					break;
				}
				else
				{
					cout << " label" << i
						 << "\n";
					offset_B.push_back(address + B_imm);
					i++;

					break;
				}
			break;
		case 7:
			if (offset_B.size() == 0)
				offset_B.push_back(address + B_imm);
			temp = address + B_imm;

			cout << "\t\tBGEU\tx" << rs1 << ", "
				 << "x" << rs2 << ", ";
			for (int a = 0; a < offset_B.size(); a++)
				if (temp == offset_B[a] && offset_B.size() > 1)
				{
					cout << " label" << a + 1
						 << "\n";
					break;
				}
				else
				{
					cout << " label" << i
						 << "\n";
					offset_B.push_back(address + B_imm);
					i++;

					break;
				}
			break;

			break;
		default:
			cout << "\tUnkown SB Instruction \n"
				 << funct3;
		}
	}
	///////////////////////////////////////////////////////////////////////////////

	///////////ecall function /////////////////////
	else if (opcode == 0x73){
		switch(funct3){
			case 0:
			if(funct7==1)
			cout << "\t\tebreak\n";
			else 
			cout << "\t\tecall\n";
			break;
			case 1:
			cout << "\t\tCSRRW\tx" << rd << ", " << I_imm  << ", x" << rs1 << "\n";
			break;
			case 2:
			cout << "\t\tCSRRS\tx" << rd << ", " << I_imm  << ", x" << rs1 << "\n";
			break;
			case 3:
			cout << "\t\tCSRRC\tx" << rd << ", " << I_imm  << ", x" << rs1 << "\n";
			break;
			case 6:
			cout << "\t\tCSRRSI\tx" << rd << ", " << I_imm  << ", x" << rs1 << "\n";
			break;
			case 7:
			cout << "\t\tCSRRCI\tx" << rd << ", " << I_imm  << ", x" << rs1 << "\n";
			break;
			default:cout << "\t\tUnkown  Instruction \n";
		}


		
	}
	////////////////////////////////////////
else if (opcode == 0xF){
	switch(funct3){
case 0:
			cout << "\t\tFENCE\t" << pred<<","<<succ<< "\n";
			break;
			case 1:
			cout << "\t\tFENCE.I\t" <<  "\n";
			break;
			default:cout << "\t\tUnkown  Instruction \n";
	}
}
	///////////JALR /////////////////////
	else if (opcode == 0x67)
	{
		cout << "\t\tJALR\tx" << rd << ", "
			 << "x" << rs1 << ", " << I_imm
			 << "\n";
	}
	////////////////////////////////////////
	///////////JAL /////////////////////
	else if (opcode == 0x6F)
	{

		// cout << pc << " " << J_imm;
		// offset_j = (pc + J_imm); //66\2=33
		// int as = (pc - offset_j) / 2;
		// as = as + i - 1;
		offset_j = (pc + J_imm + J_imm); 
		// cout<<" "<<i;
		int as = (((pc - offset_j + 4)) / 4);
		cout << "\t\tJAL\tx" << rd
			 << ", ";
		if (J_imm < 0)
		{

			cout << "loop"
				 << l
				 << "\n";

			if (g % 2 == 0)
			{
				cout << "\t";
				g++;
			}

			l++;

			cout << "\033[" << as << "A"
				 << "loop" << l - 1 << ":"
				 << "\n";

			for (int i = 0; i < as - 1; i++)
				cout << "\n";
			offset_j = 12332312;
		}
		else
		{
			cout << "return function"
				 << j
				 << "\n";
			j++;
		}
	}
	////////////////////////////////////////
	//////////////////////////begining of U instruction/////////////////////////////////////////////////////////////
	/////////// AUIPC /////////////////////
	else if (opcode == 0x17)
	{
		cout << "\t\tAUIPC\tx" << rd
			 << "," << U_imm
			 << "\n";
	}
	////////////////////////////////////////
	/////////// LUI /////////////////////
	else if (opcode == 0x37)
	{
		cout << "\t\tLUI\tx" << rd
			 << "," << U_imm
			 << "\n";
	}
	////////////////////////////////////////
	else
	{
		cout << "\t\tUnkown Instruction \n";
		cout << hex << opcode << endl;
	}
}
int main(int argc, char *argv[])
{

	unsigned int instWord = 0;
	ifstream inFile;
	ofstream outFile;
	unsigned int pc = 0;

	if(argc<1) emitError("use: disasm <machine_code_file_name>\n");

	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if (inFile.is_open())
	{
		int fsize = inFile.tellg(); //size

		inFile.seekg(0, inFile.beg); //begin of the file
		if(!inFile.read((char *)buffer, fsize)) emitError("Cannot read from input file\n");
		int i = 0;
		while (true)
		{
			if (pc == 0)
				cout << "main:\n";
			instWord = (unsigned char)buffer[pc] |
					   (((unsigned char)buffer[pc + 1]) << 8) |
					   (((unsigned char)buffer[pc + 2]) << 16) |
					   (((unsigned char)buffer[pc + 3]) << 24);

			// remove the following line once you have a complete disassembler
			// if(pc==32) break;			// stop when PC reached address 32
			i++;
			if (fsize / 4 + 1 == i)
				break;

			instDecExec(instWord, pc);
			pc += 4;
		}
	}
	else
		 emitError("Cannot access input file\n");
}
