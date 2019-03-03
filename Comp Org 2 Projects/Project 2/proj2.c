#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUMMEMS 16
#define NUMREGS 8

/* Opcode values for instructions */
#define AL 0
#define LW  35
#define SW  43
#define BEQ 4
#define HALT 63

/* Funct values for R-type instructions */
#define ADD 32
#define SUB 34
int stall_detect = 0;
int is_forwarded = 0;
int took_branch = 0;

typedef struct IFIDReg {
    unsigned int instr;		/* Integer representation of instruction */
    int	  PCPlus4;		/* PC + 4 */
}     IFIDReg_t;

typedef struct IDEXReg {
    unsigned int instr;		/* Integer representation of instruction */
    int	  PCPlus4;		/* PC + 4 */
    int	  readData1;		/* Contents of rs register */
    int	  readData2;		/* Contents of rt register */
    int	  immed;		/* Immediate field */
    int	  rsReg;		/* rs register number */
    int	  rtReg;		/* rt register number */
    int	  rdReg;		/* rd register number */
    int	  branchTarget;		/* Branch target calculated */
}     IDEXReg_t;

typedef struct EXMEMReg {
    unsigned int instr;		/* Integer representation of instruction */
    int	  ALUoutput;		/* Result of ALU operation */
    int	  rtRegData;		/* Contents of the rt register */
    int	  writeRegNum;		/* The destination register */
}     EXMEMReg_t;

typedef struct MEMWBReg {
    unsigned int instr;		/* Integer representation of instruction */
    int	  ALUoutput;		/* Result from ALU operation */
    int	  memOutput;		/* Data read from memory */
    int	  writeRegNum;		/* The destination register */
}     MEMWBReg_t;

typedef struct mipsState {
    int	  PC;			/* Program Counter */
    unsigned int instrMem[NUMMEMS];	/* Instruction memory */
    int	  dataMem[NUMMEMS];	/* Data memory */
    int	  regFile[NUMREGS];	/* Register file */

    IFIDReg_t IFID;		/* Current IFID pipeline register */
    IDEXReg_t IDEX;		/* Current IDEX pipeline register */
    EXMEMReg_t EXMEM;		/* Current EXMEM pipeline register */
    MEMWBReg_t MEMWB;		/* Current MEMWB pipeline register */

    int	  cycles;		/* Number of cycles executed so far */
    int	  forwards;		/* Number of forwarding steps performed */
    int	  stalls;		/* Number of stall cycles executed */
    int	  branches;		/* Number of branch instructions so far */
    int	  taken;		/* Number of taken branches so far */

}     mipsState_t;

int   detect_forward (mipsState_t *);
int   detect_stall (mipsState_t *);
void  simIFStage (mipsState_t *, int);
void  simIDStage (mipsState_t *, int *, int);
void  simEXStage (mipsState_t *);
void  simMEMStage (mipsState_t *);
void  simWBStage (mipsState_t *);
void  branch_forward (mipsState_t *);
void  branch_stall (mipsState_t *);
int  check_1a (mipsState_t *);
int  check_1b (mipsState_t *);
int  check_2a (mipsState_t *);
int  check_2b (mipsState_t *);
int  lw_forward_1 (mipsState_t *);
int  lw_forward_2 (mipsState_t *);

void  printState (mipsState_t *);
void  initState (mipsState_t *);
unsigned int instrToInt (char *, char *);
void  printInstruction (unsigned int);

//these check the instruction name :)
int add_check (unsigned int);
int sub_check (unsigned int);
int lw_check (unsigned int);
int sw_check (unsigned int);
int halt_check (unsigned int);
int noop_check (unsigned int);

#define get_rs(i) (((i >> 21) & 0x1F))
#define get_rt(i) (((i >> 16) & 0x1F))
#define get_rd(i) (((i >> 11) & 0x1F))

#define get_funct(i) ((i & 0x3F))
#define get_opc(i)   ((i >> 26))

#define isMemWrite(i) (get_opc(i) == SW)
#define isMemRead(i)  (get_opc(i) == LW)
#define isBranch(i)   (get_opc(i) == BEQ)

