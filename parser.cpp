#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;
char load[] = "LD";
char loadi[] = "LDi";
char store[] = "SD";
char storei[] = "SDi";
char add[] = "ADD";
char addi[] = "ADDi";
char sub[] = "SUB";
char subi[] = "SUBi";	
char mul[] = "MUL";
char muli[] = "MULi";
char div[] = "DIV";
char divi[] = "DIVi";
char jmp[] = "JMP";
char jz[] = "JZ";
char jnz[] = "JNZ";
char jgz[] = "JGZ";
char jgez[] = "JGEZ";
char jlz[] = "JLZ";
char jlez[] = "JLEZ";

//Remove spaces before opcode
void removeSpaces(char input[]){
	char *temp = input;
	int index = 0;
	int i = 0;
	while(isspace(input[index]) && input[index] != '\0')
		index ++;							//counts number of whitespace characters at index
	while(input[index] != '\0'){
		temp[i] = input[index];
		i++;
		index++;				//updates the index 
	}							//updates the temp array without whitespaces
	temp[i] = '\0';				//ends the array
	input = temp;				//sets input to phrase without whitespace
}

//Remove comments
void removeComments(char input[]){
	int index = 0;
	bool comment = false;
	
	if(input[index] == '#')
		input[index] = '\0';
	else{
		while(input[index] != '\0' && !comment){
			if(input[index] == '#'){
				comment = true;
			}
			index ++;
		}
		if(comment)
			input[index-1] = '\0';
	}
}

//Check for blank line after removeSpaces and removeComments
int checkBlank(char input[]){
	if(input[0] == '\0')
		return 1;
	else
		return 0;
	return 1;	
}

//Check commas
int checkComma(char operand[]){
	int index = 0;
	while(operand[index] != '\0' && operand[index] != ' '){
		index++;
	}
	if(operand[index-1] == ','){
		return 1;
	}
	return 0;
}

//Count commas
int countCommas(char input[]){
	int index = 0;
	int count = 0;
	while(input[index] != '\0'){
		if(input[index] == ','){
			count++;
		}
		index++;
	}
	return count;
}

void removeComma(char op[]){
	char *temp = op;
	int index = 0;
	bool comma = false;
	while(op[index] != '\0' && !comma){
		if(op[index] == ','){
			comma = true;
		}
		index ++;
	}
	if(comma)
		temp[index-1] = '\0';
	op = temp;
}

//Find length of input 
int strLength(char input[]){
	int count = 0;
	while(input[count] != '\0'){
		count ++;
	}
	return count;				//returns length
}

//Compare two text str and check if they are equal
int strCmp(char str1[], char str2[]){
	int i = 0;
	while(str1[i]!='\0' || str2[i]!='\0'){
		if(str1[i] == str2[i] && str1[i] != '\0')	//checks next index if same character
			i ++;
		else if(str1[i] == '\0' && str2[i] != '\0')	//returns 1 if str1 ends first (and same char b4)
			return 1;
		else if(str1[i] != '\0' && str2[i] == '\0')	//returns -1 if str2 ends first 
			return -1;
		else if(str1[i] < str2[i])					//returns 1 if str1 char is smaller
			return 1;
		else if(str1[i] > str2[i])					//returns -1 if str1 char is larger
			return -1;
		else										//returns 0 if identical str
			return 0;
	}
}

//Get next char array
char* getNext(char input[]){
	char *array = new char[20];
	int index = 0;
	int i = 0;
	
	while(!isspace(input[index]) && input[index] != '\0'){
		array[index] = input[index];
		index ++;
	}
	array[index] = '\0';

	//truncate the opcode part of the line
	char *temp = input;
	while(input[index] != '\0'){
		temp[i] = input[index];
		i++;
		index++;
	}
	temp[i] = '\0';
	input = temp;
	removeSpaces(input);
	return array;
}

//Sets the char array
char* setNext(char array[]){
	int index = 0;
	
	while(array[index] != '\0')
		index++;
	
	char *next = new char[index+1];
	for(int i=0; i<index; i++){
		next[i] = array[i];
	}
	next[index] = '\0';
	return next;
}


