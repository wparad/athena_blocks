//============================================================================
// Name        : Athena_Blocks.cpp
// Author      : Warren Parad
// Version     : v1.0
// Date		   : 9/25/2012
// Copyright   : Copy at your own risk
// Description : Hello World in C++, Ansi-style
/*IMPORTANT DETAILS:
 *
 * Answer: 806844323190414
 * Runtime: 88.091  [fastest time]
 * Computer Specs: hp laptop
 * 	xubuntu (ubuntu linux 10.04)
 * 	GCC 4.4.3 (x86_44)
 * 	CPU: AMD Turion II Ultra Dual-Core Mobile M620
 * 		Freq: 800 MHz
 * 		L2 Cache: 1024
 * 	RAM:  3GB (but ran the program with 2899MiB free)
 * 	Motherboard Host Bridge:
 * 		AMD K10
 * 		HP Company Device 3638
 *
 *		Three compiler flags can be set to see interesting information about the program as it runs
 	 	 //run in Debug mode switch
		#define _DEBUG 0
		//see execution running time
		#define _CHECK_EXEC_TIME 0
		//change this to show error messages instead of just seeing a "0" output on failure
		#define _SHOW_ERRORS 0
 */
/*
 *Your niece was given a set of blocks for her birthday, and she has decided to build a panel using
3”×1” and 4.5”×1" blocks. For structural integrity, the spaces between the blocks must not line up
in adjacent rows. For example, the 13.5”×3” panel below is unacceptable, because some of the
spaces between the blocks in the first two rows line up (as indicated by the dotted line).

There are 2 ways in which to build a 7.5”×1” panel, 2 ways to build a 7.5”×2” panel, 4 ways to
build a 12”×3” panel, and 7958 ways to build a 27”×5” panel. How many different ways are there
for your niece to build a 48”×10” panel? The answer will fit in a 64-bit integer. Write a program to
calculate the answer.

The program should be non-interactive and run as a single-line command which takes two
command-line arguments, width and height, in that order. Given any width between 3 and 48 that
is a multiple of 0.5, inclusive, and any height that is an integer between 1 and 10, inclusive, your
program should calculate the number of valid ways there are to build a wall of those dimensions.
Your program’s output should simply be the solution as a number, with no line-breaks or white
spaces.

Your program will be judged on how fast it runs and how clearly the code is written. We will be
running your program as well as reading the source code, so anything you can do to make this
process easier would be appreciated.

Send the source code and let us know the value that your program computes, your program’s
running time, and the kind of machine on which you ran it.
*/
//============================================================================
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <cmath>
#include <vector>
#include <sys/time.h>

//run in Debug mode switch
#define _DEBUG 0
#define _CHECK_EXEC_TIME 0
//change this to show error messages
#define _SHOW_ERRORS 0

#define MATRIX_MAX_D1 3329
#define MATRIX_MAX_D2 48
using namespace std;

//struct to cache permutations and where the cracks are in a line
struct line{
	unsigned int cracks;
	long long perm[10];
};
//global counter to keep track of the permutations for height 1
// normally i would never use a global outside of OOP, but I wanted to see if this would make a difference in
// performance.
unsigned short counter = 0;

unsigned long long wallPermutations(int,int,short,short (*)[MATRIX_MAX_D2],line*);
void linePermutations(int,unsigned int,line*);
bool overlapCrack(int, int,int);
void printLine(int,int);