int get_immed (unsigned int ins)
{
    int	  immed = ((ins & 0xFFFF));
    if (immed > 32767)
	return (immed - 65536);
    else
	return immed;
}

int hasInput (unsigned int ins)
{
    int	  opcode = get_opc (ins);
    return (opcode == LW || opcode == SW || opcode == AL);
}

int hasRtInput (unsigned int ins)
{
    int	  opcode = get_opc (ins);
    return (opcode == AL || opcode == BEQ);
}

int isWriteReg (unsigned int ins)
{
    int	  opcode = get_opc (ins);
    return (opcode == LW || opcode == AL);
}

int main ()
{
    int npc;
    mipsState_t current;
    mipsState_t next;

    initState (&current);		/* Initialize the pipeline current */

    while (1) {

	/* halt the pipeline if a halt instruction is at its WB stage */

	printState (&current);
	if (get_opc (current.MEMWB.instr) == HALT)
	    break;

	next = current;
	next.cycles++;

	/* ======  Your code Start Here ====== */
     stall_detect = 0;
     npc = current.PC;
     simWBStage(&next);
     simMEMStage(&next);
     simEXStage(&next);
     simIDStage(&next, &next.IFID.PCPlus4, current.PC);
     simIFStage(&next, npc);
     next.PC = next.IFID.PCPlus4;

	 /* ======  Your code ends Here ====== */

	current = next;	/* switch the state*/
    }

    /*
     * Remember to print the number of stalls, branches, and
     * mispredictions!
     */
    printf ("---------------- Final Report ------------------\n");
    printf ("Total cycles (including halt): %d;", current.cycles+1);
    printf (" forwards: %d;", current.forwards);
    printf (" stalls: %d;", current.stalls);
    printf (" branches: %d;", current.branches);
    printf (" taken: %d.\n", current.taken);

    return 0;
}

int check_1a (mipsState_t * s)
{
     if(!lw_check(s->EXMEM.writeRegNum) && !beq_check(s->EXMEM.writeRegNum)
     && !noop_check(s->EXMEM.instr) && !halt_check(s->EXMEM.instr)
     && s->EXMEM.writeRegNum != 0 && !sw_check(s->EXMEM.instr) && !lw_check(s->IDEX.instr))
     {
          if(s->EXMEM.writeRegNum == s->IDEX.rsReg)
          {
               return 1;
          }
     }
     return 0;
}

int check_1b(mipsState_t * s)
{
     if(!lw_check(s->EXMEM.writeRegNum) && !beq_check(s->EXMEM.writeRegNum)
     && !noop_check(s->EXMEM.instr) && !halt_check(s->EXMEM.instr)
     && s->EXMEM.writeRegNum != 0 && !sw_check(s->EXMEM.instr) && !lw_check(s->IDEX.instr))
     {
          if(s->EXMEM.writeRegNum == s->IDEX.rtReg)
          {
               return 1;
          }
     }
     return 0;
}

int check_2a (mipsState_t * s)
{
     if(!lw_check(s->MEMWB.writeRegNum) && !beq_check(s->MEMWB.writeRegNum)
     && !noop_check(s->MEMWB.instr) && !halt_check(s->MEMWB.instr) && !check_1a(s)
     && s->MEMWB.writeRegNum != 0 && !sw_check(s->MEMWB.instr))
     {
          if(s->MEMWB.writeRegNum == s->IDEX.rsReg)
          {
               return 1;
          }
     }
     return 0;
}

int check_2b(mipsState_t * s)
{
     if(!lw_check(s->MEMWB.writeRegNum) && !beq_check(s->MEMWB.writeRegNum)
     && !noop_check(s->MEMWB.instr) && !halt_check(s->MEMWB.instr) && !check_1b(s)
     && s->MEMWB.writeRegNum != 0 && !sw_check(s->MEMWB.instr))
     {
          if(s->MEMWB.writeRegNum == s->IDEX.rtReg)
          {
               return 1;
          }
     }
     return 0;
}