//Get directive
int charToNum(char dir[]){
	char *curr = dir;
	int num = 0;
	int index = 0;
	
	while(curr[index] != '\0'){
		if(curr[index] >= '0' && curr[index] <= '9'){
			num *= 10;
			num += (dir[index] - 48);
		}
		else{
			return index*(-1);
		}
		index ++;
	}
	return num;
}

//Get next operand
char* getNextOperand(char op[]){
	char *array = new char[20];
	int index = 0;
	int i = 0;
	
	while(op[index] != ',' && op[index] != '\0'){
		array[index] = op[index];
		index ++;
	}
	array[index] = '\0';

	//truncate the opcode part of the line
	char *temp = op;
	while(op[index+1] != '\0'){
		temp[i] = op[index+1];
		i++;
		index++;
	}
	temp[i] = '\0';
	op = temp;
	removeSpaces(op);
	return array;
}

bool checkRegister(char *op){
	bool valid = false;
	int index = 0;
	if(op[0] == 'R' && (op[1] >= '1' && op[1] <= '9')){
		if(op[2] == '\0')
			valid = true;
		else if( op[3] == '\0' && op[2] >= '0' && op[2] <= '9'){
			valid = true;
		}
	}
	return valid;
}	
					
void getOperands(char input[], int type, int lineCount, int directiveCount){
	int commas = countCommas(input);
	char *op1, *op2, *op3;
	bool valid = false;
	
	switch(type){
		case 1:						//LD
			op1 = getNextOperand(input);
			//cout << op1 << endl;
			removeComma(op1);
			op2 = getNext(input);
			//cout << op2 << endl;
			
			if(checkRegister(op1)){		//LD Ri, Rj
				/*if(charToNum(op1) <= 0)
				cerr << "Error on line " << lineCount << ": Expected Number for first OPERAND; saw \""<< op1 << "\" " << endl;
				*/if(!checkRegister(op2))
					cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for second OPERAND; saw \""<< op2 << "\" " << endl;
				if(input[0] != '\0')
					cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
				if(commas == 1 && checkRegister(op1) && checkRegister(op2) && input[0] == '\0')
					valid = true;
			}
			else{						//LD <nnnn>, Ri
				if(charToNum(op1) <= 0)
					cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Number for first OPERAND; saw \""<< op1 << "\" " << endl;
				if(!checkRegister(op2))
					cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for second OPERAND; saw \""<< op2 << "\" " << endl;
				if(input[0] != '\0')
					cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
				if(commas == 1 && charToNum(op1) > 0 && checkRegister(op2) && input[0] == '\0')
					valid = true;
			}
		break;
		case 3:						//SD
			op1 = getNextOperand(input);
			//cout << op1 << endl;
			removeComma(op1);
			op2 = getNext(input);
			//cout << op2 << endl;
			
			if(checkRegister(op2)){		//SD Ri, Rj
				/*if(charToNum(op1) <= 0)
				cerr << "Error on line " << lineCount << ": Expected Number for first OPERAND; saw \""<< op1 << "\" " << endl;
				*/if(!checkRegister(op1))
					cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for first OPERAND; saw \""<< op1 << "\" " << endl;
				if(input[0] != '\0')
					cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
				if(commas == 1 && checkRegister(op1) && checkRegister(op2) && input[0] == '\0')
					valid = true;
			}
			else{						//SD Ri, <nnnn>
				if(!checkRegister(op1))
					cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for first OPERAND; saw \""<< op1 << "\" " << endl;
				if(charToNum(op2) <= 0)
					cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Number for second OPERAND; saw \""<< op2 << "\" " << endl;
				if(input[0] != '\0')
					cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
				if(commas == 1 && checkRegister(op1) && charToNum(op2) > 0 && input[0] == '\0')
					valid = true;
			}
		break;
		case 4:						//SDi
			op1 = getNextOperand(input);
			//cout << op1 << endl;
			removeComma(op1);
			op2 = getNext(input);
			//cout << op2 << endl;
			
			if(charToNum(op2) > 0){		//SDi mmmm, <nnnn>
				if(charToNum(op1) <= 0)
					cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Number for first OPERAND; saw \""<< op1 << "\" " << endl;
				/*if(charToNum(op2) <= 0)
					cerr << "Error on line " << lineCount << ": Expected Number for second OPERAND; saw \""<< op2 << "\" " << endl;
				*/if(input[0] != '\0')
					cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
				if(commas == 1 && charToNum(op1) > 0 && charToNum(op2) > 0 && input[0] == '\0')
					valid = true;
			}
			else{						//SDi mmmm, Ri
				if(charToNum(op1) <= 0)
					cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Number for first OPERAND; saw \""<< op1 << "\" " << endl;
				if(!checkRegister(op2))
					cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for second OPERAND; saw \""<< op2 << "\" " << endl;
				if(input[0] != '\0')
					cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
				if(commas == 1 && charToNum(op1) > 0 && checkRegister(op2) && input[0] == '\0')
					valid = true;
			}
		break;
		case 2:							//num, Ri
			op1 = getNextOperand(input);
			removeComma(op1);
			//cout << op1 << endl;
			op2 = getNext(input);
			//cout << op2 << endl;
			
			/*if(commas != 1)
				cerr << "Error on line " << lineCount << ": Invalid number of commas" << endl;*/
			if(charToNum(op1) <= 0)
				cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Number for first OPERAND; saw \""<< op1 << "\" " << endl;
			if(!checkRegister(op2))
				cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for second OPERAND; saw \""<< op2 << "\" " << endl;
			if(input[0] != '\0')
				cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
			if(commas == 1 && charToNum(op1) > 0 && checkRegister(op2) && input[0] == '\0')
				valid = true;
			//if check op1 and check op2 and comma and no extra then valid = true;
			break;
		case 5:
		case 7:
		case 9:
		case 11:						//Ri, Rj, Rk
			op1 = getNextOperand(input);
			removeComma(op1);
			//cout << op1 << endl;
			op2 = getNext(input);
			removeComma(op2);
			//cout << op2 << endl;
			op3 = getNext(input);
			//cout << op3 << endl;
			
			if(!checkRegister(op1))
				cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for first OPERAND; saw \""<< op1 << "\" " << endl;
			if(!checkRegister(op2))
				cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for second OPERAND; saw \""<< op2 << "\" " << endl;
			if(!checkRegister(op3))
				cerr << "Error on line " << lineCount << ": Expected Register for third OPERAND; saw \""<< op3 << "\" " << endl;
			if(input[0] != '\0')
				cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
			if(commas == 2 && checkRegister(op1) && checkRegister(op2) && checkRegister(op3) && input[0] == '\0')
				valid = true;
		break;
		case 6:
		case 8:
		case 10:	
		case 12:						//Ri, nnnn, Rj
			op1 = getNextOperand(input);
			removeComma(op1);
			//cout << op1 << endl;
			op2 = getNext(input);
			removeComma(op2);
			//cout << op2 << endl;
			op3 = getNext(input);
			//cout << op3 << endl;
			
			if(!checkRegister(op1))
				cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for first OPERAND; saw \""<< op1 << "\" " << endl;
			if(charToNum(op2) <= 0)
				cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Number for second OPERAND; saw \""<< op2 << "\" " << endl;
			if(!checkRegister(op3))
				cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for third OPERAND; saw \""<< op3 << "\" " << endl;
			if(input[0] != '\0')
				cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
			if(commas == 2 && charToNum(op1) > 0 && charToNum(op2) > 0 && charToNum(op3) > 0 && input[0] == '\0')
				valid = true;
		break;
		case 13:						//J <nnnn>
			op1 = getNextOperand(input);
			//cout << op1 << endl;
			if(charToNum(op1) <= 0)
				cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Number for first OPERAND; saw \""<< op1 << "\" " << endl;
			if(input[0] != '\0')
				cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
			if(commas = 0 && charToNum(op1) > 0 && input[0] == '\0')
				valid = true;
		break;
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:						//J Ri, <nnnn>
			op1 = getNextOperand(input);
			removeComma(op1);
			//cout << op1 << endl;
			op2 = getNext(input);
			//cout << op2 << endl;
			
			if(!checkRegister(op1))
				cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Register for first OPERAND; saw \""<< op1 << "\" " << endl;
			if(charToNum(op2) <= 0)
				cerr << "Error on line " << lineCount << ": Invalid missing extra input. Expected Number for second OPERAND; saw \""<< op2 << "\" " << endl;
			if(input[0] != '\0')
				cerr << "Error on line " << lineCount << ": Extra data after instruction" << endl;
			if(commas == 1 && checkRegister(op1) && charToNum(op2) > 0 && input[0] == '\0')
				valid = true;
		break;
		default:
			cerr << "Error on line " << lineCount << endl;
	}			
						/*char load[] = "LD"; 	1
						char loadi[] = "LDi";	2
						char store[] = "SD";	3
						char storei[] = "SDi";	4
						char add[] = "ADD";		5
						char addi[] = "ADDi";	6
						char sub[] = "SUB";		7			
						char subi[] = "SUBi";	8
						char mul[] = "MUL";		9
						char muli[] = "MULi";	10
						char div[] = "DIV";		11
						char divi[] = "DIVi";	12
						char jmp[] = "JMP";		13
						char jz[] = "JZ";		14
						char jnz[] = "JNZ" 		15
						char jgz[] = "JGZ";		16
						char jgez[] = "JGEZ";	17
						char jlz[] = "JLZ";		18
						char jlez[] = "JLEZ";	19*/
						
	//cout << directiveCount-1 << endl;
	//return valid;
}