int main(int argc, char *argv[]) {

	#if _DEBUG|_CHECK_EXEC_TIME
		timeval t1, t2;
		double execTime = 0;
		gettimeofday(&t1, NULL);
	//	for(int i = 0; i < argc; i++) cout <<"arg "<<i<<": "<<argv[i]<< endl;
		//validate command line parameters
	#endif

	#if _SHOW_ERRORS
		if(argc != 3){
			cout << "Wall Permutations takes 2 arguments." << endl;
			return 1;
		}

		double decimal = 0;
		//validate width
		double width = 0.0;
		int intWidth = 0;
		istringstream wstr(argv[1]);
		if(!(wstr >> width)) {
			cout << "The width is not a valid value.: " << width <<"."<< endl;
			return 1;
		}
		//even if it is a valid double, it might not be a valid width
		else if(fmod(width*2/3.,1) != 0){
			cout << "No wall for that width can be made from 3\" and 4.5\" bricks." << endl;
			return 1;
		}
		else	{
			decimal = fmod(width,1);
			if(width < 3 || width > 48 || (decimal != 0 && decimal != .5)){
				cout << "The width is not a valid number." << endl;
				return 1;
			}
		}
		//MUST use for <int> math, this makes everything simpler including the use of an int for binary operations
		intWidth = int(width*2/3.);
		//validate height
		double height = 0;
		istringstream hstr(argv[2]);
		if(!(hstr >> height)) {
			cout << "The height is not a valid value: " << height <<"."<< endl;
			return 1;
		}
		else if( height < 1 || height > 10 || height != int(height)){
			cout << "The height is not a valid number." << endl;
			return 1;
		}
	#else
		//begin do not show errors
		if(argc != 3){
			cout<<"0"<<endl;
			return 1;
		}
		double decimal = 0;
		//validate width
		double width = 0.0;
		int intWidth = 0;
		istringstream wstr(argv[1]);
		if(!(wstr >> width)) {
			cout<<"0"<<endl;
			return 1;
		}
		//even if it is a valid double, it might not be a valid width
		else if(fmod(width*2/3.,1) != 0){
			cout<<"0"<<endl;
			return 1;
		}
		else	{
			decimal = fmod(width,1);
			if(width < 3 || width > 48 || (decimal != 0 && decimal != .5)){
				cout<<"0"<<endl;
				return 1;
			}
		}
		//MUST use for <int> math, this makes everything simpler including the use of an int for binary operations
		intWidth = int(width*2/3.);
		//validate height
		double height = 0;
		istringstream hstr(argv[2]);
		if(!(hstr >> height)) {
			cout<<"0"<<endl;
			return 1;
		}
		else if( height < 1 || height > 10 || height != int(height)){
			cout<<"0"<<endl;
			return 1;
		}
		//end do not show errors
	#endif

	//max number of possible permutations for a line at a specific width
	line* permLines = new line[MATRIX_MAX_D1];
	//generate line array
	linePermutations(intWidth,1,permLines);
	/*Max calculated no overlapping lines for the same position in a wall.
		 * Normally this would never be done, but it is here for 3 reasons:
		 * 1) A width of 48 perfectly reduces to 32 bits in this program preventing a wider line from being
		 * calculated, therefore making this assumption.
		 * 2) To create a lookup table due to computer resources this array again has to be limited, so the smallest
		 * size was chosen.
		 * 3) Without a lookup table, the program execution time is too expensive, something on the order of 3329^10
		 * computations.
		 */
	short (*allowedLines)[MATRIX_MAX_D2] = new short[MATRIX_MAX_D1+1][MATRIX_MAX_D2];
#if _DEBUG
	gettimeofday(&t2, NULL);
	execTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
	execTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
	cout << "time: " << execTime << endl;
#endif

	//populate which lines can be next to each other if for each line there is another that can be adjacent
	// then allowedLines[first line+1][count] = second line+1 and
	// 		allowedLines[second line+1][second count] = first line+1
	//the reason the +1 are in there is to allow a "NULL" at 0 and recognize this fact without negative numbers
	int next = 0;
	//compile time determination allowed pre-initialization and causes ln2Next to be 0 in 0ms.
	short *ln2Next = new short[MATRIX_MAX_D1];
	for(unsigned short ln = 0; ln < counter ; ++ln) {
			next = 0;
			for(unsigned short ln2 = ln+1; ln2 < counter; ++ln2){
				if(!overlapCrack(permLines[ln].cracks,permLines[ln2].cracks,intWidth)){
					allowedLines[ln+1][next++] = ln2+1;
					allowedLines[ln2+1][ln2Next[ln2]++] = ln+1;
				}
			}
			//uncomment line to visually see what a height 1 permutation line looks like
			#if _DEBUG
				printLine(permLines[ln].cracks,intWidth);
			#endif
	}

	#if _DEBUG|_CHECK_EXEC_TIME
		gettimeofday(&t2, NULL);
		execTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
		execTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
		cout << "time: " << execTime << endl;
	#endif
	//**************************//
	//Run the wal permutations
	cout<< wallPermutations(intWidth,(int)height,-1,allowedLines,permLines) << endl;
	delete[] allowedLines;

	#if _DEBUG|_CHECK_EXEC_TIME
		gettimeofday(&t2, NULL);
		execTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
		execTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
		cout << "time: " << execTime << endl;
	#endif
	return 0;
}
//comput wall permutations from available lines
unsigned long long wallPermutations(int width, int height,short index,
		short (*allowedLines)[MATRIX_MAX_D2],line* permLines){
#if _DEBUG
//	cout<<index<<":"<<height<<":"<<endl;
#endif
	unsigned long long tmp = 0;
	short line = 0;
	if(height < 1) return 0;  //no height no possible permutations
	unsigned long long total = 0;
	//force for height =1 either the counter for real height 1, or recursive.  If recursive grab total from the array
	// generated by the main inlined double for loop highlighted above.
	if(height == 1){
		if(index == -1) return counter;
		tmp = permLines[index].perm[0];
		if(tmp != 0) return tmp;
		for(unsigned short ln = 0; ln < MATRIX_MAX_D2;++ln){
			if(allowedLines[index+1][ln] == 0) break;
			++tmp;
		}
		permLines[index].perm[0] = tmp;
		return tmp;
	}
	//index == -1 means the first pass we have no restrictions, we also know height != 1, so we dont have to worry about anything.
	if(index == -1) {
		//for the first row there are no restrictions, assume any of the possible lines can be there
		for(unsigned short ln = 0; ln < counter; ++ln){
			total+= wallPermutations(width,height-1,ln,allowedLines,permLines);
		}
		return total;
	}

	//its not the bottom row or the first row because index == -1 and height == 1 have already taken care of those
	tmp = 0;
	for(unsigned short ln = 0; ln < MATRIX_MAX_D2;++ln){
		//for each possible line check to see if it can come next
		line = allowedLines[index+1][ln];
		//no more allowed lines remaining, we can reset and adjust back a level
		if(line == 0) 	return total;
		//CACHING
		tmp = permLines[line-1].perm[height-1]; //tmp = 0 //turn on/off caching lowest level performance enhancement;
		if(tmp == 0) {
			//recursive call for next row
			tmp=wallPermutations(width,height-1,line-1,allowedLines,permLines);
			permLines[line-1].perm[height - 1] = tmp;
		}
		//END CACHING
		total += tmp;
	}

	//return the number of permutations there are for the current line * the number of wallPermuation
	// for everything below considering this restriction
	return total;
}
/*
 * This is going to get complicated really fast to maintain performance.  I want to use bits to represent the
 * location of cracks.  then using the bitwise & we and equivalence to 0 we are going to check if a block can
 * be next to another one.
 * Since the bricks are 3 and 4.5 and the max length of the wall is 48 lets instead do this problem by 2/3
 * so bricks 2 and 3 and wall length of 32, since the crack at 0 and 32 does not count and there is no brick
 * of length 1, we cannot have cracks at 1 or 31 which leaves 2 through 30 or 29 possible cracks.  This value
 * will fit in a variable of length 32 easily.
 */
