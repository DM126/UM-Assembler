Assembler used to create programs to be executed by the Universal Machine from the [2006 ICFP programming contest](http://www.boundvariable.org/).  

Full description of the Universal Machine instruction set can be found [here](http://www.boundvariable.org/um-spec.txt)  

The assembler reads a text file containing UM instructions and converts it to a binary ouput file. Each line must contain one full instruction. This assembly language is stack-based, so the operation goes at the end of the instruction.  

8 Registers: R0-R7  

14 Instructions:  
CMOV  
INDEX  
AMEND  
ADD  
MUL  
DIV  
NAND  
HALT  
ALLOC  
ABAND  
OUT  
IN  
LOAD  
ORTH  

Sample instructions:  
7 R0 ORTH       #Load 7 into R0  
R0 R0 R1 CMOV   #Move The contents of R0 to R1  
R1 R0 R2 ADD    #Add R1 to R0 and store in R2