int branch_taken(mipsState_t * s)
{
     if(beq_check(s->IDEX.instr))
     {
          if(((s->IDEX.readData1) == (s->IDEX.readData2)) || ((s->IDEX.rtReg) == (s->IDEX.rsReg)))
          {
          s->taken++;
          return 1;
          }
     }
     return 0;
}

void branch_forward(mipsState_t * s)
{
     if(beq_check(s->IFID.instr) && lw_check(s->MEMWB.instr))
     {
          if(get_rs(s->IFID.instr) == s->MEMWB.memOutput)
          {
               s->forwards++;
               s->IDEX.readData1 == s->MEMWB.memOutput;
          }
          if(get_rt(s->IFID.instr) == s->MEMWB.memOutput)
          {
               s->forwards++;
               s->IDEX.readData2 == s->MEMWB.memOutput;
          }
     }
}

int lw_forward_1(mipsState_t * s)
{
     if(lw_check(s->MEMWB.instr) &&  s->MEMWB.writeRegNum != 0)
     {
          if(s->MEMWB.writeRegNum == get_rs(s->IDEX.instr))
          {
               return 1;
          }
     }
     return 0;
}

int lw_forward_2(mipsState_t * s)
{
     if(lw_check(s->MEMWB.instr) && s->MEMWB.writeRegNum != 0)
     {
          if(s->MEMWB.writeRegNum == get_rt(s->IDEX.instr))
          {
               return 1;
          }
     }
     return 0;
}

int detect_stall (mipsState_t * s)
{
     /* ======  Add your code Here ====== */
     if(beq_check(s->IFID.instr))
     {
          if(lw_check(s->MEMWB.instr))
          {
               if(get_rt(s->IFID.instr) == get_rt(s->MEMWB.instr))
                    return 1;
               if(get_rs(s->IFID.instr) == get_rt(s->MEMWB.instr))
                    return 1;
          }
     }
     if(lw_check(s->IDEX.instr) && s->IDEX.rtReg != 0)
     {
          if((add_check(s->IFID.instr)) || (sub_check(s->IFID.instr)))
          {
               if((get_rs(s->IFID.instr)) == (get_rt(s->IDEX.instr))
               || (get_rt(s->IFID.instr)) == (get_rt(s->IDEX.instr)))
                    return 1;
          }
          if((lw_check(s->IFID.instr)) || (sw_check(s->IFID.instr)))
          {
               if((get_rs(s->IFID.instr)) == (get_rt(s->IDEX.instr)))
                    return 1;
          }
          if(beq_check(s->IFID.instr))
          {
               if((get_rd(s->IFID.instr)) == (get_rt(s->IDEX.instr))
               || (get_rs(s->IFID.instr)) == (get_rt(s->IDEX.instr)))
                    return 1;
          }
     }
     if(beq_check(s->IFID.instr))
     {
          if(add_check(s->IDEX.instr) || (sub_check(s->IDEX.instr)))
          {
               if(get_rt(s->IFID.instr) == (s->IDEX.rdReg))
                    return 1;
               if(get_rs(s->IFID.instr) == (s->IDEX.rdReg))
                    return 1;

          }
          if(lw_check(s->IDEX.instr))
          {
               if(get_rt(s->IFID.instr) == (s->IDEX.rtReg))
                    return 1;
               if(get_rs(s->IFID.instr) == (s->IDEX.rtReg))
                    return 1;
          }
     }
     return 0;
}

void simIFStage (mipsState_t * s, int npc)
{

     if(stall_detect == 1)
          return;

     s-> IFID.instr = s->instrMem[s->IFID.PCPlus4/4];
     s-> IFID.PCPlus4 = npc+4;


}