void linePermutations(int width,unsigned int curLine, line* permLines){
	if(width == 2 )	{
		permLines[counter++].cracks = curLine; return;
	}
	else if(width == 3){
		//line up the last three block size with the the last 2 block size
		permLines[counter++].cracks = (curLine << 1); return;
	}
	else if(width <= 1) return;
	else {
		//we want to add this _ _|
		linePermutations(width-2,(curLine<<2)|1,permLines);
		//we want to add this _ _ _|
		linePermutations(width-3,(curLine<<3)|1,permLines);
	}
}
/*returns if 2 brick patterns will have overlapping cracks, this is a simple bit-wise & because of how the lines
 * were determined. intWidth is required to remove the place holder at the MSB.
 * */
bool overlapCrack(int lineA, int lineB,int intWidth){
//	return (lineA  & lineB) & (0b0111111111111111111111111111111 >> (32-intWidth));
	return (lineA  & lineB) > (1 <<(intWidth -2));
}


//Visually will print a line given a line in the <int> format generated from a height 1 wall and specified intWidth.
void printLine(int line,int intWidth){
	if(line == 0) return;
	//if we had added back one more space to the int to take the full 32 we could use a -1 instead of -2,
	//but its the same
	for(int A = intWidth - 2; A>=0 ; A--){
		if((line >> A & 1) == 1) cout<<" _";
		else cout << " _";
	}
	cout<< " _" << endl;

	for(int A = intWidth - 2; A>=0 ; A--){
		if((line >> A & 1) == 1) cout<<"|_";
		else cout << " _";
	}
	cout<< " _|" << endl;
}