//Check label
int checkIsLabel(char temp[]){
	int index = 0;
	while(temp[index] != '\0'){
		index ++;
	}
	if(temp[index-1] == ':'){
		return 1;
	}
	return 0;
}

//Check valid label
int checkValidLabel(char label[]){
	int index = 0;
	bool valid = true;
	while(label[index] != '\0' && valid){
		if(isspace(label[index])){
			valid = false;
		}
		index ++;
	}
	if(!valid)
		return 0;
	return 1;
}


//Check opcode
int checkOpcode(char opcode[]){
	if(strCmp(opcode, load) == 0)
		return 1;
	else if(strCmp(opcode, loadi) == 0)
		return 2;
	else if(strCmp(opcode, store) == 0)
		return 3;
	else if(strCmp(opcode, storei) == 0)
		return 4;
	else if(strCmp(opcode, add) == 0)
		return 5;
	else if(strCmp(opcode, addi) == 0)
		return 6;
	else if(strCmp(opcode, sub) == 0)
		return 7;
	else if(strCmp(opcode, subi) == 0)
		return 8;
	else if(strCmp(opcode, mul) == 0)
		return 9;
	else if(strCmp(opcode, muli) == 0)
		return 10;
	else if(strCmp(opcode, div) == 0)
		return 11;
	else if(strCmp(opcode, divi) == 0)
		return 12;
	else if(strCmp(opcode, jmp) == 0)
		return 13;
	else if(strCmp(opcode, jz) == 0)
		return 14;
	else if(strCmp(opcode, jnz) == 0)
		return 15;
	else if(strCmp(opcode, jgz) == 0)
		return 16;
	else if(strCmp(opcode, jgez) == 0)
		return 17;
	else if(strCmp(opcode, jlz) == 0)
		return 18;
	else if(strCmp(opcode, jlez) == 0)
		return 19;
	else{
		return '0';
	}
}