void simIDStage (mipsState_t * s, int *npc, int pc)
{

     if(detect_stall(s) == 1)
          {
               stall_detect = 1;
               s->stalls++;
               s->IDEX.instr = 0;
               s->IDEX.PCPlus4 = 0;
               s->IDEX.branchTarget = 0;
               s->IDEX.readData1 = 0;
               s->IDEX.readData2 = 0;
               s->IDEX.immed = 0;
               s->IDEX.rsReg = 0;
               s->IDEX.rtReg = 0;
               s->IDEX.rdReg = 0;
          }
     else
          s-> IDEX.instr = s->IFID.instr;

     /* ======  Add your code Here ====== */
     s-> IDEX.PCPlus4 = s->IFID.PCPlus4;
     s-> IDEX.rsReg = get_rs(s->IDEX.instr);
     s-> IDEX.readData1 = s-> regFile[s->IDEX.rsReg];
     s-> IDEX.rtReg = get_rt(s->IDEX.instr);
     s-> IDEX.readData2 = s-> regFile[s->IDEX.rtReg];
     s-> IDEX.rdReg = get_rd(s->IDEX.instr);
     s-> IDEX.immed = get_immed(s->IDEX.instr);
     s-> IDEX.branchTarget = (4 * (s->IDEX.immed)+(s->IDEX.PCPlus4));

     if(s-> IDEX.instr == 0)
     {

          s-> IDEX.PCPlus4 = 0;
          s-> IDEX.rsReg = 0;
          s-> IDEX.readData1 = 0;
          s-> IDEX.rtReg = 0;
          s-> IDEX.readData2 = 0;
          s-> IDEX.rdReg = 0;
          s-> IDEX.immed = 0;
          s-> IDEX.branchTarget = 0;
     }

}

void simEXStage (mipsState_t * s)
{
     /* ======  Add your code Here ====== */

     if(branch_taken(s))
     {
          took_branch = 1;
     }


     if(check_1a(s) == 1)
     {

          s->forwards++;
          s->IDEX.readData1 = s->EXMEM.ALUoutput;
     }

     if(check_1b(s) == 1)
     {


          s->forwards++;
          s->IDEX.readData2 = s->EXMEM.ALUoutput;
     }

     s->EXMEM.instr = s->IDEX.instr;


     if(add_check(s-> IDEX.instr))
     {
          s->EXMEM.ALUoutput = s->IDEX.readData1 + s->IDEX.readData2;
          s->EXMEM.writeRegNum = s->IDEX.rdReg;
          s->EXMEM.rtRegData = s->IDEX.readData2;
     }

     if(sub_check(s->IDEX.instr))
     {
          s->EXMEM.ALUoutput = s->IDEX.readData1 - s->IDEX.readData2;
          s->EXMEM.writeRegNum = s->IDEX.rdReg;
          s->EXMEM.rtRegData = s->IDEX.readData2;
     }

     if(lw_check(s->IDEX.instr))
     {
          s->EXMEM.ALUoutput = s->IDEX.immed;
          s->EXMEM.writeRegNum = s->IDEX.rtReg;
          s->EXMEM.rtRegData = s->IDEX.readData2;
     }

     if(sw_check(s->IDEX.instr))
     {
          s->EXMEM.ALUoutput = s->IDEX.immed;
          s->EXMEM.writeRegNum = s->IDEX.rtReg;
          s->EXMEM.rtRegData = s->IDEX.readData2;
     }

     if(beq_check(s->IDEX.instr))
     {
          s->EXMEM.ALUoutput = 0;
          s->EXMEM.writeRegNum = 0;
          s->EXMEM.rtRegData = s->IDEX.readData2;
     }

     if(halt_check(s->IDEX.instr) || noop_check(s->IDEX.instr))
     {
          s->EXMEM.ALUoutput = 0;
          s->EXMEM.writeRegNum = 0;
          s->EXMEM.rtRegData = 0;
     }
}

