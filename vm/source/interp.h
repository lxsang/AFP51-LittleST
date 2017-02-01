/*
	Little Smalltalk, version 2
	Written by Tim Budd, Oregon State University, July 1987
*/

/*
	symbolic definitions for the bytecodes
*/

#define Extended 0
#define PushInstance 1
#define PushArgument 2
#define PushTemporary 3
#define PushLiteral 4
#define PushConstant 5
#define AssignInstance 6
#define AssignTemporary 7
#define MarkArguments 8
#define SendMessage 9
#define SendUnary 10
#define SendBinary 11
#define pushBlock 	12
#define DoPrimitive 13
#define DoSpecial 15

/* a few constants that can be pushed by PushConstant 
#define minusOne 3		
#define contextConst 4		
#define nilConst 5		
#define trueConst 6		
#define falseConst 7		 */
/* constants 0 to 9 are the integers 0 to 9 */
#define nilConst   10
#define trueConst  11
#define falseConst 12
#define minusOne 13		/* the value -1 */

/* types of special instructions (opcode 15) */

#define SelfReturn 1
#define StackReturn 2
#define BlokReturn 3
#define Duplicate 4
#define PopTop 5
#define Branch 6
#define BranchIfTrue 7
#define BranchIfFalse 8
#define AndBranch 9
#define OrBranch 10
#define SendToSuper 11