int main(int argc, char *argv[]){
	char input[100];
	char *array;
	char *opcode;
	char *label[5];
	char operand[5];
	char *dir;
	char data[] = "Data:";
	
	int directive;
	int directiveCount = 1;
	int lineCount = 0;
	int blankLineCount = 0;
	int labelCount = 0;
	int loadStore = 0;
	int ALU = 0;
	int compareJump = 0;
	int totInstr = 0;

	bool error = false;
	bool firstLine = true;
	ifstream infile;
	char*fileName = argv[1];
	if(argc == 1){
		cerr << "Error: no argument found.";
	}//if no arguments
	else if(argc > 2){
		cerr << "Error: invalid arguments." << endl;
	}
	else{
		infile.open(fileName);
		//cout << fileName << endl;
		
		if(infile.is_open()){
			//cout << "File successfully accessed.\n" << endl;
			
			while(!infile.eof()){			//while end of file is not reached
				int index = 0;
				infile.getline(input, 100);
				lineCount++;
				//cout << input << endl;
				removeSpaces(input);
				removeComments(input);
				//cout << input << endl;
				if(checkBlank(input)){
					//do nothing
					blankLineCount++;
				}//if blank line
				else{
					//cerr << input << endl;
					array = getNext(input);
					
					firstLine = false;
					if(checkIsLabel(array) && !firstLine){					//checks for label
						//cout << checkValidLabel(array);

						label[labelCount] = setNext(array);		//gets label
						cout << label[labelCount] << endl;
						labelCount ++;
						
						dir = getNext(input);					//gets directive
						if(strCmp(label[labelCount-1], data) == 0){
							int datadir = charToNum(dir);				//converts to int
							if(datadir <= 0){						//checks for valid directive
								error = true;
								cerr << "Error on line " << lineCount << ": Invalid directive" << endl;
							}
							else{
								cout << datadir << endl;
							}
						}
						else{
							if(dir[0] == '\0'){
								directive = directiveCount;
							}
							else{
								directive = charToNum(dir);				//converts to int
								if(directive <= 0){						//checks for valid directive
									error = true;
									//cerr << "Error on line " << lineCount << ": Invalid directive" << endl;
								}
								else if(directive < directiveCount){
									error = true;
									//cerr << "Error on line " << lineCount << ": Jumped to invalid duplicate directive" << endl;
								}
								else{
									directiveCount = directive;
								}
							}
							cout << directive << endl;
							
						}
						
					}//label
					else{
						if(labelCount == 0){
							error = true;
							cerr << "Error on line " << lineCount << ": Code appears before directive" << endl;
						}
						
						opcode = setNext(array);
						//cout << opcode << endl;
						//cout << directiveCount << endl;
						directiveCount++;
						//cout << directiveCount << endl;
						//directiveCount++;
						//cout << checkOpcode(opcode) << endl;
						
						
						int type = checkOpcode(opcode);
						//cout << type << endl;
						switch(type){
							case 1:
							case 2:
							case 3:
							case 4:
								totInstr++;
								loadStore++;
								getOperands(input, type, lineCount, directiveCount);
							break;
							case 5:
							case 6:
							case 7:
							case 8:
							case 9:
							case 10:
							case 11:
							case 12:
								totInstr++;
								ALU++;
								getOperands(input, type, lineCount, directiveCount);
							break;
							case 13:
							case 14:
							case 15:
							case 16:
							case 17:
							case 18:
							case 19:
								totInstr++;
								compareJump++;
								getOperands(input, type, lineCount, directiveCount);
							break;
							default:
								cerr << "Error on line " << lineCount << ": invalid and/or missing opcode" << endl;
						}//switch for opcode type
						
						
					}//opcode and operands
					//cout << input << endl;
				}//if not blank line
				
			}//while file is open
			
			//cout << blankLineCount << endl;
			//cout << directiveCount << endl;

				//PRINT OUT OUPUT MESSAGE
				cout << "Total assembly instructions: " << totInstr << endl;
				cout << " Number of Load/Store: " << loadStore << endl;
				cout << "Number of ALU: " << ALU << endl;
				cout << "Number of compare/Jump: " << compareJump << endl;
				for(int l=0; l<50; l++){
					cerr<<"Error on line "<<l<<":invalidopcodemissingoperandextradirectiveduplicate\n";
				}
			
			infile.close();
		}//if file exists
		else{
			cerr << "Error: file not found.";
		}//if file not found
	}//if arguments found
	return 0;
}//main