void simMEMStage (mipsState_t *s)
{

     if(lw_check(s->EXMEM.instr))
     {
          s->MEMWB.ALUoutput = s-> EXMEM.ALUoutput;
          s->MEMWB.memOutput = s->dataMem[s->MEMWB.ALUoutput/4];
     }
     if(sw_check(s->EXMEM.instr))
     {
          s->MEMWB.ALUoutput = s-> EXMEM.ALUoutput;
          s->MEMWB.memOutput = 0;
          s->dataMem[(s->MEMWB.ALUoutput/4)] = s->regFile[s->MEMWB.writeRegNum];
     }
     if(add_check(s->EXMEM.instr))
     {
          s->MEMWB.ALUoutput = s-> EXMEM.ALUoutput;
          s->MEMWB.memOutput = 0;
     }
     if(sub_check(s->EXMEM.instr))
     {
          s->MEMWB.ALUoutput = s-> EXMEM.ALUoutput;
          s->MEMWB.memOutput = 0;
     }
     if(beq_check(s->EXMEM.instr))
     {
          s->MEMWB.memOutput = 0;
          s->MEMWB.ALUoutput = 0;
     }
     if(halt_check(s->EXMEM.instr))
     {
          s->MEMWB.memOutput = 0;
          s->MEMWB.ALUoutput = 0;
          s->MEMWB.writeRegNum = 0;
     }
     if(s->MEMWB.instr == 0)
     {
          s->MEMWB.memOutput = 0;
          s->MEMWB.ALUoutput = 0;
          s->MEMWB.writeRegNum = 0;
     }
}

void simWBStage (mipsState_t * s)
{
     /* ======  Add your code Here ====== */
     if(!sw_check(s->IDEX.instr) && !lw_check(s->MEMWB.instr))
     {
          if(check_2a(s) == 1)
          {
               s->forwards++;
               s->IDEX.readData1 = s->MEMWB.ALUoutput;
          }
          if(check_2b(s) == 1)
          {
               s->forwards++;
               s->IDEX.readData2 = s->MEMWB.ALUoutput;
          }
     }
     if(s->MEMWB.writeRegNum != 0)
     {
          if((add_check(s->IDEX.instr) || sub_check(s->IDEX.instr)
          || !sw_check(s->IDEX.instr)))
          {

               if(lw_forward_1(s) == 1)
               {
                    s->forwards++;
                    s->IDEX.readData1 = s->MEMWB.memOutput;
               }

               if(lw_forward_2(s) == 1)
               {
                    s->forwards++;
                    s->IDEX.readData2 = s->MEMWB.memOutput;
               }
          }
     }

     if(s->MEMWB.writeRegNum == 0)
          {
               s->MEMWB.ALUoutput = 0;
               s->MEMWB.memOutput = 0;
          }


     if(lw_check(s->MEMWB.instr))
          s->regFile[s->MEMWB.writeRegNum] = s->MEMWB.memOutput;
     if(add_check(s->MEMWB.instr))
          s->regFile[s->MEMWB.writeRegNum] = s->MEMWB.ALUoutput;
     if(sub_check(s->MEMWB.instr))
          s->regFile[s->MEMWB.writeRegNum] = s->MEMWB.ALUoutput;
          if(beq_check(s->MEMWB.instr))
               s->branches++;


               branch_forward(s);

          s->MEMWB.instr = s->EXMEM.instr;
          s->MEMWB.writeRegNum = s->EXMEM.writeRegNum;


}

/******************************************************************/
/*
 * The initState function accepts a pointer to the current state as an
 * argument, initializing the state to pre-execution state. In particular,
 * all registers are zero'd out. All instructions in the pipeline are NOOPS.
 * Data and instruction memory are initialized with the contents of the
 * assembly input file.
 /*
/*****************************************************************/

int add_check(unsigned int i)
{
     if(get_opc(i) == 0 && get_funct(i) == 32)
          return (1);
     else
          return (0);
}

int sub_check(unsigned int i)
{
     if(get_opc(i) == 0 && get_funct(i) == 34)
          return (1);
     else
          return (0);
}

int lw_check(unsigned int i)
{
     if(get_opc(i) == 35)
          return (1);
     else
          return (0);
}

int sw_check(unsigned int i)
{
     if(get_opc(i) == 43)
          return (1);
     else
          return (0);
}


int beq_check(unsigned int i)
{
     if(get_opc(i) == 4)
          return (1);
     else
          return (0);
}

int halt_check(unsigned int i)
{
     if(get_opc(i) == 63)
          return (1);
     else
          return (0);
}

