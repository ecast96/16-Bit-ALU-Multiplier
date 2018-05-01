/*
 Erick Castro
 CSCI 113 - Simulation Program
 
 Algorithm
 1. if MQ[0] == 1
 AC = AC + MD
 2. AC/MQ >> 1 - Shift right 1 bit

 How to Use:
 To enter values for MD and MQ, there are two vectors in main.
 Slot 0 in vector is 0th spot in 16 bit number.
 Enter number from left to right.
*/

#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
using namespace std;


//AND Gate
bool AND(bool x, bool y){
    if(x == 1 && y ==1)
         return 1;
    else
         return 0;
}

//OR Gate
bool OR(bool x, bool y){
    if(x == 1 || y == 1)
         return 1;
    else
        return 0;
}   

//XOR Gate
bool XOR(bool x, bool y){
	if(x == 1 && y != 1)
		return 1;
	else if(x != 1 && y == 1)
		return 1;
	else 
		return 0;
}

//4x1 MUX
bool MUX_4x1(bool x1, bool x2, bool x3, bool x4, int op){
    if (op == 0)
        return x1;
	else if(op == 1)
		return x2;
	else if(op == 2)
		return x3;
	else
		return x4;
    
}
// 1-bit Full Adder
bool full_adder_1bit(bool a, bool b, bool c_in, bool &c_out){
	c_out = OR(AND(a,b), AND(c_in,XOR(a,b)));
	return XOR(XOR(a,b), c_in);
}

/*
 1-bit ALU
 
 3 Operations:
    AND: OP = 0
    OR:  OP = 1
    ADD: OP = 2
*/
bool ALU_1bit(bool a, bool b, bool c_in, int op, bool  &c_out){

	bool t1 = AND(a,b);
	bool t2 = OR(a,b);
	bool t3 = full_adder_1bit(a, b, c_in, c_out);
	bool t4 = 0;

	return MUX_4x1(t1, t2, t3, t4, op);
}

/*
 1-bit ALU with Overflow check
 Same as 1-bit ALU with 3 operations
 Additional Overflow check:
    Operand sign != Result Sign -> OF
*/
bool ALU_1bit_OF(bool a, bool b, bool c_in, int op, bool &of){
    
	bool c_out;
	bool t1 = AND(a,b);
	bool t2 = OR(a,b);
	int t3 = full_adder_1bit(a, b, c_in, c_out);
	bool t4 = 0;

	of = XOR(c_in, c_out);

	return MUX_4x1(t1, t2, t3, t4, op);
}

/*
16Bit Alu
Does any operation for two 16bit numbers.
*/
void ALU_16bit(vector<bool> a, vector<bool> b, int op, vector<bool> &result, bool &of){
	bool t[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	for (int i = 0; i < 16; i++) {
		if (i == 0)
			result[i] = ALU_1bit(a[i], b[i], 0, op, t[0]);
		else if (i == 15)
			result[i] = ALU_1bit(a[i], b[i], t[i - 1], op, of);
		else
			result[i] = ALU_1bit(a[i], b[i], t[i - 1], op, t[i]);
	}
}

/*
Cycle Counter 

Counts down by 1 each cycle using ADD operation from 1 bit alu.
Subtraction is done by using 1's complement of 1 and a CIN of 1.
*/
vector<bool> cycleCounter(vector<bool> &count)
{	
	bool t[] = { 0, 0, 0, 0, 0 };
	vector<bool> result(5);

	for (int i = 0; i < 5; i++) {
		if (i == 0)
			result[i] = ALU_1bit(count[i], 0, 1, 2, t[0]);
		else
			result[i] = ALU_1bit(count[i], 1, t[i-1], 2, t[i]);
	}

	return result;
}

// Prints values of vectors
void printVectors(vector<bool> MD, vector<bool> MQ, vector<bool> AC, vector<bool> count) {
	for (long i = count.size() - 1; i >= 0; i--)
		cout << count[i];
	cout << "       ";
	for (long i = MD.size() - 1; i >= 0; i--)
		cout << MD[i];
	cout << " ";
	for (long i = AC.size() - 1; i >= 0; i--)
		cout << AC[i];
	cout << " ";
	for (long i = MQ.size() - 1; i >= 0; i--)
		cout << MQ[i];
}

/*
 16-Bit Multiplier
 
 Uses 16-Bit ALU to do 16-bit multiplication.
 Inputs:
    MD
    MQ
 Outputs:
    PR - Product
*/
void Mult_16bit(vector<bool> MD, vector<bool> MQ, vector<bool> &PR){
    vector<bool> AC = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	vector<bool> count = { 0,0,0,0,1 };

	//Prints out values before any steps are done
	cout << "Counter     " << "MD               " << "AC               " << "MQ" << endl;
	printVectors(MD, MQ, AC, count);
	cout << endl << endl;
    
    bool of;
    
    //Loop that does main operation, 16 times, or size of MD, which should be 16.
    for(int i = 0; i < MD.size(); i++){
        if(MQ[0] == 1)
            ALU_16bit(AC, MD, 2, AC, of);         //OP set to 2 for ADD operation.

		if(i > 0)
			count = cycleCounter(count);

        //Prints out current values after step 1.
		printVectors(MD, MQ, AC, count);
        cout << " --Step 1" << endl;

        //Shifts vector to the right by 1 bit
		MQ.insert(MQ.end(), AC[0]);
		MQ.erase(MQ.begin());
        //Shifts vector to the right by 1 bit
        AC.insert(AC.end(), 0);
        AC.erase(AC.begin());
	
		//Prints out current values after step 2.
		printVectors(MD, MQ, AC, count);
		cout <<  " --Step 2" << endl << endl;

    }

    //Concatenates MQ and AC into one vector, PR, the product vector.
	PR.insert(PR.begin(), MQ.begin(), MQ.end());
	PR.insert(PR.end(), AC.begin(), AC.end());
}



int main()
{
    vector<bool> MD = {1,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0};	//MD
    vector<bool> MQ = {0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1};	//MQ
    vector<bool> PR;						//Product
    
    Mult_16bit(MD, MQ, PR);					//Calls the main 16 bit Multiplier to do operation on MD and MQ, resulting in PR

    cout << "Product: ";
    for(long i = PR.size()-1; i >= 0; i--)			//Prints out Product
        cout << PR[i];
    cout << endl;


	system("pause");
    return 0;
}