int noop_check(unsigned int i)
{
     if(i == 0)
          return (1);
     else
          return (0);
}

void initState (mipsState_t * sp)
{
    unsigned int dec_inst;
    int	  data_index = 0;
    int	  inst_index = 0;
    char  line[130];
    char  instr[6];
    char  args[130];
    char *arg;

    bzero(sp, sizeof(mipsState_t));

    /* Zero out data, instructions, and registers */
    memset (sp->dataMem, 0, 4 * NUMMEMS);
    memset (sp->instrMem, 0, 4 * NUMMEMS);
    memset (sp->regFile, 0, 4 * NUMREGS);

    /* Parse assembly file and initialize data/instruction memory */
    while (fgets (line, 130, stdin)) {
	if (sscanf (line, "\t.%s %s", instr, args) == 2) {
	    arg = strtok (args, ",");
	    while (arg != NULL) {
		sp->dataMem[data_index] = atoi (arg);
		data_index += 1;
		arg = strtok (NULL, ",");
	    }
	} else if (sscanf (line, "\t%s %s", instr, args) == 2) {
	    dec_inst = instrToInt (instr, args);
	    sp->instrMem[inst_index] = dec_inst;
	    inst_index += 1;
	}
    }

    /* Zero-out all registers in pipeline to start */
    sp->IFID.instr = 0;
    sp->IFID.PCPlus4 = 0;
    sp->IDEX.instr = 0;
    sp->IDEX.PCPlus4 = 0;
    sp->IDEX.branchTarget = 0;
    sp->IDEX.readData1 = 0;
    sp->IDEX.readData2 = 0;
    sp->IDEX.immed = 0;
    sp->IDEX.rsReg = 0;
    sp->IDEX.rtReg = 0;
    sp->IDEX.rdReg = 0;
    sp->EXMEM.instr = 0;
    sp->EXMEM.ALUoutput = 0;
    sp->EXMEM.rtRegData = 0;
    sp->EXMEM.writeRegNum = 0;
    sp->MEMWB.instr = 0;
    sp->MEMWB.memOutput = 0;
    sp->MEMWB.ALUoutput = 0;
    sp->MEMWB.writeRegNum = 0;
}

/***********************************************************
 * You do not need to modify the functions below. They are provided for your
 * convenience. The printState function accepts a pointer to a state as an
 * argument and prints the formatted contents of pipeline register.
 *************************************************************/
void printState (mipsState_t * sp)
{
    int	  i;
    printf ("============== State before Cycle %d ==============\n", sp->cycles + 1);
    printf ("PC = %d\n", sp->PC);
    printf ("IF/ID : ");
    printInstruction (sp->IFID.instr);
    printf ("\tNPC: %d\n", sp->IFID.PCPlus4);

    printf ("ID/EX : ");
    printInstruction (sp->IDEX.instr);
    printf ("\tNPC: %d ", sp->IDEX.PCPlus4);
    printf ("\tData1: %d ", sp->IDEX.readData1);
    printf ("Data2: %d  ", sp->IDEX.readData2);
    printf ("rs: %d ", sp->IDEX.rsReg);
    printf ("rt: %d ", sp->IDEX.rtReg);
    printf ("rd: %d  ", sp->IDEX.rdReg);
    printf ("imm: %d ", sp->IDEX.immed);
    printf ("Target: %d\n", sp->IDEX.branchTarget);

    printf ("EX/MEM: ");
    printInstruction (sp->EXMEM.instr);
    printf ("\tALUoutput: %d", sp->EXMEM.ALUoutput);
    printf ("\trtRegData: %d", sp->EXMEM.rtRegData);
    printf ("\twriteRegNum: %d\n", sp->EXMEM.writeRegNum);

    printf ("MEM/WB: ");
    printInstruction (sp->MEMWB.instr);
    printf ("\tALUoutput: %d", sp->MEMWB.ALUoutput);
    printf ("\tmemOutput: %d", sp->MEMWB.memOutput);
    printf ("\twriteRegNum: %d\n", sp->MEMWB.writeRegNum);

    printf ("Mem[0-7]: ");
    for (i = 0; i < NUMMEMS; i++) {
	if (i == (NUMMEMS / 2))
	    printf ("\nMem[8-F]: ");
	printf ("0x%-8X ", sp->dataMem[i]);
    }
    printf ("\n");

    printf ("Reg[0-7]: ");
    for (i = 0; i < NUMREGS; i++) {
	printf ("0x%-8X ", sp->regFile[i]);
    }
    printf ("\n\n");
}

/*************************************************************/
/* The instrToInt function converts an instruction from the  */
/* assembly file into an unsigned integer representation.    */
/* For example, consider the add $0,$1,$2 instruction.       */
/* In binary, this instruction is:                           */
/* 000000 00001 00010 00000 00000 100000                     */
/* The unsigned representation in decimal is therefore:      */
/* 2228256                                                   */
/*************************************************************/
unsigned int
instrToInt (char *inst, char *args)
{
    int	  opcode, rs, rt, rd, shamt, funct, immed, newval;
    unsigned int dec_inst, tmp_inst;
    if ((strcmp (inst, "add") == 0) || (strcmp (inst, "sub") == 0)) {
	opcode = 0;
	if (strcmp (inst, "add") == 0)
	    funct = ADD;
	else
	    funct = SUB;
	shamt = 0;
	rd = atoi (strtok (args, ",$"));
	rs = atoi (strtok (NULL, ",$"));
	rt = atoi (strtok (NULL, ",$"));
	dec_inst = (opcode << 26) + (rs << 21) + (rt << 16) + (rd << 11) + (shamt << 6) + funct;
    } else if ((strcmp (inst, "lw") == 0) || (strcmp (inst, "sw") == 0)) {
	if (strcmp (inst, "lw") == 0)
	    opcode = LW;
	else
	    opcode = SW;
	rt = atoi (strtok (args, ",$"));
	newval = immed = atoi (strtok (NULL, ",("));
	rs = atoi (strtok (NULL, "($)"));
	if (immed < 0)
	    newval = 65536 + immed;
	dec_inst = (opcode << 26) + (rs << 21) + (rt << 16) + newval;
    } else if (strcmp (inst, "beq") == 0) {
	opcode = 4;
	rs = atoi (strtok (args, ",$"));
	rt = atoi (strtok (NULL, ",$"));
	newval = immed = atoi (strtok (NULL, ","));
	if (immed < 0)
	    newval = 65536 + immed;
	dec_inst = (opcode << 26) + (rs << 21) + (rt << 16) + newval;
    } else if (strcmp (inst, "halt") == 0) {
	opcode = 63;
	dec_inst = (opcode << 26);
    } else if (strcmp (inst, "noop") == 0) {
	dec_inst = 0;
    }

    return dec_inst;
}

/*************************************************/
/* The printInstruction decodes an unsigned      */
/* integer representation of an instruction      */
/* into its string representation and prints     */
/* the result to stdout.                         */
/*************************************************/
void printInstruction (unsigned int instr)
{
    char  opcmd[10];
    if (instr == 0) {
	printf ("NOOP");
    } else if (get_opc (instr) == AL) {
	if (get_funct (instr) != 0) {
	    if (get_funct (instr) == ADD)
		strcpy (opcmd, "add");
	    else
		strcpy (opcmd, "sub");
	    printf ("%s $%d,$%d,$%d", opcmd, get_rd (instr), get_rs (instr), get_rt (instr));
	} else {
	    printf ("NOOP");
	}
    } else if (get_opc (instr) == LW) {
	printf ("%s $%d,%d($%d)", "lw", get_rt (instr), get_immed (instr), get_rs (instr));
    } else if (get_opc (instr) == SW) {
	printf ("%s $%d,%d($%d)", "sw", get_rt (instr), get_immed (instr), get_rs (instr));
    } else if (get_opc (instr) == BEQ) {
	printf ("%s $%d,$%d,%d", "beq", get_rs (instr), get_rt (instr), get_immed (instr));
    } else if (get_opc (instr) == HALT) {
	printf ("%s", "halt");
    }
}
